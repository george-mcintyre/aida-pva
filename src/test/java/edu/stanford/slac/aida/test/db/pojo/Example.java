package edu.stanford.slac.aida.test.db.pojo;

import org.epics.nt.NTURI;
import org.epics.pvaccess.client.rpc.RPCClientImpl;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.pv.PVDouble;
import org.epics.pvdata.pv.PVStructure;

public class Example {
    public Double getDouble() throws RPCRequestException {
        String pvName = "XCOR:LI03:120:LEFF";

        NTURI uri = NTURI.createBuilder().addQueryString("type").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("DOUBLE");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        PVDouble field = response.getSubField(PVDouble.class, "value");
        return field.get();
    }

    public void setDouble(Double value) throws RPCRequestException {
        String pvName = "XCOR:LI31:41:BCON";

        NTURI uri = NTURI.createBuilder().addQueryDouble("value").create();
        uri.getPath().put(pvName);
        uri.getQuery().getDoubleField("value").put(value);

        RPCClientImpl client = new RPCClientImpl(pvName);
        client.request(uri.getPVStructure(), 3.0);
        client.destroy();
    }

    public void setInteger(Integer value) throws RPCRequestException {
        String pvName = "XCOR:LI31:41:BCON";

        NTURI uri = NTURI.createBuilder()
                .addQueryDouble("value")
                .addQueryString("value_type")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getDoubleField("value").put(value);
        uri.getQuery().getStringField("value").put("INTEGER_ARRAY");

        RPCClientImpl client = new RPCClientImpl(pvName);
        client.request(uri.getPVStructure(), 3.0);
        client.destroy();
    }
}
