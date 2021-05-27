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

package edu.stanford.slac.aida.provider.sample.standard;

import edu.stanford.slac.aida.lib.ChannelProviderFactory;
import edu.stanford.slac.aida.lib.PVTopStructure;
import edu.stanford.slac.aida.lib.model.AidaProvider;
import edu.stanford.slac.aida.lib.util.AidaPva;
import org.epics.pvaccess.PVFactory;
import org.epics.pvaccess.client.*;
import org.epics.pvdata.factory.ConvertFactory;
import org.epics.pvdata.factory.FieldFactory;
import org.epics.pvdata.pv.*;
import org.epics.pvdata.pv.Status.StatusType;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.logging.Logger;

/**
 * Implementation of an aida-pca-channel provider.
 */
public abstract class AidaChannelProvider extends CheckedAidaChannelProvider implements ChannelProvider {
    public static final Convert convert = ConvertFactory.getConvert();
    public static final PVDataCreate pvDataCreate = PVFactory.getPVDataCreate();
    public static final StatusCreate statusCreate = PVFactory.getStatusCreate();
    public static final Status subFieldNotArrayStatus = statusCreate.createStatus(StatusType.ERROR, "subField is not an array", null);
    public static final Status subFieldDoesNotExistStatus = statusCreate.createStatus(StatusType.ERROR, "subField does not exist", null);
    public static final Status illegalRequestStatus = statusCreate.createStatus(StatusType.ERROR, "illegal pvRequest", null);
    public static final Status fieldDoesNotExistStatus = statusCreate.createStatus(StatusType.ERROR, "field does not exist", null);
    public static final Status okStatus = statusCreate.getStatusOK();
    public static final Status destroyedStatus = statusCreate.createStatus(StatusType.ERROR, "channel destroyed", null);
    private final AidaProvider aidaProvider;

    public AidaChannelProvider() {
        // Aida server Banner on standard output
        System.out.println(AidaPva.banner());
        this.aidaProvider = ChannelProviderFactory.create();
        logHostedChannels();
    }

    protected void cacheStructure(String channelName, PVTopStructure structure) {
        this.aidaProvider.setStructure(channelName, structure);
    }

    protected PVTopStructure getStructure(String channelName) {
        return this.aidaProvider.getStructure(channelName);
    }

    private static final Status channelNotFoundStatus = statusCreate.createStatus(StatusType.ERROR, "channel not found", null);

    public abstract StandardDpChannel newChannel(AidaChannelProvider server, String channelName, ChannelRequester requester);

    /**
     * Based on channel name select the supported data source
     *
     * @param channelName channel name
     * @return the top structure based on the channel name
     */
    public abstract PVTopStructure getTopStructure(String channelName);

    public String getProviderName() {
        return this.aidaProvider.getName();
    }

    protected final ChannelFind channelFind = new ChannelFind() {
        public ChannelProvider getChannelProvider() {
            return AidaChannelProvider.this;
        }

        public void cancel() {
        }
    };


    private boolean isSupported(String channelName) {
        return this.aidaProvider.getChannelNames().contains(channelName);
    }

    public final ChannelFind channelFind(String channelName, ChannelFindRequester requester) {
        parameterCheck(channelName, requester, "channelFind");
        requester.channelFindResult(okStatus, channelFind, isSupported(channelName));
        return channelFind;
    }

    public final ChannelFind channelList(ChannelListRequester requester) {
        parameterCheck(requester, "channelList");
        requester.channelListResult(okStatus, channelFind, this.aidaProvider.getChannelNames(), true);
        return channelFind;
    }

    public Channel createChannel(String channelName, ChannelRequester requester, short priority) {
        parameterCheck(channelName, requester, "createChannel", priority);
        Channel channel = isSupported(channelName) ? newChannel(this, channelName, requester) : null;
        Status status = (channel == null) ? channelNotFoundStatus : okStatus;
        requester.channelCreated(status, channel);

        return channel;
    }

    public Channel createChannel(String channelName, ChannelRequester channelRequester, short priority, String address) {
        throw new UnsupportedOperationException();
    }

    public void destroy() {
    }

    /**
     * Log the list of channels being served
     */
    private void logHostedChannels() {
        System.out.println("\nAIDA-pva Channel Provider hosts the following channels: \n  " + lessStrings(this.aidaProvider.getChannelNames()) + "\n");
    }

    /**
     * Show first 10 strings at most
     *
     * @return 10 or less strings
     */
    private List<String> lessStrings(Set<String> allStringSet) {
        return lessStrings(new ArrayList<String>(allStringSet));
    }

    /**
     * Show first 10 strings at most
     *
     * @return 10 or less strings
     */
    private List<String> lessStrings(List<String> allStringList) {
        List<String> lessStrings;
        try {
            lessStrings = allStringList.subList(0, 9);
            lessStrings.add("...");
        } catch (Exception e) {
            lessStrings = allStringList;
        }
        return lessStrings;
    }
}
