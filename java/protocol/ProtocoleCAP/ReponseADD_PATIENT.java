package protocol.ProtocoleCAP;

import ServeurGeneriqueTCP.*;
import java.io.Serializable;

public class ReponseADD_PATIENT implements Reponse, Serializable {
    private boolean succes;
    private String message;
    private Integer patientId;

    public ReponseADD_PATIENT(boolean s, String m, Integer id) {
        succes = s;
        message = m;
        patientId = id;
    }

    public boolean isSucces() { return succes; }
    public String getMessage() { return message; }
    public Integer getPatientId() { return patientId; }
}
