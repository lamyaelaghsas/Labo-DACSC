package model.entity;

import java.sql.Date;
import java.io.Serializable;

public class Patient implements Entity, Serializable
{
    private static final long serialVersionUID = 1L;

    //=================== ATTRIBUTS ===================
    private Integer id;
    private String lastName;
    private String firstName;
    private Date birthDate;


    //=================== CONSTRUCTEURS ===================

    // Constructeur par défaut
    public Patient() {
        this.id = null;
        this.lastName = "";
        this.firstName = "";
        this.birthDate = null;
    }

    // Constructeur d'initialisation (3 param pour DAO)
    public Patient(Integer id, String lastName, String firstName) {
        this.id = id;
        this.lastName = lastName;
        this.firstName = firstName;
        this.birthDate = null;
    }

    // Constructeur d'initialisation complet
    public Patient(Integer id, String lastName, String firstName, Date birthDate) {
        this.id = id;
        this.lastName = lastName;
        this.firstName = firstName;
        this.birthDate = birthDate;
    }


    //=================== ACCESSEURS ===================

    // GET
    public Integer getId() {
        return id;
    }
    public String getLastName() {
        return lastName;
    }
    public String getFirstName() {
        return firstName;
    }
    public Date getBirthDate() {
        return birthDate;
    }

    // SET
    public void setId(Integer id) {
        this.id = id;
    }
    public void setLastName(String lastName) {
        this.lastName = lastName;
    }
    public void setFirstName(String firstName) {
        this.firstName = firstName;
    }
    public void setBirthDate(Date birthDate) {
        this.birthDate = birthDate;
    }


    //=================== METHODES ===================

    @Override
    public String toString() {
        return "Patient[id=" + id + ", lastName=" + lastName +
                ", firstName=" + firstName + ", birthDate=" + birthDate + "]";
    }
}