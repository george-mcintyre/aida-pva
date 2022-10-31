/*
 * @file
 * Channel Provider Runner.
 */
package edu.stanford.slac.aida.lib;

import org.epics.pvaccess.PVAException;

import java.util.logging.Level;
import java.util.logging.Logger;

import static edu.stanford.slac.aida.impl.AidaService.elapsedTime;

/**
 * Channel Provider Runner.
 * This class can run a Channel Provider so that it can service requests.
 * It is called by {@link edu.stanford.slac.aida.impl.AidaService#main}.
 */
public class AidaProviderRunner {
    /**
     * Logger to log info
     */
    private static final Logger logger = Logger.getLogger(AidaProviderRunner.class.getName());

    /**
     * Run the given AIDA-PVA Channel Provider
     * @param aidaChannelProvider the given AIDA-PVA Channel Provider
     */
    public static void run(ChannelProvider aidaChannelProvider) {
        logger.info("Creating RPC Server ...");
        // Create new RPCServer
        AidaRPCServer server = null;
        try {
            server = new AidaRPCServer(aidaChannelProvider);
        } catch (Exception e) {
            logger.log(Level.SEVERE, "Failed to create RPC Server: " + e.getMessage());
            return;
        }
        logger.info("Done");

        // Create new Service for handling requests on the server
        // pass it an AidaChannelProvider which implements request() method
        logger.info("Creating AIDA Service ...");
        AidaRPCService service = null;
        try {
            service = new AidaRPCService(aidaChannelProvider);
        } catch (Exception e) {
            logger.log(Level.SEVERE, "Failed to create AIDA-PVA Service: " + e.getMessage());
            return;
        }
        logger.info("Done");

        // Register each channel hosted by this server, with the service
        try {
            logger.info("Registering Services ...");
            server.registerServices(service);
            logger.info("Services Registered");
        } catch (Exception e) {
            logger.log(Level.SEVERE, "Server Failed to register channels: " + e.getMessage());
            e.printStackTrace();
            return;
        }

        // Run the server to start servicing requests
        try {
            server.printInfo();   // Print RPC service information
            logger.info("AIDA-PVA version " + System.getProperty("aida.pva.version", "-.-.-") + ":  Server Ready: " + elapsedTime());
            server.run(0);
        } catch (PVAException e) {
            e.printStackTrace();
        }
    }
}
