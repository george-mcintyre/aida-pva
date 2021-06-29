package edu.stanford.slac.aida.lib.model;

import edu.stanford.slac.aida.lib.ChannelProvider;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.NonNull;

import java.util.*;

import static edu.stanford.slac.aida.lib.model.AidaTableLayout.COLUMN_MAJOR;
import static edu.stanford.slac.aida.lib.model.AidaType.STRING;

@Data
@NoArgsConstructor
public class AidaProvider {
    private @NonNull Long id;
    private @NonNull String name;
    private String description;
    private @NonNull Set<AidaChannel> channels = new HashSet<AidaChannel>();
    private @NonNull AidaChannelConfig config;
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
        return this.channelMap.get(channelName);
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

                // Add legacy style channel name
                int indexOfLastSeparator = channel.lastIndexOf(":");
                if (indexOfLastSeparator != -1) {
                    this.channelMap.put(channel.substring(0, indexOfLastSeparator) + "//" + channel.substring(indexOfLastSeparator + 1), aidaChannel);
                }
            }
        }
    }

    private AidaChannelConfig mergeConfig(AidaChannelConfig defaultConfig, AidaChannelConfig overrides) {
        AidaChannelConfig channelConfig = new AidaChannelConfig();

        // If nothing to override then return the default
        if (overrides == null) {
            return defaultConfig;
        }

        // Partially copy when not specified for channel
        AidaType type = ((overrides.getType() == null) ? defaultConfig : overrides).getType();
        if ( type == null ) {
            type = STRING;
        }
        channelConfig.setType(type.toString());

        AidaTableLayout layout = ((overrides.getLayout() == null) ? defaultConfig : overrides).getLayout();
        if ( layout == null ) {
            layout = COLUMN_MAJOR;
        }
        channelConfig.setLayout(layout.toString());

        channelConfig.setDescription(((overrides.getDescription() == null || overrides.getDescription().length() == 0) ? defaultConfig : overrides).getDescription());
        channelConfig.setFields(((overrides.getFields() == null || overrides.getFields().isEmpty()) ? defaultConfig : overrides).getFields());

        return channelConfig;
    }

    /**
     * Add default channel configuration if not specified directly with the channel
     *
     * @param aidaChannel the channel to add the default configuration to
     */
    private void copyConfig(AidaChannel aidaChannel) {
        // Highest priority from provider itself
        AidaChannelConfig providerConfig = this.channelProvider.getNativeChannelConfig(aidaChannel.getChannel());
        // Next from yaml channel
        AidaChannelConfig channelConfig = aidaChannel.getConfig();
        // Finally from yaml global section
        AidaChannelConfig defaultConfig = getConfig();

        // Merge all together with these priorities
        AidaChannelConfig mergedConfig = mergeConfig(mergeConfig(defaultConfig, channelConfig), providerConfig);
        aidaChannel.setConfig(mergedConfig);

        // Set default labels
        setDefaultLabels(mergedConfig.getFields());
    }

    /**
     * Set the label to be the name if it is not specified
     *
     * @param fields the fields to update
     */
    private void setDefaultLabels(List<AidaField> fields) {
        if ( fields != null ) {
            for (AidaField field : fields) {
                if (field.getLabel() == null) {
                    field.setLabel(field.getName());
                }
            }
        }
    }

    @Override
    public String toString() {
        return "AidaProvider{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", description='" + description + '\'' +
                ", config=" + config +
                ((channels.size() > 100) ? ", channels=[large set omitted!]" : ", channels=" + channels) +
                '}';
    }
}
