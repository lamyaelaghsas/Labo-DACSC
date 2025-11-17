package view;

import protocol.ProtocoleCAP.*;
import ServeurGeneriqueTCP.*;
import configuration.ReadConfiguration;
import java.io.IOException;
import java.util.*;
import javax.swing.*;
import javax.swing.table.*;

public class ServeurCAP extends javax.swing.JFrame implements Logger
{
    ThreadServeur threadServeur;
    private int port;
    private int poolSize;

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
        jScrollPane1.setViewportView(jTableLogs);

        jLabel1.setText("Protocole :");

        jComboBoxProtocole.setModel(new javax.swing.DefaultComboBoxModel<>(new String[] { "CAP" }));

        jLabel2.setText("Port :");

        jTextFieldPort.setText("50002");

        jCheckBoxPool.setSelected(true);
        jCheckBoxPool.setText("Pool");

        jLabel3.setText("Taille Pool :");

        jTextFieldTaillePool.setText("5");

        jButtonStart.setText("Start");
        jButtonStart.addActionListener(new java.awt.event.ActionListener()
        {
            public void actionPerformed(java.awt.event.ActionEvent evt)
            {
                jButtonStartActionPerformed(evt);
            }
        });

        jButtonStop.setText("Stop");
        jButtonStop.setEnabled(false);
        jButtonStop.addActionListener(new java.awt.event.ActionListener()
        {
            public void actionPerformed(java.awt.event.ActionEvent evt)
            {
                jButtonStopActionPerformed(evt);
            }
        });

        jButtonViderLogs.setText("Vider Logs");
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
                                .addContainerGap()
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
                                                                .addComponent(jButtonStart)
                                                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                                .addComponent(jButtonStop)
                                                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                                .addComponent(jButtonViderLogs)))
                                                .addGap(0, 150, Short.MAX_VALUE)))
                                .addContainerGap())
        );
        layout.setVerticalGroup(
                layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                                .addContainerGap()
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                        .addComponent(jLabel1)
                                        .addComponent(jComboBoxProtocole, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addComponent(jLabel2)
                                        .addComponent(jTextFieldPort, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addComponent(jCheckBoxPool)
                                        .addComponent(jLabel3)
                                        .addComponent(jTextFieldTaillePool, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                                .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 350, Short.MAX_VALUE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                        .addComponent(jButtonStart)
                                        .addComponent(jButtonStop)
                                        .addComponent(jButtonViderLogs))
                                .addContainerGap())
        );

        pack();
    }// </editor-fold>

    private void jButtonStartActionPerformed(java.awt.event.ActionEvent evt)
    {
        try
        {
            Protocole protocole = null;
            if (((String)jComboBoxProtocole.getSelectedItem()).equals("CAP"))
                protocole = new CAP(this);
            int port = Integer.parseInt(jTextFieldPort.getText());
            if (jCheckBoxPool.isSelected())
            {
                int taillePool = Integer.parseInt(jTextFieldTaillePool.getText());
                threadServeur = new ThreadServeurPool(port,protocole,taillePool,this);
            }
            videLogs();
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
            java.util.logging.Logger.getLogger(ServeurCAP.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        catch (InstantiationException ex)
        {
            java.util.logging.Logger.getLogger(ServeurCAP.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        catch (IllegalAccessException ex)
        {
            java.util.logging.Logger.getLogger(ServeurCAP.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        catch (javax.swing.UnsupportedLookAndFeelException ex)
        {
            java.util.logging.Logger.getLogger(ServeurCAP.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
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