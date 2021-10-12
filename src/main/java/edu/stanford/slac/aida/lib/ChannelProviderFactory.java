package edu.stanford.slac.aida.lib;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.dataformat.yaml.YAMLFactory;
import edu.stanford.slac.aida.lib.model.AidaProvider;

import java.io.File;
import java.util.logging.Logger;

public class ChannelProviderFactory {
    private final static String CHANNELS_FILENAME_DEFAULT = "channels.yml";
    private static final Logger logger = Logger.getLogger(ChannelProviderFactory.class.getName());

    /**
     * Read all instance attribute pairs from the configuration file specified in an environment
     * variable of a file called "channels.yml" in the current working directory
     * <p>
     * A yaml file (*.yml or *.yaml) defines the channels of this AIDA Channel Provider
     *
     * @param channelProvider the channel provider
     * @return an AidaProvider object or null if there is a problem reading the configuration
     */
    public static AidaProvider create(ChannelProvider channelProvider) {
        // Get service name and channel definitions for the server to publish.
        // Priority: max=properties, medium=environment, low=default
        String channelsFilename = System.getProperty("aida.pva.channels.filename", CHANNELS_FILENAME_DEFAULT);
        String channelsFilenameFromEnv = System.getenv("AIDA_PVA_CHANNELS_FILENAME");
        if (channelsFilenameFromEnv != null) {
            channelsFilename = channelsFilenameFromEnv;
            System.out.println("Loading channel configuration from: " + channelsFilename);
        }

        // Set up the object mapper to read the channels
        ObjectMapper mapper = new ObjectMapper(new YAMLFactory());
        try {
            File channelSource = new File(channelsFilename);
            AidaProvider aidaProvider;
            if (channelSource.exists()) {
                aidaProvider = mapper.readValue(channelSource, AidaProvider.class);
            } else {
                throw new RuntimeException("Can't access channel provider configuration file: " + channelsFilename);
            }
            aidaProvider.setChannelProvider(channelProvider);
            return aidaProvider;
        } catch (Exception e) {
            logger.severe("Unable to initialise channel provider with " + channelsFilename + " : " + e.getMessage());
            throw new RuntimeException(e.getMessage());
        }
    }
}
