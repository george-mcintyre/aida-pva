package edu.stanford.slac.aida.provider.sample.impl;

import edu.stanford.slac.aida.provider.sample.standard.StandardDpChannel;
import edu.stanford.slac.aida.provider.sample.standard.AidaChannelProvider;
import edu.stanford.slac.aida.lib.PVTopStructure;
import org.epics.pvaccess.client.ChannelRequester;

public class DpChannelImpl extends StandardDpChannel {
    public DpChannelImpl(AidaChannelProvider dpServer, String channelName, ChannelRequester channelRequester, PVTopStructure pvTopStructure) {
        super(dpServer, channelName, channelRequester, pvTopStructure);
    }

}
