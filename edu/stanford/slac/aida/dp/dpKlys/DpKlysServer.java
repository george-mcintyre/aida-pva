//
package edu.stanford.slac.aida.dp.dpKlys;

import edu.stanford.slac.aida.lib.util.common.*; 
import edu.stanford.slac.err.*;                  // Exception logging 

/**
 * AIDA Channel Access Data Provider server main class. This is the
 * entry point "main" for the AIDA EPICS CA data prover. It implements
 * a CORBA server, serving a DpKlysI object, which as been constructed
 * automatically from an AidaObjectI CORBA Interface defined in
 * AidaObejctI.idl. It has to be started (see st.DpKlysServer) for aida
 * to get EPICS channel access data.
 *
 * @author Greg White (greg)
 */
public class DpKlysServer
{
	public static final String SERVERNAME = "EPICSKlys"; // AIDA Directory Service entry 
	static Err err;                                      // Error issuance reference
    
	public static void main (String args[])
	{
		int status = 0;
		err = Err.getInstance("dpKlys");             // Create Err singleton.

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
			DpKlysI_impl dpKlysTestIimpl = 
			    new DpKlysI_impl(da, da.m_childPersistentPOA);
            
			// Activate Servant with a platform
			// independant string that represents its name
			da.m_childPersistentPOA.activate_object_with_id(
				(serverName).getBytes(), dpKlysTestIimpl);

			// Determine the callable interface to this Servant
			DpKlysI server = dpKlysTestIimpl._this(da.m_orb);      
       
			// Register server name and IOR in AIDA Directory Service 
			da.StoreObjRef(serverName, da.m_orb.object_to_string(server));

			// Initialize the server.
			err.log("AIDA DpKlysServer Starting");
			dpKlysTestIimpl.init();

			// Start servicing requests to this server
			da.Start();
            
			// When the orb exits, do cleanup.
			da.terminate();
		}
		catch (Exception ex)
		{
			err.log(ex, " when starting DpKlysServer" );
			System.err.println(ex.getMessage() );
			status = 1;
		}
		System.exit(status);
	}
}
