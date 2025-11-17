package protocol.ProtocoleCAP;

import ServeurGeneriqueTCP.*;
import java.io.Serializable;

public class RequeteLOGIN implements Requete, Serializable
{
    private String login;
    private String password;

    public RequeteLOGIN(String l, String p)
    {
        login = l;
        password = p;
    }

    public String getLogin()
    {
        return login;
    }

    public String getPassword()
    {
        return password;
    }
}