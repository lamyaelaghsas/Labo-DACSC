package view;

import protocol.ProtocoleACBP.ACBPClient;

import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class ClientAdminGUI extends JFrame {

    private JTable tableClients;
    private JButton boutonLister;
    private ACBPClient acbpClient;

    public ClientAdminGUI() {
        setTitle("Client Admin - Gestion des connexions");
        setSize(900, 650);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        // Couleurs pastel douces
        Color rosePastel = new Color(255, 209, 220);        // Rose pastel pour le bouton
        Color jaunePastel = new Color(255, 253, 208);       // Jaune pastel très clair
        Color vertPastel = new Color(220, 240, 220);        // Vert pastel clair
        Color bleuPastel = new Color(224, 240, 255);        // Bleu pastel clair
        Color backgroundColor = new Color(252, 252, 255);    // Blanc cassé
        Color borderColor = new Color(230, 230, 235);        // Gris très clair
        Color textColor = new Color(80, 80, 90);             // Gris doux

        setLayout(new BorderLayout(0, 0));
        getContentPane().setBackground(backgroundColor);

        // Panel du haut avec jaune pastel
        JPanel topPanel = new JPanel(new BorderLayout());
        topPanel.setBackground(jaunePastel);
        topPanel.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createMatteBorder(0, 0, 1, 0, borderColor),
                BorderFactory.createEmptyBorder(20, 30, 20, 30)
        ));

        JLabel titleLabel = new JLabel("Gestion des Clients Connectés");
        titleLabel.setFont(new Font("Segoe UI", Font.PLAIN, 22));
        titleLabel.setForeground(textColor);
        topPanel.add(titleLabel, BorderLayout.WEST);

        boutonLister = new JButton("Actualiser");
        boutonLister.setFont(new Font("Segoe UI", Font.PLAIN, 13));
        boutonLister.setBackground(rosePastel);
        boutonLister.setForeground(textColor);
        boutonLister.setFocusPainted(false);
        boutonLister.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(new Color(255, 182, 193), 1),
                BorderFactory.createEmptyBorder(10, 30, 10, 30)
        ));
        boutonLister.setCursor(new Cursor(Cursor.HAND_CURSOR));

        // Effet hover rose plus foncé
        boutonLister.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseEntered(java.awt.event.MouseEvent evt) {
                boutonLister.setBackground(new Color(255, 182, 193));
            }
            public void mouseExited(java.awt.event.MouseEvent evt) {
                boutonLister.setBackground(rosePastel);
            }
        });

        topPanel.add(boutonLister, BorderLayout.EAST);
        add(topPanel, BorderLayout.NORTH);

        // Table avec fond bleu pastel alterné
        tableClients = new JTable();
        tableClients.setFont(new Font("Segoe UI", Font.PLAIN, 13));
        tableClients.setRowHeight(38);
        tableClients.setGridColor(borderColor);
        tableClients.setSelectionBackground(vertPastel);
        tableClients.setSelectionForeground(textColor);
        tableClients.setShowVerticalLines(false);
        tableClients.setShowHorizontalLines(true);
        tableClients.setBackground(Color.WHITE);
        tableClients.setForeground(textColor);
        tableClients.setIntercellSpacing(new Dimension(0, 1));

        // Header avec bleu pastel
        tableClients.getTableHeader().setFont(new Font("Segoe UI", Font.BOLD, 13));
        tableClients.getTableHeader().setBackground(bleuPastel);
        tableClients.getTableHeader().setForeground(textColor);
        tableClients.getTableHeader().setPreferredSize(new Dimension(0, 42));
        tableClients.getTableHeader().setBorder(BorderFactory.createMatteBorder(0, 0, 1, 0, borderColor));

        // Centrer le contenu des cellules
        DefaultTableCellRenderer centerRenderer = new DefaultTableCellRenderer();
        centerRenderer.setHorizontalAlignment(JLabel.CENTER);
        centerRenderer.setBackground(Color.WHITE);
        centerRenderer.setForeground(textColor);

        displayClients(new ArrayList<>());

        JScrollPane scrollPane = new JScrollPane(tableClients);
        scrollPane.setBorder(BorderFactory.createEmptyBorder(20, 30, 20, 30));
        scrollPane.getViewport().setBackground(backgroundColor);
        scrollPane.setBackground(backgroundColor);
        add(scrollPane, BorderLayout.CENTER);

        // Panel du bas avec vert pastel
        JPanel bottomPanel = new JPanel(new FlowLayout(FlowLayout.CENTER));
        bottomPanel.setBackground(vertPastel);
        bottomPanel.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createMatteBorder(1, 0, 0, 0, borderColor),
                BorderFactory.createEmptyBorder(12, 0, 12, 0)
        ));

        JLabel infoLabel = new JLabel("Serveur : 192.168.157.128:50001");
        infoLabel.setFont(new Font("Segoe UI", Font.PLAIN, 12));
        infoLabel.setForeground(new Color(100, 100, 110));
        bottomPanel.add(infoLabel);

        add(bottomPanel, BorderLayout.SOUTH);

        acbpClient = new ACBPClient(); //On crée un client ACBP pour pouvoir communiquer avec le serveur

        //Bouton Actualiser :
        boutonLister.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    boutonLister.setText("Chargement...");
                    boutonLister.setEnabled(false);
                    refreshClientList();
                    boutonLister.setText("Actualiser");
                    boutonLister.setEnabled(true);
                } catch (IOException ex) {
                    System.err.println("Erreur connexion: " + ex.getMessage());
                    JOptionPane.showMessageDialog(ClientAdminGUI.this,
                            "Erreur de connexion au serveur",
                            "Erreur",
                            JOptionPane.ERROR_MESSAGE);
                    boutonLister.setText("Actualiser");
                    boutonLister.setEnabled(true);
                }
            }
        });
    }


    private void refreshClientList() throws IOException {
        //Contacte le serveur, recup la liste, puis appelle displayClient pr afficher la liste
        List<String[]> clients = acbpClient.getConnectedClients("192.168.157.128", 50001);
        displayClients(clients);
    }

    //Affichage de la liste quand on actualise :
    public void displayClients(List<String[]> clients) {
        ClientTableModel model = new ClientTableModel(clients);
        tableClients.setModel(model);

        // Appliquer le style après avoir défini le modèle
        DefaultTableCellRenderer centerRenderer = new DefaultTableCellRenderer();
        centerRenderer.setHorizontalAlignment(JLabel.CENTER);
        centerRenderer.setBackground(Color.WHITE);
        centerRenderer.setForeground(new Color(80, 80, 90));

        for (int i = 0; i < tableClients.getColumnCount(); i++) {
            tableClients.getColumnModel().getColumn(i).setCellRenderer(centerRenderer);
        }
    }

    public static void main(String[] args) { //le programme demarre ici
        // Look and Feel moderne
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (Exception e) {
            e.printStackTrace();
        }

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                ClientAdminGUI fenetre = new ClientAdminGUI();
                fenetre.setVisible(true);
            }
        });
    }
}