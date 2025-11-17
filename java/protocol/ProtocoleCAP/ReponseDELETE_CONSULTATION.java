package protocol.ProtocoleCAP;

import ServeurGeneriqueTCP.*;
import java.io.Serializable;

public class ReponseDELETE_CONSULTATION implements Reponse, Serializable
{
    private boolean succes;
    private String message;

    ReponseDELETE_CONSULTATION(boolean s, String m)
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