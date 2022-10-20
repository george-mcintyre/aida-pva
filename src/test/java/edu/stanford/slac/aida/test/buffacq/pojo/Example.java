package edu.stanford.slac.aida.test.buffacq.pojo;

import org.epics.nt.NTURI;
import org.epics.pvaccess.client.rpc.RPCClientImpl;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.pv.PVStructure;

public class Example {
    public PVStructure getTable() throws RPCRequestException {
        String pvName = "NDRFACET:BUFFACQ";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("bpmd")
                .addQueryString("bpms")
                .addQueryInt("nrpos")
                .addQueryInt("timeout")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("bpmd").put(57);
        uri.getQuery().getStringField("bpms").put("[\"BPMS:LI02:501\", \"BPMS:DR12:334\"]");
        uri.getQuery().getIntField("nrpos").put(10);
        uri.getQuery().getIntField("timeout").put(30);

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 30.0);
        client.destroy();
        return response;
    }
}
