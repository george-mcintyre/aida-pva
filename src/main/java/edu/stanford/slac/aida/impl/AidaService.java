/**
 * @file The AidaService class is the main class that gets run to start an
 * AIDA-PVA Provider.
 */
package edu.stanford.slac.aida.impl;

import edu.stanford.slac.aida.lib.AidaProviderRunner;
import edu.stanford.slac.except.ServerInitialisationException;
import org.joda.time.DateTime;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Enumeration;
import java.util.Properties;
import java.util.logging.Logger;

/**
 * The AidaService class is the main class that gets run to start an
 * AIDA-PVA Provider.  It automatically loads the Native Provider Library on startup.
 * <p>
 * It also runs an AidaChannelProvider  listener so that it can intercept search requests for
 * channels that the loaded Channel Provider supports and direct them to the correct entry points.
 */
public class AidaService {
    /**
     * This is the default name for the AIDA-PVA Channel Provider library file.
     * Note that it is operating system agnostic, so it does not add the extension
     * e.g. `.so`, `.dll`, `.EXE`, etc.
     */
    private final static String AIDA_PVA_LIB_NAME = "aida-pva";

    /**
     * This is a simple timer to store the time that the service started so that {@link AidaService#elapsedTime()}
     * will be able to show the amount of time it took to start up the service.
     */
    private final static DateTime serviceStartTime = DateTime.now();

    /**
     * Logger to log info
     */
    private static final Logger logger = Logger.getLogger(AidaService.class.getName());

    /*
     * This static block is run once whenever the AIDA-PVA Service is started to load the Channel Provider library
     */
    static {
        // Get library name from property (commandline or resource file).
        String aidaPvaLibName = System.getProperty("aida.pva.lib.name", AIDA_PVA_LIB_NAME);

        // Override with the environment variable if it is set
        String aidaPvaLibEnv = System.getenv("AIDA_PVA_LIB_NAME");
        if (aidaPvaLibEnv != null) {
            aidaPvaLibName = aidaPvaLibEnv;
        }

        // If we've overridden the default name then log it to the console
        if (!aidaPvaLibName.equals(AIDA_PVA_LIB_NAME)) {
            logger.info("Loading Channel Provider Shared Library: " + aidaPvaLibName);
        }

        // Set default EPICS_PVA properties for AIDA_PVA server
        defaultEpicsPropertiesIfNotSet();
        loadSystemProperties();

        // Load the Channel Provider library
        System.loadLibrary(aidaPvaLibName);
    }

    /**
     * Load properties from resource file
     */
    private static void loadSystemProperties() {
        Properties prop = new Properties();
        try {
            InputStream inputStream = AidaService.class.getResourceAsStream("/application.properties");
            prop.load(inputStream);
            Enumeration<?> it = prop.propertyNames();
            while (it.hasMoreElements()) {
                String name = (String) it.nextElement();
                if ( System.getProperty(name) == null ) {
                    System.setProperty(name, prop.getProperty(name));
                }
            }
        } catch (FileNotFoundException ignored) {
        } catch (IOException ignored) {
        }
    }

    /**
     * For AIDA-PVA servers we don't normally care about the EPICS_PVA_ADDR_LIST, and EPICS_PVA_AUTO_ADDR_LIST
     * because they are for clients primarily.  But when the server delegates to another server
     * when implementing an alias it needs to act as a client.  In this case there is only one
     * way in which it should act.  It should ONLY send requests to the local host!
     *
     * So we need to set EPICS_PVA_AUTO_ADDR_LIST to no, and EPICS_AUTO_ADDR_LIST to the name of the local host.
     * We could do this on the command line when starting the service but its better to do this automatically
     * as it will never change.
     */
    private static void defaultEpicsPropertiesIfNotSet() {
        if (System.getenv("EPICS_PVA_ADDR_LIST") == null && System.getProperty("EPICS_PVA_ADDR_LIST") == null) {
            try {
                String hostname = InetAddress.getLocalHost().getHostName();
                System.setProperty("EPICS_PVA_ADDR_LIST", hostname);
                logger.info("Defaulting EPICS_PVA_ADDR_LIST to: " + hostname);
            } catch (UnknownHostException ignored) {
            }
        }

        if (System.getenv("EPICS_PVA_AUTO_ADDR_LIST") == null && System.getProperty("EPICS_PVA_AUTO_ADDR_LIST") == null) {
            System.setProperty("EPICS_PVA_AUTO_ADDR_LIST", "no");
            logger.info("Defaulting EPICS_PVA_AUTO_ADDR_LIST to: no");
        }
    }

    /**
     * This is the main entry point to the AIDA-PVA Service.
     *
     * @param args the commandline arguments to the service are ignored
     */
    public static void main(String[] args) {
        try {
            AidaProviderRunner.run(new AidaChannelProvider());
        } catch (ServerInitialisationException e) {
            logger.info("Failed to initialise service: " + e.getMessage() );
            e.printStackTrace();
        }
    }

    /**
     * Returns elapsed time string since this service started
     *
     * @return elapsed time string
     */
    public static String elapsedTime() {
        // Get the time now
        DateTime checkpoint = DateTime.now();

        // Elapsed time is now minus start time
        long elapsedMs = checkpoint.minus(serviceStartTime.getMillis()).getMillis();

        // Mess around to get minutes and seconds
        long elapsedS = elapsedMs / 1000;
        elapsedMs -= elapsedS * 1000;
        long elapsedM = elapsedS / 60;
        elapsedS -= elapsedM * 60;

        // Return the string with either minutes and seconds or just seconds and milliseconds
        if (elapsedM == 0) {
            return String.format("%d.%3ss", elapsedS, elapsedMs).replace(' ', '0');
        } else {
            return String.format("%d:%2s.%3s", elapsedM, elapsedS, elapsedMs).replace(' ', '0');
        }
    }
}
