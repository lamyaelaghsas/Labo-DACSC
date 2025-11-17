package model.tests;

import model.dao.PatientDAO;
import model.entity.Patient;
import model.viewmodel.PatientSearchVM;
import java.sql.Date;
import java.util.ArrayList;

public class TestPatientDAO_U {
    public static void main(String[] args) {
        PatientDAO dao = PatientDAO.getInstance();

        // Récupération en BD
        PatientSearchVM psvm = new PatientSearchVM();
        psvm.setId(1);
        ArrayList<Patient> patients = dao.load(psvm);

        if (patients.size() != 0) {
            Patient toUpdate = patients.get(0);
            toUpdate.setLastName("Martin");
            toUpdate.setFirstName("Julie");
            toUpdate.setBirthDate(Date.valueOf("1990-07-20"));
            dao.save(toUpdate);
            System.out.println("Update réussi.");
        } else {
            System.out.println("Patient non présent en BD !");
        }

        System.out.println("--- SELECT BY ID ---");
        dao.load();
        Patient patient2 = dao.getById(1);
        System.out.println(patient2);
    }
}