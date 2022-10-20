package edu.stanford.slac.aida.test.buffacq.pvaclient;

import org.epics.nt.NTURI;
import org.epics.pvaClient.PvaClient;
import org.epics.pvaClient.PvaClientChannel;
import org.epics.pvdata.pv.PVStructure;

public class Example {
    public PVStructure getTable() {
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

        PvaClient client = PvaClient.get();
        PvaClientChannel channel = client.channel(pvName, "pva", 30.0);
        return channel.rpc(uri.getPVStructure());
    }
}
