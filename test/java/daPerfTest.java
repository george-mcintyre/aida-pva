
/*
 * daPerfTest.java
 *
 * Abs: Performance tests AIDA
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
 * Runs the  Aida performance tests. This class is a wrapper for the tests defined 
 * in <code>PerfTests.java</code>, allowing those tests to be run interactively.
 * 
 * @author  Robert C. Sass
 * @version 15-Jul-2004 Robert C. Sass: Initial Version
 */
public class daPerfTest
{
	static Err err_;                        // Error issuance singleton.

    /**
     * Runs AIDA performance tests.
     * @param args list of test numbers to be run from 1 to number of tests available
     */
    public static void main(String args[])
    {
	err_ = Err.getInstance("daErrTest");

	PerfTests pt = new PerfTests(args);
        if (args.length < 5)  //ORBinitRef is 5th one
	{
	    System.out.println("Wrong number of agruments");
	    System.out.println("Usage: daPerfTest (int testnum string matfile string testcomment)");
	}
	else
	{
	// Run test based on command line arguments:
	// int testnumber string data file string comment
	    try
	    {
		    pt.run( Integer.parseInt(args[0]), args[1], args[2] );
	    }
	    catch (Exception e)
	    {
		System.out.println(  e.getMessage() );
	    }
	}
	System.exit(0);
    }
}

