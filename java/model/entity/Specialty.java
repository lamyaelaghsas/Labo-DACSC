package model.entity;

import java.io.Serializable;

public class Specialty implements Entity, Serializable
{
    private static final long serialVersionUID = 1L;

    //=================== ATTRIBUTS ===================
    private Integer id;
    private String name;


    //=================== CONSTRUCTEURS ===================

    // Constructeur defaut
    public Specialty() {
        this.id = null;
        this.name = "";
    }

    // Constructeur init
    public Specialty(Integer id, String name) {
        this.id = id;
        this.name = name;
    }


    //=================== ACCESSEURS ===================

    //get
    public Integer getId() {
        return id;
    }

    public String getName() {
        return name;
    }


    //set
    public void setId(Integer id) {
        this.id = id;
    }

    public void setName(String name) {
        this.name = name;
    }


    //=================== METHODES ===================

    @Override
    public String toString() {
        return "Specialty[id=" + id + ", name=" + name + "]";
    }
}