package protocol.ProtocoleCAP;

import ServeurGeneriqueTCP.*;
import java.io.Serializable;

public class ReponseUPDATE_CONSULTATION implements Reponse, Serializable
{
    private boolean succes;
    private String message;

    ReponseUPDATE_CONSULTATION(boolean s, String m)
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