// **********************************************************************
//
// Generated by the ORBacus IDL to Java Translator
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

// Version: 4.1.0

package edu.stanford.slac.aida.dp.dpSlc;

import org.omg.CORBA.*;
import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.except.*;
import edu.stanford.slac.err.*;

//
// IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI:1.0
//
//

public class DpSlcI_impl extends DpSlcIPOA
{
    private org.omg.PortableServer.POA poa_;
    private _PersistentDaObject da_;

    /*
    ** Native methods to access the SLC database
    */
    public native void DbInit() throws ServerInitialisationException;
    public native byte DbGetByte(String pvname) throws UnableToGetDataException;
    public native short DbGetShort(String pvname) throws UnableToGetDataException;
    public native int DbGetInt(String pvname) throws UnableToGetDataException;
    public native float DbGetFloat(String pvname) throws UnableToGetDataException;
    public native char DbGetChar(String pvname) throws UnableToGetDataException;
    public native String DbGetString(String pvname) throws UnableToGetDataException;

    public native byte[] DbGetByteA(String pvname) throws UnableToGetDataException;

    static DpSlcI_impl nat;  // Reference to invoke native methods

    static { System.loadLibrary("aidashr"); }

    static Err err;  // To log Slc errors

    // Constructors
    public
    DpSlcI_impl(_PersistentDaObject da, org.omg.PortableServer.POA poa)
        throws ServerInitialisationException
    {
        poa_ = poa;
	da_ = da;
        err = Err.getInstance();  // Get the Err singleton.
    }

    public org.omg.PortableServer.POA
    _default_POA()
    {
        if(poa_ != null)
            return poa_;
        else
            return super._default_POA();
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/init:1.0
    //
    public void
    init() throws ServerInitialisationException
    {
        nat = this; // Save class reference for future native calls
        try {
            nat.DbInit();
        }
        catch (Exception ex) {
            throw (ServerInitialisationException) err.log (ex, 
               " while trying to initialize for SLC database access.");
        }
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get:1.0
    //
    public org.omg.CORBA.Any
    get(String target,
        String[] args)
        throws UnableToGetDataException
    {
        System.out.println ("Entered Any get");
        // TODO: implement
        org.omg.CORBA.Any _r = _orb().create_any();
        return _r;
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get_byte:1.0
    //
    public byte
    get_byte(String target,
             String[] args)
        throws UnableToGetDataException
    {
        System.out.println ("Entered get_byte");
        try {
            return nat.DbGetByte (queryToSlcName (target));
        }
        catch (Exception ex) {
            String msg = " while trying to get byte " + target;
            throw (UnableToGetDataException) err.log (ex, msg);
        }
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get_short:1.0
    //
    public short
    get_short(String target,
              String[] args)
        throws UnableToGetDataException
    {
        System.out.println ("Entered get_short");
        try {
            return nat.DbGetShort (queryToSlcName (target));
        }
        catch (Exception ex) {
            String msg = " while trying to get short " + target;
            throw (UnableToGetDataException) err.log (ex, msg);
        }
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get_long:1.0
    //
    public int
    get_long(String target, String[] args) throws UnableToGetDataException
    {
        System.out.println ("Entered get_long");
        try {
            return nat.DbGetInt (queryToSlcName (target));
        }
        catch (Exception ex) {
            String msg = " while trying to get int " + target;
            throw (UnableToGetDataException) err.log (ex, msg);
        }
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get_boolean:1.0
    //
    public boolean
    get_boolean(String target,
                String[] args)
        throws UnableToGetDataException
    {
        System.out.println ("Entered get_boolean");
        // TODO: implement for SLC?
        boolean _r = false;
        return _r;
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get_double:1.0
    //
    public double
    get_double(String target,
               String[] args)
        throws UnableToGetDataException
    {
        System.out.println ("Entered get_double");
        // TODO: implement for SLC?
        double _r = 0.0;
        return _r;
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get_float:1.0
    //
    public float
    get_float(String target,
              String[] args)
        throws UnableToGetDataException
    {
        System.out.println ("Entered get_float");
        try {
            return nat.DbGetFloat (queryToSlcName (target));
        }
        catch (Exception ex) {
            String msg = " while trying to get float " + target;
            throw (UnableToGetDataException) err.log (ex, msg);
        }
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get_string:1.0
    //
    public String
    get_string(String target,
               String[] args)
        throws UnableToGetDataException
    {
        System.out.println ("Entered get_string");
        try {
            return nat.DbGetString (queryToSlcName (target));
        }
        catch (Exception ex) {
            String msg = " while trying to get string " + target;
            throw (UnableToGetDataException) err.log (ex, msg);
        }
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get_char:1.0
    //
    public char
    get_char(String target,
             String[] args)
        throws UnableToGetDataException
    {
        System.out.println ("Entered get_char");
//      Implement this for SLC?
        char _r = (char)0;
        return _r;
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get_wstring:1.0
    //
    public String
    get_wstring(String target,
                String[] args)
        throws UnableToGetDataException
    {
        System.out.println ("Entered get_wstring");
        // TODO: implement
        String _r = "";
        return _r;
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get_wchar:1.0
    //
    public char
    get_wchar(String target,
              String[] args)
        throws UnableToGetDataException
    {
        System.out.println ("Entered get_wchar");
        // TODO: implement
        char _r = (char)0;
        return _r;
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get_bytea:1.0
    //
    public byte[]
    get_bytea(String target,
              String[] args)
        throws UnableToGetDataException
    {
        System.out.println ("Entered get_bytea");
        try {
            return nat.DbGetByteA (queryToSlcName (target));
        }
        catch (Exception ex) {
            String msg = " while trying to get byte array " + target;
            throw (UnableToGetDataException) err.log (ex, msg);
        }
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get_shorta:1.0
    //
    public short[]
    get_shorta(String target,
               String[] args)
        throws UnableToGetDataException
    {
        System.out.println ("Entered get_shorta");
/*
        try {
            return nat.DbGetShortA (queryToSlcName (target));
        }
        catch (Exception ex) {
            String msg = " while trying to get short array " + target;
            throw (UnableToGetDataException) err.log (ex, msg);
        }
*/
        // TODO: implement
        short[] _r = new short[0];
        return _r;
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get_longa:1.0
    //
    public int[]
    get_longa(String target,
              String[] args)
        throws UnableToGetDataException
    {
        System.out.println ("Entered get_longa");
/*
        try {
            return nat.DbGetLongA (queryToSlcName (target));
        }
        catch (Exception ex) {
            String msg = " while trying to get long array " + target;
            throw (UnableToGetDataException) err.log (ex, msg);
        }
*/
        // TODO: implement
        int[] _r = new int[0];
        return _r;
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get_booleana:1.0
    //
    public boolean[]
    get_booleana(String target,
                 String[] args)
        throws UnableToGetDataException
    {
        System.out.println ("Entered get_booleana");
/*
        try {
            return nat.DbGetBoolA (queryToSlcName (target));
        }
        catch (Exception ex) {
            String msg = " while trying to get boolean array " + target;
            throw (UnableToGetDataException) err.log (ex, msg);
        }
*/
        // TODO: implement
        boolean[] _r = new boolean[0];
        return _r;
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get_doublea:1.0
    //
    public double[]
    get_doublea(String target,
                String[] args)
        throws UnableToGetDataException
    {
        System.out.println ("Entered get_doublea");
/*
        try {
            return nat.DbGetDoubleA (queryToSlcName (target));
        }
        catch (Exception ex) {
            String msg = " while trying to get double array " + target;
            throw (UnableToGetDataException) err.log (ex, msg);
        }
*/
        // TODO: implement
        double[] _r = new double[0];
        return _r;
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get_floata:1.0
    //
    public float[]
    get_floata(String target,
               String[] args)
        throws UnableToGetDataException
    {
        System.out.println ("Entered get_floata");
/*
        try {
            return nat.DbGetFloatA (queryToSlcName (target));
        }
        catch (Exception ex) {
            String msg = " while trying to get float array " + target;
            throw (UnableToGetDataException) err.log (ex, msg);
        }
*/
        // TODO: implement
        float[] _r = new float[0];
        return _r;
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get_stringa:1.0
    //
    public String[]
    get_stringa(String target,
                String[] args)
        throws UnableToGetDataException
    {
        System.out.println ("Entered get_stringa");
/*
        try {
            return nat.DbGetStringA (queryToSlcName (target));
        }
        catch (Exception ex) {
            String msg = " while trying to get string array " + target;
            throw (UnableToGetDataException) err.log (ex, msg);
        }
*/
        // TODO: implement
        String[] _r = new String[0];
        return _r;
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get_chara:1.0
    //
    public char[]
    get_chara(String target,
              String[] args)
        throws UnableToGetDataException
    {
        System.out.println ("Entered get_chara");
/*
        try {
            return nat.DbGetCharA (queryToSlcName (target));
        }
        catch (Exception ex) {
            String msg = " while trying to get char array " + target;
            throw (UnableToGetDataException) err.log (ex, msg);
        }
*/
        // TODO: implement
        char[] _r = new char[0];
        return _r;
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/get_wchara:1.0
    //
    public char[]
    get_wchara(String target,
               String[] args)
        throws UnableToGetDataException
    {
        System.out.println ("Entered get_wchara");
        // TODO: implement
        char[] _r = new char[0];
        return _r;
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/set:1.0
    //
    public void
    set(String target,
        org.omg.CORBA.AnyHolder value)
        throws UnableToSetDataException
    {
        System.out.println ("Entered set");
        // TODO: implement
        value.value = _orb().create_any();
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/startMonitor:1.0
    //
    public void
    startMonitor(String name)
        throws MonitorStartupException
    {
        System.out.println ("Entered startMonitor");
        // TODO: implement
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/stopMonitor:1.0
    //
    public void
    stopMonitor(String name)
        throws MonitorShutdownException
    {
        System.out.println ("Entered stopMonitor");
        // TODO: implement
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/status:1.0
    //
    public int
    status()
    {
        System.out.println ("Entered status");
        // TODO: implement
        int _r = 0;
        return _r;
    }

    //
    // IDL:dp.aida.slac.stanford.edu/dpSlc/DpSlcI/shutdown:1.0
    //
    public void
    shutdown()
    {
        System.out.println ("Entered shutdown");
        // TODO: implement
    }

    /*******************************
    ** Private methods
    *******************************/

    /*
    ** Converts and Aida query string that are of the form instance//attribute
    ** to an SLC p:m:u.s form by replacing  "//" with "."/
    **
    ** @param Aida query string
    ** @return SLC pmu.s string
    */

    private String queryToSlcName (String query)
    {
        int iSep = query.lastIndexOf(AQSToken.SEPARATOR);
        return query.substring(0,iSep) + "." + query.substring(iSep+2);
    }    
}