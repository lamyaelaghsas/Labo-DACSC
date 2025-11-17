package view;

import protocol.ProtocoleACBP.ACBPClient;

import javax.swing.*;
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
        setSize(800, 600);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        setLayout(new BorderLayout());

        boutonLister = new JButton("Lister les clients connectés");
        add(boutonLister, BorderLayout.NORTH);

        tableClients = new JTable();
        displayClients(new ArrayList<>());

        JScrollPane scrollPane = new JScrollPane(tableClients);
        add(scrollPane, BorderLayout.CENTER);

        acbpClient = new ACBPClient();

        boutonLister.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    refreshClientList();
                } catch (IOException ex) {
                    System.err.println("Erreur connexion: " + ex.getMessage());
                }
            }
        });
    }

    private void refreshClientList() throws IOException {
        List<String[]> clients = acbpClient.getConnectedClients("192.168.157.128 ", 50001);
        displayClients(clients);
    }

    public void displayClients(List<String[]> clients) {
        ClientTableModel model = new ClientTableModel(clients);
        tableClients.setModel(model);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                ClientAdminGUI fenetre = new ClientAdminGUI();
                fenetre.setVisible(true);
            }
        });
    }
}