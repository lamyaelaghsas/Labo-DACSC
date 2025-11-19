package model.tests;

import model.dao.PatientDAO;
import model.entity.Patient;

public class TestPatientDAO_C {

    public static void main(String[] args) {

        PatientDAO dao = PatientDAO.getInstance();

        Patient patient = new Patient();  //Creation d'un nouveau patient vide
        patient.setId(null); //id = null car on veut créer un nouveau patient et non modifier un existant
        patient.setLastName("Dupont");
        patient.setFirstName("Marie");

        dao.save(patient);
        int id = patient.getId(); //on recup l'id généré par la bd
        System.out.println("Ajout réussi --> id = " + id);

        System.out.println("--- SELECT BY ID ---");
        dao.load();
        Patient patient2 = dao.getById(id);
        System.out.println(patient2);
    }
}