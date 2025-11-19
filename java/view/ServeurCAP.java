package view;

import protocol.ProtocoleCAP.*;
import ServeurGeneriqueTCP.*;
import configuration.ReadConfiguration;
import java.awt.*;
import java.io.IOException;
import java.util.*;
import javax.swing.*;
import javax.swing.table.*;

public class ServeurCAP extends javax.swing.JFrame implements Logger
{
    ThreadServeur threadServeur;
    private int port;
    private int poolSize;

    // Couleurs pastel
    private Color rosePastel = new Color(255, 209, 220);
    private Color jaunePastel = new Color(255, 253, 208);
    private Color vertPastel = new Color(220, 240, 220);
    private Color bleuPastel = new Color(224, 240, 255);
    private Color backgroundColor = new Color(252, 252, 255);
    private Color borderColor = new Color(230, 230, 235);
    private Color textColor = new Color(80, 80, 90);

    public ServeurCAP()
    {
        initComponents();
        threadServeur = null;
        loadConfiguration();
    }

    private void loadConfiguration()
    {
        ReadConfiguration.load("java/config.properties");
        port = ReadConfiguration.getInt("PORT_CONSULTATION");
        poolSize = ReadConfiguration.getInt("POOL_SIZE");
        jTextFieldPort.setText(String.valueOf(port));
        jTextFieldTaillePool.setText(String.valueOf(poolSize));
    }

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">
    private void initComponents()
    {
        buttonGroup1 = new javax.swing.ButtonGroup();
        jScrollPane1 = new javax.swing.JScrollPane();
        jTableLogs = new javax.swing.JTable();
        jLabel1 = new javax.swing.JLabel();
        jComboBoxProtocole = new javax.swing.JComboBox<>();
        jLabel2 = new javax.swing.JLabel();
        jTextFieldPort = new javax.swing.JTextField();
        jCheckBoxPool = new javax.swing.JCheckBox();
        jLabel3 = new javax.swing.JLabel();
        jTextFieldTaillePool = new javax.swing.JTextField();
        jButtonStart = new javax.swing.JButton();
        jButtonStop = new javax.swing.JButton();
        jButtonViderLogs = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("Serveur CAP");
        getContentPane().setBackground(backgroundColor);

        jTableLogs.setModel(new javax.swing.table.DefaultTableModel(
                new Object [][]
                        {

                        },
                new String []
                        {
                                "Thread", "Message"
                        }
        )
        {
            Class[] types = new Class []
                    {
                            java.lang.String.class, java.lang.String.class
                    };
            boolean[] canEdit = new boolean []
                    {
                            false, false
                    };

            public Class getColumnClass(int columnIndex)
            {
                return types [columnIndex];
            }

            public boolean isCellEditable(int rowIndex, int columnIndex)
            {
                return canEdit [columnIndex];
            }
        });
        jTableLogs.setFont(new Font("Segoe UI", Font.PLAIN, 13));
        jTableLogs.setRowHeight(30);
        jTableLogs.setGridColor(borderColor);
        jTableLogs.setSelectionBackground(vertPastel);
        jTableLogs.setSelectionForeground(textColor);
        jTableLogs.setShowVerticalLines(false);
        jTableLogs.setShowHorizontalLines(true);
        jTableLogs.setBackground(Color.WHITE);
        jTableLogs.setForeground(textColor);

        jTableLogs.getTableHeader().setFont(new Font("Segoe UI", Font.BOLD, 13));
        jTableLogs.getTableHeader().setBackground(bleuPastel);
        jTableLogs.getTableHeader().setForeground(textColor);
        jTableLogs.getTableHeader().setPreferredSize(new Dimension(0, 35));

        jScrollPane1.setViewportView(jTableLogs);
        jScrollPane1.setBorder(BorderFactory.createLineBorder(borderColor, 1));

        jLabel1.setText("Protocole :");
        jLabel1.setForeground(textColor);
        jLabel1.setFont(new Font("Segoe UI", Font.PLAIN, 13));

        jComboBoxProtocole.setModel(new javax.swing.DefaultComboBoxModel<>(new String[] { "CAP" }));
        jComboBoxProtocole.setFont(new Font("Segoe UI", Font.PLAIN, 13));

        jLabel2.setText("Port :");
        jLabel2.setForeground(textColor);
        jLabel2.setFont(new Font("Segoe UI", Font.PLAIN, 13));

        jTextFieldPort.setText("50002");
        jTextFieldPort.setFont(new Font("Segoe UI", Font.PLAIN, 13));

        jCheckBoxPool.setSelected(true);
        jCheckBoxPool.setText("Pool");
        jCheckBoxPool.setForeground(textColor);
        jCheckBoxPool.setFont(new Font("Segoe UI", Font.PLAIN, 13));
        jCheckBoxPool.setBackground(backgroundColor);

        jLabel3.setText("Taille Pool :");
        jLabel3.setForeground(textColor);
        jLabel3.setFont(new Font("Segoe UI", Font.PLAIN, 13));

        jTextFieldTaillePool.setText("5");
        jTextFieldTaillePool.setFont(new Font("Segoe UI", Font.PLAIN, 13));

        jButtonStart.setText("Start");
        jButtonStart.setFont(new Font("Segoe UI", Font.PLAIN, 13));
        jButtonStart.setBackground(vertPastel);
        jButtonStart.setForeground(textColor);
        jButtonStart.setFocusPainted(false);
        jButtonStart.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(borderColor, 1),
                BorderFactory.createEmptyBorder(8, 20, 8, 20)
        ));
        jButtonStart.setCursor(new Cursor(Cursor.HAND_CURSOR));
        jButtonStart.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseEntered(java.awt.event.MouseEvent evt) {
                jButtonStart.setBackground(new Color(200, 230, 200));
            }
            public void mouseExited(java.awt.event.MouseEvent evt) {
                jButtonStart.setBackground(vertPastel);
            }
        });
        jButtonStart.addActionListener(new java.awt.event.ActionListener()
        {
            public void actionPerformed(java.awt.event.ActionEvent evt)
            {
                jButtonStartActionPerformed(evt);
            }
        });

        jButtonStop.setText("Stop");
        jButtonStop.setFont(new Font("Segoe UI", Font.PLAIN, 13));
        jButtonStop.setBackground(new Color(255, 220, 220));
        jButtonStop.setForeground(textColor);
        jButtonStop.setEnabled(false);
        jButtonStop.setFocusPainted(false);
        jButtonStop.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(borderColor, 1),
                BorderFactory.createEmptyBorder(8, 20, 8, 20)
        ));
        jButtonStop.setCursor(new Cursor(Cursor.HAND_CURSOR));
        jButtonStop.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseEntered(java.awt.event.MouseEvent evt) {
                if (jButtonStop.isEnabled()) {
                    jButtonStop.setBackground(new Color(255, 200, 200));
                }
            }
            public void mouseExited(java.awt.event.MouseEvent evt) {
                jButtonStop.setBackground(new Color(255, 220, 220));
            }
        });
        jButtonStop.addActionListener(new java.awt.event.ActionListener()
        {
            public void actionPerformed(java.awt.event.ActionEvent evt)
            {
                jButtonStopActionPerformed(evt);
            }
        });

        jButtonViderLogs.setText("Vider Logs");
        jButtonViderLogs.setFont(new Font("Segoe UI", Font.PLAIN, 13));
        jButtonViderLogs.setBackground(jaunePastel);
        jButtonViderLogs.setForeground(textColor);
        jButtonViderLogs.setFocusPainted(false);
        jButtonViderLogs.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(borderColor, 1),
                BorderFactory.createEmptyBorder(8, 20, 8, 20)
        ));
        jButtonViderLogs.setCursor(new Cursor(Cursor.HAND_CURSOR));
        jButtonViderLogs.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseEntered(java.awt.event.MouseEvent evt) {
                jButtonViderLogs.setBackground(new Color(255, 250, 180));
            }
            public void mouseExited(java.awt.event.MouseEvent evt) {
                jButtonViderLogs.setBackground(jaunePastel);
            }
        });
        jButtonViderLogs.addActionListener(new java.awt.event.ActionListener()
        {
            public void actionPerformed(java.awt.event.ActionEvent evt)
            {
                jButtonViderLogsActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
                layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addGroup(layout.createSequentialGroup()
                                .addGap(20, 20, 20)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                        .addComponent(jScrollPane1)
                                        .addGroup(layout.createSequentialGroup()
                                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                                        .addGroup(layout.createSequentialGroup()
                                                                .addComponent(jLabel1)
                                                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                                .addComponent(jComboBoxProtocole, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                                .addGap(18, 18, 18)
                                                                .addComponent(jLabel2)
                                                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                                .addComponent(jTextFieldPort, javax.swing.GroupLayout.PREFERRED_SIZE, 80, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                                .addGap(18, 18, 18)
                                                                .addComponent(jCheckBoxPool)
                                                                .addGap(18, 18, 18)
                                                                .addComponent(jLabel3)
                                                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                                .addComponent(jTextFieldTaillePool, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE))
                                                        .addGroup(layout.createSequentialGroup()
                                                                .addComponent(jButtonStart, javax.swing.GroupLayout.PREFERRED_SIZE, 80, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                                .addGap(10, 10, 10)
                                                                .addComponent(jButtonStop, javax.swing.GroupLayout.PREFERRED_SIZE, 80, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                                .addGap(10, 10, 10)
                                                                .addComponent(jButtonViderLogs, javax.swing.GroupLayout.PREFERRED_SIZE, 100, javax.swing.GroupLayout.PREFERRED_SIZE)))
                                                .addGap(0, 150, Short.MAX_VALUE)))
                                .addGap(20, 20, 20))
        );
        layout.setVerticalGroup(
                layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                                .addGap(20, 20, 20)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                        .addComponent(jLabel1)
                                        .addComponent(jComboBoxProtocole, javax.swing.GroupLayout.PREFERRED_SIZE, 30, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addComponent(jLabel2)
                                        .addComponent(jTextFieldPort, javax.swing.GroupLayout.PREFERRED_SIZE, 30, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addComponent(jCheckBoxPool)
                                        .addComponent(jLabel3)
                                        .addComponent(jTextFieldTaillePool, javax.swing.GroupLayout.PREFERRED_SIZE, 30, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addGap(20, 20, 20)
                                .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 350, Short.MAX_VALUE)
                                .addGap(15, 15, 15)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                        .addComponent(jButtonStart)
                                        .addComponent(jButtonStop)
                                        .addComponent(jButtonViderLogs))
                                .addGap(20, 20, 20))
        );

        pack();
    }// </editor-fold>

    //BOUTON START :
    private void jButtonStartActionPerformed(java.awt.event.ActionEvent evt)
    {
        try
        {
            Protocole protocole = null;
            if (((String)jComboBoxProtocole.getSelectedItem()).equals("CAP"))
                // Création du protocole :
                protocole = new CAP(this);
            int port = Integer.parseInt(jTextFieldPort.getText());
            if (jCheckBoxPool.isSelected())
            {
                int taillePool = Integer.parseInt(jTextFieldTaillePool.getText());
                // Création du serveur :
                threadServeur = new ThreadServeurPool(port,protocole,taillePool,this);
            }
            videLogs();
            //Lancement :
            threadServeur.start();
            jButtonStart.setEnabled(false);
            jButtonStop.setEnabled(true);
        }
        catch (NumberFormatException ex)
        {
            JOptionPane.showMessageDialog(this,"Erreur de Port et/ou taille Pool !","Erreur...",JOptionPane.ERROR_MESSAGE);
        }
        catch (IOException ex)
        {
            JOptionPane.showMessageDialog(this,"Erreur I/O !","Erreur...",JOptionPane.ERROR_MESSAGE);
        }
    }

    private void jButtonStopActionPerformed(java.awt.event.ActionEvent evt)
    {
        threadServeur.interrupt();
        jButtonStart.setEnabled(true);
        jButtonStop.setEnabled(false);
    }

    private void jButtonViderLogsActionPerformed(java.awt.event.ActionEvent evt)
    {
        videLogs();
    }

    public static void main(String args[])
    {
        try
        {
            javax.swing.UIManager.setLookAndFeel(javax.swing.UIManager.getSystemLookAndFeelClassName());
        }
        catch (Exception ex)
        {
            ex.printStackTrace();
        }

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable()
        {
            public void run()
            {
                new ServeurCAP().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify
    private javax.swing.ButtonGroup buttonGroup1;
    private javax.swing.JButton jButtonStart;
    private javax.swing.JButton jButtonStop;
    private javax.swing.JButton jButtonViderLogs;
    private javax.swing.JCheckBox jCheckBoxPool;
    private javax.swing.JComboBox<String> jComboBoxProtocole;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JTable jTableLogs;
    private javax.swing.JTextField jTextFieldPort;
    private javax.swing.JTextField jTextFieldTaillePool;
    // End of variables declaration

    @Override
    public synchronized void Trace(String message)
    {
        DefaultTableModel modele = (DefaultTableModel) jTableLogs.getModel();
        Vector<String> ligne = new Vector<>();
        ligne.add(Thread.currentThread().getName());
        ligne.add(message);
        modele.insertRow(modele.getRowCount(),ligne);
    }

    private void videLogs()
    {
        DefaultTableModel modele = (DefaultTableModel) jTableLogs.getModel();
        modele.setRowCount(0);
    }
}