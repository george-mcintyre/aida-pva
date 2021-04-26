/*
 * daErrTest.java
 *
 * Abs: Tests error conditions and appropriate error handling
 *
 * =============================================================================
 */
package test.java;
import edu.stanford.slac.err.*;              // Error handling API (singleton).
                            

/**
 * The general Aida error handling test suite runner. This class is a
 * wrapper for the tests defined in <code>ErrTests.java</code>,
 * allowing those tests to be run interactively.
 * 
 * @author  05-Sep-2004, Greg White.
 * @version 15-Sep-2004, Greg White (greg): Convert use of Err for singleton re-write.
 */
public class daErrTest
{
		static Err err_;                        // Error issuance singleton.

		/**
		 * Tests error condition in AIDA.
		 * @param args space delimited list of test numbers to be run from
		 * 1 to number of tests available
		 */
		public static void main(String args[])
		{
				err_ = Err.getInstance("daErrTest");

				ErrTests t = new ErrTests(args);
	
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
								System.out.println( "Usage: java daErrTest <test_number> ..." );
						}
				}
				System.exit(0);
		}
}

