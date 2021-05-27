package edu.stanford.slac.aida.provider.sample.type;

import org.epics.pvaccess.client.Channel;
import org.epics.pvaccess.client.ChannelRequest;

public interface DpChannel extends Channel {
    void registerRequest(ChannelRequest request);
    void unregisterRequest(ChannelRequest request);
    void setConnectionState(ConnectionState state);
}
