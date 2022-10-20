package edu.stanford.slac.aida.test.klystron.pvaclient;

import org.epics.nt.NTURI;
import org.epics.pvaClient.PvaClient;
import org.epics.pvaClient.PvaClientChannel;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.pv.PVLong;
import org.epics.pvdata.pv.PVShort;
import org.epics.pvdata.pv.PVString;
import org.epics.pvdata.pv.PVStructure;

import java.util.List;

public class Example {
    public Short getStatusShort(Integer beamCode) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("beam")
                .addQueryString("type")
                .addQueryString("dgrp")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("beam").put(beamCode);
        uri.getQuery().getStringField("type").put("SHORT");
        uri.getQuery().getStringField("dgrp").put("DEV_DGRP");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(uri.getPVStructure());

        PVShort field = response.getSubField(PVShort.class, "value");
        return field.get();
    }

    public Long getStatusLong(Integer beamCode) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("beam")
                .addQueryString("type")
                .addQueryString("dgrp")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("beam").put(beamCode);
        uri.getQuery().getStringField("type").put("LONG");
        uri.getQuery().getStringField("dgrp").put("DEV_DGRP");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(uri.getPVStructure());

        PVLong field = response.getSubField(PVLong.class, "value");
        return field.get();
    }

    public String getStatusString(Integer beamCode) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("beam")
                .addQueryString("type")
                .addQueryString("dgrp")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("beam").put(beamCode);
        uri.getQuery().getStringField("type").put("STRING");
        uri.getQuery().getStringField("dgrp").put("DEV_DGRP");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(uri.getPVStructure());

        PVString field = response.getSubField(PVString.class, "value");
        return field.get();
    }

    public PVStructure getStatusTable(Integer beamCode) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("beam")
                .addQueryString("type")
                .addQueryString("dgrp")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("beam").put(beamCode);
        uri.getQuery().getStringField("type").put("TABLE");
        uri.getQuery().getStringField("dgrp").put("DEV_DGRP");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        return channel.rpc(uri.getPVStructure());
    }

    public PVStructure getStatuses(Integer beamCode, List<String> devices) throws RPCRequestException {
        String pvName = "KLYSTRONGET:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("devices")
                .addQueryInt("beam")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("devices").put("[\"KLYS:LI31:31\", \"KLYS:LI31:32\"]");
        uri.getQuery().getIntField("beam").put(beamCode);

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        return channel.rpc(uri.getPVStructure());
    }

    public PVStructure activateKlystron(Integer beamCode, Boolean activateStatus) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:PDES";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("value")
                .addQueryInt("beam")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("value").put(activateStatus.toString());
        uri.getQuery().getIntField("beam").put(beamCode);

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        return channel.rpc(uri.getPVStructure());
    }

    public PVStructure setPdes(Double value) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:PDES";

        NTURI uri = NTURI.createBuilder().addQueryDouble("value").create();
        uri.getPath().put(pvName);
        uri.getQuery().getDoubleField("value").put(value);

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        return channel.rpc(uri.getPVStructure());
    }

    public PVStructure setKhpr(Double value) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:KPHR";

        NTURI uri = NTURI.createBuilder().addQueryDouble("value").create();
        uri.getPath().put(pvName);
        uri.getQuery().getDoubleField("value").put(value);

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        return channel.rpc(uri.getPVStructure());
    }

    public void setPconAcon(Double value) throws RPCRequestException {

        NTURI uri = NTURI.createBuilder().addQueryDouble("value").create();
        uri.getQuery().getDoubleField("value").put(value);

        String pvName = "KLYS:LI31:31:PCON";
        uri.getPath().put(pvName);
        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        channel.rpc(uri.getPVStructure());

        pvName = "KLYS:LI31:31:ACON";
        uri.getPath().put(pvName);
        channel = client.createChannel(pvName);
        channel.rpc(uri.getPVStructure());
    }

    public void setPmdls() throws RPCRequestException {
        String pvName = "KLYSTRONSET:PMDL";

        NTURI uri = NTURI.createBuilder().addQueryString("value").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("value").put("{\"names\":[\"SBST:LI00:1\", \"SBST:LI01:1\"], \"values\":[0.1, 1.2]}");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        channel.rpc(uri.getPVStructure());
    }
}
