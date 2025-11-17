package protocol.ProtocoleCAP;

import ServeurGeneriqueTCP.*;
import model.entity.Patient;
import java.io.Serializable;

public class RequeteADD_PATIENT implements Requete, Serializable
{
    private Patient patient;

    public RequeteADD_PATIENT(Patient p)
    {
        patient = p;
    }

    public Patient getPatient()
    {
        return patient;
    }
}