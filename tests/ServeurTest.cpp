#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "../lib/TCP.h"

int sEcoute = -1; //socket qui écoute les nvl connexions
int sService = -1; //socket utilisée pr "parler" avec un client spécifique

void HandlerSIGINT(int s) {
    printf("\nArret du serveur de test...\n");
    if (sService != -1) close(sService);
    if (sEcoute != -1) close(sEcoute);
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        printf("Exemple: %s 50000\n", argv[0]);
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
    
    int port = atoi(argv[1]);
    printf("=== TEST SERVEUR TCP ===\n");
    printf("Demarrage sur le port %d...\n", port);
    
    // 1. Creer socket serveur
    sEcoute = ServerSocket(port);
    if (sEcoute == -1) {
        perror("Erreur ServerSocket");
        exit(1);
    }
    printf("ServerSocket OK (socket=%d)\n", sEcoute);
    
    while (1) {
        printf("\n--- Attente d'un client ---\n");
        
        // 2. Accepter une connexion
        char ipClient[50];
        sService = Accept(sEcoute, ipClient);
        if (sService == -1) {
            perror("Erreur Accept");
            continue;
        }
        printf("Client connecte : IP=%s, socket=%d\n", ipClient, sService);
        
        // 3. Recevoir et renvoyer les messages (echo)
        char buffer[TAILLE_MAX_DATA];
        int nbLus;
        
        while ((nbLus = Receive(sService, buffer)) > 0) {
            buffer[nbLus] = '\0'; // Ajouter \0 pour affichage
            printf("Recu (%d bytes): %s\n", nbLus, buffer);
            
            // Creer la reponse echo
            char reponse[TAILLE_MAX_DATA];
            sprintf(reponse, "[ECHO] %s", buffer);
            
            int nbEnvoyes = Send(sService, reponse, strlen(reponse));
            if (nbEnvoyes == -1) {
                perror("Erreur Send");
                break;
            }
            printf("Envoye (%d bytes): %s\n", nbEnvoyes, reponse);
        }
        
        if (nbLus == 0) {
            printf("Client deconnecte proprement\n");
        } else {
            printf("Erreur de reception\n");
        }
        
        close(sService);
        sService = -1;
    }
    
    return 0;
}