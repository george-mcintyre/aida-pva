package test.java;

import java.util.*;                               // Vector
import java.lang.*;
import edu.stanford.slac.aida.lib.da.*;           // AIDA API
import edu.stanford.slac.aida.lib.util.common.*;  // DaValue


/**
 * The EPICS Klystron AIDA Data Provider test suite. 
 *
 * @see daDpKlysTest
 * @author  Bob Hall 
 * @version 01-Jun-2011 Bob Hall: Initial Version
 */
public class DpKlysTests
{
	private DaObject da;

	public DpKlysTests(String[] args)
	{
		try 
		{
			da = new DaObject(args);
		}
		catch ( Exception e ) 
		{
			System.err.println( e.getMessage() );
			e.printStackTrace();
		}
	}

	public void run(int testNumber )
	{
		try 
		{
			switch ( testNumber )
			{
			case 1:
				Test01();
				break;
			default:
				System.out.println( "No such test number parameter " + 
						    testNumber + "=\"" + testNumber + "\"" );
				System.out.println( "usage: java daDpKlysTest " +
						    "<test_number> ..." );
			}
		}
		catch (Exception e)
		{
			System.out.println( "Exception: Executing test " + testNumber);
			System.err.println( e.getMessage() + " caused by " + e );
		}
		return;
	}


	/* 
	**  Acquire SHORT type through DaServer without reference.
	*/
	public void Test01()
	{
		try
		{
			System.out.println( "=============================");
			System.out.println( "\nTEST 01: KLYS:LI28//TACT\n\n");

			String query = "KLYS:LI28//TACT";
                        String beam = "1";

			da.reset();

			da.setParam("BEAM", beam);

			Short v = (Short) da.get(query, DaValue.Type.SHORT);

                        System.out.println("v = " + v + "\n");

		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
		}
		return ;
	}
}
