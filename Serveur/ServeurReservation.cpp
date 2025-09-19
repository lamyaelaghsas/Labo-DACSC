// ServeurReservation.cpp - Inspiré du cours pages 80-87 (serveur pool de threads)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <fstream>
#include "../SocketLib/Socket.h"
#include "CBP.h"

using namespace std;

// Variables globales
int sEcoute;
int portReservation = 50000;
int nbThreadsPool = 3;

// Gestion du pool de threads (inspiré cours page 80-84)
#define TAILLE_FILE_ATTENTE 20
int socketsAcceptees[TAILLE_FILE_ATTENTE];
int indiceEcriture = 0, indiceLecture = 0;
pthread_mutex_t mutexSocketsAcceptees;
pthread_cond_t condSocketsAcceptees;

// Prototypes des fonctions
void HandlerSIGINT(int s);
void TraitementConnexion(int sService);
void* FctThreadClient(void* p);
bool LireConfiguration();

int main(int argc, char* argv[]) {
    printf("=== SERVEUR RESERVATION HOPITAL ===\n");
    
    // 1. Lecture du fichier de configuration
    if (!LireConfiguration()) {
        printf("Utilisation des valeurs par défaut: PORT=%d, THREADS=%d\n", 
               portReservation, nbThreadsPool);
    } else {
        printf("Configuration: PORT=%d, THREADS=%d\n", portReservation, nbThreadsPool);
    }
    
    // 2. Initialisation des structures pour le pool de threads
    pthread_mutex_init(&mutexSocketsAcceptees, NULL);
    pthread_cond_init(&condSocketsAcceptees, NULL);
    for (int i = 0; i < TAILLE_FILE_ATTENTE; i++) {
        socketsAcceptees[i] = -1;
    }
    
    // 3. Armement du signal SIGINT
    struct sigaction A;
    A.sa_flags = 0;
    sigemptyset(&A.sa_mask);
    A.sa_handler = HandlerSIGINT;
    if (sigaction(SIGINT, &A, NULL) == -1) {
        perror("Erreur de sigaction");
        exit(1);
    }
    
    // 4. Création de la socket d'écoute
    printf("Création socket serveur sur port %d...\n", portReservation);
    if ((sEcoute = ServerSocket(portReservation)) == -1) {
        perror("Erreur de ServerSocket");
        exit(1);
    }
    printf("✓ Socket serveur créée\n");
    
    // 5. Création du pool de threads (inspiré cours page 81)
    printf("Création du pool de %d threads...\n", nbThreadsPool);
    pthread_t th;
    for (int i = 0; i < nbThreadsPool; i++) {
        pthread_create(&th, NULL, FctThreadClient, NULL);
    }
    printf("✓ Pool de threads créé\n");
    
    // 6. Boucle principale du serveur (thread producteur)
    printf("Serveur en attente de connexions...\n");
    while (1) {
        char ipClient[50];
        
        // Accept bloquant
        int sService = Accept(sEcoute, ipClient);
        if (sService == -1) {
            perror("Erreur de Accept");
            continue;
        }
        
        printf("✓ Nouvelle connexion acceptée: IP=%s, socket=%d\n", ipClient, sService);
        
        // Ajout du client à la gestion CBP
        CBP_AjouterClient(sService, string(ipClient));
        
        // Insertion en file d'attente (production d'une tâche)
        pthread_mutex_lock(&mutexSocketsAcceptees);
        socketsAcceptees[indiceEcriture] = sService;
        indiceEcriture++;
        if (indiceEcriture == TAILLE_FILE_ATTENTE) indiceEcriture = 0;
        pthread_mutex_unlock(&mutexSocketsAcceptees);
        
        // Réveil d'un thread du pool
        pthread_cond_signal(&condSocketsAcceptees);
    }
    
    return 0;
}

// ===== FONCTION THREAD CLIENT (CONSOMMATEUR) =====
// Inspirée du cours page 82-83
void* FctThreadClient(void* p) {
    int sService;
    
    while (1) {
        printf("\t[THREAD %p] Attente d'une tâche...\n", pthread_self());
        
        // Attente d'une tâche (consommation)
        pthread_mutex_lock(&mutexSocketsAcceptees);
        while (indiceEcriture == indiceLecture) {
            pthread_cond_wait(&condSocketsAcceptees, &mutexSocketsAcceptees);
        }
        sService = socketsAcceptees[indiceLecture];
        socketsAcceptees[indiceLecture] = -1;
        indiceLecture++;
        if (indiceLecture == TAILLE_FILE_ATTENTE) indiceLecture = 0;
        pthread_mutex_unlock(&mutexSocketsAcceptees);
        
        // Traitement de la connexion
        printf("\t[THREAD %p] Prise en charge socket %d\n", pthread_self(), sService);
        TraitementConnexion(sService);
        printf("\t[THREAD %p] Fin traitement socket %d\n", pthread_self(), sService);
        
        // Retrait du client de la gestion CBP
        CBP_RetirerClient(sService);
    }
    
    return NULL;
}

// ===== TRAITEMENT D'UNE CONNEXION =====
// Inspirée du cours page 68-69
void TraitementConnexion(int sService) {
    char requete[1000], reponse[1000];
    int nbLus, nbEcrits;
    bool onContinue = true;
    
    while (onContinue) {
        printf("\t[THREAD %p] Attente requête...\n", pthread_self());
        
        // Réception requête
        if ((nbLus = Receive(sService, requete)) < 0) {
            perror("Erreur de Receive");
            close(sService);
            return;
        }
        
        // Fin de connexion ?
        if (nbLus == 0) {
            printf("\t[THREAD %p] Fin de connexion du client\n", pthread_self());
            close(sService);
            return;
        }
        
        requete[nbLus] = 0;
        printf("\t[THREAD %p] Requête reçue: %s\n", pthread_self(), requete);
        
        // Traitement via le protocole CBP
        onContinue = CBP_TraiterRequete(requete, reponse, sService);
        
        // Envoi de la réponse
        if ((nbEcrits = Send(sService, reponse, strlen(reponse))) < 0) {
            perror("Erreur de Send");
            close(sService);
            return;
        }
        
        printf("\t[THREAD %p] Réponse envoyée: %s\n", pthread_self(), reponse);
        
        if (!onContinue) {
            printf("\t[THREAD %p] Fin de session demandée\n", pthread_self());
            close(sService);
        }
    }
}

// ===== HANDLER SIGINT =====
void HandlerSIGINT(int s) {
    printf("\n=== ARRÊT DU SERVEUR ===\n");
    
    // Fermeture de la socket d'écoute
    close(sEcoute);
    
    // Fermeture des sockets en attente
    pthread_mutex_lock(&mutexSocketsAcceptees);
    for (int i = 0; i < TAILLE_FILE_ATTENTE; i++) {
        if (socketsAcceptees[i] != -1) {
            close(socketsAcceptees[i]);
        }
    }
    pthread_mutex_unlock(&mutexSocketsAcceptees);
    
    // Nettoyage du protocole CBP
    CBP_Close();
    
    printf("Serveur arrêté proprement.\n");
    exit(0);
}

// ===== LECTURE CONFIGURATION =====
bool LireConfiguration() {
    ifstream fichier("config.txt");
    if (!fichier.is_open()) {
        return false;
    }
    
    string ligne;
    while (getline(fichier, ligne)) {
        if (ligne.find("PORT_RESERVATION=") == 0) {
            portReservation = stoi(ligne.substr(17));
        } else if (ligne.find("NB_THREADS_POOL=") == 0) {
            nbThreadsPool = stoi(ligne.substr(16));
        }
    }
    
    fichier.close();
    return true;
}