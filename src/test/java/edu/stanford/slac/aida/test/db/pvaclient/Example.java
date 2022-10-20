package edu.stanford.slac.aida.test.db.pvaclient;

import org.epics.nt.NTURI;
import org.epics.pvaClient.PvaClient;
import org.epics.pvaClient.PvaClientChannel;
import org.epics.pvaccess.client.rpc.RPCClientImpl;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.pv.PVDouble;
import org.epics.pvdata.pv.PVStructure;

public class Example {
    public Double getDouble() {
        String pvName = "XCOR:LI03:120:LEFF";

        NTURI uri = NTURI.createBuilder().addQueryString("type").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("DOUBLE");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(uri.getPVStructure());

        PVDouble field = response.getSubField(PVDouble.class, "value");
        return field.get();
    }

    public void setDouble(Double value) {
        String pvName = "XCOR:LI31:41:BCON";
        NTURI uri = NTURI.createBuilder().addQueryDouble("value").create();
        uri.getPath().put(pvName);
        uri.getQuery().getDoubleField("value").put(value);

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        channel.rpc(uri.getPVStructure());
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

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        channel.rpc(uri.getPVStructure());
    }
}
