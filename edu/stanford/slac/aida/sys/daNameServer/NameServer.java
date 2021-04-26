package edu.stanford.slac.aida.sys.daNameServer;

import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.err.*;                    // exception logging API 

/**
 * <code>NameServer</code> is the AIDA directory service server. This class servers as the 
 * entry-point "main" for the name server.
 *
 * <p> Usage: aidadev sys.daNameServer.DaNameServer (at time of writing). </p>
 * 
 * <p> NamesServer is the CORBA server which hosts the AIDA 
 * daNameServer interface servant object daNameServerI. 
 *     NameServer contains an entry point and must be run as part
 *     of the AIDA run-time server set. daNameServer is the AIDA directory 
 *     service API. That is, it's a class which provides
 *     AIDA processes with a way to discover the names, attributes, and
 *     run-time access specifics of each data item available through AIDA. </p>
 *
 * @author Sep-2001 Greg White
 *         First release
 * @version 21-Jun-2005 Greg White (greg): changed err.logs to err.logls
 * @version 15-Sep-2004 Greg White (greg)
 *         Change use of Err for Err's new singleton implementation.
 * @version 13-Nov-2002 Ron MacKenzie
 *         Change import of except from aida to package except and log a message
 *         when the server starts with Err.log().
 * @version 15-Aug-2002 Greg White
 *         Removed "status =" from da.terminate() call, so compiles.
 * @version Jun-2002 George McIntyre
 *         Compatibility with NewLabour architecture
 */ 
public class NameServer
{
	private static final String SERVERNAME = "NameServer"; // Directory Service entry.
	static Err err;                                        // Err issuance singleton.

	/**
	 * Starts the Aida Directory service server.
	 * @param args -ORBconfig configfilename.conf
	 *             -ORBInitRef corbaloc of IMR host
	 *             -ORBInitRef corbaloc of Event Service host
	 *             -ORBserver_name name The name of this server is "name".
	 *        
	 */
	public static void main(String args[])
	{
		int status = 0;
		err = Err.getInstance(SERVERNAME);             // Create Err singleton.

		try
		{
			// This is new Persistent AIDA Data object that
			// AIDA servers will talk to
			_PersistentDaObjectBase da = 
				new _PersistentDaObjectBase(args, SERVERNAME);

			// Create servant for this AIDA Object (the implementation file)
			DaNameServerI_impl daNameServerIimpl = 
				new DaNameServerI_impl(da.m_childPersistentPOA);
	    
			// Activate Servant with a platform
			// independant string that represents its name
			da.m_childPersistentPOA.activate_object_with_id(
				(SERVERNAME).getBytes(), daNameServerIimpl);
	    
			// Determine the callable interface to this Servant
			DaNameServerI nameServer = daNameServerIimpl._this(da.m_orb);
	    
			// Serialise this object's reference to permanent storage
			da.setNameServerIOR(da.m_orb.object_to_string(nameServer));

			err.logl("AIDA Name Server Starting");

			// Start servicing requests to this server
			da.Start();
	    
			// When the orb exits, do cleanup.
			da.terminate();
		}
		catch (Exception ex)
		{
			err.logl(ex, " when starting NameServer");
			status = 1;
		}
		System.exit( status );
	}
}



