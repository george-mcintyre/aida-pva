// **********************************************************************
//
// Generated by the ORBacus IDL to Java Translator
//
// Copyright (c) 2002
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

// Version: 4.2.2

package edu.stanford.slac.aida.dp.dpSlcMosc;

//
// IDL:dp.aida.slac.stanford.edu/dpSlcMosc/DpSlcMoscI:1.0
//
final public class DpSlcMoscIHolder implements org.omg.CORBA.portable.Streamable
{
    public DpSlcMoscI value;

    public
    DpSlcMoscIHolder()
    {
    }

    public
    DpSlcMoscIHolder(DpSlcMoscI initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = DpSlcMoscIHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        DpSlcMoscIHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return DpSlcMoscIHelper.type();
    }
}
