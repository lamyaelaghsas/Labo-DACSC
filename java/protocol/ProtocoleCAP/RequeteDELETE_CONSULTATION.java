package protocol.ProtocoleCAP;

import ServeurGeneriqueTCP.*;
import java.io.Serializable;

public class RequeteDELETE_CONSULTATION implements Requete, Serializable
{
    private int consultationId;

    public RequeteDELETE_CONSULTATION(int id)
    {
        consultationId = id;
    }

    public int getConsultationId()
    {
        return consultationId;
    }
}