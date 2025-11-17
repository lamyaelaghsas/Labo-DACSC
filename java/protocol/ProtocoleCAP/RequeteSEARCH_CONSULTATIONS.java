package protocol.ProtocoleCAP;

import ServeurGeneriqueTCP.*;
import model.viewmodel.ConsultationSearchVM;
import java.io.Serializable;

public class RequeteSEARCH_CONSULTATIONS implements Requete, Serializable
{
    private ConsultationSearchVM searchVM;

    public RequeteSEARCH_CONSULTATIONS(ConsultationSearchVM vm)
    {
        searchVM = vm;
    }

    public ConsultationSearchVM getSearchVM()
    {
        return searchVM;
    }
}