package edu.stanford.slac.aida.test.magnet.aidava;

import edu.stanford.slac.aida.client.PvaTable;
import org.epics.pvaccess.server.rpc.RPCRequestException;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.pvaRequest;

public class Example {
    public PvaTable getValues() throws RPCRequestException {
        return (PvaTable) pvaRequest("DEV_DGRP:XCOR:BDES")
                .with("MICROS", "LI31-LI31")
                .with("UNITS", "1-100")
                .get();
    }

    public PvaTable setValues(List<String> names, List<Float> values) throws RPCRequestException {
        Map<String, List<?>> value = new HashMap<String, List<?>>();
        value.put("names", names);
        value.put("values", values);
        return (PvaTable) pvaRequest("MAGNETSET:BDES")
                .with("MAGFUNC", "TRIM")
                .set(value);
    }

    public void setBconValues(List<String> names, List<Float> values) throws RPCRequestException {
        Map<String, List<?>> value = new HashMap<String, List<?>>();
        value.put("names", names);
        value.put("values", values);
        pvaRequest("MAGNETSET:BCON").set(value);
    }
}
