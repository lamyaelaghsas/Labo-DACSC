package model.viewmodel;

public class UserSearchVM
{
    //---------------attributs---------------
    private Integer id;
    private String login;
    private String role;
    private Integer doctorId;

    //---------------constructeur---------------
    public UserSearchVM() {}

    //---------------accesseurs---------------

    //get
    public Integer getId()
    {
        return id;
    }

    public String getLogin()
    {
        return login;
    }

    public String getRole()
    {
        return role;
    }

    public Integer getDoctorId()
    {
        return doctorId;
    }

    //set
    public void setId(Integer id)
    {
        this.id = id;
    }

    public void setLogin(String login)
    {
        this.login = login;
    }

    public void setRole(String role)
    {
        this.role = role;
    }

    public void setDoctorId(Integer doctorId)
    {
        this.doctorId = doctorId;
    }
}