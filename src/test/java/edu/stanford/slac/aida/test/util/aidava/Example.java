package edu.stanford.slac.aida.test.util.aidava;

import edu.stanford.slac.aida.client.PvaTable;
import org.epics.pvaccess.server.rpc.RPCRequestException;

import java.util.HashMap;
import java.util.Map;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.*;
import static edu.stanford.slac.aida.client.AidaType.*;

public class Example {
    public Short getTrigStatusShort() throws RPCRequestException {
        return (Short) pvaRequest("TRIG:LI31:109:TACT")
                .with("BEAM", 1)
                .returning(AIDA_SHORT)
                .get();
    }

    public Long getTrigStatusLong() throws RPCRequestException {
        return (Long) pvaRequest("TRIG:LI31:109:TACT")
                .with("BEAM", 1)
                .returning(AIDA_LONG)
                .get();
    }

    public String getTrigStatusString() throws RPCRequestException {
        return (String) pvaRequest("TRIG:LI31:109:TACT")
                .with("BEAM", 1)
                .returning(AIDA_STRING)
                .get();
    }

    public PvaTable setTrigStatusStatus(Integer status) throws RPCRequestException {
        return pvaRequest("TRIG:LI31:109:TACT")
                .with("BEAM", 1)
                .set(status);
    }

    public PvaTable turnKnob(Float turnAmount) throws RPCRequestException {
        return pvaRequest("MKB:VAL")
                .with("MKB", "MKB:LI02B_XB.MKB")
                .set(turnAmount);
    }

    public void setBgrp(Boolean value) throws RPCRequestException {
        pvaRequest("BGRP:VAL")
                .with("BGRP", "LCLS")
                .with("VARNAME", "T_CAV")
                .set(value);
    }

    public Boolean getFeedbackStatusBoolean() throws RPCRequestException {
        return (Boolean) pvaGet("FBCK:LI18:28:LOOP_STAT", AIDA_BOOLEAN);
    }

    public Short getFeedbackStatusShort() throws RPCRequestException {
        return (Short) pvaGet("FBCK:LI18:28:LOOP_STAT", AIDA_SHORT);
    }

    public Long getFeedbackStatusLong() throws RPCRequestException {
        return (Long) pvaGet("FBCK:LI18:28:LOOP_STAT", AIDA_LONG);
    }

    public String getFeedbackStatusString() throws RPCRequestException {
        return (String) pvaGet("FBCK:LI18:28:LOOP_STAT", AIDA_STRING);
    }

    public void setFeedbackStatus(String status) throws RPCRequestException {
        pvaSet("FBCK:LI18:28:LOOP_STAT", status);
    }

    public PvaTable getCorrelatedPlot() throws RPCRequestException {
        Map<String, Object> primStepVar = new HashMap<String, Object>();
        primStepVar.put("name", "KLYS:LI31:31:PDES");
        primStepVar.put("low", 0.5);
        primStepVar.put("high", 1.0);
        primStepVar.put( "steps", 6);

        return (PvaTable) pvaRequest("CORRPLOT:SCAN")
                .with("FILE", "FACET_BPMS.BTN")
                .with("PRIMSTV", primStepVar)
                .get();
    }
}
