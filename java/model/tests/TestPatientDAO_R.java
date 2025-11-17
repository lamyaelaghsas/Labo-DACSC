package model.tests;

import model.dao.PatientDAO;
import model.entity.Patient;
import model.viewmodel.PatientSearchVM;
import java.sql.Date;
import java.util.ArrayList;

public class TestPatientDAO_R {
    public static void main(String[] args) {
        PatientDAO dao = PatientDAO.getInstance();

        System.out.println("--- TOUS ---");
        ArrayList<Patient> patients = dao.load();
        for (Patient p : patients) {
            System.out.println(p);
        }

        System.out.println("--- Selection par id (1) ---");
        PatientSearchVM psvm = new PatientSearchVM();
        psvm.setId(1);
        patients = dao.load(psvm);
        for (Patient p : patients) {
            System.out.println(p);
        }

        System.out.println("--- Selection sur le lastName (Dupont) ---");
        psvm.setId(null);
        psvm.setLastName("Dupont");
        patients = dao.load(psvm);
        for (Patient p : patients) {
            System.out.println(p);
        }

        System.out.println("--- Selection sur le firstName (Marie) ---");
        PatientSearchVM psvm2 = new PatientSearchVM();
        psvm2.setFirstName("Marie");
        patients = dao.load(psvm2);
        for (Patient p : patients) {
            System.out.println(p);
        }

        System.out.println("--- Selection sur birthDate (1985-03-15) ---");
        PatientSearchVM psvm3 = new PatientSearchVM();
        psvm3.setBirthDate(Date.valueOf("1985-03-15"));
        patients = dao.load(psvm3);
        for (Patient p : patients) {
            System.out.println(p);
        }
    }
}