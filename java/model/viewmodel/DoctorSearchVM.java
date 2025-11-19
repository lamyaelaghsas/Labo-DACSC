package model.viewmodel;

public class DoctorSearchVM
{
    //=================== ATTRIBUTS ===================
    private Integer id;
    private String lastName;
    private String firstName;
    private String specialty;


    //=================== CONSTRUCTEURS ===================
    public DoctorSearchVM() {}


    //=================== ACCESSEURS ===================

    //get
    public Integer getId() {
        return id;
    }
    public String getLastName() {
        return lastName;
    }
    public String getFirstName() {
        return firstName;
    }
    public String getSpecialty() {
        return specialty;
    }


    //set
    public void setId(Integer id) {
        this.id = id;
    }
    public void setLastName(String lastName) {
        this.lastName = lastName;
    }
    public void setFirstName(String firstName) {
        this.firstName = firstName;
    }
    public void setSpecialty(String specialty) {
        this.specialty = specialty;
    }
}