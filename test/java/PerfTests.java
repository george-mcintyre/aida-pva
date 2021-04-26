package test.java;

// The following libraries must me imported for all AIDA clients
// da defines objects used in AIDA clients
// util defines common objects available thoughout the AIDA system

import edu.stanford.slac.aida.lib.da.*;
import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.aida.dp.dpTestHist.*;


/**
 *Aida performance tests.
 * 
 * Each method of this class, <code>Test01, Test02, ...</code> 
 * does some performance test.
 *
 * @see daPerfTest.java
 * @author  Robert C. Sass
 * @version 15-July-2004f Robert C. Sass: Initial Version
 *
 */
public class PerfTests
{
	private DaObject da;

	public PerfTests (String [] args)
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

 
	public void run(int testNumber, String datafile, String comment )
	{
		try 
		{
			switch ( testNumber )
			{
				case 1:
					Test01(datafile, comment);
					break;

				case 2:
					Test02(datafile, comment);
					break;

				case 3:
					Test03(datafile, comment);
					break;

				case 4:
					Test04(datafile, comment);
					break;

				case 5:
					Test05(datafile, comment);
					break;

				case 6:
					Test06(datafile, comment);
					break;
				
				case 7:
					Test07(datafile, comment);
					break;

				case 8:
					Test08(datafile, comment);
					break;

				case 9:
					Test09(datafile, comment);
					break;

				case 10:
					Test10(datafile, comment);
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

				case 19:
					Test19();
					break;

				default:
					System.out.println( "No such test number parameter" + testNumber + "=\"" + testNumber + "\"" );
					System.out.println( "usage: java daPerfTest <test_number> ..." );
					System.out.println( "Valid tests are:" );
					System.out.println( "\t1 = Simple and by Reference Atomic gets of Int and Double to the test server:" );

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
	 * Simple get of a double. 
	 */
	public void Test01(String datafile, String comment)
	{
		try
		{
		    int i,j;

		    final int OUTLOOP = 900;
		    final int INLOOP =1;
		    final double INLOOPD = INLOOP;
		    double tms;
		    Double data;
		    double[] times = new double[OUTLOOP*INLOOP];

		    System.out.println( "TEST 01: Simple double get.");
		    System.out.println( "=============================");
		    Matfile mf = new Matfile (datafile, "Simple da.get of test server double", comment );
		    for (i = 0; i<OUTLOOP; i++)
		    {
                        long t0 = System.currentTimeMillis();
			for (j = 0; j < INLOOP ; j++ )
			    data = (Double)da.get("TEST//VAL", DaValue.Type.DOUBLE);
			long t1 = System.currentTimeMillis();
			tms = t1-t0;
			times[i] = tms/INLOOPD;  // Save time
		    }
		    // Write times to matlab file. Ignore first time. 
		    for (i = 0; i < OUTLOOP*INLOOP; i++)
			mf.print (times[i]);
		    mf.println();
		    mf.close();
		}
		catch (Exception e)
		{
		    System.err.println( e.getMessage() + " exception in test01");
		}
		return;
	}
	/**
	 ** Simple get of a long
	 */
	public void Test02(String datafile, String comment)
	{
		try
		{
		    int i,j;

		    final int OUTLOOP = 900;
		    final int INLOOP = 1;
		    final double INLOOPD = INLOOP;
		    double tms;
		    double[] times = new double[OUTLOOP*INLOOP];
		    Long data;

		    System.out.println( "TEST 02: Simple long get.");
		    System.out.println( "=============================");
		    Matfile mf = new Matfile (datafile, "Simple da.get of test server long", comment );
		    for (i = 0; i<OUTLOOP; i++)
		    {
                        long t0 = System.currentTimeMillis();
			for (j = 0; j < INLOOP ; j++ )
			    data = (Long)da.get("TEST//VAL", DaValue.Type.LONG);
			long t1 = System.currentTimeMillis();
			tms = t1-t0;
			times[i] = tms/INLOOPD;  // Save time
		    }
		    // Write times to matlab file. Ignore first time. 
		    for (i = 0; i < OUTLOOP*INLOOP; i++)
			mf.print (times[i]);
		    mf.println();
		    mf.close();
		}
		catch (Exception e)
		{
		    System.err.println( e.getMessage() + " exception in test02");
		}
		return;
	}

	/**
	 ** Get of Double by reference to the server.
	 */
	public void Test03(String datafile, String comment)
	{
		try
		{
		    int i,j;

		    final int OUTLOOP = 900;
		    final int INLOOP = 1;
		    final double INLOOPD = INLOOP;
		    double[] times = new double[OUTLOOP*INLOOP];
		    double tms;
		    Double data;

		    System.out.println( "TEST 03: Get Double by reference.");
		    System.out.println( "=============================");
		    Matfile mf = new Matfile (datafile, "Get double by reference", comment );
		    DaReference r = new DaReference("TEST//VAL", da);
		    r.setType(DaValue.Type.DOUBLE);
		    for (i = 0; i<OUTLOOP; i++)
		    {
                        long t0 = System.currentTimeMillis();
			for (j = 0; j < INLOOP ; j++ )
			    data = (Double)r.get();
			long t1 = System.currentTimeMillis();
			tms = t1-t0;
			times[i] = tms/INLOOPD;  // Save time
		    }
		    // Write times to matlab file. Ignore first time. 
		    for (i = 0; i < OUTLOOP*INLOOP; i++)
			mf.print (times[i]);
		    mf.println();
		    mf.close();
		}
		catch (Exception e)
		{
		    System.err.println( e.getMessage() + " exception in test03");
		}
		return;
	} //End of Test03

	/**
	 ** Get of Long by reference to the server.
	 */
	public void Test04(String datafile, String comment)
	{
		try
		{
		    int i,j;

		    final int OUTLOOP = 900;
		    final int INLOOP = 1;
		    final double INLOOPD = INLOOP;
		    double[] times = new double[OUTLOOP*INLOOP];
		    double tms;
		    Long data;

		    System.out.println( "TEST 04: Get Long by reference.");
		    System.out.println( "=============================");
		    Matfile mf = new Matfile (datafile, "Get long by reference", comment );
		    DaReference r = new DaReference("TEST//VAL", da);
		    r.setType(DaValue.Type.LONG);
		    for (i = 0; i<OUTLOOP; i++)
		    {
                        long t0 = System.currentTimeMillis();
			for (j = 0; j < INLOOP ; j++ )
			    data = (Long)r.get();
			long t1 = System.currentTimeMillis();
			tms = t1-t0;
			times[i] = tms/INLOOPD;  // Save time
		    }
		    // Write times to matlab file. Ignore first time. 
		    for (i = 0; i < OUTLOOP*INLOOP; i++)
			mf.print (times[i]);
		    mf.println();
		    mf.close();
		}
		catch (Exception e)
		{
		    System.err.println( e.getMessage() + " exception in test04");
		}
		return;
	} //End of Test04


	/**
	 ** Get small data struct by reference using DaValue.
	 */
	public void Test05(String datafile, String comment)
	{
		try
		{
		    int i,j;

		    final int OUTLOOP = 900;
		    final int INLOOP = 1;
		    final double INLOOPD = INLOOP;
		    double[] times = new double[OUTLOOP*INLOOP];
		    double tms;
		    String datastring;

		    System.out.println( "TEST 05: Get small struct by reference using DaValue.");
		    System.out.println( "=============================");
		    Matfile mf = new Matfile (datafile, "Get small struct by reference using DaValue", comment );
		    DaReference r = new DaReference("TEST//VAL", da);
                    DaValue v = new DaValue();
		    for (i = 0; i<OUTLOOP; i++)
		    {
                        long t0 = System.currentTimeMillis();
			for (j = 0; j < INLOOP ; j++ )
			{
			    v = r.getDaValue();
			    datastring = v.toString();
			}
			long t1 = System.currentTimeMillis();
			tms = t1-t0;
			times[i] = tms/INLOOPD;  // Save time
		    }
		    // Write times to matlab file. Ignore first time. 
		    for (i = 0; i < OUTLOOP*INLOOP; i++)
			mf.print (times[i]);
		    mf.println();
		    mf.close();
		}
		catch (Exception e)
		{
		    System.err.println( e.getMessage() + " exception in test05");
		}
		return;
	} //End of Test05


	/**
	 ** Get IDL data struct into same IDL struct on client using DaReference.
	 */
	public void Test06(String datafile, String comment)
	{
		try
		{
		    int i,j;

		    final int OUTLOOP = 900;
		    final int INLOOP = 1;
		    final double INLOOPD = INLOOP;
		    double[] times = new double[OUTLOOP*INLOOP];
		    double tms;
		    histData data;

		    System.out.println( "TEST 06: Get IDL data struct into same IDL struct on client using DaReference.");
		    System.out.println( "=============================");
		    Matfile mf = new Matfile (datafile, "Get IDL data struct into same IDL struct on client using DaReference.", comment );
		    DaReference r = new DaReference("TEST//VAL.HIST", da);
		    for (i = 0; i<OUTLOOP; i++)
		    {
                        long t0 = System.currentTimeMillis();
			for (j = 0; j < INLOOP ; j++ ) 
			    data = histDataHelper.extract(r.getAny());
			long t1 = System.currentTimeMillis();
			tms = t1-t0;
			times[i] = tms/INLOOPD;  // Save time
		    }
		    // Write times to matlab file. Ignore first time. 
		    for (i = 0; i < OUTLOOP*INLOOP; i++)
			mf.print (times[i]);
		    mf.println();
		    mf.close();
		}
		catch (Exception e)
		{
		    System.err.println( e.getMessage() + " exception in test06");
		}
		return;
	} //End of Test06


	/**
	 ** Get varying arrays of longs by reference to the server.
	 */
	public void Test07(String datafile, String comment)
	{
	    try
	    {
		int na,i,j,t;

		final int OUTLOOP = 10;
		final int INLOOP = 3;
		final int NARRAYS = 6;
		final double INLOOPD = INLOOP;
		double[] times = new double[OUTLOOP*INLOOP*NARRAYS];
		double tms;
		int[] data;

		System.out.println( "TEST 07: Get arrays of longs by reference.");
		System.out.println( "=============================");
		Matfile mf = new Matfile (datafile, "Get 10, 100, 1000, 10000, 100000, 1000000 long arrays by reference", comment );
		// Make a DaReference for each data size we get 10 bytes - 1 Mbyte
		DaReference r10 = new DaReference("TEST//VAL", da);
		DaReference r100 = new DaReference("TEST//VAL", da);
		DaReference r1000 = new DaReference("TEST//VAL", da);
		DaReference r10000 = new DaReference("TEST//VAL", da);
		DaReference r100000 = new DaReference("TEST//VAL", da);
		DaReference r1000000 = new DaReference("TEST//VAL", da);
		r10.setParam("SIZE", "10");
		r100.setParam("SIZE", "100");
		r1000.setParam("SIZE", "1000");
		r10000.setParam("SIZE", "10000");
		r100000.setParam("SIZE", "100000");
		r1000000.setParam("SIZE", "1000000");
		r10.compile(da);
		r100.compile(da);
		r1000.compile(da);
		r10000.compile(da);
		r100000.compile(da);
		r1000000.compile(da);
		r10.setType(DaValue.Type.LONGA);
		r100.setType(DaValue.Type.LONGA);
		r1000.setType(DaValue.Type.LONGA);
		r10000.setType(DaValue.Type.LONGA);
		r100000.setType(DaValue.Type.LONGA);
		r1000000.setType(DaValue.Type.LONGA);
		r10.compile(da);
		r100.compile(da);
		r1000.compile(da);
		r10000.compile(da);
		r100000.compile(da);
		r1000000.compile(da);
		for (na =0, t = 0; na<NARRAYS; na++)
		{
		    for (i = 0; i<OUTLOOP; i++)
		    {
			long t0 = System.currentTimeMillis();
			for (j = 0; j < INLOOP ; j++ )
			{
			    switch ( na )
			    {
				case 0:
				    data = r10.get_longa();
				    break;
				case 1:
				    data = r100.get_longa();
				    break;
				case 2:
				    data = r1000.get_longa();
				    break;
				case 3:
				    data = r10000.get_longa();
				    break;
				case 4:
				    data = r100000.get_longa();
				    break;
				case 5:
				    data = r1000000.get_longa();
				    break;
				default:
				    System.out.println ("No such array in Test06. Programmer to be shot.");
			    }
			long t1 = System.currentTimeMillis();
			tms = t1-t0;
			times[t++] = tms;  // Save time
			}
		    }
		}
		// Write times to matlab file. Don't ignore first time. 
		for (i = 0; i < OUTLOOP*INLOOP*NARRAYS; i++)
		    mf.print (times[i]);
		mf.println();
		mf.close();
	    }
	    catch (Exception e)
	    {
		System.err.println( e.getMessage() + " exception in test07");
	    }
	    return;
	}  //End of Test07


	/**
	 ** Batch test
	 */
	public void Test08(String datafile, String comment)
	{
	    try
	    {
		int i,j;

		final int OUTLOOP = 5000;
		final int INLOOP = 1;
		final double INLOOPD = INLOOP;
		double[] times = new double[OUTLOOP*INLOOP];
		double tms;
		histData data;
		String outdata;

		System.out.println( "TEST 08: Run batch tests doing gets by reference to test and testhist servers.");
		System.out.println( "=============================");
		Matfile mf = new Matfile (datafile, "Batch tests by reference.", comment);
		DaReference rh = new DaReference("TEST//VAL.HIST", da);
		DaReference r = new DaReference("TEST//VAL", da);
                DaValue v = new DaValue();
		for (i = 0; i<OUTLOOP; i++)
		{
                    long t0 = System.currentTimeMillis();
		    for (j = 0; j < INLOOP ; j++ )
		    { 
			data = histDataHelper.extract(rh.getAny());
			v = r.getDaValue();
			outdata = v.toString();
		    }
		    long t1 = System.currentTimeMillis();
		    tms = t1-t0;
		    times[i] = tms/INLOOPD;  // Save time
		}
		// Write times to matlab file. Ignore first time. 
		for (i = 0; i < OUTLOOP*INLOOP; i++)
		    mf.print (times[i]);
		mf.println();
	        mf.close();
	    }
	    catch (Exception e)
	    {
		System.err.println( e.getMessage() + " exception in test08");
	    }
	    return;
       	} //End of Test08

	/**
	 ** Time get of SLC data
	 */
	public void Test09(String datafile, String comment)
	{
	    try
	    {
		int i,j;

		final int OUTLOOP = 900;
		final int INLOOP = 1;
		final double INLOOPD = INLOOP;
		double[] times = new double[OUTLOOP*INLOOP];
		double tms;
		Float data;

		System.out.println( "TEST 09: Get SLC float.");
		System.out.println( "=============================");
		Matfile mf = new Matfile (datafile, "Get SLC float", comment );
		DaReference r = new DaReference("XCOR:LI03:120//LEFF", da);
		r.setType(DaValue.Type.FLOAT);
		for (i = 0; i<OUTLOOP; i++)
		{
                    long t0 = System.currentTimeMillis();
		    for (j = 0; j < INLOOP ; j++ )
			data = (Float) r.get();
		    long t1 = System.currentTimeMillis();
		    tms = t1-t0;
		    times[i] = tms/INLOOPD;  // Save time
		}
		// Write times to matlab file. Ignore first time. 
		for (i = 0; i < OUTLOOP*INLOOP; i++)
		    mf.print (times[i]);
		mf.println();
		mf.close();
	    }
	    catch (Exception e)
	    {
		System.err.println( e.getMessage() + " exception in test09");
	    }
	    return;
	} //End of Test09

	
	/**
	 ** Get EPICS double
	 */
	public void Test10(String datafile, String comment)
	{
	    try
	    {
		int i,j;

		final int OUTLOOP = 900;
		final int INLOOP = 1;
		final double INLOOPD = INLOOP;
		double[] times = new double[OUTLOOP*INLOOP];
		double tms;
		Double data;

		System.out.println( "TEST 10: Get EPICS double.");
		System.out.println( "=============================");
		Matfile mf = new Matfile (datafile, "Get EPICS double", comment );
		DaReference r = new DaReference("PR00:ASTS:CH117//VAL", da);
		r.setType(DaValue.Type.DOUBLE);
		for (i = 0; i<OUTLOOP; i++)
		{
		    System.out.println ("Enter test 10");
                    long t0 = System.currentTimeMillis();
		    for (j = 0; j < INLOOP ; j++ )
			{
			data = (Double) r.get();
			System.out.println ("Val= " + data);
			}
		    long t1 = System.currentTimeMillis();
		    tms = t1-t0;
		    times[i] = tms/INLOOPD;  // Save time
		}
		// Write times to matlab file. Ignore first time. 
		for (i = 0; i < OUTLOOP*INLOOP; i++)
		    mf.print (times[i]);
		mf.println();
		mf.close();
	    }
	    catch (Exception e)
	    {
		System.err.println( e.getMessage() + " exception in test10");
	    }
	    return;
	} //End of Test 10

    /**
     */
	public void Test11()
	{
		try
		{		
			System.out.println( "TEST 11: Later");
		}
		catch (Exception e)
		{
		}
		return ;
	}

    /**
     */
	public void Test12()
	{
		try
		{
			System.out.println( "TEST 12: Later");
		}
		catch (Exception e)
		{
			;
		}
		return;
	}

    /**
     */
	public void Test13()
	{
		try
		{
			System.out.println( "TEST 13: Later");
		}
		catch (Exception e)
		{
			;
		}
		return;
	}

	/**
	 */
	public void Test14()
	{
		try
		{
			System.out.println( "TEST 14: Later");
		}
		catch (Exception e)
		{
			;
		}
		return;
	}

	/**
	 */
	public void Test15()
	{
		try
		{
			System.out.println( "TEST 15: Later");
		}
		catch (Exception e)
		{
			;
		}
		return;
	}

	/**
	 */
	public void Test16()
	{
		try
		{
			System.out.println( "TEST 16: Later");
		}
		catch (Exception e)
		{
			;
		}
		return;
	}

	/**
	 */
	public void Test17()
	{
		try
		{
			System.out.println( "TEST 17: Later");
		}
		catch (Exception e)
		{
			;
		}
		return;
	}

	public void Test18()
	{
	    return;
	}

	public void Test19()
	{
	    return;
	}
}
