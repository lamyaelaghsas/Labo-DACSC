#ifndef CBP_H
#define CBP_H

#include <string>
#include <vector>
using namespace std;

// Structure pour représenter un client connecté
typedef struct {
    int socket;
    string ip;
    string nom;
    string prenom;
    int patientId;
    bool isLogged;
} CLIENT_CBP;

// Structure pour les spécialités
typedef struct {
    int id;
    string name;
} SPECIALTY_CBP;

// Structure pour les médecins
typedef struct {
    int id;
    int specialty_id;
    string lastName;
    string firstName;
} DOCTOR_CBP;

// Structure pour les consultations
typedef struct {
    int id;
    int doctor_id;
    string doctorName;
    string specialtyName;
    string date;
    string hour;
    bool isAvailable;  // true si patient_id == NULL
} CONSULTATION_CBP;

// Fonctions du protocole CBP
bool CBP_TraiterRequete(char* requete, char* reponse, int socket);

// Fonctions spécifiques aux commandes
bool CBP_Login(const string& nom, const string& prenom, int patientId, bool nouveauPatient, int socket);
void CBP_Logout(int socket);
string CBP_GetSpecialties();
string CBP_GetDoctors();
string CBP_SearchConsultations(const string& specialty, const string& doctor, const string& dateDebut, const string& dateFin);
bool CBP_BookConsultation(int consultationId, const string& reason, int socket);

// Gestion des clients connectés
void CBP_AjouterClient(int socket, const string& ip);
void CBP_RetirerClient(int socket);
CLIENT_CBP* CBP_TrouverClient(int socket);

// Fonctions utilitaires
vector<string> CBP_Split(const string& str, char delimiter);
void CBP_Close();

#endif