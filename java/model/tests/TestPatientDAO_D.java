package model.tests;

import model.dao.PatientDAO;
import model.entity.Patient;
import model.viewmodel.PatientSearchVM;
import java.util.ArrayList;

public class TestPatientDAO_D {
    public static void main(String[] args) {
        PatientDAO dao = PatientDAO.getInstance();

        // Récupération en BD et suppression
        PatientSearchVM psvm = new PatientSearchVM();
        psvm.setId(9);
        ArrayList<Patient> patients = dao.load(psvm);

        if (patients.size() != 0) {
            Patient toDelete = patients.get(0);
            dao.delete(toDelete);
            System.out.println("Delete réussi.");
        } else {
            System.out.println("Patient non présent en BD !");
        }

        // Suppression par id
        dao.delete(10);

        System.out.println("--- SELECT ALL ---");
        patients = dao.load();
        for (Patient p : patients) {
            System.out.println(p);
        }
    }
}