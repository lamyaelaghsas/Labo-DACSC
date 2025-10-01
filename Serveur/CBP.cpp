#include "CBP.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <pthread.h>

// ========== VARIABLES GLOBALES ==========

// Connexion MySQL globale
static MYSQL* connexion = NULL;

// NOUVEAU : Mutex pour proteger les acces MySQL
static pthread_mutex_t mutex_mysql = PTHREAD_MUTEX_INITIALIZER;

// Gestion des sessions clients (socket -> patient_id)
#define MAX_CLIENTS 100
static int clients_logged[MAX_CLIENTS];
static int clients_patient_id[MAX_CLIENTS];
static int nb_clients = 0;
static pthread_mutex_t mutex_clients = PTHREAD_MUTEX_INITIALIZER;

// Fonctions utilitaires
static int find_client_index(int socket);
static void add_client(int socket, int patient_id);
static void remove_client(int socket);

/**
 * Fonction principale de gestion du protocole CBP
 */
bool CBP(char* requete, char* reponse, int socket) {
    // Parsing de la requete 
    char* token = strtok(requete, "#");
    if (token == NULL) {
        strcpy(reponse, "ERROR#Requete malformee");
        return true;
    }
    
    // LOGIN
    if (strcmp(token, "LOGIN") == 0) {
        char* last_name = strtok(NULL, "#");
        char* first_name = strtok(NULL, "#");
        char* patient_id_str = strtok(NULL, "#");
        char* new_patient_str = strtok(NULL, "#");
        
        if (!last_name || !first_name || !new_patient_str) {
            strcpy(reponse, "LOGIN#ko#Parametres manquants");
            return true;
        }
        
        int patient_id = 0;
        if (patient_id_str && strlen(patient_id_str) > 0) {
            patient_id = atoi(patient_id_str);
        }
        
        bool new_patient = (strcmp(new_patient_str, "true") == 0);
        
        if (CBP_Login(last_name, first_name, patient_id, new_patient, reponse, socket)) {
            return true;
        } else {
            return false; // Echec login = deconnexion
        }
    }
    
    // LOGOUT
    else if (strcmp(token, "LOGOUT") == 0) {
        CBP_SetLoggedOut(socket);
        strcpy(reponse, "LOGOUT#ok");
        return false; // Fin de connexion
    }
    
    // Verifier que le client est connecte pour les autres commandes
    if (!CBP_IsLoggedIn(socket)) {
        strcpy(reponse, "ERROR#Non connecte");
        return true;
    }
    
    // GET_SPECIALTIES
    if (strcmp(token, "GET_SPECIALTIES") == 0) {
        CBP_GetSpecialties(reponse);
    }
    
    // GET_DOCTORS
    else if (strcmp(token, "GET_DOCTORS") == 0) {
        CBP_GetDoctors(reponse);
    }
    
    // SEARCH_CONSULTATIONS
    else if (strcmp(token, "SEARCH_CONSULTATIONS") == 0) {
        const char* specialty = strtok(NULL, "#");
        const char* doctor = strtok(NULL, "#");
        const char* start_date = strtok(NULL, "#");
        const char* end_date = strtok(NULL, "#");
        
        // Gerer les parametres optionnels
        if (!specialty) specialty = "";
        if (!doctor) doctor = "";
        if (!start_date) start_date = "2025-01-01";
        if (!end_date) end_date = "2025-12-31";
        
        CBP_SearchConsultations(specialty, doctor, start_date, end_date, reponse);
    }
    
    // BOOK_CONSULTATION
    else if (strcmp(token, "BOOK_CONSULTATION") == 0) {
        char* consultation_id_str = strtok(NULL, "#");
        char* reason = strtok(NULL, "#");
        
        if (!consultation_id_str || !reason) {
            strcpy(reponse, "BOOK#ko#Parametres manquants");
            return true;
        }
        
        int consultation_id = atoi(consultation_id_str);
        int patient_id = CBP_GetPatientId(socket);
        
        CBP_BookConsultation(consultation_id, reason, patient_id, reponse);
    }
    
    // Commande inconnue
    else {
        strcpy(reponse, "ERROR#Commande inconnue");
    }
    
    return true;
}

/**
 * Traite une requete LOGIN
 */
bool CBP_Login(const char* last_name, const char* first_name, int patient_id, 
               bool new_patient, char* reponse, int socket) {
    if (!connexion) {
        strcpy(reponse, "LOGIN#ko#Erreur base de donnees\r\n");
        return false;
    }
    
    char safe_lastname[100], safe_firstname[100];
    mysql_real_escape_string(connexion, safe_lastname, last_name, strlen(last_name));
    mysql_real_escape_string(connexion, safe_firstname, first_name, strlen(first_name));
    
    char query[500];
    MYSQL_RES* result;
    
    pthread_mutex_lock(&mutex_mysql);
    
    if (new_patient) {
        // Verifier si le patient existe deja
        sprintf(query, "SELECT id FROM patients WHERE last_name='%s' AND first_name='%s'", 
                safe_lastname, safe_firstname);
        
        if (mysql_query(connexion, query)) {
            pthread_mutex_unlock(&mutex_mysql);
            strcpy(reponse, "LOGIN#ko#Erreur requete\r\n");
            return false;
        }
        
        result = mysql_store_result(connexion);
        int num_rows = mysql_num_rows(result);
        
        if (num_rows > 0) {
            // Patient existe deja
            MYSQL_ROW row = mysql_fetch_row(result);
            int existing_id = atoi(row[0]);
            mysql_free_result(result);
            pthread_mutex_unlock(&mutex_mysql);
            sprintf(reponse, "LOGIN#ko#Patient existe deja (ID=%d)\r\n", existing_id);
            return false;
        }
        
        mysql_free_result(result);
        
        // Creer le nouveau patient
        sprintf(query, "INSERT INTO patients (last_name, first_name, birth_date) "
                       "VALUES ('%s', '%s', '1990-01-01')", 
                safe_lastname, safe_firstname);
        
        if (mysql_query(connexion, query)) {
            pthread_mutex_unlock(&mutex_mysql);
            strcpy(reponse, "LOGIN#ko#Erreur creation patient\r\n");
            return false;
        }
        
        int new_id = mysql_insert_id(connexion);
        pthread_mutex_unlock(&mutex_mysql);
        
        sprintf(reponse, "LOGIN#ok#%d\r\n", new_id);
        CBP_SetLoggedIn(socket, new_id);
        return true;
        
    } else {
    // Verifier patient existant
    if (patient_id > 0) {
        sprintf(query, "SELECT id FROM patients WHERE last_name='%s' AND first_name='%s' AND id=%d", 
                safe_lastname, safe_firstname, patient_id);
    } else {
        sprintf(query, "SELECT id FROM patients WHERE last_name='%s' AND first_name='%s'", 
                safe_lastname, safe_firstname);
    }
    
    if (mysql_query(connexion, query)) {
        pthread_mutex_unlock(&mutex_mysql);
        strcpy(reponse, "LOGIN#ko#Erreur requete\r\n");
        return false;
    }
    
    result = mysql_store_result(connexion);
    int num_rows = mysql_num_rows(result);
    
    if (num_rows == 0) {
        mysql_free_result(result);
        pthread_mutex_unlock(&mutex_mysql);
        strcpy(reponse, "LOGIN#ko#Patient inexistant\r\n");
        return false;
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int found_id = atoi(row[0]);
    mysql_free_result(result);
    pthread_mutex_unlock(&mutex_mysql);
    
    sprintf(reponse, "LOGIN#ok#%d\r\n", found_id);
    CBP_SetLoggedIn(socket, found_id);
    return true;
}
}

/**
 * Traite une requete GET_SPECIALTIES
 */
void CBP_GetSpecialties(char* reponse) {
    if (!connexion) {
        strcpy(reponse, "SPECIALTIES#ko#Erreur base de donnees");
        return;
    }
    
    char query[] = "SELECT id, name FROM specialties ORDER BY name";
    
    if (mysql_query(connexion, query)) {
        strcpy(reponse, "SPECIALTIES#ko#Erreur requete");
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(connexion);
    MYSQL_ROW row;
    
    strcpy(reponse, "SPECIALTIES#ok");
    
    while ((row = mysql_fetch_row(result))) {
        strcat(reponse, "#");
        strcat(reponse, row[0]); // id
        strcat(reponse, "#");
        strcat(reponse, row[1]); // name
    }
    
    mysql_free_result(result);
}

/**
 * Traite une requete GET_DOCTORS
 */
void CBP_GetDoctors(char* reponse) {
    if (!connexion) {
        strcpy(reponse, "DOCTORS#ko#Erreur base de donnees");
        return;
    }
    
    char query[] = "SELECT d.id, d.last_name, d.first_name, s.name "
                   "FROM doctors d JOIN specialties s ON d.specialty_id = s.id "
                   "ORDER BY d.last_name";
    
    if (mysql_query(connexion, query)) {
        strcpy(reponse, "DOCTORS#ko#Erreur requete");
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(connexion);
    MYSQL_ROW row;
    
    strcpy(reponse, "DOCTORS#ok");
    
    while ((row = mysql_fetch_row(result))) {
        strcat(reponse, "#");
        strcat(reponse, row[0]); // id
        strcat(reponse, "#");
        strcat(reponse, row[1]); // last_name
        strcat(reponse, "#");
        strcat(reponse, row[2]); // first_name
        strcat(reponse, "#");
        strcat(reponse, row[3]); // specialty name
    }
    
    mysql_free_result(result);
}

/**
 * Traite une requete SEARCH_CONSULTATIONS
 */
void CBP_SearchConsultations(const char* specialty, const char* doctor, const char* start_date, const char* end_date, char* reponse) {
    if (!connexion) {
        strcpy(reponse, "CONSULTATIONS#ko#Erreur base de donnees");
        return;
    }
    
    char query[1000];
    sprintf(query, 
        "SELECT c.id, d.last_name, d.first_name, s.name, c.date, c.hour "
        "FROM consultations c "
        "JOIN doctors d ON c.doctor_id = d.id "
        "JOIN specialties s ON d.specialty_id = s.id "
        "WHERE c.patient_id IS NULL "
        "AND c.date >= '%s' AND c.date <= '%s'", 
        start_date, end_date);
    
    // 🔍 DEBUG: Afficher les paramètres reçus
    printf("🟡 SEARCH PARAMS: specialty='%s', doctor='%s', start='%s', end='%s'\n", 
           specialty, doctor, start_date, end_date);
    
    // Ajouter filtres optionnels - style prof avec wildcard "*"
    if (strlen(specialty) > 0 && strcmp(specialty, "*") != 0) {
        strcat(query, " AND s.name LIKE '%");
        strcat(query, specialty);
        strcat(query, "%'");
    }

    if (strlen(doctor) > 0 && strcmp(doctor, "*") != 0) {
        strcat(query, " AND (d.last_name LIKE '%");
        strcat(query, doctor);
        strcat(query, "%' OR d.first_name LIKE '%");
        strcat(query, doctor);
        strcat(query, "%')");
    }
    
    strcat(query, " ORDER BY c.date, c.hour");
    
    // 🔍 DEBUG: Afficher la requête SQL complète
    printf("🟡 SQL QUERY: %s\n", query);
    
    if (mysql_query(connexion, query)) {
        printf("MYSQL ERROR: %s\n", mysql_error(connexion));
        strcpy(reponse, "CONSULTATIONS#ko#Erreur requete");
        return;
    }
    
    MYSQL_RES* result = mysql_store_result(connexion);
    MYSQL_ROW row;
    
    strcpy(reponse, "CONSULTATIONS#ok");
    
    int count = 0;
    while ((row = mysql_fetch_row(result))) {
        count++;
        printf("🟢 ROW %d: id=%s, doctor=%s %s, specialty=%s, date=%s, hour=%s\n", 
               count, row[0], row[1], row[2], row[3], row[4], row[5]);
        
        strcat(reponse, "#");
        strcat(reponse, row[0]); // consultation id
        strcat(reponse, "#");
        strcat(reponse, row[1]); // doctor last_name
        strcat(reponse, "#");
        strcat(reponse, row[2]); // doctor first_name
        strcat(reponse, "#");
        strcat(reponse, row[3]); // specialty name
        strcat(reponse, "#");
        strcat(reponse, row[4]); // date
        strcat(reponse, "#");
        strcat(reponse, row[5]); // hour
    }
    
    printf("🟡 TOTAL ROWS: %d\n", count);
    mysql_free_result(result);
}

/**
 * Traite une requete BOOK_CONSULTATION
 */
bool CBP_BookConsultation(int consultation_id, const char* reason, int patient_id, char* reponse) {
    if (!connexion) {
        strcpy(reponse, "BOOK#ko#Erreur base de donnees");
        return false;
    }
    
    char query[500];
    
    // Verifier que la consultation est libre
    sprintf(query, "SELECT patient_id FROM consultations WHERE id=%d", consultation_id);
    
    if (mysql_query(connexion, query)) {
        strcpy(reponse, "BOOK#ko#Erreur requete");
        return false;
    }
    
    MYSQL_RES* result = mysql_store_result(connexion);
    MYSQL_ROW row = mysql_fetch_row(result);
    
    if (!row) {
        mysql_free_result(result);
        strcpy(reponse, "BOOK#ko#Consultation inexistante");
        return false;
    }
    
    if (row[0] != NULL) {
        mysql_free_result(result);
        strcpy(reponse, "BOOK#ko#Consultation deja reservee");
        return false;
    }
    
    mysql_free_result(result);
    
    // Reserver la consultation
    sprintf(query, "UPDATE consultations SET patient_id=%d, reason='%s' WHERE id=%d", 
            patient_id, reason, consultation_id);
    
    if (mysql_query(connexion, query)) {
        strcpy(reponse, "BOOK#ko#Erreur reservation");
        return false;
    }
    
    strcpy(reponse, "BOOK#ok");
    return true;
}

/**
 * Initialise la connexion a la base de donnees
 */
bool CBP_InitDB() {
    connexion = mysql_init(NULL);
    if (!connexion) {
        return false;
    }
    
    if (!mysql_real_connect(connexion, "localhost", "Student", "PassStudent1_", "PourStudent", 0, NULL, 0)) {
        mysql_close(connexion);
        connexion = NULL;
        return false;
    }
    
    return true;
}

/**
 * Ferme la connexion a la base de donnees
 */
void CBP_CloseDB() {
    if (connexion) {
        mysql_close(connexion);
        connexion = NULL;
    }
}

/**
 * Gestion des sessions clients
 */
bool CBP_IsLoggedIn(int socket) {
    pthread_mutex_lock(&mutex_clients);
    int index = find_client_index(socket);
    pthread_mutex_unlock(&mutex_clients);
    return (index != -1);
}

int CBP_GetPatientId(int socket) {
    pthread_mutex_lock(&mutex_clients);
    int index = find_client_index(socket);
    int patient_id = (index != -1) ? clients_patient_id[index] : -1;
    pthread_mutex_unlock(&mutex_clients);
    return patient_id;
}

void CBP_SetLoggedIn(int socket, int patient_id) {
    pthread_mutex_lock(&mutex_clients);
    add_client(socket, patient_id);
    pthread_mutex_unlock(&mutex_clients);
}

void CBP_SetLoggedOut(int socket) {
    pthread_mutex_lock(&mutex_clients);
    remove_client(socket);
    pthread_mutex_unlock(&mutex_clients);
}

// Fonctions utilitaires privees
static int find_client_index(int socket) {
    for (int i = 0; i < nb_clients; i++) {
        if (clients_logged[i] == socket) {
            return i;
        }
    }
    return -1;
}

static void add_client(int socket, int patient_id) {
    if (nb_clients < MAX_CLIENTS) {
        clients_logged[nb_clients] = socket;
        clients_patient_id[nb_clients] = patient_id;
        nb_clients++;
    }
}

static void remove_client(int socket) {
    int index = find_client_index(socket);
    if (index != -1) {
        for (int i = index; i < nb_clients - 1; i++) {
            clients_logged[i] = clients_logged[i + 1];
            clients_patient_id[i] = clients_patient_id[i + 1];
        }
        nb_clients--;
    }
}