#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "../lib/TCP.h"

int sClient = -1; //UNE seule socket coté client pcq le client se contente juste de se connecter a un serveur

void HandlerSIGINT(int s) {
    printf("\nArret du client de test...\n");
    if (sClient != -1) 
        close(sClient);
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <ip_serveur> <port>\n", argv[0]);
        printf("Exemple: %s 127.0.0.1 50000\n", argv[0]);
        printf("Exemple: %s 192.168.1.100 50000\n", argv[0]);
        exit(1);
    }
    
    // Armement des signaux
    struct sigaction A;
    A.sa_flags = 0;
    sigemptyset(&A.sa_mask);
    A.sa_handler = HandlerSIGINT;
    if (sigaction(SIGINT, &A, NULL) == -1) {
        perror("Erreur de sigaction");
        exit(1);
    }
    
    char* ipServeur = argv[1];
    int port = atoi(argv[2]);
    
    printf("=== TEST CLIENT TCP ===\n");
    printf("Connexion a %s:%d...\n", ipServeur, port);
    
    // 1. Se connecter au serveur
    sClient = ClientSocket(ipServeur, port);
    if (sClient == -1) {
        perror("Erreur ClientSocket");
        exit(1);
    }
    printf("Connecte au serveur (socket=%d)\n", sClient);
    
    // 2. Tests d'envoi/reception
    char message[TAILLE_MAX_DATA];
    char reponse[TAILLE_MAX_DATA];
    
    printf("\n--- Tests automatiques ---\n");
    
    // Test 1: Message simple
    strcpy(message, "Hello serveur!");
    printf("Envoi: %s\n", message);
    
    int nbEnvoyes = Send(sClient, message, strlen(message));
    if (nbEnvoyes == -1) {
        perror("Erreur Send");
        exit(1);
    }
    
    int nbLus = Receive(sClient, reponse);
    if (nbLus == -1) {
        perror("Erreur Receive");
        exit(1);
    }
    reponse[nbLus] = '\0';
    printf("Recu: %s\n", reponse);
    
    // Test 2: Message protocole style
    strcpy(message, "LOGIN#wagner#abc123");
    printf("\nEnvoi: %s\n", message);
    
    nbEnvoyes = Send(sClient, message, strlen(message));
    if (nbEnvoyes == -1) {
        perror("Erreur Send");
        exit(1);
    }
    
    nbLus = Receive(sClient, reponse);
    if (nbLus == -1) {
        perror("Erreur Receive");
        exit(1);
    }
    reponse[nbLus] = '\0';
    printf("Recu: %s\n", reponse);
    
    // Test 3: Mode interactif
    printf("\n--- Mode interactif (Ctrl+C pour quitter) ---\n");
    
    while (1) {
        printf("\nMessage a envoyer: ");
        fflush(stdout);
        
        if (fgets(message, sizeof(message), stdin) == NULL) {
            break;
        }
        
        // Enlever le \n de fin
        int len = strlen(message);
        if (len > 0 && message[len-1] == '\n') {
            message[len-1] = '\0';
            len--;
        }
        
        if (len == 0) continue;
        
        // Envoyer
        nbEnvoyes = Send(sClient, message, len);
        if (nbEnvoyes == -1) {
            perror("Erreur Send");
            break;
        }
        
        // Recevoir
        nbLus = Receive(sClient, reponse);
        if (nbLus == -1) {
            perror("Erreur Receive");
            break;
        }
        if (nbLus == 0) {
            printf("Serveur deconnecte\n");
            break;
        }
        
        reponse[nbLus] = '\0';
        printf("Reponse: %s\n", reponse);
    }
    
    close(sClient);
    printf("Deconnexion OK\n");
    return 0;
}