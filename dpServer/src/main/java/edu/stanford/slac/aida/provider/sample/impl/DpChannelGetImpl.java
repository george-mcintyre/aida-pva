package edu.stanford.slac.aida.provider.sample.impl;

import edu.stanford.slac.aida.provider.sample.standard.AbstractDpChannelGet;
import edu.stanford.slac.aida.provider.sample.standard.AidaChannelProvider;
import edu.stanford.slac.aida.provider.sample.standard.StandardDpChannel;
import edu.stanford.slac.aida.lib.PVRequestUtils;
import edu.stanford.slac.aida.lib.PVTopStructure;
import org.epics.pvaccess.client.ChannelGetRequester;
import org.epics.pvdata.misc.BitSet;
import org.epics.pvdata.pv.PVStructure;

import java.util.concurrent.atomic.AtomicBoolean;

public class DpChannelGetImpl extends AbstractDpChannelGet {
    private final ChannelGetRequester channelGetRequester;
    private final PVStructure pvGetStructure;
    private final BitSet bitSet;         // for user
    private final BitSet activeBitSet;   // changed monitoring
    private final boolean process;
    private final AtomicBoolean firstGet = new AtomicBoolean(true);

    public DpChannelGetImpl(StandardDpChannel dpChannel, PVTopStructure pvTopStructure, ChannelGetRequester channelGetRequester, PVStructure pvRequest) {
        super(dpChannel, pvTopStructure, pvRequest);

        this.channelGetRequester = channelGetRequester;

        process = PVRequestUtils.getProcess(pvRequest);

        pvGetStructure = mapper.getCopyStructure();
        activeBitSet = new BitSet(pvGetStructure.getNumberFields());
        activeBitSet.set(0);    // initial get gets all

        bitSet = new BitSet(pvGetStructure.getNumberFields());

        channelGetRequester.channelGetConnect(AidaChannelProvider.okStatus, this, pvGetStructure.getStructure());
    }

    public void get() {
        if (destroyed.get()) {
            channelGetRequester.getDone(AidaChannelProvider.destroyedStatus, this, null, null);
            return;
        }

        lock();
        pvTopStructure.lock();
        try {
            if (process)
                pvTopStructure.process();

            mapper.updateCopyStructureOriginBitSet(activeBitSet, bitSet);
            activeBitSet.clear();
            if (firstGet.getAndSet(false))
                pvTopStructure.registerListener(this);
        } finally {
            pvTopStructure.unlock();
            unlock();
        }

        channelGetRequester.getDone(AidaChannelProvider.okStatus, this, pvGetStructure, bitSet);

        if (lastRequest)
            destroy();
    }

    @Override
    public void internalDestroy() {
        pvTopStructure.unregisterListener(this);
    }

    public void topStructureChanged(BitSet changedBitSet) {
        lock();
        activeBitSet.or(changedBitSet);
        unlock();
    }

}
