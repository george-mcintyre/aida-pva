package edu.stanford.slac.aida.provider.sample.standard;

import edu.stanford.slac.aida.provider.sample.impl.*;
import edu.stanford.slac.aida.provider.sample.type.DpChannel;
import edu.stanford.slac.aida.lib.PVTopStructure;
import org.epics.pvaccess.client.*;
import org.epics.pvdata.monitor.Monitor;
import org.epics.pvdata.monitor.MonitorRequester;
import org.epics.pvdata.pv.*;

import java.util.ArrayList;
import java.util.concurrent.atomic.AtomicBoolean;

public abstract class StandardDpChannel implements DpChannel {
    private final AidaChannelProvider dpServer;
    private final String channelName;
    private final ChannelRequester channelRequester;
    private final PVTopStructure pvTopStructure;

    private final ArrayList<ChannelRequest> channelRequests = new ArrayList<ChannelRequest>();

    private volatile ConnectionState connectionState = ConnectionState.NEVER_CONNECTED;

    private final AtomicBoolean destroyed = new AtomicBoolean(false);

    public StandardDpChannel(AidaChannelProvider dpServer, String channelName, ChannelRequester channelRequester, PVTopStructure pvTopStructure) {
        this.dpServer = dpServer;
        this.channelName = channelName;
        this.channelRequester = channelRequester;

        this.pvTopStructure = pvTopStructure;

        setConnectionState(ConnectionState.CONNECTED);
    }

    public final void registerRequest(ChannelRequest request) {
        synchronized (channelRequests) {
            channelRequests.add(request);
        }
    }

    public final void unregisterRequest(ChannelRequest request) {
        synchronized (channelRequests) {
            channelRequests.remove(request);
        }
    }

    public final ChannelProvider getProvider() {
        return dpServer;
    }

    /**
     * Get the requesters name.  Override only if you don't kike the way it is presented by default
     *
     * @return the name of the requester
     */
    public String getRequesterName() {
        return channelRequester.getRequesterName();
    }

    /**
     * Method used to output an error message on this channel.
     * Override to use a different format or a different target
     *
     * @param message     the message to output
     * @param messageType the message type
     */
    public void message(String message, MessageType messageType) {
        System.err.println("[" + messageType + "] " + message);
    }

    /**
     * String representing the name of the remote host name.
     * Defaults to "local"
     *
     * @return the remote host
     */
    public String getRemoteAddress() {
        return "local";
    }

    /**
     * Get the connection state
     *
     * @return connection state
     */
    public ConnectionState getConnectionState() {
        return connectionState;
    }

    /**
     * Set the state of this connection.  If overriding you must call this
     * method to update the internal state
     *
     * @param state the state to set
     */
    public void setConnectionState(ConnectionState state) {
        this.connectionState = state;
        channelRequester.channelStateChange(this, state);
    }

    /**
     * Is connected
     *
     * @return true if connected
     */
    public final boolean isConnected() {
        return getConnectionState() == ConnectionState.CONNECTED;
    }

    /**
     * Returns the channel's requester
     *
     * @return channel requester
     */
    public final ChannelRequester getChannelRequester() {
        return channelRequester;
    }

    /**
     * Get a Field which describes the subField.
     * GetFieldRequester.getDone is called after both client and server have processed the getField request.
     * This is for clients that want to introspect a PVRecord via channel access.
     *
     * @param requester The requester.
     * @param subField  The name of the subField.
     *                  If this is null or an empty string the returned Field is for the entire record.
     */
    public final void getField(GetFieldRequester requester, String subField) {
        if (requester == null)
            throw new IllegalArgumentException("requester not set in call to getField");

        if (destroyed.get()) {
            requester.getDone(AidaChannelProvider.destroyedStatus, null);
            return;
        }

        Field field;
        if (subField == null || subField.trim().length() == 0)
            field = pvTopStructure.getPVStructure().getStructure();
        else
            field = pvTopStructure.getPVStructure().getStructure().getField(subField);

        if (field != null)
            requester.getDone(AidaChannelProvider.okStatus, field);
        else
            requester.getDone(AidaChannelProvider.fieldDoesNotExistStatus, null);
    }

    /**
     * Destroy the channel. It will not honor any further requests.
     * Override to provide further processing when channel is being destroyed
     * but call this method to update internal state
     */
    public void destroy() {
        if (!destroyed.getAndSet(true)) {
            destroyRequests();

            setConnectionState(ConnectionState.DISCONNECTED);
            setConnectionState(ConnectionState.DESTROYED);
        }
    }

    /**
     * Get the channel name.  Override to change the channel name
     *
     * @return channel name
     */
    public String getChannelName() {
        return channelName;
    }

    /**
     * Get Access Rights.  Default to read/write access override if necessary
     *
     * @param pvField the field
     * @return the access rights
     */
    public AccessRights getAccessRights(PVField pvField) {
        return AccessRights.readWrite;
    }

    /**
     * Create a channel process
     *
     * @param requester requester
     * @param request   request
     * @return channel process
     */
    public ChannelProcess createChannelProcess(ChannelProcessRequester requester, PVStructure request) {
        if (requester == null)
            throw new IllegalArgumentException("requester not set in call to createChannelProcess");

        if (destroyed.get()) {
            requester.channelProcessConnect(AidaChannelProvider.destroyedStatus, null);
            return null;
        }

        return new DpChannelProcessImpl(this, pvTopStructure, requester, request);
    }

    /**
     * Create get channel
     *
     * @param requester requester
     * @param request request
     * @return get channel
     */
    public ChannelGet createChannelGet(ChannelGetRequester requester, PVStructure request) {
        parameterCheck(request, requester, "createChannelGet");

        if (destroyed.get()) {
            requester.channelGetConnect(AidaChannelProvider.destroyedStatus, null, null);
            return null;
        }

        return new DpChannelGetImpl(this, pvTopStructure, requester, request);
    }

    public ChannelPut createChannelPut(ChannelPutRequester requester, PVStructure request) {
        parameterCheck(request, requester, "createChannelPut");

        if (destroyed.get()) {
            requester.channelPutConnect(AidaChannelProvider.destroyedStatus, null, null);
            return null;
        }

        return new DpChannelPutImpl(this, pvTopStructure, requester, request);
    }

    public ChannelPutGet createChannelPutGet(ChannelPutGetRequester requester, PVStructure request) {
        parameterCheck(request, requester, "ChannelPutGet");

        if (destroyed.get()) {
            requester.channelPutGetConnect(AidaChannelProvider.destroyedStatus, null, null, null);
            return null;
        }

        return new DpChannelPutGetImpl(this, pvTopStructure, requester, request);
    }

    public ChannelRPC createChannelRPC(ChannelRPCRequester requester, PVStructure request) {
        parameterCheck(requester, "createChannelRPC");

        if (destroyed.get()) {
            requester.channelRPCConnect(AidaChannelProvider.destroyedStatus, null);
            return null;
        }

        return new DpChannelRPCImpl(this, pvTopStructure, requester, request);
    }

    public Monitor createMonitor(MonitorRequester requester, PVStructure request) {
        parameterCheck(request, requester, "createMonitor");

        if (destroyed.get()) {
            requester.monitorConnect(AidaChannelProvider.destroyedStatus, null, null);
            return null;
        }

        return new DpChannelMonitorImpl(this, pvTopStructure, requester, request);
    }

    public ChannelArray createChannelArray( ChannelArrayRequester requester, PVStructure request) {
        parameterCheck(request, requester, "createChannelArray");

        if (destroyed.get()) {
            requester.channelArrayConnect(AidaChannelProvider.destroyedStatus, null, null);
            return null;
        }

        PVField[] pvFields = request.getPVFields();
        if (pvFields.length != 1) {
            requester.channelArrayConnect(AidaChannelProvider.illegalRequestStatus, null, null);
            return null;
        }
        PVField pvField = pvFields[0];
        StringBuilder fieldName = new StringBuilder();
        while (pvField != null) {
            String name = pvField.getFieldName();
            if (name != null && name.length() > 0) {
                if (fieldName.length() > 0) fieldName.append('.');
                fieldName.append(name);
            }
            PVStructure pvs = (PVStructure) pvField;
            pvFields = pvs.getPVFields();
            if (pvFields.length != 1) break;
            pvField = pvFields[0];
        }
        if (fieldName.toString().startsWith("field."))
            fieldName = new StringBuilder(fieldName.substring(6));
        pvField = pvTopStructure.getPVStructure().getSubField(fieldName.toString());
        if (pvField == null) {
            requester.channelArrayConnect(AidaChannelProvider.subFieldDoesNotExistStatus, null, null);
            return null;
        }

        if (pvField.getField().getType() == Type.structureArray) {
            throw new RuntimeException("array of structures (structure array) not implemented");
        }

        if (pvField.getField().getType() != Type.scalarArray) {
            requester.channelArrayConnect(AidaChannelProvider.subFieldNotArrayStatus, null, null);
            return null;
        }
        PVScalarArray pvArray = (PVScalarArray) pvField;
        return new DpChannelScalarArrayImpl(this, pvTopStructure, requester, pvArray);
    }

    private void parameterCheck(PVStructure request, Requester requester, String methodName) {
        parameterCheck(requester, methodName);

        if (request == null)
            throw new IllegalArgumentException("request not set in call to " + methodName);
    }

    private void parameterCheck(Requester requester, String methodName) {
        if (requester == null)
            throw new IllegalArgumentException("requester not set in call to " + methodName);
    }

    /**
     * Destroy all requests in this channel
     */
    private void destroyRequests() {
        synchronized (channelRequests) {
            while (!channelRequests.isEmpty())
                channelRequests.get(channelRequests.size() - 1).destroy();
        }
    }
}
