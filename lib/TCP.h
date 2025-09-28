#ifndef TCP_H
#define TCP_H

// Taille maximum des données 
#define TAILLE_MAX_DATA 10000

/**
 * Crée une socket serveur et la met en écoute sur le port spécifié
 * @param port Port d'écoute du serveur
 * @return Descripteur de la socket d'écoute, ou -1 en cas d'erreur
 */
int ServerSocket(int port);

/**
 * Accepte une connexion sur une socket d'écoute
 * @param sEcoute Socket d'écoute retournée par ServerSocket()
 * @param ipClient Buffer pour recevoir l'IP du client (peut être NULL)
 * @return Descripteur de la socket de service, ou -1 en cas d'erreur
 */
int Accept(int sEcoute, char *ipClient);

/**
 * Crée une socket client et se connecte au serveur
 * @param ipServeur Adresse IP du serveur (ex: "192.168.1.100")
 * @param portServeur Port du serveur
 * @return Descripteur de la socket de service, ou -1 en cas d'erreur
 */
int ClientSocket(char* ipServeur, int portServeur);

/**
 * Envoie des données sur une socket (avec entête de taille)
 * @param sSocket Socket de service
 * @param data Pointeur vers les données à envoyer
 * @param taille Nombre d'octets à envoyer
 * @return Nombre d'octets envoyés, ou -1 en cas d'erreur
 */
int Send(int sSocket, char* data, int taille);

/**
 * Reçoit des données sur une socket (lit l'entête puis les données)
 * @param sSocket Socket de service
 * @param data Buffer pour recevoir les données
 * @return Nombre d'octets reçus, ou -1 en cas d'erreur, 0 si connexion fermée
 */
int Receive(int sSocket, char* data);

#endif