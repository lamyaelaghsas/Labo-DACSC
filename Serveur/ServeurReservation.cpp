#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include "../lib/TCP.h"
#include "CBP.h"

// Configuration du serveur
int PORT_RESERVATION = 50000;
int NB_THREADS_POOL = 3;
#define TAILLE_FILE_ATTENTE 20

// Socket d'ecoute globale
int sEcoute = -1;

// Pool de threads - Variables globales
int socketsAcceptees[TAILLE_FILE_ATTENTE];
int indiceEcriture = 0;
int indiceLecture = 0;
pthread_mutex_t mutexSocketsAcceptees;
pthread_cond_t condSocketsAcceptees;

// Prototypes
void HandlerSIGINT(int s);
void* FctThreadClient(void* p);
void TraitementConnexion(int sService);
bool LireConfiguration(const char* fichier);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <fichier_config>\n", argv[0]);
        printf("Exemple: %s config.txt\n", argv[0]);
        exit(1);
    }
    
    printf("=== SERVEUR RESERVATION HOSPITALIER ===\n");
    
    // Lecture de la configuration
    if (!LireConfiguration(argv[1])) {
        printf("Erreur lecture configuration\n");
        exit(1);
    }
    printf("Configuration: PORT=%d, THREADS=%d\n", PORT_RESERVATION, NB_THREADS_POOL);
    
    // Initialisation de la base de donnees
    if (!CBP_InitDB()) {
        printf("Erreur connexion base de donnees\n");
        exit(1);
    }
    printf("Connexion base de donnees OK\n");
    
    // Armement des signaux (style prof)
    struct sigaction A;
    A.sa_flags = 0;
    sigemptyset(&A.sa_mask);
    A.sa_handler = HandlerSIGINT;
    if (sigaction(SIGINT, &A, NULL) == -1) {
        perror("Erreur de sigaction");
        exit(1);
    }
    
    // Initialisation du pool de threads
    pthread_mutex_init(&mutexSocketsAcceptees, NULL);
    pthread_cond_init(&condSocketsAcceptees, NULL);
    for (int i = 0; i < TAILLE_FILE_ATTENTE; i++) {
        socketsAcceptees[i] = -1;
    }
    
    // Creation de la socket d'ecoute
    sEcoute = ServerSocket(PORT_RESERVATION);
    if (sEcoute == -1) {
        perror("Erreur ServerSocket");
        CBP_CloseDB();
        exit(1);
    }
    printf("Socket d'ecoute creee sur le port %d\n", PORT_RESERVATION);
    
    // Creation du pool de threads
    printf("Creation du pool de %d threads...\n", NB_THREADS_POOL);
    pthread_t th;
    for (int i = 0; i < NB_THREADS_POOL; i++) {
        if (pthread_create(&th, NULL, FctThreadClient, NULL) != 0) {
            perror("Erreur pthread_create");
            exit(1);
        }
    }
    
    // Boucle principale du serveur
    printf("Serveur pret - Attente des clients...\n");
    
    while (1) {
        char ipClient[50];
        int sService = Accept(sEcoute, ipClient);
        if (sService == -1) {
            perror("Erreur Accept");
            continue;
        }
        
        printf("Nouvelle connexion: IP=%s, socket=%d\n", ipClient, sService);
        
        // Insertion dans la file d'attente (production d'une tache)
        pthread_mutex_lock(&mutexSocketsAcceptees);
        
        // Verifier si la file n'est pas pleine
        int next_index = (indiceEcriture + 1) % TAILLE_FILE_ATTENTE;
        if (next_index == indiceLecture) {
            printf("File d'attente pleine, connexion refusee\n");
            close(sService);
        } else {
            socketsAcceptees[indiceEcriture] = sService;
            indiceEcriture = next_index;
            pthread_cond_signal(&condSocketsAcceptees);
        }
        
        pthread_mutex_unlock(&mutexSocketsAcceptees);
    }
    
    return 0;
}

/**
 * Fonction executee par chaque thread du pool
 */
void* FctThreadClient(void* p) {
    int sService;
    
    while (1) {
        // Attente d'une tache (consommation)
        pthread_mutex_lock(&mutexSocketsAcceptees);
        
        while (indiceEcriture == indiceLecture) {
            pthread_cond_wait(&condSocketsAcceptees, &mutexSocketsAcceptees);
        }
        
        sService = socketsAcceptees[indiceLecture];
        socketsAcceptees[indiceLecture] = -1;
        indiceLecture = (indiceLecture + 1) % TAILLE_FILE_ATTENTE;
        
        pthread_mutex_unlock(&mutexSocketsAcceptees);
        
        // Traitement de la connexion
        printf("[THREAD %p] Prise en charge socket %d\n", pthread_self(), sService);
        TraitementConnexion(sService);
        printf("[THREAD %p] Fin traitement socket %d\n", pthread_self(), sService);
    }
    
    return NULL;
}

/**
 * Traite une connexion client complete (session CBP)
 */
void TraitementConnexion(int sService) {
    char requete[TAILLE_MAX_DATA];
    char reponse[TAILLE_MAX_REPONSE];
    int nbLus, nbEcrits;
    bool continuer = true;
    
    while (continuer) {
        // Reception de la requete
        nbLus = Receive(sService, requete);
        if (nbLus <= 0) {
            printf("[THREAD %p] Client deconnecte (socket %d)\n", pthread_self(), sService);
            break;
        }
        
        requete[nbLus] = '\0';
        printf("[THREAD %p] Requete: %s\n", pthread_self(), requete);
        
        // Traitement via le protocole CBP
        continuer = CBP(requete, reponse, sService);
        
        // Envoi de la reponse
        nbEcrits = Send(sService, reponse, strlen(reponse));
        if (nbEcrits == -1) {
            perror("Erreur Send");
            break;
        }
        
        printf("[THREAD %p] Reponse: %s\n", pthread_self(), reponse);
        
        // Si LOGOUT ou erreur fatale, terminer la connexion
        if (!continuer) {
            printf("[THREAD %p] Fin de session demandee\n", pthread_self());
            break;
        }
    }
    
    // Nettoyer la session
    CBP_SetLoggedOut(sService);
    close(sService);
}

/**
 * Gestionnaire de signal pour arret propre
 */
void HandlerSIGINT(int s) {
    printf("\nArret du serveur en cours...\n");
    
    // Fermer la socket d'ecoute
    if (sEcoute != -1) {
        close(sEcoute);
    }
    
    // Fermer toutes les sockets en attente
    pthread_mutex_lock(&mutexSocketsAcceptees);
    for (int i = 0; i < TAILLE_FILE_ATTENTE; i++) {
        if (socketsAcceptees[i] != -1) {
            close(socketsAcceptees[i]);
        }
    }
    pthread_mutex_unlock(&mutexSocketsAcceptees);
    
    // Fermer la base de donnees
    CBP_CloseDB();
    
    printf("Serveur arrete\n");
    exit(0);
}

/**
 * Lit la configuration depuis un fichier
 */
bool LireConfiguration(const char* fichier) {
    FILE* f = fopen(fichier, "r");
    if (!f) {
        return false;
    }
    
    char ligne[100];
    while (fgets(ligne, sizeof(ligne), f)) {
        // Enlever le \n de fin
        ligne[strcspn(ligne, "\n")] = '\0';
        
        // Ignorer les commentaires et lignes vides
        if (ligne[0] == '#' || ligne[0] == '\0') {
            continue;
        }
        
        // Parser PORT=valeur
        if (strncmp(ligne, "PORT=", 5) == 0) {
            PORT_RESERVATION = atoi(ligne + 5);
        }
        // Parser THREADS=valeur
        else if (strncmp(ligne, "THREADS=", 8) == 0) {
            NB_THREADS_POOL = atoi(ligne + 8);
        }
    }
    
    fclose(f);
    
    // Verification des valeurs
    if (PORT_RESERVATION <= 0 || PORT_RESERVATION > 65535) {
        PORT_RESERVATION = 50000;
    }
    if (NB_THREADS_POOL <= 0 || NB_THREADS_POOL > 20) {
        NB_THREADS_POOL = 3;
    }
    
    return true;
}