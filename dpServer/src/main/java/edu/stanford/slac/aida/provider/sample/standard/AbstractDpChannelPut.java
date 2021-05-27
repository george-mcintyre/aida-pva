package edu.stanford.slac.aida.provider.sample.standard;

import edu.stanford.slac.aida.provider.sample.AbstractDpChannelRequest;
import edu.stanford.slac.aida.lib.PVTopStructure;
import org.epics.pvaccess.client.ChannelPut;
import org.epics.pvdata.pv.PVStructure;

public abstract class AbstractDpChannelPut extends AbstractDpChannelRequest implements ChannelPut {
    public AbstractDpChannelPut(StandardDpChannel dpChannel, PVTopStructure pvTopStructure, PVStructure pvRequest) {
        super(dpChannel, pvTopStructure, pvRequest);
    }
}
