package database;

import java.sql.*;
import java.util.logging.*;
import configuration.ReadConfiguration;

public class DatabaseConnection
{
    private static Connection conn = null; //connexion sql utilisée partt
    private static DatabaseConnection instance = null; //objet unique de la classe (singleton)

    private DatabaseConnection()
    {
        try
        {
            if (conn == null || conn.isClosed())
            {
                ReadConfiguration config = new ReadConfiguration();
                config.load("java/config.properties");

                String sCon = ReadConfiguration.get("DB_URL");
                String sUser = ReadConfiguration.get("DB_USER");
                String sPwd = ReadConfiguration.get("DB_PASSWORD");
                Class.forName("com.mysql.cj.jdbc.Driver"); //charge le "pilote" MySQL pour permettre à Java de parler avec MySQL
                conn = DriverManager.getConnection(sCon, sUser, sPwd); //On crée la connexion réelle à la base, en utilisant l’URL, l’utilisateur et le mot de passe
            }
        }
        catch (ClassNotFoundException | SQLException ex)
        {
            Logger.getLogger(DatabaseConnection.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public static synchronized DatabaseConnection getInstance() {
        if (instance == null) {
            instance = new DatabaseConnection();
        }
        return instance;
    }

    public Connection getConn() { //Permet d'accéder à la connexion JDBC (celle qui est stockée dans conn)
        return conn;
    }

    public static void close()
    {
        try
        {
            if (conn != null)
            {
                conn.close();
                System.out.println("Closing DB connection");
            }
        }
        catch (SQLException ex)
        {
            Logger.getLogger(DatabaseConnection.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
}