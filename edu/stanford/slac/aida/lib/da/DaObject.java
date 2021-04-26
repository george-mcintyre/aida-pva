/*
 * Name: DaObject.java
 *
 * Created on June 25, 2002, 2:20 PM
 * Contains all methods	to enable communication with the DaServer
 * Includes all conversion functions from CORBA	types to native	language structures
 * Mods:
 */

package	edu.stanford.slac.aida.lib.da;

import java.util.*;
import org.omg.CORBA.*;
import edu.stanford.slac.aida.lib.util.common.*;    // AIDA common library routines.
import edu.stanford.slac.aida.lib.dp.*;             // Internal Aida Server API.
import edu.stanford.slac.except.*;                  // General purpose (checked) exceptions.


/** The basic AIDA data access API class. This is the user-level class
 * that encapsulates an AIDA Data Access Object. Use methods of this class to get data.
 * @author July-2002, George McIntyre
 * @version 04-Dec-2006, Bob Hall (rdh)
 * Added logging of client request for set operation to methods set and setDaValue. 
 * @version 17-Nov-2006, Bob Hall (rdh)
 * Added set method.
 * @version 18-Oct-2006, Bob Hall (rdh)
 * Changed method setDaValue to accept a DaValue argument and return a DaValue.
 * @version 28-Sep-2006, Bob Hall (rdh)
 * Added the passing of arguments to the AidaObjectI set method in method setDaValue.
 * @version 18-Sep-2006, Bob Hall (rdh)
 * Added method setDaValue. 
 * @version 07-Oct-2004, Greg White (greg)
 * Use args from reference when called with DaReference.
 * @version 28-Sep-2004, Greg White (greg)
 * Removed err import, not used here.
 * @version 17-Sep-2004, Greg White (greg):
 * More exception handling and comments and javadoc.
 * @version 16-Sep-2004, Greg White (greg): 
 * Add exception  handling. Up-translate constructor exceptions to UNCHECKED, so 
 * users of the basic AIDA API don't have to add code to catch 
 * initialization exceptions.
 * @version 20-Nov-2002, Ron MacKenzie:
 * Convert from using aida exceptions to package except.
 */
public class DaObject extends _DaObject
{   
	/** Callable interface of the Data Access Server (DaServer).  This interface is
	 * resolved automatically on creation of a DaObject.
	 */    
	public AidaObjectI	m_da;
    
	/** No-argument constructor of DaObject. Use when there are no command line 
	 * @throws AidaInitException Probably CORBA failed to initialize.
	 */
	public DaObject()
		throws AidaInitException 
	{
		super();
	}
    
	/** Constructs a DaObject with command line arguments
	 * @param args Command line arguments that may contain CORBA arguments that
	 * expert users may wish to pass to the low level subsystems to
	 * custom initialise the ORB
	 * @throws AidaInitException Probably CORBA failed to initialize.
	 */
	public DaObject( String args[] )
		throws AidaInitException 
	{
		super(args);
	}
    
	/** Constructs a DaObject with command line arguments and conditional
	 * event service init
	 * @param args Command line arguments that may contain CORBA arguments that
	 * expert users may wish to pass to the low level subsystems to
	 * custom initialise the ORB
	 * @param events Initialise event service?
	 * @throws AidaInitException Probably CORBA failed to initialize.
	 */
	public DaObject( String args[], boolean events)
		throws AidaInitException 
	{
		super(args, events);
	}
    
	/**
	 * Initialise any fields
	 */
	protected void init() throws AidaInitException
	{
		super.init();
	
		// Get the interface to the DaServer for use with all calls
		// Note: This is for Da Clients only.  Servers operate in pull mode,
		// therefore only clients should instantiate DaObjects
		// Servers should instantiate _DaObjects directly
		m_da = getAidaServerI(_PersistentDaObject.PDO_SERVER_NAME);
		m_args = new Hashtable();
	}
    
    /** This method is used to set global Data Access Object parameters.
     * These parameters will be used each time any service is called via this
     * connection either directly or via a reference object.
     * This version allows multiple parameters to be set at once
     * @param parmlist parameter/value pairs in the form 
     * "parm=value;parm=value;parm=value;"
     */
    public void	setParam(String	parmlist)
    {
        String[] pairs = parmlist.split (";");
        for (int i = 0; i < pairs.length; ++i)
        {
            String[] pair = pairs[i].split ("=");

            // silently ignore ill-formed parameters for sake of compatability
            if (pair.length == 2)
                setParam (pair[0], pair[1]);
        }
    }
    
	/** This method is used to set global Data Access Object parameters.
	 * These parameters will be used each time any service is called via this
	 * connection either directly or via a reference object.
	 *
	 * @param name Parameter name.  Has meaning only in implementing services
	 * @param value New value for given parameter name
	 */
	public void	setParam(String	name, String value)
	{
		m_args.put(name, value);
		return ;
	}
    
	/** Reset this DaObject.  Use this method to clear any parameters that have
	 * been set for this DaObject.
	 */
	public void	reset()
	{
		m_args.clear();
	}
    
	/** Gets data specified by a given DaReference.  Returns an intrinsic Object.
	 * @param r A DaReference object that encapsulates a well formed parsed
	 * AQS (AIDA Query Specification) query.  The type of this reference
	 * must have either been successfully gleaned from the compilation
	 * process or set manually with setType()
	 * @return An arbitrary intrinsic object.  The user may call generic methods
	 * on the returned object, like toString() but in order to use the
	 * value the programmer needs to know what type was really returned
	 * and cast the returned value to that type.
	 * @throws UndefinedNameException when the Directory Service cannot resolve the 
	 * query in the given DaReference. 
	 */
	public java.lang.Object get(DaReference r)
		throws UndefinedNameException, UnableToGetDataException
	{
		return r.get(this, r.getArgs());
	}
    
	/** Gets data referenced by given target.  Returns an intrinsic Object.
	 * @param target A AQS string target to be evaluated
	 * @param type The expected return type of the query
	 * @return An arbitrary intrinsic Object.  The user may call generic methods
	 * on the returned object, like toString() but in order to use the
	 * value the programmer needs to know what type was really returned
	 * and cast the returned value to that type.
	 * @throws UnableToGetDataException error executing query
	 */    
	public java.lang.Object get(String target, int type) 
		throws UnableToGetDataException
	{
		return doGet(m_da, type, target,  getArgs() );
	}
    
	/** Internal intrinsic get helper function
	 * @param I Interface to use
	 * @param type Expected return type
	 * @param target String target reference
	 * @param args Argument list
	 * @return An arbitrary intrinsic object.  The user may call generic methods
	 * on the returned object, like toString() but in order to use the
	 * value the programmer needs to know what type was really returned
	 * and cast the returned value to that type.
	 * @throws UnableToGetDataException error executing query
	 * @throws IllegalArgumentException the type arg did not match
	 * a member of <code>DaValue.Type</code>
	 */    
	private java.lang.Object doGet(AidaObjectI I, int type, String target, String [] args)
		throws UnableToGetDataException
	{

		switch (type)
		{
		case  DaValue.Type.BYTE:
			return (new Byte(I.get_byte( target, args )));
		    
		case  DaValue.Type.SHORT:
		case  DaValue.Type.USHORT:
			return (new Short(I.get_short( target, args )));
		    
		case  DaValue.Type.LONG:
		case  DaValue.Type.ULONG:
		case  DaValue.Type.LONGLONG:
		case  DaValue.Type.ULONGLONG:
			return (new Long((long)I.get_long( target, args )));
		    
		case  DaValue.Type.BOOLEAN:
			return (new Boolean(I.get_boolean( target, args )));
		    
		case  DaValue.Type.DOUBLE:
		case  DaValue.Type.LONGDOUBLE:
			return (new Double(I.get_double( target, args )));
		    
		case  DaValue.Type.FLOAT:
			return (new Float(I.get_float( target, args )));
		    
		case  DaValue.Type.STRING:
			return (I.get_string( target, args ));
		    
		case  DaValue.Type.CHAR:
			return (new Character(I.get_char( target, args )));
		    
		case  DaValue.Type.WSTRING:
			return (I.get_wstring( target, args ));
		    
		case  DaValue.Type.WCHAR:
			return (new Character(I.get_wchar( target, args )));

		default:
			throw new IllegalArgumentException("Unrecognized type in get()");
		}

	}
    
	/** Gets given DaReference.  Return an array of intrinsic objects
	 * @param r A DaReference object that encapsulates a well formed parsed
	 * AQS (AIDA Query Specification) query.  The type of this reference
	 * must have either been successfully gleaned from the compilation
	 * process or set manually with setType()
	 * @return An arbitrary array of intrinsic objects.  The user may call generic methods
	 * on the returned array elements, like toString() but in order to use the
	 * values the programmer needs to know what type was really returned
	 * and cast the returned value to that type.
	 * @throws UndefinedNameException query string is invalid; not in Directory Service or
	 * bad syntax.
	 * @throws UnableToGetDataException error executing query
	 */
	public java.lang.Object [] geta(DaReference r) 
		throws UnableToGetDataException, UndefinedNameException
	{
		return r.geta(this, r.getArgs());
	}
    
	/** Gets data referenced by a given target. 
	 * @param target A AQS string target to be evaluated
	 * @param type The expected return type of the query
	 * @return An arbitrary array of intrinsic objects.  The user may call generic methods
	 * on the returned array elements, like toString() but in order to use the
	 * values the programmer needs to know what type was really returned
	 * and cast the returned value to that type.
	 * @throws UnableToGetDataException error executing query
	 */    
	public java.lang.Object [] geta(String target, int type) 
		throws UnableToGetDataException
	{
		return doGeta(m_da, type, target, getArgs());
	}
    
	/** Calls the actual type-specific get method of the data
	 * provider of the given target, through that data provider's
	 * CORBA callable interface stub. This is intended to be an
	 * internal helper function through which the public functions
	 * go.
	 * @param I Interface to use
	 * @param type Expected return type
	 * @param target String target reference
	 * @param args Argument list
	 * @return An arbitrary array of intrinsic objects.  The user may call generic methods
	 * on the returned array elements, like toString() but in order to use the
	 * values the programmer needs to know what type was really returned
	 * and cast the returned value to that type.
	 * @throws UnableToGetDataException error executing query
	 */    
	private java.lang.Object [] 
	doGeta(AidaObjectI I, int type, String target, String [] args) 
		throws UnableToGetDataException
	{
		switch (type)
		{
		case  DaValue.Type.BYTEA:
			return getByteArray(I.get_bytea( target, args ));
		    
		case  DaValue.Type.SHORTA:
		case  DaValue.Type.USHORTA:
			return getShortArray(I.get_shorta( target, args ));
		    
		case  DaValue.Type.LONGA:
		case  DaValue.Type.ULONGA:
		case  DaValue.Type.LONGLONGA:
		case  DaValue.Type.ULONGLONGA:
			return getLongArray(I.get_longa( target, args ));
		    
		case  DaValue.Type.BOOLEANA:
			return getBooleanArray(I.get_booleana( target, args ));
		    
		case  DaValue.Type.DOUBLEA:
		case  DaValue.Type.LONGDOUBLE:
			return getDoubleArray(I.get_doublea( target, args ));
		    
		case  DaValue.Type.FLOATA:
			return getFloatArray(I.get_floata( target, args ));
		    
		case  DaValue.Type.STRINGA:
		case  DaValue.Type.WSTRINGA:
			return (I.get_stringa( target, args ));
		    
		case  DaValue.Type.CHARA:
		case  DaValue.Type.WCHARA:
			return getCharacterArray(I.get_chara( target, args ));
		default:
			throw new IllegalArgumentException("Unrecognized type in geta()");
		}
	}
    
	/** Gets given DaReference, giving a CORBA Any.  This get
	 * method should be used only when it is known that, for the
	 * specific named data item in the given DaReference, the corresponding Data
	 * Provider will return a raw CORBA Any which encapsulates a
	 * special purpose CORBA structure intended just for data of
	 * that structure. For instance, at the time of writing, the
	 * c++ CHADS Data Provider returns history data in a CORBA Any
	 * encapsulating a histData CORBA structure, so history data
	 * queries to that data provider should use this method to get
	 * an unmollested histData inserted into an Any.
	 * @param r A DaReference object that encapsulates a well formed parsed
	 * AQS (AIDA Query Specification) query.  The type of this reference
	 * must have either been successfully gleaned from the compilation
	 * process or set manually with setType()
	 * @return A CORBA Any.  If the user has access to the original helper stubs 
	 * for the Any then data can be accessed like that.
	 * @throws UndefinedNameException query string is invalid; not in Directory Service or
	 * bad syntax.
	 * @throws UnableToGetDataException error executing query
	 */
	public Any getAny(DaReference r) 
		throws UnableToGetDataException, UndefinedNameException
	{
		return r.getAny(this, getArgs());
	}
    
	/** Gets data specified by given target query, giving a CORBA
	 * Any. See comments for getAny(DaReference).
	 * @param target A AQS string target to be evaluated
	 * @return A CORBA Any.  If the user has access to the oiginal helper stubs 
	 * for the Any then data can be accessed like that.
	 * @throws UnableToGetDataException error executing query
	 */    
	public Any getAny(String target) throws UnableToGetDataException
	{
		return (m_da.get( target, getArgs() ) );
	}
    
	/** Gets given DaReference.  Return a DaValue
	 * @param r A DaReference object that encapsulates a well formed parsed
	 * AQS (AIDA Query Specification) query.  The type of this reference
	 * must have either been successfully gleaned from the compilation
	 * process or set manually with setType()
	 * @return A DaValue.
	 * @throws UnableToGetDataException error executing query
	 * @throws UndefinedNameException query string is invalid; not in Directory Service or
	 * bad syntax.
	 * @see DaValue
	 */
	public DaValue getDaValue(DaReference r) 
		throws UnableToGetDataException, UndefinedNameException
	{
		return r.getDaValue(this, getArgs());
	}
    
	/** Gets data referenced by given target.  Return a DaValue
	 * @param target A AQS string target to be evaluated
	 * @throws UnableToGetDataException error executing query
	 * @return A DaValue.
	 * @see DaValue
	 */    
	public DaValue getDaValue(String target) 
		throws UnableToGetDataException
	{
		return doGetDaValue(m_da, target,  getArgs());
	}
    
	/** 
	 * Calls the actual "Any" get method of the data provider of
	 * the given target, through that data provider's CORBA
	 * callable interface stub. This is intended to be an internal
	 * helper function through which the public functions go.
	 * @param I The CORBA interface stub of the Data Provider
	 * which can retrieve a value for teh given target.
	 * @param target an AIDA query string
	 * @param args parameters of the AIDA query string understood
	 * by the Data Provider.
	 * @return The AIDA structured data object.
	 * @see DaValue
	 */
	private DaValue doGetDaValue(AidaObjectI I, String target, String [] args) 
		throws UnableToGetDataException
	{
		return (new DaValue(I.get( target, args ) ) );
	}

	/**
         * Performs a set operation for a server specified
         * by a target.
	 * @param target A AQS string target to be evaluated
	 * @return A DaValue containing the returned data.
	 * @throws UnableToGetDataException error executing query
	 */    
	public DaValue set(String target) 
		throws UnableToSetDataException
	{
                float [] dummyValue = {0.0f};

                DaValue v = new DaValue(dummyValue); 

                AnyHolder anyHolder = new AnyHolder(v.toAny());

                m_err.log("Client request for set operation");

                m_da.set( target, getArgs(), anyHolder);

                Any a = anyHolder.value;

                DaValue outData = new DaValue(a);

                return outData;
	}

	/**
         * Sets data specified by a DaValue for a server
         * specified by a target.
	 * @param target A AQS string target to be evaluated
         * @param v A DaValue containing the data to be set.
	 * @return A DaValue containing the returned data.
	 * @throws UnableToGetDataException error executing query
	 */    
	public DaValue setDaValue(String target, DaValue v) 
		throws UnableToSetDataException
	{
                AnyHolder anyHolder = new AnyHolder(v.toAny());

                m_err.log("Client request for set operation");

		m_da.set( target, getArgs(), anyHolder);

                Any a = anyHolder.value;

                DaValue outData = new DaValue(a);

                return outData;
	}


	// TODO: Move the following functions off into another class
    
	/** Construct array of Byte from array of byte
	 * @param ary array of byte
	 * @return array of Byte
	 */
	private Byte [] getByteArray(byte [] ary)
	{
		int len = ary.length;
		Byte [] ret_ary = new Byte[len];
		for (int i = 0; i < len ; i++ )
			ret_ary[i] = new Byte(ary[i]);
		return (ret_ary);
	}
    
    
	/** Construct array of Character from array of char
	 * @param ary array of char
	 * @return array of Character
	 */
	private Character [] getCharacterArray(char [] ary)
	{
		int len = ary.length;
		Character [] ret_ary = new Character[len];
		for (int i = 0; i < len ; i++ )
			ret_ary[i] = new Character(ary[i]);
		return (ret_ary);
	}
    
	/** Construct array of Float from array of float
	 * @param ary array of float
	 * @return array of Float
	 */
	private Float [] getFloatArray(float[] ary)
	{
		int len = ary.length;
		Float [] ret_ary = new Float[len];
		for (int i = 0; i < len ; i++ )
			ret_ary[i] = new Float(ary[i]);
		return (ret_ary);
	}
    
	/** Construct array of Double from array of double
	 * @param ary array of double
	 * @return array of Double
	 */
	private Double [] getDoubleArray(double[] ary)
	{
		int len = ary.length;
		Double [] ret_ary = new Double[len];
		for (int i = 0; i < len ; i++ )
			ret_ary[i] = new Double(ary[i]);
		return (ret_ary);
	}
    
	/** Construct array of Boolean from array of boolean
	 * @param ary array of boolean
	 * @return array of Boolean
	 */
	private Boolean [] getBooleanArray(boolean[] ary)
	{
		int len = ary.length;
		Boolean [] ret_ary = new Boolean[len];
		for (int i = 0; i < len ; i++ )
			ret_ary[i] = new Boolean(ary[i]);
		return (ret_ary);
	}
    
    
	/** Construct array of Long from array of int
	 * @param ary array of int
	 * @return array of Long
	 */
	private Long [] getLongArray(int[] ary)
	{
		int len = ary.length;
		Long [] ret_ary = new Long[len];
		for (int i = 0; i < len ; i++ )
			ret_ary[i] = new Long(ary[i]);
		return (ret_ary);
	}
    
    
	/** Construct array of Short from array of short
	 * @param ary array of short
	 * @return array of Short
	 */
	private Short [] getShortArray(short[] ary)
	{
		int len = ary.length;
		Short [] ret_ary = new Short[len];
		for (int i = 0; i < len ; i++ )
			ret_ary[i] = new Short(ary[i]);
		return (ret_ary);
	}
    
    
}

