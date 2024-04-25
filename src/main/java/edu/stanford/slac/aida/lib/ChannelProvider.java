/*
 * @file
 * Abstract implementation of a ChannelProvider which handles requests for scalar, scalarArrays, and tables and transfers them
 * to the appropriate Channel Provider endpoints.
 */
package edu.stanford.slac.aida.lib;

import edu.stanford.slac.aida.lib.model.*;
import edu.stanford.slac.aida.lib.util.AidaPva;
import edu.stanford.slac.except.AidaInternalException;
import edu.stanford.slac.except.ServerInitialisationException;
import slac.aida.NativeChannelProvider;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Set;
import java.util.logging.Logger;

import static edu.stanford.slac.aida.lib.util.AidaStringUtils.lessStrings;

/**
 * Abstract implementation of a ChannelProvider which handles requests for scalar, scalarArrays, and tables and transfers them
 * to the appropriate Channel Provider endpoints.
 * A ChannelProvider is responsible for communicating with the Channel Provider
 * to get and receive information from the AIDA-PVA network.
 * <p>
 * This class encodes generic functionality common to all ChannelProviders on the AIDA-PVA network.
 * Specific customisations should be coded in the {@link edu.stanford.slac.aida.impl.AidaChannelProvider}
 * class.
 *
 * @note This facility if subclassing ChannelProvider will be deprecated
 *
 * <p>
 * This class automatically calls the initialisation of the underlying Channel Provider
 * and shows the AIDA-PVA banner.
 * <p>
 * It also handles the five main classes of requests routing them to the correct underlying
 * - {@link slac.aida.NativeChannelProvider} entry point:
 * - ChannelProvider::requestScalar(String, AidaArguments, AidaType)
 * - ChannelProvider::requestScalarArray(String, AidaArguments, AidaType)
 * - ChannelProvider::requestTable(String, AidaArguments)
 * - ChannelProvider::setValue(String, AidaArguments)
 * - ChannelProvider::setValueWithResponse(String, AidaArguments)
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
     * Uses ChannelProviderFactory::create(ChannelProvider) to check
     * the `aida.pva.channels.filename` property or the `AIDA_PVA_CHANNELS_FILENAME`
     * environment variable to load the yaml configuration file
     * <p>
     * Lists the abbreviated list of hosted channels to the console.
     * <p>
     * Calls the ChannelProvider::logHostedChannels()
     */
    public ChannelProvider() throws ServerInitialisationException {
        // Aida server Banner on standard output
        System.out.println(AidaPva.banner());

        // Create the channel based on environment and configuration files
        this.aidaProvider = ChannelProviderFactory.create(this);

        // Log the configuration of the channel we just created
        logHostedChannels();

        // Call initialisation for the channel provider
        String providerName = getProviderName();
        logger.info("Calling Provider Initialisation for " + providerName + " ...");
        aidaServiceInit();
        logger.info(providerName + " Provider Initialized");
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
     * - collections of scalars e.g. List of Boolean, List of Integer , List of Byte , List of Long , List of Float, List of Double,  ...
     *
     * @param channelName request channel name
     * @param arguments   request arguments
     * @param aidaType    the scalar type underpinning this scalar array
     * @return List of scalar object
     */
    public List<?> requestScalarArray(String channelName, AidaArguments arguments, AidaType aidaType) throws AidaInternalException {
        switch (aidaType) {
            case BOOLEAN_ARRAY:
                List<Boolean> tList = new ArrayList<Boolean>();
                boolean[] tArray = aidaRequestBooleanArray(channelName, arguments);
                if (tArray != null) {
                    for (Boolean t : tArray) {
                        tList.add(t);
                    }
                    return tList;
                }
                break;
            case BYTE_ARRAY:
                List<Byte> bList = new ArrayList<Byte>();
                byte[] bArray = aidaRequestByteArray(channelName, arguments);
                if (bArray != null) {
                    for (Byte b : bArray) {
                        bList.add(b);
                    }
                    return bList;
                }
                break;
            case SHORT_ARRAY:
                List<Short> sList = new ArrayList<Short>();
                short[] sArray = aidaRequestShortArray(channelName, arguments);
                if (sArray != null) {
                    for (Short s : sArray) {
                        sList.add(s);
                    }
                    return sList;
                }
                break;
            case INTEGER_ARRAY:
                List<Integer> iList = new ArrayList<Integer>();
                int[] iArray = aidaRequestIntegerArray(channelName, arguments);
                if (iArray != null) {
                    for (Integer i : iArray) {
                        iList.add(i);
                    }
                    return iList;
                }
                break;
            case LONG_ARRAY:
                List<Long> lList = new ArrayList<Long>();
                long[] lArray = aidaRequestLongArray(channelName, arguments);
                if (lArray != null) {
                    for (Long l : lArray) {
                        lList.add(l);
                    }
                    return lList;
                }
                break;
            case FLOAT_ARRAY:
                List<Float> fList = new ArrayList<Float>();
                float[] fArray = aidaRequestFloatArray(channelName, arguments);
                if (fArray != null) {
                    for (Float f : fArray) {
                        fList.add(f);
                    }
                    return fList;
                }
                break;
            case DOUBLE_ARRAY:
                List<Double> dList = new ArrayList<Double>();
                double[] dArray = aidaRequestDoubleArray(channelName, arguments);
                if (dArray != null) {
                    for (Double d : dArray) {
                        dList.add(d);
                    }
                    return dList;
                }
                break;
            case STRING_ARRAY:
                String[] stringArray = aidaRequestStringArray(channelName, arguments);
                if (stringArray != null) {
                    return Arrays.asList(stringArray);
                }
                break;
        }
        // Should never get here - empty arrays should be handled above and all null responses should already be handled as exceptions
        logger.warning("Received null result when expecting an array");
        throw new AidaInternalException("Received null result when expecting an array");
    }

    /**
     * Handles tables by calling the appropriate Native Method.
     *
     * @param channelName channel name
     * @param arguments   arguments
     * @return return AidaTable
     */
    public AidaTable requestTable(String channelName, AidaArguments arguments) throws AidaInternalException {
        AidaTable table = aidaRequestTable(channelName, arguments);
        if ( table == null ) {
            logger.warning("Received null result when requesting a table");
            throw new AidaInternalException("Received null result when requesting a table");
        }
        return table;
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
     * @return return AidaTable
     */
    public AidaTable setValueWithResponse(String channelName, AidaArguments arguments) throws AidaInternalException {
        AidaTable table = aidaSetValueWithResponse(channelName, arguments);
        if ( table == null ) {
            logger.warning("Received null result when expecting a table");
            throw new AidaInternalException("Received null result when expecting a table");
        }
        return table;
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
     * Gets the configured transcoding method
     * @return The configured transcoding method
     */
    public final TranscodingMethod getTranscodingMethod() {
        return this.aidaProvider.getTranscode();
    }

    /**
     * Log the list of channels being served
     */
    private void logHostedChannels() {
        logger.info("\nAIDA-pva Channel Provider : " + getProviderName());
        logger.info("Channels hosted: \n  " + lessStrings(this.aidaProvider.getChannelNames()) + "\n");
    }
}
