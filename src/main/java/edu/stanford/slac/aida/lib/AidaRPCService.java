package edu.stanford.slac.aida.lib;

import edu.stanford.slac.aida.lib.model.*;
import edu.stanford.slac.except.*;
import org.epics.nt.NTURI;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvaccess.server.rpc.RPCService;
import org.epics.pvdata.pv.PVField;
import org.epics.pvdata.pv.PVString;
import org.epics.pvdata.pv.PVStructure;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;
import java.util.logging.Logger;
import java.util.regex.Pattern;

import static edu.stanford.slac.aida.lib.model.AidaType.*;
import static edu.stanford.slac.aida.lib.util.AidaPVHelper.*;
import static org.epics.pvdata.pv.Status.StatusType.ERROR;

/**
 * @file The AIDA-PVA Service which provides connection to the AIDA-PVA Native Channel Providers and the SLAC Network.
 */
public class AidaRPCService implements RPCService {
    /**
     * Logger to log info
     */
    private static final Logger logger = Logger.getLogger(AidaRPCService.class.getName());

    private final ChannelProvider aidaChannelProvider;

    public AidaRPCService(ChannelProvider aidaChannelProvider) {
        this.aidaChannelProvider = aidaChannelProvider;
    }

    /**
     * Callback when a channel is called
     *
     * @param pvUri the uri passed to the channel containing the name, query, and arguments
     * @return the result of the call
     * @throws RPCRequestException              if any error occurs formulating the request or decoding the response
     * @throws AidaInternalException            if any error occurs because of an implementation error in aida server code
     * @throws MissingRequiredArgumentException when a required argument was not supplied
     * @throws UnableToGetDataException         when server fails to retrieve data
     * @throws UnableToSetDataException         when server fails to set data
     * @throws UnsupportedChannelException      when server does not yet support the specified channel.
     *                                          Usually caused when channel matches a pattern specified in the channels.yml file
     *                                          but is not yet supported in the service implementation
     */
    public PVStructure request(PVStructure pvUri) throws RPCRequestException, UnableToGetDataException, UnsupportedChannelException, UnableToSetDataException, AidaInternalException, MissingRequiredArgumentException {
        PVStructure retVal;
        try {
            // Check that the parameter is always a normative type
            String type = pvUri.getStructure().getID();
            if (!NTURI.is_a(pvUri.getStructure())) {
                String msg = "Unable to get data, unexpected request type: " + type;
                throw new RPCRequestException(ERROR, msg);
            }

            // Retrieve the PV name
            PVString pvPathField = pvUri.getStringField("path");
            if (pvPathField == null) {
                throw new RPCRequestException(ERROR, "unable to determine the channel from the request specified: " + pvUri);
            }
            String channelName = pvPathField.get();
            if (channelName == null || channelName.length() == 0) {
                throw new RPCRequestException(ERROR, "unable to determine the channel from the request specified: <blank>");
            }

            // Retrieve arguments, if any given to this RPC PV channel.
            PVStructure pvUriQuery = pvUri.getStructureField("query");
            List<AidaArgument> arguments = getArguments(pvUriQuery);

            // Make sure that only one request occurs at a time because
            // the implementations are not thread safe.
            synchronized (this) {
                retVal = request(channelName, arguments);
            }
        } catch (RPCRequestException e) {
            throw e;
        } catch (UnableToGetDataException e) {
            throw new RPCRequestException(ERROR, e.getMessage(), e);
        } catch (UnsupportedChannelException e) {
            throw new RPCRequestException(ERROR, e.getMessage(), e);
        } catch (UnableToSetDataException e) {
            throw new RPCRequestException(ERROR, e.getMessage(), e);
        } catch (AidaInternalException e) {
            throw new RPCRequestException(ERROR, e.getMessage(), e);
        } catch (MissingRequiredArgumentException e) {
            throw new RPCRequestException(ERROR, e.getMessage(), e);
        } catch (Throwable e) {
            throw new RPCRequestException(ERROR, e.getMessage(), e);
        }

        return retVal;
    }

    /**
     * Make request to the specified channel with the uri and arguments specified
     * and return the NT_TABLE of results.
     *
     * @param channelName   channel name
     * @param argumentsList arguments if any
     * @return the structure containing the results.
     * @throws AidaInternalException            if any error occurs because of an implementation error in aida server code
     * @throws MissingRequiredArgumentException when a required argument was not supplied
     * @throws UnableToGetDataException         when server fails to retrieve data
     * @throws UnableToSetDataException         when server fails to set data
     * @throws UnsupportedChannelException      when server does not yet support the specified channel.
     *                                          Usually caused when channel matches a pattern specified in the channels.yml file
     *                                          but is not yet supported in the service implementation
     */
    private PVStructure request(String channelName, List<AidaArgument> argumentsList) throws UnableToGetDataException, UnsupportedChannelException, UnableToSetDataException, AidaInternalException, MissingRequiredArgumentException {
        AidaType aidaType;
        AidaChannelOperationConfig config;
        String typeArgument = null;
        boolean isSetterRequest = false;

        {
            AidaChannelOperationConfig getterConfig = aidaChannelProvider.getGetterConfig(channelName);
            AidaChannelOperationConfig setterConfig = aidaChannelProvider.getSetterConfig(channelName);
            AidaType aidaGetterType = getterConfig == null ? NONE : getterConfig.getType();
            AidaType aidaSetterType = setterConfig == null ? NONE : setterConfig.getType();

            // Get special arguments `TYPE` and `VALUE` used to determine which APIs will be called
            for (AidaArgument argument : argumentsList) {
                String argumentName = argument.getName();
                if (argumentName.equalsIgnoreCase("TYPE")) {
                    typeArgument = argument.getValue().toUpperCase();
                } else if (argumentName.equalsIgnoreCase("VALUE")) {
                    isSetterRequest = true;
                }
            }

            aidaType = isSetterRequest ? aidaSetterType : aidaGetterType;
            config = isSetterRequest ? setterConfig : getterConfig;
        }

        // See if the request type is supported for the channel
        checkThatRequestedOperationIsSupported(channelName, argumentsList, isSetterRequest, aidaType);

        if (typeArgument == null) { // If no TYPE has been specified then see if it was mandatory
            checkIfMissingTypeArgumentIsMandatory(channelName, argumentsList, isSetterRequest, aidaType);
        } else { // If a TYPE has been specified then override the Config's type with the specified one if it is compatible
            aidaType = checkIfTypeArgumentIsCompatible(channelName, argumentsList, isSetterRequest, aidaType, typeArgument);
        }

        // Verify that, if the request is a table request, the configuration defines fields
        checkThatConfigDefinesFieldsForTableRequests(aidaType, config);

        // Validate that the given arguments are allowed for this operation on this channel
        validateArguments(channelName, argumentsList, isSetterRequest, config);

        // If channelName contains a service (<service>::channelName) then remove the service part before calling the service implementation
        // This is a special measure put in place to disambiguate some channels that are used by more than one Service.
        // It allows for channels to be prefixed by "<serviceName>::".  This is done in the channel configuration file and also
        // by clients accessing the service.  But the Native Channel Provider will not know about this prefix so we need to
        // remove it before passing the request on to the Native Channel Provider
        channelName = removeServicePrefixIfPresent(channelName);

        // If the client has specified the channel with the legacy formatted channel name we need to change it to the new format before
        // passing it to the Native Channel Provider which will be expecting only new format names
        channelName = ensureNewFormatChannelName(channelName);

        // Display the log entry that indicated the request that is being passed to the Native Channel Provider with its parameters and its expected return type
        logRequest(channelName, argumentsList, isSetterRequest, aidaType);

        // Make an arguments object to pass to requests
        AidaArguments arguments = new AidaArguments(argumentsList);

        // Call entry point based on return type
        return callNativeChannelProvider(channelName, arguments, isSetterRequest, aidaType, config);
    }

    /**
     * Call the correct entry point based on the selected channel, the channel configuration, the type of operation requested (get/set)
     *
     * @param channelName     the name of the channel
     * @param arguments       the request's arguments
     * @param isSetterRequest true if this is a set operation
     * @param aidaType        the return type
     * @param config          the configuration
     * @return the PVStructure returned from the native call
     * @throws UnsupportedChannelException if operation is invalid for channel
     */
    private PVStructure callNativeChannelProvider(String channelName, AidaArguments arguments, boolean isSetterRequest, AidaType aidaType, AidaChannelOperationConfig config) throws UnsupportedChannelException {
        if (isSetterRequest) {  // Setter requests
            if (aidaType == VOID) { // Returning VOID
                this.aidaChannelProvider.setValue(channelName, arguments);

            } else if (aidaType == TABLE) { // Returning TABLES
                return asNtTable(this.aidaChannelProvider.setValueWithResponse(channelName, arguments), config);
            }

        } else { // Getter requests
            AidaType metaType = aidaType.metaType();

            if (metaType == SCALAR) { // Returning SCALAR
                return asScalar(this.aidaChannelProvider.requestScalar(channelName, arguments, aidaType));

            } else if (metaType == SCALAR_ARRAY) { // Returning SCALAR_ARRAY
                return asScalarArray(this.aidaChannelProvider.requestScalarArray(channelName, arguments, aidaType));

            } else { // Returning TABLE
                return asNtTable(this.aidaChannelProvider.requestTable(channelName, arguments), config);
            }
        }
        return NT_SCALAR_EMPTY_STRUCTURE;
    }

    /**
     * If channelName contains a service (<service>::channelName) then remove the service part before calling the service implementation
     * This is a special measure put in place to disambiguate some channels that are used by more than one Service.
     * It allows for channels to be prefixed by "<serviceName>::".  This is done in the channel configuration file and also
     * by clients accessing the service.  But the Native Channel Provider will not know about this prefix so we need to
     * remove it before passing the request on to the Native Channel Provider
     *
     * @param channelName the channel name to check for prefix
     * @return the channel name without the prefix if it was present
     */
    private String removeServicePrefixIfPresent(String channelName) {
        int servicePrefix = channelName.indexOf("::");
        if (servicePrefix != -1) {
            channelName = channelName.substring(servicePrefix + 2);
        }
        return channelName;
    }

    /**
     * If the client has specified the channel with the legacy formatted channel name we need to change it to the new format before
     * passing it to the Native Channel Provider which will be expecting only new format names
     *
     * @param channelName the channel name
     * @return the channel name in the new format
     */
    private String ensureNewFormatChannelName(String channelName) {
        int indexOfLastLegacySeparator = channelName.lastIndexOf("//");
        if (indexOfLastLegacySeparator != -1) {
            channelName = channelName.substring(0, indexOfLastLegacySeparator) + ":" + channelName.substring(indexOfLastLegacySeparator + 2);
        }
        return channelName;
    }

    /**
     * See if the request type is supported for the channel
     *
     * @param channelName     the channel name
     * @param argumentsList   the list of arguments
     * @param isSetterRequest is this a set/get operation?
     * @param aidaType        the request type
     * @throws UnsupportedChannelException if the requested operation is not supported
     */
    private void checkThatRequestedOperationIsSupported(String channelName, List<AidaArgument> argumentsList, boolean isSetterRequest, AidaType aidaType) throws UnsupportedChannelException {
        if (aidaType == NONE) {
            throw new UnsupportedChannelTypeException(channelName + argumentsList + ".  " + (isSetterRequest ? "Set" : "Get") + " requests are not supported for this channel");
        }
    }

    /**
     * Check that the configuration defines fields for table requests so that we will know how to create the {@link PVStructure} to return the result in
     *
     * @param aidaType the request type
     * @param config   the configuration
     * @throws AidaInternalException if the configuration file has been mis-configured
     */
    private void checkThatConfigDefinesFieldsForTableRequests(AidaType aidaType, AidaChannelOperationConfig config) throws AidaInternalException {
        if (aidaType == TABLE && (config == null || config.getFields() == null)) {
            throw new AidaInternalException("Table channel configured without defining fields");
        }
    }

    /**
     * Get the {@link AidaType }  of the specified TYPE argument but check that it is compatible with the operation requested on the channel
     *
     * @param channelName     the channel name
     * @param argumentsList   the list of arguments
     * @param isSetterRequest is this a set/get operation?
     * @param aidaType        the request type
     * @param typeArgument    the specified TYPE argument
     * @return the {@link AidaType} of the specified TYPE argument
     * @throws UnsupportedChannelException if not compatible
     */
    private AidaType checkIfTypeArgumentIsCompatible(String channelName, List<AidaArgument> argumentsList, boolean isSetterRequest, AidaType aidaType, String typeArgument) throws UnsupportedChannelException {
        AidaType specifiedAidaType = AidaType.valueOf(typeArgument);

        // Check that it matches the type class from the channels file
        // if channel accepts any type or if the specified type is TABLE (which matches any configuration)
        // or if the specified type is in the class of types allowed by the channel
        if (!isSpecifiedTypeCompatibleWithConfiguredType(specifiedAidaType, aidaType)) {
            throw new UnsupportedChannelTypeException(channelName + argumentsList + ".  The type specified by the 'Type' parameter must be " + (isSetterRequest ? "either VOID or TABLE" : ("a " + aidaType)) + ", but you specified " + specifiedAidaType);
        }

        return specifiedAidaType;
    }

    /**
     * When a TYPE parameter has not been specified, check to see if it was indeed required and raise an exception if so
     *
     * @param channelName     the channel name
     * @param argumentsList   the arguments list
     * @param isSetterRequest is this a set/get request?
     * @param aidaType        the request type
     * @throws UnsupportedChannelException if the TYPE parameter was required but not provided
     */
    private void checkIfMissingTypeArgumentIsMandatory(String channelName, List<AidaArgument> argumentsList, boolean isSetterRequest, AidaType aidaType) throws UnsupportedChannelException {
        if (isSetterRequest) {  // If a meta-type is set as the setter type, but the client request didn't explicitly specify a TYPE argument then error out
            if (aidaType == ANY) {
                throw new UnsupportedChannelTypeException(channelName + argumentsList + ".  The 'Type' parameter must be set 'VOID' or 'TABLE' but you didn't specify one");
            }
        } else { // If a meta-type is set as the getter type, but the client request didn't explicitly specify a TYPE argument then error out
            switch (aidaType) {
                case SCALAR:
                case SCALAR_ARRAY:
                case ANY:
                    throw new UnsupportedChannelTypeException(channelName + argumentsList + ".  The 'Type' parameter must be set to a type corresponding to " + aidaType + ", but you didn't specify one");
            }
        }
    }

    /**
     * Validate arguments:  Check that the given arguments are valid for the operation on the channel that has been requested
     *
     * @param channelName     the channel
     * @param argumentsList   the list of arguments
     * @param isSetterRequest true if this is a setter request
     * @param config          the configuration
     * @throws UnsupportedChannelException if any argument is not valid
     */
    private void validateArguments(String channelName, List<AidaArgument> argumentsList, boolean isSetterRequest, AidaChannelOperationConfig config) throws UnsupportedChannelException {
        for (AidaArgument argument : argumentsList) {
            String argumentName = argument.getName().toUpperCase();

            if (!"TYPE".equals(argumentName) && !"VALUE".equals(argumentName)) { // Ignore TYPE and VALUE arguments because they are validated elsewhere
                List<String> arguments = config.getArguments();
                if (arguments == null || !arguments.contains(argumentName)) { // if the given argument name is not in the acceptable set of argument names then error out
                    throw new UnsupportedChannelException(channelName +
                            ":  " + argumentName +
                            " is not a valid argument for " +
                            (isSetterRequest ? "set" : "get") +
                            " requests to this channel.  " +
                            (arguments == null ? "No arguments are allowed" : ("Valid arguments are: " + arguments)));
                }
            }
        }
    }

    /**
     * Display the log entry that indicated the request that is being passed to the
     * Native Channel Provider with its parameters and its expected return type
     *
     * @param channelName     the channel name
     * @param argumentsList   the arguments
     * @param isSetterRequest is this a set/get operation
     * @param aidaType        the request type
     */
    private void logRequest(String channelName, List<AidaArgument> argumentsList, boolean isSetterRequest, AidaType aidaType) {
        String normativeType = ntTypeOf(aidaType);
        logger.info("AIDA " + (isSetterRequest ? "SetValue" : "GetValue") + ": " + channelName + argumentsList + " => " + aidaType + (normativeType == null ? "" : ("::" + normativeType)));
    }

    /**
     * Get the arguments for the specified request.  Returns the list of {@link AidaArgument} for the
     * given Normative Type query {@link PVStructure}
     *
     * @param pvUriQuery the given Normative Type query {@link PVStructure}
     * @return the list of {@link AidaArgument}
     * @throws RPCRequestException if there is a problem reading the arguments
     */
    private List<AidaArgument> getArguments(PVStructure pvUriQuery) throws RPCRequestException {
        List<AidaArgument> arguments = new ArrayList<AidaArgument>();
        if (pvUriQuery != null) {
            PVField[] pvFields = pvUriQuery.getPVFields();
            for (PVField field : pvFields) {
                arguments.add(getArgument(field));
            }
        }
        return arguments;
    }

    /**
     * Convert a {@link PVField} into an {@link AidaArgument}
     *
     * @param field the {@link PVField} to convert into an {@link AidaArgument}
     * @return the {@link AidaArgument}
     * @throws RPCRequestException if something bad happens
     */
    private AidaArgument getArgument(PVField field) throws RPCRequestException {
        String name = field.getFieldName();
        if (name == null) {
            throw new RPCRequestException(ERROR, "Invalid argument name: <blank>");
        }

        // To store list of all floating point numbers found in this argument.  They are
        // lists because the arguments can be structures that contain many floating points
        // in various elements.  When extracting those elements the full json path to
        // the element in the structure is saved
        List<FloatArgument> floatArgumentList = new ArrayList<FloatArgument>();
        List<DoubleArgument> doubleArgumentList = new ArrayList<DoubleArgument>();

        // Convert all arguments to string (json or otherwise)
        // But collect all doubles and floats unchanged in ieee format
        String value = fieldToString(field, name, floatArgumentList, doubleArgumentList);

        if (value == null) {
            throw new RPCRequestException(ERROR, "Invalid argument value: <blank>");
        }

        // json parser in AIDA-PVA Module in AIDASHR is a bit flaky and crashes when it does not
        // get properly formed json, so we'll parse it here to make sure it's ok
        String trimmedValue = value.trim();
        if (trimmedValue.startsWith("[")) {
            try {
                new JSONObject("{\"_array\": " + trimmedValue + "}");
            } catch (JSONException e) {
                throw new RPCRequestException(ERROR, "Invalid json detected: argument name: " + name + ", value: " + value);
            }
            checkForLongsTerminatedByL(name, value, trimmedValue);
        } else if (trimmedValue.startsWith("{")) {
            try {
                new JSONObject(trimmedValue);
            } catch (JSONException e) {
                throw new RPCRequestException(ERROR, "Invalid json detected: argument name: " + name + ", value: " + value);
            }
            checkForLongsTerminatedByL(name, value, trimmedValue);
        }

        return new AidaArgument(name, value, floatArgumentList, doubleArgumentList);
    }

    /**
     * Simple Check for json containing literal long numbers with a suffix of "L" these are too much for the
     * simple json parser on the C side of the JNI interface
     *
     * @param name         argument name
     * @param value        argument value
     * @param trimmedValue trimmed argument value
     * @throws RPCRequestException if contains an invalid literal long
     */
    private void checkForLongsTerminatedByL(String name, String value, String trimmedValue) throws RPCRequestException {
        if (Pattern.compile(".*[0-9]+L.*").matcher(trimmedValue).matches()) {
            throw new RPCRequestException(ERROR, "JSON Long values cannot be terminated by 'L' with this json parser: argument name: " + name + ", value: " + value);
        }
    }
}
