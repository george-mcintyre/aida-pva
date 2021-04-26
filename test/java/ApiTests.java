package test.java;

// The following libraries must me imported for all AIDA clients
// da defines objects used in AIDA clients
// util defines common objects available thoughout the AIDA system
import edu.stanford.slac.aida.lib.da.*;
import edu.stanford.slac.aida.lib.util.common.*;

// For manipulating DaValue objects (Vector and Enumeration)
import java.util.*;

/**
 * This set of methods demonstrates the AIDA Application Programming Interface (API). It's
 * intended that each method introduces successively, a new API feature.
 * 
 * @author  11-Dec-2004, Greg White
 *
 */
public class ApiTests
{
	private DaObject da;

	public ApiTests (String [] args)
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

			case 3:
				Test03();
				break;

			case 4:
				Test04();
				break;

			case 5:
				Test05();
				break;

			case 6:
				Test06();
				break;

			default:
				System.out.println( "No such test number parameter" + 
						    testNumber + "=\"" + testNumber + "\"" );
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
	 * Get a Double value. 
	 */
	public void Test01()
	{
		try
		{
			Double v = (Double)da.get("TEST//VAL", DaValue.Type.DOUBLE);	
			System.out.println( "\t=" + v);

		}
		catch (Exception e)
		{
			System.out.println(e.getMessage());
		}
		return;
	}

	/*
	 * Get an array of Double value. 
	 */
	public void Test02()
	{
		try
		{
			Double[] v = (Double[])da.geta("XCOR:LI03:120//IMMS", 
						       DaValue.Type.DOUBLEA);	
			System.out.println( "IMMS of XCOR:LI03:120" );
			for (int i=0;i<v.length;i++)
				System.out.println( v[i] );
		}
		catch (Exception e)
		{
			System.out.println(e.getMessage());
		}
		return;
	}


	/*
	 * Get an array of double values, with a parameter. 
	 */
	public void Test03()
	{
		try
		{
			Float[] twiss;
			String query  = "BPMS:PR02:8032//twiss";
			da.setParam("MODE=7");
			twiss = (Float[])da.geta(query, DaValue.Type.FLOATA);
			
			System.out.println("Twiss parameters of " + query );
			for (int i = 0;i<twiss.length;i++)
				System.out.println( "v[" + i + "]=" + twiss[i] );

			da.reset();
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
			// The SLC Model Data Provider includes the cause when it throws 
			// exceptions. The following is a no-op if no cause was included.
			Throwable ce = e.getCause();
			if ( ce != null ) 
				System.err.println("\tcaused by " + ce.getMessage() );
		}
		return ;
	}

	/* Get a DaValue 
	 *
	 * In this demo case the DaValue contains history data. It's a contrived test
	 * because no parameters are used - the Data Provider for TEST/VAL.HIST has a
	 * default behaviour when no time region is specified.
	 *
	 * Also demonstrates extracting Vectors in DaValue to atomic types (eg Double).
	 */
	public void Test04()
	{
		try
		{
			DaValue v = da.getDaValue("TEST//VAL.HIST");

			// TEST//VAL.HIST returns a Vector of 3 Vectors. toString() it.
			System.out.println( "Test unparameterized history \t=" + v.toString() );

			// Extract the values sub-Vector "v[0]", and times sub-Vector "v[1]"
			Vector valuesvec = (Vector)v.get(0);
			Vector timesvec = (Vector)v.get(1);
			System.out.println("history values vector "+ valuesvec.toString());
			System.out.println("history times vector "+ timesvec.toString());

			// Convert the subvectors to array of Doubles and array of Strings.
			int N = valuesvec.size();               // Get values Vector's length 
 		        Double[] values = new Double[N];        // Allocate an array of Doubles.
			values = (Double[])valuesvec.toArray(values); // Copy Vector into array.
 		        String[] times = new String[N];        // Allocate an array of Strings.
			times = (String[])timesvec.toArray(times);    // Copy Vector into array.
			for (int i = 0;i<N;i++)
			{
				System.out.println( "history values [" + i + "]=" + values[i] );
				System.out.println( "history times [" + i + "]=" + times[i] );
			}
			da.reset();
		}
		catch (Exception e)
		{
			System.out.println( "TEST04 :" + e.toString() );
		}
		return ;
	}


	/* Get a DaValue, with parameters 
	 *
	 * Also demonstrates extracting Vectors in DaValue to individual Vectors, and 
	 * using them with an Enumeration.
	 */
	public void Test05()
	{
		try
		{
		        // This name only work in AIDA_PROD 
			String query = "XCOR:LI03:120//BACT.HIST";
		        da.setParam("STARTTIME", "07/30/2004 00:00:00");
			da.setParam("ENDTIME", "07/31/2004 00:00:00");
			DaValue v = da.getDaValue(query);

 		        Vector values = (Vector)v.elementAt(0);
			for (Enumeration e = values.elements(); e.hasMoreElements() ;)
				System.out.println(e.nextElement());

			Vector times = (Vector)v.elementAt(1);
			for (Enumeration e = times.elements(); e.hasMoreElements() ;)
				System.out.println(e.nextElement());

			da.reset();
		}
		catch (Exception e)
		{
			System.out.println( "TEST05 :" + e.toString() );
		}
		return ;
	}

	/* Get the Service ID(s) of a target 
	 *
	 * The value of each target is composed of the values contributed by a sequence of
	 * services. This test gets the sequence of service IDs.
	 */
	public void Test06()
	{
		try
		{
			// Test DaReference.getCount and getServices for valid name: 
			String query = "TEST//VAL";
			System.out.println("\nQuerying services for "+query);
			System.out.println("Correct behavior for valid query is count of"+
					   " services and service id(s)");
		        DaReference ref = new DaReference(query, da);
			int nServices = ref.getCount();
			System.out.println("Number Services = "+nServices);
			if (nServices > 0 )
			{
				int[] services = new int[nServices];
				services = ref.getServices();
				for (int i=0;i<nServices;i++) 
					System.out.println("Service id("+i+") = "+services[i]);
			}

			// Test DaReference.getCount and getServices for 2nd valid name: 
			query = "P2BPMHER//BPMS";			
			System.out.println("\nQuerying services for "+query);
			System.out.println("Correct behavior for valid query is count of"+
					   " services and service id(s)");
		        ref = new DaReference(query, da);
			nServices = ref.getCount();
			System.out.println("Number Services = "+nServices);
			if (nServices > 0 )
			{
				int[] services = new int[nServices];
				services = ref.getServices();
				for (int i=0;i<nServices;i++) 
					System.out.println("Service id("+i+") = "+services[i]);
			}

			// Test DaReference.getCount and getServices for *invalid* name.
			// Correct behavior is thrown UndefinedNameException.
			query = "doesnotexist//VAL";	
			System.out.println("\nQuerying services for INVALID query "+query);
			System.out.println("Correct behavior for invalid query is "+
					   "thrown UndefinedNameException");
		        ref = new DaReference(query, da);
			nServices = ref.getCount();
			System.out.println("Number Services = "+nServices);
			if (nServices > 0 )
			{
				int[] services = new int[nServices];
				services = ref.getServices();
				for (int i=0;i<nServices;i++) 
					System.out.println("Service id("+i+") = "+services[i]);
			}


		}
		catch (Exception e)
		{
			System.out.println( "TEST06 : "+ e.toString() );
		}
	}
}
