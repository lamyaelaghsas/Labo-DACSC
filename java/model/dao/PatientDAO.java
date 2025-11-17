package model.dao;

import database.DatabaseConnection;
import model.entity.Patient;
import model.viewmodel.PatientSearchVM;

import java.sql.*;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

public class PatientDAO {
    private static PatientDAO instance = null;
    private final DatabaseConnection connectDB;
    private final ArrayList<Patient> patients;

    private PatientDAO() {
        connectDB = DatabaseConnection.getInstance();
        patients = new ArrayList<>();
    }

    public static synchronized PatientDAO getInstance() {
        if (instance == null) {
            instance = new PatientDAO();
        }
        return instance;
    }

    public ArrayList<Patient> getList() {
        return patients;
    }

    public Patient getById(Integer id) {
        for (Patient p : patients) {
            if (p.getId().equals(id)) {
                return p;
            }
        }
        return null;
    }

    public synchronized ArrayList<Patient> load() {
        return this.load(null);
    }

    public synchronized ArrayList<Patient> load(PatientSearchVM psvm)
    {
        ArrayList<Patient> result = new ArrayList<>();

        try {
            String sql = "SELECT id, last_name, first_name, birth_date " +
                    "FROM patients ";


            if (psvm != null) {
                String where = " WHERE 1=1 ";

                if (psvm.getId() != null) {
                    where += "AND id = ? ";
                }
                if (psvm.getLastName() != null && psvm.getLastName().length() > 0) {
                    where += "AND last_name LIKE ? ";
                }
                if (psvm.getFirstName() != null && psvm.getFirstName().length() > 0) {
                    where += "AND first_name LIKE ? ";
                }
                if (psvm.getBirthDate() != null) {
                    where += "AND birth_date = ? ";
                }

                sql += where + "ORDER BY last_name, first_name;";
            }

            PreparedStatement stmt = connectDB.getConn().prepareStatement(sql);

            if (psvm != null) {
                int paramNumber = 0;

                if (psvm.getId() != null) {
                    paramNumber++;
                    stmt.setInt(paramNumber, psvm.getId());
                }
                if (psvm.getLastName() != null && psvm.getLastName().length() > 0) {
                    paramNumber++;
                    stmt.setString(paramNumber, "%" + psvm.getLastName() + "%");
                }
                if (psvm.getFirstName() != null && psvm.getFirstName().length() > 0) {
                    paramNumber++;
                    stmt.setString(paramNumber, "%" + psvm.getFirstName() + "%");
                }
                if (psvm.getBirthDate() != null) {
                    paramNumber++;
                    stmt.setDate(paramNumber, psvm.getBirthDate());
                }
            }

            ResultSet rs = stmt.executeQuery();

            while (rs.next()) {
                Integer id = rs.getInt("id");
                String lastName = rs.getString("last_name");
                String firstName = rs.getString("first_name");
                Date birthDate = rs.getDate("birth_date");

                Patient p = new Patient(id, lastName, firstName, birthDate);
                result.add(p);
            }

            stmt.close();
            patients.clear();
            patients.addAll(result);

        } catch (SQLException ex) {
            ex.printStackTrace();
        } finally {
            return result;
        }
    }

    public synchronized void save(Patient p) {
        try {
            String sql;
            if (p != null) {
                if (p.getId() != null) {
                    sql = "UPDATE patients SET " +
                            "last_name = ?, " +
                            "first_name = ?, " +
                            "birth_date = ? " +
                            "WHERE id = ?";

                    PreparedStatement pStmt = connectDB.getConn().prepareStatement(sql);
                    pStmt.setString(1, p.getLastName());
                    pStmt.setString(2, p.getFirstName());
                    pStmt.setDate(3, p.getBirthDate() != null ? Date.valueOf(p.getBirthDate().toLocalDate()) : null);
                    pStmt.setInt(4, p.getId());
                    pStmt.executeUpdate();
                    pStmt.close();
                } else {
                    sql = "INSERT INTO patients (" +
                            "last_name, " +
                            "first_name, " +
                            "birth_date) " +
                            "VALUES (?, ?, ?)";

                    PreparedStatement pStmt = connectDB.getConn().prepareStatement(sql, Statement.RETURN_GENERATED_KEYS);
                    pStmt.setString(1, p.getLastName());
                    pStmt.setString(2, p.getFirstName());
                    pStmt.setDate(3, p.getBirthDate() != null ? Date.valueOf(p.getBirthDate().toLocalDate()) : null);
                    pStmt.executeUpdate();

                    ResultSet rs = pStmt.getGeneratedKeys();
                    rs.next();
                    p.setId(rs.getInt(1));

                    rs.close();
                    pStmt.close();
                }
            }
        } catch (SQLException ex) {
            Logger.getLogger(PatientDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public synchronized void delete(Patient entity) {
        if (entity != null && entity.getId() != null) {
            this.delete(entity.getId());
        }
    }

    public synchronized void delete(Integer id) {
        if (id != null) {
            try {
                String sql = "DELETE FROM patients WHERE id = ?";
                PreparedStatement stmt = connectDB.getConn().prepareStatement(sql);
                stmt.setInt(1, id);
                stmt.executeUpdate();
                stmt.close();
            } catch (SQLException ex) {
                Logger.getLogger(PatientDAO.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
}