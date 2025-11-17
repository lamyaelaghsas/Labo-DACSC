package model.dao;

import database.DatabaseConnection;
import model.entity.Specialty;
import model.viewmodel.SpecialtySearchVM;

import java.sql.*;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

public class SpecialtyDAO {
    private static SpecialtyDAO instance = null;
    private final DatabaseConnection connectDB;
    private final ArrayList<Specialty> specialties;

    private SpecialtyDAO() {
        connectDB = DatabaseConnection.getInstance();
        specialties = new ArrayList<>();
    }

    public static synchronized SpecialtyDAO getInstance() {
        if (instance == null) {
            instance = new SpecialtyDAO();
        }
        return instance;
    }

    public ArrayList<Specialty> getList() {
        return specialties;
    }

    public Specialty getById(Integer id) {
        for (Specialty s : specialties) {
            if (s.getId().equals(id)) {
                return s;
            }
        }
        return null;
    }

    public synchronized ArrayList<Specialty> load() {
        return this.load(null);
    }

    public synchronized ArrayList<Specialty> load(SpecialtySearchVM ssvm) {
        try {
            String sql = "SELECT id, name " +
                    "FROM specialties " +
                    "ORDER BY name";

            if (ssvm != null) {
                String where = " WHERE 1=1 ";

                if (ssvm.getId() != null) {
                    where += "AND id = ? ";
                }
                if (ssvm.getName() != null && ssvm.getName().length() > 0) {
                    where += "AND name LIKE ? ";
                }

                sql += where + "ORDER BY name;";
            }

            PreparedStatement stmt = connectDB.getConn().prepareStatement(sql);

            if (ssvm != null) {
                int paramNumber = 0;

                if (ssvm.getId() != null) {
                    paramNumber++;
                    stmt.setInt(paramNumber, ssvm.getId());
                }
                if (ssvm.getName() != null && ssvm.getName().length() > 0) {
                    paramNumber++;
                    stmt.setString(paramNumber, "%" + ssvm.getName() + "%");
                }
            }

            ResultSet rs = stmt.executeQuery();
            specialties.clear();

            while (rs.next()) {
                Integer id = rs.getInt("id");
                String name = rs.getString("name");

                Specialty s = new Specialty(id, name);
                specialties.add(s);
            }

            stmt.close();

        } catch (SQLException ex) {
            ex.printStackTrace();
        } finally {
            return specialties;
        }
    }

    public synchronized void save(Specialty s) {
        try {
            String sql;
            if (s != null) {
                if (s.getId() != null) {
                    sql = "UPDATE specialties SET " +
                            "name = ? " +
                            "WHERE id = ?";

                    PreparedStatement pStmt = connectDB.getConn().prepareStatement(sql);
                    pStmt.setString(1, s.getName());
                    pStmt.setInt(2, s.getId());
                    pStmt.executeUpdate();
                    pStmt.close();
                } else {
                    sql = "INSERT INTO specialties (" +
                            "name) " +
                            "VALUES (?)";

                    PreparedStatement pStmt = connectDB.getConn().prepareStatement(sql, Statement.RETURN_GENERATED_KEYS);
                    pStmt.setString(1, s.getName());
                    pStmt.executeUpdate();

                    ResultSet rs = pStmt.getGeneratedKeys();
                    rs.next();
                    s.setId(rs.getInt(1));

                    rs.close();
                    pStmt.close();
                }
            }
        } catch (SQLException ex) {
            Logger.getLogger(SpecialtyDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public synchronized void delete(Specialty entity) {
        if (entity != null && entity.getId() != null) {
            this.delete(entity.getId());
        }
    }

    public synchronized void delete(Integer id) {
        if (id != null) {
            try {
                String sql = "DELETE FROM specialties WHERE id = ?";
                PreparedStatement stmt = connectDB.getConn().prepareStatement(sql);
                stmt.setInt(1, id);
                stmt.executeUpdate();
                stmt.close();
            } catch (SQLException ex) {
                Logger.getLogger(SpecialtyDAO.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
}