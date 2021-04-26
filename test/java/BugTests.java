package test.java;

// The following libraries must me imported for all AIDA clients
// da defines objects used in AIDA clients
// util defines common objects available thoughout the AIDA system
import edu.stanford.slac.aida.lib.da.*;
import edu.stanford.slac.aida.lib.util.common.*;


/**
 * Demonstrates known bugs in AIDA
 * 
 * Each method of this class, <code>Test01, Test02, ...</code> 
 * tests and demonstrates some known bug in Aida.
 *
 * @see daBugTest
 * @author 07-Oct-2004 Greg White: 
 *
 */
public class BugTests
{
	private DaObject da;

	public BugTests (String [] args)
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

			case 2:
				Test02();
				break;


			default:
				System.out.println( "No such test number parameter" + 
						    testNumber + "=\"" + testNumber + "\"" );
				System.out.println( "usage: java daAIDATest <test_number>..." );
			}
		}
		catch (Exception e)
		{
			System.out.println( "Exception: Executing test " + testNumber);
			System.err.println( e.getMessage() + " caused by " + e );
		}
		return;
	}


	
	/**
	 */
	public void Test01()
	{
		try
		{
			System.out.println("FIXED 07-Oct-2004 in _DaObject");
			System.out.println( 
				"TEST 01: Set param on DaReference but get with DaObject");
			System.out.println("Res: no parameters make it to data server");

			DaReference r = new DaReference("TEST//VAL");				
			r.setParam("STARTTIME=2001-09-06 12:07");
			r.setParam("ENDTIME=2001-09-06 12:10");
			r.compile(da);
			r.setType(DaValue.Type.DOUBLEA);
			Double [] v = (Double [])da.geta(r);					

			for (int i=0; i<v.length; i++)
				System.out.println( "v[" + i + "]=" + v[i] );
			da.reset();
		}
		catch (Exception e)
		{
			System.out.println("TEST 01:" + e.getMessage());
		}
		return;
	}

	/**
	 */
	public void Test02()
	{
		try
		{
			System.out.println("FIXED 07-Oct-2004 in _DaObject");
			System.out.println( 
				"TEST 02: Set param on DaObject and get with DaObject but" +
				" passing the reference:");
			System.out.println("Result, get parameters on server twice.");

			DaReference r = new DaReference("TEST//VAL");				
			da.setParam("STARTTIME=2001-09-06 12:07");
			da.setParam("ENDTIME=2001-09-06 12:10");
			r.compile(da);
			r.setType(DaValue.Type.DOUBLEA);
			Double [] v = (Double [])da.geta(r);					

			for (int i=0; i<v.length; i++)
				System.out.println( "v[" + i + "]=" + v[i] );
			da.reset();
		}
		catch (Exception e)
		{
			System.out.println("TEST 02:" + e.getMessage());
		}
		return;
	}

}
