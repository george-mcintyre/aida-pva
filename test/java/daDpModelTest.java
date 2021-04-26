package test.java;

import edu.stanford.slac.err.*;              

/**
 * The Model Data Provider AIDA Test Suite runner. 
 *
 * @see DpModelTests
 * @author Greg White 21-Jul-2008
 */
public class daDpModelTest
{
      static Err err;                  // Err issuance singleton.

      /**
       * Tests AIDA Simple operations up to complex context queries.
       * @param args list of test numbers to be run from 1 to 
       * number of tests available
       */
	public static void main(String args[])
	{
		err = Err.getInstance("daDpModelTest");

		// Class defining tests of the DpModel Data Provider
		DpModelTests t = new DpModelTests(args);
        
		// Run tests based on command line arguments
		for ( int i = 0 ; i < args.length ; i++ )
		{
			try
			{
				if (!args[i].startsWith("-ORB") && 
				    !args[i].startsWith("Event") )
					t.run( Integer.parseInt(args[i]) );
			}
			catch (NumberFormatException e)
			{
				System.out.println( "Ignored parameter" + i 
						    + "=\"" + args[i] + 
						    "\": invalid format" );
				System.out.println( 
			  "usage: java daDpModelTest <test_number> ..." );
			}
		}
		System.exit(0);
	}
}
