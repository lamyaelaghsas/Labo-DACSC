package view;

import javax.swing.table.AbstractTableModel;
import java.util.ArrayList;
import java.util.List;

public class ClientTableModel extends AbstractTableModel {

    private List<String[]> clients;
    private final String[] columnNames = {"IP", "Nom", "Prénom", "N° Patient"};


    //---------------CONSTRUCTEUR---------------
    public ClientTableModel(List<String[]> clients) {
        this.clients = new ArrayList<>(clients);
    }

    //---------------MÉTHODES AbstractTableModel---------------
    @Override
    public int getRowCount() {
        return clients.size();
    }

    @Override
    public int getColumnCount() {
        return columnNames.length;
    }

    @Override
    public String getColumnName(int column) {
        return columnNames[column];
    }

    @Override
    public Object getValueAt(int rowIndex, int columnIndex) {
        String[] client = clients.get(rowIndex);
        return client[columnIndex];
    }
}