package edu.stanford.slac.aida.dp.dpTest;

import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.err.*;     // exception logging class Err.class



/**
 * This Service implements valid returned data for all of the data access calls.
 * This is the entry-point class for the primary AIDA Test Data Provider. That is, you execute
 * this class to start the test data server. 
 * @version 15-Aug-2002, Greg White (greg)
 *      removed "status =" from da.terminate() call, so compiles
 */ 
public class DpTestServer
{

	public final static String SERVERNAME = "Test";   // Directory Service registry name.
	static Err err;                                   // Err issuance singleton reference.

	public static void main(String args[])
	{
		err = Err.getInstance(SERVERNAME);        // Error issuance singleton
		int status = 0;

		try
		{
			// This is new Persistent AIDA Data object
			// that AIDA servers will talk to
			_PersistentDaObject da = new _PersistentDaObject(args, SERVERNAME);

			// Create servant for this AIDA Object (the implementation file)
			DpTestI_impl dpTestIimpl = 
				new DpTestI_impl(da, da.m_childPersistentPOA);

			// Activate Servant with a platform
			// independant string that represents its name
			da.m_childPersistentPOA.activate_object_with_id(
				(SERVERNAME).getBytes(), dpTestIimpl);
	    
			// Determine the callable interface to this Servant
			DpTestI server = dpTestIimpl._this(da.m_orb);
	    
			// Register this name in Name Server and also the implementation
			da.StoreObjRef(SERVERNAME, da.m_orb.object_to_string(server));

			err.log("AIDA DpTestServer Starting");
	    
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
