package test.java;

import java.util.*;                               // Vector
import java.lang.*;
import edu.stanford.slac.aida.lib.da.*;           // AIDA API
import edu.stanford.slac.aida.lib.util.common.*;  // DaValue


/**
 * The DpSlcBuff AIDA Data Provider test suite. 
 *
 * @see daDpSlcBuffTest
 * @author  Greg White
 * @version 12-Aug-2006 Greg White: Initial Version
 * @version 18-Apr-2008 Bob Hall Added "LCLS_SL2//BPMS BPMD=55" test (now Test01), which uses a LCLS dgrp.
 */
public class DpSlcBuffTests
{
	private DaObject da;

	private static final int STAT_GOOD  = 1;     // 0x00000001
	private static final int STAT_OK    = 2;     // 0x00000002
	private static final int STAT_OFF   = 8;     // 0x00000008
	private static final int STAT_BAD   = 256;   // 0x00000100
	

	public DpSlcBuffTests(String[] args)
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
			case 101:
				Test101();
				break;				
			case 102:
				Test102();
			default:
				System.out.println( "No such test number parameter " + 
						    testNumber + "=\"" + testNumber + "\"" );
				System.out.println( "usage: java daDpSlcBuffTest " +
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
	** Simplest Buffered acquisition, default options, and simply print using toString();
	** There are 2 required parameters, at least one BPM name, and the BPMD number.
	*/
	public void Test01()
	{
		try
		{
			System.out.println( "=============================");
			System.out.println( "\nTEST 01: get values of given BPM for 1 pulse\n");

			da.setParam("BPMD=57");               /* BPMD is a required parameter, and it 
                                                              must agree with the DGRP that forms the
                                                              instance name - NDRFACET in this case. */
			da.setParam("BPM1","BPMS:LI11:501");  /* The BPM to acquire */ 
			DaValue v = da.getDaValue("NDRFACET//BUFFACQ");
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
	** Realistic Buff acq, giving> 1 bpm, over 180 pulses. Note that this takes
        ** NRPOS/rate-in-Hz seconds to acquire.
	*/
	public void Test02()
	{
		try
		{
			String query;
		
			System.out.println( "=============================");
			System.out.println( "\nTEST 02: get values of 4 BPMs" );

			query  = "NDRFACET//BUFFACQ";
			da.setParam("BPMD","57");             /* Required parameter */
			da.setParam("NRPOS","180");
			da.setParam("BPM1","BPMS:LI11:501");
			da.setParam("BPM2","BPMS:LI11:601");
			da.setParam("BPM3","BPMS:LI11:701");
			da.setParam("BPM4","BPMS:LI11:801");

			DaValue v = da.getDaValue(query);   /* Acquire BPM data */
			System.out.println("Successfully made acq of "+query);

			printBuffDaValue(v);

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
	 * not a valid parameter of the Buff service,
	 */
	public void Test101()
	{
		try
		{
			System.out.println( "=============================");
			System.out.println( "TEST 101: Bad argument Name - superfish=7");

			String query  = "NDRFACET//BUFFACQ";
			da.setParam("BPMD=57");
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
	** parameter name (BPM1=BPMS:GREG:DOESNTEXIST).
	*/
	public void Test102()
	{
		try
		{
			System.out.println( "=============================");
			System.out.println( "TEST 102: Bad argument value BPM1=BPMS:GREG:DOESNTEXIST");

			String query  = "NDRFACET//BUFFACQ";
			da.setParam("BPMD=57");
			da.setParam("BPM1=BPMS:GREG:DOESNTEXIST");
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
	** Prints the data in a given DaValue assuming it's SLC Buff data
	*/
	private void printBuffDaValue( DaValue v )
	{
		// System.out.println("Returned data as a string "+v.toString());

		Vector namesvec = (Vector)v.get(0); /* Extract 1st to get length */
		int Mrows = namesvec.size();        /* Len of each column = M bpms * NRPOS */
		String[] names = new String[Mrows];
		namesvec.toArray(names);

		Long[] pulseIds = new Long[Mrows];
	        ((Vector)v.get(1)).toArray(pulseIds);

		Float[] xvals = new Float[Mrows];
		((Vector)v.get(2)).toArray(xvals);

		Float[] yvals = new Float[Mrows];
		((Vector)v.get(3)).toArray(yvals);

		Float[] tmits = new Float[Mrows];
		((Vector)v.get(4)).toArray(tmits);

		Long[] stats = new Long[Mrows];
		((Vector)v.get(5)).toArray(stats);

		Long[] goodmeases = new Long[Mrows];
		((Vector)v.get(6)).toArray(goodmeases);

		for (int i=0; i<Mrows; i++ )
		{				
			int goodmeas = goodmeases[i].intValue();
			int  stat = stats[i].intValue();
			// String plane = 
			// 	((hsta & HSTA_XONLY) == HSTA_XONLY) ? "xonly" : 
			//	((hsta & HSTA_YONLY) == HSTA_YONLY) ? "yonly" : "xandy";

			// STAT_OK (so STAT_GOOD|STAT_OK) might be enough for some apps
			// but the most crude check is simply whether STAT_GOOD is on.
			String status = ( ((stat & STAT_GOOD)>0) && 
				         !(((stat & STAT_OFF)>0) || ((stat & STAT_BAD)>0)) ) ?
				"good" : "bad";
			
			System.out.println( names[i] +"\t"+
					    pulseIds[i] +"\t"+
					    xvals[i] +"\t"+ 
					    yvals[i] +"\t"+ 
					    tmits[i] +"\t"+
					    Integer.toHexString(stat) + "\t" +
					    status + "\t"+
					    goodmeas );
		}
	}		
}
