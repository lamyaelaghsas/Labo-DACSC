package model.viewmodel;

public class SpecialtySearchVM
{
    //=================== ATTRIBUTS ===================
    private Integer id;
    private String name;


    //=================== CONSTRUCTEURS ===================
    public SpecialtySearchVM() {}


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
}