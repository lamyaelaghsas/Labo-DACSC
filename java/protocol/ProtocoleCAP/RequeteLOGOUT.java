package protocol.ProtocoleCAP;

import ServeurGeneriqueTCP.*;
import java.io.Serializable;

public class RequeteLOGOUT implements Requete, Serializable
{
    private String login;

    public RequeteLOGOUT(String l)
    {
        login = l;
    }

    public String getLogin()
    {
        return login;
    }
}