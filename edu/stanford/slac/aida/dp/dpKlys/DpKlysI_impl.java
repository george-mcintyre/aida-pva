/* -*- Mode: java; c-basic-offset: 4; -*- */

/**
 * dpKlys implements the AIDA EPICS Klystron control interface.
 *
 */

package edu.stanford.slac.aida.dp.dpKlys;

import java.nio.CharBuffer;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Timer;
import java.util.TimerTask;
import org.omg.CORBA.ORB;
import org.omg.PortableServer.*;
import gov.aps.jca.*;
import gov.aps.jca.dbr.*;
import edu.stanford.slac.err.*;
import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.except.*;

//
// IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI:1.0
//
/**
 * This class defines the AIDA Data Provider for EPICS Channel Access.
 *
 * DpKlysI_impl implements the AIDA Data Provider "DpKlysI" CORBA
 * interface, by extending DpKlysIPOA
 *
 * The inderlying EPICS Data Source is accessed through the (new COSYLAB) JCA
 * interface.
 *
 */
public class DpKlysI_impl extends DpKlysIPOA
{
    // wait up to 15.0sec for JCA/CA API query completion
    private static final double PENDIO_TIMEOUT = 15.0;

    private static boolean DEBUG = true;      // Write debugging info. 
    private static boolean FAKEPVS = false;    // Use SYS0 PVs, not the real ones.

    private static Calendar calendar = Calendar.getInstance();
    private static AidaDateFormat dateFormat = new AidaDateFormat();

    private static final int BEAM_PARAM = 0;    // Index of beam code parameter in queries
    private static final int DGRP_PARAM = 1;    // Index of DGRP param, *now deprecated*
    private static final Float ATTR_MIN = -10000.0f; // Min val of PDES or KPHR
    private static final Float ATTR_MAX = 10000.0f;  // Max val of PDES or KPHR
    private static final short LINKLYSTA_ACCEL = 0x0001;
    private static final short LINKLYSTA_STANDBY = 0x0002;
    private static final short LINKLYSTA_BAD = 0x0004;


    // EPICS time zero
    private static Date epics_tzero;
    
    private static boolean m_doneInit = false; // Whether = inited this server.
    private _PersistentDaObject m_da;          // For orb access.
    private ORB m_orb;                         // CORBA ORB for this server.
    private org.omg.PortableServer.POA m_poa;  // Portable Object Adapter
    private JCALibrary m_jca;                  // Java Channel Access singleton
    private Context m_ctxt;                    // JCA config Context,
                                               // single threaded or threadsafe
    private Err m_err;                         // Err messaging singleton.
    private ChannelCache channelCache = new ChannelCache();


    DpKlysI_impl(_PersistentDaObject da, org.omg.PortableServer.POA poa)
    {
        m_da = da;      
        m_orb = m_da.m_orb; // Make access to orb easy.
        m_poa = poa;        // Portable Object Adapter, not used atow

        // Get Err issuance singleton, and log init complete
        m_err = Err.getInstance();

        calendar.set (1990, Calendar.JANUARY, 0, 0, 0);
        epics_tzero = calendar.getTime();
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
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/init:1.0
    //
    public void
    init()
        throws ServerInitialisationException
    {
        try 
        {
            // Get the JCALibrary instance.
            m_jca = JCALibrary.getInstance();
			
            // Create single threaded context with default configuration.
            m_ctxt = m_jca.createContext(JCALibrary.JNI_THREAD_SAFE);

            m_ctxt.printInfo();
        }
        catch ( Exception e )
        {
            throw (ServerInitialisationException) 
                m_err.logl( new ServerInitialisationException(e.getMessage())); 
        }
        m_err.logl("DpKlysI servant initialization completed.");        
    }


    
    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get:1.0
    //

    public org.omg.CORBA.Any
    get(String query,
        String[] args)
        throws  UnableToGetDataException
    {
 	float floatValue;                   // Acquired value of the query.
        float[] floatArray = new float[1];  // For some reason, DpSlcKlys returns array,
                                            // so we have to construct a 1 elem array
                                            // to return in the DaValue Any.
	dbg("dpKlys_impl:get entered");
	dbg("dpKlys_impl:get target = " + query);
               
        floatValue = get_float(query, args);
        floatArray[0] = floatValue;
        DaValue ret = new DaValue(floatArray);
 
        return ret.toAny();        
    } 
   
    /**
     * Returns CORBA Any representation of EPICS DBR_TIME_XXX 
     * 
     * NOT PRESENTLY USED!
     */
    public org.omg.CORBA.Any
    getAny(String query,
        String[] args)
        throws  UnableToGetDataException
    {
        try
        {
            String dateFormatSpec = "DATEFORM0";
            if ("DATEFORMAT".equals (args[0]))
                dateFormatSpec = args[1];

            MetaChannel mc = getMetaChannel (query);
            Channel ch = mc.channel;


            DBR dbr = ch.get (
                DBR_to_DBR_TIME (ch.getFieldType()), ch.getElementCount());
            m_ctxt.pendIO (PENDIO_TIMEOUT);

            // okay, now we've got a blob of data that we need to package up.
            // First the simple stuff.
            DaValue davalue = new DaValue();
            davalue.type = DaValue.Type.STRUCT;
            davalue.setName ("DBR_TIME");

            TimeStamp ts = ((TIME)dbr).getTimeStamp();
            String time = ts.toString();

            if ("EPICS".equals (dateFormatSpec))
            {
                time = (new Long (ts.secPastEpoch())).toString();
            }
            else
            {
                dateFormat.applyPattern (dateFormatSpec);
                calendar.setTimeInMillis (
                    epics_tzero.getTime() +
                    (ts.secPastEpoch() * 1000) +
                    (ts.nsec() / 1000000));
                time = dateFormat.format (calendar.getTime());
            }

            DaValue dv = new DaValue (time);
            dv.setName ("time");
            davalue.addElement (dv);

            dv = new DaValue (new Long (((STS)dbr).getStatus().getValue()));
            dv.setName ("status");
            davalue.addElement (dv);

            dv = new DaValue (new Long (((STS)dbr).getSeverity().getValue()));
            dv.setName ("severity");
            davalue.addElement (dv);

            
            if (dbr.isDOUBLE()) {
                dv = new DaValue (((DBR_Double)dbr).getDoubleValue());
            }
            else if (dbr.isINT()) {
                dv = new DaValue (((DBR_Int)dbr).getIntValue());
            }
            else if (dbr.isFLOAT()) {
                dv = new DaValue (((DBR_Float)dbr).getFloatValue());
            }
            else if (dbr.isSHORT()) {
                dv = new DaValue (((DBR_Short)dbr).getShortValue());
            }
            else if (dbr.isENUM()) {
                // transform index values to labels
                short[] idxs = ((DBR_Enum)dbr).getEnumValue();
                String[] strings = new String[idxs.length];
                for (int i = 0; i < strings.length; ++i)
                    strings[i] = mc.getEnumerationLabel (idxs[i]);
                
                dv = new DaValue (strings);
            }
            else if (dbr.isBYTE()) {
                dv = new DaValue (((DBR_Byte)dbr).getByteValue());
            }
            else { // assume isSTRING() --should get cast exception otherwise
                dv = new DaValue (((DBR_String)dbr).getStringValue());
            }
            
            dv.setName ("value");
            davalue.addElement (dv);

            return davalue.toAny();
        }
        
        catch (Exception e)
        {
            m_err.logl (e);
            throw new UnableToGetDataException (query);
        }
    }
    
	    
    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get_short:1.0
    //
    /**
     * Gets the activation status of a given klystron/subbooster, as
     * an [unsigned] short int. This is the value given by klys/sbst
     * device PV BEAMCODE&lt;bc&gt;_STAT, where bc is the
     * Beamcode. The bitfield of the value is given by: 1 ==
     * Accelerate, 2 == Standby, 3 == Bad TMSK or HSTA.
     * 
     * @param query The AIDA entity of a klystron or subbooster, of syntax 
     *        "device-name//TACT".
     * @param args  Array of AIDA DaObject parameters, in particular
     *        the BEAM parameter is required.
     *
     * @return value of KLYS:area:unit:BEAMCODEbeamcode_STAT 16 bit word.   
     */
    public short
    get_short( String query,
               String[] args )
        throws  UnableToGetDataException
    {
        String[] params;             // Parsed parameter values as strings
	String   deviceName;         // AIDA entity instance part
        short    result;             // Klystron status. 
		
	// Parse arguments to find the required beamcode
        // parameter. Then get the activation status of the Klys/sbst on
	// that beamcode.
        try
        {
            params = parseArgs(args);
	    deviceName = queryToInstance(query);

	    // Debug information
	    {
                dbg("\nOperation - get_short"); 
		dbg("Query = "+query+ " : Device Name = " + deviceName);
		dbg("BEAM = " + params[BEAM_PARAM]);
	    }

	    // Get activation status. 1=activated, 0=deactivated. 
	    // Check that a BEAM parameter was supplied.
	    if ( params[BEAM_PARAM] == "NULL" )
	    {
		throw new IllegalArgumentException("An integer BEAM code "+
						   " is required");
	    }
	    result = getKlysStatus(deviceName,
				   params[BEAM_PARAM],
				   params[DGRP_PARAM]);
        }
        catch ( Exception ex )
        {
            throw (UnableToGetDataException)
                m_err.logl( new UnableToGetDataException(query).initCause(ex));
        }
        return result;        
    }
 
    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get_short:1.0
    //
    /**
     * Gets the activation status of a given klystron/subbooster, as a
     * long int. However only the lower 16 bits are used. See
     * get_short for their interpretation.
     *
     * @param query The AIDA entity of a klystron or subbooster, of syntax 
     *        "device-name//TACT".
     * @param args  Array of AIDA DaObject parameters, in particular
     *        the BEAM parameter is required.
     *
     * @return The klystron status longword. 
     */
    public int
    get_long( String query,
              String[] args )
        throws  UnableToGetDataException
    {
	short result;
	int int_result = 0;
	result = get_short( query, args ); 
	int_result = result;
        return int_result;
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get_string:1.0
    //
    /**
     * Gets the activation status bit of a given klystron/subbooster
     * status bitfield, as a String. See get_short for more
     * information.
     *
     * @param query The AIDA entity of a klystron or subbooster, of syntax 
     *        "device-name//TACT".
     * @param args  Array of AIDA DaObject parameters, in particular
     *        the BEAM parameter is required.
     *
     * @return If the low order bit (ACCEL) of the klys/sbst status
     * bitfield == 0, then get_string returns "deactivated"; otherwise
     * return "activated".
     */
    public String
    get_string(String query,
               String[] args)
        throws  UnableToGetDataException
    {

        String activateStr = null;
	short status = get_short( query, args );

	short maskedResult = (short) (status & LINKLYSTA_ACCEL);
        if ( maskedResult == 0 )
	    activateStr = "deactivated";
	else
	    activateStr = "activated";

	return activateStr;
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get_wstring:1.0
    //
    public String
    get_wstring(String query,
                String[] args)
        throws  UnableToGetDataException
    {
        return get_string(query, args);
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get_double:1.0
    //
    public double
    get_double(String query,
               String[] args)
        throws  UnableToGetDataException
    {
        double[] _r = get_doublea( query, args );
        return _r[0];
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get_float:1.0
    //
    /**
     * Implements acquisition of Klys PDES, PHAS, KPHR as float value.   
     */
    public float
    get_float(String query,
              String[] args)
        throws  UnableToGetDataException
    {
        float[] _r = get_floata( query, args );
        return _r[0];
    }



    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get_doublea:1.0
    //
    public double[]
    get_doublea( String query,
                 String[] args )
        throws  UnableToGetDataException
    {
        double[] _r = {0.0};
        String attribute = queryToAttribute(query);

	if ( attribute.equals("PHAS") )
	    _r[0] = getPhase( queryToInstance(query) );
        else if ( attribute.equals("PDES") )
	    _r[0] = getPdes( queryToInstance(query) );
        else if ( attribute.equals("KPHR") )
	    _r[0] = getKphr( queryToInstance(query) );
        else
	{
	    m_err.logl( "Not a recognized attribute for acq as floating pt, when acquiring "+
                        query );
	    throw new UnableToGetDataException(query);
	}

	return _r;
    }
    

    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get_floata:1.0
    //
    public float[]
    get_floata( String query,
                String[] args)
        throws  UnableToGetDataException
    {
        float[] _r = {0.0F};                   // Return var
	double[] temp;                // Temp array so can reuse

	try
	{
	    temp = get_doublea( query, args );
	    _r[0] = (float)temp[0];
	}
	catch ( Exception ex )
	{
	    m_err.logl( ex, "When acquiring "+query+" as floats");
	    throw new UnableToGetDataException(query);
	}
	return _r;
    }




    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/set:1.0
    //
    public void set (
        String query,
        String[] args, 
        org.omg.CORBA.AnyHolder value)
        throws  UnableToSetDataException
    {

	short flagValue;
        short state;
        short maskedResult;

        int deactOrReactFlag;
        int i;

        Double secnValue;
        Double phasValue;
        float configValue;

        String attribute;         // The AIDA attribute part of the given query
        String instance;          // The AIDA instance part of the given query
        String pvName;            // The PV name equiv to the given AIDA query 
        String trim;

        String[] params;

        dbg("\nEntering set method");
        dbg("query = " + query);

        attribute = queryToAttribute(query);

	int nArgs = ( (args[0]==null) ? 0 : args.length );

        if (attribute.equals("PHAS"))
        {
            m_err.logl("Cannot set KLYS or SBST PHAS value");
                throw new UnableToSetDataException();
        }
	else if ((attribute.equals("PDES")) ||
                 (attribute.equals("KPHR")) )
        {
            //
            // Processing for setting the PDES value and optionally
            // trimming the phase of a subbooster or a klystron.
            // Also handles processing for setting the KPHR value
            // for a subbooster or a klystron in which case the
            // setting of the optional TRIM parameter is ignored.

            // Extract the primary/micro/unit specification of the
            // subbooster or klystron from the target name.
            // 
            pvName = queryToPVRecordName(query);
	    instance = queryToInstance(query);
	    dbg("pvName ="+ pvName);
            

            // Create a DaValue from the input parameter, the
            // specified PDES or KPHR value.  Convert it to a
            // float value.
            //
            DaValue inDaValue = new DaValue(value.value);
	    try
            {
                secnValue =
                    ((Float) inDaValue.firstElement()).doubleValue();
            }
            catch (Exception ex)
            {
                m_err.logl(ex, instance + " specified " + attribute + 
                    " value is not a float");
                throw new UnableToSetDataException();
            }
	    dbg("Supplied value as Double"+ secnValue);


	    // Check that the specified PDES or KPHR value is
            // in a reasonable range to prevent unreasonable
            // values from being processed.
            //
            if ((secnValue < ATTR_MIN) || (secnValue > ATTR_MAX))
            {
                m_err.logl(instance + "Specified " + attribute + 
                    " value " + secnValue + " is out-of-range ");
                throw new UnableToSetDataException();
            }
	   
	    // Process TRIM argument and set the PDES or KPHR.
	    try
            {
                // Get the optional parameter TRIM if present.
                // First, set the default trim value to YES
                // if the secondary is PDES or to NO otherwise (KPHR).
                //
                if (attribute.equals("PDES"))
                    trim = "YES";
                else
                    trim = "NO";
                for (i = 0; i < nArgs; i++)
                {
                    if (args[i].equalsIgnoreCase("TRIM") )
                        trim = args[i+1].toUpperCase();
                }

		// Check validity of TRIM arg value.
                //
		if ((!trim.equals("YES")) &&
                    (!trim.equals("NO")))
                {
                    m_err.logl("TRIM parameter must be YES or NO");
                    throw new UnableToSetDataException();
                }
                if ((trim.equals("YES")) &&
                    (attribute.equals("KPHR")) )
                {
                    trim = "NO";
                    m_err.logl("TRIM parameter is ignored when specified " +
                        "secondary is KPHR");
                }

		if (trim.equals("YES"))
                {
                    m_err.logl("Trimming " + instance + " to " +
                        attribute + " value " + secnValue);
                }
                else
                {
                    m_err.logl("Setting " + instance + " to " +
                        attribute + " value " + secnValue);
                }

                //  Set PHAS or KPHR, synchronized (one at a time) across AIDA clients.
                //
                synchronized(this)
                {
                    //
                    // Invoke the SetTrimPhase method to set the PDES or
                    // KPHR of the specified subbooster or klystron and
                    // also optionally perform a trim operation if the
                    // secondary is PDES.  This method returns the value
                    // of the PHAS secondary, contained in an Any object.
                    //
                    dbg("Setting the "+attribute+
                        " and acquiring the PHAS. TRIM="+trim);
                    phasValue = setTrimPhase(instance, attribute,
					     secnValue, trim);

                    if (trim.equals("YES"))
                    {
                        m_err.logl("Trim of " + instance + " to " +
                            attribute + " value " +
                            secnValue + " successfully completed");
                    }

                    Float result = new Float(phasValue);
                    DaValue outDaValue = new DaValue(result);
                    value.value = outDaValue.toAny();
                }
	    }
            catch ( Exception ex )
            {
                throw (UnableToSetDataException)
                m_err.logl( new UnableToSetDataException(query).initCause(ex));
            }
        }

	else if ((attribute.equals("PCON")) ||
                 (attribute.equals("ACON")))
        {
  	    // SLC implementation of dpKlys included setting the CONfig value.
	    // The EPICS implementation does not, since RF is not under config control.
	    m_err.logl("Setting PCON and ACON are not implemented");
	}

        else if (attribute.equals("TACT"))
        {
            // Processing for deactivating or reactivating
            // a klystron.
            //
            
            // Extract the short flag value from the specified
            // value.
            //
            DaValue inDaValue = new DaValue(value.value);
            try
            {
                flagValue =
                    ((Short) inDaValue.firstElement()).shortValue();
            }
            catch (Exception e)
            {
                m_err.logl("Specified value is not a short integer");
                    throw new UnableToSetDataException(query);
            }
	    dbg("activation/deactivation input value = " + flagValue);

            try
            {
                // Validate the specified flag value. It must
                // be equal to 0 (to deactivate the specified klystron)
                // or 1 (to reactivate the specified klystron).
                //
                deactOrReactFlag = (int) flagValue;

                if ((deactOrReactFlag != 0) &&
                    (deactOrReactFlag != 1))
                {
                    m_err.logl("Supplied value must be 0 or 1");
                    throw new UnableToSetDataException();
                }

		// Extract the (required) BEAM parameter
		params = parseArgs(args);
                dbg("BEAM = " + params[BEAM_PARAM]);

		synchronized(this)
                {
                    // Extract the device name from the query.
                    instance = queryToInstance(query);

                    // Get the status code of the specified klystron on the
                    // specified beam code, which is a required parameter.
                    // If the display group parameter is not provided,
                    // the specified klystron must be in the display group
                    // LIN_KLYS.
                    //
		    dbg("Discover the extant STATE: ACCEL, STANDBY, other?");
		    try 
		    {
			state = getKlysStatus(instance,
					      params[BEAM_PARAM],
					      params[DGRP_PARAM]); 
		    }
		    catch ( Exception e )
		    {
			throw (UnableToSetDataException) 
			    m_err.logl(new UnableToSetDataException(instance), 
				      "Can not get present status of "+
				      instance+ " on beam code "+params[BEAM_PARAM]);
		    }

                    // If the specified flag value indicates that the
                    // specified klystron is to be deactivated and
                    // the status of the klystron indicates that it is
                    // not presently in the accelerate state, indicate an error.
                    //
                    if (deactOrReactFlag == 0)
                    {
                        maskedResult = (short) (state & LINKLYSTA_ACCEL);
                        if (maskedResult == 0)
                        {
			    m_err.logl(instance + 
				      ": Cannot deactivate klystron not in " +
				      "accelerate state");
                            throw new UnableToSetDataException();
                        }
                    }
		    else
                    {
			// The specified flag value indicates that the
			// specified klystron is to be reactivated.  If the
			// status of the klystron indicates that it is
			// already activated, indicate an error.
			//
                        maskedResult = (short) (state & LINKLYSTA_STANDBY);
                        if (maskedResult == 0)
                        {
			    m_err.logl(instance + 
				      ": Cannot reactivate klystron not in " +
				      "standby state");
                            throw new UnableToSetDataException();
                        }
                    }
		    
                    // Send a log message indicating the action to be performed.
                    //
                    if (deactOrReactFlag == 0)
                        m_err.logl("Deactivating klystron " + instance);
                    else if (deactOrReactFlag == 1)
                        m_err.logl("Reactivating klystron " + instance);

		    // Deactivate or reactivate the specified klystron
                    // depending on the specified flag value on the
                    // specified beam code.  Then get the resulting
                    // status code of the klystron and return it to
                    // the user.
                    //
		    setDeactOrReact(instance,
				    deactOrReactFlag,
				    params[BEAM_PARAM]);
		   
		    state = getKlysStatus(instance,
					  params[BEAM_PARAM],
					  params[DGRP_PARAM]);

                    Short result = new Short(state);
                    DaValue outDaValue = new DaValue(result);
                    value.value = outDaValue.toAny();
                }
		    
	    }
	    catch ( Exception ex )
            {
                throw (UnableToSetDataException)
		    m_err.logl( new UnableToSetDataException(query).initCause(ex),
			"while attempting to set "+query);
            }
	}

	return;
    }

		    
    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/startMonitor:1.0
    //
    public void
    startMonitor(String name)
        throws  MonitorStartupException
    {
        // TODO: later
    }
		    
    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/stopMonitor:1.0
    //
    public void
    stopMonitor(String name)
        throws  MonitorShutdownException
    {
        // TODO: later
    }
		    
    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/status:1.0
    //
    public int
    status()
    {
        // TODO: implement
        int _r = 0;
        return _r;
    }
		    
    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/shutdown:1.0
    //
    public void
    shutdown()
    {
        // TODO: implement
    }
		 
		    
    /*
    ** PRIVATE METHOD DEFINITIONS
    */
    /** Level 2 methods **/



    /**
     * Converts an Aida query string, of the form <instance>//<attribute>
     * to an EPICS Channel Name, of the form <PVname>.<fieldname>, by
     * simply replacing the "//" with ".".
     * 
     * @param String query Aida query string
     * @return EPICS Channel Name
     */
    private String
    queryToChannelName( String query )
    {
	    return query;
    }

    /**
     * Convert the target query of the form instance//attribute
     * into the logically equivalent EPICS PV Record name of the form 
     * prim:micr:unit:secn.  Principally this is done by replacing "//" with ":",
     * though in the context of the AIDA EPICS Klys server it may also involve
     * changing parts of the name  
     */
    private String queryToPVRecordName (String query)
    {
	 String pvRecord = null;          // PV equiv to query
	 String subsPvRecord = null;      //  

	 int iSep = query.lastIndexOf(AQSToken.SEPARATOR);
	 if ( iSep <2 )
	     throw ((IllegalArgumentException) 
		    m_err.logl(new IllegalArgumentException("Invalid name: "+ 
							   query)));
	 String instance = query.substring(0, iSep);
	 String attribute = query.substring(iSep + 2);

         /*
	  Logical Mapping for AIDA Klys Server to EPICS 
	  */
	 if ( attribute.equals("PDES") )
	     pvRecord = instance + ":PDES";
	 else if ( attribute.equals("KPHR") )
	     pvRecord = instance + ":KPHR";
	 else if ( attribute.equals("PHAS") )
	     pvRecord = instance + ":PHAS";
	 else if ( attribute.equals("TACT") )
	     pvRecord = instance + ":TACT";
 	 else
	     pvRecord = instance;
	    
	 if ( FAKEPVS ) 
	 {
	     /*
	        Remap entity to EPICS PV names for testing.  
	      */
	      if ( pvRecord.equals("KLYS:LI31:11:PDES") )
	      {
	          subsPvRecord = "SIOC:SYS0:ML00:AO990";
	      }
	      else if ( pvRecord.equals("KLYS:LI31:11:KPHR") )
	      {
	          subsPvRecord = "SIOC:SYS0:ML00:AO991";
	      }
	      else if ( pvRecord.equals("KLYS:LI31:11:PHAS") )
	      {
	           subsPvRecord = "SIOC:SYS0:ML00:AO992";
	      }
	      else if ( pvRecord.equals("KLYS:LI31:11:TACT") )
	      {
	           subsPvRecord = "SIOC:SYS0:ML00:AO989";
	      }
	      if ( subsPvRecord != null )
	      {
	           dbg("Mapped "+pvRecord+" -> "+subsPvRecord);
	           return subsPvRecord;
	      }
	      else
                   return pvRecord;
	 }
	 else
	      return pvRecord;

    }

    /**
     * Returns the AIDA instance part of a given AIDA entity. The
     * entity is that part before the "//".
     */
    protected String queryToInstance(String query)
    {
        int iSep = query.lastIndexOf(AQSToken.SEPARATOR);
        return query.substring(0, iSep);
    }

    /**
     * Returns the AIDA attribute part of a given AIDA entity. The
     * attribute is that part before the "//".
     */
    protected String queryToAttribute(String query)
    {
        int iSep = query.lastIndexOf(AQSToken.SEPARATOR);
        return query.substring(iSep + 2);
    }


    /**
     * Examines the "parameters" passed from the client (those
     * things given by AIDA client calls to DaOBject.setParam()).
     *
     * Most DpKlys get methods do not recognize any params, so throw
     * exception if one is given.
     * @throws IllegalArgumentException an argument/parameter was
     * given, where none is recognized.
     */
    private String[] parseArgs( String[] args )
        throws IllegalArgumentException
    {
	String[] params = {"NULL","NULL"};  // Parameter values found
	int i;                              // indexes arguments to interface methods
		
	// nArgs is the effective number of of parameters. The AIDA
        // CORBA API results in at least 1 arg always, because the AidaObject
        // API does not define versions of each API method with a no-args arg,
        // so AIDA must supply at least 1 args arg, but args[0] to "null". 
        if ( args != null )
	{
	    int nArgs = ( (args[0].compareTo("null")==0) ? 0 : args.length );
	    
	    for ( i=0; i<nArgs; i+=2 )
	    {
		// Recognize and assign the Beamcode argument.
		if ( args[i].equalsIgnoreCase("BEAM") )
		    params[BEAM_PARAM] = args[i+1];
		
		// The DGRP param is deprecated and not respected by 
		// the EPICS Klystron interface, but we process it here 
		// for illustration should it
		// be added in the future.
		if ( args[i].equalsIgnoreCase("DGRP") )
		    params[DGRP_PARAM] = args[i+1];
	    }
	}


	// Check to make sure that the BEAM parameter may be
        // converted to an integer.
        
	if ( params[BEAM_PARAM] != "NULL" )
	{
	    try
	    {
		Integer.parseInt(params[BEAM_PARAM].trim());
	    }
	    catch (Exception e)
	    {
		System.out.println("Specified beam code is not an integer");
		
		throw new IllegalArgumentException("Specified beam code "+
					       " is not an integer");
	    }
	}

        return params;
    }


    protected double getPhase( String deviceName )
	throws UnableToGetDataException
    {
	double[] _r = {0.0};
	_r = caget_doublea( deviceNameToPhasePv(deviceName) );
	return _r[0];
    }

    protected double getPdes( String deviceName )
	throws UnableToGetDataException
    {
	double[] _r = {0.0};
	_r = caget_doublea( deviceNameToPdesPv(deviceName) );
	return _r[0];
    }

    protected double getKphr( String deviceName )
	throws UnableToGetDataException
    {
	double[] _r = {0.0};
	_r = caget_doublea( deviceNameToKphrPv(deviceName) );
	return _r[0];
    }

    protected void setPdes ( String deviceName, Double value, boolean trim )
	throws UnableToSetDataException
    {
	dbg("Setting Nominal PV "+deviceName+" = "+value);
	caset_Double( deviceNameToPdesPv(deviceName), value );
	if ( trim ) trimPdes( deviceName );
    }

    protected void setKphr ( String deviceName, Double value, boolean trim )
	throws UnableToSetDataException
    {
	dbg("Setting Nominal PV "+deviceName+" = "+value);
	caset_Double( deviceNameToKphrPv(deviceName), value );
    }

    protected void trimPdes( String deviceName )
	throws UnableToSetDataException
    {
	String trimPv;                // The PV written to, to trim PDES of device.
	final Integer TRIMCONST = 1;  // Value to write to trim.

	trimPv = deviceName + ":TRIMPHAS";    
	dbg("Trimming "+deviceName+" by "+ trimPv +" <= "+TRIMCONST);
	caset_int(trimPv, TRIMCONST);
    }


    protected String deviceNameToPhasePv( String deviceName)
    {
	if (FAKEPVS) return "SIOC:SYS0:ML00:AO992";
	return deviceName+":PHAS";
    }

    protected String deviceNameToPdesPv( String deviceName)
    {
	if (FAKEPVS) return "SIOC:SYS0:ML00:AO990"; 
	return deviceName+":PDES";
    }

    protected String deviceNameToKphrPv( String deviceName)
    {
	if (FAKEPVS) return "SIOC:SYS0:ML00:AO991";
	return deviceName+":KPHR";
    }
		  
    /**
     * Gets whether a given klystron/subbooster status shortword. 
     *
     * The status shortword is what on VMS would have been returned by 
     * linac_klys_stat for the given klys/sbst on the given beamcode.
     *
     */
    protected short getKlysStatus( String deviceName,
			 	   String beam,
				   String dgrp )
	throws UnableToGetDataException
    {
	String statusOnBeamPVname;    // PV from which to get the status for device.
	short status;                 // The KLYS/SBST's status shortwd.
	int   beamCode;               // The beamcode for which to get the klys' status
 
	beamCode = Integer.parseInt(beam);  
	
        dbg("\tPVname = " + deviceName);
	dbg("\tBEAM = " + beam);

	statusOnBeamPVname = deviceName + ":BEAMCODE" + beam + "_STAT";
        dbg("\tStatus PVname = " + statusOnBeamPVname);
        status = (short)caget_int(statusOnBeamPVname); 
	    
	dbg("\nStatus from PV ="+status);
	// status = 2;	    
	return status;	
    }


    protected void setDeactOrReact(String deviceName,
				   int deactOrReactFlag,
				   String beam)
	throws UnableToSetDataException
    {
	String controlPvName;    // The constrcuted PV name used to control the 
                                 // activation of the given device.
	try
	{
	    controlPvName = deviceName;
	    controlPvName += ":BEAMCODE";
	    controlPvName += beam;
	    controlPvName += "_TCTL";

	    if ( deactOrReactFlag == 0 )
		caset_String( controlPvName, "Deactivate" );
	    else if ( deactOrReactFlag == 1 )
		caset_String( controlPvName, "Reactivate" );
	    else if ( deactOrReactFlag == 2 )
		caset_String( controlPvName, "Activate" );
	    else
		throw new IllegalArgumentException("Unrecognized activation flag"+
						   deactOrReactFlag);
	}
	catch ( Exception ex )
	{
	     throw (UnableToSetDataException) 
		 m_err.logl(new UnableToSetDataException(deviceName).initCause(ex));
	}
	
    }

    protected Double setTrimPhase( String deviceName, 
				   String attribute,
				   Double value,
				   String trim )
	throws UnableToSetDataException
    {

	Double phaseValue;              // The readback Phase resulting from
                                        // setting PDES or KPHR
	String pvName;

	if ( attribute.equals("PDES") )
	{
	    setPdes( deviceName, value, trim.equals("YES") );
	}
	else if ( attribute.equals("KPHR") )
	{
	    setKphr( deviceName, value, trim.equals("YES") );
	}
	else
	    throw (UnableToSetDataException) 
		m_err.logl(new UnableToSetDataException("Unrecognized attribute:"+
						       attribute));
	
	dbg("Getting the resulting PHAS");
	try 
	{
	    phaseValue = getPhase(deviceName);
	}
	catch ( Exception ex )
	{
	    throw (UnableToSetDataException) 
		m_err.logl(new UnableToSetDataException(
			      "Unable to get present phase of "+deviceName));
	}
	return phaseValue;
    }

    /**
     *  Wraps channel access for acquisition of ints
     */
    protected int caget_int( String pvName )
        throws UnableToGetDataException
    {
        int[] _r={0};

	dbg("Doing INT Channel.getChannel on : "+pvName);
        try
        {
            Channel ch = getChannel(pvName);
            int n = ch.getElementCount();
            INT dbr = (INT)ch.get( DBRType.INT, n );
            m_ctxt.pendIO( PENDIO_TIMEOUT );
            _r = dbr.getIntValue();
        } 
        catch (Exception e)
        {
            log( e, pvName);
        }
        return _r[0];
    }


    /**
     *  Wraps channel access for acquisition of doubles
     */
    protected double[] caget_doublea( String pvName )
        throws UnableToGetDataException
    {
        double[] _r = {0.0};                   // Return var
		        
	dbg("Doing DOUBLE Channel.getChannel on : "+pvName);
        try
        {
            Channel ch = getChannel(pvName);
            int n = ch.getElementCount();
            DOUBLE dbr = (DOUBLE)ch.get( DBRType.DOUBLE, n );
            m_ctxt.pendIO( PENDIO_TIMEOUT );
            _r = dbr.getDoubleValue();
        } 
        catch (Exception e)
        {
            log( e, "while acquiring "+pvName);
        }
        return _r;
    }

    /**
     * Channel Access "put" a single int value to a given PV.
     */
    protected void caset_int(
        String pvName,
        Integer value)
        throws  UnableToSetDataException
    {

	dbg("Doing INT Channel.put on : "+pvName);
        try
        {
            Channel ch = getChannel (pvName);
	    ch.put( value );
            m_ctxt.flushIO();
        }
        catch (Exception e)
        {
            throw (UnableToSetDataException)
                m_err.logl (new UnableToSetDataException (e.getMessage()));
        }
    }

    /**
     * Channel Access "put" a single Double value to a given PV.
     */
    protected void caset_Double(
        String pvName,
        Double value)
        throws  UnableToSetDataException
    {
	dbg("Doing DOUBLE Channel.put on : "+pvName);

        try
        {
            Channel ch = getChannel (pvName);
	    ch.put( value );
            m_ctxt.flushIO();
        }
        catch (Exception e)
        {
            throw (UnableToSetDataException)
                m_err.logl (new UnableToSetDataException (e.getMessage()));
        }
    }

    /**
     * Channel Access "put" a single String value to a given PV.
     */
    protected void caset_String(
        String pvName,
        String value)
        throws  UnableToSetDataException
    {
	dbg("Doing STRING Channel.put on : "+pvName);

        try
        {
            Channel ch = getChannel (pvName);
	    ch.put( value );
            m_ctxt.flushIO();
        }
        catch (Exception e)
        {
            throw (UnableToSetDataException)
                m_err.logl (new UnableToSetDataException (e.getMessage()));
        }
    }

    /**
     * Channel Access "put" a given single value to a given PV, where
     * the single value is passed as a Corba ANY type.
     */
    protected void caset_any(
        String pvName,
        org.omg.CORBA.AnyHolder value)
        throws  UnableToSetDataException
    {
        try
        {
            Channel ch = getChannel (pvName);
            DaValue dv = new DaValue (value.value);

            // can't assume that CA server supports data type conversion
            DBRType type = ch.getFieldType();

            if (type.isDOUBLE())
            {
                ch.put (dv.getAsDoubles());
            }
            else if (type.isINT())
            {
                ch.put (dv.getAsInts());
            }
            else if (type.isFLOAT())
            {
                ch.put (dv.getAsFloats());
            }
            else if (type.isSHORT())
            {
                ch.put (dv.getAsShorts());
            }
            else if (type.isBYTE())
            {
                ch.put (dv.getAsBytes());
            }
            else if (type.isSTRING())
            {
                ch.put (dv.getAsStrings());
            }
            else if (type.isENUM())
            {
                // try to get the contents as enum indices, otherwise strings
                try {
                    ch.put (dv.getAsShorts());
                }
                catch (NumberFormatException e) {
                    ch.put (dv.getAsStrings());
                }
            }
            else
            {
                throw new UnsupportedOperationException (
                    "Unsupported data type");
            }

            m_ctxt.flushIO();
        }
        catch (Exception e)
        {
            throw (UnableToSetDataException)
                m_err.logl (new UnableToSetDataException (e.getMessage()));
        }
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
            m_ctxt.flushIO();
        }
        catch ( Exception ex )
        {
            m_err.logl( ex );
        }

        // Now issue error.
        throw (UnableToGetDataException) 
            m_err.logl( new UnableToGetDataException(query), 
                       "caused by: " + e.toString());
    }

    
    /**
     * Issue debug messages through the logging sytem, if DEBUG flag is on. 
     */
    private void dbg( String msg )
    {
        if (DEBUG) m_err.logl( msg );
    }


    /* Silly utility function to convert DBR_XXX to DBR_TIME_XXX
     */
    static DBRType DBR_to_DBR_TIME (DBRType dbrt)
    {
        if (dbrt.isDOUBLE()) {
            return DBR_TIME_Double.TYPE;
        }
        else if (dbrt.isINT()) {
            return DBR_TIME_Int.TYPE;
        }
        else if (dbrt.isFLOAT()) {
            return DBR_TIME_Float.TYPE;
        }
        else if (dbrt.isSHORT()) {
            return DBR_TIME_Short.TYPE;
        }
        else if (dbrt.isENUM()) {
            return DBR_TIME_Enum.TYPE;
        }
        else if (dbrt.isBYTE()) {
            return DBR_TIME_Byte.TYPE;
        }
        else if (dbrt.isSTRING()) {
            return DBR_TIME_String.TYPE;
        }
        else return DBRType.UNKNOWN;
    }

    /**
     * Acquires access to a JCA Channel for a given EPICS Channel Name.
     * @throws IllegalArgumentException query is invalid, may be not an
     * instance//attribute pair?
     */
    protected Channel
    getChannel( String query )
        throws IllegalArgumentException, CAException, TimeoutException
    {
        return (getMetaChannel (query)).channel;
    }

    /**
     * Acquires access to a JCA Channel for a given EPICS Channel Name.
     * @throws IllegalArgumentException query is invalid, may be not an
     * instance//attribute pair?
     */
    protected MetaChannel
    getMetaChannel( String query )
        throws IllegalArgumentException, CAException, TimeoutException
    {
        try
        {
            return channelCache.getMetaChannel (queryToChannelName (query));
        }
        // Err.log doesn't yet examine cause and issue that too,
        // so logl it here.
        catch ( CAException e )
        {
            m_err.logl( e, query );
            throw e;
        }
        catch ( TimeoutException e )
        {
            m_err.logl( e, query );
            throw e;
        }
    }

    /**
       Encapsulates jca Channel, associating some other info along with it
     */
    private class MetaChannel
    {
        private String[] enumerationLabels = new String[0];
        public Channel channel;

        
        MetaChannel (Channel channel)
            throws CAException, IllegalStateException, TimeoutException
        {
            this.channel = channel;

            // gather labels for enum types
            if (channel.getFieldType().isENUM())
            {
                DBR_LABELS_Enum labels = (DBR_LABELS_Enum)channel.get (
                    DBRType.LABELS_ENUM, 1);
                m_ctxt.pendIO (PENDIO_TIMEOUT);
                enumerationLabels = labels.getLabels();
            }
        }
        
        public String getEnumerationLabel (int idx)
        {
            if (idx > enumerationLabels.length)
                return "Unknown Label: index out of range";
            return enumerationLabels[idx];
        }
    }
    

    /* Cache of recently used channels.
       
       Requested channels are created on demand if they don't already exist in
       the cache.  Channels note the time when they are requested.
       
       Periodic scans of the cached channels flush those that have not been
       referenced recently enough.

       There's an inherent race condition in this system: a channel that is
       requested via the cache, and held longer than its linger time may not
       be used with any confidence.

       The assumption is that none of the server code holds on to a channel
       across requests, and none of the requests should take anywhere near the
       linger time to complete.  If the premise changes, then we'll need
       reference counting.
     */
    private class ChannelCache
    {
        /* The approximate amount of time a channel will may remain connected
           after its most recent reference.
         */
        static final int CHANNEL_LINGER_MILLIS = 5 * 60 * 1000;

        
        /* The flushing thread runs at this interval to disconnect channels
           that have not been referenced recently enough.
         */
        static final int FLUSH_INTERVAL_MILLIS = 30 * 1000;


        /* Associates reference time with a channel object.
         */
        private class CachedChannel extends MetaChannel
        {
            private long referenceTimeMillis = 0;

            public CachedChannel (Channel channel)
                throws CAException, IllegalStateException, TimeoutException
            {
                super (channel);
            }

            public void updateReferenceTime ()
            {
                referenceTimeMillis = System.currentTimeMillis();
            }

            public boolean isNotReferencedSince (long sinceMillis)
            {
                return (referenceTimeMillis < sinceMillis);
            }

            public void destroyChannel ()
                throws CAException, IllegalArgumentException
            {
                channel.destroy();
            }
        }


        /* Maps channel name -> CachedChannel
         */
        private final HashMap cachedChannels = new HashMap();


        /* Creates the flushing thread.
         */
        public ChannelCache ()
        {
            // create a daemon-threaded timer and assign it a new fixed-delay
            // task to flush decrepit channels
            new Timer (true).schedule (

                // task
                new TimerTask() {
                     public void run () {
                        destroyDecrepitChannels();
                    }
                },
                
                // initial delay
                CHANNEL_LINGER_MILLIS,
                
                // subsequent periodicity
                FLUSH_INTERVAL_MILLIS);
        }

        
        /* Finds in cache or adds as necessary.
         */
        synchronized public MetaChannel getMetaChannel (String name)
            throws
            CAException,
            IllegalArgumentException,
            IllegalStateException,
            TimeoutException
        {
            CachedChannel cc = (CachedChannel)cachedChannels.get (name);
            
            if (cc == null)
            {
                Channel channel = m_ctxt.createChannel (name);
                m_ctxt.pendIO (PENDIO_TIMEOUT);
                cc = new CachedChannel (channel);
                cachedChannels.put (name, cc);
           }

            cc.updateReferenceTime();
            return cc;
        }

        /* Runs through list of cached channels, destroying those that have
           lingered long enough.
        */
        synchronized public void destroyDecrepitChannels ()
        {
            long since = System.currentTimeMillis() - CHANNEL_LINGER_MILLIS;

            Iterator cci = cachedChannels.values().iterator();
            while (cci.hasNext())
            {
                CachedChannel cc = (CachedChannel)cci.next();
                if (cc.isNotReferencedSince (since))
                {
                    cci.remove();
                    try
                    {
                        cc.destroyChannel();
                        cc = null;
                    }
                    catch (Exception e)
                    {
                        m_err.logl (e);
                    } 
                }
            }
        }
    }

    /** Unimplemented Methods AIDA Data Provider (DaObject) methdos.
        These methods are not implemented for the Klystron service 
    **/
    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get_byte:1.0
    //
    public byte
    get_byte( String query,
              String[] args )
        throws  UnableToGetDataException
    {

	throw (UnableToGetDataException)
            m_err.logl( new UnableToGetDataException(query).initCause(
		 new UnsupportedOperationException("get_byte")));
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get_char:1.0
    //
    public char
    get_char(String query,
             String[] args)
        throws  UnableToGetDataException
    {
	throw (UnableToGetDataException)
            m_err.logl( new UnableToGetDataException(query).initCause(
                new UnsupportedOperationException("get_char")));    
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get_wchar:1.0
    //
    public char
    get_wchar(String query,
              String[] args)
        throws  UnableToGetDataException
    {
	throw (UnableToGetDataException)
            m_err.logl( new UnableToGetDataException(query).initCause(
                new UnsupportedOperationException("get_wchara")));
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get_boolean:1.0
    //
    public boolean
    get_boolean(String query,
                String[] args)
        throws  UnableToGetDataException
    {
        throw (UnableToGetDataException)
            m_err.logl( new UnableToGetDataException(query).initCause(
                new UnsupportedOperationException("get_boolean")));
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get_booleana:1.0
    //
    public boolean[]
    get_booleana(String query,
                 String[] args)
        throws  UnableToGetDataException
    {
       throw (UnableToGetDataException)
            m_err.logl( new UnableToGetDataException(query).initCause(
                new UnsupportedOperationException("get_booleana")));    
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get_bytea:1.0
    //
    public byte[]
    get_bytea( String query,
               String[] args)
        throws  UnableToGetDataException
    {
	throw (UnableToGetDataException)
            m_err.logl( new UnableToGetDataException(query).initCause(
                new UnsupportedOperationException("get_bytea")));    
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get_chara:1.0
    //
    public char[]
    get_chara(String query,
              String[] args)
        throws  UnableToGetDataException
    {
	throw (UnableToGetDataException)
            m_err.logl( new UnableToGetDataException(query).initCause(
                new UnsupportedOperationException("get_chara")));    
    }


    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get_chara:1.0
    //
    public char[]
    get_wchara(String query,
               String[] args)
        throws  UnableToGetDataException
    {
	throw (UnableToGetDataException)
            m_err.logl( new UnableToGetDataException(query).initCause(
                new UnsupportedOperationException("get_wchara")));        
    }


    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get_short:1.0
    //
    public short[]
    get_shorta( String query,
                String[] args )
        throws  UnableToGetDataException
    {
		throw (UnableToGetDataException)
            m_err.logl( new UnableToGetDataException(query).initCause(
                new UnsupportedOperationException("get_shorta")));    
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get_longa:1.0
    //
    public int[]
    get_longa( String query,
               String[] args)
        throws  UnableToGetDataException
    {
		throw (UnableToGetDataException)
            m_err.logl( new UnableToGetDataException(query).initCause(
                new UnsupportedOperationException("get_longa")));    
	}

    //
    // IDL:dp.aida.slac.stanford.edu/dpKlysTest/DpKlysI/get_stringa:1.0
    //
    public String[]
    get_stringa(String query,
                String[] args)
        throws  UnableToGetDataException
    {
		throw (UnableToGetDataException)
            m_err.logl( new UnableToGetDataException(query).initCause(
                new UnsupportedOperationException("get_stringa")));    
    }
	
}

