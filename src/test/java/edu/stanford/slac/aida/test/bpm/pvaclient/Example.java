package edu.stanford.slac.aida.test.bpm.pvaclient;

import org.epics.nt.NTURI;
import org.epics.pvaClient.PvaClient;
import org.epics.pvaClient.PvaClientChannel;
import org.epics.pvdata.pv.PVStructure;

public class Example {
    public PVStructure getTable() {
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

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        return channel.rpc(uri.getPVStructure());
    }
}
