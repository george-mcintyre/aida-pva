package edu.stanford.slac.aida.impl;

import edu.stanford.slac.aida.lib.AidaProviderRunner;
import org.joda.time.DateTime;

import java.util.logging.Logger;

/**
 * @file
 * The AidaService class is the main class that gets run to start an
 * AIDA-PVA Provider.  It automatically loads the Native Provider Library on startup.
 * <p>
 * It also runs an AidaChannelProvider  listener so that it can intercept search requests for
 * channels that the loaded Channel Provider supports and direct them to the correct entry points.
 */
public class AidaService {
    /**
     * This is the default name for the AIDA-PVA Native Provider Library.
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
     * This static block is run once whenever the  Service is started to load the Native Provider Library
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
            logger.info("Loading Chanel Provider Shared Library: " + aidaPvaLibName);
        }

        // Load the Native Provider Library
        System.loadLibrary(aidaPvaLibName);
    }

    /**
     * This is the main entry point to the AIDA-PVA Service.
     *
     * @param args the commandline arguments to the service are ignored
     */
    public static void main(String[] args) {
        AidaProviderRunner.run(new AidaChannelProvider());
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
        Long elapsedMs = checkpoint.minus(serviceStartTime.getMillis()).getMillis();

        // Mess around to get minutes and seconds
        Long elapsedS = elapsedMs / 1000;
        elapsedMs -= elapsedS * 1000;
        Long elapsedM = elapsedS / 60;
        elapsedS -= elapsedM * 60;

        // Return the string with either minutes and seconds or just seconds and milliseconds
        if (elapsedM == 0) {
            return String.format("%d.%3ss", elapsedS, elapsedMs).replace(' ', '0');
        } else {
            return String.format("%d:%2s.%3s", elapsedM, elapsedS, elapsedMs).replace(' ', '0');
        }
    }
}
