package edu.stanford.slac.aida.dp.dpModel;

import edu.stanford.slac.aida.lib.util.common.*; 
import edu.stanford.slac.err.*;                  // Exception logging 

/**
 * The AIDA Model Server Data Provider server main class. This is the
 * entry point "main" for the "new" AIDA Model data prover - the one
 * replacing the old AIDA model server, which returned the Dimad
 * model. This new one returns model data from the LCLS Relational
 * Database, computed by XAL modelling. This class implements a CORBA
 * server which then joins the Aida network and implements the
 * standard Aida interface for data server.It has been constructed
 * automatically from an AidaObjectI CORBA Interface defined in
 * AidaObjectI.idl. The server has to be started (see
 * st.DpModelServer) for aida to get model data.
 *
 * @author Greg White (greg) 2-July-2008.
 */
public class DpModelServer
{
	public static final String SERVERNAME = "Model";    // Must strictly match 
	                                                    // AIDA Directory Service entry 
	static Err err;                                     // Error issuance reference
    
	public static void main (String args[])
	{
		int status = 0;
		err = Err.getInstance("DpModel");           // Create Err singleton.


		try {
                        String serverName;

                        String realm = System.getProperty("realm");
                        if (realm == null)
                            serverName = SERVERNAME;
                        else
                            serverName = SERVERNAME + "_" + realm.toLowerCase(); 

      
			// This is new Persistent AIDA Data object
			// that AIDA servers will talk to
			_PersistentDaObject da = new _PersistentDaObject(args, serverName);

			// Create servant for this AIDA Object (the implementation file)
			DpModelI_impl dpModelIimpl = new DpModelI_impl(da, da.m_childPersistentPOA);
            
			// Activate Servant with a platform
			// independant string that represents its name
			da.m_childPersistentPOA.activate_object_with_id(
				(serverName).getBytes(), dpModelIimpl);

			// Determine the callable interface to this Servant
			DpModelI server = dpModelIimpl._this(da.m_orb);      
       
			// Register server name and IOR in AIDA Directory Service 
			da.StoreObjRef(serverName, da.m_orb.object_to_string(server));

			// Initialize the server.
			err.log("AIDA DpModelServer Starting");
			dpModelIimpl.init();

			// Start servicing requests to this server
			da.Start();
            
			// When the orb exits, do cleanup.
			da.terminate();
		}
		catch (Exception ex)
		{
			err.log(ex, " when starting DpModelServer" );
			System.err.println(ex.getMessage() );
			status = 1;
		}
		System.exit(status);
	}
}
