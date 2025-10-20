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
    int sEcoute; //var qui va stocker le descripteur de la socket d'ecoute
    
    // 1. Création de la socket
    if ((sEcoute = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return -1; //on met return -1 car c'est une librairie 
    }
    
    // 2. Option SO_REUSEADDR (pour éviter "Address already in use") si on arrete le serveur avec ctrl C et quon essaye de le relancer direct, on aurait l'erreur Address already... pcq le syst garde le port reservé pdt 30-60sec et SO_REUSEADDR force la reutilisation immdeiate du port
    int opt = 1;
    if (setsockopt(sEcoute, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1) { //Dit au système "si le port était occupé, libère-le immédiatement"
        close(sEcoute);
        return -1;
    }
    
    // 3. Construction de l'adresse réseau 
    struct addrinfo hints;
    struct addrinfo *results;
    memset(&hints, 0, sizeof(struct addrinfo)); //on met tous les champs de addrinfo à 0 
    hints.ai_family = AF_INET;  //je veux ipv4
    hints.ai_socktype = SOCK_STREAM; //je veux tcp 
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV; // AI_PASSIVE = je suis un serveur et AI_NUMERICSERV = le port est un nb pas un nom
    
    char port_str[10];
    sprintf(port_str, "%d", port); //convertit le port en string pcq getaddrinfo attend une string pas un int
    
    if (getaddrinfo(NULL, port_str, &hints, &results) != 0) { //prépare une structure d'adresse réseau avec le port demandé
        close(sEcoute);
        return -1;
    }
    
    // 4. Bind (liaison socket <-> adresse)
    if (bind(sEcoute, results->ai_addr, results->ai_addrlen) == -1) { //attache la socket au port spécifié
        freeaddrinfo(results); //si bind echoue, freeaddrinfo va liberer la memoire allouée par getaddrinfo()
        close(sEcoute);
        return -1;
    }
    
    freeaddrinfo(results); //getaddrinfo a alloué de la memoire dyn. et on en a plus besoin apres bind donc faut liberer la mem pour eviter les fuites 
    
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
int Accept(int sEcoute, char *ipClient) 
{ 
    int sService;
    
    // BLOQUE le programme jusqu'à ce qu'un client se connecte
    if ((sService = accept(sEcoute, NULL, NULL)) == -1) //bloque le prog en attendant qu'un client se connecte. qd ca arrive, retourne une nvl socket pour communiquer avec ce client
        return -1;
    
    
    // Récupération IP client si demandée 
    if (ipClient != NULL) 
    {
        struct sockaddr_in adrClient; //Structure qui contient une adresse IPv4 (IP + port)
        socklen_t adrClientLen = sizeof(struct sockaddr_in); //adrClientLen : Taille de la structure (16 bytes)
        
        //recuperation des info (adresse ip + port) sur le client connecté à la socket de service obtenue par le serveur
        if (getpeername(sService, (struct sockaddr*)&adrClient, &adrClientLen) == 0) 
        { 
            
            //Buffers pour stocker l'IP et le port en format string.
            char host[NI_MAXHOST];
            char port[NI_MAXSERV];
            
            if (getnameinfo((struct sockaddr*)&adrClient, adrClientLen,
                           host, NI_MAXHOST, port, NI_MAXSERV,
                           NI_NUMERICHOST | NI_NUMERICSERV) == 0) {  //convertit l'adresse réseau en string IP lisible (ex : "192.168.1.100").Sans NI_NUMERICHOST, on pourrais avoir "pc-de-jean.local" au lieu de "192.168.1.50".
                
                strcpy(ipClient, host);
            } else {
                strcpy(ipClient, "Inconnu");
            }
        } else {
            strcpy(ipClient, "Inconnu");
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
    hints.ai_flags = AI_NUMERICSERV; //pas de AI-PASSIVE car c'est pas un seerveur qui ecoute. AI_NUMERICSERV = Le port est un nombre
    
    char port_str[10];
    sprintf(port_str, "%d", portServeur); //Convertit 50000 en "50000" car getaddrinfo() attend une string
    
    if (getaddrinfo(ipServeur, port_str, &hints, &results) != 0) { //Prépare l'adresse du serveur (IP + port)
        close(sClient);
        return -1;
    }
    
    // 3. Connexion au serveur
    if (connect(sClient, results->ai_addr, results->ai_addrlen) == -1) { //Établit la connexion TCP avec le serveur
        freeaddrinfo(results); //libere 
        close(sClient);
        return -1;
    }
    
    freeaddrinfo(results);
    return sClient;
}

/**
 * Style : 2 envois : d'abord l'entete puis les données
 */
int Send(int sSocket, char* data, int taille) {
    // 1. Creation de l'entete
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
        return nbLus; // Erreur
    }
    
    if (nbLus != 4) {
        return -1; // Entête incomplète
    }
    
    entete[4] = '\0';
    int taille = atoi(entete); //Convertit l'entête en nombre (ex: "0025" -> 25)
    
    // Vérification taille raisonnable
    if (taille < 0 || taille > TAILLE_MAX_DATA) {
        return -1;
    }
    
    // 2. Lecture des données
    nbLus = recv(sSocket, data, taille, 0); //Lit exactement le nombre de bytes annoncé
    return nbLus;
}

int Echanger(int sClient, const char* requete, char* reponse)
{
    int nbBytes;

    // Le client envoie un la requete (trame CBP) au serveur (+1 car \0 inclus)
    nbBytes = send(sClient, requete, strlen(requete) + 1, 0); 
    if (nbBytes <= 0)
    {
        perror("Erreur lors de l’envoi de la requête\n");
        return -1;
    }

    // Le client attend de recevoir une réponse du serveur Réservation
    nbBytes = recv(sClient, reponse, 500, 0); //Il reçoit au maximum 500 caractères
    if (nbBytes <= 0)
    {
        perror("Erreur lors de la réception de la réponse\n");
        return -1;
    }

    reponse[nbBytes] = '\0'; // fin de chaîne

    return 0;
}