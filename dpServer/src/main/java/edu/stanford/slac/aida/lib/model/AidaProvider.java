package edu.stanford.slac.aida.lib.model;

import edu.stanford.slac.aida.lib.PVTopStructure;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.logging.Logger;

public class AidaProvider {
    private static final Logger logger = Logger.getLogger(AidaProvider.class.getName());

    private Long id;
    private String name = "";
    private String description;
    private Set<AidaChannel> channels = new HashSet<AidaChannel>();
    private final Map<String, AidaChannel> channelMap = new HashMap<String, AidaChannel>();

    public AidaProvider() {
    }

    public AidaProvider(Long id, String name, String description, Set<AidaChannel> channels) {
        this.id = id;
        this.name = name;
        this.description = description;
        this.channels = channels;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public Set<AidaChannel> getChannelSet() {
        return channels;
    }

    public void setChannels(Set<AidaChannel> channels) {
        this.channels = channels;
    }

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
     * The total number of instance//attribute pairs.
     * This method does not count pairs in their EPICS PV form so
     * Aida instance attribute, {@code PEP2INJF//BPMS} and EPICS PV name
     * {@code PEP2INJF:BPMS} are counted only once even though they are both supported
     *
     * @return count
     */
    public Integer getInstanceAttributeCount() {
        return this.channels.size();
    }

    /**
     * Set structure for a channel
     *
     * @param channelName channel to set structure
     * @param structure   structure to set
     */
    public void setStructure(String channelName, PVTopStructure structure) {
        AidaChannel channel = getAidaChannel(channelName);
        if (channel == null) {
            logger.severe("Channel not supported: " + channelName);
            return;
        }

        channel.setStructure(structure);
    }

    /**
     * Retrieve the channel structure
     *
     * @param channelName channel name to retrieve structure for
     * @return the structure
     */
    public PVTopStructure getStructure(String channelName) {
        AidaChannel channel = getAidaChannel(channelName);
        if (channel == null) {
            logger.severe("Channel not supported: " + channelName);
            return null;
        }

        return channel.getStructure();
    }

    private AidaChannel getAidaChannel(String channelName) {
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
            for (AidaChannel aidaChannel : getChannelSet()) {
                String instance = aidaChannel.getInstance();
                String attribute = aidaChannel.getAttribute();
                if (attribute == null || attribute.equals("")) {
                    this.channelMap.put(instance, aidaChannel);
                } else {
                    // Add EPICS style name
                    this.channelMap.put(instance + ":" + attribute, aidaChannel);

                    // Add AIDA style name
                    this.channelMap.put(instance + "//" + attribute, aidaChannel);
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
                ((channels.size() > 100) ? "channels=[large set omitted!]" : ", channels=" + channels) +
                '}';
    }
}
