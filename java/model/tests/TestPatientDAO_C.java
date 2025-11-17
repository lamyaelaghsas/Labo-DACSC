package model.tests;

import model.dao.PatientDAO;
import model.entity.Patient;

public class TestPatientDAO_C {
    public static void main(String[] args) {
        PatientDAO dao = PatientDAO.getInstance();

        Patient patient = new Patient();
        patient.setId(null); // pour création (id n'existe pas encore)
        patient.setLastName("Dupont");
        patient.setFirstName("Marie");

        dao.save(patient);
        int id = patient.getId();
        System.out.println("Ajout réussi --> id = " + id);

        System.out.println("--- SELECT BY ID ---");
        dao.load();
        Patient patient2 = dao.getById(id);
        System.out.println(patient2);
    }
}