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

//
// IDL:dp.aida.slac.stanford.edu/dpSlcHist/DpSlcHistI:1.0
//
final public class DpSlcHistIHolder implements org.omg.CORBA.portable.Streamable
{
    public DpSlcHistI value;

    public
    DpSlcHistIHolder()
    {
    }

    public
    DpSlcHistIHolder(DpSlcHistI initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = DpSlcHistIHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        DpSlcHistIHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return DpSlcHistIHelper.type();
    }
}
