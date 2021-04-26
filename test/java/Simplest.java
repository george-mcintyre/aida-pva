package test.java;

import edu.stanford.slac.err.*;                     // Error issuance
import edu.stanford.slac.aida.lib.da.*;             // AIDA Api
import edu.stanford.slac.aida.lib.util.common.*;    // DaValue (for Types)

/**
 * The Simplest Aida complete program, which just attempts to get a given
 * named variable's value as a short int.
 *
 * <p>For this demo the objective is brevity, so the class scope
 * objects DaObject and Err are shown being instantiated in the (main)
 * method, but they would normally be instantiated in a separate
 * constructor.</p>
 */
public class Simplest
{
	/*
	 * The main takes a single argument, an aida name.
	 * @param args A single Aida name, of the form <instance>//<attribute>
	 */
	public static void main( String[] args )
	{
		DaObject da;
	        Err err_;                        // Error issuance singleton.

		String name = args[0];
		try 
		{
			err_ = Err.getInstance("Simplest"); 
			da = new DaObject();
			Double v = (Double) da.get(name,DaValue.Type.DOUBLE);
			System.out.println( name + " as a Double value " + v );
			System.exit(0);
		}
		catch ( Exception e ) 
		{
			System.err.println(e.toString());
			System.exit(1);
		}
	}
}
