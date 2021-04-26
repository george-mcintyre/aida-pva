
/*
 * daAidaTest.java
 *
 * Abs: Tests AIDA context with simple scenario
 *
 * Args: see @param of main()
 *
 * Rem:
 *
 * Auth: see @author
 * -----------------------------------------------------------------------------
 * Mod:  see @version for class def
 * =============================================================================
 */
package test.java;

import edu.stanford.slac.err.*;              // Error handling API (singleton).


/**
 * The general Aida test and demo suite runner. This class is a wrapper for the tests defined 
 * in <code>Tests.java</code>, allowing those tests to be run interactively.
 * 
 * @author  George McIntyre
 * @version 25-Jun-2002 George McIntyre: Initial Version
 * @version 12-Nov-2002, Ron MacKenzie:  Change import of except from aida to package except
 * @version 15-Sep-2004, Greg White (greg): Convert use of Err for singleton re-write.
 */
public class daAIDATest
{
	static Err err;                             // Err issuance singleton.

    /**
     * Tests AIDA Simple operations up to complex context queries.
     * @param args list of test numbers to be run from 1 to number of tests available
     */
    public static void main(String args[])
    {
      	err = Err.getInstance("daAIDATest");

	Tests t = new Tests(args);
	
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

