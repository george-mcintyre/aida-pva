/*
 * daDpTestHistTest.java
 *
 */
package test.java;

import edu.stanford.slac.err.*;                // Message logging API.

/**
 * DpTestHist data provider test suite runner. Allows for running the
 * tests defined in DpTestHistTests interactiveley.
 *
 * @see DpTestHistTests
 * @author  Greg White (greg)
 * @version 18-May-2004, Greg White (greg): Initial Version
 */
public class daDpTestHistTest
{
 	static Err err_;  // Singleton Err reference.

	/**
	 * Tests Simple operations up to complex context queries.
	 * @param args list of test numbers to be run from 1 to number of tests available
	 */
	public static void main(String args[])
	{
		err_ = Err.getInstance("daDpTestHistTest");		

		DpTestHistTests t = new DpTestHistTests(args);
	
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
				System.out.println( "Ignored parameter" + i + "=\"" + 
						    args[i] + "\": invalid format" );
				System.out.println( "usage: java daDpTestHistTest " +
						    "<test_number> ..." );
			}
		}
		System.exit(0);
	}
}

