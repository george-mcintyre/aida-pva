package test.java;

import edu.stanford.slac.err.*;              

/**
 * The SLC Model Data Provider AIDA Test Suite runner. 
 *
 * @see DpSlcModelTests
 * @author  Greg White
 */
public class daDpSlcModelTest
{
      static Err err;                  // Err issuance singleton.

      /**
       * Tests AIDA Simple operations up to complex context queries.
       * @param args list of test numbers to be run from 1 to 
       * number of tests available
       */
	public static void main(String args[])
	{
		err = Err.getInstance("daDpSlcModelTest");

		// Class defining tests of the DpSlcModel Data Provider
		DpSlcModelTests t = new DpSlcModelTests(args);
        
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
			  "usage: java daDpSlcModelTest <test_number> ..." );
			}
		}
		System.exit(0);
	}
}
