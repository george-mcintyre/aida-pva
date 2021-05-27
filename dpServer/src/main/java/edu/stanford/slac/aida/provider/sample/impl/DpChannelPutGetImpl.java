package edu.stanford.slac.aida.provider.sample.impl;

import edu.stanford.slac.aida.provider.sample.AbstractDpChannelRequest;
import edu.stanford.slac.aida.provider.sample.standard.AidaChannelProvider;
import edu.stanford.slac.aida.provider.sample.standard.StandardDpChannel;
import edu.stanford.slac.aida.lib.Mapper;
import edu.stanford.slac.aida.lib.PVRequestUtils;
import edu.stanford.slac.aida.lib.PVTopStructure;
import org.epics.pvaccess.client.ChannelPutGet;
import org.epics.pvaccess.client.ChannelPutGetRequester;
import org.epics.pvdata.misc.BitSet;
import org.epics.pvdata.pv.MessageType;
import org.epics.pvdata.pv.PVField;
import org.epics.pvdata.pv.PVStructure;
import org.epics.pvdata.pv.Type;

public class DpChannelPutGetImpl extends AbstractDpChannelRequest implements ChannelPutGet {
    private final ChannelPutGetRequester channelPutGetRequester;
    private PVStructure pvGetStructure;
    private PVStructure pvPutStructure;
    private BitSet pvGetBitSet;
    private BitSet pvPutBitSet;
    private Mapper putMapper;
    private Mapper getMapper;
    private boolean process;

    public DpChannelPutGetImpl(StandardDpChannel dpChannel, PVTopStructure pvTopStructure, ChannelPutGetRequester channelPutGetRequester, PVStructure pvRequest) {
        super(dpChannel, pvTopStructure, null);

        this.channelPutGetRequester = channelPutGetRequester;

        PVField pvField = pvRequest.getSubField("putField");
        if (pvField == null || pvField.getField().getType() != Type.structure) {
            channelPutGetRequester.message("pvRequest does not have a putField request structure", MessageType.error);
            channelPutGetRequester.message(pvRequest.toString(), MessageType.warning);
            channelPutGetRequester.channelPutGetConnect(AidaChannelProvider.illegalRequestStatus, null, null, null);
            return;
        }
        putMapper = new Mapper(pvTopStructure.getPVStructure(), pvRequest, "putField");


        pvField = pvRequest.getSubField("getField");
        if (pvField == null || pvField.getField().getType() != Type.structure) {
            channelPutGetRequester.message("pvRequest does not have a getField request structure", MessageType.error);
            channelPutGetRequester.message(pvRequest.toString(), MessageType.warning);
            channelPutGetRequester.channelPutGetConnect(AidaChannelProvider.illegalRequestStatus, null, null, null);
            return;
        }
        getMapper = new Mapper(pvTopStructure.getPVStructure(), pvRequest, "getField");

        process = PVRequestUtils.getProcess(pvRequest);

        pvPutStructure = putMapper.getCopyStructure();
        pvGetStructure = getMapper.getCopyStructure();

        // TODO
        pvPutBitSet = new BitSet(pvPutStructure.getNumberFields());
        pvPutBitSet.set(0);
        pvGetBitSet = new BitSet(pvGetStructure.getNumberFields());
        pvGetBitSet.set(0);

        channelPutGetRequester.channelPutGetConnect(AidaChannelProvider.okStatus, this, pvPutStructure.getStructure(), pvGetStructure.getStructure());
    }

    /* (non-Javadoc)
     * @see org.epics.pvaccess.client.ChannelPutGet#putGet(boolean)
     */
    public void putGet(PVStructure pvPutStructure, BitSet pvPutBitSet) {
        if (destroyed.get()) {
            channelPutGetRequester.putGetDone(AidaChannelProvider.destroyedStatus, this, null, null);
            return;
        }

        lock();
        pvTopStructure.lock();
        try {
            putMapper.updateOriginStructure(pvPutStructure, pvPutBitSet);
            if (process)
                pvTopStructure.process();
            getMapper.updateCopyStructure(null);
        } finally {
            pvTopStructure.unlock();
            unlock();
        }

        channelPutGetRequester.putGetDone(AidaChannelProvider.okStatus, this, pvGetStructure, pvGetBitSet);

        if (lastRequest)
            destroy();
    }

    /* (non-Javadoc)
     * @see org.epics.pvaccess.client.ChannelPutGet#getPut()
     */
    public void getPut() {
        if (destroyed.get()) {
            channelPutGetRequester.getPutDone(AidaChannelProvider.destroyedStatus, this, null, null);
            return;
        }

        lock();
        pvTopStructure.lock();
        try {
            putMapper.updateCopyStructure(null);
        } finally {
            pvTopStructure.unlock();
            unlock();
        }

        channelPutGetRequester.getPutDone(AidaChannelProvider.okStatus, this, pvPutStructure, pvPutBitSet);
    }

    /* (non-Javadoc)
     * @see org.epics.pvaccess.client.ChannelPutGet#getGet()
     */
    public void getGet() {
        if (destroyed.get()) {
            channelPutGetRequester.getGetDone(AidaChannelProvider.destroyedStatus, this, null, null);
            return;
        }

        lock();
        pvTopStructure.lock();
        try {
            getMapper.updateCopyStructure(null);
        } finally {
            pvTopStructure.unlock();
            unlock();
        }

        channelPutGetRequester.getGetDone(AidaChannelProvider.okStatus, this, pvGetStructure, pvGetBitSet);
    }

}
