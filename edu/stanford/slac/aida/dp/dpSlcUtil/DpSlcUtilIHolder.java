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

package edu.stanford.slac.aida.dp.dpSlcUtil;

//
// IDL:dp.aida.slac.stanford.edu/dpSlcUtil/DpSlcUtilI:1.0
//
final public class DpSlcUtilIHolder implements org.omg.CORBA.portable.Streamable
{
    public DpSlcUtilI value;

    public
    DpSlcUtilIHolder()
    {
    }

    public
    DpSlcUtilIHolder(DpSlcUtilI initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = DpSlcUtilIHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        DpSlcUtilIHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return DpSlcUtilIHelper.type();
    }
}