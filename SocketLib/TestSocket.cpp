#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "Socket.h"

// Test simple serveur
void testServeur() {
    printf("=== TEST SERVEUR ===\n");
    
    // Création socket serveur sur port 50000
    int sEcoute = ServerSocket(50000);
    if (sEcoute == -1) {
        printf("Erreur création serveur\n");
        return;
    }
    printf("✓ Serveur créé, en attente sur port 50000...\n");
    
    // Attente d'un client
    char ipClient[50];
    int sService = Accept(sEcoute, ipClient);
    if (sService == -1) {
        printf("Erreur accept\n");
        close(sEcoute);
        return;
    }
    printf("✓ Client connecté depuis %s\n", ipClient);
    
    // Réception d'un message
    char buffer[1000];
    int nbRecu = Receive(sService, buffer);
    if (nbRecu > 0) {
        buffer[nbRecu] = 0;  // Fin de chaîne
        printf("✓ Message reçu: %s\n", buffer);
        
        // Réponse
        char reponse[1000];
        sprintf(reponse, "[SERVEUR] %s", buffer);
        Send(sService, reponse, strlen(reponse));
        printf("✓ Réponse envoyée: %s\n", reponse);
    }
    
    close(sService);
    close(sEcoute);
    printf("=== FIN TEST SERVEUR ===\n");
}

// Test simple client
void testClient() {
    printf("=== TEST CLIENT ===\n");
    
    // Connexion au serveur
    int sClient = ClientSocket("127.0.0.1", 50000);
    if (sClient == -1) {
        printf("Erreur connexion serveur\n");
        return;
    }
    printf("✓ Connecté au serveur\n");
    
    // Envoi message
    char message[] = "Hello from client!";
    Send(sClient, message, strlen(message));
    printf("✓ Message envoyé: %s\n", message);
    
    // Réception réponse
    char buffer[1000];
    int nbRecu = Receive(sClient, buffer);
    if (nbRecu > 0) {
        buffer[nbRecu] = 0;
        printf("✓ Réponse reçue: %s\n", buffer);
    }
    
    close(sClient);
    printf("=== FIN TEST CLIENT ===\n");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s [serveur|client]\n", argv[0]);
        return 1;
    }
    
    if (strcmp(argv[1], "serveur") == 0) {
        testServeur();
    } else if (strcmp(argv[1], "client") == 0) {
        // Petit délai pour laisser le serveur démarrer
        sleep(1);
        testClient();
    } else {
        printf("Paramètre invalide. Utilisez 'serveur' ou 'client'\n");
    }
    
    return 0;
}