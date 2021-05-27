package edu.stanford.slac.aida.provider.sample.impl;

import edu.stanford.slac.aida.provider.sample.AbstractDpChannelRequest;
import edu.stanford.slac.aida.provider.sample.standard.AidaChannelProvider;
import edu.stanford.slac.aida.provider.sample.standard.StandardDpChannel;
import edu.stanford.slac.aida.lib.PVTopStructure;
import org.epics.pvaccess.client.ChannelArray;
import org.epics.pvaccess.client.ChannelArrayRequester;
import org.epics.pvdata.pv.PVArray;
import org.epics.pvdata.pv.PVScalarArray;

public class DpChannelScalarArrayImpl extends AbstractDpChannelRequest implements ChannelArray {
    private final ChannelArrayRequester channelArrayRequester;
    private final PVScalarArray pvArray;
    private final PVScalarArray pvCopy;
    private final boolean process;

    public DpChannelScalarArrayImpl(StandardDpChannel dpChannel, PVTopStructure pvTopStructure, ChannelArrayRequester channelArrayRequester, PVScalarArray array) {
        super(dpChannel, pvTopStructure, null);

        this.channelArrayRequester = channelArrayRequester;
        this.pvArray = array;
        this.pvCopy = AidaChannelProvider.pvDataCreate.createPVScalarArray(pvArray.getScalarArray().getElementType());

        process = false; // TODO PVRequestUtils.getProcess(pvRequest);

        channelArrayRequester.channelArrayConnect(AidaChannelProvider.okStatus, this, pvCopy.getArray());
    }

    public void putArray(PVArray pvCopyArray, int offset, int count, int stride) {
        PVScalarArray pvCopy = (PVScalarArray) pvCopyArray;
        if (destroyed.get()) {
            channelArrayRequester.putArrayDone(AidaChannelProvider.destroyedStatus, this);
            return;
        }

        if (stride != 1)
            throw new UnsupportedOperationException("stride != 1");

        lock();
        pvTopStructure.lock();
        try {
            if (count <= 0) count = pvCopy.getLength();
            AidaChannelProvider.convert.copyScalarArray(pvCopy, 0, pvArray, offset, count);

            if (process)
                pvTopStructure.process();

        } finally {
            pvTopStructure.unlock();
            unlock();
        }

        channelArrayRequester.putArrayDone(AidaChannelProvider.okStatus, this);

        if (lastRequest)
            destroy();
    }

    public void getArray(int offset, int count, int stride) {
        if (destroyed.get()) {
            channelArrayRequester.getArrayDone(AidaChannelProvider.destroyedStatus, this, null);
            return;
        }

        if (stride != 1)
            throw new UnsupportedOperationException("stride != 1");

        lock();
        pvTopStructure.lock();
        try {
            if (count == 0) count = pvArray.getLength() - offset;
            int len = AidaChannelProvider.convert.copyScalarArray(pvArray, offset, pvCopy, 0, count);
            if (!pvCopy.isImmutable()) pvCopy.setLength(len);
        } finally {
            pvTopStructure.unlock();
            unlock();
        }

        channelArrayRequester.getArrayDone(AidaChannelProvider.okStatus, this, pvCopy);

        if (lastRequest)
            destroy();
    }

    public void setLength(int length) {
        if (destroyed.get()) {
            channelArrayRequester.setLengthDone(AidaChannelProvider.destroyedStatus, this);
            return;
        }

        // TODO process???

        lock();
        pvTopStructure.lock();
        try {
            if (pvArray.getLength() != length) pvArray.setLength(length);
        } finally {
            pvTopStructure.unlock();
            unlock();
        }

        channelArrayRequester.setLengthDone(AidaChannelProvider.okStatus, this);

        if (lastRequest)
            destroy();
    }

    public void getLength() {
        if (destroyed.get()) {
            channelArrayRequester.getLengthDone(AidaChannelProvider.destroyedStatus, this, 0);
            return;
        }

        int length;

        lock();
        pvTopStructure.lock();
        try {
            length = pvArray.getLength();
        } finally {
            pvTopStructure.unlock();
            unlock();
        }

        channelArrayRequester.getLengthDone(AidaChannelProvider.okStatus, this, length);

        if (lastRequest)
            destroy();
    }


}
