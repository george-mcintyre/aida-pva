/*
 * @file
 * AIDA-PVA pvAccess RPC server implementation which provides access to PVAccess and the EPICS network.
 */
package edu.stanford.slac.aida.lib;

import org.epics.pvaccess.server.rpc.RPCServer;

/**
 * AIDA-PVA pvAccess RPC server implementation which provides access to PVAccess and the EPICS network.
 */
public class AidaRPCServer extends RPCServer {
    private final ChannelProvider aidaChannelProvider;

    /**
     * Constructor
     *
     * @param aidaChannelProvider to handle requests on this server
     */
    public AidaRPCServer(ChannelProvider aidaChannelProvider) {
        this.aidaChannelProvider = aidaChannelProvider;
    }
    /**
     * Constructor.  Not used by AIDA-PVA.  May be removed
     *
     * @param threads             the number of threads to use
     * @param queueSize           the size of the queueNative
     * @param aidaChannelProvider to handle requests on this server
     */
    public AidaRPCServer(int threads, int queueSize, ChannelProvider aidaChannelProvider) {
        super(threads, queueSize);
        this.aidaChannelProvider = aidaChannelProvider;
    }

    /**
     * Register all channels in the given service
     * This works by asking the channel provider for the channels that it supports.
     * The channel provider will load the supported channels from its CHANNEL.YML file
     * if they are not already loaded.
     * Once we have the list of channel names we pass them to the EPICs subsystem
     * to register them so that the RPCServer will listen out for requests on our behalf.
     *
     * @param service the given service
     */
    public void registerServices(AidaRPCService service) {
        for (String channelName : aidaChannelProvider.getChannelNames()) {
            registerService(channelName, service);
        }
    }
}
