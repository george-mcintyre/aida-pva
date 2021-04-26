/*
 * _DaObjectBase.java
 *
 * Created on 07 July 2002, 17:50
 * Mod:
 *   27-Oct-2007, Bob Hall (rdh)
 *      Added processing of AIDA_CLIENT_TIMEOUT Java system property
 *      to be able to set a client timeout value. 
 */
 
package edu.stanford.slac.aida.lib.util.common;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.CosEventComm.*;
import org.omg.CosEventChannelAdmin.*;

import java.io.*;
import java.util.*;
import java.net.*;

import edu.stanford.slac.err.*;                     // Err issuance.
import edu.stanford.slac.except.*;                  // General purpose checked exceptions.


/** 
 * Handles CORBA initialization and initial references to the AIDA
 * Name Server for the AIDA data access API <code>DaObject</code>
 * class hierarchy.  It contains only low level ORB connection and
 * initialisation for non-persistent AIDA objects.
 * @author George McIntyre
 * @version 16-Sep-2004, Greg White (greg): Remove catch from
 * constructor, propagate exceptions
 * @version 15-Sep-2004, Greg White (greg): Add error handling.
 * @version 13-Nov-2002, Ron MacKenzie (ronm): Change import of except
 * from aida to except package.
 */
public class _DaObjectBase
{
	/** Internal CORBA ORB associated with this Data Access Object
	 */    
	public    ORB           m_orb;
	protected POAManager    m_POA_manager;
	protected POA           m_rootPOA;
	protected POA           m_consumerPOA;
	protected EventChannel  m_event_channel;
	protected Properties    m_props;
	protected static Err    m_err;             // Error issuance object.

	/** User Arguments for addition to any data retrieval operations prior to call.
	 * Stored as a string array this is organised as key value/pairs.
	 */    
	private   String []	_args;		// Command line arguments
    

	/** CORBA Properties that are passed to ORB.init() to initialise the ORB.
	 */    
	public static final String [][] DAO_PROPS =
	{
		{"org.omg.CORBA.ORBClass", "com.ooc.CORBA.ORB"},
		{"org.omg.CORBA.ORBSingletonClass", "com.ooc.CORBA.ORBSingleton"}
	};
    
	/**
	 * No arg Constructor (for when there are no CORBA parameters to override).
	 * @throws AidaInitException Init error, probably in CORBA initialization.
	 */
	public _DaObjectBase() 
		throws AidaInitException
	{
		m_err = Err.getInstance();        // Get Err logging object reference.
		init();
		return;
	}
    
	/** Constructor with corba argument list.
	 * @param args a list of CORBA properties name value pairs, in particular
	 * the -ORBconfig configfilename.  Properties that are not CORBA are ignored
	 * so that this parameter may be the argv argument passed into the main
	 * program allowing CORBA parameters to be set from the command line.
	 * @throws AidaInitException Init error, probably in CORBA initialization.
	 */
	public _DaObjectBase(String[] args) 
		throws AidaInitException
	{
		m_err = Err.getInstance();        // Get Err logging object reference.
		_args = args;
		init();
		return;
	}
    
	/** Constructor with CORBA argument list and given Portable
	 * Object Adapter (POA).
	 * @param args a list of CORBA properties name value pairs, in particular
	 * the -ORBconfig configfilename.  Properties that are not CORBA are ignored
	 * so that this parameter may be the argv argument passed into the main
	 * program allowing CORBA parameters to be set from the command line.
	 * @param childPersistentPOAName Server name for persistent CORBA servers
	 * @throws AidaInitException Init error, probably in CORBA initialization.
	 */    
	public _DaObjectBase(String[] args, String childPersistentPOAName) 
		throws AidaInitException
	{
		m_err = Err.getInstance();        // Get Err logging object reference.
		_args = args;
		init(childPersistentPOAName);
		return;
	}
    
	/** Constructs a _DaObjectBase including EventService initialization if desired
	 * @param args a list of CORBA properties name value pairs, in particular
	 * the -ORBconfig configfilename.  Properties that are not CORBA are ignored
	 * so that this parameter may be the argv argument passed into the main
	 * program allowing CORBA parameters to be set from the command line.
	 * @param events whether to initialize EventService. If this is true a
	 * push_consumerImpl must be in the classpath
	 * @throws AidaInitException Init error, probably in CORBA initialization.
	 */
	public _DaObjectBase(String[] args, boolean events)
		throws AidaInitException
	{
		this( args );
		if ( events ) initEventService();
	}
    
	/** Constructs a _DaObjectBase without EventService
	 * @param args a list of CORBA properties name value pairs, in particular
	 * the -ORBconfig configfilename.  Properties that are not CORBA are ignored
	 * so that this parameter may be the argv argument passed into the main
	 * program allowing CORBA parameters to be set from the command line.
	 * @param childPersistentPOAName Server name for persistent CORBA servers
	 * @param events whether to initialize EventService. If this is true a
	 * push_consumerImpl must be in the classpath
	 * @throws AidaInitException Init error, probably in CORBA initialization.
	 */    
	public _DaObjectBase(String[] args, String childPersistentPOAName, boolean events)
		throws AidaInitException
	{
		this( args, childPersistentPOAName );
		if ( events ) initEventService();
	}


	/**
	 * Initialise the _DaObjectBase given Portable Object Adapter.  Set CORBA Properties,
	 * initialise the ORB Find the rootPOA and get its Manager.
	 * @version 16-Sep-2004, Greg White (greg): Change throw to
	 * AidaInitException, so super-class overriders can throw the same exception.
	 * @throws AidaInitException up-translated exception from CORBA initialization.
	 */
	protected void init(String childPersistentPOAName) throws AidaInitException
	{
		this.init();
	}


    public ORB getORB ()
    {
        return m_orb;
    }

	/**
	 * Initialise the _DaObjectBase.  Set CORBA Properties,
	 * initialise the ORB Find the rootPOA and get its Manager.
	 * @version 16-Sep-2004, Greg White (greg): Change throw to
	 * AidaInitException, so super-class overriders can throw the same exception.
	 * @throws AidaInitException up-translated exception from CORBA initialization.
	 */
	protected void init() throws AidaInitException
	{
		try
		{
			// Set Properties
			m_props = System.getProperties();
			for ( int i = 0; i < DAO_PROPS.length; i++ )
				m_props.put(DAO_PROPS[i][0], DAO_PROPS[i][1]);

                        // If the AIDA_CLIENT_TIMEOUT System property exists,
                        // set the ORBACUS property ooc.orb.client_timeout to
                        // this value.
                        String clientTimeout = System.getProperty("AIDA_CLIENT_TIMEOUT");
                        if (clientTimeout != null)
                        {
                            m_props.put("ooc.orb.client_timeout", clientTimeout);
                        }

			// Initialise the Orb
			m_orb = ORB.init(_args, m_props);
	    
			// Setup the root POA and its manager
			m_rootPOA = 
				POAHelper.narrow(m_orb.resolve_initial_references("RootPOA"));
			m_POA_manager = m_rootPOA.the_POAManager();
		}
		catch ( Exception ex )
		{
			m_orb = null;
			System.err.print( java.util.Calendar.getInstance().getTime() + ": "); 
			ex.printStackTrace();
			throw (AidaInitException) 
				m_err.log(new AidaInitException(
						  "_DaObjectBase failed to initialize. Caused by: " +
						  ex.getMessage()));
		}
	}
    
	/** Return the ProxyPushConsumer to anyone who wants to know
	 * @return Proxy Push Consumer
	 */
	public ProxyPushConsumer getPPConsumer()
	{
		if ( m_event_channel == null )
			return null;
		else
			return (m_event_channel.for_suppliers()).obtain_push_consumer();
	}

	/**
	 * Initializes a CORBA object as consumer of EventService events
	 */
	protected void initEventService()
	{
		try
		{
			// Create a persistent POA for Event Service.
			Policy[] policy = new Policy[2];
			policy[0] = m_rootPOA.create_lifespan_policy(
				LifespanPolicyValue.PERSISTENT);
			policy[1] = m_rootPOA.create_id_assignment_policy(
				IdAssignmentPolicyValue.USER_ID);
			m_consumerPOA = 
				m_rootPOA.create_POA("consumer", m_POA_manager, policy);
	    
			// Get event channel from the initial reference.
			m_event_channel = EventChannelHelper.narrow(
				m_orb.resolve_initial_references("EventService"));
			if ( m_event_channel == null )
			{
				throw (IllegalStateException) 
					m_err.log(new IllegalStateException(
							  "`EventService' is not an EventChannel object reference"));
			}
		}
		catch( Exception ex )
		{
			String assn = "Error initializing EventService: " + ex.toString();
			System.err.println( java.util.Calendar.getInstance().getTime() + ": " +
					    assn);
			m_err.log(new IllegalStateException(assn));
			System.exit(1);
		}
	}
    
	/** Connects given consumer to the Event Service. 
	 * [23-Sep-2004, Greg White: Not sure this is ever used].
	 * @param consumer a PushConsumer that will receive events
	 */
    
	public void EventConnect(PushConsumer consumer)
	{
		try
		{
			((m_event_channel.for_consumers()).
			 obtain_push_supplier()).connect_push_consumer(consumer);
		}
		catch (Exception ex)
		{
			m_err.log(new AidaInitException(ex.getMessage()));
		}
	}
    
	/**
	 * Starts the CORBA server 
	 * @throws IllegalStateException Error activating the server,
	 * or in <code>Orb.run()</code>.
	 */ 
	public void Start()
	{
		try
		{
			m_POA_manager.activate();
			m_err.logl("Server ready");
			m_orb.run();
		}
		catch (Exception ex)
		{
			System.err.print( java.util.Calendar.getInstance().getTime() + ": ");
			ex.printStackTrace();
			System.err.print( java.util.Calendar.getInstance().getTime() + ": ");
			System.err.println("Aida cannot activate server:" + ex.getMessage());
			m_err.log( 
				new IllegalStateException("Aida cannot activate server: " + 
							  ex.getMessage()));
			System.exit(1);
		}
		return;
	}

	/** Get the AIDA Name Server's Interoperable Object Reference.
	 * In this implementation it is stored in a file in a well
	 * know location and is accessed here by way of a URL.
	 * @return the AIDA Name Server's Interoperable Object Reference
	 */    
	public String getNameServerIOR()
	{   
		InputStream in = null;
		try
		{
			URL url = new URL(System.getProperty("AIDA_NAMESERVER_IOR_URL"));
			in = url.openStream();
			BufferedReader bfin = new BufferedReader(new InputStreamReader( in ));
			String ior = bfin.readLine();
			return ior;
		}
		catch (IOException e)
		{
			System.err.print( java.util.Calendar.getInstance().getTime() + ": ");
			System.err.println(e.getMessage() + 
					   " when retrieving AIDA_NAMESERVER_IOR_URL property");
			m_err.log(e, " when retrieving AIDA_NAMESERVER_IOR_URL property");
			System.exit(1);
		}
		finally {
			try {in.close(); } catch (Exception e) {}
		}
            
		return null;
	} 
    
	/** Get the AIDA Name Server's Interoperable Object Reference.
	 * In this implementation it is stored in a file in a well
	 * know location and is accessed here by way of a URL.
	 * @param ior The Interoperable Object Reference for the current server
	 */    
	public void setNameServerIOR(String ior)
	{
		try
		{
			PrintWriter out = new PrintWriter(
				new FileOutputStream(
					System.getProperty("AIDA_NAMESERVER_IOR_FILE")));
			out.println(ior);
			out.flush();
			out.close();
		}
		catch (Exception e)
		{
			System.err.print( java.util.Calendar.getInstance().getTime() + ": ");
			System.err.println(e.getMessage() + 
					   "when serializing NameServer IOR");
			m_err.log(e, "when serializing NameServer IOR");
			System.exit(1);
		}
	}
}
