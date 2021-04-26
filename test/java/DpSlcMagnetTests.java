package test.java;

// The following libraries must me imported for all AIDA clients
// da defines objects used in AIDA clients
// util defines common objects available thoughout the AIDA system

import java.util.*;

import org.omg.CORBA.*;

import edu.stanford.slac.aida.lib.da.*;
import edu.stanford.slac.aida.lib.util.common.*;


/**
 * The SLC AIDA Test Suite. Tests Da access to SLC Magnet data.
 *
 * @author  15-June-2006 Bob Hall. Based on DpSlcTests.java
 */
public class DpSlcMagnetTests
{    
	private DaObject da;
    
	public DpSlcMagnetTests(String[] args)
	{
		try
		{
			da = new DaObject(args);
		}
		catch ( Exception e )
		{
			System.err.println( e.getMessage() );
			e.printStackTrace();
		}
	}

        /**
         * Test of magnet "get" method using DaObject getDaValue (through
         * DaServer), without parameters.
         */    
	private void simpleTest01()
	{
		System.out.println( "===========\n TEST 01 \n-------------");

		try
		{
                    System.out.println("Getting data for DEV_DGRP//XCOR:BDES");

                    String query = "DEV_DGRP//XCOR:BDES";

                    System.out.println("making call to getDaValue");

                    DaValue v = da.getDaValue(query);

                    System.out.println(v.toString());

                    da.reset();
		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}
		return;
	}

        /**
         * Test of magnet "get" method using DaObject getDaValue (through
         * DaServer), with parameters.
         */    
	private void simpleTest02()
	{
		System.out.println( "===========\n TEST 02 \n-------------");

		try
		{
                    System.out.println("Getting data for DEV_DGRP//XCOR:BDES");

                    String query = "DEV_DGRP//XCOR:BDES";

                    System.out.println("making call to getDaValue");

                    da.setParam("MICROS", "LI31-LI31");
                    da.setParam("UNITS", "1-100");

                    DaValue v = da.getDaValue(query);

                    System.out.println(v.toString());

                    da.reset();
		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}
		return;
	}

        /**
         * Test of magnet "get" method using DaObject getDaValue (through
         * DaServer), without parameters, and extracting results from vectors.
        */
	private void simpleTest03()
	{
		System.out.println( "===========\n TEST 03 \n-------------");

                int i;

		try
		{
                    System.out.println("Getting data for DEV_DGRP//XCOR:BDES");

                    String query = "DEV_DGRP//XCOR:BDES";

                    System.out.println("making call to getDaValue");

                    DaValue v = da.getDaValue(query);

                    Vector names = (Vector)v.get(0);

                    System.out.println("names = " + names);
 
                    Vector secnValues = (Vector)v.get(1);

                    System.out.println("secnValues = " + secnValues);

                    int numPvs = names.size();

                    System.out.println("numPvs = " + numPvs); 

                    for (i = 0; i < numPvs; i++)
                    {
                        String name = names.elementAt(i).toString();
                        System.out.println("name = " + name);

                        float secnValue = ((Float)secnValues.elementAt(i)).floatValue();
                        System.out.println("secnValue = " + secnValue); 
                    }

                    da.reset();
		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}
		return;
	}

        /**
         * Test of magnet "get" method using DaReference, without
         * parameters.
        */
	private void simpleTest04()
	{
		System.out.println( "===========\n TEST 04 \n-------------");

		try
		{
                    System.out.println("Getting data for DEV_DGRP//XCOR:BDES");

                    String query = "DEV_DGRP//XCOR:BDES";

                    DaReference r = new DaReference(query, da);

                    System.out.println("making call to getAny");

                    Any anyData = r.getAny();

                    DaValue v = new DaValue(anyData);

                    System.out.println(v.toString());

		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}
		return;
	}

        /**
         * Test of magnet "set" method using DaObject getDaValue (through
         * DaServer).
         */    
	private void simpleTest05()
	{
		System.out.println( "===========\n TEST 05 \n-------------");

		try
		{
                    String query = "MAGNETSET//BDES";

                    String [] stringsParam = {"XCOR:LI31:41"};

                    float [] setValues = {4.0f};

                    DaValue inData = new DaValue();

                    inData.type = DaValue.Type.STRUCT;

                    inData.addElement(new DaValue(stringsParam));
                    inData.addElement(new DaValue(setValues));

                    da.setParam("MAGFUNC", "TRIM");

                    DaValue outData = da.setDaValue(query, inData);

                    System.out.println("outData = " + outData.toString()); 
		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}

		return;
	}

        /**
         * Test of magnet "set" method using DaObject getDaValue (through
         * DaServer) and extracting results from vectors.
        */
	private void simpleTest06()
	{
		System.out.println( "===========\n TEST 06 \n-------------");

                int i;

		try
		{
                    String query = "MAGNETSET//BDES";

                    String [] stringsParam = {"XCOR:LI31:41"};

                    float [] setValues = {4.0f};

                    DaValue inData = new DaValue();

                    inData.type = DaValue.Type.STRUCT;

                    inData.addElement(new DaValue(stringsParam));
                    inData.addElement(new DaValue(setValues));

                    da.setParam("MAGFUNC", "TRIM");

                    DaValue outData = da.setDaValue(query, inData);

                    Vector nameStates = (Vector)outData.get(0);

                    System.out.println("nameStates = " + nameStates);
 
                    Vector bactValues = (Vector)outData.get(1);

                    System.out.println("bactValues = " + bactValues);

                    int numPvs = bactValues.size();

                    System.out.println("numPvs = " + numPvs); 

                    for (i = 0; i < numPvs; i++)
                    {
                        String state = nameStates.elementAt(i).toString();
                        System.out.println("state = " + state);

                        float bact = ((Float)bactValues.elementAt(i)).floatValue();
                        System.out.println("bact = " + bact); 
                    }

		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}

		return;
	}

        /**
         * Test of magnet "set" method using DaReference.
        */
	private void simpleTest07()
	{
		System.out.println( "===========\n TEST 07 \n-------------");

		try
		{
                    String query = "MAGNETSET//BDES";

                    String [] stringsParam = {"XCOR:LI31:41"};

                    float [] setValues = {4.0f};

                    DaValue inData = new DaValue();

                    inData.type = DaValue.Type.STRUCT;

                    inData.addElement(new DaValue(stringsParam));
                    inData.addElement(new DaValue(setValues));

                    AnyHolder paramData = new AnyHolder(inData.toAny());

                    DaReference r = new DaReference(query, da);

                    r.setParam("MAGFUNC", "TRIM");

                    Any a = r.set(paramData);

                    DaValue outData = new DaValue(a);

                    System.out.println("outData = " + outData.toString()); 
		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}

		return;
	}

        /**
         * Error handling test of magnet "set" method using DaReference.
        */
	private void simpleTest08()
	{
		System.out.println( "===========\n TEST 08 \n-------------");

		try
		{
                    String query = "MAGNETSET//BDES";

                    String [] stringsParam = {"XCOZ:LI31:41"};

                    float [] setValues = {4.0f};

                    DaValue inData = new DaValue();

                    inData.type = DaValue.Type.STRUCT;

                    inData.addElement(new DaValue(stringsParam));
                    inData.addElement(new DaValue(setValues));

                    AnyHolder paramData = new AnyHolder(inData.toAny());

                    DaReference r = new DaReference(query, da);

                    r.setParam("MAGFUNC", "TRIM");

                    Any a = r.set(paramData);

                    DaValue outData = new DaValue(a);

                    System.out.println("outData = " + outData.toString()); 
		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}

		return;
	}

        /**
         * Test of magnet "set" method using DaReference.
        */
	private void simpleTest09()
	{
		System.out.println( "===========\n TEST 09 \n-------------");

		try
		{
                    String query = "MAGNETSET//BDES";

                    String [] stringsParam = {"XCOR:LI31:41"};

                    float [] setValues = {10.0f};

                    DaValue inData = new DaValue();

                    inData.type = DaValue.Type.STRUCT;

                    inData.addElement(new DaValue(stringsParam));
                    inData.addElement(new DaValue(setValues));

                    AnyHolder paramData = new AnyHolder(inData.toAny());

                    DaReference r = new DaReference(query, da);

                    r.setParam("MAGFUNC", "TRIM");

                    Any a = r.set(paramData);

                    DaValue outData = new DaValue(a);

                    System.out.println("outData = " + outData.toString()); 
		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}

		return;
	}

        /**
         * Test of magnet "set" method using DaObject getDaValue (through
         * DaServer) without trim or perturb.
         */    
	private void simpleTest10()
	{
		System.out.println( "===========\n TEST 10 \n-------------");

		try
		{
                    String query = "MAGNETSET//BDES";

                    String [] stringsParam = {"XCOR:LI31:41"};

                    float [] setValues = {5.0f};

                    DaValue inData = new DaValue();

                    inData.type = DaValue.Type.STRUCT;

                    inData.addElement(new DaValue(stringsParam));
                    inData.addElement(new DaValue(setValues));

                    da.setParam("MAGFUNC", "NOFUNC");

                    da.setDaValue(query, inData);

		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}

		return;
	}

        /**
         * Test of magnet "set" method using DaObject getDaValue (through
         * DaServer) setting the configuration secondary BCON.
         */    
	private void simpleTest11()
	{
		System.out.println( "===========\n TEST 11 \n-------------");

		try
		{
                    String query = "MAGNETSET//BCON";

                    String [] stringsParam = {"XCOR:LI31:41"};

                    float [] setValues = {5.0f};

                    DaValue inData = new DaValue();

                    inData.type = DaValue.Type.STRUCT;

                    inData.addElement(new DaValue(stringsParam));
                    inData.addElement(new DaValue(setValues));

                    da.setDaValue(query, inData);

		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}

		return;
	}

	public void run(int testNumber )
	{
		try
		{
			switch ( testNumber )
			{
			case 1:
				simpleTest01();
				break;

			case 2:
				simpleTest02();
				break;

			case 3:
				simpleTest03();
				break;

			case 4:
				simpleTest04();
				break;

			case 5:
				simpleTest05();
				break;

			case 6:
				simpleTest06();
				break;

			case 7:
				simpleTest07();
				break;

			case 8:
				simpleTest08();
				break;

			case 9:
				simpleTest09();
				break;

			case 10:
				simpleTest10();
				break;

			case 11:
				simpleTest11();
				break;
                    
			default:
				System.out.println( "No such SlcMagnet test number parameter " + 
						    testNumber + "=\"" + testNumber + "\"" );
				System.out.println( "usage: java daSlcMagnetTest <test_number> ..." );
			}
		}
		catch (Exception e)
		{
			System.out.println( "Exception: Executing test " + testNumber);
			System.err.println( e.getMessage() + " caused by " + e );
		}
		return;
	}
}
