#include "mainwindowclientconsultationbooker.h"
#include "ui_mainwindowclientconsultationbooker.h"
#include <QInputDialog>
#include <QMessageBox>
#include <iostream>
#include <unistd.h>
using namespace std;

MainWindowClientConsultationBooker::MainWindowClientConsultationBooker(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClientConsultationBooker)
{
    ui->setupUi(this);
    logoutOk();
    nbConsultations = 0;
    // Configuration de la table des employes (Personnel Garage)
    ui->tableWidgetConsultations->setColumnCount(5);
    ui->tableWidgetConsultations->setRowCount(0);
    QStringList labelsTableConsultations;
    labelsTableConsultations << "Id" << "Spécialité" << "Médecin" << "Date" << "Heure";
    ui->tableWidgetConsultations->setHorizontalHeaderLabels(labelsTableConsultations);
    ui->tableWidgetConsultations->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetConsultations->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetConsultations->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetConsultations->horizontalHeader()->setVisible(true);
    ui->tableWidgetConsultations->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetConsultations->verticalHeader()->setVisible(false);
    ui->tableWidgetConsultations->horizontalHeader()->setStyleSheet("background-color: lightyellow");
    int columnWidths[] = {40, 150, 200, 150, 100};
    for (int col = 0; col < 5; ++col)
        ui->tableWidgetConsultations->setColumnWidth(col, columnWidths[col]);

    // Exemples d'utilisation (à supprimer)
    // this->addTupleTableConsultations(1,"Neurologie","Martin Claire","2025-10-01", "09:00");
    // this->addTupleTableConsultations(2,"Cardiologie","Lemoine Bernard","2025-10-06", "10:15");
    // this->addTupleTableConsultations(3,"Dermatologie","Maboul Paul","2025-10-23", "14:30");

    // this->addComboBoxSpecialties("--- TOUTES ---");
    // this->addComboBoxSpecialties("Dermatologie");
    // this->addComboBoxSpecialties("Cardiologie");

    // this->addComboBoxDoctors("--- TOUS ---");
    // this->addComboBoxDoctors("Martin Claire");
    // this->addComboBoxDoctors("Maboul Paul");
}

MainWindowClientConsultationBooker::~MainWindowClientConsultationBooker()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions utiles Table des livres encodés (ne pas modifier) ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowClientConsultationBooker::addTupleTableConsultations(int id,
                                                                    string specialty,
                                                                    string doctor,
                                                                    string date,
                                                                    string hour)
{
    int nb = ui->tableWidgetConsultations->rowCount();
    nb++;
    ui->tableWidgetConsultations->setRowCount(nb);
    ui->tableWidgetConsultations->setRowHeight(nb-1,10);

    // id
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(QString::number(id));
    ui->tableWidgetConsultations->setItem(nb-1,0,item);

    // specialty
    item = new QTableWidgetItem;
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(QString::fromStdString(specialty));
    ui->tableWidgetConsultations->setItem(nb-1,1,item);

    // doctor
    item = new QTableWidgetItem;
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(QString::fromStdString(doctor));
    ui->tableWidgetConsultations->setItem(nb-1,2,item);

    // date
    item = new QTableWidgetItem;
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(QString::fromStdString(date));
    ui->tableWidgetConsultations->setItem(nb-1,3,item);

    // hour
    item = new QTableWidgetItem;
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(QString::fromStdString(hour));
    ui->tableWidgetConsultations->setItem(nb-1,4,item);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowClientConsultationBooker::clearTableConsultations() {
    ui->tableWidgetConsultations->setRowCount(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int MainWindowClientConsultationBooker::getSelectionIndexTableConsultations() const
{
    QModelIndexList list = ui->tableWidgetConsultations->selectionModel()->selectedRows();
    if (list.size() == 0) return -1;
    QModelIndex index = list.at(0);
    int ind = index.row();
    return ind;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions utiles des comboboxes (ne pas modifier) //////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowClientConsultationBooker::addComboBoxSpecialties(string specialty) {
    ui->comboBoxSpecialties->addItem(QString::fromStdString(specialty));
}

string MainWindowClientConsultationBooker::getSelectionSpecialty() const {
    return ui->comboBoxSpecialties->currentText().toStdString();
}

void MainWindowClientConsultationBooker::clearComboBoxSpecialties() {
    ui->comboBoxSpecialties->clear();
    this->addComboBoxSpecialties("--- TOUTES ---");
}

void MainWindowClientConsultationBooker::addComboBoxDoctors(string doctor) {
    ui->comboBoxDoctors->addItem(QString::fromStdString(doctor));
}

string MainWindowClientConsultationBooker::getSelectionDoctor() const {
    return ui->comboBoxDoctors->currentText().toStdString();
}

void MainWindowClientConsultationBooker::clearComboBoxDoctors() {
    ui->comboBoxDoctors->clear();
    this->addComboBoxDoctors("--- TOUS ---");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonction utiles de la fenêtre (ne pas modifier) ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
string MainWindowClientConsultationBooker::getLastName() const {
    return ui->lineEditLastName->text().toStdString();
}

string MainWindowClientConsultationBooker::getFirstName() const {
    return ui->lineEditFirstName->text().toStdString();
}

int MainWindowClientConsultationBooker::getPatientId() const {
    return ui->spinBoxId->value();
}

void MainWindowClientConsultationBooker::setLastName(string value) {
    ui->lineEditLastName->setText(QString::fromStdString(value));
}

string MainWindowClientConsultationBooker::getStartDate() const {
    return ui->dateEditStartDate->date().toString("yyyy-MM-dd").toStdString();
}

string MainWindowClientConsultationBooker::getEndDate() const {
    return ui->dateEditEndDate->date().toString("yyyy-MM-dd").toStdString();
}

void MainWindowClientConsultationBooker::setFirstName(string value) {
    ui->lineEditFirstName->setText(QString::fromStdString(value));
}

void MainWindowClientConsultationBooker::setPatientId(int value) {
    if (value > 0) ui->spinBoxId->setValue(value);
}

bool MainWindowClientConsultationBooker::isNewPatientSelected() const {
    return ui->checkBoxNewPatient->isChecked();
}

void MainWindowClientConsultationBooker::setNewPatientChecked(bool state) {
    ui->checkBoxNewPatient->setChecked(state);
}

void MainWindowClientConsultationBooker::setStartDate(string date) {
    QDate qdate = QDate::fromString(QString::fromStdString(date), "yyyy-MM-dd");
    if (qdate.isValid()) ui->dateEditStartDate->setDate(qdate);
}

void MainWindowClientConsultationBooker::setEndDate(string date) {
    QDate qdate = QDate::fromString(QString::fromStdString(date), "yyyy-MM-dd");
    if (qdate.isValid()) ui->dateEditEndDate->setDate(qdate);
}

void MainWindowClientConsultationBooker::loginOk() {
    ui->lineEditLastName->setReadOnly(true);
    ui->lineEditFirstName->setReadOnly(true);
    ui->spinBoxId->setReadOnly(true);
    ui->checkBoxNewPatient->setEnabled(false);
    ui->pushButtonLogout->setEnabled(true);
    ui->pushButtonLogin->setEnabled(false);
    ui->pushButtonRechercher->setEnabled(true);
    ui->pushButtonReserver->setEnabled(true);
}

void MainWindowClientConsultationBooker::logoutOk() {
    ui->lineEditLastName->setReadOnly(false);
    setLastName("");
    ui->lineEditFirstName->setReadOnly(false);
    setFirstName("");
    ui->spinBoxId->setReadOnly(false);
    setPatientId(1);
    ui->checkBoxNewPatient->setEnabled(true);
    setNewPatientChecked(false);
    ui->pushButtonLogout->setEnabled(false);
    ui->pushButtonLogin->setEnabled(true);
    ui->pushButtonRechercher->setEnabled(false);
    ui->pushButtonReserver->setEnabled(false);
    setStartDate("2025-09-15");
    setEndDate("2025-12-31");
    clearComboBoxDoctors();
    clearComboBoxSpecialties();
    clearTableConsultations();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions permettant d'afficher des boites de dialogue (ne pas modifier) ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowClientConsultationBooker::dialogMessage(const string& title,const string& message) {
   QMessageBox::information(this,QString::fromStdString(title),QString::fromStdString(message));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowClientConsultationBooker::dialogError(const string& title,const string& message) {
   QMessageBox::critical(this,QString::fromStdString(title),QString::fromStdString(message));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
string MainWindowClientConsultationBooker::dialogInputText(const string& title,const string& question) {
    return QInputDialog::getText(this,QString::fromStdString(title),QString::fromStdString(question)).toStdString();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int MainWindowClientConsultationBooker::dialogInputInt(const string& title,const string& question) {
    return QInputDialog::getInt(this,QString::fromStdString(title),QString::fromStdString(question));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions gestion des boutons (TO DO) //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowClientConsultationBooker::on_pushButtonLogin_clicked()
{
    string lastName = this->getLastName();
    string firstName = this->getFirstName();
    int patientId = this->getPatientId();
    bool newPatient = this->isNewPatientSelected();

    cout << "lastName = " << lastName << endl;
    cout << "FirstName = " << firstName << endl;
    cout << "patientId = " << patientId << endl;
    cout << "newPatient = " << newPatient << endl;

    // Connexion sur le serveur
    if ((sClient = ClientSocket("127.0.0.1", 50000)) == -1)
    {
        dialogError("Erreur","Erreur de ClientSocket");
        return;
    }

    // Phase de login
    if (!CBP_Login(lastName.c_str(), firstName.c_str(), patientId, newPatient))
    {
        dialogError("Erreur", "Login incorect");
        ::close(sClient);
        return;
    }

    CBP_GetSpecialties();
    CBP_GetDoctors();
    
    loginOk();
}

void MainWindowClientConsultationBooker::on_pushButtonLogout_clicked()
{
    CBP_Logout();

    logoutOk();
}

void MainWindowClientConsultationBooker::on_pushButtonRechercher_clicked()
{
    string specialty = this->getSelectionSpecialty();
    string doctor = this->getSelectionDoctor();
    string startDate = this->getStartDate();
    string endDate = this->getEndDate();

    cout << "specialty = " << specialty << endl;
    cout << "doctor = " << doctor << endl;
    cout << "startDate = " << startDate << endl;
    cout << "endDate = " << endDate << endl;

    CBP_SearchConsultations(specialty.c_str(), doctor.c_str(), startDate.c_str(), endDate.c_str());
}

void MainWindowClientConsultationBooker::on_pushButtonReserver_clicked()
{
    int selectedRow = this->getSelectionIndexTableConsultations();
    cout << "selectedRow = " << selectedRow << endl;

    // Vérification simple
    if (selectedRow < 0 || selectedRow >= nbConsultations) {
        dialogError("Erreur", "Sélection invalide");
        return;
    }

    string reason = dialogInputText("Reservation", "Motif de consultation :");

    if (reason.empty()) {
        dialogError("Erreur", "Le motif est obligatoire");
        return;
    }

    // Récupérer l'ID réel à partir du tableau
    int consultationId = consultationIds[selectedRow];

    if (CBP_BookConsultation(consultationId, reason.c_str())) {
        dialogMessage("Succès", "Consultation réservée avec succès");
        on_pushButtonRechercher_clicked();
    }
}


//***** Gestion du protocole CBP ***********************************
bool MainWindowClientConsultationBooker::CBP_Login(const char* nom, const char* prenom, int noPatient, bool nouveauPatient)
{
    char requete[200], reponse[200];
    bool onContinue = true;
    
    // ***** Construction de la requete *********************
    if (nouveauPatient)
        sprintf(requete, "LOGIN#%s#%s#%d#true", nom, prenom, noPatient);
    else
        sprintf(requete, "LOGIN#%s#%s#%d#false", nom, prenom, noPatient);
    
    // ***** Envoi requete + réception réponse **************
    Echange(requete, reponse);
    
    // ***** Parsing de la réponse **************************
    char *ptr = strtok(reponse, "#"); // entête = LOGIN (normalement...)
    ptr = strtok(NULL, "#"); // statut = oui ou non
    
    if (strcmp(ptr, "Oui") == 0)
    {
        if (nouveauPatient)
        {
            ptr = strtok(NULL, "#");
            setPatientId(atoi(ptr));
        }
    }
    else
    {
        ptr = strtok(NULL,"#"); // raison du non
        printf("Erreur de login: %s\n",ptr);
        onContinue = false;
    }
    
    return onContinue;
}

void MainWindowClientConsultationBooker::CBP_Logout()
{
    char requete[200],reponse[200];

    // ***** Construction de la requete *********************
    sprintf(requete,"LOGOUT");

    // ***** Envoi requete + réception réponse **************
    Echange(requete,reponse);

    ::close(sClient);

}

void MainWindowClientConsultationBooker::CBP_GetSpecialties()
{
    char requete[200], reponse[2000];
    
    // ***** Construction de la requete *********************
    sprintf(requete, "GET_SPECIALTIES");

    // ***** Envoi requete + réception réponse **************
    Echange(requete, reponse);
    
    // ***** Parsing de la réponse **************************
    char *ptr = strtok(reponse, "#");
    ptr = strtok(NULL, "#");
    if (ptr != NULL && strcmp(ptr, "ko") == 0)
    {
        ptr = strtok(NULL, "#");
        if (ptr != NULL)
        {
            dialogError("Erreur", ptr);
        }
        return;
    }
    clearComboBoxSpecialties();
    
    while (ptr != NULL)
    {
        char id[50];
        strcpy(id, ptr);
        
        ptr = strtok(NULL, "#");
        if (ptr != NULL)
        {
            char name[100];
            strcpy(name, ptr);
            addComboBoxSpecialties(name);
        }
        ptr = strtok(NULL, "#");
    }
}

void MainWindowClientConsultationBooker::CBP_GetDoctors()
{
    char requete[200], reponse[2000];
    
    // ***** Construction de la requete *********************
    sprintf(requete, "GET_DOCTORS");

    // ***** Envoi requete + réception réponse **************
    Echange(requete, reponse);
    
    // ***** Parsing de la réponse **************************
    char *ptr = strtok(reponse, "#");
    ptr = strtok(NULL, "#");
    if (ptr != NULL && strcmp(ptr, "ko") == 0)
    {
        ptr = strtok(NULL, "#");
        if (ptr != NULL)
        {
            dialogError("Erreur", ptr);
        }
        return;
    }
    clearComboBoxDoctors();
    
    while (ptr != NULL)
    {
        char id[50];
        strcpy(id, ptr);
        
        ptr = strtok(NULL, "#");
        if (ptr != NULL)
        {
            char lastName[100];
            strcpy(lastName, ptr);
            
            ptr = strtok(NULL, "#");
            if (ptr != NULL)
            {
                char firstName[100];
                strcpy(firstName, ptr);
                
                char fullName[200];
                sprintf(fullName, "%s %s", lastName, firstName);
                addComboBoxDoctors(fullName);
            }
        }
        ptr = strtok(NULL, "#");
    }
}

void MainWindowClientConsultationBooker::CBP_SearchConsultations(const char* specialty, const char* doctor, const char* startDate, const char* endDate)
{
    char requete[500], reponse[5000];
    
    // ***** Construction de la requete *********************
    sprintf(requete, "SEARCH_CONSULTATIONS#%s#%s#%s#%s", specialty, doctor, startDate, endDate);

    // ***** Envoi requete + réception réponse **************
    Echange(requete, reponse);
    
    // ***** Parsing de la réponse **************************
    char *ptr = strtok(reponse, "#");
    ptr = strtok(NULL, "#");
    if (ptr != NULL && strcmp(ptr, "ko") == 0)
    {
        ptr = strtok(NULL, "#");
        if (ptr != NULL)
        {
            dialogError("Erreur", ptr);
        }
        else
        {
            dialogError("Erreur", "Erreur lors de la recherche");
        }
        return;
    }
    clearTableConsultations();
    nbConsultations = 0;

    while (ptr != NULL)
    {
        char id[50];
        strcpy(id, ptr);
        
        ptr = strtok(NULL, "#");
        if (ptr != NULL)
        {
            char specialtyName[100];
            strcpy(specialtyName, ptr);
            
            ptr = strtok(NULL, "#");
            if (ptr != NULL)
            {
                char doctorName[100];
                strcpy(doctorName, ptr);
                
                ptr = strtok(NULL, "#");
                if (ptr != NULL)
                {
                    char date[50];
                    strcpy(date, ptr);
                    
                    ptr = strtok(NULL, "#");
                    if (ptr != NULL)
                    {
                        char hour[50];
                        strcpy(hour, ptr);
                        
                        addTupleTableConsultations(atoi(id), specialtyName, doctorName, date, hour);
                        consultationIds[nbConsultations] = atoi(id);
                        nbConsultations++;
                    }
                }
            }
        }
        ptr = strtok(NULL, "#");
    }
}

bool MainWindowClientConsultationBooker::CBP_BookConsultation(int consultationId, const char* reason)
{
    char requete[500], reponse[200];

    // ***** Construction de la requete *********************
    sprintf(requete, "BOOK_CONSULTATION#%d#%s", consultationId, reason);

    // ***** Envoi requete + réception réponse **************
    Echange(requete, reponse);
    
    // ***** Parsing de la réponse **************************
    char *ptr = strtok(reponse, "#");
    ptr = strtok(NULL, "#");
    if (ptr != NULL && strcmp(ptr, "Oui") == 0)
    {
        return true;
    }
    else
    {
        ptr = strtok(NULL, "#");
        if (ptr != NULL)
        {
            dialogError("Erreur", ptr);
        }
        else
        {
            dialogError("Erreur", "Impossible de reserver cette consultation");
        }
        return false;
    }
}




//***** Echange de données entre client et serveur ******************
void MainWindowClientConsultationBooker::Echange(char* requete, char* reponse)
{
    int nbEcrits, nbLus;
    
    // ***** Envoi de la requete ****************************
    if ((nbEcrits = Send(sClient, requete, strlen(requete))) == -1)
    {
        perror("Erreur de Send");
        ::close(sClient);
        exit(1);
    }
    
    // ***** Attente de la reponse **************************
    if ((nbLus = Receive(sClient, reponse)) < 0)
    {
        perror("Erreur de Receive");
        ::close(sClient);
        exit(1);
    }
    
    if (nbLus == 0)
    {
        printf("Serveur arrete, pas de reponse reçue...\n");
        ::close(sClient);
        exit(1);
    }
    
    reponse[nbLus] = 0;
}


