package edu.stanford.slac.aida.dp.dpRdb;

import edu.stanford.slac.aida.lib.util.common.*; 
import edu.stanford.slac.err.*;                  // Exception logging 

/**
 * The AIDA Relational Database Data Provider server main class. This is the
 * entry point "main" for the AIDA RDB data prover. It implements
 * a CORBA server, serving a DpRdbI object, which as been constructed
 * automatically from an AidaObjectI CORBA Interface defined in
 * AidaObejctI.idl. It has to be started (see st.DpRdbServer) for aida
 * to get relational database data.
 *
 * @author Greg White (greg) 5-June-2007.
 */
public class DpRdbServer
{
	public static final String SERVERNAME = "Database"; // AIDA Directory Service entry 
	static Err err;                                     // Error issuance reference
    
	public static void main (String args[])
	{
		int status = 0;
		err = Err.getInstance("dpRdb");       // Create Err singleton.


		try {
                        String serverName;

                        String realm = System.getProperty("realm");
                        if (realm == null)
                        {
                            serverName = SERVERNAME;
                        }
                        else
                        {
                            serverName = SERVERNAME + "_" + realm.toLowerCase(); 
                        }

      
			// This is new Persistent AIDA Data object
			// that AIDA servers will talk to
			_PersistentDaObject da = new _PersistentDaObject(args, serverName);

			// Create servant for this AIDA Object (the implementation file)
			DpRdbI_impl dpRdbIimpl = new DpRdbI_impl(da, da.m_childPersistentPOA);
            
			// Activate Servant with a platform
			// independant string that represents its name
			da.m_childPersistentPOA.activate_object_with_id(
				(serverName).getBytes(), dpRdbIimpl);

			// Determine the callable interface to this Servant
			DpRdbI server = dpRdbIimpl._this(da.m_orb);      
       
			// Register server name and IOR in AIDA Directory Service 
			da.StoreObjRef(serverName, da.m_orb.object_to_string(server));

			// Initialize the server.
			err.log("AIDA DpRdbServer Starting");
			dpRdbIimpl.init();

			// Start servicing requests to this server
			da.Start();
            
			// When the orb exits, do cleanup.
			da.terminate();
		}
		catch (Exception ex)
		{
			err.log(ex, " when starting DpRdbServer" );
			System.err.println(ex.getMessage() );
			status = 1;
		}
		System.exit(status);
	}
}
