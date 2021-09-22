package edu.stanford.slac.aida.lib.model;

import edu.stanford.slac.aida.lib.ChannelProvider;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.NonNull;
import org.epics.pvaccess.util.WildcardMatcher;

import java.util.*;

import static edu.stanford.slac.aida.lib.model.AidaTableLayout.COLUMN_MAJOR;
import static edu.stanford.slac.aida.lib.model.AidaType.STRING;
import static java.lang.Boolean.FALSE;
import static java.lang.Boolean.TRUE;

@Data
@NoArgsConstructor
public class AidaProvider {
    private @NonNull Long id;
    private @NonNull String name;
    private String description;
    private @NonNull Set<AidaChannel> channels = new HashSet<AidaChannel>();
    private @NonNull AidaChannelConfig getterConfig;
    private @NonNull AidaChannelConfig setterConfig;
    private ChannelProvider channelProvider;

    private final Map<String, AidaChannel> channelMap = new HashMap<String, AidaChannel>();

    /**
     * Get the set of supported channels.  Aida providers
     * support both EPICs and legacy AIDA names for their channels
     *
     * @return the list of supported channel names
     */
    public Set<String> getChannelNames() {
        if (this.channelMap.isEmpty()) {
            setChannelNames();
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
            setChannelNames();
        }

        // Look for an exact match or a wild card match if that fails
        AidaChannel aidaChannel = this.channelMap.get(channelName);
        if (aidaChannel == null) {
            for (Map.Entry<String, AidaChannel> entry : this.channelMap.entrySet()) {
                if (WildcardMatcher.match(getAidaName(entry.getKey()), channelName)) {
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
    private void setChannelNames() {
        synchronized (this.channelMap) {
            for (AidaChannel aidaChannel : getChannels()) {
                String channel = aidaChannel.getChannel();

                copyConfig(aidaChannel);

                this.channelMap.put(channel, aidaChannel);

                // Add legacy style channel name as well as new style, regardless as to how it is specified in the channels file
                int indexOfLastSeparator = channel.lastIndexOf(":");
                int indexOfLastLegacySeparator = channel.lastIndexOf("//");

                // If specified using legacy separator then add entry with the new style as well
                if (indexOfLastLegacySeparator != -1) {
                    this.channelMap.put(channel.substring(0, indexOfLastLegacySeparator) + ":" + channel.substring(indexOfLastLegacySeparator + 2), aidaChannel);

                    // If specified with new naming style then add an entry with the legacy separator for backwards compatibility
                } else if (indexOfLastSeparator != -1) {
                    this.channelMap.put(channel.substring(0, indexOfLastSeparator) + "//" + channel.substring(indexOfLastSeparator + 1), aidaChannel);
                }
            }
        }
    }

    /**
     * Merges the given default config with the given overrides to produce a new merged config
     *
     * @param defaultConfig default config
     * @param overrides     config containing overrides
     * @return the merged config
     */
    private AidaChannelConfig mergeConfig(AidaChannelConfig defaultConfig, AidaChannelConfig overrides) {
        AidaChannelConfig channelConfig = new AidaChannelConfig();

        // If nothing to override then return the default
        if (overrides == null) {
            return defaultConfig;
        }

        // Partially copy when not specified for channel
        AidaType type = ((overrides.getType() == null) ? defaultConfig : overrides).getType();
        if (type == null) {
            type = STRING;
        }
        channelConfig.setType(type.toString());

        AidaTableLayout layout = ((overrides.getLayout() == null) ? defaultConfig : overrides).getLayout();
        if (layout == null) {
            layout = COLUMN_MAJOR;
        }
        channelConfig.setLayout(layout.toString());

        channelConfig.setDescription(((overrides.getDescription() == null || overrides.getDescription().length() == 0) ? defaultConfig : overrides).getDescription());
        channelConfig.setFields(((overrides.getFields() == null || overrides.getFields().isEmpty()) ? defaultConfig : overrides).getFields());

        return channelConfig;
    }

    /**
     * Add default channel configurations if not specified directly with the channel
     * Bot the getter and the setter configs are copied
     *
     * @param aidaChannel the channel to add the default configurations to
     */
    private void copyConfig(AidaChannel aidaChannel) {
        AidaChannelConfig providerConfig, channelConfig, defaultConfig, mergedConfig;

        // GETTER CONFIG
        // Highest priority from provider itself
        providerConfig = this.channelProvider.getNativeChannelConfig(aidaChannel.getChannel(), TRUE);
        // Next from yaml channel
        channelConfig = aidaChannel.getGetterConfig();
        // Finally from yaml global section
        defaultConfig = getGetterConfig();
        // Merge all together with these priorities
        mergedConfig = mergeConfig(mergeConfig(defaultConfig, channelConfig), providerConfig);
        aidaChannel.setGetterConfig(mergedConfig);
        // Set default labels
        setDefaultLabels(mergedConfig.getFields());

        // SETTER CONFIG
        // Highest priority from provider itself
        providerConfig = this.channelProvider.getNativeChannelConfig(aidaChannel.getChannel(), FALSE);
        // Next from yaml channel
        channelConfig = aidaChannel.getGetterConfig();
        // Finally from yaml global section
        defaultConfig = getGetterConfig();
        // Merge all together with these priorities
        mergedConfig = mergeConfig(mergeConfig(defaultConfig, channelConfig), providerConfig);
        aidaChannel.setGetterConfig(mergedConfig);
        // Set default labels
        setDefaultLabels(mergedConfig.getFields());
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

    /**
     * Get legacy aida name
     *
     * @param channelName the channel name
     * @return the legacy aida name
     */
    public static String getAidaName(String channelName) {
        if (channelName.lastIndexOf("//") != -1) {
            return channelName;
        }

        int start = channelName.lastIndexOf(":");
        // if channel name does not contain any colons then return unchanged
        if (start == -1) {
            return channelName;
        }

        return (channelName.substring(0, start) + "//" + channelName.substring(start + 1));
    }

    @Override
    public String toString() {
        return "AidaProvider{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", description='" + description + '\'' +
                ", getterConfig=" + getterConfig +
                ", setterConfig=" + setterConfig +
                ((channels.size() > 100) ? ", channels=[large set omitted!]" : ", channels=" + channels) +
                '}';
    }
}
