#ifndef CBP_H
#define CBP_H

#include <stdbool.h>

#define TAILLE_MAX_REPONSE 10000 // Taille max pour les reponses

typedef struct {
    int id;
    char lastName[30];
    char firstName[30];
    char birthDate[20];
} PATIENT;

typedef struct {
    int id;
    char name[30];
} SPECIALTY;

typedef struct {
    int id;
    int specialtyId;
    char lastName[30];
    char firstName[30];
} DOCTOR;

typedef struct {
    int id;
    int doctorId;
    int patientId;
    char date[20];
    char hour[10];
    char reason[100];
} CONSULTATION;

/**
 * Fonction principale de gestion du protocole CBP
 * @param requete Requete recue du client
 * @param reponse Buffer pour la reponse (sera rempli)
 * @param socket Socket du client (pour gerer l'etat de session)
 * @return true si la connexion doit continuer, false si LOGOUT
 */
bool CBP(char* requete, char* reponse, int socket);

/**
 * Traite une requete LOGIN
 * @param lastName Nom du patient
 * @param firstName Prenom du patient  
 * @param patientId Id du patient (0 si nouveau)
 * @param newPatient true si nouveau patient
 * @param reponse Buffer pour la reponse
 * @return true si login OK
 */
bool CBP_Login(const char* last_name, const char* first_name, int patient_id, bool new_patient, char* reponse, int socket);

/**
 * Traite une requete GET_SPECIALTIES
 * @param reponse Buffer pour la reponse
 */
void CBP_GetSpecialties(char* reponse);

/**
 * Traite une requete GET_DOCTORS
 * @param reponse Buffer pour la reponse
 */
void CBP_GetDoctors(char* reponse);

/**
 * Traite une requete SEARCH_CONSULTATIONS
 * @param specialty Specialite recherchee (ou "" pour toutes)
 * @param doctor Medecin recherche (ou "" pour tous)
 * @param startDate Date de debut
 * @param endDate Date de fin
 * @param reponse Buffer pour la reponse
 */
void CBP_SearchConsultations(const char* specialty, const char* doctor, const char* startDate, const char* endDate, char* reponse);

/**
 * Traite une requete BOOK_CONSULTATION
 * @param consultationId Id de la consultation a reserver
 * @param reason Raison de la consultation
 * @param patientId Id du patient qui reserve
 * @param reponse Buffer pour la reponse
 * @return true si reservation OK
 */
bool CBP_BookConsultation(int consultationId, const char* reason, int patientId, char* reponse);

/**
 * Initialise la connexion a la base de donnees
 * @return true si OK
 */
bool CBP_InitDB();

/**
 * Ferme la connexion a la base de donnees
 */
void CBP_CloseDB();

/**
 * Gestion de l'etat des sessions clients
 */
bool CBP_IsLoggedIn(int socket);
int CBP_GetPatientId(int socket);
void CBP_SetLoggedIn(int socket, int patientId);
void CBP_SetLoggedOut(int socket);

#endif