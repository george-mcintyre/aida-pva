package edu.stanford.slac.aida.provider.sample.impl;

import edu.stanford.slac.aida.provider.sample.AbstractDpChannelRequest;
import edu.stanford.slac.aida.provider.sample.standard.AidaChannelProvider;
import edu.stanford.slac.aida.provider.sample.standard.StandardDpChannel;
import edu.stanford.slac.aida.lib.PVTopStructure;
import org.epics.pvdata.misc.BitSet;
import org.epics.pvdata.monitor.Monitor;
import org.epics.pvdata.monitor.MonitorElement;
import org.epics.pvdata.monitor.MonitorRequester;
import org.epics.pvdata.pv.PVStructure;
import org.epics.pvdata.pv.Status;

import java.util.concurrent.atomic.AtomicBoolean;

public class DpChannelMonitorImpl extends AbstractDpChannelRequest implements Monitor, PVTopStructure.PVTopStructureListener, MonitorElement {
    private final MonitorRequester monitorRequester;
    private final PVStructure pvGetStructure;
    private final BitSet bitSet;        // for user
    private final BitSet activeBitSet;        // changed monitoring
    private final AtomicBoolean started = new AtomicBoolean(false);


    // TODO tmp
    private final BitSet allChanged;
    private final BitSet noOverrun;


    public DpChannelMonitorImpl(StandardDpChannel dpChannel, PVTopStructure pvTopStructure, MonitorRequester monitorRequester, PVStructure pvRequest) {
        super(dpChannel, pvTopStructure, pvRequest);

        this.monitorRequester = monitorRequester;

        pvGetStructure = mapper.getCopyStructure();
        activeBitSet = new BitSet(pvGetStructure.getNumberFields());
        activeBitSet.set(0);    // initial get gets all

        bitSet = new BitSet(pvGetStructure.getNumberFields());


        allChanged = new BitSet(pvGetStructure.getNumberFields());
        allChanged.set(0);
        noOverrun = new BitSet(pvGetStructure.getNumberFields());

        monitorRequester.monitorConnect(AidaChannelProvider.okStatus, this, pvGetStructure.getStructure());
    }

    @Override
    public void internalDestroy() {
        pvTopStructure.unregisterListener(this);
    }

    public void topStructureChanged(BitSet changedBitSet) {
        lock();
        activeBitSet.or(changedBitSet);

        // add to queue, trigger
        lock();
        pvTopStructure.lock();
        try {
            mapper.updateCopyStructureOriginBitSet(activeBitSet, bitSet);
            activeBitSet.clear();
        } finally {
            pvTopStructure.unlock();
            unlock();
        }
        unlock();
        // TODO not a safe copy...
        monitorRequester.monitorEvent(this);
    }

    public Status start() {
        if (started.getAndSet(true))
            return AidaChannelProvider.okStatus;

        // force monitor immediately
        topStructureChanged(allChanged);

        pvTopStructure.registerListener(this);

        return AidaChannelProvider.okStatus;
    }

    public Status stop() {
        if (!started.getAndSet(false))
            return AidaChannelProvider.okStatus;

        // TODO clear queue

        pvTopStructure.unregisterListener(this);

        return AidaChannelProvider.okStatus;
    }


    private final AtomicBoolean pooled = new AtomicBoolean(false);

    public MonitorElement poll() {
        if (pooled.getAndSet(true))
            return null;

        return this;
    }

    public void release(MonitorElement monitorElement) {
        pooled.set(false);
    }

    /* (non-Javadoc)
     * @see org.epics.pvdata.monitor.MonitorElement#getPVStructure()
     */
    public PVStructure getPVStructure() {
        return pvGetStructure;
    }

    /* (non-Javadoc)
     * @see org.epics.pvdata.monitor.MonitorElement#getChangedBitSet()
     */
    public BitSet getChangedBitSet() {
        return allChanged;
    }

    /* (non-Javadoc)
     * @see org.epics.pvdata.monitor.MonitorElement#getOverrunBitSet()
     */
    public BitSet getOverrunBitSet() {
        return noOverrun;
    }


}
