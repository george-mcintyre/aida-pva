package edu.stanford.slac.aida.test.magnet.pvaclient;

import org.epics.nt.NTURI;
import org.epics.pvaClient.PvaClient;
import org.epics.pvaClient.PvaClientChannel;
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

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        return channel.rpc(uri.getPVStructure());
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

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        return channel.rpc(uri.getPVStructure());
    }

    public void setBconValues(List<String> names, List<Float> values) throws RPCRequestException {
        String pvName = "MAGNETSET:BCON";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("value")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("value").put("{\"names\": [ \"XCOR:LI31:41\"], \"values\": [ 5.0 ] }");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        channel.rpc(uri.getPVStructure());
    }
}
