package edu.stanford.slac.aida.test.klystron.aidava;

import edu.stanford.slac.aida.client.PvaTable;
import org.epics.pvaccess.server.rpc.RPCRequestException;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.pvaRequest;
import static edu.stanford.slac.aida.client.AidaPvaClientUtils.pvaSet;
import static edu.stanford.slac.aida.client.AidaType.*;

public class Example {
    public Short getStatusShort() throws RPCRequestException {
        return (Short) pvaRequest("KLYS:LI31:31:TACT")
                .with("BEAM", 8)
                .with("DGRP", "DEV_DGRP")
                .returning(AIDA_SHORT)
                .get();
    }

    public Long getStatusLong() throws RPCRequestException {
        return (Long) pvaRequest("KLYS:LI31:31:TACT")
                .with("BEAM", 8)
                .with("DGRP", "DEV_DGRP")
                .returning(AIDA_LONG)
                .get();
    }

    public String getStatusString() throws RPCRequestException {
        return (String) pvaRequest("KLYS:LI31:31:TACT")
                .with("BEAM", 8)
                .with("DGRP", "DEV_DGRP")
                .returning(AIDA_STRING)
                .get();
    }

    public PvaTable getStatusTable() throws RPCRequestException {
        return (PvaTable) pvaRequest("KLYS:LI31:31:TACT")
                .with("BEAM", 8)
                .with("DGRP", "DEV_DGRP")
                .returning(AIDA_TABLE)
                .get();
    }

    public PvaTable getStatuses(Integer beamCode, List<String> devices) throws RPCRequestException {
        return (PvaTable) pvaRequest("KLYSTRONGET:TACT")
                .with("BEAM", beamCode)
                .with("DEVICES", devices)
                .get();
    }

    public PvaTable activateKlystron(Integer beamCode, Boolean activateStatus) throws RPCRequestException {
        return pvaRequest("KLYS:LI31:31:TACT")
                .with("BEAM", beamCode)
                .set(activateStatus);
    }

    public PvaTable setPdes(Float value) throws RPCRequestException {
        return pvaRequest("KLYS:LI31:31:PDES").set(value);
    }

    public PvaTable setKhpr(Float value) throws RPCRequestException {
        return pvaRequest("KLYS:LI31:31:KPHR").set(value);
    }

    public void setPconAcon(Float value) throws RPCRequestException {
        pvaSet("KLYS:LI31:31:PCON", value);
        pvaSet("KLYS:LI31:31:ACON", value);
    }

    public void setPmdls(List<String>subBoosters, List<Float> values) throws RPCRequestException {
        Map<String, List<?>> args = new HashMap<String, List<?>>();
        args.put("names", subBoosters);
        args.put("values", values);
        pvaSet("KLYSTRONSET:PMDL", args);
    }
}
