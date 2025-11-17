package protocol.ProtocoleACBP;

import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.List;

public class ACBPClient {

    public List<String[]> getConnectedClients(String serverAddress, int serverPort) throws IOException
    {
        List<String[]> clients = new ArrayList<>();
        Socket csocket;

        // Création de la socket et connexion sur le serveur
        csocket = new Socket(serverAddress, serverPort);
        System.out.println("Connexion établie.");

        // Création des flux
        DataOutputStream dos = new DataOutputStream(csocket.getOutputStream());
        DataInputStream dis = new DataInputStream(csocket.getInputStream());

        // Préparation des données à envoyer
        String request = "LIST_CLIENTS";
        int tailleRequete = request.length();
        String header = String.valueOf(tailleRequete);
        while (header.length() < 4) {
            header = "0" + header;
        }

        // Envoi des données
        dos.write(header.getBytes());
        dos.write(request.getBytes());
        dos.flush();
        System.out.println("Requête envoyée.");

        // Lecture de la réponse
        //header (4 bytes)
        StringBuffer headerBuffer = new StringBuffer();
        for (int i = 0; i < 4; i++) {
            byte b = dis.readByte();
            headerBuffer.append((char)b);
        }
        String tailleReponseStr = headerBuffer.toString();
        int tailleReponse = Integer.parseInt(tailleReponseStr);

        //message (tailleReponse bytes)
        StringBuffer messageBuffer = new StringBuffer();
        for (int i = 0; i < tailleReponse; i++) {
            byte b = dis.readByte();
            messageBuffer.append((char)b);
        }
        String response = messageBuffer.toString();
        System.out.println("Réponse reçue.");

        String[] parts = response.split("#");
        for (int i = 1; i < parts.length; i += 4) {
            String[] client = new String[4];
            client[0] = parts[i];
            client[1] = parts[i + 1];
            client[2] = parts[i + 2];
            client[3] = parts[i + 3];
            clients.add(client);
        }

        // Fermeture de la connexion
        csocket.close();

        return clients;
    }
}