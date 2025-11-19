package protocol.ProtocoleCAP;

import ServeurGeneriqueTCP.*;
import model.entity.Consultation;
import java.io.Serializable;

public class RequeteADD_CONSULTATION implements Requete, Serializable
{
    private Consultation consultation;

    //Constructeur
    public RequeteADD_CONSULTATION(Consultation c)
    {
        consultation = c;
    }

    //Methode : permet au serveur de récupérer la consultation envoyée par le client
    public Consultation getConsultation()
    {
        return consultation;
    }
}