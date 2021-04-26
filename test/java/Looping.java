package test.java;

import edu.stanford.slac.err.*;                     // Error issuance
import edu.stanford.slac.aida.lib.da.*;             // AIDA Api
import edu.stanford.slac.aida.lib.util.common.*;    // DaValue (for Types)

/**
 * The simplest looping Acquisition with Aida
 *
 * <p>For this demo the objective is brevity, so the class scope
 * objects DaObject and Err are shown being instantiated in the (main)
 * method, but they would normally be instantiated in a separate
 * constructor.</p>
 */
public class Looping
{
	/*
	 * The main takes a single argument, an aida name, which in
	 * this case should be acquireable as a Float.
	 */
	public static void main( String[] args )
	{
		DaObject da;
	        Err err_;                        // Error issuance singleton.

		String name = args[0];
		try 
		{
			err_ = Err.getInstance("Looping"); 
			da = new DaObject();
			DaReference r = new DaReference(name,da);
			while (true)
			{
				float v = Float.NaN;
				try 
				{
					v = r.get_float();
				}
				catch ( Exception e )
				{
					System.err.println(e.toString());
					System.err.println("retrying forever");
				}
				System.out.println( name + " = " + v );
				Thread.sleep(10000);  // milliseconds
			}
		}
		catch ( Exception e ) 
		{
			System.err.println(e.toString());
			System.exit(1);
		}
	}
}
