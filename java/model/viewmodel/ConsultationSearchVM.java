package model.viewmodel;

import java.io.Serializable;
import java.sql.Date;
import java.sql.Time;

public class ConsultationSearchVM implements Serializable
{
    private static final long serialVersionUID = 1L;
    //---------------attributs---------------
    private Integer id;
    private Integer patientId;
    private Integer doctorId;
    private Date dateMin;
    private Date dateMax;
    private Time time;
    private String reason;
    private String patientLastName;
    private String patientFirstName;


    //---------------constructeur---------------
    public ConsultationSearchVM() {}


    //---------------accesseurs---------------

    //get
    public Integer getId() {
        return id;
    }

    public Integer getPatientId() {
        return patientId;
    }

    public Integer getDoctorId() {
        return doctorId;
    }

    public Date getDateMin() {
        return dateMin;
    }

    public Date getDateMax() {
        return dateMax;
    }

    public Time getTime() {
        return time;
    }

    public String getReason() {
        return reason;
    }

    public String getPatientLastName()
    {
        return patientLastName;
    }

    public String getPatientFirstName()
    {
        return patientFirstName;
    }


    //set
    public void setId(Integer id) {
        this.id = id;
    }

    public void setPatientId(Integer patientId) {
        this.patientId = patientId;
    }

    public void setDoctorId(Integer doctorId) {
        this.doctorId = doctorId;
    }

    public void setDateMin(Date dateMin) {
        this.dateMin = dateMin;
    }

    public void setDateMax(Date dateMax) {
        this.dateMax = dateMax;
    }

    public void setTime(Time time) {
        this.time = time;
    }

    public void setReason(String reason) {
        this.reason = reason;
    }

    public void setPatientLastName(String patientLastName)
    {
        this.patientLastName = patientLastName;
    }

    public void setPatientFirstName(String patientFirstName)
    {
        this.patientFirstName = patientFirstName;
    }
}