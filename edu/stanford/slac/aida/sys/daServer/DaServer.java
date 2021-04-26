package edu.stanford.slac.aida.sys.daServer;

import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.err.*;     // exception logging class Err.class 

/**
 * <code>DaServer</code> is the CORBA server which hosts the primary 
 *     callable interface servant object for AIDA, <code>DaServerI</code>. 
 *     That is, <code>DaServer</code> contains an entry point and must be run as part
 *     of the AIDA run-time server set. 
 * 
 * <p> Usage: aidadev sys.daServer.DaServer (at time of writing). </p>
 *  
 * <p> <code>DaServer</code> hosts the CORBA servant object for <code>DaServerI</code>, 
 * which is in turn the client callable interface for AIDA. </p>
 *
 * @author    Sep-2001 Greg White 
 *         First release
 * @version 16-Sep-2004, Greg White (greg): Convert for Err singleton re-write.
 * @version 13-Nov-2002 Ron MacKenzie
 *         Change import of except from aida to except and log a message
 *         when the server starts with Err.log().
 * @version 15-Aug-2002 Greg White
 *         Removed "status =" from da.terminate() call, so compiles.
 * @version Jun-2002 George McIntyre
 *         Compatibility with NewLabour architecture (using _PersistentDaObject
 */ 
public class DaServer
{
	private static final String SERVERNAME = _PersistentDaObject.PDO_SERVER_NAME;
	static Err err;                             // Err issuance singleton.
    
	public static void main(String args[])
	{
		int status=0;
		err = Err.getInstance(SERVERNAME);  // Create Err singleton.

		try
		{
			_PersistentDaObject da = new _PersistentDaObject(args, SERVERNAME);

			// Create servant for this AIDA Object (the implementation file)
			DaServerI_impl daServerIimpl = new DaServerI_impl(da);

			// Activate Servant with a platform
			// independant string that represents its name
			da.m_childPersistentPOA.activate_object_with_id(
				(SERVERNAME).getBytes(), daServerIimpl);
	    
			// Determine the callable interface to this Servant
			DaServerI server = daServerIimpl._this(da.m_orb);
	    
			// Register this name in Name Server
			da.StoreObjRef(SERVERNAME, da.m_orb.object_to_string(server));

			err.log("AIDA DaServer Starting");
	    
			// Start servicing requests to this server
			da.Start();
	    
			// When the orb exits, do cleanup.
			da.terminate();
		}
		catch (Exception ex)
		{
			err.log(ex, " when starting AIDA " + SERVERNAME );
			status = 1;
		}
		System.exit(status);
	}
}
