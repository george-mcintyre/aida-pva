package edu.stanford.slac.aida.reference.implementation;

import edu.stanford.slac.aida.lib.AidaProviderRunner;

import static slac.aida.NativeChannelProvider.aidaServiceInit;

public class ReferenceService {
    private final static String AIDA_PVA_LIB_NAME = "aida-pva";
    static {
        // Get library name from property or environment.
        String aidaPvaLibName = System.getProperty("aida.pva.lib.name", AIDA_PVA_LIB_NAME);
        String aidaPvaLibEnv = System.getenv("AIDA_PVA_LIB_NAME");
        if (aidaPvaLibEnv != null) {
            aidaPvaLibName = aidaPvaLibEnv;
        }

        System.loadLibrary(aidaPvaLibName);

        // Call initialisation for the channel provider
        aidaServiceInit();
    }

    public static void main(String[] args) {
        AidaProviderRunner.run(new ReferenceChannelProvider());
    }
}
