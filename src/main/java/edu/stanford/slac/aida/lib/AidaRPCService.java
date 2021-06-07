package edu.stanford.slac.aida.lib;

import edu.stanford.slac.aida.lib.model.AidaArgument;
import edu.stanford.slac.aida.lib.model.AidaChannelConfig;
import edu.stanford.slac.aida.lib.model.AidaType;
import org.epics.nt.NTURI;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvaccess.server.rpc.RPCService;
import org.epics.pvdata.pv.*;

import java.util.ArrayList;
import java.util.List;

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
     * @throws RPCRequestException if any error occurs
     */
    public PVStructure request(PVStructure pvUri) throws RPCRequestException {
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
     */
    private PVStructure request(String channelName, List<AidaArgument> arguments) {
        AidaChannelConfig channelConfig = aidaChannelProvider.getChannelConfig(channelName);
        AidaType aidaType = channelConfig.getType();
        Type channelType = typeOf(aidaType);
        if (channelType == null) {
            // TODO log exception
            return null;
        }

        if (channelType.equals(scalar)) {
            return asScalar(
                    this.aidaChannelProvider.requestScalar(channelName, aidaType, arguments),
                    this.aidaChannelProvider.getChannelConfig(channelName));
        } else if (channelType.equals(scalarArray)) {
            return asScalarArray(
                    this.aidaChannelProvider.requestScalarArray(channelName, aidaType, arguments),
                    this.aidaChannelProvider.getChannelConfig(channelName));
        } else {
            return asNtTable(
                    this.aidaChannelProvider.requestTable(channelName, arguments),
                    this.aidaChannelProvider.getChannelConfig(channelName));
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
