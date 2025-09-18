#ifndef SOCKET_H
#define SOCKET_H

// Création du socket (IPv4, TCP)
int Socket();

// Associer une adresse (IP + port) à un socket (côté serveur)
int Bind(int sockfd, const char* ip, int port);

// Mise en écoute (côté serveur)
int Listen(int sockfd, int backlog);

// Accepter une connexion entrante (côté serveur)
int Accept(int sockfd);

// Se connecter à un serveur (côté client)
int Connect(int sockfd, const char* ip, int port);

// Envoyer des données
int Send(int sockfd, const void* buffer, int length);

// Recevoir des données
int Receive(int sockfd, void* buffer, int length);

// Fermer un socket
int Close(int sockfd);

#endif
