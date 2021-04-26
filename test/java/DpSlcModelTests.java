package test.java;

import edu.stanford.slac.aida.lib.da.*;           // AIDA API
import edu.stanford.slac.aida.lib.util.common.*;  // DaValue


/**
 * The DpSlcModel AIDA Data Provider test suite. 
 *
 * @see daDpSlcModelTest
 * @author  Greg White
 * @version 18-May-2004 Greg white: Initial Version
 */
public class DpSlcModelTests
{
	private DaObject da;
	
	public DpSlcModelTests(String[] args)
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

			case 7:
				Test07();
				break;

			case 8:
				Test08();
				break;

			case 9:
				Test09();
				break;

			case 10:
				Test10();
				break;

			case 11:
				Test11();
				break;

			case 12:
				Test12();
				break;

			case 101:
				Test101();
				break;

			case 102:
				Test102();
				break;

			case 103:
				Test103();
				break;

			default:
				System.out.println( "No such test number parameter " + 
						    testNumber + "=\"" + testNumber + "\"" );
				System.out.println( "usage: java daDpSlcModelTest " +
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


	private void Test01()
	{
		try
		{
			String query;
		
			System.out.println( "TEST 01: get TWISS params into DaValue");
			System.out.println( "=============================");

			query  = "BPMS:PR02:8032//twiss";
			da.setParam("MODE=7");
			da.setParam("DGRP=NULL");
			DaValue v = da.getDaValue(query);
			System.out.println( "\t=" + v.toString() );

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
			Throwable ce = e.getCause();
			if ( ce != null ) 
				System.err.println("\tcaused by " + ce.getMessage() );
		}
		da.reset();
		return ;
	}

	private void Test02()
	{
		try
		{
			String query;
		
			System.out.println( "TEST 02: get TWISS params into Float[]");
			System.out.println( "=============================");

			Float[] twiss;
			query  = "BPMS:PR02:8032//twiss";
			da.setParam("MODE=7");
			da.setParam("DGRP=NULL");
			twiss = (Float[])da.geta(query, DaValue.Type.FLOATA);
			
			for (int i = 0;i<twiss.length;i++)
				System.out.println( "v[" + i + "]=" + twiss[i] );

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
			Throwable ce = e.getCause();
			if ( ce != null ) 
				System.err.println("\tcaused by " + ce.getMessage() );
		}
		da.reset();
		return ;
	}


	private void Test03()
	{
		System.out.println( "TEST 03: get TWISS params into Double[] with DaReference");
		System.out.println( "=============================");
		try
		{
			DaReference r = new DaReference("BPMS:PR02:8032//twiss");				
			r.setParam("MODE","7");
			r.setParam("DGRP","NULL");
			r.compile(da);
			r.setType(DaValue.Type.DOUBLEA);
			Double [] v = (Double [])da.geta(r);					

			for (int i=0; i<v.length; i++)
				System.out.println( "v[" + i + "]=" + v[i] );
                        
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
			Throwable ce = e.getCause();
			if ( ce != null ) 
				System.err.println("\tcaused by " + ce.getMessage() );
		}
		da.reset();
		return ;
	}

	private void Test04()
	{
		
		System.out.println( "TEST 04: get R-matrix into DaValue");
		System.out.println( "=============================");
		try
		{
			String query;
		
			query  = "BPMS:PR02:8032//R";
			da.setParam("MODE=7");
			da.setParam("DGRP=NULL");
			DaValue v = da.getDaValue(query);
			System.out.println( "\t=" + v.toString() );

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
			Throwable ce = e.getCause();
			if ( ce != null ) 
				System.err.println("\tcaused by " + ce.getMessage() );
		}
		da.reset();
		return ;
	}

	private void Test05()
	{
		
		System.out.println( "TEST 05: get R-matrix into DaValue, non-NULL DGRP");
		System.out.println( "=============================");
		try
		{
			String query;
		
			query  = "XCOR:LI03:400//R";
			da.setParam("MODE=1");        // See top left of SCP Model Diagnostics 
			// da.setParam("DGRP=NDR_ELEC"); // Panel
			DaValue v = da.getDaValue(query);
			System.out.println( "\t=" + v.toString() );

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
			Throwable ce = e.getCause();
			if ( ce != null ) 
				System.err.println("\tcaused by " + ce.getMessage() );
		}
		da.reset();
		return ;
	}

	private void Test06()
	{
		
		System.out.println( "TEST 06: get Twiss into DaValue, non-NULL DGRP");
		System.out.println( "=============================");
		try
		{
			String query;
		
			query  = "XCOR:LI03:400//twiss";
			da.setParam("MODE=1");        // See top left of SCP Model Diagnostics 
			da.setParam("DGRP=ELECEP01"); // Panel
			DaValue v = da.getDaValue(query);
			System.out.println( "\t=" + v.toString() );

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
			Throwable ce = e.getCause();
			if ( ce != null ) 
				System.err.println("\tcaused by " + ce.getMessage() );
		}
		da.reset();
		return ;
	}

	private void Test07()
	{
		
		System.out.println( "TEST 07: get R-matrix into Double array");
		System.out.println( "=============================");
		try
		{
			String query;
		
			query  = "XCOR:LI03:400//R";
			da.setParam("MODE=1");        // See top left of SCP Model Diagnostics 
			// da.setParam("DGRP=NDR_ELEC"); // Panel
			Double[] R = (Double[]) da.geta(query, DaValue.Type.DOUBLEA);
			for (int i = 0;i<R.length;i++)
				System.out.println( "R["+((int)Math.floor(i/6)+1)+
						     ","+((int)(i%6.0+1))+"]=" + R[i] );

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
			Throwable ce = e.getCause();
			if ( ce != null ) 
				System.err.println("\tcaused by " + ce.getMessage() );
		}
		da.reset();
		return ;
	}


	private void Test08()
	{
		
		System.out.println( "TEST 08: get R-matrix into Float[], no DGRP");
		System.out.println( "=============================");
		try
		{
			String query;
		
			query  = "BPMS:PR02:8032//R";
			da.setParam("MODE=7");
			Float[] R = (Float[]) da.geta(query, DaValue.Type.FLOATA);
			for (int i = 0;i<R.length;i++)
				System.out.println( "R["+((int)Math.floor(i/6)+1)+
						     ","+((int)(i%6.0+1))+"]=" + R[i] );



			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
			Throwable ce = e.getCause();
			if ( ce != null ) 
				System.err.println("\tcaused by " + ce.getMessage() );
		}
		da.reset();
		return ;
	}

	private void Test09()
	{
		
		System.out.println( "TEST 09: get R-matrix from A to B into Double array." +
				    "A is upstream of B. Giving LCLS MODE");

		try
		{
			Double[] R;                    // The R matrix from A to B computed.
			String A = "BPMS:IA20:221//R"; // Device A
			String B = "QUAD:IM20:961";    // Device B 

			da.setParam("B="+B); 
			da.setParam("MODE=5");         // LCLS e- Twiss mode

			System.out.println( "Query = " + A + da.toStringArgs() );

			R = (Double[]) da.geta(A, DaValue.Type.DOUBLEA);
			for (int i = 0;i<R.length;i++)
				System.out.println( "R["+((int)Math.floor(i/6)+1)+
						     ","+((int)(i%6.0+1))+"]=" + R[i] );

			/* Cant specify POS for A to B until SLC Model server adapted.
			System.out.println( A + "to middle of "+ B );
                        // da.setParam("POS=MID");
			R = (Double[]) da.geta(A, DaValue.Type.DOUBLEA);
			for (int i = 0;i<R.length;i++)
				System.out.println( "R["+((int)Math.floor(i/6)+1)+
						     ","+((int)(i%6.0+1))+"]=" + R[i] );

			System.out.println( A + "to end of "+ B );
                        da.setParam("POS=END");
			R = (Double[]) da.geta(A, DaValue.Type.DOUBLEA);
			for (int i = 0;i<R.length;i++)
				System.out.println( "R["+((int)Math.floor(i/6)+1)+
						     ","+((int)(i%6.0+1))+"]=" + R[i] );
			*/
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
			Throwable ce = e.getCause();
			if ( ce != null ) 
				System.err.println("\tcaused by " + ce.getMessage() );
		}
		da.reset();
		return ;
	}

		
	private void Test10()
	{
		System.out.println( "TEST 10: get R-matrix from A to B into Double array.");
		System.out.println( "         A downstream of B. Not giving MODE.");
		try
		{

			Double[] R;                    // The R matrix from A to B computed.
			String A = "QUAD:IM20:961//R"; // Device and R attribute of A
			String B = "BPMS:IA20:221";    // Device B
			da.setParam("B="+B); 

			System.out.println( "Query = " + A + da.toStringArgs() );
			R = (Double[]) da.geta(A, DaValue.Type.DOUBLEA);

			for (int i = 0;i<R.length;i++)
				System.out.println( "R["+((int)Math.floor(i/6)+1)+
						     ","+((int)(i%6.0+1))+"]=" + R[i] );

		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
			Throwable ce = e.getCause();
			if ( ce != null ) 
				System.err.println("\tcaused by " + ce.getMessage() );
		}
		da.reset();
		return ;
	}


	private void Test11()
	{
       		System.out.println( "TEST 11: get R-matrix from A to B into Double array.");
		System.out.println( "         without supplying MODE (use default LCLS MODE)");
		try
		{

			Double[] R;                    // The R matrix from A to B computed.
			String A = "QUAD:IM20:361//R"; // Device and R attribute of A
			String B = "BPMS:IA20:511";    // Device B

			da.setParam("B="+B); 
			System.out.println( "Query = " + A + da.toStringArgs() );

			R = (Double[]) da.geta(A, DaValue.Type.DOUBLEA);

			for (int i = 0;i<R.length;i++)
				System.out.println( "R["+((int)Math.floor(i/6)+1)+
						     ","+((int)(i%6.0+1))+"]=" + R[i] );

		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
			Throwable ce = e.getCause();
			if ( ce != null ) 
				System.err.println("\tcaused by " + ce.getMessage() );
		}
		da.reset();
		return ;
	}

	private void Test12()
	{
		System.out.println( "TEST 12: Speed tests\n");
		try
		{
			final int N = 100;
			long t1=0, t2=0;
			double sumT=0;
			double [] T = new double [N];
			String query = null;
			DaValue v = null;

			System.out.println( "All tests over " + N + " tries ...");

			// Test 1. Get R matrix.
			query  = "QUAD:LM24:713//R";
			da.setParam("MODE","5");
			da.setParam("TYPE","DATABASE");
			da.setParam("POS=BEG");
			System.out.printf( query + " TYPE=DATABASE, POS=BEG:" );
			sumT=0;
			for (int i = 0; i<N; i++)
			{
				t1 = System.currentTimeMillis();
				v = da.getDaValue(query);
				t2 = System.currentTimeMillis();
				T[i] = t2-t1;
				sumT+=T[i];
			}
			// Statistics
			System.out.println( "\t1st " + T[0] +
					    "\tMean " + sumT/N + 
					    "\tMedian = " + PerformanceUtilities.median(T) );
			// System.out.println( "\t=" + v.toString() );
					    
			// Test 2. Get Twiss.
			query  = "QUAD:LM24:713//twiss";
			da.setParam("TYPE","DATABASE");
			da.setParam("POS=BEG");
			System.out.printf( query + " TYPE=DATABASE, POS=BEG: ");
			sumT=0;
			for (int i = 0; i<N; i++)
			{
				t1 = System.currentTimeMillis();
				v = da.getDaValue(query);
				t2 = System.currentTimeMillis();
				T[i] = t2-t1;
				sumT+=T[i];
			}
			// Statistics
			System.out.println( "\t1st " + T[0] +
					    "\tMean " + sumT/N + 
					    "\tMedian = " + PerformanceUtilities.median(T) );
			// System.out.println( "\t=" + v.toString() );

		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
			Throwable ce = e.getCause();
			if ( ce != null ) 
				System.err.println("\tcaused by " + ce.getMessage() );
		}
		da.reset();
		System.out.println( "=============================");
		return ;
	}

	// Error handling tests follow. These should not succeed in acquiring data, but
	// rather test appropriate error handling.
	// 
	private void Test101()
	{
		try
		{
			String query;
		
			System.out.println( "TEST 101: Bad argument Name - superfish=7");
			System.out.println( "=============================");

			query  = "BPMS:PR02:8032//twiss";
			da.setParam("superfish=7");
			DaValue v = da.getDaValue(query);
			System.out.println( "\t=" + v.toString() );

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
			Throwable ce = e.getCause();
			if ( ce != null ) 
				System.err.println("\tcaused by " + ce.getMessage() );
		}
		da.reset();
		return ;
	}

	private void Test102()
	{
	    System.out.println( "TEST 102: Test meaningful message when"+
				"device not in default mode 5, and MODE arg not given");
	    System.out.println( "=============================");

		try
		{
			String query  = "BPMS:PR02:8032//twiss";
			// da.setParam("MODE=7");
			da.setParam("DGRP=NULL");
			System.out.println( "Query = "+ query + da.toStringArgs());
 
			DaValue v = da.getDaValue(query);

			System.out.println( "\t=" + v.toString() );
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
			Throwable ce = e.getCause();
			if ( ce != null ) 
				System.err.println("\tcaused by " + ce.getMessage() );
		}
		da.reset();
		return ;
	}

	private void Test103()
	{
       		System.out.println( "TEST 101: Test illegal argument combination: POS "+
				    "given when getting R-matrix from A to B");
		try
		{

			Double[] R;                    // The R matrix from A to B computed.
			String A = "QUAD:IM20:361//R"; // Device and R attribute of A
			String B = "BPMS:IA20:511";    // Device B

			da.setParam("B="+B); 
			da.setParam("POS=MID");
			System.out.println( "Query = " + A + da.toStringArgs() );

			R = (Double[]) da.geta(A, DaValue.Type.DOUBLEA);

			for (int i = 0;i<R.length;i++)
				System.out.println( "R["+((int)Math.floor(i/6)+1)+
						     ","+((int)(i%6.0+1))+"]=" + R[i] );

		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
			Throwable ce = e.getCause();
			if ( ce != null ) 
				System.err.println("\tcaused by " + ce.getMessage() );
		}
		da.reset();
		return ;
	}

}
