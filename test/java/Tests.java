package test.java;

// The following libraries must me imported for all AIDA clients
// da defines objects used in AIDA clients
// util defines common objects available thoughout the AIDA system
import edu.stanford.slac.aida.lib.da.*;
import edu.stanford.slac.aida.lib.util.common.*;
// dpChads is needed for the tests using the real Archiver data provider.
import edu.stanford.slac.aida.dp.dpChads.*;

/**
 * The general Aida test and demo suite.
 * 
 * Each method of this class, <code>Test01, Test02, ...</code> 
 * tests and demonstrates some API function of Aida.
 *
 * @see daAIDATest
 * @author  George McIntyre
 * @version 07-Oct-2004, Greg White (greg): Made Test 10 set params on DaObject, to test fix.
 * @version 01-Oct-2004, Greg White (greg): Removed comments on test 11 & 12
 * claiming those tests exhibit a bug, that bug is fixed.
 * @version 21-May-2004, Greg White (greg): Fix test 5 so that time
 * range and interval spec give subset.
 * @version 21-May-2004, Bob Hall (rdh): Changed "attribute" part of
 * PV specification for Chads requests * to be "HIST.subsystem" (where
 * subsystem is pepii, nlcta, or pack) instead of "HIST".
 * @version 30-Apr-2004, Bob Hall (rdh): Modified Test 11 to also
 * fetch history data for NLCTA and 8-pack PVs.
 * @version 28-Jan-2003, Ron MacKenzie (ronm): Reduce test 13 to 100
 * iterations because fetching real history data.
 * @version 07-Jan-2003, Ron MacKenzie (ronm): Output entire sequences
 * returned in test 11.
 * @version 12-Nov-2002, Ron MacKenzie: Change import of except from
 * aida to package except
 * @version 25-Jun-2002 George McIntyre: Initial Version
 *
 */
public class Tests
{
	private DaObject da;

	public Tests (String [] args)
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
	 * Gets double value. Tests simplest acqusition. 
	 * Demos <code>Double v = (Double)da.get("TEST//VAL", DaValue.Type.DOUBLE);</code>
	 */
	public void Test01()
	{
		try
		{
			System.out.println( "TEST 01: Use the DaObject (DaServer) to "+
					    "get a Double Value");
			System.out.println( "=============================");

			System.out.println( "da.get(\"TEST//VAL\", DaValue.Type.DOUBLE)" );
			Double v = (Double)da.get("TEST//VAL", DaValue.Type.DOUBLE);	
			System.out.println( "\t=" + v);

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println("TEST 01:" + e.getMessage());
		}
		return;
	}

	/**
	 * Gets float value with reference. Tests simple get with
	 * reference.  Demos simple way to get simple value data fast:
	 * <code>Float v =
	 * (Float)da.get(DaReference("TEST//VAL"));</code>
	 */
	public void Test02()
	{
		try
		{
			System.out.println( "TEST 02: Use a DaReference (reference to " + 
					    "an AIDA object) to get a Float Value");
			System.out.println( "=============================");

			System.out.println( "DaReference r = DaReference(\"TEST//VAL\", da)"  );
			DaReference r = new DaReference("TEST//VAL", da);

			System.out.println( "r.setType(DaValue.Type.FLOAT)" );
			r.setType(DaValue.Type.FLOAT);

			System.out.println( "da.get(r)" );
			Float v = (Float)da.get(r);				
			System.out.println( "\t=" + v);

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println("TEST 02:" + e.getMessage());
		}
		return;
	}

	/**
	 * Gets simple structured value. Tests DaValue on server to
	 * DaValue on client through DaServer.  Demos simplest way to
	 * get structured data: <code>DaValue v =
	 * da.getDaValue("TEST//VAL")</code>
	 */
	public void Test03()
	{
		try
		{
			String target;
		
			System.out.println( "TEST 03: get structured data (DaValue on " + 
					    "server to DaValue on client");
			System.out.println( "=============================");

			target  = "TEST//VAL";
			System.out.println( "da.getDaValue(\"" + target + "\")" );
			DaValue v = da.getDaValue(target);

			System.out.println( "\t=" + v.toString() );

			System.out.println( "da.reset()" );
			da.reset();

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println( "TEST03 :" + e.toString() );
		}
		return ;
	}

	/**
	 * Gets small data structure by reference. Tests performance
	 * by timing acqusition over 1000 cycles. Demos <code> DaValue =
	 * DaReference.getDaValue() </code>.
	 */
	public void Test04()
	{
		try
		{
			System.out.println( "TEST 04: Performance Test; getDaValue() of small"+ 
					    " aggregate data with reference");
			System.out.println( "=============================");
                
			System.out.println( "DaReference r = DaReference(\"TEST//VAL\", da)"  );
			DaReference r = new DaReference("TEST//VAL", da);
			DaValue v = new DaValue();
			System.out.println( "r.getDaValue()" );
                
			long t0 = System.currentTimeMillis();
			for ( int i = 0; i < 1000 ; i++ )
				v = r.getDaValue();
			long t1 = System.currentTimeMillis();
			long tms = t1-t0;
			long ms = tms/1000;
			long dec = (tms - ((tms/1000)*1000))/10 ;
			System.out.println( "\tAverage time in milliseconds for single "+
					    " getDaValue() =" + ms + "." + dec );
			System.out.println( "\tLast value returned =" + v.toString() );
                
			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println("TEST 04:" + e.getMessage());
		}
		return;
	}

	/**
	 * Gets small data structure, given parameters. Tests parameter parsing. 
	 * Demos <code> DaValue = daObject.getDaValue("TEST//VAL"); </code> 
	 */
	public void Test05()
	{
		try
		{
			System.out.println( "TEST 05: Pass parameters to the Service and " + 
					    "get a DaValue via the DaServer");
			System.out.println( "=============================");

			System.out.println( "da.setParam(\"STARTTIME\", \"2001-09-06 12:04\")" );
			da.setParam("STARTTIME", "2001-09-06 12:04");

			System.out.println( "da.setParam(\"ENDTIME\", \"2001-09-06 12:11\")" );
			da.setParam("ENDTIME", "2001-09-06 12:11");

			System.out.println( "da.getDaValue(\"TEST//VAL\")" );
			DaValue v = da.getDaValue("TEST//VAL");

			System.out.println( "\t=" + v.toString() );

			System.out.println( "da.reset()" );
			da.reset();

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println("TEST 05:" + e.getMessage());
		}
		return;
	}


	/**
	 * Gets small data structure, given parameters, all parameters in one string. Tests 
	 * parameter parsing. Demos <code> daObject.setParam("<parameter-name>=<param-value>"); 
	 * DaValue = daObject.getDaValue("TEST//VAL"); </code> 
	 */
	public void Test06()
	{
		try
		{
			System.out.println( "TEST 06: As TEST 05 but 2 parameters in one " + 
					    "call to setParam()");
			System.out.println( "=============================");

			System.out.println( "da.setParam(\"ENDTIME=2001-09-06 12:10;"+
					    "STARTTIME=2001-09-06 12:04\")" );
			da.setParam("ENDTIME=2001-09-06 12:10;STARTTIME=2001-09-06 12:04");

			System.out.println( "da.getDaValue(\"TEST//VAL\")" );
			DaValue v = da.getDaValue("TEST//VAL");				

			System.out.println( "\t=" + v.toString() );

			System.out.println( "da.reset()" );
			da.reset();

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println("TEST 06:" + e.getMessage());
		}
		return;
	}

	/**
	 * Gets small data structure, given parameters, plus filtering
	 * for only first n values. Tests parameter parsing and
	 * filtering. Demos <code> DaValue =
	 * daObject.getDaValue("TEST//VAL[1-4]"); </code>
	 */
	public void Test07()
	{
		try
		{
		    System.out.println( "FUNCTIONALITY NOT YET IMPLEMENTED!!");
			System.out.println( "TEST 07: Get only sub-array of array data," +
					    " by constant indeces");
			System.out.println( "=============================");

			System.out.println( 
				"da.setParam(\"ENDTIME=2001-09-06 12:10;STARTTIME=2001-09-06 12:04\")" );
			da.setParam("ENDTIME=2001-09-06 12:10;STARTTIME=2001-09-06 12:04");

			System.out.println( "da.getDaValue(\"TEST//VAL[1-4]\")" );
			DaValue v = da.getDaValue("TEST//VAL[1-4]");

			System.out.println( "\t=" + v.toString() );

			System.out.println( "da.reset()" );
			da.reset();

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println("TEST 07:" + e.getMessage()); 
		}
		return;
	}

	/**
	 * Gets all of, plus dynamically evaluated sub-array of, an
	 * array of aggregate data structures. Tests dynamic query
	 * expression evaluation. Demos
	 * <code>DaObject.getDaValue("all={TEST//VAL},
	 * constrained={TEST//VAL[(#LENGTH-4)-]}");</code>
	 */
	public void Test08()
	{
		try
		{
			String target;
		        System.out.println( "FUNCTIONALITY NOT YET IMPLEMENTED!!");
			System.out.println( "TEST 08: Get only sub-array of array data," + 
					    "by evaluated expression");
			System.out.println( "=============================");

			System.out.println( 
				"da.setParam(\"ENDTIME=2001-09-06 12:10;STARTTIME=2001-09-06 12:04\")" );
			da.setParam("ENDTIME=2001-09-06 12:10;STARTTIME=2001-09-06 12:04");

			target  = "all={TEST//VAL}";
			target += ",constrained={TEST//VAL[(#LENGTH-4)-]}";
			System.out.println( "da.getDaValue(\"" + target + "\")" );
			DaValue v = da.getDaValue(target);

			System.out.println( "\t=" + v.toString() );

			System.out.println( "da.reset()" );
			da.reset();

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println("TEST 08:" + e.getMessage());
		}
		return ;
	}

	/**
	 * Gets small data structure by complied reference, given
	 * parameters on the reference object . Tests compiled reference.  Demos <code>
	 * DaObject da; DaReference r = new DaReference("TEST//VAL");
	 * r.compile(da); DaValue = r.getDaValue() </code>
	 */
	public void Test09()
	{
		try
		{
			System.out.println( "TEST 09: getDaValue() aggregate data, "+
					    "with compiled reference");
			System.out.println( "=============================");

			System.out.println( "DaReference r = new DaReference(\"TEST//VAL\")" );
			DaReference r = new DaReference("TEST//VAL");				

			System.out.println( "r.setParam(\"STARTTIME\", \"2001-09-06 12:06\")" );
			r.setParam("STARTTIME", "2001-09-06 12:06");

			System.out.println( "r.setParam(\"ENDTIME\", \"2001-09-06 12:10\")" );
			r.setParam("ENDTIME", "2001-09-06 12:10");

			System.out.println( "r.compile(da)" );
			r.compile(da);

			System.out.println( "r.getDaValue()" );
			DaValue v = r.getDaValue();					

			System.out.println( "\t=" + v.toString() );

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println("TEST 09:" + e.getMessage());
		}
		return;
	}

	
	/**
	 * Gets array of double, given parameters on the DaObject, by compiled
	 * reference. Tests client side return type determination.  Demos
	 * <code> r.setType(DaValue.Type.DOUBLEA); da.geta(r) </code>
	 */
	public void Test10()
	{
		try
		{
			System.out.println( "TEST 10: geta() to get array of DOUBLE");
			System.out.println( "=============================");

			System.out.println( "DaReference r = new DaReference(\"TEST//VAL\")" );
			DaReference r = new DaReference("TEST//VAL");				

			System.out.println( "da.setParam(\"STARTTIME\",\"2001-09-06 12:07\")" );
			da.setParam("STARTTIME","2001-09-06 12:07");

			System.out.println( "da.setParam(\"ENDTIME\",\"2001-09-06 12:10\")" );
			da.setParam("ENDTIME","2001-09-06 12:10");
						
			System.out.println( "r.compile()" );
			r.compile(da);

			System.out.println( "r.setType(DaValue.Type.DOUBLEA)" );
			r.setType(DaValue.Type.DOUBLEA);

			System.out.println( "Double [] v = (Double [])da.geta(r)" );
			Double [] v = (Double [])da.geta(r);					

			for (int i=0; i<v.length; i++)
				System.out.println( "v[" + i + "]=" + v[i] );
                        
			System.out.println( "da.reset()" );
			da.reset();

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println("TEST 10:" + e.getMessage());
		}
		return;
	}


	/**
	 * Gets IDL defined structure on server into DaValue on client,
	 * using DaObject (ie via DaServer). Tests a client's ability to
	 * interact with a DaValue on client side, even when server
	 * returns unknown structure defined by CORBA Any. Demos <code>
	 * DaValue v = da.getDaValue("TEST//VAL.HIST"); </code>
	 */
	public void Test11()
	{
		try
		{
			String target;
		
			System.out.println( "TEST 11: get IDL struct on server into"+ 
					    " DaValue on client");
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
			System.out.println("TEST 11:" + e.getMessage());
		}
		return ;
	}


	/**
	 * Gets IDL defined structure on server into DaValue on client,
	 * using DaReference (ie skipping DaServer).
	 */
	public void Test12()
	{
		try
		{
			System.out.println( "TEST 12: get IDL struct on server into DaValue "+
					    "on client, with reference.");
			System.out.println( "=============================");

			System.out.println( 
				"DaReference r = DaReference(\"TEST//VAL.HIST\", da)"  );
			DaReference r = new DaReference("TEST//VAL.HIST", da);

			System.out.println( "r.getDaValue()" );
			DaValue v = r.getDaValue();					

			System.out.println( "\t=" + v.toString() );

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println("TEST 12:" + e.getMessage());
		}
		return;
	}

	/**
	 * Gets IDL struct on server into same IDL struct on client, using
	 * DaReference, given parameters.  This function acquires real
	 * History Data for 3 Archiver variables. Tests extraction of
	 * arbitrary CORBA Any from * DaReference object. Demos <code>
	 * histData f = histDataHelper.extract(r.getAny()); </code>
	 */
	public void Test13()
	{
		try
		{
			System.out.println( "TEST 13: get IDL struct on server into same IDL"+
					    " struct on client for real PV History.");
			System.out.println( "=============================");


			// Test 12 now retrieves EPICS Channel Archiver data for
			// three PVS: a PEPII PV, a NLCTA PV, and a 8-Pack PV.
			// The first PV is a PEPII PV.
			System.out.println ( "Get EPICS Channel Archiver data for a PEPII PV");
			System.out.println ( 
				"DaReference r = DaReference(\"HR81:CAV1:VACM//HIST.pepii\", da)"  );
			DaReference r = new DaReference("HR81:CAV1:VACM//HIST.pepii", da);
			// HR23:CAV1:VACM//HIST also works
         
			System.out.println( 
				"r.setParam (STARTTIME, 2003-02-01 12:20)");

			r.setParam("STARTTIME", "02/01/2003 12:20:00");

			System.out.println(
				"r.setParam (ENDTIME, 2003-02-01 12:40)");

			r.setParam("ENDTIME", "02/01/2003 12:40:00");

			// I guess we don't need to compile like we do in
			// tests above.  This is from a test above.
			// System.out.println( "r.compile(da)" );
			// r.compile(da);

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

			// System.out.println( "\tf.d=" + f.d[0] );
			// System.out.println( "\tf.s=" + f.s[0] );

			// The second PV is a NLCTA PV.
			System.out.println ( "Get EPICS Channel Archiver data for a NLCTA PV");

			System.out.println ( 
				"DaReference r1 = DaReference(\"TRS1:KLY:OUT:PAVE//HIST.nlcta\", da)"  );
			DaReference r1 = new DaReference("TRS1:KLY:OUT:PAVE//HIST.nlcta", da);

			System.out.println( 
				"r1.setParam (STARTTIME, 2004-04-30 10:00)");

			r1.setParam("STARTTIME", "04/30/2004 10:00:00");

			System.out.println(
				"r1.setParam (ENDTIME, 2004-04-30 10:10)");

			r1.setParam("ENDTIME", "04/30/2004 10:10:00");

			System.out.println
				( "histData f1 = histDataHelper.extract(r1.getAny())" );
                        
			histData f1 = histDataHelper.extract(r1.getAny());

			System.out.println( "# entries returned = " + 
					    f1.d.length);
                        
			if (f1.d.length > 0) {
				for ( int ii = 0; ii < f1.d.length ; ii++ ) {
					System.out.println( 
						"\t" + f1.s[ii] + "\t" + f1.d[ii]  + "\t" + 
						f1.repCnt_l[ii] );
				}
			}

			// The third PV is an 8-pack PV.
			System.out.println ( "Get EPICS Channel Archiver data for a 8-Pack PV");
			System.out.println ( "DaReference r2 = DaReference(\"TRS8:TWT56:OUTFWD:VOLT//HIST.pack\", da)"  );
			DaReference r2 = new DaReference("TRS8:TWT56:OUTFWD:VOLT//HIST.pack", da);

			System.out.println( 
				"r2.setParam (STARTTIME, 2004-04-30 06:40)");

			r2.setParam("STARTTIME", "04/30/2004 06:40:00");

			System.out.println(
				"r2.setParam (ENDTIME, 2004-04-30 06:50)");

			r2.setParam("ENDTIME", "04/30/2004 06:50:00");

			System.out.println
				( "histData f2 = histDataHelper.extract(r2.getAny())" );
                        
			histData f2 = histDataHelper.extract(r2.getAny());

			System.out.println( "# entries returned = " + 
					    f2.d.length);
                        
			if (f2.d.length > 0) {
				for ( int ii = 0; ii < f2.d.length ; ii++ ) {
					System.out.println( 
						"\t" + f2.s[ii] + "\t" + f2.d[ii]  + "\t" + 
						f2.repCnt_l[ii] );
				}
			}

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println("TEST 13:" + e.getMessage());
		}
		return;
	}

	/**
	 * Gets IDL struct on server into same IDL struct on client, using
	 * DaReference, given parameters. Tests performance of CORBA Any
	 * acquisition of real history data; prints mean time for one
	 * acquisition over 100 cycles.
	 */
	public void Test14()
	{
		try
		{
			System.out.println( "TEST 14: Performance Test; getAny() of small "+
					    "aggregate data with reference");
			System.out.println( "Doing 100 fetches (not 1000 like other tests) "+
					    "because were fetching real history data from ChanArch");
			System.out.println( "=============================");

			// System.out.println( "DaReference r = DaReference(\"TEST//VAL.HIST\", da)"  );
			// DaReference r = new DaReference("TEST//VAL.HIST", da);

			System.out.println ( "DaReference r = DaReference(\"HR23:CAV1:VACM//HIST.pepii\", da)"  );
			DaReference r = new DaReference("HR23:CAV1:VACM//HIST.pepii", da);

			System.out.println( 
				"r.setParam (STARTTIME, 2003-02-01 12:20)");

			r.setParam("STARTTIME", "02/01/2003 12:20:00");

			System.out.println(
				"r.setParam (ENDTIME, 2003-02-01 12:40)");

			r.setParam("ENDTIME", "02/01/2003 12:40:00");

			System.out.println( "r.getAny()" );
                        
			long t0 = System.currentTimeMillis();
			for ( int i = 0; i < 100 ; i++ )
				r.getAny();
			long t1 = System.currentTimeMillis();

			long tms = t1-t0;
			long ms = tms/100;
			long dec = (tms - ((tms/100)*100))/10 ;
			System.out.println( "\tAverage time in milliseconds for" +
					    "single getAny() =" + ms + "." + dec );

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println("TEST 14:" + e.getMessage());
		}
		return;
	}

	/**
	 * Gets a float using the simplest Aida API function, 1000
	 * times. Tests performance of basic API.
	 */
	public void Test15()
	{
		try
		{
			System.out.println( "TEST 15: Performance Test; get via DaServer:");
			System.out.println( "=============================");

			System.out.println( "da.get(\"TEST//VAL\", DaValue.Type.DOUBLE)" );

			long t0 = System.currentTimeMillis();
			for ( int i = 0; i < 1000 ; i++ )
				da.get("TEST//VAL", DaValue.Type.DOUBLE);
			long t1 = System.currentTimeMillis();
			long tms = t1-t0;
			long ms = tms/1000;
			long dec = (tms - ((tms/1000)*1000))/10 ;
			System.out.println( "\tAverage time in milliseconds for single get()="+ 
					    ms + "." + dec );

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println("TEST 15:" + e.getMessage());
		}
		return;
	}

	/**
	 * Gets a float by reference through daServer 1000 times. Tests
	 * performance of indirect DaReference API method.  Demos <code>
	 * DaObject.get(DaReference("TEST//VAL")); </code>
	 */
	public void Test16()
	{
		try
		{
			System.out.println( "TEST 16: Performance Test; get via DaServer with"+
					    " reference: Float ");
			System.out.println( "=============================");

			System.out.println( "DaReference r = DaReference(\"TEST//VAL\", da)"  );
			DaReference r = new DaReference("TEST//VAL", da);

			System.out.println( "r.setType(DaValue.Type.FLOAT)" );
			r.setType(DaValue.Type.FLOAT);

			System.out.println( "da.get(r)" );
                        
			long t0 = System.currentTimeMillis();
			for ( int i = 0; i < 1000 ; i++ )
				da.get(r);
			long t1 = System.currentTimeMillis();

			long tms = t1-t0;
			long ms = tms/1000;
			long dec = (tms - ((tms/1000)*1000))/10 ;
			System.out.println( "\tAverage time in milliseconds for single get()="+ 
					    ms + "." + dec );

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println("TEST 15:" + e.getMessage());
			;
		}
		return;
	}

	/**
	 * Gets a float by reference directly to Data Provider 1000
	 * times. Tests performance of direct DaReference API method.
	 * Demos <code> DaReference.get("TEST//VAL"); </code>
	 */
	public void Test17()
	{
		try
		{
			System.out.println( "TEST 17: Performance Test; get direct to data "+
					    "provider with reference: Float ");
			System.out.println( "=============================");

			System.out.println( "DaReference r = DaReference(\"TEST//VAL\", da)"  );
			DaReference r = new DaReference("TEST//VAL", da);

			System.out.println( "r.setType(DaValue.Type.FLOAT)" );
			r.setType(DaValue.Type.FLOAT);

			System.out.println( "r.get()" );
                        
			long t0 = System.currentTimeMillis();
			for ( int i = 0; i < 1000 ; i++ )
				r.get();
			long t1 = System.currentTimeMillis();

			long tms = t1-t0;
			long ms = tms/1000;
			long dec = (tms - ((tms/1000)*1000))/10 ;
			System.out.println( "\tAverage time in milliseconds for single get()="+ 
					    ms + "." + dec );

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println("TEST 17:" + e.getMessage());
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
