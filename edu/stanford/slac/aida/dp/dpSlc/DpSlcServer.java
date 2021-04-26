package edu.stanford.slac.aida.dp.dpSlc;

/**************************************************************

 Abs:     AIDA SLC data provider server.

 Class:   DpSlcServer

 Name:    DPSLCSERVER.JAVA
 
 Package: edu.stanford.slac.aida.dp.dpSlc
 
 Rem:     Longer remarks about the code defined by this file.

 Auth:    dd-mmm-yyyy, Bob Sass   (user)
 Rev:     dd-mmm-yyyy, Reviewer's Name (user)

 -----------------------------------------------------------
 Mod:  (Latest to oldest)
 
  27-Sep-2004, Bob Hall (rdh)
               Changed to use the singleton Err pattern.

  03-Jun-2004, Ron MacKenzie (Ronm)
               Added this header
               Added Err.log() call to say the server is up.

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
public class DpSlcServer
{
    static Err err;

    public final static String SERVERNAME = "SLC";
    
    public static void main(String args[])
    {
	int status = 0;

        err = Err.getInstance("DpSlcServer");

	// This is new Persistent AIDA Data object that AIDA servers will talk to
	_PersistentDaObject da = new _PersistentDaObject(args, SERVERNAME);

	try
	{
	    // Create servant for this AIDA Object (the implementation file)
	    DpSlcI_impl dpSlcIimpl = new DpSlcI_impl(da, da.m_childPersistentPOA);
            dpSlcIimpl.init();  // Init for SLC database access

	    // Activate Servant with a platform independant string that represents its name
	    da.m_childPersistentPOA.activate_object_with_id((SERVERNAME).getBytes(), dpSlcIimpl);
	    
	    // Determine the callable interface to this Servant
	    DpSlcI server = dpSlcIimpl._this(da.m_orb);
	    
	    // Register this name in Name Server and also the implementation
	    da.StoreObjRef(SERVERNAME, da.m_orb.object_to_string(server));
	   
            err.log("AIDA DpSlcServer Starting");
 
	    // Start servicing requests to this server
	    da.Start();
	    
	    // When the orb exits, do cleanup.
	    da.terminate();
	}
	catch (Exception ex)
	{
	    System.err.println(ex.getMessage() );
	}
	System.exit(status);
    }
}
