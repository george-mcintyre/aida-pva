package test.java; 
import edu.stanford.slac.err.*;                // Message logging API.
 

/** 
 * The Channel Archiver Data Provider (CHADS) AIDA Test Suite runner.
 * 
 * Tests Da access to EPICS Channel Archiver data though 
 * the DpCa data provider. 
 * 
 * @author  17-Mar-2003, Ron MacKenzie. Copied Greg's daDpCaTest.java.
 * @version 14-Sep-2004, Greg White. Added Err.getEnstance()
 */ 
public class daDpChadsTest 
{ 
	static Err err_;  // Singleton Err reference.

    /** 
     * Tests AIDA Simple operations up to complex context queries. 
     * @param args list of test numbers to be run from 1 to number of tests available 
     */ 
    public static void main(String args[]) 
    { 
        err_ = Err.getInstance("daDpChadsTest");		

        // Class defining tests of the DpCa Data Provider 
        DpChadsTests t = new DpChadsTests(args); 
         
        // Run tests based on command line arguments 
        for ( int i = 0 ; i < args.length ; i++ ) 
        { 
            try 
            { 
                if (!args[i].startsWith("-ORB") && !args[i].startsWith("Event") ) 
                    t.run( Integer.parseInt(args[i]) ); 
            } 
            catch (NumberFormatException e) 
            { 
                System.out.println( "Ignored parameter" + i + "=\"" + args[i] + "\": invalid format" ); 
                System.out.println( "usage: java daDpChadsTest <test_number> ..." ); 
            } 
        } 
        System.exit(0); 
    } 
} 
 
