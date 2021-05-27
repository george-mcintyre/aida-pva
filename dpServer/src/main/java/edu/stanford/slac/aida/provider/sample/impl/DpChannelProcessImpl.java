package edu.stanford.slac.aida.provider.sample.impl;

import edu.stanford.slac.aida.provider.sample.AbstractDpChannelRequest;
import edu.stanford.slac.aida.provider.sample.standard.AidaChannelProvider;
import edu.stanford.slac.aida.provider.sample.standard.StandardDpChannel;
import edu.stanford.slac.aida.lib.PVTopStructure;
import org.epics.pvaccess.client.ChannelProcess;
import org.epics.pvaccess.client.ChannelProcessRequester;
import org.epics.pvdata.pv.PVStructure;

public class DpChannelProcessImpl extends AbstractDpChannelRequest implements ChannelProcess {
    private final ChannelProcessRequester channelProcessRequester;

    public DpChannelProcessImpl(StandardDpChannel dpChannel, PVTopStructure pvTopStructure, ChannelProcessRequester channelProcessRequester, PVStructure pvRequest) {
        super(dpChannel, pvTopStructure, pvRequest);

        this.channelProcessRequester = channelProcessRequester;

        channelProcessRequester.channelProcessConnect(AidaChannelProvider.okStatus, this);
    }

    /* (non-Javadoc)
     * @see org.epics.pvaccess.client.ChannelProcess#process(boolean)
     */
    public void process() {
        if (destroyed.get()) {
            channelProcessRequester.processDone(AidaChannelProvider.destroyedStatus, this);
            return;
        }

        pvTopStructure.lock();
        try {
            pvTopStructure.process();
        } finally {
            pvTopStructure.unlock();
        }

        channelProcessRequester.processDone(AidaChannelProvider.okStatus, this);

        if (lastRequest)
            destroy();
    }
}
