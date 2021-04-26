package test.java;

import edu.stanford.slac.err.Err;

/**
 * The Channel Access AIDA Test Suite runner. Tests Da access to EPICS data though
 * the DpCa data provider.
 * 
 * @see DpCaTests
 * @author  11-Sep-2002 Greg White. Based on daAIDATest.java
 * @version 20-Nov-2002, Ron MacKenzie. Change except from aida to pkg except.
 */
public class daDpCaTest
{
	static Err err_;                        // Error issuance singleton.

	/**
	 * Tests AIDA Simple operations up to complex context queries.
	 * @param args list of test numbers to be run from 1 to number of tests available
	 */
	public static void main(String args[])
	{

		// Register with the Error Messaging Service
		err_ = Err.getInstance("daDpCaTest");

		// Class defining tests of the DpCa Data Provider
		DpCaTests t = new DpCaTests(args);
        
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

