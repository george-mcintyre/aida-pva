package edu.stanford.slac.aida.lib;

import edu.stanford.slac.aida.lib.model.*;
import edu.stanford.slac.aida.lib.util.AidaPva;
import slac.aida.NativeChannelProvider;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Set;
import java.util.logging.Logger;

import static edu.stanford.slac.aida.lib.util.AidaStringUtils.lessStrings;

/**
 * @file
 * Abstract implementation of a ChannelProvider which handles requests for scalar, scalarArrays, and tables and transfers them
 * to the appropriate Channel Provider endpoints.
 * A ChannelProvider is responsible for communicating with the Channel Provider
 * to get and receive information from the AIDA-PVA network.
 * <p>
 * This class encodes generic functionality common to all ChannelProviders on the AIDA-PVA network.
 * Specific customisations should be coded in the {@link edu.stanford.slac.aida.impl.AidaChannelProvider}
 * class.
 *
 * @note  This facility if subclassing ChannelProvider will be deprecated
 *
 * <p>
 * This class automatically calls the initialisation of the underlying Channel Provider
 * and shows the AIDA-PVA banner.
 * <p>
 * It also handles the five main classes of requests routing them to the correct underlying
 * - {@link slac.aida.NativeChannelProvider} entry point:
 * - {@link ChannelProvider#requestScalar(String, AidaArguments, AidaType)}
 * - {@link ChannelProvider#requestScalarArray(String, AidaArguments, AidaType)}
 * - {@link ChannelProvider#requestTable(String, AidaArguments)}
 * - {@link ChannelProvider#setValue(String, AidaArguments)}
 * - {@link ChannelProvider#setValueWithResponse(String, AidaArguments)}
 */
public abstract class ChannelProvider extends NativeChannelProvider {
    /**
     * Logger to log info
     */
    private static final Logger logger = Logger.getLogger(ChannelProvider.class.getName());

    private final AidaProvider aidaProvider;

    /**
     * Constructor for the channel provider that reads the configuration from environment variables and
     * configuration files.
     * <p>
     * Uses {@link ChannelProviderFactory#create(ChannelProvider)} to check
     * the `aida.pva.channels.filename` property or the `AIDA_PVA_CHANNELS_FILENAME`
     * environment variable to load the yaml configuration file
     * <p>
     * Lists the abbreviated list of hosted channels to the console.
     * <p>
     * Calls the {@link ChannelProvider#logHostedChannels()}
     */
    public ChannelProvider() {
        // Aida server Banner on standard output
        System.out.println(AidaPva.banner());

        // Create the channel based on environment and configuration files
        this.aidaProvider = ChannelProviderFactory.create(this);

        // Log the configuration of the channel we just created
        logHostedChannels();

        // Call initialisation for the channel provider
        aidaServiceInit();
    }

    /**
     * Handles scalar requests by calling the appropriate Native Method.  This can return :-
     * - scalars e.g. Boolean, Integer, Byte, Long, Float, Double, ...
     *
     * @param channelName request channel name
     * @param arguments   request arguments
     * @param aidaType    the return type
     * @return object
     */
    public Object requestScalar(String channelName, AidaArguments arguments, AidaType aidaType) {
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
        return null;
    }

    /**
     * Handles scalar array requests by calling the appropriate Native Method.  This can return :-
     * - collections of scalars e.g. List of {@link Boolean}, List of {@link Integer} , List of {@link Byte} , List of {@link Long} , List of {@link Float}, List of {@link Double},  ...
     *
     * @param channelName request channel name
     * @param arguments   request arguments
     * @param aidaType    the scalar type underpinning this scalar array
     * @return List of scalar object
     */
    public List<?> requestScalarArray(String channelName, AidaArguments arguments, AidaType aidaType) {
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
        return null;
    }

    /**
     * Handles tables by calling the appropriate Native Method.
     *
     * @param channelName channel name
     * @param arguments   arguments
     * @return return list of lists
     */
    public List<List<Object>> requestTable(String channelName, AidaArguments arguments) {
        return aidaRequestTable(channelName, arguments).asList();
    }

    /**
     * Handles setting values with a void return by calling the appropriate Native Method.
     *
     * @param channelName channel name
     * @param arguments   arguments
     *                    contains {@code value} that specifies the value as a literal string or in json if it is a table
     */
    public void setValue(String channelName, AidaArguments arguments) {
        aidaSetValue(channelName, arguments);
    }

    /**
     * Handles setting values with a void return by calling the appropriate Native Method.
     *
     * @param channelName channel name
     * @param arguments   arguments
     *                    contains {@code value} that specifies the value as a literal string or in json if it is a table
     * @return return list of lists
     */
    public List<List<Object>> setValueWithResponse(String channelName, AidaArguments arguments) {
        return aidaSetValueWithResponse(channelName, arguments).asList();
    }

    /**
     * Get Setter configuration
     *
     * @param channelName the channel name to retrieve configuration
     * @return the configuration
     */
    public AidaChannelOperationConfig getSetterConfig(String channelName) {
        AidaChannel channel = this.aidaProvider.getAidaChannel(channelName);
        return (channel == null) ? null : channel.getSetterConfig();
    }

    /**
     * Get Getter configuration
     *
     * @param channelName the channel name to retrieve configuration
     * @return the configuration
     */
    public AidaChannelOperationConfig getGetterConfig(String channelName) {
        AidaChannel channel = this.aidaProvider.getAidaChannel(channelName);
        return (channel == null) ? null : channel.getGetterConfig();
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
        logger.info("\nAIDA-pva Channel Provider : " + getProviderName());
        logger.info("Channels hosted: \n  " + lessStrings(this.aidaProvider.getChannelNames()) + "\n");
    }
}
