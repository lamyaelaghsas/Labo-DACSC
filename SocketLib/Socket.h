#ifndef SOCKET_H
#define SOCKET_H

#define TAILLE_MAX_DATA 10000

// Fonction pour créer une socket serveur (écoute sur un port)
int ServerSocket(int port);

// Fonction pour accepter une connexion client (retourne socket de service)
int Accept(int sEcoute, char *ipClient);

// Fonction pour se connecter à un serveur (côté client)
int ClientSocket(char* ipServeur, int portServeur);

// Fonction pour envoyer des données (avec taille)
int Send(int sSocket, char* data, int taille);

// Fonction pour recevoir des données (retourne la taille reçue)
int Receive(int sSocket, char* data);

#endif