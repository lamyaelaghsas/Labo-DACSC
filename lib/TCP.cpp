#include "TCP.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Crée une socket serveur (tuyau de communication réseau) et la met en écoute sur le port spécifié
 * Style : socket() + bind() + listen()
 */
int ServerSocket(int port) {
    int sEcoute;
    
    // 1. Création de la socket
    if ((sEcoute = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return -1;
    }
    
    // 2. Option SO_REUSEADDR (pour éviter "Address already in use")
    int opt = 1;
    if (setsockopt(sEcoute, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1) { //Dit au système "si le port était occupé, libère-le immédiatement"
        close(sEcoute);
        return -1;
    }
    
    // 3. Construction de l'adresse réseau 
    struct addrinfo hints;
    struct addrinfo *results;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV; // pour connexion passive
    
    char port_str[10];
    sprintf(port_str, "%d", port);
    
    if (getaddrinfo(NULL, port_str, &hints, &results) != 0) { //prépare une structure d'adresse réseau avec le port demandé
        close(sEcoute);
        return -1;
    }
    
    // 4. Bind (liaison socket <-> adresse)
    if (bind(sEcoute, results->ai_addr, results->ai_addrlen) == -1) { //attache la socket au port spécifié
        freeaddrinfo(results);
        close(sEcoute);
        return -1;
    }
    
    freeaddrinfo(results);
    
    // 5. Listen 
    if (listen(sEcoute, SOMAXCONN) == -1) { //met la socket en mode écoute (elle peut mtn recevoir des connexions)
        close(sEcoute);
        return -1;
    }
    
    return sEcoute;
}

/**
 * Accepte une connexion sur une socket d'écoute
 * Style : accept() + getpeername() optionnel
 */
int Accept(int sEcoute, char *ipClient) { 
    int sService;
    
    // Accept de la connexion
    if ((sService = accept(sEcoute, NULL, NULL)) == -1) { //bloque en attendant qu'un client se connecte. qd ca arrive, retourne une nvl socket pour parler avec ce client
        return -1;
    }
    
    // Récupération IP client si demandée 
    if (ipClient != NULL) {
        struct sockaddr_in adrClient;
        socklen_t adrClientLen = sizeof(struct sockaddr_in);
        
        if (getpeername(sService, (struct sockaddr*)&adrClient, &adrClientLen) == 0) { //recupere l'adresse reseau du client qui vient de se connecter
            char host[NI_MAXHOST];
            char port[NI_MAXSERV];
            
            if (getnameinfo((struct sockaddr*)&adrClient, adrClientLen,
                           host, NI_MAXHOST, port, NI_MAXSERV,
                           NI_NUMERICHOST | NI_NUMERICSERV) == 0) {  //convertit l'adresse réseau en string IP lisible (ex : "192.168.1.100")
                strcpy(ipClient, host);
            } else {
                strcpy(ipClient, "Unknown");
            }
        } else {
            strcpy(ipClient, "Unknown");
        }
    }
    
    return sService; //Résultat : Une socket de service pour communiquer avec 1 client spécifique
}

/**
 * Crée une socket client et se connecte au serveur
 * Style : socket() + connect()
 */
int ClientSocket(char* ipServeur, int portServeur) {
    int sClient;
    
    // 1. Création de la socket
    if ((sClient = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return -1;
    }
    
    // 2. Construction de l'adresse du serveur 
    struct addrinfo hints;
    struct addrinfo *results;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    
    char port_str[10];
    sprintf(port_str, "%d", portServeur);
    
    if (getaddrinfo(ipServeur, port_str, &hints, &results) != 0) { //Prépare l'adresse du serveur (IP + port)
        close(sClient);
        return -1;
    }
    
    // 3. Connexion au serveur
    if (connect(sClient, results->ai_addr, results->ai_addrlen) == -1) { //connect() = Établit la connexion TCP avec le serveur
        freeaddrinfo(results);
        close(sClient);
        return -1;
    }
    
    freeaddrinfo(results);
    return sClient;
}

/**
 * Envoie des données sur une socket avec entête de taille
 * Le problème : TCP est un "flux de bytes". Si tu envoies "Hello" puis "World", le destinataire peut recevoir "Hel" puis "loWorld".
 * Style : stratégie entête + données
 */
int Send(int sSocket, char* data, int taille) {
    // 1. Envoi de l'entête (4 bytes pour la taille)
    char entete[5];
    sprintf(entete, "%04d", taille); //Crée un entête avec la taille (ex: "0005" pour 5 bytes)
    
    if (send(sSocket, entete, 4, 0) != 4) { //Envoie d'abord les 4 bytes de l'entête
        return -1;
    }
    
    // 2. Envoi des données
    int nbEnvoyes = send(sSocket, data, taille, 0); //Envoie ensuite les données réelles
    return nbEnvoyes; //Résultat : Le destinataire sait exactement combien de bytes lire
}

/**
 * Reçoit des données sur une socket (lit l'entête puis les données)
 * Style : lecture entête puis données
 */
int Receive(int sSocket, char* data) {
    // 1. Lecture de l'entête (4 bytes)
    char entete[5];
    int nbLus = recv(sSocket, entete, 4, 0); //Lit exactement 4 bytes (l'entête de taille)
    
    if (nbLus <= 0) {
        return nbLus; // Erreur ou connexion fermée
    }
    
    if (nbLus != 4) {
        return -1; // Entête incomplète
    }
    
    entete[4] = '\0';
    int taille = atoi(entete); //Convertit l'entête en nombre (ex: "0025" → 25)
    
    // Vérification taille raisonnable
    if (taille < 0 || taille > TAILLE_MAX_DATA) {
        return -1;
    }
    
    // 2. Lecture des données
    nbLus = recv(sSocket, data, taille, 0); //Lit exactement le nombre de bytes annoncé
    return nbLus;
}