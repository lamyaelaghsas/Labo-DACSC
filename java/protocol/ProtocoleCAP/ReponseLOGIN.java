package protocol.ProtocoleCAP;

import ServeurGeneriqueTCP.*;
import java.io.Serializable;

public class ReponseLOGIN implements Reponse, Serializable
{
    private boolean valide;
    private int doctorId;

    public ReponseLOGIN(boolean v, int docId)
    {
        valide = v;
        doctorId = docId;
    }

    public boolean isValide()
    {
        return valide;
    }

    public int getDoctorId()
    {
        return doctorId;
    }
}