#ifndef CLIENT_H
#define CLIENT_H

typedef struct
{
    int socket;
    int patientId;
    char nom[50];
    char prenom[50];
    char noPatient[50];
    char ip[50];
} CLIENT;

#endif
