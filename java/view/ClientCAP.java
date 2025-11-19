package view;

import protocol.ProtocoleCAP.*;
import java.io.*;
import java.net.Socket;
import javax.swing.JOptionPane;
import java.awt.*;
import javax.swing.*;

public class ClientCAP extends javax.swing.JFrame
{
    private Socket socket;
    private String login;
    private ObjectOutputStream oos; //RequeteLOGIN
    private ObjectInputStream ois; //ReponseLOGIN

    // Couleurs pastel
    private final Color rosePastel = new Color(255, 209, 220);
    private final Color jaunePastel = new Color(255, 253, 208);
    private final Color vertPastel = new Color(220, 240, 220);
    private final Color bleuPastel = new Color(224, 240, 255);
    private final Color backgroundColor = new Color(255, 250, 255);
    private final Color borderColor = new Color(240, 220, 240);
    private final Color textColor = new Color(100, 80, 100);

    //Constructeur
    public ClientCAP()
    {
        initComponents();
        applyCustomStyling();
        oos = null;
        ois = null;
    }

    private void applyCustomStyling()
    {
        // Fond général
        getContentPane().setBackground(backgroundColor);

        // Style du titre
        jLabelTitle.setForeground(textColor);
        jLabelTitle.setFont(new Font("Segoe UI", Font.BOLD, 18));

        // Style des labels
        styleLabel(jLabelIP);
        styleLabel(jLabelPort);
        styleLabel(jLabelUsername);
        styleLabel(jLabelPassword);

        // Style des champs de texte
        styleTextField(jTextFieldIP);
        styleTextField(jTextFieldPort);
        styleTextField(jTextFieldUsername);
        stylePasswordField(jPasswordFieldPassword);

        // Style des boutons
        styleButton(jButtonLogin, vertPastel);
        styleButton(jButtonLogout, rosePastel);
    }

    private void styleLabel(JLabel label)
    {
        label.setForeground(textColor);
        label.setFont(new Font("Segoe UI", Font.PLAIN, 12));
    }

    private void styleTextField(JTextField textField)
    {
        textField.setBackground(Color.WHITE);
        textField.setForeground(textColor);
        textField.setFont(new Font("Segoe UI", Font.PLAIN, 12));
        textField.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(borderColor, 2),
                BorderFactory.createEmptyBorder(6, 10, 6, 10)
        ));
    }

    private void stylePasswordField(JPasswordField passwordField)
    {
        passwordField.setBackground(Color.WHITE);
        passwordField.setForeground(textColor);
        passwordField.setFont(new Font("Segoe UI", Font.PLAIN, 12));
        passwordField.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(borderColor, 2),
                BorderFactory.createEmptyBorder(6, 10, 6, 10)
        ));
    }

    private void styleButton(JButton button, Color bgColor)
    {
        button.setBackground(bgColor);
        button.setForeground(textColor);
        button.setFocusPainted(false);
        button.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(new Color(255, 192, 203), 2),
                BorderFactory.createEmptyBorder(10, 20, 10, 20)
        ));
        button.setCursor(new Cursor(Cursor.HAND_CURSOR));
        button.setFont(new Font("Segoe UI", Font.BOLD, 12));

        // Effet hover
        button.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseEntered(java.awt.event.MouseEvent evt) {
                if (button.isEnabled()) {
                    button.setBackground(bgColor.darker());
                }
            }
            public void mouseExited(java.awt.event.MouseEvent evt) {
                button.setBackground(bgColor);
            }
        });
    }

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">
    private void initComponents()
    {
        jLabelTitle = new javax.swing.JLabel();
        jLabelIP = new javax.swing.JLabel();
        jTextFieldIP = new javax.swing.JTextField();
        jLabelPort = new javax.swing.JLabel();
        jTextFieldPort = new javax.swing.JTextField();
        jLabelUsername = new javax.swing.JLabel();
        jTextFieldUsername = new javax.swing.JTextField();
        jLabelPassword = new javax.swing.JLabel();
        jPasswordFieldPassword = new javax.swing.JPasswordField();
        jButtonLogin = new javax.swing.JButton();
        jButtonLogout = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("ClientCAP - Consultation");

        jLabelTitle.setFont(new java.awt.Font("Tahoma", 1, 14));
        jLabelTitle.setText("Connexion Médecin");

        jLabelIP.setText("IP Serveur:");

        jTextFieldIP.setText("127.0.0.1");

        jLabelPort.setText("Port Serveur:");

        jTextFieldPort.setText("50002");

        jLabelUsername.setText("Login:");

        jLabelPassword.setText("Password:");

        jButtonLogin.setText("Login");
        jButtonLogin.addActionListener(new java.awt.event.ActionListener()
        {
            public void actionPerformed(java.awt.event.ActionEvent evt)
            {
                jButtonLoginActionPerformed(evt);
            }
        });

        jButtonLogout.setText("Logout");
        jButtonLogout.setEnabled(false);
        jButtonLogout.addActionListener(new java.awt.event.ActionListener()
        {
            public void actionPerformed(java.awt.event.ActionEvent evt)
            {
                jButtonLogoutActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
                layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addGroup(layout.createSequentialGroup()
                                .addGap(40, 40, 40)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                        .addComponent(jLabelTitle)
                                        .addGroup(layout.createSequentialGroup()
                                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                                        .addComponent(jLabelIP)
                                                        .addComponent(jLabelPort)
                                                        .addComponent(jLabelUsername)
                                                        .addComponent(jLabelPassword))
                                                .addGap(18, 18, 18)
                                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                                                        .addComponent(jTextFieldIP)
                                                        .addComponent(jTextFieldPort)
                                                        .addComponent(jTextFieldUsername)
                                                        .addComponent(jPasswordFieldPassword, javax.swing.GroupLayout.DEFAULT_SIZE, 220, Short.MAX_VALUE)))
                                        .addGroup(layout.createSequentialGroup()
                                                .addComponent(jButtonLogin, javax.swing.GroupLayout.PREFERRED_SIZE, 120, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                .addGap(18, 18, 18)
                                                .addComponent(jButtonLogout, javax.swing.GroupLayout.PREFERRED_SIZE, 120, javax.swing.GroupLayout.PREFERRED_SIZE)))
                                .addContainerGap(40, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
                layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addGroup(layout.createSequentialGroup()
                                .addGap(30, 30, 30)
                                .addComponent(jLabelTitle)
                                .addGap(25, 25, 25)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                        .addComponent(jLabelIP)
                                        .addComponent(jTextFieldIP, javax.swing.GroupLayout.PREFERRED_SIZE, 32, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addGap(12, 12, 12)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                        .addComponent(jLabelPort)
                                        .addComponent(jTextFieldPort, javax.swing.GroupLayout.PREFERRED_SIZE, 32, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addGap(12, 12, 12)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                        .addComponent(jLabelUsername)
                                        .addComponent(jTextFieldUsername, javax.swing.GroupLayout.PREFERRED_SIZE, 32, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addGap(12, 12, 12)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                        .addComponent(jLabelPassword)
                                        .addComponent(jPasswordFieldPassword, javax.swing.GroupLayout.PREFERRED_SIZE, 32, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addGap(25, 25, 25)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                        .addComponent(jButtonLogin, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addComponent(jButtonLogout, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addContainerGap(35, Short.MAX_VALUE))
        );

        pack();
        setLocationRelativeTo(null);
    }// </editor-fold>

    private void jButtonLoginActionPerformed(java.awt.event.ActionEvent evt) //LOGIN
    {
        String ipServeur = jTextFieldIP.getText();
        int portServeur = Integer.parseInt(jTextFieldPort.getText());
        String username = jTextFieldUsername.getText();
        String password = new String(jPasswordFieldPassword.getPassword());

        try
        {
            socket = new Socket(ipServeur,portServeur); //on etablit une connexion avec serveur avec l'ip et le port saisis
            RequeteLOGIN requete = new RequeteLOGIN(username,password);
            oos = new ObjectOutputStream(socket.getOutputStream());//pour envoyer des objets vers le serveur
            ois = new ObjectInputStream(socket.getInputStream());//pour lire des objets venant du serveur
            oos.writeObject(requete); //On envoie l’objet RequeteLOGIN au serveur à travers le socket
            ReponseLOGIN reponse = (ReponseLOGIN) ois.readObject();//On envoie l’objet RequeteLOGIN au serveur à travers le socket

            //Si le serveur a validé le login
            if (reponse.isValide())
            {
                this.login = username;
                int doctorId = reponse.getDoctorId(); //récupère l’ID du médecin, envoyé par le serveur

                //this.setVisible(false);//on cache la fenetre de login
                new ClientConsultationGUI(username, doctorId);//on ouvre la fenetre ClientConsultationGUI
            }
            else
            {
                JOptionPane.showMessageDialog(this,"Erreur de login !","Erreur...",JOptionPane.ERROR_MESSAGE);
                socket.close();
            }
        }
        catch (IOException | ClassNotFoundException ex)
        {
            JOptionPane.showMessageDialog(this,"Problème de connexion !","Erreur...",JOptionPane.ERROR_MESSAGE);
        }
    }

    private void jButtonLogoutActionPerformed(java.awt.event.ActionEvent evt) //LOGOUT
    {
        try
        {
            RequeteLOGOUT requete = new RequeteLOGOUT(login);
            oos.writeObject(requete);
            oos.close();
            ois.close();
            socket.close();
            jButtonLogin.setEnabled(true);
            jButtonLogout.setEnabled(false);
        }
        catch (IOException ex)
        {
            JOptionPane.showMessageDialog(this,"Problème de connexion !","Erreur...",JOptionPane.ERROR_MESSAGE);
        }
    }


    public static void main(String args[])
    {
        try
        {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels())
            {
                if ("Nimbus".equals(info.getName())) //THEME
                {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        }
        catch (ClassNotFoundException ex)
        {
            java.util.logging.Logger.getLogger(ClientCAP.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        catch (InstantiationException ex)
        {
            java.util.logging.Logger.getLogger(ClientCAP.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        catch (IllegalAccessException ex)
        {
            java.util.logging.Logger.getLogger(ClientCAP.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        catch (javax.swing.UnsupportedLookAndFeelException ex)
        {
            java.util.logging.Logger.getLogger(ClientCAP.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable()
        {
            public void run()
            {
                new ClientCAP().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify
    private javax.swing.JButton jButtonLogin;
    private javax.swing.JButton jButtonLogout;
    private javax.swing.JLabel jLabelTitle;
    private javax.swing.JLabel jLabelIP;
    private javax.swing.JLabel jLabelPort;
    private javax.swing.JLabel jLabelUsername;
    private javax.swing.JLabel jLabelPassword;
    private javax.swing.JTextField jTextFieldIP;
    private javax.swing.JTextField jTextFieldPort;
    private javax.swing.JTextField jTextFieldUsername;
    private javax.swing.JPasswordField jPasswordFieldPassword;
    // End of variables declaration
}