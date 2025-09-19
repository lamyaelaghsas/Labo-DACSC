#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../SocketLib/Socket.h"

void testProtocoleCBP(int sClient) {
    char requete[1000], reponse[1000];
    int nbEcrits, nbLus;
    
    printf("=== TEST DU PROTOCOLE CBP ===\n");
    
    // 1. Test LOGIN
    printf("\n1. Test LOGIN...\n");
    strcpy(requete, "LOGIN#Durand#Jean#123#");
    Send(sClient, requete, strlen(requete));
    nbLus = Receive(sClient, reponse);
    reponse[nbLus] = 0;
    printf("   Requête: %s\n", requete);
    printf("   Réponse: %s\n", reponse);
    
    // 2. Test GET_SPECIALTIES
    printf("\n2. Test GET_SPECIALTIES...\n");
    strcpy(requete, "GET_SPECIALTIES");
    Send(sClient, requete, strlen(requete));
    nbLus = Receive(sClient, reponse);
    reponse[nbLus] = 0;
    printf("   Requête: %s\n", requete);
    printf("   Réponse: %s\n", reponse);
    
    // 3. Test GET_DOCTORS
    printf("\n3. Test GET_DOCTORS...\n");
    strcpy(requete, "GET_DOCTORS");
    Send(sClient, requete, strlen(requete));
    nbLus = Receive(sClient, reponse);
    reponse[nbLus] = 0;
    printf("   Requête: %s\n", requete);
    printf("   Réponse: %s\n", reponse);
    
    // 4. Test SEARCH_CONSULTATIONS
    printf("\n4. Test SEARCH_CONSULTATIONS...\n");
    strcpy(requete, "SEARCH_CONSULTATIONS#TOUTES#TOUS#2025-10-01#2025-10-31");
    Send(sClient, requete, strlen(requete));
    nbLus = Receive(sClient, reponse);
    reponse[nbLus] = 0;
    printf("   Requête: %s\n", requete);
    printf("   Réponse: %s\n", reponse);
    
    // 5. Test BOOK_CONSULTATION
    printf("\n5. Test BOOK_CONSULTATION...\n");
    strcpy(requete, "BOOK_CONSULTATION#1#Check-up general");
    Send(sClient, requete, strlen(requete));
    nbLus = Receive(sClient, reponse);
    reponse[nbLus] = 0;
    printf("   Requête: %s\n", requete);
    printf("   Réponse: %s\n", reponse);
    
    // 6. Test LOGOUT
    printf("\n6. Test LOGOUT...\n");
    strcpy(requete, "LOGOUT");
    Send(sClient, requete, strlen(requete));
    nbLus = Receive(sClient, reponse);
    reponse[nbLus] = 0;
    printf("   Requête: %s\n", requete);
    printf("   Réponse: %s\n", reponse);
    
    printf("\n=== FIN TEST CBP ===\n");
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <ip_serveur> <port>\n", argv[0]);
        printf("Exemple: %s 127.0.0.1 50000\n", argv[0]);
        return 1;
    }
    
    // Connexion au serveur
    printf("Connexion au serveur %s:%s...\n", argv[1], argv[2]);
    int sClient = ClientSocket(argv[1], atoi(argv[2]));
    if (sClient == -1) {
        printf("Erreur: Impossible de se connecter au serveur\n");
        return 1;
    }
    
    printf("✓ Connecté au serveur!\n");
    
    // Test du protocole CBP
    testProtocoleCBP(sClient);
    
    // Fermeture
    close(sClient);
    printf("Connexion fermée.\n");
    
    return 0;
}