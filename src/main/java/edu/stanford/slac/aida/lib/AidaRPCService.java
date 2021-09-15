package edu.stanford.slac.aida.lib;

import edu.stanford.slac.aida.exception.UnableToGetDataException;
import edu.stanford.slac.aida.exception.UnableToSetDataException;
import edu.stanford.slac.aida.exception.UnsupportedChannelException;
import edu.stanford.slac.aida.exception.UnsupportedChannelTypeException;
import edu.stanford.slac.aida.lib.model.AidaArgument;
import edu.stanford.slac.aida.lib.model.AidaChannelConfig;
import edu.stanford.slac.aida.lib.model.AidaType;
import org.epics.nt.NTURI;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvaccess.server.rpc.RPCService;
import org.epics.pvdata.pv.*;

import java.util.ArrayList;
import java.util.List;

import static edu.stanford.slac.aida.lib.model.AidaProvider.getAidaName;
import static edu.stanford.slac.aida.lib.model.AidaType.*;
import static edu.stanford.slac.aida.lib.util.AidaPVHelper.*;
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
     * @throws RPCRequestException         if any error occurs formulating the request or decoding the response
     * @throws UnableToGetDataException    when server fails to retrieve data
     * @throws UnableToSetDataException    when server fails to set data
     * @throws UnsupportedChannelException when server does not yet support the specified channel.
     *                                     Usually caused when channel matches a pattern specified in the channels.yml file
     *                                     but is not yet supported in the service implementation
     */
    public PVStructure request(PVStructure pvUri) throws RPCRequestException, UnableToGetDataException, UnsupportedChannelException, UnableToSetDataException {
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

        return request(channelName, arguments);
    }

    /**
     * Make request to the specified channel with the uri and arguments specified
     * and return the NT_TABLE of results.
     *
     * @param channelName channel name
     * @param arguments   arguments if any
     * @return the structure containing the results.
     * @throws UnableToGetDataException    when server fails to retrieve data
     * @throws UnableToSetDataException    when server fails to set data
     * @throws UnsupportedChannelException when server does not yet support the specified channel.
     *                                     Usually caused when channel matches a pattern specified in the channels.yml file
     *                                     but is not yet supported in the service implementation
     */
    private PVStructure request(String channelName, List<AidaArgument> arguments) throws UnableToGetDataException, UnsupportedChannelException, UnableToSetDataException {
        AidaChannelConfig channelConfig = aidaChannelProvider.getChannelConfig(channelName);

        if (channelConfig == null) {
            throw new UnsupportedChannelException("Could not find configuration for this channel.  Perhaps the channels.yml file contains an invalid pattern: " + channelName);
        }

        AidaType aidaType = channelConfig.getType();

        // Get special arguments type and value
        String typeArgument = null;
        AidaArgument valueArgument = null;
        for (AidaArgument argument : arguments) {
            if (argument.getName().equalsIgnoreCase("AType")) {
                typeArgument = argument.getValue().toUpperCase();
            }
            if (argument.getName().equalsIgnoreCase("AValue")) {
                valueArgument = argument;
            }
        }

        // If you've specified a type the override the config type with the specified one
        if (typeArgument != null) {
            try {
                AidaType specifiedAidaType = AidaType.valueOf(typeArgument);

                // Check that it matches the type class from the channels file
                if (aidaType.equals(ANY) ||
                        (!aidaType.equals(SCALAR) && !aidaType.equals(SCALAR_ARRAY)) ||
                        specifiedAidaType.metaType().equals(aidaType.metaType())) {
                    aidaType = specifiedAidaType;
                } else {
                    throw new UnsupportedChannelTypeException("The type specified by the 'AType' parameter must be a " + aidaType + ", but you specified " + specifiedAidaType);
                }
            } catch (IllegalArgumentException e) {
                throw new UnsupportedChannelTypeException("The type specified by the 'AType' parameter is not a recognised AIDA type" + typeArgument);
            }
        } else {
            // If no class is specified then use a default for each type class
            switch (aidaType) {
                case SCALAR:
                    aidaType = INTEGER;
                    break;
                case SCALAR_ARRAY:
                    aidaType = INTEGER_ARRAY;
                    break;
                case ANY:
                    aidaType = TABLE;
            }
        }

        Type channelType = typeOf(aidaType);
        if (aidaType.equals(NONE) || valueArgument != null) {
            System.out.println("AIDA SetValue: " + channelName + arguments + " => " + aidaType + ":" + (channelType == null ? "" : channelType));
        } else {
            System.out.println("AIDA Request : " + channelName + arguments + " => " + aidaType + ":" + (channelType == null ? "" : channelType));
        }

        // Call entry point based on return type
        if (aidaType.equals(NONE)) {
            // If type is NONE then call setValue
            this.aidaChannelProvider.setValue(channelName, arguments);
            return NT_SCALAR_EMPTY_STRUCTURE;
        } else if (valueArgument != null) {
            // If a type is specified but type is not NONE then call setValue returning a table of results
            List<List<Object>> returnValue = this.aidaChannelProvider.setValueWithResponse(channelName, arguments);
            return asNtTable(returnValue, channelConfig);
        } else {
            // Otherwise this is a regular get request
            if (channelType == null) {
                throw new UnsupportedChannelException("Could not find return type for this channel.  Perhaps the channels.yml file contains an invalid pattern: " + channelName);
            }

            if (channelType.equals(scalar)) {
                Object returnValue = this.aidaChannelProvider.requestScalar(channelName, aidaType, arguments);
                return asScalar(returnValue, channelConfig);
            } else if (channelType.equals(scalarArray)) {
                List<?> returnValue = this.aidaChannelProvider.requestScalarArray(channelName, aidaType, arguments);
                return asScalarArray(returnValue, channelConfig);
            } else {
                List<List<Object>> returnValue = this.aidaChannelProvider.requestTable(channelName, arguments);
                return asNtTable(returnValue, channelConfig);
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
