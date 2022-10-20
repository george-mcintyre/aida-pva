package edu.stanford.slac.aida.test.db.aidapva;

import org.epics.pvaccess.server.rpc.RPCRequestException;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.*;
import static edu.stanford.slac.aida.client.AidaType.AIDA_DOUBLE;

public class Example {
    public Double getDouble() throws RPCRequestException {
        return (Double) pvaGet("XCOR:LI03:120:LEFF", AIDA_DOUBLE);
    }

    public void setDouble(Double value) throws RPCRequestException {
        pvaSet("XCOR:LI31:41:BCON", value);
    }

    public void setInteger(Integer value) throws RPCRequestException {
        pvaRequest("FBCK:LI11:26:HSTA")
                .with("VALUE_TYPE", "INTEGER_ARRAY")
                .set(value);
    }
}
