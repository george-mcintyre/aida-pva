package edu.stanford.slac.aida.lib;

import org.epics.pvaccess.server.rpc.RPCServer;

public class AidaRPCServer extends RPCServer {
    private final ChannelProvider aidaChannelProvider;

    public AidaRPCServer(ChannelProvider aidaChannelProvider) {
        this.aidaChannelProvider = aidaChannelProvider;
    }

    public AidaRPCServer(int threads, int queueSize, ChannelProvider aidaChannelProvider) {
        super(threads, queueSize);
        this.aidaChannelProvider = aidaChannelProvider;
    }

    /**
     * Register all channels in the given service
     *
     * @param service the given service
     */
    public void registerServices(AidaRPCService service) {
        for (String channelName : aidaChannelProvider.getChannelNames()) {
            registerService(channelName, service);
        }
    }
}
