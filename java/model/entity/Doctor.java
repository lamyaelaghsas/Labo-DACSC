package model.entity;

import java.io.Serializable;

public class Doctor implements Entity, Serializable
{
    private static final long serialVersionUID = 1L;

    //=================== ATTRIBUTS ===================
    private Integer id;
    private String lastName;
    private String firstName;
    private Integer specialtyId;


    //=================== CONSTRUCTEURS ===================

    // Constructeur defaut
    public Doctor()
    {
        this.id = null;
        this.lastName = "";
        this.firstName = "";
        this.specialtyId = null;
    }

    // Constructeur init
    public Doctor(Integer id, String lastName, String firstName, Integer specialtyId)
    {
        this.id = id;
        this.lastName = lastName;
        this.firstName = firstName;
        this.specialtyId = specialtyId;
    }


    //=================== ACCESSEURS ===================

    //get
    public Integer getId()
    {
        return id;
    }

    public String getLastName()
    {
        return lastName;
    }

    public String getFirstName()
    {
        return firstName;
    }

    public Integer getSpecialtyId()
    {
        return specialtyId;
    }


    //set
    public void setId(Integer id)
    {
        this.id = id;
    }

    public void setLastName(String lastName)
    {
        this.lastName = lastName;
    }

    public void setFirstName(String firstName)
    {
        this.firstName = firstName;
    }

    public void setSpecialtyId(Integer specialtyId)
    {
        this.specialtyId = specialtyId;
    }


    //=================== METHODES ===================

    @Override
    public String toString()
    {
        return "Doctor[id=" + id + ", lastName=" + lastName +
                ", firstName=" + firstName + ", specialtyId=" + specialtyId + "]";
    }
}