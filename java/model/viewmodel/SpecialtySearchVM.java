package model.viewmodel;

public class SpecialtySearchVM
{
    //---------------attributs---------------
    private Integer id;
    private String name;


    //---------------constructeur---------------
    public SpecialtySearchVM() {}


    //---------------accesseurs---------------

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
}