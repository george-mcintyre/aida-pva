package test.java;

import java.util.*;                               // Vector
import java.lang.*;
import edu.stanford.slac.aida.lib.da.*;           // AIDA API
import edu.stanford.slac.aida.lib.util.common.*;  // DaValue


/**
 * The DpSlcBpm AIDA Data Provider test suite. 
 *
 * @see daDpSlcBpmTest
 * @author  Greg White
 * @version 12-Aug-2006 Greg White: Initial Version
 * @version 18-Apr-2008 Bob Hall Added "LCLS_SL2//BPMS BPMD=55" test (now Test01), which uses a LCLS dgrp.
 */
public class DpSlcBpmTests
{
	private DaObject da;

	private static final int HSTA_XONLY = 64;    // 0x00000040
	private static final int HSTA_YONLY = 128;   // 0x00000080
	private static final int STAT_GOOD  = 1;     // 0x00000001
	private static final int STAT_OK    = 2;     // 0x00000002
	private static final int STAT_OFF   = 8;     // 0x00000008
	private static final int STAT_BAD   = 256;   // 0x00000100
	

	public DpSlcBpmTests(String[] args)
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
			default:
				System.out.println( "No such test number parameter " + 
						    testNumber + "=\"" + testNumber + "\"" );
				System.out.println( "usage: java daDpSlcBpmTest " +
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
	** Simplest BPM acquisition, default options, and simply print using toString();
	** This test uses a LCLS BPM dgrp rather than a PEPII one.
	*/
	public void Test01()
	{
		try
		{
			System.out.println( "=============================");
			System.out.println( "\nTEST 01: get all values of all BPMs of "+
			       "LCLS dgrp under meas def 55, default options\n\n");

			da.setParam("BPMD=55");            /* Required parameter */
			DaValue v = da.getDaValue("LCLS_SL2//BPMS");
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
	** Simplest BPM acquisition, default options, and simply print using toString();
	*/
	public void Test02()
	{
		try
		{
			System.out.println( "=============================");
			System.out.println( "\nTEST 02: get all values of all BPMs of "+
			       "P2BPMHER dgrp under meas def 38, default options\n\n");

			da.setParam("BPMD=38");            /* Required parameter */
			DaValue v = da.getDaValue("P2BPMHER//BPMS");
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
	** Get all BPM values (names, values, hstas etc), from given
	** DGRP, with optional argument for sortorder. Extract
	** individual value vectors and convert each element of each
	** vector to native type one at a time.
	*/
	public void Test03()
	{
		try
		{
			String query;
		
			System.out.println( "=============================");
			System.out.println( "\nTEST 03: get all values of all BPMs of "+
					    "P2BPMHER dgrp, under meas def 38\n" +
				            "SORTORDER=1 (by dgrp z) ");

			query  = "P2BPMHER//BPMS";
			da.setParam("BPMD=38");             /* Required parameter */
			da.setParam("N=1024");
			da.setParam("SORTORDER=1");         /* 1 is "Z" order */

			DaValue v = da.getDaValue(query);   /* Acquire BPM data */
			printBpmDaValue(v);

			da.reset();
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
		}
		return ;
	}


	/*
	** Tests getting the difference orbit to the GOLD config (GOLD). 
	*/
	public void Test04()
	{
		try
		{
			System.out.println( "=============================");
			System.out.println( "\nTEST 04: get the diff to the GOLD orbit of "+
                                            "all BPMs of P2BPMHER dgrp, under meas def 38\n");

			String query  = "P2BPMHER//BPMS";
			da.setParam("BPMD=38");             /* Required parameter */
			da.setParam("N=1024");
			da.setParam("CNFTYPE=GOLD");        /* Return diff to the GOLD orbit */
 
			DaValue v = da.getDaValue(query);   /* Acquire BPM data */
			printBpmDaValue(v);

			da.reset();
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
		}
		return ;
	}


	/*
	** Tests getting the difference orbit to the most recently loaded bpm config (LOADED).
	** That config may have been loaded by any process, such as a SCP. 
	*/
	public void Test05()
	{
		try
		{
			System.out.println( "=============================");
			System.out.println( "\nTEST 05: get the diff to the most recently "+
                                            "loaded config, for all BPMs of P2BPMHER dgrp, "+
                                            "under meas def 38\n");

			String query  = "P2BPMHER//BPMS";
			da.setParam("BPMD=38");             /* Required parameter */
			da.setParam("N=1024");
			da.setParam("CNFTYPE=LOADED");      /* Return diff to most recently */
 
			DaValue v = da.getDaValue(query);   /* Acquire BPM data */
			printBpmDaValue(v);

			da.reset();
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
		}
		return ;
	}

	/*
	** Tests getting the difference orbit to a given SCRATCH config. In the case of 
        ** cnftype SCRATCH, NORMAL, or TEMPORARY, there is a further required parameter - the
        ** config number of the file in the directory given by the cnftype.   
	*/
	public void Test06()
	{
		try
		{
			System.out.println( "=============================");
			System.out.println( "\nTEST 06: get the diff to SCRATCH "+
                                            "config #16386, for all BPMs of P2BPMHER dgrp, "+
                                            "under meas def 38\n");

			String query  = "P2BPMHER//BPMS";
			da.setParam("BPMD=38");             /* Required parameter */
			da.setParam("N=1024");
			da.setParam("CNFTYPE=SCRATCH");     
			da.setParam("CNFNUM=16386");
 
			DaValue v = da.getDaValue(query);   /* Acquire BPM data */
			printBpmDaValue(v);

			da.reset();
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
		}
		return ;
	}

	/*
	** Tests getting the difference orbit to a given NORMAL config. In the case of 
        ** cnftype SCRATCH, NORMAL, or TEMPORARY, there is a further required parameter - the
        ** config number of the file in the directory given by the cnftype.   
	*/
	public void Test07()
	{
		try
		{
			System.out.println( "=============================");
			System.out.println( "\nTEST 07: get the diff to NORMAL "+
                                            "config #4, for all BPMs of P2BPMHER dgrp, "+
                                            "under meas def 38\n");

			String query  = "P2BPMHER//BPMS";
			da.setParam("BPMD=38");             /* Required parameter */
			da.setParam("N=1024");
			da.setParam("CNFTYPE=NORMAL");     
			da.setParam("CNFNUM=1262");         /* "Walter md full in" */
 
			DaValue v = da.getDaValue(query);   /* Acquire BPM data */
			printBpmDaValue(v);

			da.reset();
		}
		catch (Exception e)
		{
			System.err.println( e.toString() );
		}
		return ;
	}


	/* 
	** Simplest BPM acquisition, via DaReference
	*/
	public void Test08()
	{
		try
		{
			System.out.println( "=============================");
			System.out.println( "\nTEST 08: get all values of all BPMs of "+
			       "P2BPMHER dgrp under meas def 38, default options, via DaReference\n\n");

			da.setParam("BPMD=38");            /* Required parameter */

			DaReference r = new DaReference("P2BPMHER//BPMS", da);
			r.setType(DaValue.Type.STRUCT);
			DaValue v = r.getDaValue();
			System.out.println( v.toString() );
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

			String query  = "P2BPMHER//BPMS";
			da.setParam("BPMD=38");
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
	** Test behaviour when passed a bad parameter value of valid
	** parameter name (SORTORDER=0). SORTORDER must be 1 or 2.
	*/
	public void Test102()
	{
		try
		{
			System.out.println( "=============================");
			System.out.println( "TEST 102: Bad argument value - SORTORDER=0");

			String query  = "P2BPMHER//BPMS";
			da.setParam("BPMD=38");
			da.setParam("SORTORDER=0");
			DaValue v = da.getDaValue(query);
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
	** Test behaviour when passed a bad CNFNUM. Examples of bad are
	** 0 (there is never one and thisis trapped specially), or a CNFNUM
        ** that doesn't exist.
	*/
	public void Test103()
	{
		try
		{
			System.out.println( "=============================");
			System.out.println( "TEST 103: Bad argument value - invalid CNFNUM");

			String query  = "P2BPMHER//BPMS";
			da.setParam("BPMD=38");
			da.setParam("CNFTYPE=NORMAL");
			da.setParam("CNFNUM=9999");
			DaValue v = da.getDaValue(query);
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
	** Test behaviour given an invalid parameter value for the
	** given DGRP. The DGRP is the query instance for a BPM
	** acquisition (in this case P2BPMHER), so BPMD=39 is
	** incompatible because 39 is a LER measurement definition
	*/
	public void Test104()
	{
		try
		{
			System.out.println( "=============================");
			System.out.println( "TEST 104: BPMD not valid for DGRP");

			String query  = "P2BPMHER//BPMS";
			da.setParam("BPMD=39");          /* 39 is a LER measurement def. */
			da.setParam("SORTORDER=1");
			DaValue v = da.getDaValue(query);
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
	** Prints the data in a given DaValue assuming it's SLC Bpm data
	*/
	private void printBpmDaValue( DaValue v )
	{
		Vector namesvec = (Vector)v.get(0); /* Extract 1st to get length */
		int Mbpm = namesvec.size();         /* Number of Bpms is len of vec */
		String[] names = new String[Mbpm];
		namesvec.toArray(names);
		Float[] xvals = new Float[Mbpm];
		((Vector)v.get(1)).toArray(xvals);
		Float[] yvals = new Float[Mbpm];
		((Vector)v.get(2)).toArray(yvals);
		Float[] zvals = new Float[Mbpm];
		((Vector)v.get(3)).toArray(zvals);
		Float[] tmits = new Float[Mbpm];
		((Vector)v.get(4)).toArray(tmits);
		Long[] hstas = new Long[Mbpm];
		((Vector)v.get(5)).toArray(hstas);
		Long[] stats = new Long[Mbpm];
		((Vector)v.get(6)).toArray(stats);

		for (int i=0; i<Mbpm; i++ )
		{				
			int hsta = hstas[i].intValue();
			int  stat = stats[i].intValue();
			String plane = 
				((hsta & HSTA_XONLY) == HSTA_XONLY) ? "xonly" : 
				((hsta & HSTA_YONLY) == HSTA_YONLY) ? "yonly" : "xandy";

			// STAT_OK (so STAT_GOOD|STAT_OK) might be enough for some apps
			// but the most crude check is simply whether STAT_GOOD is on.
			String status = ( ((stat & STAT_GOOD)>0) && 
				         !(((stat & STAT_OFF)>0) || ((stat & STAT_BAD)>0)) ) ?
				"good" : "bad";
			
			System.out.println( names[i] +"\t"+ 
					    xvals[i] +"\t"+ 
					    yvals[i] +"\t"+ 
					    zvals[i] +"\t"+ 
					    tmits[i] +"\t"+
					    Integer.toHexString(hsta) + "\t" +
					    plane + "\t"+
					    Integer.toHexString(stat) + "\t" +
					    status );
		}
	}		
}
