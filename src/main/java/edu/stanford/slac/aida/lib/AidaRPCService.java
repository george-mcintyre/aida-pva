package edu.stanford.slac.aida.lib;

import edu.stanford.slac.aida.lib.model.AidaArgument;
import edu.stanford.slac.aida.lib.model.AidaChannelConfig;
import edu.stanford.slac.aida.lib.model.AidaType;
import edu.stanford.slac.except.*;
import org.epics.nt.NTURI;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvaccess.server.rpc.RPCService;
import org.epics.pvdata.pv.*;

import java.util.ArrayList;
import java.util.List;

import static edu.stanford.slac.aida.lib.model.AidaProvider.getAidaName;
import static edu.stanford.slac.aida.lib.model.AidaType.*;
import static edu.stanford.slac.aida.lib.util.AidaPVHelper.*;
import static java.lang.Boolean.FALSE;
import static java.lang.Boolean.TRUE;
import static org.epics.pvdata.pv.Type.scalar;
import static org.epics.pvdata.pv.Type.scalarArray;

public class AidaRPCService implements RPCService {
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
        // Check that the parameter is always a normative type
        String type = pvUri.getStructure().getID();
        if (!NTURI.is_a(pvUri.getStructure())) {
            String msg = "Unable to get data, unexpected request type: " + type;
            throw new RPCRequestException(Status.StatusType.ERROR, msg);
        }

        // Retrieve the PV name
        PVString pvPathField = pvUri.getStringField("path");
        if (pvPathField == null) {
            throw new RPCRequestException(Status.StatusType.ERROR, "unable to determine the channel from the request specified: " + pvUri);
        }
        String channelName = pvPathField.get();
        if (channelName == null || channelName.length() == 0) {
            throw new RPCRequestException(Status.StatusType.ERROR, "unable to determine the channel from the request specified: <blank>");
        }

        // Convert the channelName to old AIDA format including '//' before the attribute name
        // So that services can use simple matching to decode the pvName
        channelName = getAidaName(channelName);

        // Retrieve arguments, if any given to this RPC PV channel.
        PVStructure pvUriQuery = pvUri.getStructureField("query");
        List<AidaArgument> arguments = getArguments(pvUriQuery);

        // Make sure that only one request occurs at a time because
        // the implementations are not thread safe.
        PVStructure retVal;
        synchronized (this) {
            retVal = request(channelName, arguments);
        }

        return retVal;
    }

    /**
     * Make request to the specified channel with the uri and arguments specified
     * and return the NT_TABLE of results.
     *
     * @param channelName channel name
     * @param arguments   arguments if any
     * @return the structure containing the results.
     * @throws AidaInternalException            if any error occurs because of an implementation error in aida server code
     * @throws MissingRequiredArgumentException when a required argument was not supplied
     * @throws UnableToGetDataException         when server fails to retrieve data
     * @throws UnableToSetDataException         when server fails to set data
     * @throws UnsupportedChannelException      when server does not yet support the specified channel.
     *                                          Usually caused when channel matches a pattern specified in the channels.yml file
     *                                          but is not yet supported in the service implementation
     */
    private PVStructure request(String channelName, List<AidaArgument> arguments) throws UnableToGetDataException, UnsupportedChannelException, UnableToSetDataException, AidaInternalException, MissingRequiredArgumentException {
        AidaChannelConfig channelGetterConfig = aidaChannelProvider.getChannelConfig(channelName, TRUE);
        AidaChannelConfig channelSetterConfig = aidaChannelProvider.getChannelConfig(channelName, FALSE);

        if (channelGetterConfig == null) {
            throw new UnsupportedChannelException("Could not find configuration for this channel.  Perhaps the channels.yml file contains an invalid pattern: " + channelName);
        }

        AidaType aidaGetterType = channelGetterConfig.getType();
        AidaType aidaSetterType = channelSetterConfig.getType();

        // Get special arguments type and value
        String typeArgument = null;
        AidaArgument valueArgument = null;
        for (AidaArgument argument : arguments) {
            if (argument.getName().equalsIgnoreCase("Type")) {
                typeArgument = argument.getValue().toUpperCase();
            }
            if (argument.getName().equalsIgnoreCase("Value")) {
                valueArgument = argument;
            }
        }

        // If you've specified a type then override the getterConfig type with the specified one
        if (typeArgument != null) {
            try {
                AidaType specifiedAidaType = AidaType.valueOf(typeArgument);

                // Check that it matches the type class from the channels file
                // if channel accepts any type or if the specified type is TABLE (which matches any configuration)
                // or if the specified type is in the class of types allowed by the channel
                if (aidaGetterType.equals(ANY) || specifiedAidaType.equals(TABLE) ||
                        ((aidaGetterType.equals(SCALAR) || aidaGetterType.equals(SCALAR_ARRAY)) && specifiedAidaType.metaType().equals(aidaGetterType.metaType()))) {
                    aidaGetterType = specifiedAidaType;
                } else {
                    throw new UnsupportedChannelTypeException("The type specified by the 'Type' parameter must be a " + aidaGetterType + ", but you specified " + specifiedAidaType);
                }
            } catch (IllegalArgumentException e) {
                throw new UnsupportedChannelTypeException("The type specified by the 'Type' parameter is not a recognised AIDA type" + typeArgument);
            }
        } else {
            // If no class is specified then use a default for each type class if the channel config specifies that it needs one
            switch (aidaGetterType) {
                case SCALAR:
                    aidaGetterType = INTEGER;
                    break;
                case SCALAR_ARRAY:
                    aidaGetterType = INTEGER_ARRAY;
                    break;
                case ANY:
                    aidaGetterType = TABLE;
            }
        }

        Type channelGetterType = typeOf(aidaGetterType);
        Type channelSetterType = typeOf(aidaSetterType);
        Boolean isGetterRequest;
        if (!aidaSetterType.equals(NONE) && valueArgument != null) {
            isGetterRequest = FALSE;
            System.out.println("AIDA SetValue: " + channelName + arguments + " => " + aidaSetterType + ":" + (channelSetterType == null ? "" : channelSetterType));
        } else {
            isGetterRequest = TRUE;
            System.out.println("AIDA Request : " + channelName + arguments + " => " + aidaGetterType + ":" + (channelGetterType == null ? "" : channelGetterType));
        }

        // Call entry point based on return type
        if (!isGetterRequest) {
            if (aidaSetterType.equals(VOID)) {
                this.aidaChannelProvider.setValue(channelName, arguments);
                return NT_SCALAR_EMPTY_STRUCTURE;
            } else if ( aidaSetterType.equals(TABLE) ) {
                List<List<Object>> returnValue = this.aidaChannelProvider.setValueWithResponse(channelName, arguments);
                return asNtTable(returnValue, channelSetterConfig);
            } else {
                throw new UnsupportedChannelException("Setters can only be VOID or TABLE but found: " + aidaSetterType.name());
            }
        } else {
            // Otherwise this is a regular get request
            if (channelGetterType == null) {
                throw new UnsupportedChannelException("Could not find return type for this channel.  Perhaps the channels.yml file contains an invalid pattern: " + channelName);
            }

            if (channelGetterType.equals(scalar)) {
                Object returnValue = this.aidaChannelProvider.requestScalar(channelName, aidaGetterType, arguments);
                return asScalar(returnValue, channelGetterConfig);
            } else if (channelGetterType.equals(scalarArray)) {
                List<?> returnValue = this.aidaChannelProvider.requestScalarArray(channelName, aidaGetterType, arguments);
                return asScalarArray(returnValue, channelGetterConfig);
            } else {
                List<List<Object>> returnValue = this.aidaChannelProvider.requestTable(channelName, arguments);
                return asNtTable(returnValue, channelGetterConfig);
            }
        }
    }

    /**
     * Get the arguments for the specified request
     *
     * @param pvUriQuery the specified request
     * @return the list of aida channel arguments
     * @throws RPCRequestException if there is a problem reading the arguments
     */
    private List<AidaArgument> getArguments(PVStructure pvUriQuery) throws RPCRequestException {
        List<AidaArgument> arguments = new ArrayList<AidaArgument>();
        if (pvUriQuery != null) {
            PVField[] pvFields = pvUriQuery.getPVFields();
            for (PVField field : pvFields) {
                String name = field.getFieldName();
                if (name == null) {
                    throw new RPCRequestException(Status.StatusType.ERROR, "Invalid argument name: <blank>");
                }
                String value = pvUriQuery.getStringField(name).get();
                if (value == null) {
                    throw new RPCRequestException(Status.StatusType.ERROR, "Invalid argument value: <blank>");
                }
                arguments.add(new AidaArgument(name, value));
            }
        }
        return arguments;
    }
}
