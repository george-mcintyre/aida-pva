package test.java;

import edu.stanford.slac.err.*;              

/**
 * The EPICS Klystron AIDA Test Suite runner. 
 *
 * @see DpKlysTests
 * @author Bob Hall
 * @version 01-Jun-2011  Bob Hall: Initial version
 */
public class daDpKlysTest
{
      static Err err;                  // Err issuance singleton.

      /**
       * Tests AIDA Simple operations up to complex context queries.
       * @param args list of test numbers to be run from 1 to 
       * number of tests available
       */
	public static void main(String args[])
	{
		err = Err.getInstance("daDpKlysTest");
		int testnum = 0;

		// Class defining tests of the DpSlcBpm Data Provider
		DpKlysTests t = new DpKlysTests(args);
        
		// Run tests based on command line arguments
		for ( int i = 0 ; i < args.length ; i++ )
		{
			try
			{
			        testnum = Integer.parseInt(args[i]);
				t.run( testnum );
			}
			catch (NumberFormatException e)
			{
				System.out.println( "Ignored parameter" + i 
						    + "=\"" + args[i] + 
						    "\": invalid format" );
				System.out.println( 
			  "usage: aidatest daDpKlysTest <test_number> ..." );
			}
		}
		System.exit(0);
	}
}
