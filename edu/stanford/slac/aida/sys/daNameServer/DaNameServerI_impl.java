// Orbacus Version: 4.1.0

package edu.stanford.slac.aida.sys.daNameServer;

import java.sql.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.aida.lib.dp.*;
import edu.stanford.slac.err.*;
import edu.stanford.slac.except.*;
import java.io.BufferedReader;
import java.io.InputStreamReader;

//
// IDL:sys.aida.slac.stanford.edu/daNameServer/DaNameServerI:1.0
//
/**
 * Defines the Aida Directory Service API implementation. 
 * 
 * <p> <code>DaNameServiceI_impl</code> implements the DaNameServerI IDL interface. </p>
 *
 * @author Greg White, George McIntyre, Oct-2001
 * @version 26-Oct-2012, Partha/Greg - Implemented Oracle wallet support.
 * @version 05-May-2011, Bob Hall. Modified to invoke getPwd to get Oracle password only once during initialization.
 * @version 04-Apr-2011, Bob Hall. Changed to use getPwd to get Oracle password.
 * @version 08-Mar-2011, Bob Hall. Modified default connection string to reference slacprod instead of slac-oracle03.
 * @version Greg White,  6-Nov-2009: Parameterize the DB connection JDBC URL. 
 * @version Greg White, 16-Oct-2008: Add pattern matching and substitution of Transform.
 * @version Greg White, 18-Jun-2008: Retrieve Statement from ResultSet to close explicitly.
 * @version Greg White, 13-Jun-2008: Explicitly close ResultSets.
 * @version Bob Hall, 13-Feb-2008: Changed Oracle password.
 * @version Greg White, 31-Oct-2007: Fixed population of ref.services in getTargetRef, 
 * to support DaObject.getCount() and DaObject.getServices().
 * @version Greg White, 6-Dec-2005: Changed N_CONNECTION_STRING for new Oracle server host.
 * @version Greg White, 21-Jun-2005: added getConnection(), and
 * implemented retry loop if stmt.executeQuery() fails. This should
 * fix JDBC stale connection problems.
 * @version Greg White, 15-Sep-2004: Convert for Err singleton re-write.
 * @version Greg White, Sep-2004: Added exception handling.
 * @version 25-Jan-2004, Greg White: Changed Connecting string from
 * sage to slac-oracle, and SLAC to SLACPROD
 * @version 13-Nov-2002, Ron MacKenzie Change import of except from aida to package except.
 */
public class DaNameServerI_impl extends DaNameServerIPOA
{
	private POA poa_;
	private ORB orb_;
	private static int stat = 0;
	private int groupId;
        private String password;
	static Err err_;               // Error issuance singleton.
	static boolean debug = false;  // May be turned on by DaNameServer.debug property 

	/* Strings */
	// TODO: protect these access username/passwords
	private static final String N_CONNECTION_STRING = 
	//	"jdbc:oracle:thin:@slacprod.slac.stanford.edu:1521:SLACPROD";
        "jdbc:oracle:oci:/@SLACPROD";

	/* Errors */
	private static final String N_DATABASE_SET_ERROR = 
	"SQLException encountered storing Proxy Object for ";
	private static final String N_DATABASE_GET_ERROR = 
	"SQLException encountered getting Proxy Object for ";
	private static final String N_DATABASE_INIT = 
	"SQLException encountered starting ";

	private static final int MAX_RETRIES = 2;   // Try a sql query at most 3 times. 
	private static final int MAX_SERVICES = 10; // Target must equate to <= 10 services.
	private static Connection m_Conn = null;    // JDBC connection for queries

	/* Query Translation Patterns */ 
	// Pattern for recognizing "&instance" in a query translation. This syntax is
        // chosen to look like SQLplus macro substitution to a user adding names and
        // transforms to the AIDA db, although the substitution is of course done
        // entirely in java.
	private static final Pattern instancePattern = 
		Pattern.compile("(\\&instance)",
				Pattern.CASE_INSENSITIVE | Pattern.MULTILINE );;
	// Pattern for recognizing "&attribute" in a query translation.
	private static final Pattern attributePattern = 
		Pattern.compile("(\\&attribute)",
				Pattern.CASE_INSENSITIVE | Pattern.MULTILINE );

	/**
	 * Constructs and initializes the AIDA Name Server.
	 * @param poa Portable Object Adaptor
	 */
	public DaNameServerI_impl(POA poa)
	{
		err_ = Err.getInstance();    // get Err issuance API singlton.
		poa_ = poa;
		String id = System.getProperty("AIDAGROUPID");
		groupId = (id==null)?1:Integer.parseInt(id);
		if ( System.getProperty("aida.trace") != null ) debug = true;
		initDB();
	}
    
	/**
	 * Gets the portable object adapter under which this instance was 
	 * constructed, if non-null, or the default POA.
	 * @return POA 
	 */
	public POA _default_POA()
	{
		if(poa_ != null)
			return poa_;
		else
			return super._default_POA();
	}
    
    
	//
	// IDL:sys.aida.slac.stanford.edu/daNameServer/DaNameServerI/getTargetRef:1.0
	//
	/**
	 * Gets an Aida reference object for a given target. The reference object can
	 * then be used to make efficient data accesses for that target.
	 *
	 * @param  target   An Aida accessible data item.
	 * @return          A structure specifying the names, locations, and 
	 *                  other Aida system information that will help Aida retrieve the
	 *                  data corresponding to this target quickly. 
	 * @throws UndefinedNameException
	 */
	public AidaObjectRef getTargetRef(String target) throws UndefinedNameException
	{
		AidaObjectRef ref = new AidaObjectRef(); // Encapsulates Aida meta-data. 
		String [] pair = target.split(AQSToken.SEPARATOR);
		                                   // Instance,Attribute pair in target 
		String sqlString = "";             // SQL Query that gets DP of target.
		// Statement stmt = null;             // JDBC statement that runs query on Aida db.
		ResultSet rs = null;               // JDBC ResultSet of target lookup in Aida db. 
		int iService = 0;                  // Count of RowSets/Services (see comment)  
		int services[] = new int[MAX_SERVICES]; // Service identifiers

	       
		// Istantiate but clear the Aida reference's arguments array
		// because CORBA requires that all elements are non null.
		ref.args = new String [1];
		ref.args[0] = "";
	
		// Determine instance and attribute
		ref.inst = pair[0];
		if ( pair.length == 1 )
			ref.attr = null;
		else
			ref.attr = pair[1];
	
		String serviceList = "";
		String sorList = "";
	
	
		// TODO: Put into database as a stored procedure
		sqlString  = " SELECT N.TRANSFORM, " ;
		sqlString += "        N.TYPE_ID, " ;
		sqlString += "        N.COUNT, " ;
		sqlString += "        SI.SERVICE_ID, " ;
		sqlString += "        I.SOR " ;
	
		sqlString += " FROM  AIDA_NAMES N, " ;
		sqlString += "       AIDA_DIRECTORY D, " ;
		sqlString += "       AIDA_SERVICE_INTERFACE SI,  " ;
		sqlString += "	 AIDA_INTERFACES I " ;
		sqlString += " WHERE N.ID = D.NAME_ID " ;
		sqlString += "   AND D.SERVICE_ID = SI.SERVICE_ID AND SI.GROUP_ID = " + 
		    groupId + " ";
		sqlString += "   AND SI.INTERFACE_ID = I.ID ";
	
		// If '*' is to be allowed in query string then replace all occurrences
		// of '*' with '%' in the following strings and use the LIKE operator.
		// Also would need to return the actual instance and attribute fields from N
		sqlString += "   AND N.INSTANCE = '" + ref.inst + "' ";
		sqlString += "   AND N.ATTRIBUTE = '" + ref.attr + "' ";
	
		sqlString += " ORDER BY \"D\".\"ORDER_NO\" ";
	
		// Each RowSet returned by the above query is equivalent to one service
		// involved in processing the named target. Iterate through them (usually
		// there will be only one) and get the info Aida needs to call the service(s).
		try
		{
			// Looking up given instance attribute in the Name Service DB.
			rs = executeQuery( sqlString );
			boolean first = true; 
			while ( rs.next() )
			{
				if ( first )
				{
					first = false;
		    
					String transform = rs.getString(1);
					ref.type = rs.getInt(2);
					ref.count = rs.getInt(3);
		    
					// Process transform if any. If the transform
					// is null, then the input query will be passed
					// to the data provider. If the transform is
					// non-null, then if it begins '/' then it
					// additionally contains regular expressions to
					// be processed by the NameServer substitute
					// method before being processed by the data
					// providers. Pass the text after the '/' to
					// substitute for further processing. If it
					// does not begin '/', then the litteral
					// transform string will be what is passed to
					// the data provider.
					//
					if ( transform == null )	 // Leave unchanged
						ref.data = target ;
					else if (transform.charAt(0) == '/')  // Regexp
						ref.data = substitute(transform.substring(1), 
						                      ref.inst, ref.attr );
       					else			             
						ref.data = transform;    // Simple replacement
		    
					services[iService] = rs.getInt(4);
					sorList = rs.getString(5);
				}
				else
				{
					services[iService] = rs.getInt(4);
					sorList += "," + rs.getString(5);
				}
				iService++; // If == MAX_SERVICES, next iter will cause
                                            // exception like ArrayStoreException caught below.
			}
	
			// Nothing found return empty reference
			if ( first )
			{
				throw (UndefinedNameException)
					err_.log( new UndefinedNameException(target),
						  "when resolving services for name" );
			}

		}
		catch (Exception ex)
		{
			// Instance//attribute was not found (or some other error
			// with select stmt
			throw (UndefinedNameException) 
				err_.log( (new UndefinedNameException( 
					       "when resolving " + target )).initCause(ex));
		}
		finally
		{
			// Clean up jdbc resources.
			try
			{
				if ( rs != null ) 				
				{
					Statement stmt = rs.getStatement();
					if (debug) System.err.println(
						   "Closing ResultSet in getTargetRef");
					rs.close();
					if ( stmt != null ) 
					{
						if (debug) System.err.println(
							"Closing Statement in getTargetRef");
						stmt.close();
					}
				}
			}
			catch ( Exception ex )
			{
				err_.log(ex, "when closing JDBC Statement or ResultSet");
			}
		}
		int nServices = iService; // The number of services = counter ending val
		ref.sor = sorList.split(",");
		ref.services = new int [nServices];				
		for ( int i = 0; i < nServices; i++ )
			ref.services[i] = services[i];
	
		return ref;
	}
    
	//
	// IDL:sys.aida.slac.stanford.edu/daNameServer/DaNameServerI/StoreObjRef:1.0
	//
	/**
	 * Stores a given CORBA IOR for an Aida server in a place where clients can
	 * access it.
	 *
	 * @param name      The name of the server under which to store the IOR
	 * @param ior       The Interoperable Object Reference (IOR) of the proxy object
	 *                  implementing the interface given in the name parameter.
	 */
	public void StoreObjRef(String name, String ior)
	{
		String sqlString = "";
		
		try
		{
			err_.log("Adding Service " + name + 
				 " to Services known by the Name Server");

			// Use an ODBC escape sequence to call the stored procedure
			sqlString  = "{call AIDA_REGISTER_INTERFACE_SP(?, ?, ?) } " ;
			CallableStatement cs= m_Conn.prepareCall(sqlString);
			cs.setString(1, name);
			cs.setString(2, ior);
			cs.setLong(3, groupId); // Set Aida group from property

			cs.execute();
			// m_Conn.createStatement().execute(sqlString);
		}
		catch (Exception ex)
		{
			String suppl = "when updating references for " + name; 
			if ( sqlString != null) suppl.concat(":" + sqlString); 
			if ( ex.getClass().getName() == "java.sql.SQLException" ) 
				suppl.concat(": "+((SQLException)ex).getSQLState());
			err_.log( ex, suppl );
		}
		return;
	}
    
    
	//
	// IDL:sys.aida.slac.stanford.edu/daNameServer/DaNameServerI/GetObjRef:1.0
	//
	/**
	 * Retrieves the CORBA IOR for a given Aida server.
	 * 
	 * @param  name  The name of the server for which to get the IOR.
	 * @return The IOR (CORBA Interoperable Object Reference) for
	 *         the server given by name. 
	 */
	public String GetObjRef(String name)
	{
		String objref = null;
		String sqlString = "";
		// Statement stmt = null;
		ResultSet rs = null;
		try
		{
			// TODO: Put into database as a stored procedure
			sqlString  = " SELECT I.SOR " ;
			sqlString += " FROM AIDA_INTERFACES I, " ;
			sqlString += "  AIDA_SERVICE_INTERFACE SI,  " ;
			sqlString += "  AIDA_SERVICES S " ;
			sqlString += " WHERE I.ID = SI.INTERFACE_ID ";
			sqlString += "   AND SI.SERVICE_ID = S.ID ";
			sqlString += "   AND S.NAME = '" + name + "' ";
			sqlString += "   AND SI.GROUP_ID = '" + groupId + "' ";

 			// rs = executeQuery( sqlString, stmt );
 			rs = executeQuery( sqlString );
			if ( rs.next() )
				objref = rs.getString(1);
			else
				objref = null;
		}
		catch (Exception ex)
		{
			String suppl = "when getting proxy object for " + name;
			err_.log( ex, suppl );
		}
		finally
		{
			// Clean up jdbc resources.
			try
			{
				if ( rs != null ) 
				{
					Statement stmt = rs.getStatement();
					if (debug) System.err.println(
						"Closing ResultSet in GetObjRef");
					rs.close();
					
					if ( stmt != null ) 
					{
						if (debug) System.err.println(
							"Closing Statement");
						stmt.close();
					}
				}
			}
			catch ( Exception ex )
			{
				err_.log(ex, "when closing java.sql.Statement");
			}
		}
		return (objref);
	}
    
			
	//
	// IDL:sys.aida.slac.stanford.edu/daNameServer/DaNameServerI/status:1.0
	//
	/**
	 * Returns the status of the server (or it will anyway). This function may
	 * also be used as a way for the Watcher to make the IMR restart the
	 * server if it is down.
	 *
	 * @return      stat 0 if this is the first time status was called (that is
	 *              the server has just been restarted), 1 otherwise.
	 */
	public int
	status()
	{
		if ( stat == 0 )
			return stat++;
		else
			return stat;
	}
    
	//
	// IDL:sys.aida.slac.stanford.edu/daNameServer/DaNameServerI/shutdown:1.0
	//
	/**
	 * Proper server process shutdown. Call this to shutdown the server process
	 * remotely.
	 */
	public void
	shutdown()
	{
		orb_.shutdown(false);
	}


//
// Private Methods
//	
	/**
	 * Initializes connections to the persistent store that contains all AIDA
	 * directory service information. 
	 */
	private void initDB() 
	{ 
		try
		{
		        err_.logl("Initializing Oracle database connection");
			DriverManager.registerDriver(
				new oracle.jdbc.driver.OracleDriver());
		}
		catch ( Exception ex )
		{
			err_.logl( ex, "while registering driver for connection to Oracle.");
		}

		try
		{
			// Get Oracle password only once during initialization using
                	// script getPwd.  If done in getConnection one can experience
                	// an AFS token expiration problem after getConnection is
                	// called later (e.g., after an Oracle outage).
			Runtime r = Runtime.getRuntime();
			Process p = r.exec("getPwd aida");
                	BufferedReader is =
				new BufferedReader(new InputStreamReader(p.getInputStream()));
	
			password = is.readLine();
		}
		catch ( Exception ex )
		{
			err_.logl( ex, "while getting Oracle password.");
		}

		// Acquire an Oracle Database connection. 
		getConnection();

		if ( m_Conn != null )
			err_.logl("Oracle database connection completed successfully");
		else
			err_.logl( new ServerInitialisationException(
					  "null Connection to database"));
	}

        /**
	 * Initializes a database connection, to the Aida Names Database.
	 *
	 * If the connection is non-null, then the old connection is
	 * first closed. This part is inclued so that this routine can
	 * be used to renew a stale connection.
	 * @version 1.0 17-Jun-2005, Greg White.
	 */
	private void getConnection()
	{
		// If we already have a connection dispose of it.
		try 
		{
			if ( m_Conn != null )
			{
				if ( ! m_Conn.isClosed() ) 
				{
					err_.logl("Closing Aida nameserver db connection");
					m_Conn.close();
					m_Conn = null;
				}
			}
		}
		catch ( Exception ex )
		{
			err_.logl( ex, "while closing Aida nameserver database connection");
		}
		
		// Having dealt with a possible stale connection, get a new one.
		try
		{
			String connectionString = 
				System.getProperty("AIDA_CONNECTION_STRING",
						   N_CONNECTION_STRING);
			err_.logl( "Initializing database connection: "+ connectionString );

			m_Conn = DriverManager.getConnection(connectionString);
			//       	 System.getProperty("AIDA_DATABASE_USERID"), password);
		}
		catch (Exception ex)
		{
			err_.logl( ex, 
				  "while Aida Name Service initialising " + 
				  "connection to database");
		}
	}

	/**
	 * Processes the given transform string for regular expressions embedded in it.
	 * 
	 * The most common mapping, and all that supported for now, is replacing
         * occurances of "&amp;instance" with the aida query instance value, and
         * occurances of "&amp;attribute" with the attribute. This method is called if
         * the transform string begins with "/" (in the first character), which
         * indicates that substitutions of regular expressions found in the transform
         * string should be made by this method. The string resulting from this
         * method is what the Data Provider assigned to the instance attribute pair,
         * will actually receive. The noop transform would be
         * "/&amp;instance//&amp;attribute".
	 *
	 * @param instance The query "instance" part (eg device-name, eg
	 *                 "XCOR:LI21:401")
	 * @param attribute The query "attribute" part (eg secondary, PV value name,
         *                  or simply a property of the instance, eg "Z".)
	 * @param transform The AIDA_NAMES transform field corresponding to the
         *                  instance and attribute also given; it will begin "/".
	 * @return An Aida accessible data item specification, such as a SQL
         *         expression to be processed by the Rdb data provider (or possibly
         *         another query).
	 */
	private String substitute(String original, String instance, String attribute) 
	{
		String finalString; // The String after all replacements

		String afterinstsub = instancePattern.matcher(original).replaceAll(instance);
		finalString = attributePattern.matcher(afterinstsub).replaceAll(attribute);

		/* You could now, if you wanted, parse final string for regexps matching
                   the components of a device-name, PV name, etc, and do replacements of
                   those too. Eg, replace (\w(2)[0-9](2):) with the text matching from the
                   instance, which is likely to be an "area" aka micro name.
		*/ 
		return finalString;
	}


	/**
	 * Queries the AIDA Name Server database with the query in
	 * sqlString. This is a wrapper to give appropriate error
	 * handling and retry logic.
	 * 
	 * @param sqlString the SQL query, in "ascii" (actually UTF-16
	 * or whatever java String is).
	 * @return The ResultSet given by stmt.executeQuery(sqlString)
	 * @version 2.0 18-Jun-2007, Greg White, moves mgnt of Statement to caller.
	 */
	private ResultSet executeQuery( String sqlString  )
		throws SQLException
	{
		ResultSet rs = null;
		Statement stmt = null;
		int nRetries = 0;
		boolean bRetry = false;

		// Create a jdbc Statement and execute the given
		// query. If the query fails to execute for whatever
		// reason, try to get a new connection and loop -
		// re-creating the statement and re-executing the
		// query. Try up to 3 times.
		do
		{
			try 
			{
				if (debug) System.err.println("Creating Statement");
				stmt = m_Conn.createStatement(
					ResultSet.TYPE_FORWARD_ONLY,
					ResultSet.CONCUR_READ_ONLY);
				if (debug) System.err.println("Creating ResultSet");
				rs = stmt.executeQuery( sqlString );
				bRetry = false;
			}
			catch ( Exception ex )
			{
				// We encountered an error in the execution of the sql query,
				// so try to fix this by getting a new Oracle connection
				// and set logic so we'll go through the do loop again.
				if ( nRetries < MAX_RETRIES ) 
				{
					err_.log( ex, "when executing SQL query - " + 
						   "retrying with new java.sql.Connection");
					getConnection();
					bRetry = true;
					nRetries++;
				}
				else
				{
					bRetry = false;
					String suppl = "when executing SQL query " + sqlString;
					if ( ex.getClass().getName()=="java.sql.SQLException") 
					   suppl.concat(": "+((SQLException)ex).getSQLState());
					err_.log( ex, suppl );
				}
			}
		} while ( bRetry );
				
		if ( rs != null && nRetries < MAX_RETRIES )
			return rs;
		else
			throw (SQLException) 
				err_.log( new SQLException("Unable to execute query"));  
	}
	

}
