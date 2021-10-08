package edu.stanford.slac.aida.lib.model;


import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.NonNull;

import java.util.HashSet;
import java.util.Set;

/**
 * The config group class controls how the provider responds to a channel request.
 * There is a channel getterConfig for the get requests (getter) and a setterConfig for the set requests (setter)
 * All setter requests are differentiated by containing a VALUE argument.
 * A request that contains the VALUE parameter is sent to the setter, otherwise it is sent to the getter.
 * <p>
 * The list of channels are the channels that will use the specified config
 */
@Data
@NoArgsConstructor
public class AidaConfigGroup {
    private String name;
    private AidaChannelConfig getterConfig;
    private AidaChannelConfig setterConfig;
    private @NonNull Set<String> channels = new HashSet<String>();

    @Override
    public String toString() {
        return "AidaConfigGroup{" +
                "name='" + name + '\'' +
                ", getterConfig=" + getterConfig +
                ", setterConfig=" + setterConfig +
                ((channels.size() > 100) ? ", channels=[large set omitted!]" : ", channels=" + channels) +
                '}';
    }
}
