package test.java;

import java.util.*;                               // Vector
import java.lang.*;
import edu.stanford.slac.aida.lib.da.*;           // AIDA API
import edu.stanford.slac.aida.lib.util.common.*;  // DaValue


/**
 * The Relational Database AIDA Data Provider test suite. 
 *
 * @see daDpRdbTest
 * @author  Greg White
 * @version 10-Jun-2008 Greg White: Initial Version
 */
public class DpRdbTests
{
	private DaObject da;

	public DpRdbTests(String[] args)
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
			case 101:
				Test101();
				break;				
			default:
				System.out.println( "No such test number parameter " + 
						    testNumber + "=\"" + testNumber + "\"" );
				System.out.println( "usage: java daDpRdbTest " +
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
	** Acquisition, whole machine: BSA rootnames, EPICS only. Simply print using toString();
	*/
	public void Test01()
	{
		try
		{
			System.out.println( "=============================");
			System.out.println( "\nTEST 01: whole machine: BSA rootnames, EPICS only\n\n");

			/* Do the acquisition */
			DaValue v = da.getDaValue("LCLS//BSAnames");

			/* Just print out the results using DaValue's own toString */
			System.out.println( v.toString() );

			da.reset(); 
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
		}
		return ;
	}

	/* 
	** Acquisition, whole machine: BSA rootnames, EPICS only. Extract names and print them. 
	*/
	public void Test02()
	{
		try
		{
			System.out.println( "=============================");
			System.out.println( "\nTEST 02: whole machine: BSA rootnames, EPICS only\n" +
					    "Extract names and print them as array of Strings\n\n");

			/* Do the acquisition */
			DaValue v = da.getDaValue("LCLS//BSA.rootnames.EPICS");

			/* Extract the names returned. In this case there is a single vector */
			Vector namesvec = (Vector)v.get(0); /* Extract 1st to get length */
			int bsaM = namesvec.size();         /* Number of BSA rootnames is len of vec */

			/* Convert the vector to an array of Strings if you like */
			String[] names = new String[bsaM];
			namesvec.toArray(names);

			/* Print em out one by one if you must */
			for (int i=0; i<bsaM; i++) 
			    System.out.println( names[i] );

			da.reset();
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
		}
		return ;
	}

	/* 
	** Acquisition, via DaReference
	*/
	public void Test03()
	{
		try
		{
			System.out.println( "=============================");
			System.out.println( "\nTEST 03: \n\n");

			DaReference r = new DaReference("LCLS//BSAnames", da);
			r.setType(DaValue.Type.STRUCT);
			DaValue v = r.getDaValue();

			/* Extract the names returned. In this case there are two vectors of Strings, first the
                           BSA root-names, then the sources (EPICS or SLC) 
                        */
			Vector namesvec = (Vector)v.get(0); // Extract 1st column, BSA root-names
			int bsaM = namesvec.size();         // Number of BSA rootnames is len of vec
			String[] names = new String[bsaM];  // Allocate and convert to array of Strings 
			namesvec.toArray(names);

			// Extract sources column, and convert to array of Strings.
			Vector sourcesvec = (Vector)v.get(1); 
			String[] sources = new String[bsaM];
			sourcesvec.toArray(sources);

			// Print em out one by one if you must
			System.out.println( "BSA Root names \tSources");
			for (int i=0; i<bsaM; i++) 
			    System.out.println( names[i] + "\t" + sources[i] );

			da.reset();
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
		}
		return ;
	}

	/* 
	** Acquisition, timing tests
	*/
	public void Test04()
	{
		try
		{
			System.out.println( "=============================");
			System.out.println( "\nTEST 04: Timing Tests");

			long t1=0, t2=0, columnsN=0, rowsM=0;
			String query;                           // The Aida Query string
			DaValue v;                              // The aggregate value returned

			// getDaValue on DaObject directly. No JVM warmup.
			query = "LCLS//BSA.rootnames.EPICS";
			System.out.println( "\n1." + query + "(M>1xN>1). DaObject. No JVM warmup. Possible Oracle warmup.");
			t1 = System.currentTimeMillis();
			v = da.getDaValue( query );
			t2 = System.currentTimeMillis();
			columnsN = ((Vector)v).size();
			rowsM = ((Vector)v.get(0)).size();
			System.out.println("M Rows = " + rowsM + "\t N Columns = " + columnsN + 
					   " of Strings \t Elapsed time = "+ (t2 - t1) + " ms");
 
			// getDaValue on DaReference. No JVM warmup.
		        query = "LCLS//BSA.rootnames.EPICS";
			System.out.println( "\n2. " + query+ "  (M>1xN>1). DaReference. No JVM warmup. Possible Oracle warmup.");
			DaReference r = new DaReference(query, da);
			r.setType(DaValue.Type.STRUCT);
			t1 = System.currentTimeMillis();
			v = r.getDaValue();
			t2 = System.currentTimeMillis();
			columnsN = ((Vector)v).size();
			rowsM = ((Vector)v.get(0)).size();
			System.out.println("M Rows = " + rowsM + "\t N Columns = " + columnsN + 
					   " of Strings \t Elapsed time = "+ (t2 - t1) + " ms");

			// getDaValue on DaReference. No JVM warmup.
		        query = "LCLS//BSA.rootnames.EPICS";
			System.out.println( "\n3. " + query+ "  (M>1xN>1). Compiled DaReference."+
					    " No JVM warmup. Possible Oracle warmup.");
			DaReference rc = new DaReference(query, da);
			rc.setType(DaValue.Type.STRUCT);
			rc.compile(da); // Compile the above DaReference
			t1 = System.currentTimeMillis();
			v = rc.getDaValue();
			t2 = System.currentTimeMillis();
			columnsN = ((Vector)v).size();
			rowsM = ((Vector)v.get(0)).size();
			System.out.println("M Rows = " + rowsM + "\t N Columns = " + columnsN + 
					   " of Strings \t Elapsed time = "+ (t2 - t1) + " ms" );
			da.reset();

		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
		}
		return ;
	}

	// Error handling tests follow. These should not succeed in
	// acquiring data, but rather test appropriate error
	// handling.
	//

	/* Test behaviour given invalid parameter name. superfish is
	 * not a valid parameter.
	 */
	public void Test101()
	{
		try
		{
			System.out.println( "=============================");
			System.out.println( "TEST 101: Bad argument Name - superfish=7");

			String query  = "LCLS//BSA.rootnames.EPICS";
			da.setParam("superfish=7");
			DaValue v = da.getDaValue(query);
			System.out.println( "\t=" + v.toString() );
			da.reset();
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
		}
		return ;
	}
    
        /*
	** Tests to folllow:
	** 1) Test behaviour when passed a bad parameter value of valid
	** parameter name.
	*/
}
