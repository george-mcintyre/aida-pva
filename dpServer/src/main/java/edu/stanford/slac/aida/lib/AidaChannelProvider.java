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

package edu.stanford.slac.aida.lib;

import edu.stanford.slac.aida.lib.model.AidaArgument;
import edu.stanford.slac.aida.lib.model.AidaChannelConfig;
import edu.stanford.slac.aida.lib.model.AidaProvider;
import edu.stanford.slac.aida.lib.model.AidaType;
import edu.stanford.slac.aida.lib.util.AidaPva;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import static edu.stanford.slac.aida.lib.util.AidaStringUtils.lessStrings;

/**
 * Implementation of an aida-pva-channel provider.
 */
public abstract class AidaChannelProvider extends AidaNativeChannelProvider {
    private final AidaProvider aidaProvider;
    private final Map<String, AidaChannelConfig> channelConfigMap = new HashMap<String, AidaChannelConfig>();

    /**
     * Constructor for the channel provider that reads the configuration from environment variables and
     * configuration files.  Check AIDA_CHANNELS_FILENAME property or environment variable to control
     * loading of the yaml configuration file
     */
    public AidaChannelProvider() {
        // Aida server Banner on standard output
        System.out.println(AidaPva.banner());

        // Create the channel based on environment and configuration files
        this.aidaProvider = ChannelProviderFactory.create(this);

        // Log the configuration of the channel we just created
        logHostedChannels();
    }

    /**
     * Handles scalar requests.  This can return :-
     * - scalars e.g. Boolean, Integer, Byte, Long, Float, Double, ...
     *
     * @param channelName request channel name
     * @param arguments   request arguments
     * @return object
     */
    public Object requestScalar(String channelName, AidaType aidaType, List<AidaArgument> arguments) {
        switch (aidaType) {
            case BOOLEAN:
                return aidaRequestBoolean(channelName, arguments);
            case BYTE:
                return aidaRequestByte(channelName, arguments);
            case SHORT:
                return aidaRequestShort(channelName, arguments);
            case INTEGER:
                return aidaRequestInteger(channelName, arguments);
            case LONG:
                return aidaRequestLong(channelName, arguments);
            case FLOAT:
                return aidaRequestFloat(channelName, arguments);
            case DOUBLE:
                return aidaRequestDouble(channelName, arguments);
            case STRING:
                return aidaRequestString(channelName, arguments);
        }
        // TODO handle error
        return null;
    }

    /**
     * Handles scalar array requests.  This can return :-
     * - collections of scalars e.g. List<Boolean>, List<Integer>, List<Byte>, List<Long>, List<Float>, List<Double>,  ...
     *
     * @param channelName request channel name
     * @param aidaType the scalar type underpinning this scalar array
     * @param arguments   request arguments
     * @return List of scalar object
     */
    public List<?> requestScalarArray(String channelName, AidaType aidaType, List<AidaArgument> arguments) {
        switch (aidaType) {
            case BOOLEAN_ARRAY:
                return aidaRequestBooleanArray(channelName, arguments);
            case BYTE_ARRAY:
                return aidaRequestByteArray(channelName, arguments);
            case SHORT_ARRAY:
                return aidaRequestShortArray(channelName, arguments);
            case INTEGER_ARRAY:
                return aidaRequestIntegerArray(channelName, arguments);
            case LONG_ARRAY:
                return aidaRequestLongArray(channelName, arguments);
            case FLOAT_ARRAY:
                return aidaRequestFloatArray(channelName, arguments);
            case DOUBLE_ARRAY:
                return aidaRequestDoubleArray(channelName, arguments);
            case STRING_ARRAY:
                return aidaRequestStringArray(channelName, arguments);
        }
        // TODO handle error
        return null;
    }

    /**
     * Handles tables
     * @param channelName channel name
     * @param arguments arguments
     * @return return list of lists
     */
    public List<List<?>> requestTable(String channelName, List<AidaArgument> arguments) {
        return aidaRequestTable(channelName, arguments);
    }

    /**
     * Called by the framework during channel initialisation to get channel config
     * and also when making aida requests to retrieve cached config
     *
     * @param channelName the channel for which config is to be returned
     * @return the channel config
     */
    public AidaChannelConfig getChannelConfig(String channelName) {
        AidaChannelConfig config = channelConfigMap.get(channelName);
        if ( config == null ) {
            try {
                config = aidaChannelConfig(channelName);
                channelConfigMap.put(channelName, config);
            } catch (UnsatisfiedLinkError unsatisfiedLinkError) {
                config = new AidaChannelConfig();
            }
        }
        return config;
    }

    /**
     * Get the name of this channel provider
     *
     * @return name of channel provider
     */
    public String getProviderName() {
        return this.aidaProvider.getName();
    }

    /**
     * Get the names of the channels hosted by this channel provider
     *
     * @return channel names
     */
    public Set<String> getChannelNames() {
        return this.aidaProvider.getChannelNames();
    }

    /**
     * Log the list of channels being served
     */
    private void logHostedChannels() {
        System.out.println("\nAIDA-pva Channel Provider : " + getProviderName());
        System.out.println("Channels hosted: \n  " + lessStrings(this.aidaProvider.getChannelNames()) + "\n");
    }
}
