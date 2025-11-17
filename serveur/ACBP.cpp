#include "ACBP.h"
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "CLIENT.h"

extern CLIENT clients[];
extern int nbClients;
extern pthread_mutex_t mutexClients;

//***** Parsing de la requete et creation de la reponse *************
void ACBP(char* requete, char* reponse)
{
    // ***** Récupération nom de la requete *****************
	char *ptr = strtok(requete,"#");

    // ***** LIST_CLIENTS ******************************************
	if (strcmp(ptr,"LIST_CLIENTS") == 0)
	{
		printf("\t[THREAD ADMIN] LIST_CLIENTS\n");

		pthread_mutex_lock(&mutexClients);

		char buffer[2000] = "";
		strcpy(buffer, "LIST_CLIENTS");

		for(int i=0; i<nbClients; i++) //boucle sur tous les clients connectés
		{
            char ligneClient[200];
			sprintf(ligneClient,"#%s#%s#%s#%s",
            clients[i].ip,
            clients[i].nom,
            clients[i].prenom,
            clients[i].noPatient);
            strcat(buffer, ligneClient);
		}

		pthread_mutex_unlock(&mutexClients);

		strcpy(reponse, buffer);
	}
}