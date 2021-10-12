package edu.stanford.slac.aida.impl;

import edu.stanford.slac.aida.lib.AidaProviderRunner;
import org.joda.time.DateTime;

public class AidaService {
    private final static String AIDA_PVA_LIB_NAME = "aida-pva";
    private final static DateTime serviceStartTime = DateTime.now();

    static {
        // Get library name from property or environment.
        String aidaPvaLibName = System.getProperty("aida.pva.lib.name", AIDA_PVA_LIB_NAME);
        String aidaPvaLibEnv = System.getenv("AIDA_PVA_LIB_NAME");
        if (aidaPvaLibEnv != null) {
            aidaPvaLibName = aidaPvaLibEnv;
        }
        if ( !aidaPvaLibName.equals(AIDA_PVA_LIB_NAME) ) {
            System.out.println("Loading Provider Shared Library: " + aidaPvaLibName);
        }

        System.loadLibrary(aidaPvaLibName);
    }

    public static void main(String[] args) {
        AidaProviderRunner.run(new AidaChannelProvider());
    }

    /**
     * Returns elapsed time string since service started
     * @return elapsed time string
     */
    public static String elapsedTime() {
        DateTime checkpoint = DateTime.now();
        Long elapsedMs = checkpoint.minus(serviceStartTime.getMillis()).getMillis();
        Long elapsedS = elapsedMs/1000;
        elapsedMs -= elapsedS*1000;
        Long elapsedM = elapsedS/60;
        elapsedS -= elapsedM*60;
        if ( elapsedM == 0 ) {
            return String.format("%d.%3ss",  elapsedS , elapsedMs).replace(' ', '0');
        } else {
            return String.format("%d:%2s.%3s",  elapsedM, elapsedS , elapsedMs).replace(' ', '0');
        }
    }
}
