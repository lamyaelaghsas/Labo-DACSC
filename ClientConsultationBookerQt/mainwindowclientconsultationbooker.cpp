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

    sClient = -1; //pour la connexion au serveur

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
    setStartDate("2025-10-01");
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

void MainWindowClientConsultationBooker::Echange(char* requete, char* reponse) {
    int nbEcrits, nbLus;
    
    // Envoi de la requete
    if ((nbEcrits = Send(sClient, requete, strlen(requete))) == -1) {
        strcpy(reponse, "ERROR#Erreur Send");
        return;
    }
    
    printf("[CLIENT] Envoi requete: %s\n", requete);
    
    // Reception de la reponse
    if ((nbLus = Receive(sClient, reponse)) < 0) {
        strcpy(reponse, "ERROR#Erreur Receive");
        return;
    }
    
    if (nbLus == 0) {
        strcpy(reponse, "ERROR#Serveur arrete");
        return;
    }
    
    reponse[nbLus] = '\0';
    
    printf("[CLIENT] Reponse recue: %s\n", reponse);
}


void MainWindowClientConsultationBooker::on_pushButtonLogin_clicked()
{
    string lastName = this->getLastName();
    string firstName = this->getFirstName();
    int patientId = this->getPatientId();
    bool newPatient = this->isNewPatientSelected();

    // Connexion au serveur
    sClient = ClientSocket((char*)"127.0.0.1", 50000);
    if (sClient == -1) {
        dialogError("Erreur", "Impossible de se connecter au serveur");
        return;
    }
    
    // Requête LOGIN
    char requete[500], reponse[500];
    sprintf(requete, "LOGIN#%s#%s#%d#%s", 
            lastName.c_str(), firstName.c_str(), patientId, 
            newPatient ? "true" : "false");
    
    Echange(requete, reponse);
    
    // Parsing de la réponse  
    char *ptr = strtok(reponse, "#"); // entête = LOGIN
    ptr = strtok(NULL, "#"); // statut = ok ou ko
    
    if (strcmp(ptr, "ok") == 0) {
        loginOk();
        
        // CHARGER SPECIALITES
        strcpy(requete, "GET_SPECIALTIES");
        Echange(requete, reponse);
        
        // Parser et remplir combobox spécialités
        clearComboBoxSpecialties();
        ptr = strtok(reponse, "#"); // SPECIALTIES
        ptr = strtok(NULL, "#"); // ok/ko
        if (strcmp(ptr, "ok") == 0) {
            while ((ptr = strtok(NULL, "#")) != NULL) { // id
                char* nom = strtok(NULL, "#"); // nom
                if (nom != NULL) {
                    addComboBoxSpecialties(string(nom));
                }
            }
        }
        
        // CHARGER MEDECINS
        strcpy(requete, "GET_DOCTORS");
        Echange(requete, reponse);
        
        // Parser et remplir combobox médecins
        clearComboBoxDoctors();
        ptr = strtok(reponse, "#"); // DOCTORS
        ptr = strtok(NULL, "#"); // ok/ko
        if (strcmp(ptr, "ok") == 0) {
            while ((ptr = strtok(NULL, "#")) != NULL) { // id
                char* nom = strtok(NULL, "#"); // nom
                char* prenom = strtok(NULL, "#"); // prenom
                char* specialite = strtok(NULL, "#"); // specialite
                if (nom != NULL && prenom != NULL) {
                    string nomComplet = string(nom) + " " + string(prenom);
                    addComboBoxDoctors(nomComplet);
                }
            }
        }
        
    } else {
        char* raison = strtok(NULL, "#"); // raison du ko
        string msgErreur = raison ? string(raison) : "Échec authentification";
        dialogError("Erreur de login", msgErreur);
        ::close(sClient);
        sClient = -1;
    }
}

void MainWindowClientConsultationBooker::on_pushButtonLogout_clicked()
{
    logoutOk();
}

void MainWindowClientConsultationBooker::on_pushButtonRechercher_clicked()
{
    string specialty = this->getSelectionSpecialty();
    string doctor = this->getSelectionDoctor();
    string startDate = this->getStartDate();
    string endDate = this->getEndDate();

    // Vider le tableau
    clearTableConsultations();
    
    // Construction requête avec paramètres obligatoires  
    char requete[1000], reponse[2000];
    
    // Transformation selon sélection
    const char* specialtyParam = (specialty == "--- TOUTES ---") ? "*" : specialty.c_str();
    
    // Extraire seulement le nom de famille du médecin
    string doctorForSearch;
    if (doctor == "--- TOUS ---") {
        doctorForSearch = "*";
    } else {
        // Prendre seulement le premier mot (nom de famille)
        size_t pos = doctor.find(' ');
        if (pos != string::npos) {
            doctorForSearch = doctor.substr(0, pos);  // "Maboul Paul" -> "Maboul"
        } else {
            doctorForSearch = doctor;
        }
    }
    
    // Construction requête  
    sprintf(requete, "SEARCH_CONSULTATIONS#%s#%s#%s#%s", 
            specialtyParam, doctorForSearch.c_str(), startDate.c_str(), endDate.c_str());
    
    // Echange  
    Echange(requete, reponse);
    
    // Parsing réponse  
    char *ptr = strtok(reponse, "#"); // entête = CONSULTATIONS
    ptr = strtok(NULL, "#"); // statut = ok ou ko
    
    if (strcmp(ptr, "ok") == 0) {
        int nbConsultations = 0;
        
        // Parcourir les données
        while ((ptr = strtok(NULL, "#")) != NULL) { // id consultation
            char* doctorLastName = strtok(NULL, "#");
            char* doctorFirstName = strtok(NULL, "#");
            char* specialtyName = strtok(NULL, "#");
            char* date = strtok(NULL, "#");
            char* hour = strtok(NULL, "#");
            
            if (doctorLastName && doctorFirstName && specialtyName && date && hour) {
                string doctorName = string(doctorLastName) + " " + string(doctorFirstName);
                
                addTupleTableConsultations(
                    atoi(ptr),              // ID
                    string(specialtyName),  // Spécialité
                    doctorName,             // Médecin
                    string(date),           // Date
                    string(hour)            // Heure
                );
                nbConsultations++;
            }
        }
        
        // Message résultat
        if (nbConsultations == 0) {
            dialogMessage("Recherche", "Aucune consultation trouvée");
        } else {
            char msg[100];
            sprintf(msg, "%d consultation(s) trouvée(s)", nbConsultations);
            dialogMessage("Recherche", string(msg));
        }
        
    } else {
        char* raison = strtok(NULL, "#");
        string msgErreur = raison ? string(raison) : "Erreur recherche";
        dialogError("Erreur", msgErreur);
    }
}

void MainWindowClientConsultationBooker::on_pushButtonReserver_clicked()
{
    // Récupérer la ligne sélectionnée
    int selectedRow = this->getSelectionIndexTableConsultations();
    
    if (selectedRow == -1) {
        dialogError("Erreur", "Veuillez sélectionner une consultation à réserver");
        return;
    }
    
    // Récupérer l'ID de la consultation depuis le tableau
    QTableWidgetItem* item = ui->tableWidgetConsultations->item(selectedRow, 0); // Colonne ID
    if (!item) {
        dialogError("Erreur", "Impossible de récupérer l'ID de la consultation");
        return;
    }
    
    int consultationId = item->text().toInt();
    
    // Demander la raison de la consultation  
    string reason = dialogInputText("Raison de la consultation", "Entrez la raison de votre consultation:");
    
    if (reason.empty()) {
        dialogMessage("Annulation", "Réservation annulée");
        return;
    }
    
    // Construction requête  
    char requete[500], reponse[500];
    sprintf(requete, "BOOK_CONSULTATION#%d#%s", consultationId, reason.c_str());
    
    // Echange  
    Echange(requete, reponse);
    
    // Parsing réponse  
    char *ptr = strtok(reponse, "#"); // entête = BOOK
    ptr = strtok(NULL, "#"); // statut = ok ou ko
    
    if (strcmp(ptr, "ok") == 0) {
        dialogMessage("Réservation", "Consultation réservée avec succès !");
        
        // Actualiser la recherche pour voir que la consultation a disparu
        on_pushButtonRechercher_clicked();
        
    } else {
        char* raison = strtok(NULL, "#"); // raison du ko
        string msgErreur = raison ? string(raison) : "Erreur de réservation";
        dialogError("Erreur de réservation", msgErreur);
    }
}


