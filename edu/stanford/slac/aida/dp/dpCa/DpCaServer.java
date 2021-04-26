// Java
//
// Mod:
// 20-Nov-2002, Ron MacKenzie 
//   Change exceptions from aida to except package.
//
package edu.stanford.slac.aida.dp.dpCa;

import edu.stanford.slac.aida.lib.util.common.*; 
import edu.stanford.slac.err.*;                  // Exception logging 

/**
 * AIDA Channel Access Data Provider server main class. This is the
 * entry point "main" for the AIDA EPICS CA data prover. It implements
 * a CORBA server, serving a DpCaI object, which as been constructed
 * automatically from an AidaObjectI CORBA Interface defined in
 * AidaObejctI.idl. It has to be started (see st.DpCaServer) for aida
 * to get EPICS channel access data.
 *
 * @author Greg White (greg)
 * @version Greg White (greg) 27-Feb-2005. Changed logging instance to dpCa.
 */
public class DpCaServer
{
	public static final String SERVERNAME = "EPICS"; // AIDA Directory Service entry 
	static Err err;                                  // Error issuance reference
    
	public static void main (String args[])
	{
		int status = 0;
		err = Err.getInstance("dpCa");       // Create Err singleton.


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
			DpCaI_impl dpCaIimpl = new DpCaI_impl(da, da.m_childPersistentPOA);
            
			// Activate Servant with a platform
			// independant string that represents its name
			da.m_childPersistentPOA.activate_object_with_id(
				(serverName).getBytes(), dpCaIimpl);

			// Determine the callable interface to this Servant
			DpCaI server = dpCaIimpl._this(da.m_orb);      
       
			// Register server name and IOR in AIDA Directory Service 
			da.StoreObjRef(serverName, da.m_orb.object_to_string(server));

			// Initialize the server.
			err.log("AIDA DpCaServer Starting");
			dpCaIimpl.init();

			// Start servicing requests to this server
			da.Start();
            
			// When the orb exits, do cleanup.
			da.terminate();
		}
		catch (Exception ex)
		{
			err.log(ex, " when starting DpCaServer" );
			System.err.println(ex.getMessage() );
			status = 1;
		}
		System.exit(status);
	}
}
