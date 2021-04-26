package test.java;

import edu.stanford.slac.err.*;              

/**
 * The SLC Buff Data Provider AIDA Test Suite runner. 
 *
 * @see DpSlcBuffTests
 * @author  Greg White
 */
public class daDpSlcBuffTest
{
      static Err err;                  // Err issuance singleton.

      /**
       * Tests AIDA Simple operations up to complex context queries.
       * @param args list of test numbers to be run from 1 to 
       * number of tests available
       */
	public static void main(String args[])
	{
		err = Err.getInstance("daDpSlcBuffTest");
		int testnum = 0;

		// Class defining tests of the DpSlcBuff Data Provider
		DpSlcBuffTests t = new DpSlcBuffTests(args);
        
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
			  "usage: java daDpSlcBuffTest <test_number> ..." );
			}
		}
		System.exit(0);
	}
}
