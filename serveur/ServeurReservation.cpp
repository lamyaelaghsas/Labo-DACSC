#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include "TCP.h"
#include "CBP.h"
#include "ACBP.h"



void HandlerSIGINT(int s);
void TraitementConnexion(int sService);
void* FctThreadClient(void* p);
void* FctThreadAdmin(void* p);
void* TraiterRequeteAdmin(void* p);

int sEcoute;

// Gestion du pool de threads          
int NB_THREADS_POOL;    
int PORT_RESERVATION; 
#define TAILLE_FILE_ATTENTE 20
int socketsAcceptees[TAILLE_FILE_ATTENTE]; //tab où je stocke les clients connectés
int indiceEcriture=0, indiceLecture=0;
pthread_mutex_t mutexSocketsAcceptees;
pthread_cond_t condSocketsAcceptees;
int PORT_ADMIN;
int sEcouteAdmin;

// ============== Fonction principale du serveur ==============
int main(int argc,char* argv[])
{
    // ------ Lecture du fichier de configuration ------
    FILE* f;
    if ((f = fopen("config.txt", "r")) == NULL)
    {
        perror("Erreur d'ouverture du fichier.\n");
        printf("Impossible de lire config.txt.\n");
        exit(1);
    }

    char ligne[100];
    while (fgets(ligne, 100, f) != NULL)
    {
        char *cle = strtok(ligne, "="); //strtok retourne la partie avant =
        char *valeur = strtok(NULL, "="); // retourne la partie apres =
        
        if (strcmp(cle, "NB_THREADS_POOL") == 0)
            NB_THREADS_POOL = atoi(valeur);
        else if (strcmp(cle, "PORT_RESERVATION") == 0)
            PORT_RESERVATION = atoi(valeur);
    }
    fclose(f);

    // ------ Initialisation socketsAcceptees ------
    pthread_mutex_init(&mutexSocketsAcceptees,NULL); 
    pthread_cond_init(&condSocketsAcceptees,NULL); //Initialise une variable de condition
    
    // ------ Initialisation du tableau socketsAcceptees à -1 (tte les cases sont vides) ------
    for (int i=0 ; i<TAILLE_FILE_ATTENTE ; i++) 
        socketsAcceptees[i] = -1;

    // ------ Armement des signaux ------
    struct sigaction A;
    A.sa_flags = 0;
    sigemptyset(&A.sa_mask);
    A.sa_handler = HandlerSIGINT;

    if (sigaction(SIGINT,&A,NULL) == -1)
    {
        perror("Erreur de sigaction");
        exit(1);
    }

    // ------ Creation de la socket d'écoute principale du serveur, celle qui va écouter les connexions entrantes ------
    if ((sEcoute = ServerSocket(PORT_RESERVATION)) == -1) //Elle écoute sur le port lu dans config.txt (ex : 50000)
    {
        perror("Erreur de ServeurSocket");
        exit(1);
    }

    // ------ Creation du pool de threads ------
    printf("Création du pool de threads.\n");
    pthread_t th;
    for (int i=0 ; i<NB_THREADS_POOL ; i++)
        pthread_create(&th,NULL,FctThreadClient,NULL); //2 threads sont crées et lancés 

    // -------------------- Mise en boucle du serveur : Se prépare à accepter des connexions infiniment --------------------
    int sService; //sService sera la socket pour parler avec un client précis.
    char ipClient[50];
    printf("Demarrage du serveur.\n");
    while(1)
    {
        printf("Attente d'une connexion...\n");
        if ((sService = Accept(sEcoute,ipClient)) == -1) //le serveur est pret, mtn il attend qu'un client se connecte
        {
            perror("Erreur de Accept");
            close(sEcoute);
            CBP_Close();    
            exit(1);
        }
        printf("Connexion acceptée : IP=%s socket=%d\n",ipClient,sService);

        // ------ Insertion en liste d'attente et réveil d'un thread du pool ------
        pthread_mutex_lock(&mutexSocketsAcceptees);
        socketsAcceptees[indiceEcriture] = sService; //On place la nouvelle socket dans le tableau
        indiceEcriture++;
        if (indiceEcriture == TAILLE_FILE_ATTENTE) indiceEcriture = 0;
        pthread_mutex_unlock(&mutexSocketsAcceptees);
        pthread_cond_signal(&condSocketsAcceptees); //un thread est réveillé par pthread_cond_signal -> FctThreadClient
    }
}

void* FctThreadClient(void* p)
{
    int sService;
    
    while(1) //le thread tourne en boucle infinie
    {
        printf("\t[THREAD %p] Attente socket...\n",pthread_self());

        // Attente d'une tâche
        pthread_mutex_lock(&mutexSocketsAcceptees);
        while (indiceEcriture == indiceLecture) // Si indiceEcriture == indiceLecture, alors la file est vide → on attend.
            pthread_cond_wait(&condSocketsAcceptees,&mutexSocketsAcceptees); //le thread s’endort jusqu’à ce qu’un autre le réveille (avec pthread_cond_signal quand un client se connecte)

        sService = socketsAcceptees[indiceLecture];//on lit la socket a traiter
        socketsAcceptees[indiceLecture] = -1;//puis on la vide, on libere la case dans la file
        indiceLecture++; //on avance l'indice de lecture dans la file
        if (indiceLecture == TAILLE_FILE_ATTENTE) indiceLecture = 0;//si lindice arrive a la fin, on le remet au debut a 0
        pthread_mutex_unlock(&mutexSocketsAcceptees);

        // Traitement de la connexion (consommation de la tâche)
        printf("\t[THREAD %p] Je m'occupe de la socket %d\n",pthread_self(),sService);

        TraitementConnexion(sService); //le thread traite la requete
    }
}


void HandlerSIGINT(int s)
{
    printf("\nArret du serveur.\n");
    close(sEcoute); //ferme la socket d'ecoute principal du serveur (pr ne plus accepter de client)

    pthread_mutex_lock(&mutexSocketsAcceptees); //pendant ce bloc, aucun autre thread ne pourra lire ou écrire dans ce tableau

    for (int i=0 ; i<TAILLE_FILE_ATTENTE ; i++)
        if (socketsAcceptees[i] != -1) close(socketsAcceptees[i]); //si la case contient un socket valide, on appelle close pour terminer la connexion avec ce client

    pthread_mutex_unlock(&mutexSocketsAcceptees);

    CBP_Close();    
    exit(0);
}

void TraitementConnexion(int sService) 
{
    char requete[200], reponse[200];
    int nbLus, nbEcrits;
    bool onContinue = true;

    while (onContinue)
    {
        printf("\t[THREAD %p] Attente requete...\n",pthread_self());
        // ------ Reception Requete ------
        if ((nbLus = Receive(sService,requete)) < 0) //Je lis la trame que le client m’envoie, si ca echoue
        {
            perror("Erreur de Receive");
            close(sService);
            HandlerSIGINT(0);
        }

        // ------ Fin de connexion ------
        if (nbLus == 0)
        {
            printf("\t[THREAD %p] Fin de connexion du client.\n",pthread_self());
            close(sService);
            return;
        }

        requete[nbLus] = '\0';
        printf("\t[THREAD %p] Requete recue = %s\n",pthread_self(),requete);

        // ------ Traitement de la requete ------
        onContinue = CBP(requete,reponse,sService); 
        
        // ------ Envoi de la reponse ------
        if ((nbEcrits = Send(sService,reponse,strlen(reponse))) < 0)
        {
            perror("Erreur de Send");
            close(sService);
            HandlerSIGINT(0);
        }

        printf("\t[THREAD %p] Reponse envoyee = %s\n",pthread_self(),reponse);

        if (!onContinue)
            printf("\t[THREAD %p] Fin de connexion de la socket %d\n",pthread_self(),sService);
    }
}


void* FctThreadAdmin(void* p)
{
	// Creation de la socket d'écoute Admin
	if ((sEcouteAdmin = ServerSocket(PORT_ADMIN)) == -1)
	{
		perror("Erreur de ServerSocket Admin");
		exit(1);
	}

	// Mise en boucle du serveur
	int sService;
	pthread_t th;
	while(1)
	{
		printf("[THREAD ADMIN %p] Attente d'une connexion...\n",pthread_self());
		if ((sService = Accept(sEcouteAdmin,NULL)) == -1)
		{
			perror("Erreur de Accept Admin");
			continue;
		}

		// Creation d'un thread "à la demande" pour traiter cette requête
		int *pSocket = (int*)malloc(sizeof(int));
		*pSocket = sService;
		pthread_create(&th,NULL,TraiterRequeteAdmin,(void*)pSocket);
	}
}

void* TraiterRequeteAdmin(void* p)
{
	int sService = *((int*)p);
	free(p);

	char requete[200], reponse[200];
	int nbLus, nbEcrits;

	printf("\t[THREAD ADMIN %p] Je m'occupe de la socket %d\n",pthread_self(),sService);
	printf("\t[THREAD ADMIN %p] Attente requete...\n",pthread_self());

	// ***** Reception Requete ******************
	if ((nbLus = Receive(sService,requete)) <= 0)
	{
		perror("Erreur de Receive");
		close(sService);
		pthread_exit(NULL);
	}

	requete[nbLus] = 0;
	printf("\t[THREAD ADMIN %p] Requete recue = %s\n",pthread_self(),requete);

	// ***** Traitement de la requete ***********
	ACBP(requete,reponse);

	// ***** Envoi de la reponse ****************
	if ((nbEcrits = Send(sService,reponse,strlen(reponse))) < 0)
	{
		perror("Erreur de Send");
		close(sService);
		pthread_exit(NULL);
	}

	printf("\t[THREAD ADMIN %p] Reponse envoyee = %s\n",pthread_self(),reponse);

		close(sService);

	pthread_exit(NULL);
}
