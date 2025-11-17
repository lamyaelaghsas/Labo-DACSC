package protocol.ProtocoleCAP;

import ServeurGeneriqueTCP.*;
import model.entity.Consultation;
import java.io.Serializable;

public class RequeteUPDATE_CONSULTATION implements Requete, Serializable
{
    private Consultation consultation;

    public RequeteUPDATE_CONSULTATION(Consultation c)
    {
        consultation = c;
    }

    public Consultation getConsultation()
    {
        return consultation;
    }
}