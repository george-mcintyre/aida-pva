package edu.stanford.slac.aida.lib.model;

import edu.stanford.slac.aida.lib.ChannelProvider;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.NonNull;
import org.epics.pvaccess.util.WildcardMatcher;

import java.util.*;

@Data
@NoArgsConstructor
public class AidaProvider {
    private @NonNull Long id;
    private @NonNull String name;
    private String description;
    private List<String> arguments;
    private @NonNull List<AidaConfigGroup> configurations = new ArrayList<AidaConfigGroup>();
    private ChannelProvider channelProvider;

    /**
     * A map of channel to channel configuration so that configuration can be looked up by channel name
     */
    private final Map<String, AidaChannel> channelMap = new HashMap<String, AidaChannel>();

    /**
     * Get the set of supported channels.  AIDA providers
     * support both AIDA-PVA and legacy AIDA names for their channels
     *
     * @return the list of supported channel names
     */
    public Set<String> getChannelNames() {
        if (this.channelMap.isEmpty()) {
            cacheChannelNamesAndLoadConfig();
        }
        return this.channelMap.keySet();
    }

    /**
     * Get the channel based on the channel name
     * Make sure that all config is fully specified
     *
     * @param channelName the channel name
     * @return the fully configured AIDA channel
     */
    public AidaChannel getAidaChannel(String channelName) {
        if (this.channelMap.isEmpty()) {
            cacheChannelNamesAndLoadConfig();
        }

        // Look for an exact match or a wild card match if that fails
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
     * Set the channel names based on set of instance attribute pairs
     */
    private void cacheChannelNamesAndLoadConfig() {
        synchronized (this.channelMap) {
            for (AidaConfigGroup configuration : getConfigurations()) {
                AidaChannelConfig getterConfig = configuration.getGetterConfig();
                AidaChannelConfig setterConfig = configuration.getSetterConfig();

                // Set default labels in the config
                if (getterConfig != null) {
                    setDefaultLabels(getterConfig.getFields());
                }
                if (setterConfig != null) {
                    setDefaultLabels(setterConfig.getFields());
                }

                // Set give all channels that are in this configuration group the same config
                for (String channelName : configuration.getChannels()) {
                    AidaChannel aidaChannel = new AidaChannel(channelName, configuration.getGetterConfig(), configuration.getSetterConfig());
                    this.channelMap.put(channelName, aidaChannel);

                    // Add legacy style channel name as well as new style, regardless as to how it is specified in the channels file
                    int indexOfLastSeparator = channelName.lastIndexOf(":");
                    int indexOfLastLegacySeparator = channelName.lastIndexOf("//");

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

    /**
     * Set the label to be the name if it is not specified
     *
     * @param fields the fields to update
     */
    private void setDefaultLabels(List<AidaField> fields) {
        if (fields != null) {
            for (AidaField field : fields) {
                if (field.getLabel() == null) {
                    field.setLabel(field.getName());
                }
            }
        }
    }

}
