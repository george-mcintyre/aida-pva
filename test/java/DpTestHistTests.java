
package test.java;

import edu.stanford.slac.aida.lib.da.*;
import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.aida.dp.dpTestHist.*;

/**
 * The DpTestHist AIDA Data Provider test suite. Tests acquisition of arbitrary IDL 
 * structure (histData) from Java implemented server. 
 *
 * see daDpTestHistTest
 * @author  Greg White
 * @version 18-May-2004 Greg white: Initial Version
 */
public class DpTestHistTests
{
	private DaObject da;

	public DpTestHistTests(String[] args)
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

				

				default:
					System.out.println( "No such test number parameter" + testNumber + "=\"" + testNumber + "\"" );
					System.out.println( "usage: java daDpTestHistTest <test_number> ..." );
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
			String target;
		
			System.out.println( "TEST 01: get IDL struct on server into DaValue on client");
			System.out.println( "=============================");

			target  = "TEST//VAL.HIST";
			System.out.println( "da.getDaValue(\"" + target + "\")" );
			DaValue v = da.getDaValue(target);

			System.out.println( "\t=" + v.toString() );

			System.out.println( "da.reset()" );
			da.reset();

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			;
		}
		return ;
	}

	private void Test02()
	{
		try
		{
			System.out.println( "TEST 02: get IDL struct on server into DaValue on client, with reference.");
			System.out.println( "=============================");

			System.out.println( "DaReference r = DaReference(\"TEST//VAL.HIST\", da)"  );
			DaReference r = new DaReference("TEST//VAL.HIST", da);

			System.out.println( "r.getDaValue()" );
			DaValue v = r.getDaValue();					

			System.out.println( "\t=" + v.toString() );

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			;
		}
		return;
	}

        
	private void Test03()
	{
		try
		{
			System.out.println( "TEST 03: get IDL struct on server into same IDL struct on client.");
			System.out.println( "=============================");

			System.out.println( 
			   "da.setParam (STARTTIME, 09/06/2003 12:04)");
               da.setParam("STARTTIME", "2003-09-06 12:04:00");

            System.out.println(
			   "da.setParam (ENDTIME, 09/06/2003 12:09)");
			   da.setParam("ENDTIME", "2003-09-06 12:09:00");

  			System.out.println ( "Any = da.getAny(\"TEST//VAL.HIST\")");
			histData f = histDataHelper.extract( da.getAny("TEST//VAL.HIST"));
 
                        System.out.println( "# entries returned = " + 
                                             f.d.length);
                        
                        if (f.d.length > 0) {
			  for ( int ii = 0; ii < f.d.length ; ii++ ) {
                            System.out.println( 
                              "\t" + f.s[ii] + "\t" + f.d[ii]  + "\t" + 
			      f.repCnt_l[ii] );
			  }
			}
                }
		catch (Exception e)
		{
			System.out.println(e.toString());
		}
		return;
	}

	private void Test04()
	{
		try
		{
			System.out.println( "TEST 04: get IDL struct on server into same IDL struct on client by DaReference");
			System.out.println( "=============================");

			System.out.println ( "DaReference r = DaReference(\"TEST//VAL.HIST\", da)"  );
			DaReference r = new DaReference("TEST//VAL.HIST", da);
         
			System.out.println( 
		   "r.setParam (STARTTIME, 2003-09-06 12:04)");
			r.setParam("STARTTIME", "2003-09-06 12:04:00");

                        System.out.println(
		    "r.setParam (ENDTIME, 2003-09-06 12:09)");
			r.setParam("ENDTIME", "2003-09-06 12:09:00");

			System.out.println
			  ( "histData f = histDataHelper.extract(r.getAny())" );
			histData f = histDataHelper.extract(r.getAny());

                        System.out.println( "# entries returned = " + 
                                             f.d.length);
                        if (f.d.length > 0) {
			  for ( int ii = 0; ii < f.d.length ; ii++ ) {
                            System.out.println( 
                              "\t" + f.s[ii] + "\t" + f.d[ii]  + "\t" + 
			      f.repCnt_l[ii] );
			  }
			}
                }
		catch (Exception e)
		{
			System.out.println(e.toString());
		}
		return;
	}


	private void Test05()
	{
		try
		{
			System.out.println( "TEST 05: get IDL struct on server into Double array");
			System.out.println( "with parameters by DaObject");
			System.out.println( "=============================");


			System.out.println( "da.setParam (STARTTIME, 2003-09-06 12:04)");
			da.setParam("STARTTIME", "2003-09-06 12:04:00");

			System.out.println("da.setParam (ENDTIME, 2003-09-06 12:09)");
			da.setParam("ENDTIME", "2003-09-06 12:09:00");

			Double[] hist = (Double[]) da.geta("TEST//VAL.HIST", DaValue.Type.DOUBLEA);

			if (hist.length > 0) {
				for ( int ii = 0; ii < hist.length ; ii++ ) {
					System.out.println("\thist[" + ii + "] = " + hist[ii]);
				}
			}
			da.reset();
                }
		catch (Exception e)
		{
			System.out.println(e.toString());
		}
		return;
	}
}
