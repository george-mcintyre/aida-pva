package edu.stanford.slac.aida.provider.sample;

import edu.stanford.slac.aida.provider.sample.standard.StandardDpChannel;
import edu.stanford.slac.aida.lib.Mapper;
import edu.stanford.slac.aida.lib.PVTopStructure;
import org.epics.pvaccess.client.Channel;
import org.epics.pvaccess.client.ChannelRequest;
import org.epics.pvdata.pv.PVStructure;

import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.locks.ReentrantLock;

public abstract class AbstractDpChannelRequest implements ChannelRequest {
    private final StandardDpChannel dpChannel;
    protected final PVTopStructure pvTopStructure;
    protected final AtomicBoolean destroyed = new AtomicBoolean();
    protected final Mapper mapper;
    protected final ReentrantLock lock = new ReentrantLock();
    protected volatile boolean lastRequest = false;

    public AbstractDpChannelRequest(StandardDpChannel dpChannel, PVTopStructure pvTopStructure, PVStructure pvRequest) {
        this.dpChannel = dpChannel;
        this.pvTopStructure = pvTopStructure;

        if (pvRequest != null)
            mapper = new Mapper(pvTopStructure.getPVStructure(), pvRequest);
        else
            mapper = null;

        dpChannel.registerRequest(this);
    }

    public void lock() {
        lock.lock();
    }

    public void unlock() {
        lock.unlock();
    }

    public final void destroy() {
        if (destroyed.getAndSet(true))
            return;
        dpChannel.unregisterRequest(this);
        internalDestroy();
    }

    protected void internalDestroy() {
        // noop
    }

    public void cancel() {
        // noop, not supported
    }

    public Channel getChannel() {
        return dpChannel;
    }

    public void lastRequest() {
        lastRequest = true;
    }

}
