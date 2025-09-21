#include "Socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// ===== FONCTION SERVERSOCKET =====
// Inspirée du cours page 48-49
int ServerSocket(int port) {
    int sEcoute;
    
    // 1. Création de la socket
    if ((sEcoute = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Erreur de socket()");
        return -1;
    }
    
    // 2. Option pour réutiliser l'adresse (éviter "Address already in use")
    int option = 1;
    setsockopt(sEcoute, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));
    
    // 3. Construction de l'adresse avec getaddrinfo()
    struct addrinfo hints, *results;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;  // Pour serveur
    
    char portStr[10];
    sprintf(portStr, "%d", port);
    
    if (getaddrinfo(NULL, portStr, &hints, &results) != 0) {
        close(sEcoute);
        return -1;
    }
    
    // 4. Liaison avec bind()
    if (bind(sEcoute, results->ai_addr, results->ai_addrlen) < 0) {
        perror("Erreur de bind()");
        freeaddrinfo(results);
        close(sEcoute);
        return -1;
    }
    freeaddrinfo(results);
    
    // 5. Mise en écoute avec listen()
    if (listen(sEcoute, SOMAXCONN) == -1) {
        perror("Erreur de listen()");
        close(sEcoute);
        return -1;
    }
    
    return sEcoute;
}

// ===== FONCTION ACCEPT =====
// Inspirée du cours page 50-52
int Accept(int sEcoute, char *ipClient) {
    int sService;
    struct sockaddr_in adrClient;
    socklen_t adrClientLen = sizeof(struct sockaddr_in);
    
    // Accept bloquant jusqu'à connexion d'un client
    if ((sService = accept(sEcoute, (struct sockaddr*)&adrClient, &adrClientLen)) == -1) {
        perror("Erreur de accept()");
        return -1;
    }
    
    // Si on veut récupérer l'IP du client
    if (ipClient != NULL) {
        char host[NI_MAXHOST];
        char port[NI_MAXSERV];
        getnameinfo((struct sockaddr*)&adrClient, adrClientLen,
                   host, NI_MAXHOST, port, NI_MAXSERV,
                   NI_NUMERICHOST | NI_NUMERICSERV);
        strcpy(ipClient, host);
    }
    
    return sService;
}

// ===== FONCTION CLIENTSOCKET =====
int ClientSocket(char* ipServeur, int portServeur) {
    int sClient;
    
    // 1. Création de la socket
    if ((sClient = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Erreur de socket()");
        return -1;
    }
    
    // 2. Construction de l'adresse du serveur
    struct addrinfo hints, *results;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    
    char portStr[10];
    sprintf(portStr, "%d", portServeur);
    
    if (getaddrinfo(ipServeur, portStr, &hints, &results) != 0) {
        close(sClient);
        return -1;
    }
    
    // 3. Connexion au serveur
    if (connect(sClient, results->ai_addr, results->ai_addrlen) == -1) {
        perror("Erreur de connect()");
        freeaddrinfo(results);
        close(sClient);
        return -1;
    }
    freeaddrinfo(results);
    
    return sClient;
}

// ===== FONCTION SEND =====
// Inspirée du cours page 45-46 (stratégie avec entête de taille)
int Send(int sSocket, char* data, int taille) {
    // 1. Envoi d'abord la taille des données (entête de 4 bytes)
    char entete[5];
    sprintf(entete, "%04d", taille);  // Format sur 4 caractères
    
    if (write(sSocket, entete, 4) != 4) {
        perror("Erreur envoi entête");
        return -1;
    }
    
    // 2. Envoi des données
    int totalEnvoye = 0;
    while (totalEnvoye < taille) {
        int nbEnvoye = write(sSocket, data + totalEnvoye, taille - totalEnvoye);
        if (nbEnvoye <= 0) {
            perror("Erreur envoi données");
            return -1;
        }
        totalEnvoye += nbEnvoye;
    }
    
    return totalEnvoye;
}

// ===== FONCTION RECEIVE =====
// Inspirée du cours page 45-46 (lecture entête puis données)
int Receive(int sSocket, char* data) {
    // 1. Lecture de l'entête (taille des données)
    char entete[5];
    entete[4] = 0;  // Fin de chaîne
    
    int nbLu = 0;
    while (nbLu < 4) {
        int n = read(sSocket, entete + nbLu, 4 - nbLu);
        if (n <= 0) {
            if (n == 0) return 0;  // Connexion fermée
            perror("Erreur lecture entête");
            return -1;
        }
        nbLu += n;
    }
    
    // 2. Conversion de l'entête en taille
    int taille = atoi(entete);
    if (taille > TAILLE_MAX_DATA) {
        printf("Erreur: taille trop grande (%d)\n", taille);
        return -1;
    }
    
    // 3. Lecture des données
    int totalLu = 0;
    while (totalLu < taille) {
        int nbLu = read(sSocket, data + totalLu, taille - totalLu);
        if (nbLu <= 0) {
            perror("Erreur lecture données");
            return -1;
        }
        totalLu += nbLu;
    }
    
    return totalLu;
}