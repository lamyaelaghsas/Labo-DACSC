package protocol.ProtocoleCAP;

import ServeurGeneriqueTCP.*;
import model.dao.*;
import model.entity.*;
import model.viewmodel.*;
import java.net.Socket;
import java.util.HashMap;
import java.util.ArrayList;

public class CAP implements Protocole
{
    private HashMap<String,Socket> clientsConnectes; //string = loggin du medecin et Socket = connexion du medecin
    private Logger logger;

    public CAP(Logger log)
    {
        logger = log;
        clientsConnectes = new HashMap<>();
    }

    @Override
    public String getNom()
    {
        return "CAP";
    }


    @Override
    public synchronized Reponse TraiteRequete(Requete requete, Socket socket) throws FinConnexionException
    {
        if (requete instanceof RequeteLOGIN) return TraiteRequeteLOGIN((RequeteLOGIN) requete, socket);
        if (requete instanceof RequeteADD_CONSULTATION) return TraiteRequeteADD_CONSULTATION((RequeteADD_CONSULTATION) requete);
        if (requete instanceof RequeteADD_PATIENT) return TraiteRequeteADD_PATIENT((RequeteADD_PATIENT) requete);
        if (requete instanceof RequeteUPDATE_CONSULTATION) return TraiteRequeteUPDATE_CONSULTATION((RequeteUPDATE_CONSULTATION) requete);
        if (requete instanceof RequeteSEARCH_CONSULTATIONS) return TraiteRequeteSEARCH_CONSULTATIONS((RequeteSEARCH_CONSULTATIONS) requete);
        if (requete instanceof RequeteDELETE_CONSULTATION) return TraiteRequeteDELETE_CONSULTATION((RequeteDELETE_CONSULTATION) requete);
        if (requete instanceof RequeteLOGOUT) TraiteRequeteLOGOUT((RequeteLOGOUT) requete);
        return null;
    }


    private synchronized ReponseLOGIN TraiteRequeteLOGIN(RequeteLOGIN requete, Socket socket) throws FinConnexionException
    {
        logger.Trace("RequeteLOGIN reçue de " + requete.getLogin());

        UserDAO userDAO = UserDAO.getInstance();//J’utilise le DAO des utilisateurs pour pouvoir accéder à leurs données
        UserSearchVM usvm = new UserSearchVM(); //cree un filtre de recherche pr les users avec leur login
        usvm.setLogin(requete.getLogin());
        ArrayList<User> users = userDAO.load(usvm); //lancer la recherche

        if (users.size() > 0) //si User trouvé
        {
            User user = users.get(0); //on prend le 1er user de la liste
            if (user.getPassword().equals(requete.getPassword()))
            {
                String ipPortClient = socket.getInetAddress().getHostAddress() + "/" + socket.getPort(); //chaine avec ip+port du client
                logger.Trace(requete.getLogin() + " correctement loggé de " + ipPortClient);
                clientsConnectes.put(requete.getLogin(), socket); //Je garde en mémoire que ce client est maintenant connecté
                return new ReponseLOGIN(true, user.getDoctorId());
            }
        }

        logger.Trace(requete.getLogin() + " --> erreur de login");
        throw new FinConnexionException(new ReponseLOGIN(false, -1));
    }


    private synchronized ReponseADD_CONSULTATION TraiteRequeteADD_CONSULTATION(RequeteADD_CONSULTATION requete)
    {
        logger.Trace("RequeteADD_CONSULTATION reçue");

        try {
            ConsultationDAO consultationDAO = ConsultationDAO.getInstance(); //on utilise le dao pour interagir avec la bd des consultations
            Consultation c = requete.getConsultation();//on recup les infos de la consultation envoyée par le client
            consultationDAO.save(c); //on sauvg la consultation
            return new ReponseADD_CONSULTATION(true, "Consultation ajoutée avec succès"); //renvoie d'une reponse positive au client pr dire que la consultation a bien ete ajt
        } catch (Exception e) {
            e.printStackTrace();
            return new ReponseADD_CONSULTATION(false, "Erreur lors de l'ajout de la consultation : " + e.getMessage());
        }
    }


    private synchronized ReponseADD_PATIENT TraiteRequeteADD_PATIENT(RequeteADD_PATIENT requete)
    {
        try {
            PatientDAO patientDAO = PatientDAO.getInstance();
            Patient p = requete.getPatient();
            patientDAO.save(p);
            return new ReponseADD_PATIENT(true, "Patient ajouté avec succès !", p.getId());
        } catch (Exception e) {
            e.printStackTrace();
            return new ReponseADD_PATIENT(false, "Erreur lors de l'ajout du patient : " + e.getMessage(), null);
        }
    }


    private synchronized ReponseUPDATE_CONSULTATION TraiteRequeteUPDATE_CONSULTATION(RequeteUPDATE_CONSULTATION requete)
    {
        logger.Trace("RequeteUPDATE_CONSULTATION reçue");

        try {
            ConsultationDAO consultationDAO = ConsultationDAO.getInstance();
            Consultation c = requete.getConsultation();

            ConsultationSearchVM csvm = new ConsultationSearchVM();//crée un filtre pour rechercher la consultation à modifier
            csvm.setId(c.getId()); //on indique dans le filtre l’ID de la consultation à rechercher
            ArrayList<Consultation> list = consultationDAO.load(csvm); //cherche la consultation correspondante dans la base

            if (list.isEmpty()) {
                return new ReponseUPDATE_CONSULTATION(false, "Consultation introuvable.");
            }

            Consultation existante = list.get(0);

            //si consultation déjà réservée
            if (existante.getPatientLastName() != null && !existante.getPatientLastName().isEmpty() && existante.getPatientFirstName() != null && !existante.getPatientFirstName().isEmpty()) {
                return new ReponseUPDATE_CONSULTATION(false, "Cette consultation est déjà réservée par un patient !");
            }

            consultationDAO.save(c);
            return new ReponseUPDATE_CONSULTATION(true, "Consultation réservée avec succès !");

        } catch (Exception e) {
            e.printStackTrace();
            return new ReponseUPDATE_CONSULTATION(false, "Erreur lors de la mise à jour : " + e.getMessage());
        }
    }

    private synchronized ReponseSEARCH_CONSULTATIONS TraiteRequeteSEARCH_CONSULTATIONS(RequeteSEARCH_CONSULTATIONS requete)
    {
        logger.Trace("RequeteSEARCH_CONSULTATIONS reçue");

        ConsultationDAO consultationDAO = ConsultationDAO.getInstance();
        ArrayList<Consultation> consultations = consultationDAO.load(requete.getSearchVM());

        return new ReponseSEARCH_CONSULTATIONS(consultations);//on renvoie au client toutes les consultations qui correspondent aux critères
    }

    private synchronized ReponseDELETE_CONSULTATION TraiteRequeteDELETE_CONSULTATION(RequeteDELETE_CONSULTATION requete)
    {

        logger.Trace("RequeteDELETE_CONSULTATION reçue");

        try {
            ConsultationDAO consultationDAO = ConsultationDAO.getInstance();
            ConsultationSearchVM csvm = new ConsultationSearchVM();
            csvm.setId(requete.getConsultationId());
            ArrayList<Consultation> list = consultationDAO.load(csvm);
            Consultation c = list.get(0);

            //Si la consultation est déja reservée par un patient
            if (c.getPatientLastName() != null && !c.getPatientLastName().isEmpty()  && c.getPatientFirstName() != null && !c.getPatientFirstName().isEmpty()) {
                return new ReponseDELETE_CONSULTATION(false, "Impossible de supprimer : cette consultation est déjà réservée par un patient.");
            }

            // si libre
            consultationDAO.delete(requete.getConsultationId());
            return new ReponseDELETE_CONSULTATION(true, "Consultation supprimée avec succès !");

        } catch (Exception e) {
            e.printStackTrace();
            return new ReponseDELETE_CONSULTATION(false, "Erreur lors de la suppression : " + e.getMessage());
        }
    }

    private synchronized void TraiteRequeteLOGOUT(RequeteLOGOUT requete) throws FinConnexionException
    {
        logger.Trace("RequeteLOGOUT reçue de " + requete.getLogin());
        clientsConnectes.remove(requete.getLogin());
        logger.Trace(requete.getLogin() + " correctement déloggé");
        throw new FinConnexionException(null); //utilisée pour fermer la socket du client -> coupe la communication entre serveur tcp et client
    }


}