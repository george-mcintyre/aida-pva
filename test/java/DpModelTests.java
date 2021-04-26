package test.java;

import edu.stanford.slac.aida.lib.da.*;           // AIDA API
import edu.stanford.slac.aida.lib.util.common.*;  // DaValue

/**
 * The DpModel AIDA Data Provider test suite. 
 * 
 * Run these tests (after setting up the aida development environment) with 
 * aidatest daDpModelTest 1 2 3 4 5 6 (or any combination of these test units)
 *
 * @see <a
 * href="http://www.slac.stanford.edu/grp/cd/soft/aida/newLabourCheatsheet.html">Setting
 * the Aida development environment</a>
 * @see daDpModelTest for client "runner" of these tests
 * @author  Greg White
 * @version 21-Jul-2008 Greg white: Initial Version
 */
public class DpModelTests
{
	private DaObject da;
	
	public DpModelTests(String[] args)
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
                        case 13:
                                Test13();
                                break;
                        case 14:
                                Test14();
                                break;
                        case 15:
                                Test15();
                                break;
                        case 16:
                                Test16();
                                break;
			case 17:
				Test17();
				break;
                        case 18:
                                Test18();
                                break;
                        case 25:
                                Test25();
                                break;
                        case 26:
                                Test26();
                                break;
                        case 27:
                                Test27();
                                break;
                        case 28:
                                Test28();
                                break;
                        case 29:
                                Test29();
                                break;
                        case 30:
                                Test30();
                                break;
                        case 31:
                                Test31();
                                break;
                        case 32:
                                Test32();
                                break;
                        case 33:
                                Test33();
                                break;
                        case 34:
                                Test34();
                                break;
                        case 35:
                                Test35();
                                break;
                        case 36:
                                Test36();
                                break;
                        case 37:
                                Test37();
                                break;
                        case 38:
                                Test38();
                                break;
                        case 39:
                                Test39();
                                break;
                        case 40:
                                Test40();
                                break;
                        case 41:
                                Test41();
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
                        case 104:
                                Test104();
                                break;
                        case 201:
                                Test201();
                                break;
                        case 202:
                                Test202();
                                break;
                        case 1001:
                                Test1001();
                                break;
			default:
				System.out.println( "No such test number parameter " + 
						    testNumber + "=\"" + testNumber + "\"" );
				System.out.println( "usage: java daDpModelTest " +
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
		System.out.println( "TEST 01: get TWISS params into DaValue");
		System.out.println( "Giving TYPE=DESIGN, MODE=5");
		System.out.println( "Req: twiss of GOLD, DESIGN, MODE 5\n");
		try
		{
			String q = "BPMS:IN20:235//twiss";
			da.setParam("TYPE","DESIGN");
			da.setParam("MODE","5");
			System.out.println("Query = " + q + da.toStringArgs() );

			DaValue v = da.getDaValue(q);

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
		System.out.println( "=============================");
		return ;
	}

	private void Test02()
	{
		System.out.println( "TEST 02: get TWISS params into Float[]");
		System.out.println( "   Giving MODE=5, TYPE=EXTANT");
		System.out.println( "   Req: twiss GOLD, EXTANT, MODE 5");
		try
		{
			Float[] twiss;
			String query  = "BPMS:IN20:235//twiss";
			da.setParam("MODE","5");
			da.setParam("TYPE","EXTANT");
			System.out.println("Query = " + query + da.toStringArgs() );
			twiss = (Float[])da.geta(query, DaValue.Type.FLOATA);
			
			for (int i = 0;i<twiss.length;i++)
				System.out.println( "v[" + i + "]=" + twiss[i] );

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


	private void Test03()
	{
		System.out.println( "TEST 03: get Gus Spect device TWISS params into "+
				    "Double[] with compiled DaReference");
		System.out.println( "  Giving MODE=51");
		System.out.println( "  Require: twiss GOLD, EXTANT, MODE 51");
		try
		{
			String query = "BPMS:IN20:821//twiss";
			DaReference r = new DaReference(query); 
			da.setParam("MODE","51");
			r.compile(da);
			r.setType(DaValue.Type.DOUBLEA);
			System.out.println("Query = " + query + da.toStringArgs() );

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
		System.out.println( "=============================");
		return ;
	}


	private void Test04()
	{
		System.out.println( "TEST 04: get EXTANT (aka DATABASE) R-matrix "+
				    "from MIDdle of QUAD into DaValue");
		System.out.println( "  Giving POS=MID, TYPE=EXTANT");
		System.out.println( "  Require: MID R GOLD, EXTANT, MODE 5");
		try
		{
			String query  = "QUAD:IN20:361//R";
			da.setParam("TYPE","EXTANT");
			da.setParam("POS=MID");
			System.out.println("Query = " + query + da.toStringArgs() );

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
		System.out.println( "=============================");
		return ;
	}



	private void Test06()
	{
		System.out.println( "TEST 06: get TWISS params into DaValue");
		System.out.println( "Giving TYPE=DESIGN");
		System.out.println( "Req: twiss GOLD, DESIGN, MODE 5\n");
		try
		{
			String query = "BPMS:IN20:235//twiss";
			da.setParam("TYPE","DESIGN");
			System.out.println("Query = " + query + da.toStringArgs() );
			
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
		System.out.println( "=============================");
		return ;
	}

	private void Test07()
	{
		System.out.println( "TEST 07: get TWISS params into Float[]");
		System.out.println( "   Giving TYPE=DESIGN");
		System.out.println( "   Req: twiss GOLD, DESIGN, MODE 5");
		try
		{
			Float[] twiss;
			String query  = "BPMS:IN20:235//twiss";
			da.setParam("TYPE","DESIGN");
			System.out.println("Query = " + query + da.toStringArgs() );

			twiss = (Float[])da.geta(query, DaValue.Type.FLOATA);
			
			for (int i = 0;i<twiss.length;i++)
				System.out.println( "v[" + i + "]=" + twiss[i] );

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


	private void Test08()
	{
		System.out.println( "TEST 08: get TWISS params into Double[] with \n" +
				    " compiled DaReference");
		System.out.println( "  Giving TYPE=DESIGN");
		System.out.println( "  Require: twiss GOLD, DESIGN, MODE 5");
		try
		{
			String query = "BPMS:IN20:235//twiss";
			DaReference r = new DaReference(query); 
			r.setParam("TYPE","DESIGN");
			r.compile(da);
			r.setType(DaValue.Type.DOUBLEA);
			System.out.println("Query = " + query + da.toStringArgs() );

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
		System.out.println( "=============================");
		return ;
	}



	private void Test09()
	{
		System.out.println( "TEST 09: get EXTANT (aka DATABASE) R-matrix "+
		                    "of QUAD in GunSpect line (mode 51) only");
		System.out.println( "  Require: R (defaul POS) GOLD, "+
				    "EXTANT, MODE 51");
		try
		{
			String query = "QUAD:IN20:831//R";
			System.out.println("Query = " + query + da.toStringArgs() );

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
		System.out.println( "=============================");
		return ;
	}

	private void Test10()
	{
		System.out.println( "TEST 10: get EXTANT (aka DATABASE) twiss "+
		                    "of BPMS in GunSpect line (mode 51) only");
		System.out.println( "  Require: twiss GOLD, EXTANT, MODE 51");
		try
		{
			String query = "BPMS:IN20:821//twiss"; 
			System.out.println("Query = " + query + da.toStringArgs() );

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
		System.out.println( "=============================");
		return ;
	}


	private void Test11()
	{
		System.out.println( "TEST 11: get TWISS params into DaValue\n\n");
		System.out.println( "Giving RUNID");
		System.out.println( "Req: twiss w/ given RUNID\n");
		try
		{
			String q = "BPMS:IN20:235//twiss";
			da.setParam("RUNID","2202");
			System.out.println("Query = " + q + da.toStringArgs() );

			DaValue v = da.getDaValue(q);

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
		System.out.println( "=============================");
		return ;
	}

	private void Test12()
	{
		System.out.println( "TEST 12: get Rmat of device into DaValue\n\n");
		System.out.println( "Giving RUNID");
		System.out.println( "Req: Device Rmat w/ given RUNID\n");
		try
		{
			String q = "BPMS:IN20:235//R";
			da.setParam("RUN","2202");
			System.out.println("Query = " + q + da.toStringArgs());

			DaValue v = da.getDaValue(q);

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
		System.out.println( "=============================");
		return ;
	}

	private void Test13()
	{
		System.out.println( "TEST 13: "+
				    "Get GOLD DESIGN TWISS params into DaValue");
		try
		{
			String query = "BPMS:IN20:235//twiss";
			da.setParam("TYPE","DESIGN");
			da.setParam("RUN=GOLD");
			System.out.println("Query = " + query + da.toStringArgs() );
			
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
		System.out.println( "=============================");
		return ;
	}

	private void Test14()
	{
		System.out.println( "TEST 14: "+
				    "Get expicitly LATEST TWISS params into DaValue");
		try
		{
			String query = "BPMS:IN20:235//twiss";
			da.setParam("TYPE","DESIGN");
			da.setParam("RUN=LATEST");
			System.out.println("Query = " + query + da.toStringArgs() );
			
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
		System.out.println( "=============================");
		return ;
	}

	private void Test15()
	{
		System.out.println( "TEST 15: get TWISS params into DaValue");
		System.out.println( "Giving RUN = GOLD, and explicit MODE=5");
		try
		{
			String q = "BPMS:IN20:235//twiss";
			da.setParam("RUN","GOLD");
			da.setParam("MODE","5");
			System.out.println("Query = " + q + da.toStringArgs() );

			DaValue v = da.getDaValue(q);

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
		System.out.println( "=============================");
		return ;
	}

	private void Test16()
	{
		System.out.println( "TEST 16: get GOLD EXTANT R-matrix "+
		                    "of QUAD in GunSpect line (mode 51) only");
		try
		{
			String query = "QUAD:IN20:831//R";
			da.setParam("RUN=GOLD");
			System.out.println("Query = " + query + da.toStringArgs() );

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
		System.out.println( "=============================");
		return ;
	}

	private void Test17()
	{
		System.out.println( "TEST 17: get EXTANT (aka DATABASE) R-matrix "+
				    "from MIDdle of QUAD into DaValue");
		System.out.println( "  RUN = LATEST, Giving POS=MID, TYPE=EXTANT");
		System.out.println( "  Require: MID R w/ max RUNID, EXTANT, MODE 5");
		try
		{
			String query  = "QUAD:IN20:361//R";
			da.setParam("TYPE","EXTANT");
			da.setParam("POS=MID");
			da.setParam("RUN","LATEST");
			System.out.println("Query = " + query + da.toStringArgs() );

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
		System.out.println( "=============================");
		return ;
	}


	private void Test18()
	{
		System.out.println( "TEST 18: get LATEST EXTANT R-matrix "+
		                    "of QUAD in GunSpect line (mode 51) only");
		try
		{
			String query = "QUAD:IN20:831//R";
			da.setParam("RUN","LATEST");
			System.out.println("Query = " + query + da.toStringArgs() );

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
		System.out.println( "=============================");
		return ;
	}


	private void Test25()
	{
		
		System.out.println( "TEST 25: get R-matrix from A to B, "+
				    "A upsream of B (nominal case); ");
		System.out.println( "Giving RUNID");
		System.out.println( "Req: rmat from model of runid:");
		try
		{
			String B = "BPMS:LI27:301";    // Device B
			String A = "BPMS:IN20:235//R"; // Device A
                        String runid = "2801";         // Specify a modelling run.
			Double[] R;                    // The R matrix from A to B computed.

			da.setParam("B="+B); 
			da.setParam("RUNID="+runid);
			System.out.println("Query = " + A + da.toStringArgs());
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
		System.out.println( "=============================");
		return ;
	}

	private void Test26()
	{
		
		System.out.println( "TEST 26: get R-matrix from A to B, "+
				    "A upsream of B (nominal case); ");
		System.out.println( "Giving non-latest RUNID");
		System.out.println( "Req: rmat from model of runid:");
		try
		{
			String B = "BPMS:LI27:301";    // Device B
			String A = "BPMS:IN20:235//R"; // Device A
                        String runid = "2202";         // Specify a modelling run.
			Double[] R;                    // The R matrix from A to B computed.

			da.setParam("B="+B); 
			da.setParam("RUNID="+runid);
			System.out.println("Query = " + A + da.toStringArgs());
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
		System.out.println( "=============================");
		return ;
	}

	private void Test27()
	{
		
		System.out.println( "TEST 27: get R-matrix from A to B, "+
				    "A DOWNSTREAM of B; ");
		System.out.println( "Giving RUNID");
		System.out.println( "Req: rmat from model of runid:");
		try
		{
			String A = "BPMS:LI27:301//R"; // Device A
			String B = "BPMS:IN20:235"; // Device B
                        String runid = "2801";         // Specify a modelling run.
			Double[] R;                    // The R matrix from A to B computed.

			da.setParam("B="+B); 
			da.setParam("RUNID="+runid);
			System.out.println("Query = " + A + da.toStringArgs());
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
		System.out.println( "=============================");
		return ;
	}

	private void Test28()
	{
		
		System.out.println( "TEST 08: get R-matrix from A to B, "+
				    "A DOWNSTREAM of B; ");
		System.out.println( "Giving RUNID");
		System.out.println( "Req: rmat from model of runid + warning TYPE is ignored:");
		try
		{
			String A = "BPMS:LI27:301//R"; // Device A
			String B = "BPMS:IN20:235"; // Device B
                        String runid = "2202";         // Specify a modelling run.
			Double[] R;                    // The R matrix from A to B computed.

			da.setParam("B="+B); 
			da.setParam("RUNID="+runid);
			da.setParam("TYPE=DESIGN");
			System.out.println("Query = " + A + da.toStringArgs());
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
		System.out.println( "=============================");
		return ;
	}

	private void Test29()
	{
		
		System.out.println( "TEST 29: get R-matrix from A to B, "+
				    "A upstream of B (nominal case); ");
		System.out.println( "Giving MODE 5. NOT Giving RUNID");
		System.out.println( "Req: rmat from GOLD model in MODE:");
		try
		{
			String q = "BPMS:IN20:235//R";
			da.setParam("B=BPMS:LI27:301"); 
			da.setParam("MODE=5");
			da.setParam("RUN=LATEST");
			System.out.println("Query = " + q + da.toStringArgs());
			Double[] R = (Double[]) da.geta(q, DaValue.Type.DOUBLEA);

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
		System.out.println( "=============================");
		return ;
	}

	private void Test30()
	{
		
		System.out.println( "TEST 30: get R-matrix from A to B, "+
				    "A upstream of B (nominal case); ");
		System.out.println( "Giving MODE, TYPE=DESIGN. NOT Giving RUNID");
		System.out.println( "Req: rmat from GOLD DESIGN of MODE 5:");
		try
		{
			String q = "BPMS:IN20:235//R";
			da.setParam("B=BPMS:LI27:301"); 
			da.setParam("MODE=5");
			da.setParam("TYPE=DESIGN");
			System.out.println("Query = " + q + da.toStringArgs());
			Double[] R = (Double[]) da.geta(q, DaValue.Type.DOUBLEA);

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
		System.out.println( "=============================");
		return ;
	}

	private void Test31()
	{
		
		System.out.println( "TEST 31: get R-matrix from A to B, "+
				    "A upstream of B (nominal case); ");
		System.out.println( "Giving non-mode = 5 MODE. NOT Giving RUNID");
		System.out.println( "Req: rmat from GOLD EXTANT MODE 52 model");
		try
		{
			String q = "BPMS:IN20:235//R";
			da.setParam("B=BPMS:IN20:925"); 
			da.setParam("MODE=52");
			System.out.println("Query = " + q + da.toStringArgs());

			Double[] R = (Double[]) da.geta(q, DaValue.Type.DOUBLEA);

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
		System.out.println( "=============================");
		return ;
	}

	private void Test32()
	{
		
		System.out.println( "TEST 32: get R-matrix from A to B, "+
				    "A is DOWNSTREAM of B");
		System.out.println( "Giving MODE. NOT Giving RUNID");
		System.out.println( "Req: rmat B*inv(A), (where A and B are defined"+
				    "as given in args, not by relative position) from "+
				    "model of GOLD EXTANT MODE 5:");
		try
		{
			String q = "BPMS:LI27:301//R";
			da.setParam("B=BPMS:IN20:235"); 
			da.setParam("MODE=5");
			System.out.println("Query = " + q + da.toStringArgs());

			Double[] R = (Double[]) da.geta( q, DaValue.Type.DOUBLEA);
			
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
		System.out.println( "=============================");
		return ;
	}

	private void Test33()
	{
		
		System.out.println( "TEST 33: get R-matrix from A to B, "+
				    "A is DOWNSTREAM of B");
		System.out.println( "Giving non mode-5 A (downstream) device. " + 
				    "NOT Giving RUNID");
		System.out.println( "Req: rmat B*inv(A), (where A and B are defined "+
				    "as given in args, not by relative position) from "+
				    "model of GOLD MODE 52:");
		try
		{
			String q = "BPMS:IN20:925//R";
			da.setParam("B=BPMS:IN20:235"); 
			da.setParam("MODE=52");
			System.out.println("Query = " + q + da.toStringArgs());

			Double[] R = (Double[]) da.geta( q, DaValue.Type.DOUBLEA);
			
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
		System.out.println( "=============================");
		return ;
	}

	private void Test34()
	{
		
		System.out.println( "TEST 34: get R-matrix from A to B, "+
				    "A is upstream of B (nominal)");
		System.out.println( "NOT Giving RUNID; NOT giving MODE "+ 
				    "B (downstream) device IS IN fact in mode 5."); 
		System.out.println( "Req: rmat B*inv(A) from "+
				    "model of GOLD EXTANT MODE 5:");
		try
		{
			String q = "BPMS:IN20:235//R";
			da.setParam("B=BPMS:LI27:301"); 
			da.setParam("RUN=LATEST");
			System.out.println("Query = " + q + da.toStringArgs());

			Double[] R = (Double[]) da.geta( q, DaValue.Type.DOUBLEA);
			
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
		System.out.println( "=============================");
		return ;
	}

	private void Test35()
	{
		
		System.out.println( "TEST 35: get R-matrix from A to B, "+
				    "A is DOWNSTREAM of B");
		System.out.println( "NOT Giving RUNID; NOT giving MODE "+ 
				    "A (downstream) device IS IN fact in mode 5."); 
		System.out.println( "Req: rmat B*inv(A) from "+
				    "model of GOLD EXTANT MODE 5:");
		try
		{
			String q = "BPMS:LI27:301//R";
			da.setParam("B=BPMS:IN20:235"); 
			System.out.println("Query = " + q + da.toStringArgs());

			Double[] R = (Double[]) da.geta( q, DaValue.Type.DOUBLEA);
			
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
		System.out.println( "=============================");
		return ;
	}


	private void Test36()
	{
		
		System.out.println( "TEST 36: get R-matrix from A to B "+
				    "(A is upstream of B)");
		System.out.println( "NOT Giving RUNID; NOT giving MODE "+ 
				    "B (downstream) device IS NOT IN mode 5."); 
		System.out.println( "Req: rmat B*inv(A) from model of GOLD"+ 
				    "EXTANT of any mode containing both:");
		try
		{
			String q = "BPMS:IN20:235//R";
			da.setParam("B=BPMS:IN20:925"); 
			da.setParam("RUN=LATEST");
			System.out.println("Query = " + q + da.toStringArgs());

			Double[] R = (Double[]) da.geta( q, DaValue.Type.DOUBLEA);
			
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
		System.out.println( "=============================");
		return ;
	}

	private void Test37()
	{
		
		System.out.println( "TEST 37: get R-matrix from A to B "+
				    "(A is downstream of B)");
		System.out.println( "NOT Giving RUNID; NOT giving MODE; Giving TYPE=DESIGN "+ 
				    "A (downstream) device IS NOT IN mode 5."); 
		System.out.println( "Req: rmat B*inv(A) from "+
				    "model of GOLD DESIGN run of any mode:");
		try
		{
			String q = "BPMS:IN20:925//R";
			da.setParam("B=BPMS:IN20:235");
			da.setParam("TYPE=DESIGN");
			System.out.println("Query = " + q + da.toStringArgs());

			Double[] R = (Double[]) da.geta( q, DaValue.Type.DOUBLEA);
			
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
		System.out.println( "=============================");
		return ;
	}

	private void Test38()
	{
		
		System.out.println( "TEST 38: get R-matrix from A to B "+
		    "A and B are the same device and A POS is downstream of B POS!");
		System.out.println( "NOT Giving RUNID; NOT giving MODE; Giving TYPE=DESIGN "+ 
		    "device IS NOT IN mode 5."); 
		System.out.println( "Req: rmat B*inv(A) from "+
		    "model of GOLD DESIGN run of any mode:");
		try
		{
			String q = "QUAD:IN20:831//R";
			da.setParam("B=QUAD:IN20:831");
			da.setParam("POS=END");
			da.setParam("POSB=BEGIN");
			da.setParam("TYPE=DESIGN");
			System.out.println("Query = " + q + da.toStringArgs());

			Double[] R = (Double[]) da.geta( q, DaValue.Type.DOUBLEA);
			
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
		System.out.println( "=============================");
		return ;
	}


	private void Test39()
	{
		
		System.out.println( "TEST 39: get R-matrix from A to B, "+
				    "A upstream of B (nominal case); ");
		System.out.println( "Giving MODE. Giving RUN = GOLD");
		System.out.println( "Req: rmat from model of max runid in MODE:");
		try
		{
			String q = "BPMS:IN20:235//R";
			da.setParam("B=BPMS:LI27:301"); 
			da.setParam("MODE=5");
			da.setParam("RUN","GOLD");
			System.out.println("Query = " + q + da.toStringArgs());
			Double[] R = (Double[]) da.geta(q, DaValue.Type.DOUBLEA);

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
		System.out.println( "=============================");
		return ;
	}

	private void Test40()
	{
		
		System.out.println( "TEST 40: get R-matrix from A to B, "+
				    "A is upstream of B (nominal)");
		System.out.println( "RUN = GOLD; NOT giving MODE "+ 
				    "B (downstream) device IS in mode 5."); 
		System.out.println( "Req: rmat B*inv(A) from "+
				    "model of LATEST EXTANT runid in MODE 5:");
		try
		{
			String q = "BPMS:IN20:235//R";
			da.setParam("B=BPMS:LI27:301");
			da.setParam("run","gold");
			System.out.println("Query = " + q + da.toStringArgs());

			Double[] R = (Double[]) da.geta( q, DaValue.Type.DOUBLEA);
			
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
		System.out.println( "=============================");
		return ;
	}

	private void Test41()
	{
		
		System.out.println( "TEST 41: get R-matrix from A to B "+
				    "(A is upstream of B)");
		System.out.println( "RUN = GOLD; NOT giving MODE "+ 
				    "B (downstream) device IS NOT in mode 5."); 
		System.out.println( "Req: rmat B*inv(A) from model of max"+ 
				    "EXTANT runid of any mode containing both:");
		try
		{
			String q = "BPMS:IN20:235//R";
			da.setParam("B=BPMS:IN20:925"); 
			da.setParam("run=gold");
			System.out.println("Query = " + q + da.toStringArgs());

			Double[] R = (Double[]) da.geta( q, DaValue.Type.DOUBLEA);
			
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
			System.out.println( "TEST 101: Bad argument Name - superfish=7\n\n");

			String q = "BPMS:IN20:235//twiss";
      			da.setParam("superfish=7");
			System.out.println("Query = " + q + da.toStringArgs());

			DaValue v = da.getDaValue(q);

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
		System.out.println( "=============================");
		return ;
	}

	private void Test102()
	{
		System.out.println( "TEST 102: get Rmat of device into DaValue\n\n");
		System.out.println( "Giving INVALID RUNID");
		System.out.println( "Req: Error, invalid RUNID\n");
		try
		{
			String q = "BPMS:IN20:235//R";
			da.setParam("RUNID","453452");
			System.out.println("Query = " + q + da.toStringArgs());

			DaValue v = da.getDaValue(q);

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
		System.out.println( "=============================");
		return ;
	}

	private void Test103()
	{
		System.out.println( "TEST 103: get R-matrix "+
		                    "of BPMS:BSY0:52 (which is in 52SL2 (mode 53) only");
		System.out.println( "  Giving MODE=5 (which is wrong)");		
		System.out.println( "  Require: Error msg saying device not in mode");
		try
		{
			String q = "BPMS:BSY0:52//R"; 
			da.setParam("MODE=5");
			System.out.println("Query = " + q + da.toStringArgs());

			DaValue v = da.getDaValue(q);
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
		System.out.println( "=============================");
		return ;
	}

	private void Test104()
	{
		
		System.out.println( "TEST 104: get R-matrix from A to B, "+
				    "A upsream of B (nominal case); ");
		System.out.println( "Giving value invalid Runid");
		System.out.println( "Req: Message that when TYPE & RUNID given "+
				    "- TYPE is ignored, then INVALID RUNID");
		try
		{
			String B = "BPMS:LI27:301";    // Device B
			String A = "BPMS:IN20:235//R"; // Device A
                        String runid = "334223";       // Specify a modelling run.
			Double[] R;                    // The Rmat A to B computed.

			da.setParam("B="+B); 
			da.setParam("RUNID="+runid);
			da.setParam("TYPE=DESIGN");
			System.out.println("Query = " + A + da.toStringArgs());
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
		System.out.println( "=============================");
		return ;
	}

	private void Test201()
	{
		System.out.println( "TEST 201: Get twiss of device into DaValue\n\n");
		System.out.println( "Giving RUNID and TYPE=DESIGN");
		System.out.println( "Req: Device twiss from given RUNID & " +
				    " WARNING that type is ignored when RUNID given.\n");
		try
		{
			String q = "BPMS:IN20:235//R";
			da.setParam("RUNID","2202");
			da.setParam("TYPE","DESIGN");
			System.out.println("Query = " + q + da.toStringArgs());

			DaValue v = da.getDaValue(q);

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
		System.out.println( "=============================");
		return ;
	}

	private void Test202()
	{
		
		System.out.println( "TEST 202: get R-matrix from A to B, "+
				    "A upsream of B (nominal case); ");
		System.out.println( "Giving TYPE & RUNID");
		System.out.println( "Req: Message that when TYPE & RUNID given "+
				    "- TYPE is ignored");
		try
		{
			String B = "BPMS:LI27:301";    // Device B
			String A = "BPMS:IN20:235//R"; // Device A
                        String runid = "2202";         // Specify a modelling run.
			Double[] R;                    // The Rmat A to B computed.

			da.setParam("B="+B); 
			da.setParam("RUNID="+runid);
			da.setParam("TYPE=DESIGN");
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
		System.out.println( "=============================");
		return ;
	}

	private void Test1001()
	{
		System.out.println( "TEST 1001: Speed tests\n");
		try
		{
			final int N = 100;
			long t1=0, t2=0;
			double sumT=0;
			double [] T = new double [N];
			String query = null;

			System.out.println( "All tests over " + N + 
					    " tries, results in ms ...");

			// Test 1. Get R matrix.
			query  = "QUAD:IN20:361//R";
			da.setParam("TYPE","DATABASE");
			da.setParam("POS=BEG");
			System.out.printf( query + " TYPE=DATABASE, POS=BEG:" );
			sumT=0;
			for (int i = 0; i<N; i++)
			{
				t1 = System.currentTimeMillis();
				da.geta(query,DaValue.Type.DOUBLEA);
				t2 = System.currentTimeMillis();
				T[i] = t2-t1;
				sumT+=T[i];
			}
			// Statistics
			System.out.println( "\t1st " + T[0] +
					    "\tMean " + sumT/N + 
					    "\tMedian = " + PerformanceUtilities.median(T) );
					    
			// Test 2. Get Twiss.
			query  = "QUAD:IN20:361//twiss";
			da.setParam("TYPE","DATABASE");
			da.setParam("POS=BEG");
			System.out.printf( query + " TYPE=DATABASE, POS=BEG: ");
			sumT=0;
			for (int i = 0; i<N; i++)
			{
				t1 = System.currentTimeMillis();
				da.geta( query, DaValue.Type.DOUBLEA );
				t2 = System.currentTimeMillis();
				T[i] = t2-t1;
				sumT+=T[i];
			}
			// Statistics
			System.out.println( "\t1st " + T[0] +
					    "\tMean " + sumT/N + 
					    "\tMedian = " + PerformanceUtilities.median(T) );
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


}
