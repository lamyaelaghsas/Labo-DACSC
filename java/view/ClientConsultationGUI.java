package view;

import model.viewmodel.*;
import protocol.ProtocoleCAP.*;
import model.entity.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.table.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.text.SimpleDateFormat;

public class ClientConsultationGUI extends JFrame implements ActionListener
{
    private Socket socket;
    private ObjectOutputStream oos;
    private ObjectInputStream ois;

    private JTable tableConsultations;
    private DefaultTableModel modelTable;

    private JTextField txtPatientName;
    private JTextField txtPatientFirstName;
    private JTextField txtDateDebut;
    private JTextField txtDateFin;
    private JButton btnRechercher;
    private JButton btnAjouterPatient;
    private JButton btnNouvelleConsult;
    private JButton btnReserverConsult;
    private JButton btnSupprimerConsult;
    private JButton btnModifierConsult;
    private JButton btnDeconnexion;

    private String username;
    private int doctorId;

    // Couleurs pastel
    private Color rosePastel = new Color(255, 209, 220);
    private Color jaunePastel = new Color(255, 253, 208);
    private Color vertPastel = new Color(220, 240, 220);
    private Color bleuPastel = new Color(224, 240, 255);
    private Color backgroundColor = new Color(252, 252, 255);
    private Color borderColor = new Color(230, 230, 235);
    private Color textColor = new Color(80, 80, 90);

    //-------------FONCTIONS------------------
    public ClientConsultationGUI(String user, int docId)
    {
        username = user;
        doctorId = docId;

        setTitle("Gestion des Consultations - " + username);
        setSize(1200, 700);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        getContentPane().setBackground(backgroundColor);

        initComponents();
        loadConsultations();

        setVisible(true);
    }

    private void initComponents()
    {
        JPanel panelNord = new JPanel();
        panelNord.setLayout(new BoxLayout(panelNord, BoxLayout.Y_AXIS));
        panelNord.setBackground(backgroundColor);
        panelNord.setBorder(BorderFactory.createEmptyBorder(20, 20, 10, 20));

        // Titre
        JLabel titre = new JLabel("Gestion des Consultations");
        titre.setFont(new Font("Segoe UI", Font.PLAIN, 22));
        titre.setForeground(textColor);
        titre.setAlignmentX(Component.LEFT_ALIGNMENT);
        panelNord.add(titre);
        panelNord.add(Box.createVerticalStrut(15));

        //------------RECHERCHE AVEC FILTRE---------------
        JPanel panelFiltres = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 5));
        panelFiltres.setBackground(jaunePastel);
        panelFiltres.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(borderColor, 1),
                BorderFactory.createEmptyBorder(10, 10, 10, 10)
        ));

        JLabel lblNom = new JLabel("Nom Patient :");
        lblNom.setForeground(textColor);
        panelFiltres.add(lblNom);
        txtPatientName = new JTextField(12);
        txtPatientName.setFont(new Font("Segoe UI", Font.PLAIN, 13));
        panelFiltres.add(txtPatientName);

        JLabel lblPrenom = new JLabel("Prénom Patient :");
        lblPrenom.setForeground(textColor);
        panelFiltres.add(lblPrenom);
        txtPatientFirstName = new JTextField(12);
        txtPatientFirstName.setFont(new Font("Segoe UI", Font.PLAIN, 13));
        panelFiltres.add(txtPatientFirstName);

        JLabel lblDateDebut = new JLabel("Date début (yyyy-MM-dd) :");
        lblDateDebut.setForeground(textColor);
        panelFiltres.add(lblDateDebut);
        txtDateDebut = new JTextField(10);
        txtDateDebut.setFont(new Font("Segoe UI", Font.PLAIN, 13));
        panelFiltres.add(txtDateDebut);

        JLabel lblDateFin = new JLabel("Date fin :");
        lblDateFin.setForeground(textColor);
        panelFiltres.add(lblDateFin);
        txtDateFin = new JTextField(10);
        txtDateFin.setFont(new Font("Segoe UI", Font.PLAIN, 13));
        panelFiltres.add(txtDateFin);

        btnRechercher = createStyledButton("Rechercher", rosePastel);
        btnRechercher.addActionListener(this);
        panelFiltres.add(btnRechercher);

        panelFiltres.setAlignmentX(Component.LEFT_ALIGNMENT);
        panelNord.add(panelFiltres);
        panelNord.add(Box.createVerticalStrut(10));

        //------------Boutons------------
        JPanel panelActions = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 5));
        panelActions.setBackground(backgroundColor);

        btnAjouterPatient = createStyledButton("Nouveau Patient", vertPastel);
        btnAjouterPatient.addActionListener(this);
        panelActions.add(btnAjouterPatient);

        btnNouvelleConsult = createStyledButton("Nouvelle Consultation", vertPastel);
        btnNouvelleConsult.addActionListener(this);
        panelActions.add(btnNouvelleConsult);

        btnReserverConsult = createStyledButton("Reserver Consultation", bleuPastel);
        btnReserverConsult.addActionListener(this);
        panelActions.add(btnReserverConsult);

        btnSupprimerConsult = createStyledButton("Supprimer Consultation", new Color(255, 220, 220));
        btnSupprimerConsult.addActionListener(this);
        panelActions.add(btnSupprimerConsult);

        btnModifierConsult = createStyledButton("Modifier Consultation", bleuPastel);
        btnModifierConsult.addActionListener(this);
        panelActions.add(btnModifierConsult);

        btnDeconnexion = createStyledButton("Déconnexion", new Color(240, 240, 245));
        btnDeconnexion.addActionListener(this);
        panelActions.add(btnDeconnexion);

        panelActions.setAlignmentX(Component.LEFT_ALIGNMENT);
        panelNord.add(panelActions);

        String[] colonnes = {"ID", "NomPatient", "PrenomPatient", "Médecin", "Raison", "Date", "Heure"};
        modelTable = new DefaultTableModel(colonnes, 0)
        {
            public boolean isCellEditable(int row, int column)
            {
                return false;
            }
        };

        tableConsultations = new JTable(modelTable);
        tableConsultations.setFont(new Font("Segoe UI", Font.PLAIN, 13));
        tableConsultations.setRowHeight(35);
        tableConsultations.setGridColor(borderColor);
        tableConsultations.setSelectionBackground(vertPastel);
        tableConsultations.setSelectionForeground(textColor);
        tableConsultations.setShowVerticalLines(false);
        tableConsultations.setShowHorizontalLines(true);
        tableConsultations.setBackground(Color.WHITE);
        tableConsultations.setForeground(textColor);
        tableConsultations.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

        tableConsultations.getTableHeader().setFont(new Font("Segoe UI", Font.BOLD, 13));
        tableConsultations.getTableHeader().setBackground(bleuPastel);
        tableConsultations.getTableHeader().setForeground(textColor);
        tableConsultations.getTableHeader().setPreferredSize(new Dimension(0, 40));
        tableConsultations.getTableHeader().setBorder(BorderFactory.createMatteBorder(0, 0, 1, 0, borderColor));

        DefaultTableCellRenderer centerRenderer = new DefaultTableCellRenderer();
        centerRenderer.setHorizontalAlignment(JLabel.CENTER);
        for (int i = 0; i < tableConsultations.getColumnCount(); i++) {
            tableConsultations.getColumnModel().getColumn(i).setCellRenderer(centerRenderer);
        }

        tableConsultations.addMouseListener(new MouseAdapter()
        {
            public void mouseClicked(MouseEvent e)
            {
                if (e.getClickCount() == 2)
                {
                    modifierConsultation();
                }
            }
        });

        JScrollPane scrollPane = new JScrollPane(tableConsultations);
        scrollPane.setBorder(BorderFactory.createEmptyBorder(10, 20, 20, 20));
        scrollPane.getViewport().setBackground(backgroundColor);

        add(panelNord, BorderLayout.NORTH);
        add(scrollPane, BorderLayout.CENTER);
    }

    private JButton createStyledButton(String text, Color bgColor)
    {
        JButton button = new JButton(text);
        button.setFont(new Font("Segoe UI", Font.PLAIN, 12));
        button.setBackground(bgColor);
        button.setForeground(textColor);
        button.setFocusPainted(false);
        button.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(borderColor, 1),
                BorderFactory.createEmptyBorder(8, 15, 8, 15)
        ));
        button.setCursor(new Cursor(Cursor.HAND_CURSOR));

        Color hoverColor = bgColor.darker();
        button.addMouseListener(new MouseAdapter() {
            public void mouseEntered(MouseEvent evt) {
                button.setBackground(hoverColor);
            }
            public void mouseExited(MouseEvent evt) {
                button.setBackground(bgColor);
            }
        });

        return button;
    }

    private void loadConsultations()
    {
        ConsultationSearchVM searchVM = new model.viewmodel.ConsultationSearchVM();
        searchVM.setDoctorId(doctorId);
        searchConsultations(searchVM);
    }

    private void searchConsultations(ConsultationSearchVM searchVM)
    {
        try
        {
            RequeteSEARCH_CONSULTATIONS req = new RequeteSEARCH_CONSULTATIONS(searchVM);
            ReponseSEARCH_CONSULTATIONS rep =
                    (ReponseSEARCH_CONSULTATIONS) envoyerRequete(req);

            ArrayList<Consultation> consultations = rep.getConsultations();

            modelTable.setRowCount(0);

            for (Consultation c : consultations)
            {
                Object[] row =
                        {
                                c.getId(),
                                (c.getPatientLastName() != null ? c.getPatientLastName() : ""),
                                (c.getPatientFirstName() != null ? c.getPatientFirstName() : ""),
                                (c.getDoctorLastName() != null ? c.getDoctorLastName() : "") + " " + (c.getDoctorFirstName() != null ? c.getDoctorFirstName() : ""),
                                c.getReason(),
                                c.getDate(),
                                c.getTime()
                        };
                modelTable.addRow(row);
            }
        }
        catch (Exception e)
        {
            JOptionPane.showMessageDialog(this, "Erreur communication : " + e.getMessage(), "Erreur", JOptionPane.ERROR_MESSAGE);
        }
    }

    private Object envoyerRequete(Object requete) throws Exception
    {
        try {

            Socket socket = new Socket("localhost", 50002);
            ObjectOutputStream oos =  new ObjectOutputStream(socket.getOutputStream());
            ObjectInputStream ois = new ObjectInputStream(socket.getInputStream());

            oos.writeObject(requete);
            oos.flush();

            if (requete instanceof RequeteLOGOUT) {
                return null;
            }

            Object reponse = ois.readObject();
            return reponse;

        } catch (IOException | ClassNotFoundException ex) {
            ex.printStackTrace();
            return null;

        } finally {
            try {
                if (ois != null) ois.close();
                if (oos != null) oos.close();
                if (socket != null) socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }


    //-----------------BOUTONS---------------

    public void actionPerformed(ActionEvent e)
    {
        if (e.getSource() == btnRechercher)
        {
            rechercherConsultations();
        }

        else if (e.getSource() == btnAjouterPatient)
        {
            ajouterPatient();
        }

        else if (e.getSource() == btnNouvelleConsult)
        {
            nouvelleConsultation();
        }

        else if (e.getSource() == btnReserverConsult)
        {
            reserverConsultation();
        }

        else if (e.getSource() == btnSupprimerConsult)
        {
            supprimerConsultation();
        }

        else if (e.getSource() == btnModifierConsult)
        {
            modifierConsultation();
        }

        else if (e.getSource() == btnDeconnexion)
        {
            deconnexion();
        }
    }


    //----------------FONCTIONS POUR BOUTONS----------------

    private void rechercherConsultations()
    {
        model.viewmodel.ConsultationSearchVM searchVM = new model.viewmodel.ConsultationSearchVM();
        searchVM.setDoctorId(doctorId);

        String nom = txtPatientName.getText().trim();
        if (!nom.isEmpty())
            searchVM.setPatientLastName(nom);

        String prenom = txtPatientFirstName.getText().trim();
        if (!prenom.isEmpty())
            searchVM.setPatientFirstName(prenom);

        String dateDebut = txtDateDebut.getText().trim();
        if (!dateDebut.isEmpty())
        {
            try
            {
                SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
                java.util.Date utilDate = sdf.parse(dateDebut);
                searchVM.setDateMin(new java.sql.Date(utilDate.getTime()));
            }
            catch (Exception ex)
            {
                JOptionPane.showMessageDialog(this,
                        "Format date début invalide ! Utilisez yyyy-MM-dd",
                        "Erreur", JOptionPane.ERROR_MESSAGE);
                return;
            }
        }

        String dateFin = txtDateFin.getText().trim();
        if (!dateFin.isEmpty())
        {
            try
            {
                SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
                java.util.Date utilDate = sdf.parse(dateFin);
                searchVM.setDateMax(new java.sql.Date(utilDate.getTime()));
            }
            catch (Exception ex)
            {
                JOptionPane.showMessageDialog(this,
                        "Format date fin invalide ! Utilisez yyyy-MM-dd",
                        "Erreur", JOptionPane.ERROR_MESSAGE);
                return;
            }
        }

        searchConsultations(searchVM);
    }


    private void ajouterPatient()
    {
        try
        {
            String nom = JOptionPane.showInputDialog(this, "Nom du patient :");
            if (nom == null || nom.trim().isEmpty()) return;

            String prenom = JOptionPane.showInputDialog(this, "Prénom du patient :");
            if (prenom == null || prenom.trim().isEmpty()) return;

            Patient p = new Patient(null, nom.trim(), prenom.trim(), null);
            RequeteADD_PATIENT req = new RequeteADD_PATIENT(p);
            ReponseADD_PATIENT rep = (ReponseADD_PATIENT) envoyerRequete(req);

            if (rep.isSucces())
            {
                JOptionPane.showMessageDialog(this,  rep.getMessage(), "Succès", JOptionPane.INFORMATION_MESSAGE);
            }
            else
            {
                JOptionPane.showMessageDialog(this, rep.getMessage(), "Erreur", JOptionPane.ERROR_MESSAGE);
            }
        }
        catch (Exception e)
        {
            JOptionPane.showMessageDialog(this, "Erreur communication : " + e.getMessage(), "Erreur", JOptionPane.ERROR_MESSAGE);
        }
    }


    private void nouvelleConsultation()
    {
        JTextField txtDate = new JTextField();
        JTextField txtHeure = new JTextField();

        Object[] message = {
                "Date (yyyy-MM-dd):", txtDate,
                "Heure (HH:mm):", txtHeure
        };

        int option = JOptionPane.showConfirmDialog(this, message, "Nouvelle Consultation", JOptionPane.OK_CANCEL_OPTION);
        if (option == JOptionPane.OK_OPTION)
        {
            String dateStr = txtDate.getText().trim();
            String heureStr = txtHeure.getText().trim();

            if (dateStr.isEmpty() || heureStr.isEmpty())
            {
                JOptionPane.showMessageDialog(this, "Veuillez remplir la date et l'heure !");
                return;
            }

            try
            {
                SimpleDateFormat sdfDate = new SimpleDateFormat("yyyy-MM-dd");
                SimpleDateFormat sdfTime = new SimpleDateFormat("HH:mm");

                java.util.Date utilDate = sdfDate.parse(dateStr);
                java.util.Date utilTime = sdfTime.parse(heureStr);

                java.sql.Date sqlDate = new java.sql.Date(utilDate.getTime());
                java.sql.Time sqlTime = new java.sql.Time(utilTime.getTime());

                model.entity.Consultation c = new model.entity.Consultation();
                c.setDoctorId(doctorId);
                c.setDate(sqlDate);
                c.setTime(sqlTime);
                c.setReason(null);
                c.setPatientId(null);

                RequeteADD_CONSULTATION req = new RequeteADD_CONSULTATION(c);
                ReponseADD_CONSULTATION rep = (ReponseADD_CONSULTATION) envoyerRequete(req);

                JOptionPane.showMessageDialog(this, rep.getMessage(), "Résultat", JOptionPane.INFORMATION_MESSAGE);

                if (rep.isSucces())
                {
                    loadConsultations();
                }

            }
            catch (Exception ex)
            {
                JOptionPane.showMessageDialog(this, "Erreur : " + ex.getMessage(), "Erreur", JOptionPane.ERROR_MESSAGE);
                ex.printStackTrace();
            }
        }
    }


    private void reserverConsultation()
    {
        int selectedRow = tableConsultations.getSelectedRow();
        if (selectedRow == -1) {
            JOptionPane.showMessageDialog(this, "Sélectionnez une consultation", "Attention", JOptionPane.WARNING_MESSAGE);
            return;
        }

        try {
            Integer consultId = (Integer) tableConsultations.getValueAt(selectedRow, 0);

            model.viewmodel.ConsultationSearchVM searchVM = new model.viewmodel.ConsultationSearchVM();
            searchVM.setId(consultId);

            RequeteSEARCH_CONSULTATIONS reqCheck = new RequeteSEARCH_CONSULTATIONS(searchVM);
            ReponseSEARCH_CONSULTATIONS repCheck = (ReponseSEARCH_CONSULTATIONS) envoyerRequete(reqCheck);

            if (repCheck.getConsultations().isEmpty()) {
                JOptionPane.showMessageDialog(this, "Consultation introuvable !");
                return;
            }

            Consultation existing = repCheck.getConsultations().get(0);
            if (existing.getPatientLastName() != null && !existing.getPatientLastName().isEmpty()) {
                JOptionPane.showMessageDialog(this,
                        "Cette consultation est déjà réservée par un patient : "
                                + existing.getPatientLastName() + " " + existing.getPatientFirstName(),
                        "Consultation déjà réservée",
                        JOptionPane.WARNING_MESSAGE);
                return;
            }

            String nom = JOptionPane.showInputDialog(this, "Nom du patient :");
            if (nom == null || nom.trim().isEmpty()) return;

            String prenom = JOptionPane.showInputDialog(this, "Prénom du patient :");
            if (prenom == null || prenom.trim().isEmpty()) return;

            String raison = JOptionPane.showInputDialog(this, "Raison de la consultation :");
            if (raison == null || raison.trim().isEmpty()) return;

            Patient p = new Patient(null, nom.trim(), prenom.trim(), null);
            RequeteADD_PATIENT reqPatient = new RequeteADD_PATIENT(p);
            ReponseADD_PATIENT repPatient = (ReponseADD_PATIENT) envoyerRequete(reqPatient);

            if (!repPatient.isSucces()) {
                JOptionPane.showMessageDialog(this, repPatient.getMessage(), "Erreur", JOptionPane.ERROR_MESSAGE);
                return;
            }

            java.sql.Date date = (java.sql.Date) tableConsultations.getValueAt(selectedRow, 5);
            java.sql.Time time = (java.sql.Time) tableConsultations.getValueAt(selectedRow, 6);

            Consultation c = new Consultation();
            c.setId(consultId);
            c.setPatientId(repPatient.getPatientId());
            c.setReason(raison);
            c.setDate(date);
            c.setTime(time);

            RequeteUPDATE_CONSULTATION req = new RequeteUPDATE_CONSULTATION(c);
            ReponseUPDATE_CONSULTATION rep = (ReponseUPDATE_CONSULTATION) envoyerRequete(req);

            JOptionPane.showMessageDialog(this, rep.getMessage(), "Résultat", JOptionPane.INFORMATION_MESSAGE);

            if (rep.isSucces()) {
                loadConsultations();
            }

        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "Erreur : " + e.getMessage(), "Erreur", JOptionPane.ERROR_MESSAGE);
            e.printStackTrace();
        }
    }


    private void modifierConsultation()
    {
        int selectedRow = tableConsultations.getSelectedRow();
        if (selectedRow == -1) {
            JOptionPane.showMessageDialog(this, "Sélectionnez une consultation", "Attention", JOptionPane.WARNING_MESSAGE);
            return;
        }

        Integer consultId = (Integer) tableConsultations.getValueAt(selectedRow, 0);
        JTextField txtDate = new JTextField();
        JTextField txtHeure = new JTextField();

        Object[] message = {
                "Nouvelle date (yyyy-MM-dd):", txtDate,
                "Nouvelle heure (HH:mm):", txtHeure
        };

        int option = JOptionPane.showConfirmDialog(this, message, "Modifier Consultation", JOptionPane.OK_CANCEL_OPTION);
        if (option == JOptionPane.OK_OPTION) {
            String dateStr = txtDate.getText().trim();
            String heureStr = txtHeure.getText().trim();

            if (dateStr.isEmpty() || heureStr.isEmpty()) {
                JOptionPane.showMessageDialog(this, "Veuillez remplir la date et l'heure !");
                return;
            }

            try {
                SimpleDateFormat sdfDate = new SimpleDateFormat("yyyy-MM-dd");
                SimpleDateFormat sdfTime = new SimpleDateFormat("HH:mm");

                java.util.Date utilDate = sdfDate.parse(dateStr);
                java.util.Date utilTime = sdfTime.parse(heureStr);

                java.sql.Date sqlDate = new java.sql.Date(utilDate.getTime());
                java.sql.Time sqlTime = new java.sql.Time(utilTime.getTime());

                model.entity.Consultation c = new model.entity.Consultation();
                c.setId(consultId);
                c.setDate(sqlDate);
                c.setTime(sqlTime);

                RequeteUPDATE_CONSULTATION req = new RequeteUPDATE_CONSULTATION(c);
                ReponseUPDATE_CONSULTATION rep = (ReponseUPDATE_CONSULTATION) envoyerRequete(req);

                JOptionPane.showMessageDialog(this, rep.getMessage(), "Résultat", JOptionPane.INFORMATION_MESSAGE);

                if (rep.isSucces()) {
                    loadConsultations();
                }

            } catch (Exception ex) {
                JOptionPane.showMessageDialog(this, "Erreur : " + ex.getMessage(), "Erreur", JOptionPane.ERROR_MESSAGE);
                ex.printStackTrace();
            }
        }
    }


    private void supprimerConsultation()
    {
        int selectedRow = tableConsultations.getSelectedRow();
        if (selectedRow == -1) {
            JOptionPane.showMessageDialog(this, "Sélectionnez une consultation", "Attention", JOptionPane.WARNING_MESSAGE);
            return;
        }

        Integer consultId = (Integer) tableConsultations.getValueAt(selectedRow, 0);

        int confirm = JOptionPane.showConfirmDialog(this,
                "Voulez-vous vraiment supprimer cette consultation ?",
                "Confirmer la suppression",
                JOptionPane.YES_NO_OPTION);
        if (confirm != JOptionPane.YES_OPTION) return;

        try {
            RequeteDELETE_CONSULTATION req = new RequeteDELETE_CONSULTATION(consultId);
            ReponseDELETE_CONSULTATION rep = (ReponseDELETE_CONSULTATION) envoyerRequete(req);

            JOptionPane.showMessageDialog(this, rep.getMessage(), "Résultat", JOptionPane.INFORMATION_MESSAGE);

            if (rep.isSucces()) {
                loadConsultations();
            }

        } catch (Exception ex) {
            JOptionPane.showMessageDialog(this, "Erreur serveur : " + ex.getMessage(), "Erreur", JOptionPane.ERROR_MESSAGE);
            ex.printStackTrace();
        }
    }


    private void deconnexion()
    {
        try {
            RequeteLOGOUT req = new RequeteLOGOUT(username);

            envoyerRequete(req);

            System.out.println("LOGOUT envoyé au serveur");

        } catch (Exception e) {
            System.out.println("Impossible d'envoyer LOGOUT : " + e.getMessage());
        }
        finally {

            dispose();
            System.exit(0);
        }
    }
}
