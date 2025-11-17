package model.entity;

import java.io.Serializable;

public class User implements Entity, Serializable
{
    private static final long serialVersionUID = 1L;

    //=================== ATTRIBUTS ===================
    private Integer id;
    private String username;
    private String password;
    private Integer doctorId;
    private String role;


    //=================== CONSTRUCTEURS ===================

    // Constructeur par défaut
    public User()
    {
        this.id = null;
        this.username = "";
        this.password = "";
        this.doctorId = null;
        this.role = "";
    }

    // Constructeur d'initialisation
    public User(Integer id, String username, String password, Integer doctorId, String role)
    {
        this.id = id;
        this.username = username;
        this.password = password;
        this.doctorId = doctorId;
        this.role = role;
    }


    //=================== ACCESSEURS ===================

    //get
    public Integer getId()
    {
        return id;
    }

    public String getUsername()
    {
        return username;
    }

    public String getPassword()
    {
        return password;
    }

    public Integer getDoctorId()
    {
        return doctorId;
    }

    public String getRole()
    {
        return role;
    }


    //set
    public void setId(Integer id)
    {
        this.id = id;
    }

    public void setUsername(String username)
    {
        this.username = username;
    }

    public void setPassword(String password)
    {
        this.password = password;
    }

    public void setDoctorId(Integer doctorId)
    {
        this.doctorId = doctorId;
    }

    public void setRole(String role)
    {
        this.role = role;
    }


    //=================== METHODES ===================

    @Override
    public String toString()
    {
        return "User{" +
                "id=" + id +
                ", username='" + username + '\'' +
                ", doctorId=" + doctorId +
                ", role='" + role + '\'' +
                '}';
    }
}