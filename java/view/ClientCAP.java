package view;

import protocol.ProtocoleCAP.*;
import java.io.*;
import java.net.Socket;
import javax.swing.JOptionPane;

public class ClientCAP extends javax.swing.JFrame
{
    private Socket socket;
    private String login;
    private ObjectOutputStream oos;
    private ObjectInputStream ois;

    public ClientCAP()
    {
        initComponents();
        oos = null;
        ois = null;
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
        setTitle("DACSC - Client Consultation");

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
                                .addGap(30, 30, 30)
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
                                                        .addComponent(jPasswordFieldPassword, javax.swing.GroupLayout.DEFAULT_SIZE, 200, Short.MAX_VALUE)))
                                        .addGroup(layout.createSequentialGroup()
                                                .addComponent(jButtonLogin, javax.swing.GroupLayout.PREFERRED_SIZE, 100, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                .addGap(18, 18, 18)
                                                .addComponent(jButtonLogout, javax.swing.GroupLayout.PREFERRED_SIZE, 100, javax.swing.GroupLayout.PREFERRED_SIZE)))
                                .addContainerGap(30, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
                layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addGroup(layout.createSequentialGroup()
                                .addGap(20, 20, 20)
                                .addComponent(jLabelTitle)
                                .addGap(18, 18, 18)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                        .addComponent(jLabelIP)
                                        .addComponent(jTextFieldIP, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addGap(10, 10, 10)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                        .addComponent(jLabelPort)
                                        .addComponent(jTextFieldPort, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addGap(10, 10, 10)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                        .addComponent(jLabelUsername)
                                        .addComponent(jTextFieldUsername, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addGap(10, 10, 10)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                        .addComponent(jLabelPassword)
                                        .addComponent(jPasswordFieldPassword, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addGap(20, 20, 20)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                        .addComponent(jButtonLogin)
                                        .addComponent(jButtonLogout))
                                .addContainerGap(30, Short.MAX_VALUE))
        );

        pack();
        setLocationRelativeTo(null);
    }// </editor-fold>

    private void jButtonLoginActionPerformed(java.awt.event.ActionEvent evt)
    {
        String ipServeur = jTextFieldIP.getText();
        int portServeur = Integer.parseInt(jTextFieldPort.getText());
        String username = jTextFieldUsername.getText();
        String password = new String(jPasswordFieldPassword.getPassword());

        try
        {
            socket = new Socket(ipServeur,portServeur);
            RequeteLOGIN requete = new RequeteLOGIN(username,password);
            oos = new ObjectOutputStream(socket.getOutputStream());
            ois = new ObjectInputStream(socket.getInputStream());
            oos.writeObject(requete);
            ReponseLOGIN reponse = (ReponseLOGIN) ois.readObject();

            if (reponse.isValide())
            {
                this.login = username;
                int doctorId = reponse.getDoctorId();

                this.setVisible(false);
                new ClientConsultationGUI(username, doctorId);
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

    private void jButtonLogoutActionPerformed(java.awt.event.ActionEvent evt)
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
                if ("Nimbus".equals(info.getName()))
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