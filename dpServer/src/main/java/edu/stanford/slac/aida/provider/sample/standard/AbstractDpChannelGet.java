package edu.stanford.slac.aida.provider.sample.standard;

import edu.stanford.slac.aida.provider.sample.AbstractDpChannelRequest;
import edu.stanford.slac.aida.lib.PVTopStructure;
import org.epics.pvaccess.client.ChannelGet;
import org.epics.pvdata.pv.PVStructure;

public abstract class AbstractDpChannelGet extends AbstractDpChannelRequest implements ChannelGet, PVTopStructure.PVTopStructureListener {
    public AbstractDpChannelGet(StandardDpChannel dpChannel, PVTopStructure pvTopStructure, PVStructure pvRequest) {
        super(dpChannel, pvTopStructure, pvRequest);
    }
}
