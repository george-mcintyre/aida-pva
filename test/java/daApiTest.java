/*
 * daApiTest.java
 *
 * Abs: Runs AIDA demo methods in ApiTests.java
 *
 * =============================================================================
 */
package test.java;
import edu.stanford.slac.err.*;              // Error handling API (singleton).
                            

/**
 * The general Aida demo test suite runner. This class is a
 * wrapper for the demo methods defined in <code>ApiTests.java</code>,
 * allowing those demos to be run interactively. Each demo exercises some
 * aspect of the AIDA API.
 * 
 * @author  11-Dec-2004, Greg White.
 */
public class daApiTest
{
	static Err err_;                        // Error issuance singleton.

	/**
	 */
	public static void main(String args[])
	{
		// Register with the Error Messaging Service
		err_ = Err.getInstance("daApiTest");

		ApiTests t = new ApiTests(args);
	
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
				System.out.println( "Ignored parameter" + i + 
						    "=\"" + args[i] + "\": invalid format" );
				System.out.println( "Usage: java daApiTest <test_number> ..." );
			}
		}
		System.exit(0);
	}
}

