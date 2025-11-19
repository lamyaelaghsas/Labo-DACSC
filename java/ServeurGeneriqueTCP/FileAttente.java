package ServeurGeneriqueTCP;

import java.net.Socket;
import java.util.LinkedList;

public class FileAttente
{
    private LinkedList<Socket> fileAttente;

    public FileAttente()
    {
        fileAttente = new LinkedList<>();
    }

    public synchronized void addConnexion(Socket socket)
    {
        fileAttente.addLast(socket); //ajout a la fin de la liste
        notify(); //on reveille le thread pour lui dire quune connexion arrive avec addconnexion
    }

    public synchronized Socket getConnexion() throws InterruptedException
    {
        while (fileAttente.isEmpty()) wait(); //tant que la file est vide, le thread attend
        return fileAttente.remove();//Dès qu’un socket est disponible, on le retire de la file et on le renvoie
    }
}