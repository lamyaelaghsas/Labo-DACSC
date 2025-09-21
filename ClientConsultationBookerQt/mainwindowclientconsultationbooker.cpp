#include "mainwindowclientconsultationbooker.h"
#include "ui_mainwindowclientconsultationbooker.h"
#include <QInputDialog>
#include <QMessageBox>
#include <iostream>
#include <unistd.h> 

#include "../SocketLib/Socket.h"

using namespace std;

int sClient = -1;

MainWindowClientConsultationBooker::MainWindowClientConsultationBooker(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClientConsultationBooker)
{
    ui->setupUi(this);
    logoutOk();

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
    this->addTupleTableConsultations(1,"Neurologie","Martin Claire","2025-10-01", "09:00");
    this->addTupleTableConsultations(2,"Cardiologie","Lemoine Bernard","2025-10-06", "10:15");
    this->addTupleTableConsultations(3,"Dermatologie","Maboul Paul","2025-10-23", "14:30");

    //this->addComboBoxSpecialties("--- TOUTES ---");
    this->addComboBoxSpecialties("Dermatologie");
    this->addComboBoxSpecialties("Cardiologie");

    //this->addComboBoxDoctors("--- TOUS ---");
    this->addComboBoxDoctors("Martin Claire");
    this->addComboBoxDoctors("Maboul Paul");
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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//mes fonctions :
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Echange(const char* requete, char* reponse) {
    int nbEcrits, nbLus;
    
    if ((nbEcrits = Send(sClient, (char*)requete, strlen(requete))) == -1) {
        strcpy(reponse, "ERROR");
        return;
    }
    
    if ((nbLus = Receive(sClient, reponse)) < 0) {
        strcpy(reponse, "ERROR");
        return;
    }
    
    reponse[nbLus] = 0;
}

bool CBP_Login(const char* nom, const char* prenom, int patientId, bool nouveauPatient) {
    char requete[200], reponse[200];
    
    if (nouveauPatient) {
        sprintf(requete, "LOGIN#%s#%s#%d#nouveau", nom, prenom, patientId);
    } else {
        sprintf(requete, "LOGIN#%s#%s#%d", nom, prenom, patientId);
    }
    
    Echange(requete, reponse);
    return (strstr(reponse, "LOGIN#ok") != NULL);
}

void CBP_Logout() {
    char requete[200], reponse[200];
    strcpy(requete, "LOGOUT");
    Echange(requete, reponse);
}

bool CBP_GetSpecialties(char* buffer) {
    char requete[200], reponse[500];
    strcpy(requete, "GET_SPECIALTIES");
    Echange(requete, reponse);
    
    if (strstr(reponse, "GET_SPECIALTIES#ok#") != NULL) {
        char* data = strstr(reponse, "GET_SPECIALTIES#ok#") + 19;
        strcpy(buffer, data);
        return true;
    }
    return false;
}

bool CBP_GetDoctors(char* buffer) {
    char requete[200], reponse[500];
    strcpy(requete, "GET_DOCTORS");
    Echange(requete, reponse);
    
    if (strstr(reponse, "GET_DOCTORS#ok#") != NULL) {
        char* data = strstr(reponse, "GET_DOCTORS#ok#") + 15;
        strcpy(buffer, data);
        return true;
    }
    return false;
}

bool CBP_SearchConsultations(const char* specialty, const char* doctor, 
                            const char* dateDebut, const char* dateFin, char* buffer) {
    char requete[500], reponse[1000];
    sprintf(requete, "SEARCH_CONSULTATIONS#%s#%s#%s#%s", specialty, doctor, dateDebut, dateFin);
    Echange(requete, reponse);
    
    if (strstr(reponse, "SEARCH_CONSULTATIONS#ok#") != NULL) {
        char* data = strstr(reponse, "SEARCH_CONSULTATIONS#ok#") + 24;
        strcpy(buffer, data);
        return true;
    }
    return false;
}

bool CBP_BookConsultation(int consultationId, const char* reason) {
    char requete[200], reponse[200];
    sprintf(requete, "BOOK_CONSULTATION#%d#%s", consultationId, reason);
    Echange(requete, reponse);
    
    return (strstr(reponse, "BOOK_CONSULTATION#ok") != NULL);
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

    cout << "Login: " << lastName << " " << firstName << endl;

    // Connexion au serveur 
    if ((sClient = ClientSocket("127.0.0.1", 50000)) == -1) {
        dialogError("Erreur", "Impossible de se connecter au serveur");
        return;
    }

    // Login CBP
    if (CBP_Login(lastName.c_str(), firstName.c_str(), patientId, newPatient)) {
        // Charger les spécialités
        char buffer[500];
        if (CBP_GetSpecialties(buffer)) {
            clearComboBoxSpecialties();
            char* token = strtok(buffer, "|");
            while (token != NULL) {
                char* colon = strchr(token, ':');
                if (colon != NULL) {
                    addComboBoxSpecialties(string(colon + 1));
                }
                token = strtok(NULL, "|");
            }
        }

        // Charger les médecins
        if (CBP_GetDoctors(buffer)) {
            clearComboBoxDoctors();
            char* token = strtok(buffer, "|");
            while (token != NULL) {
                char* colon = strchr(token, ':');
                if (colon != NULL) {
                    addComboBoxDoctors(string(colon + 1));
                }
                token = strtok(NULL, "|");
            }
        }

        loginOk();
    } else {
        dialogError("Erreur", "Échec authentification");
        ::close(sClient);
        sClient = -1;
    }
}
void MainWindowClientConsultationBooker::on_pushButtonLogout_clicked()
{
    CBP_Logout();
    if (sClient != -1) {
        ::close(sClient);
        sClient = -1;
    }
    logoutOk();
}

void MainWindowClientConsultationBooker::on_pushButtonRechercher_clicked()
{
    string specialty = this->getSelectionSpecialty();
    string doctor = this->getSelectionDoctor();
    string startDate = this->getStartDate();
    string endDate = this->getEndDate();

    cout << "Recherche: " << specialty << ", " << doctor << endl;

    clearTableConsultations();

    char buffer[2000];
    if (CBP_SearchConsultations(specialty.c_str(), doctor.c_str(), 
                               startDate.c_str(), endDate.c_str(), buffer)) {
        
        // Parser les consultations (style cours)
        char* token = strtok(buffer, "|");
        while (token != NULL) {
            // Format: id:specialty:doctor:date:hour
            char* parts[5];
            int i = 0;
            char* part = strtok(token, ":");
            while (part != NULL && i < 5) {
                parts[i++] = part;
                part = strtok(NULL, ":");
            }
            
            if (i == 5) {
                addTupleTableConsultations(atoi(parts[0]), string(parts[1]), 
                                         string(parts[2]), string(parts[3]), string(parts[4]));
            }
            
            token = strtok(NULL, "|");
        }
    }
}

void MainWindowClientConsultationBooker::on_pushButtonReserver_clicked()
{
    int selectedRow = this->getSelectionIndexTableConsultations();
    
    if (selectedRow == -1) {
        dialogError("Erreur", "Sélectionnez une consultation");
        return;
    }

    QTableWidget* table = ui->tableWidgetConsultations;
    int consultationId = table->item(selectedRow, 0)->text().toInt();

    string reason = dialogInputText("Raison", "Raison de la consultation:");
    if (reason.empty()) return;

    if (CBP_BookConsultation(consultationId, reason.c_str())) {
        dialogMessage("Succès", "Consultation réservée !");
        table->removeRow(selectedRow);
    } else {
        dialogError("Erreur", "Impossible de réserver");
    }
}


