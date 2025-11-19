package protocol.ProtocoleCAP;

import ServeurGeneriqueTCP.*;
import java.io.Serializable;

public class ReponseADD_CONSULTATION implements Reponse, Serializable
{
    private boolean succes;
    private String message;

    //Constructeur :
    ReponseADD_CONSULTATION(boolean s, String m)
    {
        succes = s;
        message = m;
    }
    public boolean isSucces()
    {
        return succes;
    }
    public String getMessage()
    {
        return message;
    }
}