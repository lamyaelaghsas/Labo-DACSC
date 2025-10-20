#include "CBP.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <mysql/mysql.h>


//***** Etat du protocole : liste des clients loggés ****************
typedef struct
{
    int socket;
    int patientId;
} CLIENT;
CLIENT clients[NB_MAX_CLIENTS];
int nbClients = 0;


int estPresent(int socket);
void ajoute(int socket, int patientId);
void retire(int socket);

pthread_mutex_t mutexClients = PTHREAD_MUTEX_INITIALIZER;


//***** Connexion globale MySQL *************************************
MYSQL* connexionBD = NULL;


//*******************************************************************
//***** Parsing de la requete et creation de la reponse *************
bool CBP(char* requete, char* reponse,int socket)
{
    // ***** Récupération nom de la requete *****************
    char *ptr = strtok(requete,"#");


    // ***** LOGIN ******************************************
    if (strcmp(ptr,"LOGIN") == 0) 
    {
        char nom[50], prenom[50], noPatient[50], nouveauPatient[50];

        strcpy(nom, strtok(NULL, "#"));
        strcpy(prenom, strtok(NULL, "#"));
        strcpy(noPatient, strtok(NULL, "#"));
        strcpy(nouveauPatient, strtok(NULL, "#"));

        printf("\t[THREAD %p] LOGIN de %s\n",pthread_self(),nom);
        
        if (estPresent(socket) >= 0)
        {
            sprintf(reponse,"LOGIN#Non#Client deja loge");
            return false;
        }
        
        int patientId;
        if(CBP_Login(nom,prenom,noPatient,nouveauPatient,&patientId))
        {
            if (strcmp(nouveauPatient, "true") == 0)
                sprintf(reponse,"LOGIN#Oui#%d", patientId);
            else
                sprintf(reponse,"LOGIN#Oui");
            
            ajoute(socket, patientId);
        }
        else
        {
            sprintf(reponse,"LOGIN#Non");
            return false;
        }
    }



    // ***** LOGOUT *****************************************
    if (strcmp(ptr,"LOGOUT") == 0)
    {
        printf("\t[THREAD %p] LOGOUT\n",pthread_self());
        retire(socket);
        sprintf(reponse,"LOGOUT#ok");
        return false;
    }


    // ***** GET_SPECIALTIES ********************************
    if (strcmp(ptr,"GET_SPECIALTIES") == 0)
    {
        printf("\t[THREAD %p] GET_SPECIALTIES\n",pthread_self());
        if (estPresent(socket) == -1) sprintf(reponse,"GET_SPECIALTIES#ko#Client non loggé !");
        else
        {
            char* specialties = CBP_GetSpecialties();
            sprintf(reponse,"GET_SPECIALTIES#%s",specialties);
            free(specialties);
        }
    }


    // ***** GET_DOCTORS ************************************
    if (strcmp(ptr,"GET_DOCTORS") == 0)
    {
        printf("\t[THREAD %p] GET_DOCTORS\n",pthread_self());
        if (estPresent(socket) == -1) sprintf(reponse,"GET_DOCTORS#ko#Client non loggé !");
        else
        {
            char* doctors = CBP_GetDoctors();
            sprintf(reponse,"GET_DOCTORS#%s",doctors);
            free(doctors);
        }
    }
    

    // ***** SEARCH_CONSULTATIONS ***************************
    if (strcmp(ptr,"SEARCH_CONSULTATIONS") == 0)
    {
        char specialite[50], medecin[50], dateDebut[50], dateFin[50];

        strcpy(specialite, strtok(NULL, "#"));
        strcpy(medecin, strtok(NULL, "#"));
        strcpy(dateDebut, strtok(NULL, "#"));
        strcpy(dateFin, strtok(NULL, "#"));

        printf("\t[THREAD %p] SEARCH_CONSULTATIONS\n",pthread_self());
        
        if (estPresent(socket) == -1)
        {
            sprintf(reponse,"SEARCH_CONSULTATIONS#ko#Client non loge");
        }
        else
        {
            char* consultations = CBP_SearchConsultations(specialite,medecin,dateDebut,dateFin);
            sprintf(reponse,"SEARCH_CONSULTATIONS#%s",consultations);
            free(consultations);
        }
    }


    // ***** BOOK_CONSULTATION *******************************
    if (strcmp(ptr,"BOOK_CONSULTATION") == 0)
    {
        char consultationId[50], reason[50];

        strcpy(consultationId, strtok(NULL, "#"));
        strcpy(reason, strtok(NULL, "#"));

        printf("\t[THREAD %p] BOOK_CONSULTATION %s\n",pthread_self(),consultationId);
        
        int patientId = estPresent(socket);
        if (patientId == -1)
        {
            sprintf(reponse,"BOOK_CONSULTATION#ko#Client non loge");
        }
        else
        {
            if (CBP_BookConsultation(consultationId,reason,patientId))
                sprintf(reponse,"BOOK_CONSULTATION#Oui");
            else
                sprintf(reponse,"BOOK_CONSULTATION#Non");
        }
    }

    return true;

}


//***** Traitement des requetes *************************************
bool CBP_Login(const char* nom, const char* prenom, const char* noPatient,const char* nouveauPatient, int* patientId)
{
    // Connexion à la base de données
    if (connexionBD == NULL)
    {
        connexionBD = mysql_init(NULL);
        if (mysql_real_connect(connexionBD,"localhost","Student","PassStudent1_","PourStudent",0,NULL,0) == NULL)
        {
            fprintf(stderr, "Erreur de connexion à la BD: %s\n",mysql_error(connexionBD));
            return false;
        }
        printf("Connexion établie avec succès à la BD.\n");
    }
    
    
    // Construction et exécution de la requête
    char requete[256];
    sprintf(requete, "SELECT id FROM patients WHERE last_name='%s' AND first_name='%s' AND id=%s", nom, prenom, noPatient);  
    if (mysql_query(connexionBD, requete) != 0)
    {
        fprintf(stderr, "Erreur de mysql_query: %s\n", mysql_error(connexionBD));
        return false;
    }
    
    // Affichage du Result Set
    MYSQL_RES *ResultSet;
    if ((ResultSet = mysql_store_result(connexionBD)) == NULL)
    {
        fprintf(stderr, "Erreur de mysql_store_result: %s\n", mysql_error(connexionBD));
        return false;
    }
    
    MYSQL_ROW ligne = mysql_fetch_row(ResultSet);
    

    //CAS 1 : patient existe avec nom+prenom+ID
    if (ligne != NULL)
    {
        if (strcmp(nouveauPatient, "true") == 0)
        {
            return false;
        }
        else
        {
            *patientId = atoi(ligne[0]);
            return true;
        }
    }
    //CAS 2 : patient nexiste pas avec nom+prenom+ID
    else
    {
        if (strcmp(nouveauPatient, "true") == 0)
        {
            sprintf(requete, "INSERT INTO patients VALUES (NULL,'%s','%s',NULL)", nom, prenom);
            
            if (mysql_query(connexionBD, requete) != 0)
            {
                fprintf(stderr, "Erreur de mysql_query: %s\n", mysql_error(connexionBD));
                return false;
            }
            
            sprintf(requete, "SELECT id FROM patients WHERE last_name='%s' AND first_name='%s' ORDER BY id DESC LIMIT 1", nom, prenom);
            
            if (mysql_query(connexionBD, requete) != 0)
            {
                fprintf(stderr, "Erreur de mysql_query: %s\n", mysql_error(connexionBD));
                return false;
            }
            
            if ((ResultSet = mysql_store_result(connexionBD)) == NULL)
            {
                fprintf(stderr, "Erreur de mysql_store_result: %s\n", mysql_error(connexionBD));
                return false;
            }
            
            ligne = mysql_fetch_row(ResultSet);
            
            if (ligne != NULL)
            {
                *patientId = atoi(ligne[0]);
                return true;
            }
        }
        else
        {
            return false;
        }
    }

    return false;
}



char* CBP_GetSpecialties()
{
    // Construction et exécution de la requête
    char requete[256];
    sprintf(requete,"SELECT id, name FROM specialties;");

    if (mysql_query(connexionBD,requete) != 0)
    {
        fprintf(stderr, "Erreur de mysql_query: %s\n",mysql_error(connexionBD));
        return NULL;
    }
    
    // Affichage du Result Set
    MYSQL_RES *ResultSet;
    if ((ResultSet = mysql_store_result(connexionBD)) == NULL)
    {
        fprintf(stderr, "Erreur de mysql_store_result: %s\n",mysql_error(connexionBD));
        return NULL;
    }
    
    
    char* reponse = (char*)malloc(2000);
    strcpy(reponse,"");
    
    
    MYSQL_ROW ligne;
    while ((ligne = mysql_fetch_row(ResultSet)) != NULL)
    {
        if (strlen(reponse) > 0) strcat(reponse,"#");
        
        
        strcat(reponse,ligne[0]); //id
        strcat(reponse,"#");
        strcat(reponse,ligne[1]); //name
    }
    
    
    
    return reponse;
}



char* CBP_GetDoctors()
{
    // Construction et exécution de la requête
    char requete[256];
    sprintf(requete,"SELECT id ,last_name, first_name FROM doctors;");

    if (mysql_query(connexionBD,requete) != 0)
    {
        fprintf(stderr, "Erreur de mysql_query: %s\n",mysql_error(connexionBD));
        return NULL;
    }
    
    // Affichage du Result Set
    MYSQL_RES *ResultSet;
    if ((ResultSet = mysql_store_result(connexionBD)) == NULL)
    {
        fprintf(stderr, "Erreur de mysql_store_result: %s\n",mysql_error(connexionBD));
        return NULL;
    }
    
    
    char* reponse = (char*)malloc(2000);
    strcpy(reponse,"");
    
    
    MYSQL_ROW ligne;
    while ((ligne = mysql_fetch_row(ResultSet)) != NULL)
    {
        if (strlen(reponse) > 0) strcat(reponse,"#");
        
        
        strcat(reponse,ligne[0]); //id
        strcat(reponse,"#");
        strcat(reponse,ligne[1]); //last_name
        strcat(reponse,"#");
        strcat(reponse,ligne[2]); //first_name
    }
    
    
    
    return reponse;
}



char* CBP_SearchConsultations(const char* specialite, const char* medecin, const char* dateDebut, const char* dateFin)
{
    // Construction de la requête avec gestion des filtres
    char requete[1000];
    
    // Cas 1 : --- TOUTES --- et --- TOUS --- (pas de filtre)
    if (strcmp(specialite, "--- TOUTES ---") == 0 && strcmp(medecin, "--- TOUS ---") == 0)
    {
        sprintf(requete,"SELECT c.id, s.name, CONCAT(d.last_name, ' ', d.first_name), c.date, c.hour "
                        "FROM consultations c "
                        "JOIN doctors d ON c.doctor_id = d.id "
                        "JOIN specialties s ON d.specialty_id = s.id "
                        "WHERE c.patient_id IS NULL "
                        "AND c.date BETWEEN '%s' AND '%s' ", dateDebut, dateFin);
    }
    // Cas 2 : Spécialité choisie, --- TOUS --- médecins
    else if (strcmp(medecin, "--- TOUS ---") == 0)
    {
        sprintf(requete,"SELECT c.id, s.name, CONCAT(d.last_name, ' ', d.first_name), c.date, c.hour "
                        "FROM consultations c "
                        "JOIN doctors d ON c.doctor_id = d.id "
                        "JOIN specialties s ON d.specialty_id = s.id "
                        "WHERE c.patient_id IS NULL "
                        "AND c.date BETWEEN '%s' AND '%s' "
                        "AND s.name = '%s' ", dateDebut, dateFin, specialite);
    }
    // Cas 3 : --- TOUTES --- spécialités, médecin choisi
    else if (strcmp(specialite, "--- TOUTES ---") == 0)
    {
        sprintf(requete,"SELECT c.id, s.name, CONCAT(d.last_name, ' ', d.first_name), c.date, c.hour "
                        "FROM consultations c "
                        "JOIN doctors d ON c.doctor_id = d.id "
                        "JOIN specialties s ON d.specialty_id = s.id "
                        "WHERE c.patient_id IS NULL "
                        "AND c.date BETWEEN '%s' AND '%s' "
                        "AND CONCAT(d.last_name, ' ', d.first_name) = '%s' ", dateDebut, dateFin, medecin);
    }
    // Cas 4 : Spécialité ET médecin choisis
    else
    {
        sprintf(requete,"SELECT c.id, s.name, CONCAT(d.last_name, ' ', d.first_name), c.date, c.hour "
                        "FROM consultations c "
                        "JOIN doctors d ON c.doctor_id = d.id "
                        "JOIN specialties s ON d.specialty_id = s.id "
                        "WHERE c.patient_id IS NULL "
                        "AND c.date BETWEEN '%s' AND '%s' "
                        "AND s.name = '%s' "
                        "AND CONCAT(d.last_name, ' ', d.first_name) = '%s' ", 
                        dateDebut, dateFin, specialite, medecin);
    }

    if (mysql_query(connexionBD,requete) != 0)
    {
        fprintf(stderr, "Erreur de mysql_query: %s\n",mysql_error(connexionBD));
        return NULL;
    }

    // Affichage du Result Set
    MYSQL_RES *ResultSet;
    if ((ResultSet = mysql_store_result(connexionBD)) == NULL)
    {
        fprintf(stderr, "Erreur de mysql_store_result: %s\n",mysql_error(connexionBD));
        return NULL;
    }
    
    char* reponse = (char*)malloc(2000);
    strcpy(reponse,"");
    
    MYSQL_ROW ligne;
    while ((ligne = mysql_fetch_row(ResultSet)) != NULL)
    {
        if (strlen(reponse) > 0) strcat(reponse,"#");
        
        strcat(reponse,ligne[0]); //id
        strcat(reponse,"#");
        strcat(reponse,ligne[1]); //specialite
        strcat(reponse,"#");
        strcat(reponse,ligne[2]); //medecin
        strcat(reponse,"#");
        strcat(reponse,ligne[3]); //date
        strcat(reponse,"#");
        strcat(reponse,ligne[4]); //heure
    }
    
    return reponse;
}



bool CBP_BookConsultation(const char* consultationId, const char* reason, int patientId)
{
    // Vérification de la disponibilité de la consultation
    char requete[256];
    sprintf(requete, "SELECT id FROM consultations WHERE id = %s AND patient_id IS NULL", consultationId);
    
    if (mysql_query(connexionBD, requete) != 0)
    {
        fprintf(stderr, "Erreur de mysql_query: %s\n", mysql_error(connexionBD));
        return false;
    }
    
    MYSQL_RES *ResultSet;
    if ((ResultSet = mysql_store_result(connexionBD)) == NULL)
    {
        fprintf(stderr, "Erreur de mysql_store_result: %s\n", mysql_error(connexionBD));
        return false;
    }
    
    MYSQL_ROW ligne;
    if ((ligne = mysql_fetch_row(ResultSet)) == NULL)
        return false;  // Consultation non disponible
    
    // Construction et exécution de la requête 
    sprintf(requete, "UPDATE consultations "
                    "SET patient_id = %d, reason = '%s' "
                    "WHERE id = %s", patientId, reason, consultationId);
    
    if (mysql_query(connexionBD, requete) != 0)
    {
        fprintf(stderr, "Erreur de mysql_query: %s\n", mysql_error(connexionBD));
        return false;
    }
    
    return true;
}


//***** Fin prématurée **********************************************
// Fonction appelée au CTRL+C pour fermer proprement le serveur
void CBP_Close()
{
    // Fermeture de la connexion MySQL
    mysql_close(connexionBD);
    
    // Fermeture de toutes les sockets clientes encore ouvertes
    pthread_mutex_lock(&mutexClients);
    for(int i=0; i<nbClients; i++)
        close(clients[i].socket);
    pthread_mutex_unlock(&mutexClients);
}

//***** Gestion de l'état du protocole ******************************
// Ajoute un client à la liste des clients connectés
// Paramètres : socket du client, ID du patient connecté
void ajoute(int socket, int patientId)
{
    pthread_mutex_lock(&mutexClients);  // Protection accès concurrent
    clients[nbClients].socket = socket;
    clients[nbClients].patientId = patientId;
    nbClients++;
    pthread_mutex_unlock(&mutexClients);
}

// Vérifie si une socket est déjà connectée
// Retourne : l'ID du patient si connecté, -1 sinon
int estPresent(int socket)
{
    int indice = -1;
    pthread_mutex_lock(&mutexClients);  // Protection accès concurrent
    for(int i=0; i<nbClients; i++)
        if (clients[i].socket == socket) { 
            indice = clients[i].patientId; 
            break; 
        }
    pthread_mutex_unlock(&mutexClients);
    return indice;
}

// Retire un client de la liste des clients connectés (au LOGOUT)
// Paramètre : socket du client à retirer
void retire(int socket)
{
    pthread_mutex_lock(&mutexClients);  // Protection accès concurrent
    
    // Recherche de la position du client dans le tableau
    int pos = -1;
    for(int i=0; i<nbClients; i++)
    {
        if (clients[i].socket == socket) 
        { 
            pos = i; 
            break; 
        }
    }
    
    // Si socket non trouvée, on sort
    if (pos == -1)
    {
        pthread_mutex_unlock(&mutexClients);
        return;
    }
    
    // Décalage du tableau pour supprimer l'élément
    for (int i=pos; i<=nbClients-2; i++)
        clients[i] = clients[i+1];
    nbClients--;
    
    pthread_mutex_unlock(&mutexClients);
}

