package edu.stanford.slac.aida.test.bpm.pojo;

import org.epics.nt.NTURI;
import org.epics.pvaccess.client.rpc.RPCClientImpl;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.pv.PVStructure;

public class Example {
    public PVStructure getTable() throws RPCRequestException {
        String pvName = "P2BPMHER:BPMS";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("bpmd")
                .addQueryString("cnftype")
                .addQueryInt("n")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("bpmd").put(38);
        uri.getQuery().getStringField("cnftype").put("GOLD");
        uri.getQuery().getIntField("n").put(1024);

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();
        return response;
    }
}
