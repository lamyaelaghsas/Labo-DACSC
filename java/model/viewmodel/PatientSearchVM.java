package model.viewmodel;

import java.sql.Date;

public class PatientSearchVM
{
    //---------------attributs---------------
    private Integer id;
    private String lastName;
    private String firstName;
    private Date birthDate;

    //---------------constructeur---------------
    public PatientSearchVM() {}


    //---------------accesseurs---------------

    //get
    public Integer getId() { return id; }

    public String getLastName() { return lastName; }

    public String getFirstName() { return firstName; }

    public Date getBirthDate() { return birthDate; }

    //set
    public void setId(Integer id) { this.id = id; }

    public void setLastName(String lastName) { this.lastName = lastName; }

    public void setFirstName(String firstName) { this.firstName = firstName; }

    public void setBirthDate(Date birthDate) { this.birthDate = birthDate; }
}