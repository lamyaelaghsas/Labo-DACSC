#ifndef CBP_H
#define CBP_H


#define NB_MAX_CLIENTS 100

bool CBP(char* requete, char* reponse, int socket);
bool CBP_Login(const char* nom, const char* prenom, const char* noPatient,const char* nouveauPatient, int* patientId);
char* CBP_GetSpecialties();
char* CBP_GetDoctors();
char* CBP_SearchConsultations(const char* specialite, const char* medecin, const char* dateDebut, const char* dateFin);
bool CBP_BookConsultation(const char* consultationId, const char* reason, int patientId);
void CBP_Close();

#endif