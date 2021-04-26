// **********************************************************************
//
// Mod:
//   12-Nov-2002, Ron MacKenzie
//     Change import of except from aida to package except and log a message
//     when the server starts with Err.log().
//
// **********************************************************************

package edu.stanford.slac.aida.dp.dpTestHist;

import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.err.*;     // exception logging class Err.class 


/**
 * This is the entry-point class for the AIDA JAVA Test History Data
 * Provider. You execute this class to start the test history
 * data server.  This test data provider exists only to validate
 * Aida's acquisition of IDL defined type values. That is, the get()
 * method of DpTestHistI_impl returns an Any constructed by streaming an
 * IDL defined type into a CORBA Any by that type's insert() method.
 * At the time of writing DpTestHist only implements the get() and
 * get_doublea() methods!  
 * <p> This Service is accessed with Instance
 * string: "test" and Attribute "val.hist" ie "test//val.hist" </p>
 * @author Greg White Jan-2004.
 * @version 16-Sep-2004, Greg White (greg): Convert for Err singleton re-write.
 */ 
public class DpTestHistServer
{
		public final static String SERVERNAME = "TestHist";  // Directory Service entry.
		static Err err;                                      // Err issuance singleton.

		public static void main(String args[])
		{
				int status = 0;
				err = Err.getInstance(SERVERNAME);           // Create Err singleton.

				try
				{
				        // This is new Persistent AIDA Data object that AIDA 
					    // servers will talk to
				        _PersistentDaObject da = new _PersistentDaObject(args, SERVERNAME);

						// Create servant for this AIDA Object (the implementation file)
						DpTestHistI_impl DpTestHistIimpl = 
								new DpTestHistI_impl(da, da.m_childPersistentPOA);

						// Activate Servant with a platform
						// independant string that represents its name
						da.m_childPersistentPOA.activate_object_with_id(
								(SERVERNAME).getBytes(), DpTestHistIimpl);
	    
						// Determine the callable interface to this Servant
						DpTestHistI server = DpTestHistIimpl._this(da.m_orb);
	    
						// Register this name in Name Server and also the implementation
						da.StoreObjRef(SERVERNAME, da.m_orb.object_to_string(server));

						err.log("AIDA DpTestHistServer Starting");
	    
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
