package edu.stanford.slac.aida.lib;

import org.epics.pvaccess.PVAException;

import static edu.stanford.slac.aida.impl.AidaService.elapsedTime;

public class AidaProviderRunner {
    public static void run(ChannelProvider aidaChannelProvider) {
        // Create new RPCServer
        AidaRPCServer server = new AidaRPCServer(aidaChannelProvider);

        // Create new Service for handling requests on the server
        // pass it an AidaChannelProvider which implements request() method
        AidaRPCService service = new AidaRPCService(aidaChannelProvider);

        // Register each channel hosted by this server, with the service
        server.registerServices(service);

        // Run the server to start servicing requests
        try {
            System.out.println("Server Ready: " + elapsedTime());
            server.run(0);
        } catch (PVAException e) {
            e.printStackTrace();
        }
    }
}
