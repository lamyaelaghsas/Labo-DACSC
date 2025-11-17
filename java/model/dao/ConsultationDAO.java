package model.dao;

import database.DatabaseConnection;
import model.entity.Consultation;
import model.viewmodel.ConsultationSearchVM;

import java.sql.*;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

public class ConsultationDAO {
    private static ConsultationDAO instance = null;
    private final DatabaseConnection connectDB;
    private final ArrayList<Consultation> consultations;

    private ConsultationDAO() {
        connectDB = DatabaseConnection.getInstance();
        consultations = new ArrayList<>();
    }

    public static synchronized ConsultationDAO getInstance() {
        if (instance == null) {
            instance = new ConsultationDAO();
        }
        return instance;
    }

    public ArrayList<Consultation> getList() {
        return consultations;
    }

    public Consultation getById(Integer id) {
        for (Consultation c : consultations) {
            if (c.getId().equals(id)) {
                return c;
            }
        }
        return null;
    }

    public synchronized ArrayList<Consultation> load() {
        return this.load(null);
    }

    public synchronized ArrayList<Consultation> load(ConsultationSearchVM csvm)
    {
        ArrayList<Consultation> result = new ArrayList<>();
        try
        {
            String sql = "SELECT " +
                    "c.id, " +
                    "c.doctor_id, " +
                    "c.patient_id, " +
                    "c.date, " +
                    "c.hour, " +
                    "c.reason, " +
                    "p.last_name AS patient_last_name, " +
                    "p.first_name AS patient_first_name, " +
                    "d.last_name AS doctor_last_name, " +
                    "d.first_name AS doctor_first_name " +
                    "FROM consultations c " +
                    "LEFT JOIN patients p ON c.patient_id = p.id " +
                    "LEFT JOIN doctors d ON c.doctor_id = d.id ";


            if (csvm != null)
            {
                String where = " WHERE 1=1 ";

                if (csvm.getId() != null)
                    where += " AND c.id = ? ";
                if (csvm.getPatientId() != null)
                    where += " AND c.patient_id = ? ";
                if (csvm.getDoctorId() != null)
                    where += " AND c.doctor_id = ? ";
                if (csvm.getDateMin() != null)
                    where += " AND DATE(c.date) >= ? ";
                if (csvm.getDateMax() != null)
                    where += " AND DATE(c.date) <= ? ";
                if (csvm.getTime() != null)
                    where += " AND c.hour = ? ";
                if (csvm.getReason() != null && csvm.getReason().length() > 0)
                    where += " AND c.reason LIKE ? ";
                if (csvm.getPatientLastName() != null && csvm.getPatientLastName().length() > 0)
                    where += " AND LOWER(p.last_name) LIKE ? ";
                if (csvm.getPatientFirstName() != null && csvm.getPatientFirstName().length() > 0)
                    where += " AND LOWER(p.first_name) LIKE ? ";

                sql += where + " ORDER BY c.date, c.hour;";
            }

            PreparedStatement stmt = connectDB.getConn().prepareStatement(sql);

            int paramNumber = 0;
            if (csvm != null)
            {
                if (csvm.getId() != null)
                {
                    paramNumber++;
                    stmt.setInt(paramNumber, csvm.getId());
                }
                if (csvm.getPatientId() != null)
                {
                    paramNumber++;
                    stmt.setInt(paramNumber, csvm.getPatientId());
                }
                if (csvm.getDoctorId() != null)
                {
                    paramNumber++;
                    stmt.setInt(paramNumber, csvm.getDoctorId());
                }
                if (csvm.getDateMin() != null)
                {
                    paramNumber++;
                    stmt.setDate(paramNumber, csvm.getDateMin());
                }
                if (csvm.getDateMax() != null)
                {
                    paramNumber++;
                    stmt.setDate(paramNumber, csvm.getDateMax());
                }
                if (csvm.getTime() != null)
                {
                    paramNumber++;
                    String timeStr = csvm.getTime().toString().substring(0, 5);
                    stmt.setString(paramNumber, timeStr);
                }
                if (csvm.getReason() != null && csvm.getReason().length() > 0)
                {
                    paramNumber++;
                    stmt.setString(paramNumber, "%" + csvm.getReason() + "%");
                }
                if (csvm.getPatientLastName() != null && csvm.getPatientLastName().length() > 0)
                {
                    paramNumber++;
                    stmt.setString(paramNumber, "%" + csvm.getPatientLastName().toLowerCase() + "%");
                }
                if (csvm.getPatientFirstName() != null && csvm.getPatientFirstName().length() > 0)
                {
                    paramNumber++;
                    stmt.setString(paramNumber, "%" + csvm.getPatientFirstName().toLowerCase() + "%");
                }
            }

            ResultSet rs = stmt.executeQuery();


            while (rs.next())
            {
                Integer id = rs.getInt("id");
                Integer doctorId = rs.getInt("doctor_id");
                Integer patientId = rs.getInt("patient_id");
                Date date = rs.getDate("date");
                String timeStr = rs.getString("hour");
                Time time = null;
                if (timeStr != null && !timeStr.isEmpty()) {
                    if (timeStr.length() == 5) timeStr = timeStr + ":00";
                    time = Time.valueOf(timeStr);
                }
                String reason = rs.getString("reason");

                Consultation c = new Consultation(id, doctorId, patientId, date, time, reason);
                c.setPatientLastName(rs.getString("patient_last_name"));
                c.setPatientFirstName(rs.getString("patient_first_name"));
                c.setDoctorLastName(rs.getString("doctor_last_name"));
                c.setDoctorFirstName(rs.getString("doctor_first_name"));
                result.add(c);
            }

            stmt.close();
            consultations.clear();
            consultations.addAll(result);

        }
        catch (SQLException ex)
        {
            ex.printStackTrace();
        }
        return result;
    }

    public synchronized void save(Consultation c) {
        try {
            String sql;
            if (c != null) {
                if (c.getId() != null) {
                    sql = "UPDATE consultations SET date = ?, hour = ?, reason = ?, patient_id = ? WHERE id = ?";
                    PreparedStatement stmt = connectDB.getConn().prepareStatement(sql);
                    stmt.setDate(1, c.getDate());
                    stmt.setString(2, c.getTime().toString().substring(0,5));
                    stmt.setString(3, c.getReason());
                    if (c.getPatientId() != null)
                        stmt.setInt(4, c.getPatientId());
                    else
                        stmt.setNull(4, Types.INTEGER);
                    stmt.setInt(5, c.getId());
                    stmt.executeUpdate();
                    stmt.close();
                } else {
                    sql = "INSERT INTO consultations (doctor_id, patient_id, date, hour, reason) VALUES (?, ?, ?, ?, ?)";
                    PreparedStatement stmt = connectDB.getConn().prepareStatement(sql, Statement.RETURN_GENERATED_KEYS);
                    stmt.setInt(1, c.getDoctorId());
                    if (c.getPatientId() != null)
                        stmt.setInt(2, c.getPatientId());
                    else
                        stmt.setNull(2, Types.INTEGER);
                    stmt.setDate(3, c.getDate());
                    stmt.setString(4, c.getTime().toString().substring(0,5));
                    stmt.setString(5, c.getReason());
                    stmt.executeUpdate();

                    ResultSet rs = stmt.getGeneratedKeys();
                    if (rs.next()) c.setId(rs.getInt(1));
                    rs.close();
                    stmt.close();
                }
            }
        } catch (SQLException ex) {
            ex.printStackTrace();
        }
    }

    public synchronized void delete(Consultation entity) {
        if (entity != null && entity.getId() != null) {
            this.delete(entity.getId());
        }
    }

    public synchronized void delete(Integer id) {
        if (id != null) {
            try {
                String sql = "DELETE FROM consultations WHERE id = ?";
                PreparedStatement stmt = connectDB.getConn().prepareStatement(sql);
                stmt.setInt(1, id);
                stmt.executeUpdate();
                stmt.close();
            } catch (SQLException ex) {
                Logger.getLogger(ConsultationDAO.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
}