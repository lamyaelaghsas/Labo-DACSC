package model.dao;

import database.DatabaseConnection;
import model.entity.Doctor;
import model.viewmodel.DoctorSearchVM;

import java.sql.*;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

public class DoctorDAO
{
    private static DoctorDAO instance = null;
    private final DatabaseConnection connectDB;
    private final ArrayList<Doctor> doctors;

    private DoctorDAO()
    {
        connectDB = DatabaseConnection.getInstance();
        doctors = new ArrayList<>();
    }

    public static synchronized DoctorDAO getInstance()
    {
        if (instance == null)
        {
            instance = new DoctorDAO();
        }
        return instance;
    }

    public ArrayList<Doctor> getList()
    {
        return doctors;
    }

    public Doctor getById(Integer id)
    {
        for (Doctor d : doctors)
        {
            if (d.getId().equals(id))
            {
                return d;
            }
        }
        return null;
    }

    public synchronized ArrayList<Doctor> load()
    {
        return this.load(null);
    }

    public synchronized ArrayList<Doctor> load(DoctorSearchVM dsvm)
    {
        ArrayList<Doctor> result = new ArrayList<>();
        try
        {
            String sql = "SELECT doctors.id, doctors.specialty_id, doctors.last_name, doctors.first_name, specialties.name " +
                    "FROM doctors " +
                    "LEFT JOIN specialties ON doctors.specialty_id = specialties.id ";

            if (dsvm != null)
            {
                String where = " WHERE 1=1 ";

                if (dsvm.getId() != null)
                {
                    where += "AND doctors.id = ? ";
                }
                if (dsvm.getLastName() != null && dsvm.getLastName().length() > 0)
                {
                    where += "AND doctors.last_name LIKE ? ";
                }
                if (dsvm.getFirstName() != null && dsvm.getFirstName().length() > 0)
                {
                    where += "AND doctors.first_name LIKE ? ";
                }
                if (dsvm.getSpecialty() != null && dsvm.getSpecialty().length() > 0)
                {
                    where += "AND specialties.name LIKE ? ";
                }

                sql += where + "ORDER BY doctors.last_name, doctors.first_name;";
            }

            PreparedStatement stmt = connectDB.getConn().prepareStatement(sql);

            if (dsvm != null)
            {
                int paramNumber = 0;

                if (dsvm.getId() != null)
                {
                    paramNumber++;
                    stmt.setInt(paramNumber, dsvm.getId());
                }
                if (dsvm.getLastName() != null && dsvm.getLastName().length() > 0)
                {
                    paramNumber++;
                    stmt.setString(paramNumber, "%" + dsvm.getLastName() + "%");
                }
                if (dsvm.getFirstName() != null && dsvm.getFirstName().length() > 0)
                {
                    paramNumber++;
                    stmt.setString(paramNumber, "%" + dsvm.getFirstName() + "%");
                }
                if (dsvm.getSpecialty() != null && dsvm.getSpecialty().length() > 0)
                {
                    paramNumber++;
                    stmt.setString(paramNumber, "%" + dsvm.getSpecialty() + "%");
                }
            }

            ResultSet rs = stmt.executeQuery();

            while (rs.next())
            {
                Integer id = rs.getInt("doctors.id");
                Integer specialtyId = rs.getInt("doctors.specialty_id");
                String lastName = rs.getString("doctors.last_name");
                String firstName = rs.getString("doctors.first_name");

                Doctor d = new Doctor(id, lastName, firstName, specialtyId);
                result.add(d);
            }

            stmt.close();
            doctors.clear();
            doctors.addAll(result);

        }
        catch (SQLException ex)
        {
            ex.printStackTrace();
        }
        finally
        {
            return result;
        }
    }

    public synchronized void save(Doctor d)
    {
        try
        {
            String sql;
            if (d != null)
            {
                if (d.getId() != null)
                {
                    sql = "UPDATE doctors SET " +
                            "specialty_id = ?, " +
                            "last_name = ?, " +
                            "first_name = ? " +
                            "WHERE id = ?";

                    PreparedStatement pStmt = connectDB.getConn().prepareStatement(sql);
                    pStmt.setInt(1, d.getSpecialtyId());
                    pStmt.setString(2, d.getLastName());
                    pStmt.setString(3, d.getFirstName());
                    pStmt.setInt(4, d.getId());
                    pStmt.executeUpdate();
                    pStmt.close();
                }
                else
                {
                    sql = "INSERT INTO doctors (" +
                            "specialty_id, " +
                            "last_name, " +
                            "first_name) " +
                            "VALUES (?, ?, ?)";

                    PreparedStatement pStmt = connectDB.getConn().prepareStatement(sql, Statement.RETURN_GENERATED_KEYS);
                    pStmt.setInt(1, d.getSpecialtyId());
                    pStmt.setString(2, d.getLastName());
                    pStmt.setString(3, d.getFirstName());
                    pStmt.executeUpdate();

                    ResultSet rs = pStmt.getGeneratedKeys();
                    rs.next();
                    d.setId(rs.getInt(1));

                    rs.close();
                    pStmt.close();
                }
            }
        }
        catch (SQLException ex)
        {
            Logger.getLogger(DoctorDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public synchronized void delete(Doctor entity)
    {
        if (entity != null && entity.getId() != null)
        {
            this.delete(entity.getId());
        }
    }

    public synchronized void delete(Integer id)
    {
        if (id != null)
        {
            try
            {
                String sql = "DELETE FROM doctors WHERE id = ?";
                PreparedStatement stmt = connectDB.getConn().prepareStatement(sql);
                stmt.setInt(1, id);
                stmt.executeUpdate();
                stmt.close();
            }
            catch (SQLException ex)
            {
                Logger.getLogger(DoctorDAO.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
}