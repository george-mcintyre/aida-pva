package edu.stanford.slac.aida.lib;

import org.epics.pvaccess.client.rpc.RPCClientImpl;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.pv.PVStructure;

public class RequestExecutor {
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
