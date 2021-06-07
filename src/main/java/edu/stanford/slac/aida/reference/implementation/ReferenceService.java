package edu.stanford.slac.aida.reference.implementation;

import edu.stanford.slac.aida.lib.AidaProviderRunner;

public class ReferenceService {
    static {
        System.loadLibrary("aida-pva-reference");
    }

    public static void main(String[] args) {
        AidaProviderRunner.run(new ReferenceChannelProvider());
    }
}
