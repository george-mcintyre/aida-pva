package edu.stanford.slac.aida.test.bpm.aidava;

import edu.stanford.slac.aida.client.PvaTable;
import org.epics.pvaccess.server.rpc.RPCRequestException;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.pvaRequest;

public class Example {
    public PvaTable getTable() throws RPCRequestException {
        return (PvaTable) pvaRequest("P2BPMHER:BPMS")
                .with("BPMD", 38)
                .with("CNFTYPE", "GOLD")
                .with("N", 1024)
                .get();
    }
}
