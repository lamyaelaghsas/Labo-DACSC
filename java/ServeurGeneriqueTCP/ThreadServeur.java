package ServeurGeneriqueTCP;

import java.io.IOException;
import java.net.ServerSocket;


public abstract class ThreadServeur extends Thread
{
    protected int port;
    protected Protocole protocole;
    protected Logger logger;
    protected ServerSocket ssocket;

    public ThreadServeur(int port, Protocole protocole, Logger logger) throws IOException
    {
        super("TH Serveur (port=" + port + ",protocole=" + protocole.getNom() + ")");
        this.port = port;
        this.protocole = protocole;
        this.logger = logger;

        ssocket = new ServerSocket();
        ssocket.setReuseAddress(true);
        ssocket.bind(new java.net.InetSocketAddress(port));
    }

    @Override
    public void interrupt()
    {
        super.interrupt();
        try
        {
            if (ssocket != null && !ssocket.isClosed())
                ssocket.close();
        }
        catch (IOException e) {}
    }
}