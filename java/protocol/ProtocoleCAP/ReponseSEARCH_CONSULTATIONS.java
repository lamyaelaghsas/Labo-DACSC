package protocol.ProtocoleCAP;

import ServeurGeneriqueTCP.*;
import model.entity.Consultation;
import java.io.Serializable;
import java.util.ArrayList;

public class ReponseSEARCH_CONSULTATIONS implements Reponse, Serializable
{
    private ArrayList<Consultation> consultations;

    ReponseSEARCH_CONSULTATIONS(ArrayList<Consultation> c)
    {
        consultations = c;
    }

    public ArrayList<Consultation> getConsultations()
    {
        return consultations;
    }
}