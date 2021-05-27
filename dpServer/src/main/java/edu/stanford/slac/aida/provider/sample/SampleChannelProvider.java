/*
 * Copyright (c) 2004 by Cosylab
 *
 * The full license specifying the redistribution, modification, usage and other
 * rights and obligations is included with the distribution of this project in
 * the file "LICENSE-CAJ". If the license is not included visit Cosylab web site,
 * <http://www.cosylab.com>.
 *
 * THIS SOFTWARE IS PROVIDED AS-IS WITHOUT WARRANTY OF ANY KIND, NOT EVEN THE
 * IMPLIED WARRANTY OF MERCHANTABILITY. THE AUTHOR OF THIS SOFTWARE, ASSUMES
 * _NO_ RESPONSIBILITY FOR ANY CONSEQUENCE RESULTING FROM THE USE, MODIFICATION,
 * OR REDISTRIBUTION OF THIS SOFTWARE.
 */

package edu.stanford.slac.aida.provider.sample;

import edu.stanford.slac.aida.lib.PVTopStructure;
import edu.stanford.slac.aida.lib.RPCTopStructure;
import edu.stanford.slac.aida.provider.sample.impl.DpChannelImpl;
import edu.stanford.slac.aida.provider.sample.standard.AidaChannelProvider;
import edu.stanford.slac.aida.provider.sample.standard.StandardDpChannel;
import org.epics.pvaccess.client.ChannelRequester;

import java.util.HashMap;

/**
 * Implementation of an aida-pca-channel provider.
 */
public class SampleChannelProvider extends AidaChannelProvider {
    private final HashMap<String, PVTopStructure> tops = new HashMap<String, PVTopStructure>();

    public SampleChannelProvider() {
        super();
    }

    public StandardDpChannel newChannel(AidaChannelProvider server, String channelName, ChannelRequester requester) {
        return new DpChannelImpl(this, channelName, requester, getTopStructure(channelName));
    }

    /**
     * Based on channel name select the supported data source
     *
     * @param channelName channel name
     * @return the top structure based on the channel name
     */
    public PVTopStructure getTopStructure(String channelName) {
        PVTopStructure cached = getStructure(channelName);
        if (cached != null)
            return cached;

        synchronized (this) {
            PVTopStructure retVal;

            // For each supported channel
            if ("AIDA:SAMPLE:DEVICE1".equals(channelName)) {
                retVal = new RPCTopStructure();
            } else if ("AIDA:SAMPLE:DEVICE2".equals(channelName)) {
                retVal = new RPCTopStructure();
            } else {
                throw new UnsupportedOperationException("Unsupported channel");
            }

            cacheStructure(channelName, retVal);
            return retVal;
        }
    }


}
