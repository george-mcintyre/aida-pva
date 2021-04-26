package test.java;

import edu.stanford.slac.err.*;                     // Error issuance
import edu.stanford.slac.aida.lib.da.*;             // AIDA Api
import edu.stanford.slac.aida.lib.util.common.*;    // DaValue (for Types)

/**
 * Stress tests acquisitions with Aida
 *
 */
public class LoadTest
{
	/*
	 * The main takes a single argument, an aida name, which in
	 * this case should be acquireable as a Float.
	 */
	public static void main( String[] args )
	{
		DaObject da;
	        Err err_;                        // Error issuance singleton.

		String query = "P2BPMHER//BPMS";
		try 
		{
			err_ = Err.getInstance("LoadTest"); 
			da = new DaObject();
			da.setParam("BPMD=38");
			da.setParam("REF=0");               /* Return absolute orbit */
			da.setParam("SORTORDER=1");         /* 1 is "Z" order */
			DaReference r = new DaReference(query);
			
			DaValue v = null;

			while (true)
			{
				v = r.getDaValue(da);
				System.out.println( query +  " = " + v.toString() );
				Thread.sleep(1000);  // milliseconds, 1.0s appart then
			}
		}
		catch ( Exception e ) 
		{
			System.err.println(e.toString());
			System.exit(1);
		}
	}
}
