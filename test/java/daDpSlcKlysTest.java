package test.java;

import java.util.*;

// The following libraries must me imported for all AIDA clients
// da defines objects used in AIDA clients
// util defines common objects available thoughout the AIDA system
import edu.stanford.slac.aida.lib.da.*;
import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.except.*;
import edu.stanford.slac.err.*;              // Error handling API (singleton).

/**
 * The Channel Access AIDA Test Suite. Tests Da access to EPICS data though
 * the DpSlcKlys data provider.
 *
 * @author  01-May-2007 Bob Hall. Based on daDpSlcTest.java
 */
public class daDpSlcKlysTest
{
    static Err err;                             // Err issuance singleton.

    /**
     * Tests AIDA Simple operations up to complex context queries.
     * @param args list of test numbers to be run from 1 to number of tests available
     */
    public static void main(String args[])
    {
        err = Err.getInstance("daDpSlcKlysTest");

        // Class defining tests of the DpSlcKlys Data Provider
        DpSlcKlysTests t = new DpSlcKlysTests(args);
        
        // Run tests based on command line arguments
        for ( int i = 0 ; i < args.length ; i++ )
        {
            try
            {
                if (!args[i].startsWith("-ORB") && !args[i].startsWith("Event") )
                    t.run( Integer.parseInt(args[i]) );
            }
            catch (NumberFormatException e)
            {
                System.out.println( "Ignored parameter" + i + "=\"" + args[i] + "\": invalid format" );
                System.out.println( "usage: java daAIDATest <test_number> ..." );
            }
        }
        System.exit(0);
    }
}
