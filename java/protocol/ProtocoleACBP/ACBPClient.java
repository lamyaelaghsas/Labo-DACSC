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
        DataOutputStream dos = new DataOutputStream(csocket.getOutputStream()); //pour envoyer des données vers le serveur
        DataInputStream dis = new DataInputStream(csocket.getInputStream()); //pour recevoir des données depuis le serveur

        // 1. Préparer la requête à envoyer
        String request = "LIST_CLIENTS";
        int tailleRequete = request.length(); // = 12

        String header = String.valueOf(tailleRequete);// on convertit 12 en txt = "12"
        while (header.length() < 4) { // -> "0012" car le serveur attend un header de 4 caracteres
            header = "0" + header;
        }

        // 2. Envoi de la requête
        dos.write(header.getBytes()); //envoie le header
        dos.write(request.getBytes()); //envoie la requete
        dos.flush(); //force l'envoi
        System.out.println("Requête envoyée.");

        // 3. Lecture de la réponse du serveur
        //   a. Lecture du header de la réponse (4 caractères)
        StringBuffer headerBuffer = new StringBuffer();
        for (int i = 0; i < 4; i++) {
            byte b = dis.readByte();
            headerBuffer.append((char)b);
        }
        String tailleReponseStr = headerBuffer.toString();
        int tailleReponse = Integer.parseInt(tailleReponseStr);

        //   b. Lecture du message complet (140 caractères dans notre exemple)
        StringBuffer messageBuffer = new StringBuffer();
        for (int i = 0; i < tailleReponse; i++) {
            byte b = dis.readByte();
            messageBuffer.append((char)b);
        }
        String response = messageBuffer.toString();
        System.out.println("Réponse reçue.");

        String[] parts = response.split("#");//découpage du message
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