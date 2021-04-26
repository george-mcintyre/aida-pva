package edu.stanford.slac.aida.dp.dpSlc;

/**************************************************************

 Abs:     AIDA SLC data provider server.

 Class:   DpSlcHistServer

 Name:    DPSLCHISTSERVER.JAVA
 
 Package: edu.stanford.slac.aida.dp.dpSlc
 
 Rem:     Longer remarks about the code defined by this file.

 Auth:    14-Sep-2004, Bob Hall (rdh)
 Rev:     dd-mmm-yyyy, Reviewer's Name (user)

 -----------------------------------------------------------
 Mod:  (latest to oldest)

          27-Sep-2004, Bob Hall (rdh)
          Changed to use the singleton Err pattern.

*************************************************************/


import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.except.*;
import edu.stanford.slac.err.*;     // exception logging class Err.class

/**
 * Mod: 15-Aug-2002, Greg White (greg)
 *      removed "status =" from da.terminate() call, so compiles
 */ 
public class DpSlcHistServer
{
    static Err err;  // Singleton Err reference.

    public final static String HIST_DATA_PROVIDER = "SLCHist";
    
    public static void main(String args[])
    {
	int status = 0;

        Err err = Err.getInstance("DpSlcHistServer");

	// This is new Persistent AIDA Data object that AIDA servers will talk to
	_PersistentDaObject histDa = new _PersistentDaObject(args,
            HIST_DATA_PROVIDER);

	try
	{
	    // Create servant for this AIDA Object (the implementation file)
	    DpSlcHistI_impl dpSlcHistIimpl = new DpSlcHistI_impl(histDa,
                histDa.m_childPersistentPOA);

            dpSlcHistIimpl.init();  // Init for SLC database access 

	    // Activate Servant with a platform independant string that represents its name
	    histDa.m_childPersistentPOA.activate_object_with_id(
                (HIST_DATA_PROVIDER).getBytes(), dpSlcHistIimpl);
	    
	    // Determine the callable interface to this Servant
	    DpSlcHistI histServer = dpSlcHistIimpl._this(histDa.m_orb);
	    
	    // Register the data providers in Name Server and also the implementation
	    histDa.StoreObjRef(HIST_DATA_PROVIDER,
                histDa.m_orb.object_to_string(histServer));
	   
            err.log("AIDA DpSlcHistServer Starting");
 
	    // Start servicing requests to this server
	    histDa.Start();
	    
	    // When the orb exits, do cleanup.
	    histDa.terminate();

	}
	catch (Exception ex)
	{
	    System.err.println(ex.getMessage() );
	}
	System.exit(status);
    }
}
