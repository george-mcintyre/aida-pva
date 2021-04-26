
package edu.stanford.slac.aida.dp.dpRdb;

import java.sql.*;
import org.omg.CORBA.ORB;
import org.omg.PortableServer.*;
import edu.stanford.slac.err.*;
import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.except.*;
import java.io.BufferedReader;
import java.io.InputStreamReader;

/**
 * This class defines the AIDA Relational Database Data Provider CORBA server implementation
 *
 * DpRdbI_impl implements the AIDA Data Provider "DpRdbI" CORBA
 * interface, by extending DpRdbIPOA
 *  
 * @author Greg White
 * @version 26-Oct-2012, Partha / Greg. Implemented Oracle Wallet support 
 * @version 05-May-2011, Bob Hall. Modified to invoke getPwd to get Oracle password only once during initialization.
 * @version 04-Apr-2011, Bob Hall. Changed to use getPwd to get Oracle password.
 * @version 08-Mar-2011, Bob Hall. Modified default connection string to reference slacprod instead of slac-oracle03.
 * @version 22-Sep-2008, Greg White. Return " " in place of null Strings returned by Oracle.
 * @version 9-Jun-2008, initial version.
 *
 */
public class DpRdbI_impl extends DpRdbIPOA
{
	private static final boolean debug = false; // Turn debug messages on or off.

	private static boolean m_doneInit = false; // Whether = inited this server.
        private String password;
	private _PersistentDaObject m_da;          // For orb access.
	private ORB m_orb;                         // CORBA ORB for this server.
	private org.omg.PortableServer.POA m_poa;  // Portable Object Adapter
	private Err m_err;                         // Err messaging singleton.

	// Strings
	// TODO: Externalize protect access username/passwords and strings
	// TODO: Convert connection string to java Property.
	private static final String N_CONNECTION_STRING = 
	"jdbc:oracle:oci:/@SLACPROD";
	//"jdbc:oracle:thin:@slacprod.slac.stanford.edu:1521:SLACPROD";

	// Errors
	private static final String N_DATABASE_SET_ERROR = 
	  "SQLException encountered storing Proxy Object for ";
	private static final String N_DATABASE_GET_ERROR = 
	  "SQLException encountered getting Proxy Object for ";
	private static final String N_DATABASE_INIT = 
	  "SQLException encountered starting ";
	private static final String NORESULTSETMETADATA = 
	  "No ResultSet metadata available so cannot construct DaValue return members";

	private static final int MAX_RETRIES = 2;   // Try a sql query at most 3 times. 
	private static final int MAX_SERVICES = 10; // Target must equate to <= 10 services.
	private static Connection m_Conn = null;    // JDBC connection for queries


    
	DpRdbI_impl(_PersistentDaObject da, org.omg.PortableServer.POA poa)
	{
		m_da = da;      
		m_orb = m_da.m_orb; // Make access to orb easy.
		m_poa = poa;        // Portable Object Adapter, not used atow

		// Get Err issuance singleton, and log init complete
		m_err = Err.getInstance();
	}
    
	public org.omg.PortableServer.POA
	_default_POA()
	{
		if(m_poa != null)
			return m_poa;
		else
			return super._default_POA();
	}
    
	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/init:1.0
	//
	public void
	init()
		throws ServerInitialisationException
	{
		try 
		{
			initDB();
		}
		catch ( Exception e )
		{
			throw (ServerInitialisationException) 
				m_err.log( new ServerInitialisationException(e.getMessage())); 
		}
		m_err.log("DpRdbI servant initialization completed.");        
	}


    
	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get:1.0
	//
        /**
	 * Takes the SQL "select" expression, executes it, and returns the results.
	 * @param query The SQL select expression from the Aida names database
	 * @param args  Array of name/value string pairs
	 * @return DaValue (encapsulated by CORBA Any), each vector in which 
	 *         corresponds to one row of the ResultSet resulting from the query.
	 */
	public org.omg.CORBA.Any
	get(String query,
	    String[] args)
		throws  UnableToGetDataException
	{
		DaValue ret = new DaValue();
		ret.type = DaValue.Type.STRUCT;
		ResultSet rs = null;

		// NOTE: Putting this in causes TypeCode conversion exception in DaValue toAny conversion
		// ret.setName(query);   

		try
		{
			// Replace values of any passed arguments for matched arg names in the query
			// String lquery = substituteArgs( args, query );

			rs = executeQuery( query );
			ResultSetMetaData rsmd = rs.getMetaData();
			if (rsmd == null) 
				throw new UnableToGetDataException(NORESULTSETMETADATA);    

			// Get number of rows in ResultSet
			rs.last();
			int rowsM = rs.getRow();

			// Get number of columns in ResultSet
			int columnsN = rsmd.getColumnCount();
			if (debug) System.err.println( "Num Columns = "+ columnsN );

			// For each column, extract all the rows of the column from the ResultSet
			// and add the whole column to what we return. So we're transposing the ResultSet
			// where the slow moving index is row, to a DaValue where the slow moving index is
			// what was the ResultSet column. Ie, each vector of the DaValue, is one complete 
			// column.
			for (int colj = 1; colj <= columnsN; colj++)
			{
				rs.beforeFirst();  // Reset cursor to first row.
				int i=0;           // Reset row indexer.
				DaValue colDaValue = null; 
				if (debug) System.err.println( "\nColumn Name = "+rsmd.getColumnName(colj));
				switch (rsmd.getColumnType(colj))
				{
				case java.sql.Types.DECIMAL:
				case java.sql.Types.DOUBLE:
				case java.sql.Types.REAL:
				case java.sql.Types.NUMERIC:
				case java.sql.Types.FLOAT:
				    {
					double[] coldata = new double[rowsM];
					while ( rs.next() )
					{
						coldata[i++] = rs.getDouble(colj);
					}
					colDaValue = new DaValue( coldata );
					break;
				    }
				case java.sql.Types.INTEGER:
				case java.sql.Types.SMALLINT:
				case java.sql.Types.BIGINT:
				    {
					long[] coldata = new long[rowsM];
					while ( rs.next() )
					{
						coldata[i++] = rs.getLong(colj);
					}
					colDaValue = new DaValue( coldata );
					break;
				    }
				case java.sql.Types.TINYINT:
				case java.sql.Types.BIT:
				    {
					byte[] coldata = new byte[rowsM];
					while ( rs.next() )
					{
						coldata[i++] = rs.getByte(colj);
					}
					colDaValue = new DaValue( coldata );
					break;
				    }
				case java.sql.Types.VARCHAR:
				case java.sql.Types.CHAR:
				case java.sql.Types.LONGVARCHAR:
				    {
					String[] coldata = new String[rowsM];
					while ( rs.next() )
					{
					        String d =  rs.getString(colj);
						coldata[i++] = (d == null || d.length() == 0 ) ? " " : d;
						if (debug) System.err.println("coldata = '"+coldata[i-1]+"'");
					}
					colDaValue = new DaValue( coldata );
					break;
				    }
				default:
				    {
					String[] coldata = new String[rowsM];
					while ( rs.next() )
					{
					        String d =  rs.getString(colj);
						coldata[i++] = (d == null || d.length() == 0 ) ? " " : d;
						if (debug) System.err.println("coldata = '"+coldata[i-1]+"'");
					}
					colDaValue = new DaValue( coldata );
					break;
				    }
				} // column type
				// Get the colun name, and then add the column's 
				// DaValue to our return DaValue. 
				colDaValue.setName( rsmd.getColumnName(colj) );
				ret.addElement( colDaValue );
			} // For each column
		} // try block processing ResultSet
       
		catch (Exception e)
		{
			throw (UnableToGetDataException) 
				m_err.log( new UnableToGetDataException(
						   "when processing SQL query " + query).initCause(e));
		}
		finally
		{
			// Free JDBC resources. This involves recovering the Statement used
			// to make the query here. Can't free the Statement in executeQuery
			// because the ReusltSet is processed here - can't close a Statement
			// before processing one of its ResultSets, and can't pass Statement
			// from here to executeQuery in case connection fails and
			// getConnection gets a new connection, which would null the
			// Statement passed from here. So, solution is, ask the ResultSet
			// what was the Statement used to construct it, and specifically
			// free that one.
			// 
			try
			{
				if ( rs != null )
				{
					Statement stmt = rs.getStatement();
					if ( stmt != null ) stmt.close();
					rs.close();
				}
			}
			catch ( Exception e )
			{
				m_err.log(e, "when attempting to free JDBC resources for query "+ query);
			}
		}
		return ret.toAny();
	}
    
	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get_byte:1.0
	//
	public byte
	get_byte(String target,
		 String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		// TODO: implement
		byte _r = (byte)0;
		return _r;
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get_short:1.0
	//
	public short
	get_short(String target,
		  String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		// TODO: implement
		short _r = (short)0;
		return _r;
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get_long:1.0
	//
	public int
	get_long(String target,
		 String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		// TODO: implement
		int _r = 0;
		return _r;
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get_boolean:1.0
	//
	public boolean
	get_boolean(String target,
		    String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		// TODO: implement
		boolean _r = false;
		return _r;
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get_double:1.0
	//
	public double
	get_double(String target,
		   String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		// TODO: implement
		double _r = 0.0;
		return _r;
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get_float:1.0
	//
	public float
	get_float(String target,
		  String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		// TODO: implement
		float _r = 0.0f;
		return _r;
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get_string:1.0
	//
	public String
	get_string(String target,
		   String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		// TODO: implement
		String _r = "";
		return _r;
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get_char:1.0
	//
	public char
	get_char(String target,
		 String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		// TODO: implement
		char _r = (char)0;
		return _r;
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get_wstring:1.0
	//
	public String
	get_wstring(String target,
		    String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		// TODO: implement
		String _r = "";
		return _r;
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get_wchar:1.0
	//
	public char
	get_wchar(String target,
		  String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		// TODO: implement
		char _r = (char)0;
		return _r;
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get_bytea:1.0
	//
	public byte[]
	get_bytea(String target,
		  String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		// TODO: implement
		byte[] _r = new byte[0];
		return _r;
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get_shorta:1.0
	//
	public short[]
	get_shorta(String target,
		   String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		// TODO: implement
		short[] _r = new short[0];
		return _r;
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get_longa:1.0
	//
	public int[]
	get_longa(String target,
		  String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		// TODO: implement
		int[] _r = new int[0];
		return _r;
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get_booleana:1.0
	//
	public boolean[]
	get_booleana(String target,
		     String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		// TODO: implement
		boolean[] _r = new boolean[0];
		return _r;
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get_doublea:1.0
	//
	public double[]
	get_doublea(String target,
		    String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		// TODO: implement
		double[] _r = new double[0];
		return _r;
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get_floata:1.0
	//
	public float[]
	get_floata(String target,
		   String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		// TODO: implement
		float[] _r = new float[0];
		return _r;
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get_stringa:1.0
	//
	public String[]
	get_stringa(String target,
		    String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		// TODO: implement
		String[] _r = new String[0];
		return _r;
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get_chara:1.0
	//
	public char[]
	get_chara(String target,
		  String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		// TODO: implement
		char[] _r = new char[0];
		return _r;
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/get_wchara:1.0
	//
	public char[]
	get_wchara(String target,
		   String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		// TODO: implement
		char[] _r = new char[0];
		return _r;
	}
    


	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/set:1.0
	//
	public void set (
		String query,
		String[] args, 
		org.omg.CORBA.AnyHolder value)
		throws  UnableToSetDataException
	{
		try 
		{
			// TODO: implement, if we want to do support direct write into the DB!
		}
		catch (Exception e)
		{
			throw (UnableToSetDataException)
				m_err.log (new UnableToSetDataException (e.getMessage()));
		}
	}
		    
	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/startMonitor:1.0
	//
	public void
	startMonitor(String name)
		throws  MonitorStartupException
	{
		// TODO: later
	}
		    
	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/stopMonitor:1.0
	//
	public void
	stopMonitor(String name)
		throws  MonitorShutdownException
	{
		// TODO: later
	}
		    
	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/status:1.0
	//
	public int
	status()
	{
		// TODO: implement
		int _r = 0;
		return _r;
	}
		    
	//
	// IDL:dp.aida.slac.stanford.edu/dpRdb/DpRdbI/shutdown:1.0
	//
	public void
	shutdown()
	{
		// If we have a DB connection, dispose of it.
		closeConnection();
	}
		 
		    
        /*
        ** PRIVATE METHOD DEFINITIONS
        */

	/**
	 * Initializes connections to the persistent store that contains all AIDA
	 * directory service information. 
	 */
	private void initDB() 
	{ 
		try
		{
		        m_err.logl("Initializing Oracle database connection");
			DriverManager.registerDriver(
				new oracle.jdbc.driver.OracleDriver());
		}
		catch ( Exception ex )
		{
			m_err.logl( ex, "while registering driver for connection to Oracle.");
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
			m_err.logl( ex, "while getting Oracle password.");
		}
		
		getConnection();

		if ( m_Conn != null )
			m_err.logl("Oracle database connection completed successfully");
		else
			m_err.logl( new ServerInitialisationException(
					   "null Connection to database"));
	}

        /**
	 * Initializes a database connection, to the Oracle Database.
	 *
	 * If the connection is non-null, then the old connection is
	 * first closed. This part is inclued so that this routine can
	 * be used to renew a stale connection.
	 */
	private void getConnection()
	{
		// If we already have a connection dispose of it.
		closeConnection();
		
		// Having dealt with a possible stale connection, get a new one.
		try
		{
			// Get DB connection JDBC URL, using hard coded one (for AIDAPROD)
			// as the default if none given in Properties. Then get connection.
			String connectionString = 
				System.getProperty("AIDA_CONNECTION_STRING",
						   N_CONNECTION_STRING);
			m_err.logl( "Initializing database connection: "+ connectionString );

			m_Conn = DriverManager.getConnection(connectionString); 
			//		   System.getProperty("AIDA_DATABASE_USERID"), password);
		}
		catch (Exception ex)
		{
			m_err.logl( ex, 
				   "while Aida DpRdb initialising connection to database");
		}
	}

	/**
	 * Disposes of existing Db connection.
	 */
	private void closeConnection()
	{
		// If we have a connection, dispose of it.
		try 
		{
			if ( m_Conn != null )
			{
				if ( ! m_Conn.isClosed() ) 
				{
					m_err.logl("Closing Database db connection");
					m_Conn.close();
					m_Conn = null;
				}
			}
		}
		catch ( Exception ex )
		{
			m_err.logl( ex, "while closing database connection");
		}
	}
	
	/**
	 * Examines the "parameters", aka "arguments", passed from the client (those
	 * things given by DaOBject.setParam()).
	 *
	 */
	private String SubstituteArgs( String[] args, String query )
		throws IllegalArgumentException
	{

		// nArgs is the effective number of of parameters. The AIDA
		// CORBA API results in at least 1 arg always, because the AidaObject
		// API does not define versions of each API method with a no-args arg,
		// so AIDA must supply at least 1 args arg, but args[0] to "null". 
		int nArgs = ( (args[0].compareTo("null")==0) ? 0 : args.length );
		
		return query;
	}

	/**
	 * Queries the AIDA Name Server database with the query in
	 * sqlString. This is a wrapper to give appropriate error
	 * handling and retry logic.
	 * 
	 * @param sqlString the SQL query, in "ascii" (actually UTF-16
	 * or whatever java String is).
	 * @return The ResultSet given by stmt.executeQuery(sqlString)
	 * @version 1.0 19-Jun-2005, Greg White
	 */
	private ResultSet executeQuery( String sqlString )
		throws SQLException
	{
		Statement stmt = null;     // The Statement on which the ResultSet is acquired.
		ResultSet rs = null;       // ResultSet receiving SQL results from AIDA query
		                           // Note that ResultSet and Statement are closed by callers.
		int nRetries = 0;
		boolean bRetry = false;

		// Create a jdbc Statement and execute the given query. If the query fails
		// to execute for whatever reason, try to get a new connection and loop -
		// re-creating the statement and re-executing the query. Try up to 3 times.
		do
		{
			try 
			{
				// Create a statement with "Scrollable" ResultSet, as necessary
				// for processing each column as a unit in the get method.
				stmt = m_Conn.createStatement(
					ResultSet.TYPE_SCROLL_INSENSITIVE,
					ResultSet.CONCUR_READ_ONLY);
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
					m_err.log( ex, "when executing SQL query - " + 
						   "retrying with new java.sql.Connection");
					getConnection();
					bRetry = true;
					nRetries++;
				}
				else
				{
					bRetry = false;
					String suppl = "when executing SQL query " + sqlString;
					if ( ex.getClass().getName() == "java.sql.SQLException" ) 
						suppl.concat(": "+((SQLException)ex).getSQLState());
					m_err.log( ex, suppl );
				}
			}
		} while ( bRetry );
				
		if ( rs != null && nRetries < MAX_RETRIES )
			return rs;
		else
			throw (SQLException) 
				m_err.log( new SQLException("Unable to execute query"));  
	}

	/**
	 * Issue message to error log and throw standard exception
	 * @throws UnableToGetDataException all get methods throw this exception on error.
	 */
	private void 
	log( Exception e, String query )
		throws UnableToGetDataException
	{
		// Before issuing error and throwing general
		// exception, try to clean up a little.
		try
		{
			// TODO: add oracle cleanup, if any?
		}
		catch ( Exception ex )
		{
			m_err.log( ex );
		}

		// Now issue error.
		throw (UnableToGetDataException) 
			m_err.log( new UnableToGetDataException(query), 
				   "caused by: " + e.toString());
	}

}

