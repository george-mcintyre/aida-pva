/*
 * @file
 * This class captures the metadata associated with an AIDA-PVA Channel Provider.
 */
package edu.stanford.slac.aida.lib.model;

import edu.stanford.slac.aida.lib.ChannelProvider;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.NonNull;
import org.epics.pvaccess.util.WildcardMatcher;

import java.util.*;

/**
 * This class encapsulates an {@link AidaProvider}.
 * <p>
 * It is used configure how an AIDA-PVA will operate.
 * <p>
 * It has an {@link AidaProvider#id} to identify the Provider.
 * It has an {@link AidaProvider#name} to give it a name.
 * It has a {@link AidaProvider#description} displayed during startup alongside the {@link AidaProvider#id} and {@link AidaProvider#name}.
 * The {@link AidaProvider#configurations} define the different {@link AidaConfigGroup} we define for requests to this channel.
 * Finally, the {@link AidaProvider#channelProvider} defines the actual class that will implement the functionality to service this channel.
 * @note
 * It uses the `@Data` annotation to provide all the getters and setters,
 * a constructor with all required arguments,
 * and an equals(), hashcode() and toString()  method.
 * @note
 * It also uses the `@NoArgsConstructor` annotation to provide a constructor
 * with no arguments.
 */
@Data
@NoArgsConstructor
public class AidaProvider {
    /**
     * The {@link AidaProvider#id} identifies the Provider.  By convention, we use the `AIDA_SERVICES.ID` from the
     * AIDA Oracle Database.
     */
    private @NonNull Long id;

    /**
     * The {@link AidaProvider#name} identifies the Provider by name.  By convention, we use the `AIDA_SERVICES.NAME` from the
     * AIDA Oracle Database.
     */
    private @NonNull String name;

    /**
     * The {@link AidaProvider#description} describes what the Provider does.  By convention, we use the `AIDA_SERVICES.DESCRIPTION` from the
     * AIDA Oracle Database.
     */
    private String description;

    /**
     * The {@link AidaProvider#configurations} lists the different {@link AidaConfigGroup} we define for requests to this channel.
     * The groups are defined with reference to the appropriate documentation for the Provider. https://www.slac.stanford.edu/grp/cd/soft/aida
     */
    private @NonNull List<AidaConfigGroup> configurations = new ArrayList<AidaConfigGroup>();

    /**
     * The {@link AidaProvider#channelProvider} defines the actual class that will implement the functionality to service this channel
     */
    private ChannelProvider channelProvider;

    /**
     * A map of Channel Name => {@link AidaChannel} channel configuration, so that configuration can be looked up by Channel Name
     */
    private final Map<String, AidaChannel> channelMap = new HashMap<String, AidaChannel>();

    /**
     * Get the set of supported Channel Names.  AIDA-PVA Providers
     * support both AIDA-PVA and legacy AIDA names for their channels.  The list
     * returned will contain both.
     *
     * @return the list of supported channel names
     */
    public Set<String> getChannelNames() {
        loadChannelMapIfNotLoaded();
        return this.channelMap.keySet();
    }

    /**
     * Get the channel based on the channel name.
     * <p>
     * First, make sure that all config is loaded.
     * <p>
     * Channels are matched literally but also by using the {@link WildcardMatcher#match}
     * to see if any of the patterns specified as supported by this channel correspond to the
     * channel name specified in the request
     *
     * @param channelName the channel name from the request
     * @return the {@link AidaChannel} object that matches the given channel name
     */
    public AidaChannel getAidaChannel(String channelName) {
        loadChannelMapIfNotLoaded();

        // Look for an exact match or a {@link WildcardMatcher#match} if that fails
        AidaChannel aidaChannel = this.channelMap.get(channelName);
        if (aidaChannel == null) {
            for (Map.Entry<String, AidaChannel> entry : this.channelMap.entrySet()) {
                if (WildcardMatcher.match(entry.getKey(), channelName)) {
                    aidaChannel = entry.getValue();
                    break;
                }
            }
        }

        return aidaChannel;
    }

    /**
     * For speed the list of channels and their configurations are cached in a memory resident
     * {@link HashMap}.  This method is used to prime that {@link HashMap} from the list of
     * {@link AidaProvider#configurations} that have been loaded in from the Channel Provider's
     * CHANNELS.YML file.
     */
    private void loadChannelMapIfNotLoaded() {
        /// Make sure that no other thread does this at the same time
        synchronized (this.channelMap) {
            // Only load the map if it is empty
            if (this.channelMap.isEmpty()) {
                // Get all the configuration groups
                for (AidaConfigGroup configuration : getConfigurations()) {
                    AidaChannelOperationConfig getterConfig = configuration.getGetterConfig();
                    AidaChannelOperationConfig setterConfig = configuration.getSetterConfig();

                    // In the getter config, set the labels to be the field names, if the labels are not specified
                    if (getterConfig != null) {
                        setDefaultLabels(getterConfig.getFields());
                    }
                    // In the setter config, set the labels to be the field names, if the labels are not specified
                    if (setterConfig != null) {
                        setDefaultLabels(setterConfig.getFields());
                    }

                    // Set give all channels that are in this configuration group the same config
                    for (String channelName : configuration.getChannels()) {
                        AidaChannel aidaChannel = new AidaChannel(channelName, configuration.getGetterConfig(), configuration.getSetterConfig());
                        this.channelMap.put(channelName, aidaChannel);

                        // Get index of last separator using new and legacy format channel names
                        int indexOfLastSeparator = channelName.lastIndexOf(":");
                        int indexOfLastLegacySeparator = channelName.lastIndexOf("//");

                        // Add legacy style channel name as well as new style, regardless as to how it is specified in the channels file

                        // If specified using legacy separator then add entry with the new style as well
                        if (indexOfLastLegacySeparator != -1) {
                            this.channelMap.put(channelName.substring(0, indexOfLastLegacySeparator) + ":" + channelName.substring(indexOfLastLegacySeparator + 2), aidaChannel);

                            // If specified with new naming style then add an entry with the legacy separator for backwards compatibility
                        } else if (indexOfLastSeparator != -1) {
                            String legacyStyle = null;

                            String[] parts = channelName.split(":");
                            int nParts = parts.length;
                            if (nParts == 3) {
                                // If uri has three parts like for MAGNET then need to add legacy separator after second not first
                                legacyStyle = parts[0] + "//" + parts[1] + ":" + parts[2];
                            } else {
                                // Otherwise, add before last part
                                legacyStyle = channelName.substring(0, indexOfLastSeparator) + "//" + channelName.substring(indexOfLastSeparator + 1);
                            }

                            this.channelMap.put(legacyStyle, aidaChannel);
                        }
                    }
                }
            }
        }
    }

    /**
     * Set the labels to be the field names, if the labels are not specified
     *
     * @param fields the fields to update
     */
    private void setDefaultLabels(List<AidaField> fields) {
        // If any fields are given
        if (fields != null) {
            // For each of the given fields
            for (AidaField field : fields) {
                // If the label is not specified
                if (field.getLabel() == null) {
                    // Use the name as the label
                    field.setLabel(field.getName());
                }
            }
        }
    }

}
