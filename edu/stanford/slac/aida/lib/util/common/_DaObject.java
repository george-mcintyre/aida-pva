// Mod:
//   17-Jan-2004, Greg White (greg)
//     Do getAidaServerI in stages for debugging server acquisition (stuck POA).
//   13-Nov-2002, Ron MacKenzie (ronm)
//     Change import of except from aida to except package.

package edu.stanford.slac.aida.lib.util.common;

import java.util.*;

import edu.stanford.slac.except.*;
import edu.stanford.slac.aida.sys.daNameServer.*;
import edu.stanford.slac.aida.lib.dp.*;

/** 
 * Encapsulates interactions with the Aida Name Server and
 * acquisition parameter handling, for the AIDA data access API
 * <code>DaObject</code> class hierarchy. <code>_DaObject</code> is
 * used in preference to the <code>DaObject</code> when coding AIDA
 * system components, since it contains methods for registering with
 * AIDA, and finding the Name Server and other system interfaces etc.
 * @author 24-Jun-2002, George McIntyre
 * @version 10-Feb-2009, Greg White (greg): Added toStringArgs method
 * @version 16-Sep-2004, Greg White (greg): Extended, added error
 * handling and comments.
 */
public class _DaObject extends _DaObjectBase
{
	/** Arguments to be passed to the service that will execute any requests that
	 * are transmitted via this Data Access Object.
	 */    
	public Hashtable	m_args;	    // Global List of (parm, value) pairs: 
	// global for all levels

	/** Internal reference to a callable CORBA interface to the Name Server
	 */    
	private   DaNameServerI m_ns;

	/** No-arg constructor.
	 * Make a connection to the ORB.  Find the Name Server.
	 * @throws AidaInitException Probably CORBA failed to initialize.
	 */    
	public _DaObject()
		throws AidaInitException 
	{
		super();
	}
    
	/** Create and initialise a DaObject, with argument list.
	 * Make a connection to the ORB.  Find the Name Server.
	 * @param args a list of CORBA properties name value pairs, in
	 * particular the -ORBconfig configfilename
	 * @param childPersistentPOAName Name of this persistent server
	 * @throws AidaInitException Probably CORBA failed to initialize.
	 */    
	public _DaObject(String[] args, String childPersistentPOAName)
		throws AidaInitException 
	{
		super(args, childPersistentPOAName);
	}
    
	/** Create and initialise a DaObject, with argument list
	 * Make a connection to the ORB.  Find the Name Server.
	 * @param args a list of CORBA properties name value pairs, in
	 * particular the -ORBconfig configfilename
	 * @param childPersistentPOAName Name of this persistent server
	 * @param events whether to initialize EventService. If this
	 * is true a push_consumerImpl must be in the classpath
	 * @throws AidaInitException Probably CORBA failed to initialize.
	 */    
	public _DaObject(String[] args, String childPersistentPOAName, boolean events) 
		throws AidaInitException
	{
		super(args, childPersistentPOAName, events);
	}
    
	/** Constructs a _DaObject without EventService.
	 * Make a connection to the ORB.  Find the Name Server.
	 * @param args a list of CORBA properties name value pairs, in
	 * particular the -ORBconfig configfilename
	 * @throws AidaInitException Probably CORBA failed to initialize.
	 */
	public _DaObject(String[] args) throws AidaInitException
	{
		super(args);
	}
  
	/** 
	 * Constructs a _DaObject including EventService initialization if desired
	 * @param args a list of CORBA properties name value pairs, in
	 * particular the -ORBconfig configfilename
	 * @param events whether to initialize EventService. If this
	 * is true a push_consumerImpl must be in the classpath
	 * @throws AidaInitException Probably CORBA failed to initialize.
	 */
	public _DaObject(String[] args, boolean events) throws AidaInitException 
	{
		super(args, events);
	}

	/**
	 * Initializes the hash table of queries to data-providers,
	 * and acquires the Name Server's CORBA
	 * interface. <code>_DaObject::init</code> extends
	 * <code>_DaObjectBase::init</code>, which has initialized
	 * CORBA communications.
	 * @throws AidaInitException Probably CORBA failed to initialize.
	 */
	protected void init() throws AidaInitException
	{
		super.init();

		// Connect to Name Service
		m_args = new Hashtable();
		m_ns = getNameServerI();
	}

	/** 
	 * Gets a AIDA target reference structure that refers to the
	 * given target.
	 * @param target The given String AQS (AIDA Query Specification)
	 * @version 10-Sep-2004, Greg White: propagate UndefinedNameException
	 * @return An AidaObjectRef, IDL structure with details of run-time info 
	 * to get a target.
	 * @throws UndefinedNameException The Directory Service could
	 * not resolve the target. Either there is no such name in the
	 * Directories database, or it was poorly specified in the
	 * database.
	 */
	public AidaObjectRef getTargetRef(String target) throws UndefinedNameException
	{
		return (m_ns.getTargetRef(target));
	}
    
	/**
	 * Stores CORBA IOR for an Aida server in a place where clients can
	 * access them
	 * @param aidaObjectName the name of the server under which to store the IOR
	 * @param ior the Interoperable Object Reference (IOR) of the proxy object
	 *            implementing the interface in the name parameter.
	 */
	public void StoreObjRef(String aidaObjectName, String ior)
	{
		m_ns.StoreObjRef(aidaObjectName, ior);
		return;
	}

	/** 
	 * Retrieves CORBA IOR for an Aida server for clients.  A
	 * CORBA IOR is an Inter-Operable Reference to a CORBA object
	 * (Interface, type, etc).  The IOR can be used by ORB
	 * functions to access stub objects referencing the given AIDA
	 * Server object.
	 * @param aidaObjectName The name of the AIDA Server Object
	 * for which the IOR is required
	 * @return IOR for the given server name, retrieved from the Name Server
	 */
	public byte[] GetObjRef(String aidaObjectName)
	{
		return ( (m_ns.GetObjRef(aidaObjectName)).getBytes() );
	}
    
	/** Gets a Service's callable interface given its service
	 * name, as recorded in the Directory Service.
	 * @param service Name of service whose interface is desired
	 * @return A CORBA server stub containg the callable interface to the given service
	 * @throws IllegalStateException couldn't narrow CORBA IOR for Aida Server.
	 */
	public AidaObjectI getAidaServerI(String service)
	{
		try
		{					
			if ( service.startsWith("IOR") )
				return (AidaObjectIHelper.unchecked_narrow(
						m_orb.string_to_object( service)));

			m_err.log("Making connection to " + service );
			String iorString = m_ns.GetObjRef(service);
			org.omg.CORBA.Object absObj = m_orb.string_to_object( iorString );
			AidaObjectI aidaServerI = AidaObjectIHelper.unchecked_narrow( absObj );
			return aidaServerI;

//			return (AidaObjectIHelper.unchecked_narrow(
//					m_orb.string_to_object( m_ns.GetObjRef(service) )));
		}
		catch ( Exception e )
		{
			System.out.println("original exception" + e.toString());
			throw (IllegalStateException) 
				m_err.log(new IllegalStateException(e.toString()), 
					  " when narrowing an Aida server IOR for " + service);
		}
	}

	/** Get the Name Service's callable interface
	 * @return A CORBA server stub containg the callable interface of the 
	 * Aida Name Service
	 */
	public DaNameServerI getNameServerI()
	{
		m_err.log("Making connection to Name Service");
		return (DaNameServerIHelper.narrow(
		    m_orb.string_to_object(getNameServerIOR())));
	}

	/** Gets all the parameters that have been set so far for 
	 * this Data Access Object
	 * @return an array of Strings containing the parameter/value pairs in 
	 * even/odd respective indicies
	 */
	public String [] getArgs()
	{
		int i = 0;
		String [] args = null;

		if ( !m_args.isEmpty() )
			args = new String [m_args.size()*2];
	
		for ( Enumeration e = m_args.keys(); e.hasMoreElements();)
		{
			String param;
			args[i++] = param = (String )e.nextElement();
			args[i++] = (String )m_args.get(param);
		}
        
		if ( args == null )
		{
			args = new String [2];
			args[0] = "null";  // at least one parameter needed for CORBA
			args[1] = " ";	   // at least one parameter needed for CORBA
		}
		return args;
	}
    
	/** Method that allows addition of a set of arguments
	 * specified in an array similar to that returned from
	 * getArgs() to be added the current set of args.  This method
	 * simply concatenates and does not merge.  The current set of
	 * args are left unchanged.
	 * @param a1 Parameter list 1
	 * @param a2 Parameter List 2
	 * @return An array of Strings containing the concatenated parameter list
	 */    
	public String [] concatArgs(String [] concatenee)
	{
		String [] currentArgs = getArgs();
		if ( currentArgs == null || (currentArgs[0].compareTo("null") == 0) )
			return concatenee;
		else if ( concatenee == null || (concatenee[0].compareTo("null") == 0) )
			return currentArgs ;

		int i, j, l1=currentArgs.length, l2=concatenee.length;

		String [] r = new String [l1+l2];
		for ( i=0; i < l1 ; i++ )
			r[i] = currentArgs[i];
		for ( j=0 ; j < l2 ; i++, j++ )
			r[i] = concatenee[j];
		return r;
	}

	/**
	 * Converts the String array args to a single String suitable for printing.
	 * @param args The parameters client program supplied to a DaObject method
	 * @return A single String, formed by concatenation of String[] args.
	 */
	public String toStringArgs()
	{
		String s = ""; // Ret var: args as a single string.

		String [] currentArgs = getArgs();
		if ( ! (currentArgs == null || 
		       (currentArgs[0].compareTo("null") == 0)) )
		for (int i = 0; i < currentArgs.length; i++)
			s = s+" "+currentArgs[i];
		return s;
	}
    
}


