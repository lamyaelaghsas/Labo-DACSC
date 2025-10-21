#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "TCP.h"


int ServerSocket(int port)
{
    int sEcoute;
    char portStr[10];
    
    sprintf(portStr, "%d", port);
    
    // Creation de la socket
    if ((sEcoute=socket(AF_INET, SOCK_STREAM, 0))== -1)
        return -1;
    
    // utilisation du port sans attendre
    int value = 1;
    setsockopt(sEcoute, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int));

    // Construction de l'adresse
    struct addrinfo hints;
    struct addrinfo *results;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE | AI_NUMERICSERV;
    if (getaddrinfo(NULL, portStr, &hints, &results) != 0)
    {
        close(sEcoute);
        return -1;
    }
    
    // Liaison de la socket à l'adresse
    if (bind(sEcoute, results->ai_addr, results->ai_addrlen) < 0)
    {
        close(sEcoute);
        return -1;
    }
    
    freeaddrinfo(results);
    
    // Mise à l'écoute de la socket
    if (listen(sEcoute, SOMAXCONN)== -1)
    {
        close(sEcoute);
        return -1;
    }
    
    return sEcoute;
}

int Accept(int sEcoute, char *ipClient)
{
    int sService;
    
    // Attente d'une connexion
    if ((sService = accept(sEcoute, NULL, NULL)) == -1)
        return -1;
    
    // Recuperation d'information sur le client connecte
    if (ipClient != NULL)
    {
        struct sockaddr_in adrClient;
        socklen_t adrClientLen = sizeof(struct sockaddr_in);
        
        getpeername(sService, (struct sockaddr*)&adrClient, &adrClientLen);
        
        char host[NI_MAXHOST];
        getnameinfo((struct sockaddr*)&adrClient, adrClientLen,
                    host, NI_MAXHOST,
                    NULL, 0,
                    NI_NUMERICHOST);
        
        strcpy(ipClient, host);
    }
    
    return sService;
}


int ClientSocket(const char* ipServeur, int portServeur)
{
    int sClient;
    char portStr[10];
    
    sprintf(portStr, "%d", portServeur);
    
    // Creation de la socket
    if ((sClient = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return -1;
    
    // Construction de l'adresse du serveur
    struct addrinfo hints;
    struct addrinfo *results;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    if (getaddrinfo(ipServeur, portStr, &hints, &results) != 0)
    {
        close(sClient);
        return -1;
    }
    
    // Demande de connexion
    if (connect(sClient, results->ai_addr, results->ai_addrlen) == -1)
    {
        close(sClient);
        return -1;
    }
    
    freeaddrinfo(results);
    return sClient;
}


int Send(int sSocket, char* data, int taille)
{
    // 1. Envoi de l'entête (4 bytes pour la taille)
    char entete[5];
    sprintf(entete, "%04d", taille); //convertit la taille du msg en chaine de 4 caracteres
    
    if (send(sSocket, entete, 4, 0) != 4) 
        return -1;
    
    // 2. Envoi des données
    int nbEnvoyes = send(sSocket, data, taille, 0); //envoi des 4 caracteres = taille, Envoie le message réel (ex. "LOGIN#Marie#Dupont#123#true")
    return nbEnvoyes;
}

int Receive(int sSocket, char* data)
{
    // 1. Lecture de l'entête (4 bytes)
    char entete[5];
    int nbLus = recv(sSocket, entete, 4, 0); //Lit les 4 premiers caractères qui indiquent la taille du message
    
    if (nbLus <= 0)
        return nbLus;
    
    if (nbLus != 4)
        return -1;
    
    entete[4] = '\0';
    int taille = atoi(entete); //je transforme en entiers pour savoir cmb d'octets je dois lire
    
    // Vérification taille raisonnable
    if (taille < 0 || taille > TAILLE_MAX_DATA)
        return -1;
    
    // 2. Lecture des données
    nbLus = recv(sSocket, data, taille, 0); //Puis je lis exactement ce nb d'octets
    return nbLus;
}