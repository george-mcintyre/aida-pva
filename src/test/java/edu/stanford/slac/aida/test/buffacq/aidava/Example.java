package edu.stanford.slac.aida.test.buffacq.aidava;

import java.util.Arrays;
import java.util.List;
import edu.stanford.slac.aida.client.PvaTable;
import org.epics.pvaccess.server.rpc.RPCRequestException;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.pvaRequest;

public class Example {
    public PvaTable getTable() throws RPCRequestException {
        return (PvaTable) pvaRequest("NDRFACET:BUFFACQ")
                .with("BPMD", 57)
                .with("NRPOS", 10)
                .with("BPMS", Arrays.asList("BPMS:LI02:501", "BPMS:DR12:334"))
                .timeout(30)
                .get();
    }
}
