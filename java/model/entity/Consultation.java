package model.entity;

import java.sql.Date;
import java.sql.Time;
import java.io.Serializable;

public class Consultation implements Entity, Serializable
{
    private static final long serialVersionUID = 1L;

    //=================== ATTRIBUTS ===================
    private Integer id;
    private Integer doctorId;
    private Integer patientId;
    private Date date;
    private Time time;
    private String reason;
    private String patientLastName;
    private String patientFirstName;
    private String doctorLastName;
    private String doctorFirstName;

    //=================== CONSTRUCTEURS ===================

    // Constructeur par defaut (vide)
    public Consultation() {
        this.id = null;
        this.doctorId = null;
        this.patientId = null;
        this.date = null;
        this.time = null;
        this.reason = "";
    }

    // Constructeur d'initialisation
    public Consultation(Integer id, Integer doctorId, Integer patientId, Date date, Time time, String reason) {
        this.id = id;
        this.doctorId = doctorId;
        this.patientId = patientId;
        this.date = date;
        this.time = time;
        this.reason = reason;
    }


    //=================== ACCESSEURS ===================

    //get
    public Integer getId() {
        return id;
    }
    public Integer getDoctorId() {
        return doctorId;
    }
    public Integer getPatientId() {
        return patientId;
    }
    public Date getDate() {
        return date;
    }
    public Time getTime() {
        return time;
    }
    public String getReason() {
        return reason;
    }
    public String getPatientLastName() { return patientLastName; }
    public String getPatientFirstName() { return patientFirstName; }
    public String getDoctorLastName() { return doctorLastName; }
    public String getDoctorFirstName() { return doctorFirstName; }

    //set
    public void setId(Integer id) {
        this.id = id;
    }
    public void setDoctorId(Integer doctorId) {
        this.doctorId = doctorId;
    }
    public void setPatientId(Integer patientId) {
        this.patientId = patientId;
    }
    public void setDate(Date date) {
        this.date = date;
    }
    public void setTime(Time time) {
        this.time = time;
    }
    public void setReason(String reason) {
        this.reason = reason;
    }
    public void setPatientLastName(String n) { patientLastName = n; }
    public void setPatientFirstName(String n) { patientFirstName = n; }
    public void setDoctorLastName(String n) { doctorLastName = n; }
    public void setDoctorFirstName(String n) { doctorFirstName = n; }


    //=================== METHODES ===================

    @Override
    public String toString() {
        return "Consultation[id=" + id + ", doctorId=" + doctorId +
                ", patientId=" + patientId + ", date=" + date +
                ", time=" + time + ", reason=" + reason + "]";
    }

}