package edu.stanford.slac.aida.lib.da;

import java.util.*;
import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.except.*;             // General Exceptions

/** This class encapsulates a reference to a given AIDA query,
 * including parameters.  It can be parsed, compiled and executed
 * using relevant methods.
 *
 * @author  25-Jul-2002 George McIntyre
 * @version  1-Nov-2007 Greg White Added getCount and getService
 */
public class DaReference extends _DaReference
{
	/** Internal copy of original given target string
	 */    
	private String	m_target;

	/** Creates a new instance of DaReference.  This is a
	 * reference to an AIDA data object.  If you use this,
	 * no-argument, constructor the DaReference must be
	 * initialised later with setTarget().
	 */
	public DaReference()
	{
		super();
	}
    
	/** Constructor of a DaReference that takes a target
	 * string as a parameter.  This string is immediately parsed
	 * against the AIDA Query Specification language.
	 *
	 * @param target This is the string representation of the AIDA
	 * Query that is to be referenced by this object
	 */    
	public DaReference(String target)
		throws UndefinedNameException
	{
		super(target);
	}
    
	/** To set the target of this DaReference.  Can be called after
	 * a DaReference is constructed with no parameters to assign the
	 * target AQS.
	 * @param target AIDA Query Specification that specifies the required
	 * target Data Access.
	 */    
	public void setTarget(String target)
		throws UndefinedNameException
	{
		super.setTarget(target);
	}
    
	/** This constructor creates a DaReference object,
	 * parses the given AIDA Query Specification and then
	 * compiles it against a valid DaObject (Data Access Object)
	 * @param target AIDA Query Specification that specifies the required
	 * target Data Access.
	 * @param da A data access object with with to compile
	 * the DaReference target string
	 *
	 */    
	public DaReference(String target, DaObject da)
		throws UndefinedNameException
	{
		super(target, da);
	}

	/** Initialise the DaReference object
	 */    
	protected void init()
	{
		super.init();

		// Create a new set of parameters to send along with all data exchanges
		m_args = new Hashtable();	// Arguments to be sent with get() method
	}

	/** Set parameters for an Aida query in one string.  For example
	 * <code>"parm=value;parm=value;parm=value"</code>.
	 *
	 * @param parmlist String containing parameter/value pairs in the form.
	 * PARAMETER=VALUE;PARAMETER=VALUE...
	 */
	public void	setParam(String	parmlist)
	{
		StringTokenizer	tk1 = new StringTokenizer( parmlist, ";" );
		while (tk1.hasMoreTokens())
		{
			StringTokenizer tk2 = new StringTokenizer( tk1.nextToken(),	"=" );
			if (tk2.countTokens() == 2)
				setParam(tk2.nextToken(), tk2.nextToken());
		}
		return ;
	}

	/** Set a single parameter for this DaReference.
	 * @param name Name of the parameter whose value is being set
	 * @param value The value of the parameter being set.
	 * This is always specified as a string and is converted as
	 * necessary.
	 */
	public void	setParam(String	name, String value)
	{
		m_args.put(name, value);
		return ;
	}
    
	/** Set the type data expected from this DaReference.
	 * @param type A DaValue.Type code specifying the expected return type of the
	 * query
	 * @see DaValue.Type
	 */
	public void setType(int type)
	{
		if ( m_ref != null )
			m_ref.type = type;
	}
    
	/** Set count of elements in the array that is expected to be returned from
	 * this DaReference.  Note that this works only for simple arrays.
	 * @param count The expected length of the array to be returned
	 */
	public void setCount(int count)
	{
		if ( m_ref != null )
			m_ref.count = count;
	}
    
	/** Get the expected type of the object that this query would return.
	 * @return DaValue.Type code representing the expected return type of the query
	 */
    
	public int getType()
	{
		if ( m_ref == null )
			return -1;
		else
			return m_ref.type;
	}
    
	/**
	 * Get the Service IDs of the Aida Data Providers that would partake in 
	 * processing the query of this DaReference. For example see test/java/ApiTests.java.
	 * @return array of service IDs in the order in which they would be called; the 
	 * 1st element will be valued -1 if the DaReference has not yet been setup.
	 */
	public int[] getServices()
	{
		if ( m_ref == null )
			return new int[-1];
		else
			return m_ref.services;
	}

	/** Get the number of Aida Data Provider services that would partake in 
	 * processing the query of this DaReference. For example see test/java/ApiTests.java.
	 * @return The expected length of the array of services (may be used to size an
	 * array receiving return from getServices), or -1 if DaReference has not been set up.
	 */    
	public int getCount()
	{
		if ( m_ref == null )
			return -1;
		else
			return 1;
	}
    
}
