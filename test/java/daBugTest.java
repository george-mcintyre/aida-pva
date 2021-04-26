/*
 * daBugTest.java
 */
package test.java;

import edu.stanford.slac.err.*;              // Error handling API (singleton).


/**
 * Aida bug demo suite runner. This class is a wrapper for the tests defined 
 * in <code>BugTests.java</code>, allowing those tests to be run interactively.
 * 
 * @author  Greg White
 */
public class daBugTest
{
		static Err err;                             // Err issuance singleton.

		/**
		 * Tests AIDA Simple operations up to complex context queries.
		 * @param args list of test numbers to be run from 1 to number of tests available
		 */
		public static void main(String args[])
		{
				err = Err.getInstance("daBugTest");

				BugTests t = new BugTests(args);
	
				// Run tests based on command line arguments
				for ( int i = 0 ; i < args.length ; i++ )
				{
						try
						{
								if (!args[i].startsWith("-ORB")&&!args[i].startsWith("Event") )
										t.run( Integer.parseInt(args[i]) );
						}
						catch (NumberFormatException e)
						{
								System.out.println( "Ignored parameter" + i + "=\"" + 
													args[i] + "\": invalid format" );
								System.out.println( "usage: java daBugTest <test_number> ..." );
						}
				}
				System.exit(0);
		}
}

