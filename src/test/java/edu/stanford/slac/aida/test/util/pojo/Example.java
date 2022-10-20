package edu.stanford.slac.aida.test.util.pojo;

import org.epics.nt.NTURI;
import org.epics.pvaccess.client.rpc.RPCClientImpl;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.pv.*;

public class Example {
    public Short getTrigStatusShort() throws RPCRequestException {
        String pvName = "TRIG:LI31:109:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("type")
                .addQueryInt("beam")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("SHORT");
        uri.getQuery().getIntField("beam").put(1);

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        PVShort field = response.getSubField(PVShort.class, "value");
        return field.get();
    }

    public Long getTrigStatusLong() throws RPCRequestException {
        String pvName = "TRIG:LI31:109:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("type")
                .addQueryInt("beam")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("LONG");
        uri.getQuery().getIntField("beam").put(1);

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        PVLong field = response.getSubField(PVLong.class, "value");
        return field.get();
    }

    public String getTrigStatusString() throws RPCRequestException {
        String pvName = "TRIG:LI31:109:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("type")
                .addQueryInt("beam")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("STRING");
        uri.getQuery().getIntField("beam").put(1);

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        PVString field = response.getSubField(PVString.class, "value");
        return field.get();
    }

    public PVStructure setTrigStatusStatus(Integer status) throws RPCRequestException {
        String pvName = "TRIG:LI31:109:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("value")
                .addQueryInt("beam")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("value").put(status);
        uri.getQuery().getIntField("beam").put(1);

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 30.0);
        client.destroy();

        return response;
    }

    public PVStructure turnKnob(Float turnAmount) throws RPCRequestException {
        String pvName = "MKB:VAL";

        NTURI uri = NTURI.createBuilder()
                .addQueryDouble("value")
                .addQueryString("mkb")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getDoubleField("value").put(turnAmount);
        uri.getQuery().getStringField("mkb").put("MKB:LI02B_XB.MKB");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 30.0);
        client.destroy();

        return response;
    }

    public void setBgrp(Boolean value) throws RPCRequestException {
        String pvName = "BGRP:VAL";

        NTURI uri = NTURI.createBuilder()
                .addQueryDouble("value")
                .addQueryString("bgrp")
                .addQueryString("varname")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("value").put(value.toString());
        uri.getQuery().getStringField("bgrp").put("LCLS");
        uri.getQuery().getStringField("varname").put("T_CAV");

        RPCClientImpl client = new RPCClientImpl(pvName);
        client.request(uri.getPVStructure(), 3.0);
        client.destroy();
    }

    public Boolean getFeedbackStatusBoolean() throws RPCRequestException {
        String pvName = "FBCK:LI18:28:LOOP_STAT";

        NTURI uri = NTURI.createBuilder().addQueryString("type").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("BOOLEAN");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        PVBoolean field = response.getSubField(PVBoolean.class, "value");
        return field.get();
    }

    public Short getFeedbackStatusShort() throws RPCRequestException {
        String pvName = "FBCK:LI18:28:LOOP_STAT";

        NTURI uri = NTURI.createBuilder().addQueryString("type").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("SHORT");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        PVShort field = response.getSubField(PVShort.class, "value");
        return field.get();
    }

    public Long getFeedbackStatusLong() throws RPCRequestException {
        String pvName = "FBCK:LI18:28:LOOP_STAT";

        NTURI uri = NTURI.createBuilder().addQueryString("type").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("LONG");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        PVLong field = response.getSubField(PVLong.class, "value");
        return field.get();
    }

    public String getFeedbackStatusString() throws RPCRequestException {
        String pvName = "FBCK:LI18:28:LOOP_STAT";

        NTURI uri = NTURI.createBuilder().addQueryString("type").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("STRING");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        PVString field = response.getSubField(PVString.class, "value");
        return field.get();
    }

    public void setFeedbackStatus(String status) throws RPCRequestException {
        String pvName = "FBCK:LI18:28:LOOP_STAT";

        NTURI uri = NTURI.createBuilder().addQueryString("value").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("value").put(status);

        RPCClientImpl client = new RPCClientImpl(pvName);
        client.request(uri.getPVStructure(), 3.0);
        client.destroy();
    }

    public PVStructure getCorrelatedPlot() throws RPCRequestException {
        String pvName = "CORRPLOT:SCAN";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("file")
                .addQueryString("primstv")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("file").put("FACET_BPMS.BTN");
        uri.getQuery().getStringField("primstv").put("{\"name\": \"KLYS:LI31:31:PDES\", \"low\": 0.5, \"high\": 1.0, \"steps\": 6}");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 30.0);
        client.destroy();

        return response;
    }
}
