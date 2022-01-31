package edu.stanford.slac.aida.lib;

import org.epics.pvaccess.client.rpc.RPCClientImpl;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.factory.FieldFactory;
import org.epics.pvdata.pv.FieldCreate;
import org.epics.pvdata.pv.PVStructure;

public class RequestExecutor {
    /**
     * To be able to create fields in PV Access you need a field creator factory
     */
    private final static FieldCreate fieldCreate = FieldFactory.getFieldCreate();

    /**
     * Normative Type ID string for URIs
     */
    public static final String NTURI_ID = "epics:nt/NTURI:1.0";


    static PVStructure executeRequest(PVStructure pvUri, String channelName) throws RPCRequestException {
        RPCClientImpl client = null;
        try {
            client = new RPCClientImpl(channelName);
            return client.request(pvUri, 3.0);
        } finally {
            if (client != null) {
                client.destroy();
            }
        }
    }
}
