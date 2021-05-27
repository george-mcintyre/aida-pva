package edu.stanford.slac.aida.provider.sample;

import edu.stanford.slac.aida.lib.AidaProviderRunner;

public class SampleProvider {
    public static void main(String[] args) {
        AidaProviderRunner.run(new SampleChannelProvider());
    }
}
