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

import edu.stanford.slac.aida.lib.model.*;
import edu.stanford.slac.aida.lib.util.AidaPva;
import slac.aida.NativeChannelProvider;

import java.util.*;

import static edu.stanford.slac.aida.lib.util.AidaStringUtils.lessStrings;

/**
 * Implementation of an aida-pva-channel provider.
 */
public abstract class ChannelProvider extends NativeChannelProvider {
    private final AidaProvider aidaProvider;
    private final Map<String, AidaChannelConfig> channelConfigMap = new HashMap<String, AidaChannelConfig>();

    /**
     * Constructor for the channel provider that reads the configuration from environment variables and
     * configuration files.  Check AIDA_CHANNELS_FILENAME property or environment variable to control
     * loading of the yaml configuration file
     */
    public ChannelProvider() {
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
     * @param aidaType    the scalar type underpinning this scalar array
     * @param arguments   request arguments
     * @return List of scalar object
     */
    public List<?> requestScalarArray(String channelName, AidaType aidaType, List<AidaArgument> arguments) {
        switch (aidaType) {
            case BOOLEAN_ARRAY:
                List<Boolean> tList = new ArrayList<Boolean>();
                for (Boolean b : aidaRequestBooleanArray(channelName, arguments)) {
                    tList.add(b);
                }
                return tList;
            case BYTE_ARRAY:
                List<Byte> bList = new ArrayList<Byte>();
                for (Byte b : aidaRequestByteArray(channelName, arguments)) {
                    bList.add(b);
                }
                return bList;
            case SHORT_ARRAY:
                List<Short> sList = new ArrayList<Short>();
                for (Short s : aidaRequestShortArray(channelName, arguments)) {
                    sList.add(s);
                }
                return sList;
            case INTEGER_ARRAY:
                List<Integer> iList = new ArrayList<Integer>();
                for (Integer i : aidaRequestIntegerArray(channelName, arguments)) {
                    iList.add(i);
                }
                return iList;
            case LONG_ARRAY:
                List<Long> lList = new ArrayList<Long>();
                for (Long l : aidaRequestLongArray(channelName, arguments)) {
                    lList.add(l);
                }
                return lList;
            case FLOAT_ARRAY:
                List<Float> fList = new ArrayList<Float>();
                for (Float f : aidaRequestFloatArray(channelName, arguments)) {
                    fList.add(f);
                }
                return fList;
            case DOUBLE_ARRAY:
                List<Double> dList = new ArrayList<Double>();
                for (Double d : aidaRequestDoubleArray(channelName, arguments)) {
                    dList.add(d);
                }
                return dList;
            case STRING_ARRAY:
                return Arrays.asList(aidaRequestStringArray(channelName, arguments));
        }
        // TODO handle error
        return null;
    }

    /**
     * Handles tables
     *
     * @param channelName channel name
     * @param arguments   arguments
     * @return return list of lists
     */
    public List<List<Object>> requestTable(String channelName, List<AidaArgument> arguments) {
        AidaTable table = null;
        try {
            table = aidaRequestTable(channelName, arguments);
            return table.asList();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return new ArrayList<List<Object>>();
    }

    /**
     * Get channel configuration
     *
     * @param channelName the channel name to retrieve configuration
     * @return the configuration
     */
    public AidaChannelConfig getChannelConfig(String channelName) {
        AidaChannelConfig channelConfig = null;
        AidaChannel channel = this.aidaProvider.getAidaChannel(channelName);
        if ( channel != null) {
            channelConfig = channel.getConfig();
        }
        return channelConfig;
    }

    /**
     * Called retrieves the native channel config if any is available
     *
     * @param channelName the channel for which config is to be returned
     * @return the channel config
     */
    public AidaChannelConfig getNativeChannelConfig(String channelName) {
        AidaChannelConfig config = channelConfigMap.get(channelName);
        if (config == null) {
            try {
                config = aidaChannelConfig(channelName);
                channelConfigMap.put(channelName, config);
            } catch (UnsatisfiedLinkError unsatisfiedLinkError) {
                System.err.println("Warning! Unsatisfied Link for native call.  Returning empty config");
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
