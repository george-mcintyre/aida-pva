package edu.stanford.slac.aida.test.magnet.pojo;

import org.epics.nt.NTURI;
import org.epics.pvaccess.client.rpc.RPCClientImpl;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.pv.PVStructure;

import java.util.List;

public class Example {
    public PVStructure getValues() throws RPCRequestException {
        String pvName = "DEV_DGRP:XCOR:BDES";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("micros")
                .addQueryString("units")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("micros").put("LI31-LI31");
        uri.getQuery().getStringField("units").put("1-100");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();
        return response;
    }

    public PVStructure setValues() throws RPCRequestException {
        String pvName = "MAGNETSET:BDES";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("magfunc")
                .addQueryString("value")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("magfunc").put("TRIM");
        uri.getQuery().getStringField("value").put("{\"names\": [ \"XCOR:LI31:41\"], \"values\": [ 4.0 ] }");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();
        return response;
    }

    public void setBconValues(List<String> names, List<Float> values) throws RPCRequestException {
        String pvName = "MAGNETSET:BCON";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("value")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("value").put("{\"names\": [ \"XCOR:LI31:41\"], \"values\": [ 5.0 ] }");

        RPCClientImpl client = new RPCClientImpl(pvName);
        client.request(uri.getPVStructure(), 3.0);
        client.destroy();
    }
}
