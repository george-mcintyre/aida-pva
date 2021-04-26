package test.java;

import edu.stanford.slac.err.*;              // Error handling API (singleton).

/**
 * The SLC History Data provider AIDA Test Suite runner. 
 *
 * @see DpSlcHistTests
 * @author  10-Aug-2004 Bob Hall. Based on daDpSlcTest.java
 */
public class daDpSlcHistTest
{
    static Err err;                             // Err issuance singleton.

    /**
     * Tests AIDA Simple operations up to complex context queries.
     * @param args list of test numbers to be run from 1 to number of tests available
     */
    public static void main(String args[])
    {
        err = Err.getInstance("daDpSlcHistTest");

        // Class defining tests of the DpSlcHist Data Provider
        DpSlcHistTests t = new DpSlcHistTests(args);
        
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
                System.out.println( "usage: java daDpSlcHistTest <test_number> ..." );
            }
        }
        System.exit(0);
    }
}
