package edu.stanford.slac.aida.lib;

import edu.stanford.slac.aida.provider.sample.standard.AidaChannelProvider;
import org.epics.pvaccess.server.impl.remote.ServerContextImpl;
import org.epics.pvaccess.server.impl.remote.plugins.DefaultBeaconServerDataProvider;

public class AidaProviderRunner {
    public static void run(AidaChannelProvider aidaChannelProvider) {
        // Create a context with default configuration values.
        final ServerContextImpl context = new ServerContextImpl();
        context.setBeaconServerStatusProvider(new DefaultBeaconServerDataProvider(context));

        try {
            context.initialize(aidaChannelProvider);

            // Display basic information about the context.
            System.out.println(context.getVersion().getVersionString());
            context.printInfo();
            System.out.println();

            new Thread(new Runnable() {

                public void run() {
                    try {
                        System.out.println("Running server...");
                        context.run(0);
                        System.out.println("Done.");
                    } catch (Throwable th) {
                        System.out.println("Failure:");
                        th.printStackTrace();
                    }
                }
            }, "pvAccess server").start();
        } catch (Throwable th) {
            th.printStackTrace();
        }
    }
}
