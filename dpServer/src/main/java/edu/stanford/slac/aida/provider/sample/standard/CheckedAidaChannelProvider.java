package edu.stanford.slac.aida.provider.sample.standard;

import org.epics.pvaccess.client.ChannelFindRequester;
import org.epics.pvaccess.client.ChannelListRequester;
import org.epics.pvaccess.client.ChannelProvider;
import org.epics.pvdata.pv.Requester;

public class
CheckedAidaChannelProvider {
    protected void parameterCheck(String channelName, Requester requester, String methodName, Short priority) {
        parameterCheck(channelName, requester, methodName);

        if (priority < ChannelProvider.PRIORITY_MIN || priority > ChannelProvider.PRIORITY_MAX)
            throw new IllegalArgumentException("priority out of range");
    }

    protected void parameterCheck(String channelName, Requester requester, String methodName) {
        if (channelName == null)
            throw new IllegalArgumentException("channel name not set in call to " + methodName);

        parameterCheck(requester, methodName);
    }

    protected void parameterCheck(String channelName, ChannelFindRequester requester, String methodName) {
        if (channelName == null)
            throw new IllegalArgumentException("channel name not set in call to " + methodName);

        parameterCheck(requester, methodName);
    }

    protected void parameterCheck(Requester requester, String methodName) {
        if (requester == null)
            throw new IllegalArgumentException("requester not set in call to " + methodName);
    }

    protected void parameterCheck(ChannelFindRequester requester, String methodName) {
        if (requester == null)
            throw new IllegalArgumentException("requester not set in call to " + methodName);
    }

    protected void parameterCheck(ChannelListRequester requester, String methodName) {
        if (requester == null)
            throw new IllegalArgumentException("requester not set in call to " + methodName);
    }
}
