package edu.stanford.slac.aida.provider.sample.impl;

import edu.stanford.slac.aida.provider.sample.standard.AidaChannelProvider;
import edu.stanford.slac.aida.provider.sample.standard.StandardDpChannel;
import edu.stanford.slac.aida.provider.sample.standard.AbstractDpChannelPut;
import edu.stanford.slac.aida.lib.PVRequestUtils;
import edu.stanford.slac.aida.lib.PVTopStructure;
import org.epics.pvaccess.client.ChannelPutRequester;
import org.epics.pvdata.misc.BitSet;
import org.epics.pvdata.pv.PVStructure;

public class DpChannelPutImpl extends AbstractDpChannelPut {
    private final ChannelPutRequester channelPutRequester;
    private final PVStructure pvPutStructure;
    private final BitSet bitSet;        // for user
    private final boolean process;

    public DpChannelPutImpl(StandardDpChannel dpChannel, PVTopStructure pvTopStructure, ChannelPutRequester channelPutRequester, PVStructure pvRequest) {
        super(dpChannel, pvTopStructure, pvRequest);

        this.channelPutRequester = channelPutRequester;

        process = PVRequestUtils.getProcess(pvRequest);

        pvPutStructure = mapper.getCopyStructure();
        bitSet = new BitSet(pvPutStructure.getNumberFields());

        channelPutRequester.channelPutConnect(AidaChannelProvider.okStatus, this, pvPutStructure.getStructure());
    }

    public void put(PVStructure pvStructure, BitSet pvBitSet) {
        if (destroyed.get()) {
            channelPutRequester.putDone(AidaChannelProvider.destroyedStatus, this);
            return;
        }

        lock();
        pvTopStructure.lock();
        try {
            mapper.updateOriginStructure(pvStructure, pvBitSet);

            if (process)
                pvTopStructure.process();

        } finally {
            pvTopStructure.unlock();
            unlock();
        }

        channelPutRequester.putDone(AidaChannelProvider.okStatus, this);

        if (lastRequest)
            destroy();
    }

    public void get() {
        if (destroyed.get()) {
            channelPutRequester.getDone(AidaChannelProvider.destroyedStatus, this, null, null);
            return;
        }

        lock();
        pvTopStructure.lock();
        try {
            mapper.updateCopyStructure(null);
        } finally {
            pvTopStructure.unlock();
            unlock();
        }

        // TODO
        bitSet.clear();
        bitSet.set(0);
        channelPutRequester.getDone(AidaChannelProvider.okStatus, this, pvPutStructure, bitSet);
    }

}
