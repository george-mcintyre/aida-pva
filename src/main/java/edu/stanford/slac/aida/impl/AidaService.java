package edu.stanford.slac.aida.impl;

import edu.stanford.slac.aida.lib.AidaProviderRunner;

public class AidaService {
    private final static String AIDA_PVA_LIB_NAME = "aida-pva";

    static {
        // Get library name from property or environment.
        String aidaPvaLibName = System.getProperty("aida.pva.lib.name", AIDA_PVA_LIB_NAME);
        String aidaPvaLibEnv = System.getenv("AIDA_PVA_LIB_NAME");
        if (aidaPvaLibEnv != null) {
            aidaPvaLibName = aidaPvaLibEnv;
        }

        System.loadLibrary(aidaPvaLibName);
    }

    public static void main(String[] args) {
        AidaProviderRunner.run(new AidaChannelProvider());
    }
}
