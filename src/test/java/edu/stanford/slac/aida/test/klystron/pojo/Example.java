package edu.stanford.slac.aida.test.klystron.pojo;

import org.epics.nt.NTURI;
import org.epics.pvaccess.client.rpc.RPCClientImpl;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.pv.*;

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

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();


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

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();


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

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();


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

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        return response;
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

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        return response;
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

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        return response;
    }

    public PVStructure setPdes(Double value) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:PDES";

        NTURI uri = NTURI.createBuilder().addQueryDouble("value").create();
        uri.getPath().put(pvName);
        uri.getQuery().getDoubleField("value").put(value);

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        return response;
    }

    public PVStructure setKhpr(Double value) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:KPHR";

        NTURI uri = NTURI.createBuilder().addQueryDouble("value").create();
        uri.getPath().put(pvName);
        uri.getQuery().getDoubleField("value").put(value);

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        return response;
    }

    public void setPconAcon(Double value) throws RPCRequestException {

        NTURI uri = NTURI.createBuilder().addQueryDouble("value").create();
        uri.getQuery().getDoubleField("value").put(value);

        String pvName = "KLYS:LI31:31:PCON";
        uri.getPath().put(pvName);
        RPCClientImpl client = new RPCClientImpl(pvName);
        client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        pvName = "KLYS:LI31:31:ACON";
        uri.getPath().put(pvName);
        client = new RPCClientImpl(pvName);
        client.request(uri.getPVStructure(), 3.0);
        client.destroy();
    }

    public void setPmdls() throws RPCRequestException {
        String pvName = "KLYSTRONSET:PMDL";

        NTURI uri = NTURI.createBuilder().addQueryString("value").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("value").put("{\"names\":[\"SBST:LI00:1\", \"SBST:LI01:1\"], \"values\":[0.1, 1.2]}");

        RPCClientImpl client = new RPCClientImpl(pvName);
        client.request(uri.getPVStructure(), 3.0);
        client.destroy();
    }
}
