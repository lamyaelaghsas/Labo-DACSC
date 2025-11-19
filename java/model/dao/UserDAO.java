package model.dao;

import database.DatabaseConnection;
import model.entity.User;
import model.viewmodel.UserSearchVM;

import java.sql.*;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

public class UserDAO
{
    private static UserDAO instance = null;
    private final DatabaseConnection connectDB;
    private final ArrayList<User> users;

    private UserDAO()
    {
        connectDB = DatabaseConnection.getInstance();
        users = new ArrayList<>();
    }

    public static synchronized UserDAO getInstance()
    {
        if (instance == null)
        {
            instance = new UserDAO();
        }
        return instance;
    }

    public ArrayList<User> getList()
    {
        return users; //Retourne la liste des utilisateurs déjà chargés depuis la base
    }

    public User getById(Integer id)
    {
        for (User u : users)
        {
            if (u.getId().equals(id))
            {
                return u;
            }
        }
        return null;
    }

    public synchronized ArrayList<User> load()
    {
        return this.load(null);
    }

    public synchronized ArrayList<User> load(UserSearchVM usvm)
    {
        ArrayList<User> result = new ArrayList<>();

        try
        {
            String sql = "SELECT users.id, users.username, users.password, users.doctor_id, users.role " +
                    "FROM users ";

            if (usvm != null)
            {
                String where = " WHERE 1=1 ";

                if (usvm.getId() != null)
                {
                    where += "AND users.id = ? ";
                }
                if (usvm.getLogin() != null && usvm.getLogin().length() > 0)
                {
                    where += "AND users.username = ? ";
                }
                if (usvm.getRole() != null && usvm.getRole().length() > 0)
                {
                    where += "AND users.role = ? ";
                }
                if (usvm.getDoctorId() != null)
                {
                    where += "AND users.doctor_id = ? ";
                }

                sql += where + "ORDER BY users.username;";
            }

            PreparedStatement stmt = connectDB.getConn().prepareStatement(sql);

            if (usvm != null)
            {
                int paramNumber = 0;

                if (usvm.getId() != null)
                {
                    paramNumber++;
                    stmt.setInt(paramNumber, usvm.getId());
                }
                if (usvm.getLogin() != null && usvm.getLogin().length() > 0)
                {
                    paramNumber++;
                    stmt.setString(paramNumber, usvm.getLogin());
                }
                if (usvm.getRole() != null && usvm.getRole().length() > 0)
                {
                    paramNumber++;
                    stmt.setString(paramNumber, usvm.getRole());
                }
                if (usvm.getDoctorId() != null)
                {
                    paramNumber++;
                    stmt.setInt(paramNumber, usvm.getDoctorId());
                }
            }

            ResultSet rs = stmt.executeQuery();

            while (rs.next())
            {
                Integer id = rs.getInt("users.id");
                String username = rs.getString("users.username");
                String password = rs.getString("users.password");
                Integer doctorId = rs.getInt("users.doctor_id");
                String role = rs.getString("users.role");

                User u = new User(id, username, password, doctorId, role);
                result.add(u);
            }

            stmt.close();
            users.clear();
            users.addAll(result);

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

    public synchronized void save(User u)
    {
        try
        {
            String sql;
            if (u != null)
            {
                if (u.getId() != null)
                {
                    sql = "UPDATE users SET " +
                            "username = ?, " +
                            "password = ?, " +
                            "doctor_id = ?, " +
                            "role = ? " +
                            "WHERE id = ?";

                    PreparedStatement pStmt = connectDB.getConn().prepareStatement(sql);
                    pStmt.setString(1, u.getUsername());
                    pStmt.setString(2, u.getPassword());
                    pStmt.setInt(3, u.getDoctorId());
                    pStmt.setString(4, u.getRole());
                    pStmt.setInt(5, u.getId());
                    pStmt.executeUpdate();
                    pStmt.close();
                }
                else
                {
                    sql = "INSERT INTO users (" +
                            "username, " +
                            "password, " +
                            "doctor_id, " +
                            "role) " +
                            "VALUES (?, ?, ?, ?)";

                    PreparedStatement pStmt = connectDB.getConn().prepareStatement(sql, Statement.RETURN_GENERATED_KEYS);
                    pStmt.setString(1, u.getUsername());
                    pStmt.setString(2, u.getPassword());
                    pStmt.setInt(3, u.getDoctorId());
                    pStmt.setString(4, u.getRole());
                    pStmt.executeUpdate();

                    ResultSet rs = pStmt.getGeneratedKeys();
                    rs.next();
                    u.setId(rs.getInt(1));

                    rs.close();
                    pStmt.close();
                }
            }
        }
        catch (SQLException ex)
        {
            Logger.getLogger(UserDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public synchronized void delete(User entity)
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
                String sql = "DELETE FROM users WHERE id = ?";
                PreparedStatement stmt = connectDB.getConn().prepareStatement(sql);
                stmt.setInt(1, id);
                stmt.executeUpdate();
                stmt.close();
            }
            catch (SQLException ex)
            {
                Logger.getLogger(UserDAO.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
}