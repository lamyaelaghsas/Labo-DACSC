#include "CBP.h"
#include <iostream>
#include <sstream>
#include <pthread.h>
#include <algorithm>

// Variables globales pour gérer l'état du protocole
vector<CLIENT_CBP> clients;
pthread_mutex_t mutexClients = PTHREAD_MUTEX_INITIALIZER;

// Données simulées (en attendant la BD)
vector<SPECIALTY_CBP> specialties = {
    {1, "Cardiologie"},
    {2, "Dermatologie"}, 
    {3, "Neurologie"},
    {4, "Ophtalmologie"}
};

vector<DOCTOR_CBP> doctors = {
    {1, 1, "Dupont", "Alice"},
    {2, 2, "Lemoine", "Bernard"},
    {3, 3, "Martin", "Claire"},
    {4, 2, "Maboul", "Paul"}
};

vector<CONSULTATION_CBP> consultations = {
    {1, 3, "Martin Claire", "Neurologie", "2025-10-01", "09:00", true},
    {2, 1, "Dupont Alice", "Cardiologie", "2025-10-02", "14:30", true},
    {3, 2, "Lemoine Bernard", "Dermatologie", "2025-10-03", "11:15", false}, // déjà réservée
    {4, 4, "Maboul Paul", "Dermatologie", "2025-10-04", "09:00", true},
    {5, 4, "Maboul Paul", "Dermatologie", "2025-10-04", "09:30", true}
};

// ===== FONCTION PRINCIPALE DE TRAITEMENT =====
bool CBP_TraiterRequete(char* requete, char* reponse, int socket) {
    printf("\t[THREAD %p] CBP: Traitement de '%s'\n", pthread_self(), requete);
    
    // Parsing de la requête
    vector<string> params = CBP_Split(string(requete), '#');
    
    if (params.empty()) {
        sprintf(reponse, "ERROR#Requete vide");
        return true;
    }
    
    string commande = params[0];
    
    // ===== LOGIN =====
    if (commande == "LOGIN") {
        if (params.size() != 4 && params.size() != 5) {
            sprintf(reponse, "LOGIN#ko#Mauvais nombre de parametres");
            return true;
        }
        
        string nom = params[1];
        string prenom = params[2];
        int patientId = (params[3] == "0") ? 0 : stoi(params[3]);
        bool nouveauPatient = (params.size() == 5 && params[4] == "nouveau");
        
        if (CBP_Login(nom, prenom, patientId, nouveauPatient, socket)) {
            sprintf(reponse, "LOGIN#ok");
        } else {
            sprintf(reponse, "LOGIN#ko#Echec authentification");
        }
        return true;
    }
    
    // Vérifier si client est loggé pour les autres commandes
    CLIENT_CBP* client = CBP_TrouverClient(socket);
    if (client == nullptr || !client->isLogged) {
        sprintf(reponse, "ERROR#Client non connecte");
        return true;
    }
    
    // ===== LOGOUT =====
    if (commande == "LOGOUT") {
        CBP_Logout(socket);
        sprintf(reponse, "LOGOUT#ok");
        return false; // Fin de connexion
    }
    
    // ===== GET_SPECIALTIES =====
    if (commande == "GET_SPECIALTIES") {
        string result = CBP_GetSpecialties();
        sprintf(reponse, "GET_SPECIALTIES#ok#%s", result.c_str());
        return true;
    }
    
    // ===== GET_DOCTORS =====
    if (commande == "GET_DOCTORS") {
        string result = CBP_GetDoctors();
        sprintf(reponse, "GET_DOCTORS#ok#%s", result.c_str());
        return true;
    }
    
    // ===== SEARCH_CONSULTATIONS =====
    if (commande == "SEARCH_CONSULTATIONS") {
        if (params.size() != 5) {
            sprintf(reponse, "SEARCH_CONSULTATIONS#ko#Mauvais nombre de parametres");
            return true;
        }
        
        string specialty = params[1];
        string doctor = params[2];
        string dateDebut = params[3];
        string dateFin = params[4];
        
        string result = CBP_SearchConsultations(specialty, doctor, dateDebut, dateFin);
        sprintf(reponse, "SEARCH_CONSULTATIONS#ok#%s", result.c_str());
        return true;
    }
    
    // ===== BOOK_CONSULTATION =====
    if (commande == "BOOK_CONSULTATION") {
        if (params.size() != 3) {
            sprintf(reponse, "BOOK_CONSULTATION#ko#Mauvais nombre de parametres");
            return true;
        }
        
        int consultationId = stoi(params[1]);
        string reason = params[2];
        
        if (CBP_BookConsultation(consultationId, reason, socket)) {
            sprintf(reponse, "BOOK_CONSULTATION#ok");
        } else {
            sprintf(reponse, "BOOK_CONSULTATION#ko#Consultation non disponible");
        }
        return true;
    }
    
    // Commande inconnue
    sprintf(reponse, "ERROR#Commande inconnue: %s", commande.c_str());
    return true;
}

// ===== IMPLÉMENTATION DES COMMANDES =====

bool CBP_Login(const string& nom, const string& prenom, int patientId, bool nouveauPatient, int socket) {
    pthread_mutex_lock(&mutexClients);
    
    // Chercher le client dans la liste
    for (auto& client : clients) {
        if (client.socket == socket) {
            client.nom = nom;
            client.prenom = prenom;
            client.patientId = nouveauPatient ? rand() % 1000 + 100 : patientId; // ID simulé
            client.isLogged = true;
            pthread_mutex_unlock(&mutexClients);
            printf("\t[THREAD %p] Login réussi pour %s %s (ID=%d)\n", 
                   pthread_self(), nom.c_str(), prenom.c_str(), client.patientId);
            return true;
        }
    }
    
    pthread_mutex_unlock(&mutexClients);
    return false;
}

void CBP_Logout(int socket) {
    pthread_mutex_lock(&mutexClients);
    
    for (auto& client : clients) {
        if (client.socket == socket) {
            printf("\t[THREAD %p] Logout de %s %s\n", 
                   pthread_self(), client.nom.c_str(), client.prenom.c_str());
            client.isLogged = false;
            break;
        }
    }
    
    pthread_mutex_unlock(&mutexClients);
}

string CBP_GetSpecialties() {
    string result = "";
    for (const auto& spec : specialties) {
        if (!result.empty()) result += "|";
        result += to_string(spec.id) + ":" + spec.name;
    }
    return result;
}

string CBP_GetDoctors() {
    string result = "";
    for (const auto& doc : doctors) {
        if (!result.empty()) result += "|";
        result += to_string(doc.id) + ":" + doc.lastName + " " + doc.firstName;
    }
    return result;
}

string CBP_SearchConsultations(const string& specialty, const string& doctor, 
                              const string& dateDebut, const string& dateFin) {
    string result = "";
    
    for (const auto& consult : consultations) {
        if (!consult.isAvailable) continue;  // Déjà réservée
        
        // Filtrage simple (version basique)
        bool match = true;
        if (specialty != "TOUTES" && consult.specialtyName.find(specialty) == string::npos) {
            match = false;
        }
        if (doctor != "TOUS" && consult.doctorName.find(doctor) == string::npos) {
            match = false;
        }
        
        if (match) {
            if (!result.empty()) result += "|";
            result += to_string(consult.id) + ":" + consult.specialtyName + ":" + 
                     consult.doctorName + ":" + consult.date + ":" + consult.hour;
        }
    }
    
    return result;
}

bool CBP_BookConsultation(int consultationId, const string& reason, int socket) {
    // Chercher la consultation
    for (auto& consult : consultations) {
        if (consult.id == consultationId && consult.isAvailable) {
            consult.isAvailable = false;  // Marquer comme réservée
            
            CLIENT_CBP* client = CBP_TrouverClient(socket);
            printf("\t[THREAD %p] Consultation %d réservée par %s %s (raison: %s)\n", 
                   pthread_self(), consultationId, 
                   client ? client->nom.c_str() : "?",
                   client ? client->prenom.c_str() : "?",
                   reason.c_str());
            return true;
        }
    }
    return false;
}

// ===== GESTION DES CLIENTS =====

void CBP_AjouterClient(int socket, const string& ip) {
    pthread_mutex_lock(&mutexClients);
    
    CLIENT_CBP client = {socket, ip, "", "", 0, false};
    clients.push_back(client);
    
    pthread_mutex_unlock(&mutexClients);
    printf("Client ajouté: socket=%d, IP=%s\n", socket, ip.c_str());
}

void CBP_RetirerClient(int socket) {
    pthread_mutex_lock(&mutexClients);
    
    clients.erase(
        remove_if(clients.begin(), clients.end(),
                  [socket](const CLIENT_CBP& c) { return c.socket == socket; }),
        clients.end()
    );
    
    pthread_mutex_unlock(&mutexClients);
    printf("Client retiré: socket=%d\n", socket);
}

CLIENT_CBP* CBP_TrouverClient(int socket) {
    pthread_mutex_lock(&mutexClients);
    
    for (auto& client : clients) {
        if (client.socket == socket) {
            pthread_mutex_unlock(&mutexClients);
            return &client;
        }
    }
    
    pthread_mutex_unlock(&mutexClients);
    return nullptr;
}

// ===== UTILITAIRES =====

vector<string> CBP_Split(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

void CBP_Close() {
    pthread_mutex_lock(&mutexClients);
    clients.clear();
    pthread_mutex_unlock(&mutexClients);
}