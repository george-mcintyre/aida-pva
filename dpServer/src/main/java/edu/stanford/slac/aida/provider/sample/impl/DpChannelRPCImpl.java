package edu.stanford.slac.aida.provider.sample.impl;

import edu.stanford.slac.aida.provider.sample.AbstractDpChannelRequest;
import edu.stanford.slac.aida.provider.sample.standard.AidaChannelProvider;
import edu.stanford.slac.aida.provider.sample.standard.StandardDpChannel;
import edu.stanford.slac.aida.lib.PVTopStructure;
import org.epics.pvaccess.client.ChannelRPC;
import org.epics.pvaccess.client.ChannelRPCRequester;
import org.epics.pvdata.pv.PVStructure;
import org.epics.pvdata.pv.Status;

/**
 * User: george
 */
public class DpChannelRPCImpl extends AbstractDpChannelRequest implements ChannelRPC {
    private final ChannelRPCRequester channelRPCRequester;

    public DpChannelRPCImpl(StandardDpChannel dpChannel, PVTopStructure pvTopStructure, ChannelRPCRequester channelRPCRequester, PVStructure pvRequest) {
        super(dpChannel, pvTopStructure, pvRequest);

        this.channelRPCRequester = channelRPCRequester;

        channelRPCRequester.channelRPCConnect(AidaChannelProvider.okStatus, this);
    }

    public void request(PVStructure pvArgument) {
        if (destroyed.get()) {
            channelRPCRequester.requestDone(AidaChannelProvider.destroyedStatus, this, null);
            return;
        }

        // TODO async support
        PVStructure result = null;
        Status status = AidaChannelProvider.okStatus;
        pvTopStructure.lock();
        try {
            result = pvTopStructure.request(pvArgument);
        } catch (Throwable th) {
            status = AidaChannelProvider.statusCreate.createStatus(Status.StatusType.ERROR, "exception caught: " + th.getMessage(), th);
        } finally {
            pvTopStructure.unlock();
        }

        channelRPCRequester.requestDone(status, this, result);

        if (lastRequest)
            destroy();
    }
}
