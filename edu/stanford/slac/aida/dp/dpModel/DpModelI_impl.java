//-*-java-*-
package edu.stanford.slac.aida.dp.dpModel;

import java.util.*;
import java.sql.*;
import java.util.regex.Pattern;
import java.lang.Integer;
import org.omg.CORBA.ORB;
import Jama.Matrix;
import org.omg.PortableServer.*;
import edu.stanford.slac.err.*;
import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.except.*;
import java.lang.IllegalArgumentException;
import java.io.BufferedReader;
import java.io.InputStreamReader;

/**
 * This class defines the (new) AIDA Model Data Provider CORBA server
 * implementation, which gets online model data, computed by XAL, out of the
 * Relational Database.
 *
 * DpModelI_impl implements the AIDA Data Provider "DpModelI" CORBA
 * interface, by extending DpModelIPOA.
 * <p>
 * Aida Parameters: The get methods of this implementation take the following 
 * Ai da parameters 
 * (given as name/value pairs).
 * </p>
 *  <p>
 *   <b>B</b> If given the user wants the rMat from element A (the target) to 
 *   that given in B parameter value. If A is downstream of B, then the inverse 
 *   matrix of A-&gt;B is returned.
 *  </p><p>
 *  <b>TYPE</b> Can be "database" (equivalent to "computed"), or "design".
 *  </p><p>
 *   <b>POS</b> From where to get the model of sliced elements (QUADS, 
 *   sliced BENDS etc). 
 *   May be
 *       "beg", "beginning", "mid", "middle", or "end". Default is "end".
 *  </p><p>
 *   <b>POSB</b> If B param is given and is a sliced element, then POSB may 
 *        be used to indicate to which position, begining, middle, or end, 
 *        in B, the Rmat of A to B should be computed.
 *  </p><p>
 *   <b>MODE</b> The Modelling MODE for which the data is wanted. Devices 
 *        can be modelled in more than one modelling mode. A mode 
 *        identifies a beamline run under particular initial conditions 
 *        and energy scaling. MODEs are identied by an integer in model//modes.
 *  </p><p>
 *   DGRP Deprecated
 *  </p>
 * <p><b>KNOWN BUG:</b> Actually the bug is in the DB, not in code. A.t.t.o.w. 
 * (29th Sep 2008) the 1st section of a multi-section device like 
 * ACCL:IN2:400 has the 
 * model as it existed at the EXIT of that element, so ACCL:IN2:400//twiss 
 * POS=BEG does NOT get the model as it is entering the cavity!</p> 
 *  
 * @author Greg White
 * @version 26-Oct-2012, Partha/Greg - Implemented Oracle wallet support.
 * @version 05-May-2011, Bob Hall. Modified to invoke getPwd to get Oracle password only once during initialization.
 * @version 04-Apr-2011, Bob Hall. Changed to use getPwd to get Oracle password.
 * @version 08-Mar-2011, Bob Hall. Modified default connection string to reference slacprod instead of slac-oracle03.
 * @version 23-Apr-2009, Greg White, Switch default to acquiring RUN=GOLD.
 * @version 30-Mar-2009, Greg White, Add support for GOLD. As released, default
 *          is still RUN=LATEST.
 * @version 14-Jan-2009, Greg White, Add handling of MODE, being the 
 *          combination of beamline and run-time conditions (eg Energy, 
 *          focusing lattice, initial conditions etc).
 * @version 09-Jan-2009, Greg White, Remove code to inverse resulting matrix 
 *          if A is downstream of B.
 * @version 05-Dec-2008, Greg White, Fixed Rmatrix for R14 - was giving 
 *          R13 value in R14.
 * @version 13-Oct-2008, Greg White, Removed ORDER_BY PSI_X for Rmat gets! 
 * @version 29-Sept-2008, Greg White, Fixed handling for multi-segment 
 *          elements like KLYS (eg ACCL:IN20:400). MID gets middle element 
 *          (eg 4th if 7 sections like L0A___1, L0A___2 ... L0A___7).  
 * @version 16-July-2008, Greg White, Added A to B.  
 * @version  9-July-2008, initial version.
 *
 */
public class DpModelI_impl extends DpModelIPOA
{
	private static boolean m_doneInit = false; // Whether = inited this
						   // server.
        private String password;
	private _PersistentDaObject m_da;          // For orb access.
	private ORB m_orb;                         // CORBA ORB for this server.
	private org.omg.PortableServer.POA m_poa;  // Portable Object Adapter
	private Err m_err;                         // Err messaging singleton.

	protected static final boolean debug = false; // Debug information
						      // toggle.

	// DB management
	// TODO: Externalize protect access username/passwords and strings
	// TODO: Convert connection string to java Property.
	private static final String N_CONNECTION_STRING = 
	//"jdbc:oracle:thin:@slacprod.slac.stanford.edu:1521:SLACPROD";
	"jdbc:oracle:oci:/@SLACPROD";
	private static final int MAX_RETRIES = 2;   // Try sql query <= 3 times.
	private static Connection m_Conn = null;    // JDBC connection. 
	private static final int DGRP_PARAM=0;      // Deprecated.
	private static final int MODE_PARAM=1;      // Index of MODE in Params.
	private static final int B_PARAM=2;         // Index of B in Params.
        private static final int TYPE_PARAM=3;      // Index of TYPE in Params.
        private static final int POS_PARAM=4;       // Index of POS in Params.
	private static final int POSB_PARAM=5;      // Index of POSB in Params.
	private static final int RUN_PARAM=6;       // Index of RUN in Params.

	private static final int PSIX = 1, PSIY = 6;// Indeces of PSI twiss
						    // params from DB.
   
        private static final Map indexMap;
	static 
	{
		indexMap = new HashMap(3);
		indexMap.put("BEGIN", new Integer(1));
		indexMap.put("MIDDLE", new Integer(2));
		indexMap.put("END", new Integer(3));
	}

        // Mode 5 is the nominal full machine mode, and the one we try if no
	// runID and no Mode has been given.
	private static final String FULLMACHINE_MODE_DEFAULT = "5"; 

	// Message Strings
        private static final String SET_NOT_SUPPORTED =
		"Set operation is not supported in Aida Model server";
	private static final String N_DATABASE_INIT = 
		"SQLException encountered starting ";
	private static final String INVALIDATTRIBUTE = 
		"Query with unrecognized attribute part received";
	private static final String NORESULTSETMETADATA = 
		"No ResultSet metadata available so " + 
		"cannot construct DaValue return members";
	private static final String INVALIDSYNTAXOFMODE_PARAM = 
	    "Invalid MODE argument syntax, must be 1-99. "; // +
            // ."See AIDA query LCLS//model_modes for valid modes";
	private static final String DGRP_DEPRECATED =
	    "DGRP argument deprecated following transition from SLC Modelling";
        private static final String TYPE_PARAM_VAL_ILLEGAL = 
	     "Specified TYPE param must be valued COMPUTED, DATABASE, EXTANT" + 
	     " (all meaning EXTANT) or DESIGN";
        private static final String POS_PARAM_VAL_ILLEGAL =
	     "Specified position (POS or POSB) param must be valued " + 
		"BEG, BEGINNING (same as BEG), MIDDLE (same as MID), or END";
	private static final String UNABLETOPROCESSSQL = 
	        "Unable to execute SQL query successfully";
	private static final String UNABLETOEXTRATCTRESULTSET = 
	        "Unable to extract data from JDBC Result Set";
	private static final String NOMATCH = 
	        "No matching model data for query ";
	private static final String TOOMANYROWS =
		"Too many matching rows in model database " + 
		" (should be 1 exactly) for query "; 
	private static final String INCONSISTENT_PSI =
		"Inconsistent phase advances in each plane detected "+
		"for A and B";	
	private static final String ILLEGALRUN_PARAM =
	        "The RUNID param, if supplied, must be a positive integer, "+
	        "or 'NULL' for latest run";	
        private static final String NOGOLD = 
	        " model not found in any GOLD model of any MODE";
	private static final String WARNING_FOUNDINRUNID =
		"WARNING: Device model was not found in MODE " + 
		FULLMACHINE_MODE_DEFAULT + 
		"; data returned is from RunId ";
	private static final String WARNING_BOTHMODEANDRUNID =
		"WARNING: Mutually exclusive params supplied: RunID and MODE. "+
		"Only RunID will be used, given MODE ignored.";
	private static final String WARNING_BOTHTYPEANDRUNID =
		"WARNING: Mutually exclusive params supplied: RunID and TYPE. "+
		"Only RunID will be used, given TYPE ignored.";
  
	DpModelI_impl(_PersistentDaObject da, org.omg.PortableServer.POA poa)
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
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/init:1.0
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
			    m_err.log( new ServerInitialisationException(
			    e.getMessage())); 
		}
		m_err.log("DpModelI servant initialization completed.");        
	}
    
	//
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get:1.0
	//
        /**
	 * Retrives the model parameters for a given element, subject to the
	 * AIDA parameters given, and returning an Any.
	 * @param modelledElement/modelledParameter The name of a modelled
	 *        element and the model parameter required for the element,
	 *        given as an AIDA name.
	 * @param args Array of name/value string pairs, as described in class
	 *        comments.
	 * @return DaValue (encapsulated by CORBA Any)
	 */
	public org.omg.CORBA.Any
	get(String query,
	    String[] args)
		throws  UnableToGetDataException
	{
		double[] modelData = get_modelData( query, args );
		
		return new DaValue( modelData ).toAny();
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get_doublea:1.0
	//
        /**
	 * As get:Any, retrives the model parameters for a given element,
	 * subject to the AIDA parameters given, returning an array of doubles.
	 * @param query The name of a modelled element and the model parameter
	 *        required for the element, given as an AIDA name.
	 * @param args Array of name/value string pairs, as described in class
	 *        comments.
	 * @return array of doubles
	 */
	public double[]
	get_doublea(String query,
		    String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		return get_modelData( query, args );
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get_floata:1.0
	//
        /**
	 * Retrives the model parameters for a given element, subject to the
	 * AIDA parameters given, and returning an array of floats.
	 * @param modelledElement/modelledParameter The name of a modelled
	 *        element and the model parameter required for the element,
	 *        given as an AIDA name.
	 * @param args Array of name/value string pairs, as described in class
	 *        comments.
	 * @return array of floats
	 */
	public float[]
	get_floata(String target,
		   String[] args)
		throws edu.stanford.slac.except.UnableToGetDataException
	{
		double[] modelData = get_modelData( target, args );
		
		float[] ret = new float[ modelData.length ];
		for (int i=0;  i<modelData.length; i++)
			ret[i] = (float)modelData[i];
		
		return ret;
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
			m_err.logl( ex, 
                        "while registering driver for connection to Oracle.");
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
			m_err.logl( ex, 
                        "while getting Oracle password.");
		}
		
		getConnection();

		if ( m_Conn != null )
			m_err.logl(
                         "Oracle database connection completed successfully");
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
		
		try
		{
			// Having dealt with a possible stale connection, get a new one.
			String connectionString = 
				System.getProperty( "AIDA_CONNECTION_STRING",
						    N_CONNECTION_STRING );
			//String userId = System.getProperty("AIDA_DATABASE_USERID","AIDA"); 

			m_err.logl( "Initializing database connection: " + 
				    connectionString );

			m_Conn = DriverManager.getConnection(
				connectionString);  
		}
		catch (Exception ex)
		{
			m_err.logl( ex, 
				   "while Aida DpModel initialising " + 
				   "connection to database");
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
					m_err.logl(
					   "Closing Database db connection");
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
	 * Common routine for all get_ methods of dpModel, gets model parameters
	 * (twiss or Rmat) for the given aida name, subject to the parameters
	 * also given.
	 * @param query The name of a modelled element and the model parameter
	 *        required for the element, given as an AIDA name.
	 * @param args  Array of name/value string pairs
	 * @return model data as array of doubles.  
	 */
	private double[] get_modelData( String query, String[] args )
		throws UnableToGetDataException
	{
		/* If the request is just for twiss, get that. If it's for R,
		   then if there is no B device (that is, the Rmat of A to B has
		   NOT been requested, then simply get the Rmat of A), otherwise
		   (A and B have both been given, then get the Rmat from A to B)
		 */
		double[] ret = null;
		DBG("\n\nSeen request to get ",query);

		try
		{
			Query q = new Query(query);
			String[] params = parseArgs(args);

			// If twiss params were requested, get those, otherwise
			// if R matrix was requested, get that. Any other
			// attribute is not recognized (and in fact AIDA
			// shouldn't have made it this far because an AIDA name
			// with invalid attribute part should not be in the AIDA
			// Directory Service).
			if ( q.attribute().equals("twiss") )
			{
				ret = get_twissA( q, params );
			}
			else if ( q.attribute().equals("R") )
			{
				if ( params[B_PARAM].equals("NULL") )
				{
					ret = get_rmatA( q, params ); 
				}
				else // There is a B device, get A to B.
				{
					ret = get_rmat_AtoB( q, params );
				}
			}
			else 
				// Aida query attribute not recognized
				throw (IllegalArgumentException) 
			           m_err.log( new IllegalArgumentException( 
				   INVALIDATTRIBUTE + " : " + q.attribute() ));

		}
		catch ( Exception ex )
		{
			throw (UnableToGetDataException) 
			   m_err.log( new UnableToGetDataException(query).
			   initCause(ex));
		}

		return ret;
	}
	
	/**
	 * Examines the "parameters", passed from the client as part of the AIDA
	 * query.
	 * @param args String array of parameter/value pairs, even numbered
	 * elements (0indexed) are parameter names, odd numbered are the values
	 * (passed as String, even if they're numeric.
	 * @return String array of parameter values, fully instantiated; each
	 * element being the value of the corresponding parameter, as index by
	 * class variable param_INDEX static constants.
	 * @throws IllegalArgumentException if any parameter name or value is
	 * found to be invalid.
	 */
	private String[] parseArgs( String[] args )
		throws IllegalArgumentException
	{
		// Parameters for query, initialized to defaults.
		String[] paramNames=
			{"DGRP","MODE","B","TYPE","POS","POSB","RUN"};
		String[] params=
			{"NULL","NULL","NULL","EXTANT","END","BEG","GOLD"};

		boolean posSpecified = false,
			modeSpecified = false,
			typeSpecified = false,
			goldSpecified = false,
			runIDSpecified = false;
		
		// nArgs is the effective number of of parameters. The AIDA
		// CORBA API results in at least 1 arg always, because the
		// AidaObject API does not define versions of each API method
		// with a no-args arg, so AIDA must supply at least 1 args arg,
		// but args[0] to "null".
		int nArgs = ( (args[0].compareTo("null")==0) ? 0 : args.length );
		
		// Extract the given parameters and override defaults. If an
		// unrecognized argument is given, complain.
		for (int i=0; i< nArgs ; i+=2 ) 
		{
		      if (args[i].equalsIgnoreCase(paramNames[DGRP_PARAM]) )
			      params[DGRP_PARAM] = args[i+1];
		      else if (args[i].equalsIgnoreCase(paramNames[B_PARAM]) )
			      params[B_PARAM] = args[i+1];
		      else if (args[i].equalsIgnoreCase(paramNames[POSB_PARAM]) )
			      params[POSB_PARAM] = args[i+1];
		      else if (args[i].equalsIgnoreCase(paramNames[TYPE_PARAM]) )
		      {
			      params[TYPE_PARAM] = args[i+1];
			      typeSpecified = true;
		      }
		      else if (args[i].equalsIgnoreCase(paramNames[MODE_PARAM]) )
		      {
			      params[MODE_PARAM] = args[i+1].trim();
			      modeSpecified = true;
		      }
                      // For backward compatibility, RUN param can be given
		      // as "RUNID" or "RUN", case-insensitive.
		      else if ( Pattern.matches("^((RUN)(ID)?)|((run)(id)?)$",
				args[i]))
		      {
			      params[RUN_PARAM] = args[i+1].trim();
		      }
		      else if (args[i].equalsIgnoreCase(paramNames[POS_PARAM]) )
		      {
			      params[POS_PARAM] = args[i+1];
			      posSpecified = true;
		      }
		      else
		      {
			      throw (IllegalArgumentException) 
				 m_err.log( new IllegalArgumentException( 
				 args[i]+" is not a recognized argument name"));
		      }
		      
		}

		// DGRP parameter is deprecated. For now, if it's given, we
                // ignore it.  TODO: Later, complain of deprecated argument.
		if ( params[DGRP_PARAM] != "NULL" )
		{
			// throw new IllegalArgumentException(DGRP_DEPRECATED);
		}

                // Check that the type parameter, if specified, is valid.  If
		// specified to be "DATABASE" or "COMPUTED", this is equivalent
		// to "EXTANT". "EXTANT" is default.
		if ( Pattern.matches("^(COMP.*)|(DATA.*)|(EXT.*)$",
				     params[TYPE_PARAM]) )
			params[TYPE_PARAM] = "EXTANT";
		else if ( Pattern.matches("^(DES.*)$",params[TYPE_PARAM]) )
			params[TYPE_PARAM] = "DESIGN";
		else
			throw (IllegalArgumentException) 
			    m_err.log(new IllegalArgumentException(
			    TYPE_PARAM_VAL_ILLEGAL));

                // Check that the POS position parameter is valid.
		if ( Pattern.matches("^(B|b).*$", params[POS_PARAM]) )
			params[POS_PARAM] = "BEGIN";
		else if ( Pattern.matches("^(M|m).*$",params[POS_PARAM]) )  
			params[POS_PARAM] = "MIDDLE";
		else if ( Pattern.matches("^(E|e).*$",params[POS_PARAM]) )
			params[POS_PARAM] = "END";
		else
			throw (IllegalArgumentException)
			    m_err.log(new 
                            IllegalArgumentException(POS_PARAM_VAL_ILLEGAL)); 

                // Check that the POSB position parameter is valid.
		if ( Pattern.matches("^(B|b).*$", params[POSB_PARAM]) )
			params[POSB_PARAM] = "BEGIN";
		else if ( Pattern.matches("^(M|m).*$",params[POSB_PARAM]) )  
			params[POSB_PARAM] = "MIDDLE";
		else if ( Pattern.matches("^(E|e).*$",params[POSB_PARAM]) )
			params[POSB_PARAM] = "END";
		else
			throw (IllegalArgumentException)
			    m_err.log(new 
                            IllegalArgumentException(POS_PARAM_VAL_ILLEGAL)); 

		// Check that the RUNID position parameter is valid. It must be
		// valued "NULL" or 0, or a positive non-leading-zero integer.
		if ( params[RUN_PARAM].equalsIgnoreCase( "NULL" ) || 
		     params[RUN_PARAM].equalsIgnoreCase( "0" ) ||
		     params[RUN_PARAM].equalsIgnoreCase( "LATEST" ) )
		{
			params[RUN_PARAM] = "LATEST";
		}
		else if ( params[RUN_PARAM].equalsIgnoreCase( "GOLD" ) ||
			  params[RUN_PARAM].equals("1") )
		{
			params[RUN_PARAM] = "GOLD";
			goldSpecified = true;
		}
	        // A numerical RunID is an optional one or more 0s,
	        // followed by a number of at least 2 digits. This
	        // distinguishes RUN IDs from 0 = latest, and 1 =
	        // gold.
		else if ( Pattern.matches("^(0+)?[1-9][0-9]+$",
					    params[RUN_PARAM]) )
			runIDSpecified = true;
		else
			throw (IllegalArgumentException)
			    m_err.log(new 
                            IllegalArgumentException(ILLEGALRUN_PARAM));

		// Check syntax of MODE arg, and basic value NULL or
		// (1-99). Real validity should be checked by running a static
		// function at initialization to get valid modes.
		if ( params[MODE_PARAM].equalsIgnoreCase( "NULL" ) || 
		     params[MODE_PARAM].equalsIgnoreCase( "0" ) )
		{
			params[MODE_PARAM] = "NULL";
		}
		else if ( ! Pattern.matches("^[1-9][0-9]?$",
					    params[MODE_PARAM]) )
			throw (IllegalArgumentException)
			    m_err.log(new 
			    IllegalArgumentException(INVALIDSYNTAXOFMODE_PARAM));

		// Both RunID and Mode or type were specified, warn
		// that only RunID is used.
		if ( modeSpecified && runIDSpecified ) 
		{
			params[MODE_PARAM] = "NULL";
			m_err.log(WARNING_BOTHMODEANDRUNID);
		}
		if ( typeSpecified && runIDSpecified )
		{ 
			params[TYPE_PARAM] = "NULL";
			m_err.log(WARNING_BOTHTYPEANDRUNID);
		}

		if (debug) 
		{
			for ( int i = 0; i<params.length; i++)
			    DBG( paramNames[i], params[i]);
		}
 
		return params;
	}

	/**
	 * Acquires the Twiss of a given device, subject to the parameters given.
	 *
	 * This method is algrithmically identical to get_rmatA, but is kept
	 * separate for the sake of explicit coding.
	 */
	private double[] get_twissA( Query a, String[] params )
		throws UnableToGetDataException
	{
		double[] modeldata = null; // The twiss params of device a.
		Integer runID;             // The largest (latest) run ID of a
					   // model containing the device A.

		// Extract the individual parameters, as passed and defaulted.
		String  typeParam =  params[TYPE_PARAM],
		        posParam =   params[POS_PARAM], 
			runIdParam = params[RUN_PARAM], 
			modeParam =  params[MODE_PARAM];
		
		// TODO: Collapse LATEST and GOLD processing, so we don't
		// have two identical blocks.	      
		
		// If the runID has been given, use that runID;
		if ( ! (runIdParam.equals("LATEST") ||
		        runIdParam.equals("GOLD")) )
		{
			try
			{
			       modeldata = get_twiss( a, 
						      typeParam, 
						      posParam,
						      runIdParam,
						      "NULL" ); // null MODE 
			}
			catch ( Exception ex )
			{
				throw new UnableToGetDataException( 
				    "Check use of Run ID "+runIdParam);
			}
		}
		else if ( runIdParam.equals("GOLD") )
		{
			// GOLD run is required, see if MODE was given.
			if ( ! modeParam.equals("NULL") )   
			{
				// MODE has been given, so get latest GOLD model of
				// that MODE.
				try
				{
					modeldata = get_twiss( a,
							       typeParam, 
							       posParam,
							       "GOLD", // GOLD Run 
							       modeParam );
				}
				catch ( Exception ex )
				{
					throw new UnableToGetDataException( 
					    "Check use of MODE "+modeParam);
				}
			}
			else 
			{
				// GOLD given, but not MODE; try MODE 5
				// first, then with any MODE.
				runID = get_runID( a.instance(),
						   typeParam,
						   posParam,
						   FULLMACHINE_MODE_DEFAULT,
						   "GOLD");
				if ( runID == 0 )
				{
					runID = get_runID( a.instance(),
							   typeParam,
							   posParam,
							   "NULL", // NULL MODE
							   "GOLD");// RUN 
					if ( runID == 0 )
					{
						throw new UnableToGetDataException( 
					            a.instance() + 
						    " model not found in any MODE");
					}
					else
					{
						m_err.log( a.instance() + 
						   WARNING_FOUNDINRUNID +
						   runID);
					}
				}
				// We have the max runID that contains the device in a GOLD
				// MODE run, so get the model data with that runID.
				modeldata = get_twiss( a, 
						       typeParam,
						       posParam,
						       runID.toString(),
						       "NULL" ); // MODE
			}	

		}
		else if ( runIdParam.equals("LATEST") )
		{    
			// LATEST run is required, see if MODE was given.
			if ( ! modeParam.equals("NULL") )   
			{
				// MODE has been given, so get latest model of
				// that MODE.
				try
				{
					modeldata = get_twiss( a,
							       typeParam, 
							       posParam,
							       "NULL",     // 
							       modeParam );
				}
				catch ( Exception ex )
				{
					throw new UnableToGetDataException( 
					    "Check use of MODE "+modeParam);
				}
			}
			else
			{
				// Neither RunID nor MODE given, try MODE 5
				// first, then with any MODE.
				runID = get_runID( a.instance(),
						   typeParam,
						   posParam,
						   FULLMACHINE_MODE_DEFAULT,
						   runIdParam);
				if ( runID == 0 )
				{
					runID = get_runID( a.instance(),
							   typeParam,
							   posParam,
							   "NULL",
							   runIdParam);
					if ( runID == 0 )
					{
						throw new UnableToGetDataException( 
					            a.instance() + 
						    " model not found in any MODE");
					}
					else
					{
						m_err.log( a.instance() + 
						   WARNING_FOUNDINRUNID +
						   runID);
					}
				}
				// We have the max runID that contains the device in any
				// MODE, so get the model data with that runID.
				modeldata = get_twiss( a, 
						       typeParam,
						       posParam,
						       runID.toString(),
						       "NULL" );
			}	
		}
		return modeldata;
	}

	/**
	 * Acquires the R matrix data of a given device, subject to the
	 * parameters given.
	 *
	 * This method is algrithmically identical to get_twissA, but is kept
	 * separate for the sake of explicit coding.
	 */
	private double[] get_rmatA( Query a, String[] params )
		throws UnableToGetDataException
	{
		double[] modeldata = null; // 6x6 Rmat of A column order (1st 6
					   // are 1st column)
		Integer runID;             // The largest (latest) run ID of a
					   // model containing the device A.

		// Extract the individual parameters, as passed and defaulted.
		String  typeParam =  params[TYPE_PARAM],
		        posParam =   params[POS_PARAM], 
			runIdParam = params[RUN_PARAM], 
			modeParam =  params[MODE_PARAM];
		
		
		// If the runID has been given, use that runID;
		if ( ! (runIdParam.equals("LATEST") ||
		        runIdParam.equals("GOLD")) )
		{
			try
			{
			       modeldata = get_rmat( a, 
						     typeParam, 
						     posParam,
						     runIdParam,
						     "NULL" ); // MODE
			}
			catch ( Exception ex )
			{
				throw new UnableToGetDataException( 
				    "Check use of Run ID "+runIdParam);
			}
		}
		else if ( runIdParam.equals("GOLD") )
		{
			// GOLD run is required, see if MODE was given.
			if ( ! modeParam.equals("NULL") )   
			{
				// MODE has been given, so get latest GOLD model of
				// that MODE.
				try
				{
					modeldata = get_rmat( a,
							      typeParam, 
							      posParam,
							      "GOLD",
							      modeParam );
				}
				catch ( Exception ex )
				{
					throw new UnableToGetDataException( 
					    "Check use of MODE "+modeParam);
				}
			}
			else
			{
				// GOLD wanted, but no MODE specified MODE
				// given; try MODE 5 first, then with any MODE.
				runID = get_runID( a.instance(),
						   typeParam,
						   posParam,
						   FULLMACHINE_MODE_DEFAULT,
						   "GOLD");
				if ( runID == 0 )
				{
					runID = get_runID( a.instance(),
							   typeParam,
							   posParam,
							   "NULL", // MODE
							   "GOLD");
					if ( runID == 0 )
					{
						throw new UnableToGetDataException( 
					            a.instance() + NOGOLD ); 
					}
					else
					{
						m_err.log( a.instance() + 
						   WARNING_FOUNDINRUNID +
						   runID);
					}
				}
				// We have the max runID that contains the device in any
				// MODE, so get the model data with that runID.
				modeldata = get_rmat( a, 
						      typeParam,
						      posParam,
						      runID.toString(),
						      "NULL" ); // MODE
			}	
		}		
		else // RunParam == LATEST
		{       // RunParam = LATEST, see if MODE was given.
			if ( ! modeParam.equals("NULL") )   
			{
				// MODE has been given, so get latest model of
				// that MODE.
				try
				{
					modeldata = get_rmat( a,
							      typeParam, 
							      posParam,
							      "NULL",
							      modeParam );
				}
				catch ( Exception ex )
				{
					throw new UnableToGetDataException( 
					    "Check use of MODE "+modeParam);
				}
			}
			else
			{
				// Neither RunID nor MODE given, try MODE 5
				// first, then with any MODE.
				runID = get_runID( a.instance(),
						   typeParam,
						   posParam,
						   FULLMACHINE_MODE_DEFAULT );
				if ( runID == 0 )
				{
					runID = get_runID( a.instance(),
							   typeParam,
							   posParam,
							   "NULL" );
					if ( runID == 0 )
					{
						throw new UnableToGetDataException( 
					            a.instance() + 
						    " model not found in any MODE");
					}
					else
					{
						m_err.log( a.instance() + 
						   WARNING_FOUNDINRUNID +
						   runID);
					}
				}
				// We have the max runID that contains the device in any
				// MODE, so get the model data with that runID.
				modeldata = get_rmat( a, 
						      typeParam,
						      posParam,
						      runID.toString(),
						      "NULL" );
			}	
		}
		return modeldata;
	}


	private double[] get_twiss( Query q, 
				    String typeParam, String posParam, 
				    String runParam, String modeParam  )
		throws UnableToGetDataException
	{
		// Spell out mapping of overloaded RunParam to the two
		// args to GET_MODE_TWISS_DATA which implement it.
		String run = null;
		String gold = null;

		if ( runParam.equals("GOLD") || runParam.equals("1") )
		{
			run = null;
			gold = "1";      // Gold wanted, run arg is ignored
		}
		else if ( runParam.equals("NULL") || 
			  runParam.equals("LATEST") || runParam.equals("0") )
		{
			run = null;
			gold = null;
		}
		else
		{
			run = runParam;  // Explict runid, gold arg is ignored
			gold = null;
		}
		
				
                // Form the SQL query that will get the model data. See comments
                // in get_rmat.
		String sql = 
		    "SELECT KINETIC_ENERGY, " +
			"PSI_X, BETA_X, ALPHA_X, ETA_X, ETAP_X,"+
			"PSI_Y, BETA_Y, ALPHA_Y, ETA_Y, ETAP_Y,"+
			"ZPOS, LEFF, SLEFF, ORDINAL FROM "+
			"TABLE(MACHINE_MODEL.TEST_ONLINE_MODEL_PKG.GET_MODE_TWISS_DATA("+  
		         "'" + q.instance() +"'" +  
                        ",'" + posParam     +"'" + 
                        "," + (typeParam.equals("NULL") ? "null" : 
                              "'"+typeParam+"'") + 
                        "," + (modeParam.equals("NULL") ? "null" : 
                              "'"+modeParam+"'") + 
                        "," + (run == null  ? "null" : "'"+run+"'") +
	                "," + (gold == null ? "null" : "'"+gold+"'") +
		        ")) T ORDER BY PSI_X";

		return executeQuery( sql, (Integer)indexMap.get(posParam) );
	}

	private double[] get_rmat( Query q, 
				   String typeParam, String posParam, 
				   String runParam, String modeParam )
		throws UnableToGetDataException
	{
		String gold = null;
		String run = null;

		if ( runParam.equals("GOLD") || runParam.equals("1") )
		{
			run = null;
			gold = "1";      // Gold wanted, run arg is ignored
		}
		else if ( runParam.equals("LATEST") || 
			  runParam.equals("NULL") || runParam.equals("0") )
		{
			run = null;
			gold = null;
		}
		else
		{
			run = runParam;  // Explict runid, gold arg is ignored
			gold = null;
		}

		// Form the SQL query that will get the model data. We specify
		// that the order must be by PSI_X because if we didn't rows
		// come back in arbitrary order, and that's no good when getting
		// data for elements such as ACCL, or KLYS, since those return
		// one row for each segment (like L0A___1, L0A___2 etc). In such
		// cases just one row will be picked by executeQuery, the one
		// corresponding to the POS arg given in the aida query.
		String sql = 
		    "SELECT R11, R12, R13, R14, R15, R16,"+
		    "R21, R22, R23, R24, R25, R26,"+
		    "R31, R32, R33, R34, R35, R36,"+
		    "R41, R42, R43, R44, R45, R46,"+
		    "R51, R52, R53, R54, R55, R56,"+
		    "R61, R62, R63, R64, R65, R66 FROM TABLE("+
		    "MACHINE_MODEL.TEST_ONLINE_MODEL_PKG.GET_MODE_RMATRIX_DATA(" +  
		     "'" + q.instance() +"'," + 
                     "'" + posParam + "',"+
		     (typeParam.equals("NULL") ? "null" : "'"+typeParam+"'") + 
		     "," + (modeParam.equals("NULL") ? "null" : "'"+modeParam+"'") + 
                     "," + (run == null ? "null" : "'"+run+"'") +
	             "," + (gold == null ? "null" : "'"+gold+"'") +
                     ")) T";
		DBG("sql for Rmat", sql );

		return executeQuery( sql, (Integer)indexMap.get(posParam) );
	}

		
	/**
	 * Computes the transfer matrix from a device A to device B, given the
	 * additional constraints specified in the given parmameters.
	 */
	private double[] get_rmat_AtoB( Query q, String[] params )
		throws UnableToGetDataException
	{
		// 6x6 Rmats of A and B in column order (1st 6 are 1st column)
		double[] rmatA = null;     // The 6x6 of device A 
		double[] rmatB = null;     // The 6x6 of device B 
		double[] rmatU = null;     // The 6x6 of the upstream device
		double[] rmatD = null;     // The 6x6 of the downstream device 

		// Extract the individual parameters, as passed and defaulted.
		String  typeParam = params[TYPE_PARAM],
			BParam = params[B_PARAM],
		        posParam = params[POS_PARAM], 
			posBParam = params[POSB_PARAM], 
			runIdParam = params[ RUN_PARAM], 
			modeParam = params[ MODE_PARAM];

		// Extract the names of the devices A and B
		Query a = q;                   // Device A, as given in query 
		Query b = new Query( BParam ); // Device B (as se in B=<device> param
		Query u = null;                // The upstream device
		Query d = null;                // The downstream device.
		String posU = null;            // The position in the device, of
					       // the model required by the user
					       // for the upstream device.
		String posD = null;            // As posU for the downstream device.
		Integer runIdD;                // The run ID of the downstream device.

		// Check which device is upstream of the other. This is needed
		// because to compute the transfer matrix, th einput rmats must
		// of course come from the same model. We know that by
		// convention all modes start at the cathode and go to a
		// desitinaton. Therefore, a model that contains the downsream
		// device must contain the upstream device too. Therefore, to
		// get the rmats of the two devices from the same model, we
		// first establish the runid to be used using the downstream
		// device, and then get the upstream device from the same
		// runid. To so this, we establish the upstream (u) and
		// downstream (d) device names and positions, and later assign
		// the rmats found for u and d to A and B.
		boolean AisDownstreamofB = isDownstream(a, b, params);
		if ( AisDownstreamofB )
		{
			u = b; posU = posBParam; 
			d = a; posD = posParam;
		}
		else
		{
			u = a; posU = posParam;
			d = b; posD = posBParam;
		}

		// First get the Rmat of u and the Rmat of d. Then compute
		// transfer matrix between them. If the RunID was supplied, then
		// get the Rmat using that RunID and ignore the mode arg
		// completely. If RunID was not given, look for the latest RunID
		// that contains d, subject to: if the MODE arg is given, use
		// it, otherwise try MODE 5 first, and if that doesn't work,
		// finally just use any MODE.
		if ( ! ( runIdParam.equals("LATEST") || runIdParam.equals("GOLD")) )
		{
			try
			{
				// Get rmats of u and d devices using MODE="NULL" since
				// RunId is given.
				rmatU = get_rmat( u, 
						  typeParam, 
						  posU,
						  runIdParam,
						  "NULL" ); // MODE
				rmatD = get_rmat( d, 
						  typeParam, 
						  posD,
						  runIdParam,
						  "NULL"); // MODE
			}
			catch ( Exception ex ) 
			{
				throw new UnableToGetDataException( 
				    "Check use of Run ID "+runIdParam);
			}
		}
		else 
		{
                        // RunId has not been given, so we must find the max
                        // runID (using get_RunID) that contains the downstream
                        // device d. Make using given MODE if one was given, or
                        // if no mode given then using MODE=5, and if all else
                        // fails by using any mode.
			if ( ! modeParam.equals("NULL") )
			{
				runIdD = get_runID( d.instance(), 
						    typeParam,
						    posD,
						    modeParam,
						    runIdParam ); 
				if ( runIdD == 0 )
				{
					throw new UnableToGetDataException(
					    d.instance() + ": No " + typeParam + 
                                            " model was found in mode " + 
                                            modeParam);  
				}
				// Now we have a runId for d, get both rmats for
				// both u and d device (A and B) using that
				// runID (ignoring now MODE).
				rmatD = get_rmat( d, 
						  typeParam, 
						  posD,
						  runIdD.toString(),
						  "NULL"); // MODE
				// get U with downstream's runID
				rmatU = get_rmat( u, 
						  typeParam, 
						  posU,
						  runIdD.toString(),  
						  "NULL"); // MODE
			}
			else
			{
				// Neither runId nor Mode has been given, try
				// mode 5 first. If not found, try any mode. If
				// not found in any mode, throw exception. If
				// found in a non-mode 5 mode, issue a warning.
				runIdD = get_runID( d.instance(), 
						    typeParam,
						    posD,
						    FULLMACHINE_MODE_DEFAULT,
						    runIdParam ); 
				if ( runIdD == 0 )
				{
					runIdD = get_runID( d.instance(), 
							    typeParam,
							    posD,
							    "NULL",
							    runIdParam ); 
					if ( runIdD == 0 )
						throw new 
                                                   UnableToGetDataException(
                                                   d.instance() + " " + typeParam +
                                          " model was not found in any mode ");  
					else
						// Device not in 5, but found in
						// RunID.
						m_err.log( d.instance() + 
						   WARNING_FOUNDINRUNID +
						   runIdD);
				}
				
				// So now we have a RunID that contains at least
				// the downstream device (d) - so because all
				// beamlines start at Cathode, it should also
				// contain the upstream device (u). Proceed to
				// get Rmats of u and d using this RunID.
				rmatD = get_rmat( d, 
						  typeParam, 
						  posD,
						  runIdD.toString(),
						  "NULL");
				// get U using Downstream's runid. 
				rmatU = get_rmat( u, 
						  typeParam, 
						  posU,
						  runIdD.toString(), 
						  "NULL"); 
			} // endif mode given switch
		} // endif RunID given switch

		// Assign rmats for A and B depending on which was, in fact,
		// upstream and downstream.
		if ( AisDownstreamofB )
		{
			rmatA = rmatD;
			rmatB = rmatU;
		}
		else
		{
			rmatA = rmatU;
			rmatB = rmatD;
		}

		// Now we have the Rmat of A and of B, compute the transfer matrix.
		Matrix A = new Matrix( rmatA, 6 ).transpose();
		if (debug) printMatrix( "rmatA", A );
		Matrix B = new Matrix( rmatB, 6 ).transpose();
		if (debug) printMatrix( "rmatB", B );
		Matrix RmatAB = B.times(A.inverse());
		if (debug) printMatrix( "RmatAB", RmatAB); 
		
		return RmatAB.getRowPackedCopy();
	}		


	/**
	 * get_runID returns the model upload RunID of the chronologically
	 * most recently uploaded model for the device matching the input
	 * arguments, if one exists, and 0 otherwise; in this signature, the
	 * run param can be used to confine the search to GOLD models.
	 *
	 * @param instance An EPICS channel access addressible device name, or
	 * more sepcifically an XAL element EPICS name, which should match an
	 * Element in the model upload/Symbols table.
	 * @param type A model upload type - valid values are "EXTANT" or
	 * "DESIGN".
	 * @param mode The model run conditions "mode"; this is a String
	 * containing an integer whose value is one of the recognized model
	 * modes. Presently the valid model modes are "5","51","52" and "53".
	 * @param run Specifies whether the search should be for the
	 * chronologically most recent run matching above criteria, or
	 * specifically chronologically most recent GOLD run matching
	 * other criteria. Run is a String which must be given valued
	 * "LATEST" or "GOLD". If mode is null, then GOLD is ignored! 
	 *
	 * mode | run  | result
         * -----------------------------------------------------------------
	 * N    | null | Get runid of latest run of mode N
	 * N    | 1    | Get runid of latest gold run of mode N
	 * null | null | Get runid of latest run of any mode
	 * null | 1    | ERROR. Not permitted, must supply a mode to get gold.
	 *
	 * @return An int giving the highest valued Run ID which matched the
	 * input arguments, or 0 if no model matching the input arguments was
	 * found.
	 */
	private int get_runID( String instance, 
			       String type, 
			       String pos,
			       String mode,
			       String run)
	{
		double[] dbresult = null;  // The result of the SQL query.

		String sql = 
			"Select MACHINE_MODEL.TEST_ONLINE_MODEL_PKG.GET_MODE_RUNID("+  
			"'" + instance +"'," + 
			(type.equals("NULL") ? "null" : "'"+type+"'") + "," +
			(mode.equals("NULL") ? "null" : "'"+mode+"'") + "," +
			(run.equals("GOLD") && !mode.equals("NULL") ? "1" : "null" ) +
			") as run_id from dual";
		DBG( "get runID sql", sql );

		try 
		{
			dbresult = executeQuery( sql, 
						 (Integer)indexMap.get(pos) );
		}
		catch ( Exception ex )
		{
			return 0;
		}
		return (new Double(dbresult[0])).intValue();
	}

	/**
	 * get_runID returns the model upload RunID of the chronologically
	 * most recently uploaded model for the device matching the input
	 * arguments, if one exists, and 0 otherwise.
	 * @param instance An EPICS channel access addressible device name, or
	 * more sepcifically an XAL element EPICS name, which should match an
	 * Element in the model upload/Symbols table.
	 * @param type A model upload type - valid values are "EXTANT" or
	 * "DESIGN".
	 * @param mode The model run conditions "mode"; this is a String
	 * containing an integer whose value is one of the recognized model
	 * modes. Presently the valid model modes are "5","51","52" and "53".
	 * @return An int giving the highest valued Run ID which matched the
	 * input arguments, or 0 if no model matching the input arguments was
	 * found.
	 */
	private int get_runID( String instance, 
			       String type, 
			       String pos,
			       String mode )
	{
		return get_runID( instance, type, pos, mode, "LATEST" );
	}

	/**
	 * Queries the AIDA Name Server database with the query in
	 * sqlString. This is a wrapper to give appropriate error
	 * handling and retry logic.
	 * 
	 * @param sqlString The SQL query, in "ascii" (actually UTF-16
	 * or whatever java String is).
	 * @param position The position within a device composed of more than 1
	 * modelled element, for which the model data is required; regognized
	 * values are 1 (for beginning), 2 (for middle), or 3 (for END). A
	 * mapping is made in this method from these 3 values, to any number of
	 * elements, 1, 2, 3 or more.
	 * @return The ResultSet given by stmt.executeQuery(sqlString)
	 * @version 1.0 19-Jun-2005, Greg White
	 */
	private double[] executeQuery( String sqlString, Integer position )
		throws UnableToGetDataException
	{
		Statement stmt = null;     // The Statement on which the
					   // ResultSet is acquired.
		ResultSet rs = null;       // ResultSet receiving SQL results
		                           // from AIDA query ResultSet and
		                           // Statement are closed by callers.
		int nRetries = 0;
		boolean bRetry = false;
		double[] modelData = null;

	        DBG("sqlString", sqlString); 
		DBG("Position", position.toString() );

		try 
		{

			// Create a jdbc Statement and execute the given
			// query. If the query fails to execute for whatever
			// reason, try to get a new connection and loop -
			// re-creating the statement and re-executing the
			// query. Try up to 3 times.
			do
			{
				try 
				{
					// Create a statement with "Scrollable"
					// ResultSet, as necessary for
					// processing each column as a unit in
					// the get method.
 					stmt = m_Conn.createStatement(
	 				    ResultSet.TYPE_SCROLL_INSENSITIVE,
					    ResultSet.CONCUR_READ_ONLY);
					rs = stmt.executeQuery( sqlString );
					bRetry = false;
				}
				catch ( Exception ex )
				{
					// We encountered an error in the
					// execution of the sql query, so try to
					// fix this by getting a new Oracle
					// connection and set logic so we'll go
					// through the do loop again.
					if ( nRetries < MAX_RETRIES ) 
					{
						m_err.log( ex, 
                                                "when executing SQL query - " + 
						"retrying with new Connection");
						getConnection();
						bRetry = true;
						nRetries++;
					}
					else
					{
						bRetry = false;
						String suppl = 
                                                "when executing SQL query " + 
							sqlString;
						if ( ex.getClass().getName() == 
						     "java.sql.SQLException" ) 
						        suppl.concat(": "+
							((SQLException)ex).
							getSQLState());
						m_err.log( ex, suppl );
					}
				}
			} while ( bRetry );
			
			if ( rs == null || nRetries >= MAX_RETRIES )
				throw new UnableToGetDataException(
				    UNABLETOPROCESSSQL + " against " + 
				    N_CONNECTION_STRING);

			// Extract the data from the (single column) ResulSet. 
			ResultSetMetaData rsmd = rs.getMetaData();
			if (rsmd == null) 
				throw new 
				UnableToGetDataException(NORESULTSETMETADATA); 

			// Get number of rows in ResultSet. Check if got too few
			// (<1)
			rs.last();
			int rowsM = rs.getRow();
			DBG( "rowsM", rowsM );

			if ( rowsM < 1 ) 
				throw (UnableToGetDataException)
			        m_err.log(new UnableToGetDataException(
				NOMATCH + sqlString));

			// Decide the rowset we're going to return. Normally
			// (for all 0-length devices, and devices in more than 1
			// element whose multi-elementness is handled by the SQL
			// procedure (eg QUADs) only 1 rowset will have been
			// retuned. However, some devices correspond to >1
			// element and the SQL query has returned >1 row. This
			// is normal for KLYS, ACCL etc, corresponding to eg
			// elements K21_1D1, K21_1D2, K21_1D3, K21_1D4. For
			// those, decide by checking the pos argument. The pos
			// default is 3 (for END).For sliced waveguides (whether
			// with 1 or more structures), both the begin and end of
			// each "slice" is returned by the SQL. So we simply
			// choose the middle one.
			//
			int m = rowsM; // Default is to take the last returned
				       // rowset.
			if ( m > 1 )
			{
				// If we got >1 row there was >1 modelled element
				// corresponding to the device. 
				switch ( position )
				{
				case 1 :        // BEG, return the 1st rowset.
					m = 1;  
					break;
				case 2 :        // MID, return middle rowset.
					m = (int)Math.ceil(rowsM/2.0);
					break;
				case 3 :        // END, return the last rowset.
					m = rowsM;
					break;
				default :
					m = rowsM; // shouldn't ever happen
				}
			} 
			DBG("Advance to ResultSet # ", m);
 
			// Actually advance to the mth rowset (often just the
			// 1st).
			rs.beforeFirst(); 
			for ( int mi = 0; mi < m; mi++) 
			{
				boolean isok = rs.next();
			}
			// Get number of columns in ResultSet
			int columnsN = rsmd.getColumnCount();
			modelData = new double[ columnsN ];
			DBG("Num Columns", columnsN );

			// For each column, extract all the rows of the column
			// from the ResultSet and add the whole column to what
			// we return.
			for (int colj = 1; colj <= columnsN; colj++)
			{
				modelData[colj-1] = rs.getDouble(colj); 
				if (debug) 
					System.out.println( 
                                        "Data "+(colj-1)+" = "+
					modelData[colj-1] ); 
			}
		}
		catch ( Exception ex )
		{
			throw (UnableToGetDataException)
			    m_err.log( new UnableToGetDataException(
			    UNABLETOPROCESSSQL).initCause(ex));
		}  
		finally
		{
			try
			{
				if ( rs != null ) rs.close();
				if ( stmt != null ) stmt.close();
			}
			catch ( Exception ex )
			{
				m_err.log( ex, "when freeing JDBC resources");
			}
		}
		
		return modelData;
	}


	/**
	 * Static method for issuing messages if static "debug" flag is on. This
	 * method should be inlined by the compiler.
	 */
	private static final void DBG( String context, Object msg )
	{
		if (debug) System.err.println( context + " : " + msg.toString() );
	}

			
	/**
	 * Is element A downstream of element B, established by comparison of
	 * phase advances.
	 *
	 * @throws UnableToGetDataException if inconsistent phase advances
	 * across X and Y.
	 */
	private boolean isDownstream( Query a, Query b, String[] params )
		throws UnableToGetDataException
	{

		// Ensure that POS param used for B, is that whch was passed in
		// POSB. Especially important when a and b are in fact the same
		// device!
		double[] A_twiss = get_twissA( a, params );

		String[] paramsB = params;
		paramsB[POS_PARAM] = params[POSB_PARAM];
		double[] B_twiss = get_twissA( b, paramsB ); 

		// Check that A's X and Y phases advances are > or < B in both
		// planes consistently.
		if ( A_twiss[ PSIX ] > B_twiss[ PSIX ] && 
		       A_twiss[ PSIY ] < B_twiss[ PSIY ] ||
		     A_twiss[ PSIX ] < B_twiss[ PSIX ] && 
		       A_twiss[ PSIY ] > B_twiss[ PSIY ] )
			throw (UnableToGetDataException)
			    m_err.log( 
                            new UnableToGetDataException(INCONSISTENT_PSI));

		// If consistent, any one of these will do to establish
		// downstreamness.
		return (A_twiss[ PSIX ] > B_twiss[ PSIX ]);
	}



	// AIDA DP INTERFACE METHODS WE HAVE NOT IMPLEMENTED FOR THIS SERVER
	//
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/set:1.0
	//
	public void set (
		String query,
		String[] args, 
		org.omg.CORBA.AnyHolder value)
		throws  UnableToSetDataException
	{
		throw (UnableToSetDataException)
		   m_err.log (new UnableToSetDataException(SET_NOT_SUPPORTED));
	}
		    
	//
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/startMonitor:1.0
	//
	public void
	startMonitor(String name)
		throws  MonitorStartupException
	{
		// TODO: later
	}
		    
	//
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/stopMonitor:1.0
	//
	public void
	stopMonitor(String name)
		throws  MonitorShutdownException
	{
		// TODO: later
	}
		    
	//
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/status:1.0
	//
	public int
	status()
	{
		// TODO: implement
		int _r = 0;
		return _r;
	}
		    
	//
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/shutdown:1.0
	//
	public void
	shutdown()
	{
		// If we have a DB connection, dispose of it.
		closeConnection();
	}

	//
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get_byte:1.0
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
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get_short:1.0
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
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get_long:1.0
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
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get_boolean:1.0
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
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get_double:1.0
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
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get_float:1.0
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
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get_string:1.0
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
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get_char:1.0
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
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get_wstring:1.0
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
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get_wchar:1.0
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
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get_bytea:1.0
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
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get_shorta:1.0
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
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get_longa:1.0
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
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get_booleana:1.0
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
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get_stringa:1.0
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
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get_chara:1.0
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
	// IDL:dp.aida.slac.stanford.edu/dpModel/DpModelI/get_wchara:1.0
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

	private static void printMatrix( String label, Matrix M)
	{
		System.out.println(label);
		M.print(8,4);
	}
}


class Query
{
	private String m_queryname = null;
	private String m_instance = null;
	private String m_attribute = null;

	Query( String queryname ) 
	{
		String[] iaPair = queryname.split(AQSToken.SEPARATOR);
		m_instance = iaPair[0];
		if ( iaPair.length == 1 )
			m_attribute = null;
		else
			m_attribute = iaPair[1];
		if (DpModelI_impl.debug) 
			System.out.println("I = '"+m_instance +
					   "', A = '"+m_attribute+"'");
	}
    
	protected String instance() 
	{ 
		return m_instance;
	}
 
	protected String attribute()
	{
		return m_attribute;
	}
}



