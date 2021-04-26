package test.java;

// The following libraries must me imported for all AIDA clients
// da defines objects used in AIDA clients
// util defines common objects available thoughout the AIDA system

import java.util.*;

import org.omg.CORBA.*;

import edu.stanford.slac.aida.lib.da.*;
import edu.stanford.slac.aida.lib.util.common.*;


/**
 * The SLC AIDA Test Suite. Tests Da access to SLC Utility data.
 *
 * @author  04-Dec-2006 Bob Hall. Based on DpSlcTests.java
 * @version 01-Aug-2007 Bob Hall. Added trigger get activation status, deactivate, and reactivate functionality.
 * @version 07-Aug-2007 Bob Hall. Added BGRP set variable functionality.
 */
public class DpSlcUtilTests
{    
	private DaObject da;
    
	public DpSlcUtilTests(String[] args)
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
         * Use DaObject setDaValue method and DaServer process.  Extract device
         * string and secondary value vectors.
         */
        private void simpleTest01()
        {
                System.out.println( "===========\n TEST 01 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");
                int i;
                int j;

                try
                {
                    String query = "MKB//VAL";

                    da.setParam("MKB", "mkb:gregstestli31.mkb");

                    Float relativeDelta = new Float(0.1f);

                    DaValue inData = new DaValue(relativeDelta);

                    for (i = 0; i < 3; i++)
                    {
                        DaValue outData = da.setDaValue(query, inData);

                        Vector deviceNames = (Vector)outData.get(0);

                        Vector secnValues = (Vector)outData.get(1);

                        int numDevices = deviceNames.size();

                        System.out.println("numDevices = " + numDevices);

                        for (j = 0; j < numDevices; j++)
                        {
                            String device = deviceNames.elementAt(j).toString();
                            System.out.println("device = " + device);

                            float secn = ((Float)secnValues.elementAt(j)).floatValue();
                            System.out.println("secn value = " + secn);
                        }

                        System.out.println("sleeping for 5 seconds");
                        Thread.sleep(5000);

                        System.out.println(" ");
                    }

                    System.out.println("*** reversing sign of relative delta");
                    System.out.println(" "); 

                    relativeDelta = new Float(-0.1f);

                    inData = new DaValue(relativeDelta);

                    for (i = 0; i < 3; i++)
                    {
                        DaValue outData = da.setDaValue(query, inData);

                        Vector deviceNames = (Vector)outData.get(0);

                        Vector secnValues = (Vector)outData.get(1);

                        int numDevices = deviceNames.size();

                        System.out.println("numDevices = " + numDevices);

                        for (j = 0; j < numDevices; j++)
                        {
                            String device = deviceNames.elementAt(j).toString();
                            System.out.println("device = " + device);

                            float secn = ((Float)secnValues.elementAt(j)).floatValue();
                            System.out.println("secn value = " + secn);
                        }

                        System.out.println("sleeping for 5 seconds");
                        Thread.sleep(5000);

                        System.out.println(" ");
                    }

                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }
                return;
        }


        /**
         * Use DaReference set method. 
         */    
	private void simpleTest02()
	{
		System.out.println( "===========\n TEST 02 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");

                int i;

		try
		{
                    String query = "MKB//VAL";

                    DaReference r = new DaReference(query, da); 

                    r.setParam("MKB", "mkb:gregstestli31.mkb");

                    for (i = 0; i < 3; i++)
                    {
                        Float relativeDelta = new Float(0.1f);

                        DaValue inData = new DaValue(relativeDelta);

                        AnyHolder paramData = new AnyHolder(inData.toAny());

                        Any a = r.set(paramData);

                        DaValue outData = new DaValue(a);

                        System.out.println("outData = " + outData.toString());

                        System.out.println("sleeping for 5 seconds");
                        Thread.sleep(5000);

                        System.out.println(" ");
                    }

                    System.out.println("*** reversing sign of relative delta");
                    System.out.println(" "); 

                    for (i = 0; i < 3; i++)
                    {
                        Float relativeDelta = new Float(-0.1f);

                        DaValue inData = new DaValue(relativeDelta);

                        AnyHolder paramData = new AnyHolder(inData.toAny());

                        Any a = r.set(paramData);

                        DaValue outData = new DaValue(a);

                        System.out.println("outData = " + outData.toString());

                        System.out.println("sleeping for 5 seconds");
                        Thread.sleep(5000);

                        System.out.println(" ");
                    }

		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}
		return;
	}

        /**
         * Test of specified absolute multiknob file, which is not permitted.
         */    
	private void simpleTest03()
	{
		System.out.println( "===========\n TEST 03 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");
                System.out.println( "The requested set operation should fail since the");
                System.out.println( "specified multiknob file is absolute, which is not permitted.");

                int i;

		try
		{
                    String query = "MKB//VAL";

                    DaReference r = new DaReference(query, da); 

                    r.setParam("MKB", "mkb:li31test.mkb");

                    for (i = 0; i < 3; i++)
                    {
                        Float relativeDelta = new Float(0.1f);

                        DaValue inData = new DaValue(relativeDelta);

                        AnyHolder paramData = new AnyHolder(inData.toAny());

                        Any a = r.set(paramData);

                        DaValue outData = new DaValue(a);

                        System.out.println("outData = " + outData.toString());

                        System.out.println("sleeping for 5 seconds");
                        Thread.sleep(5000);

                        System.out.println(" ");
                    }

                    System.out.println("*** reversing sign of relative delta");
                    System.out.println(" "); 

                    for (i = 0; i < 3; i++)
                    {
                        Float relativeDelta = new Float(-0.1f);

                        DaValue inData = new DaValue(relativeDelta);

                        AnyHolder paramData = new AnyHolder(inData.toAny());

                        Any a = r.set(paramData);

                        DaValue outData = new DaValue(a);

                        System.out.println("outData = " + outData.toString());

                        System.out.println("sleeping for 5 seconds");
                        Thread.sleep(5000);

                        System.out.println(" ");
                    }

		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}
		return;
	}


        /**
         * PROD environment test that uses DaObject setDaValue method and DaServer process.
         * Extract device string and secondary value vectors.
         */
        private void simpleTest04()
        {
                System.out.println( "===========\n TEST 04 \n-------------");
                System.out.println( "This test should only be run in the PROD environment");

                int i;
                int j;

                try
                {
                    String query = "MKB//VAL";

                    da.setParam("MKB", "mkb:li02b_xb.mkb");

                    Float relativeDelta = new Float(1.0f);

                    DaValue inData = new DaValue(relativeDelta);

                    for (i = 0; i < 3; i++)
                    {
                        DaValue outData = da.setDaValue(query, inData);

                        Vector deviceNames = (Vector)outData.get(0);

                        Vector secnValues = (Vector)outData.get(1);

                        int numDevices = deviceNames.size();

                        System.out.println("numDevices = " + numDevices);

                        for (j = 0; j < numDevices; j++)
                        {
                            String device = deviceNames.elementAt(j).toString();
                            System.out.println("device = " + device);

                            float secn = ((Float)secnValues.elementAt(j)).floatValue();
                            System.out.println("secn value = " + secn);
                        }

                        System.out.println("sleeping for 5 seconds");
                        Thread.sleep(5000);

                        System.out.println(" ");
                    }

                    System.out.println("*** reversing sign of relative delta"); 
                    System.out.println(" "); 

                    relativeDelta = new Float(-1.0f);

                    inData = new DaValue(relativeDelta);

                    for (i = 0; i < 3; i++)
                    {
                        DaValue outData = da.setDaValue(query, inData);

                        Vector deviceNames = (Vector)outData.get(0);

                        Vector secnValues = (Vector)outData.get(1);

                        int numDevices = deviceNames.size();

                        System.out.println("numDevices = " + numDevices);

                        for (j = 0; j < numDevices; j++)
                        {
                            String device = deviceNames.elementAt(j).toString();
                            System.out.println("device = " + device);

                            float secn = ((Float)secnValues.elementAt(j)).floatValue();
                            System.out.println("secn value = " + secn);
                        }

                        System.out.println("sleeping for 5 seconds");
                        Thread.sleep(5000);

                        System.out.println(" ");
                    }

                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }
                return;
        }

        /**
         * Acquire SHORT type through DaServer without reference.
         */
        private void simpleTest05()
        {
                System.out.println( "===========\n TEST 05 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");

                try
                {
                    da.setParam("BEAM=1");
                    Short v = (Short) da.get("TRIG:LI31:109//TACT", DaValue.Type.SHORT);

                    System.out.println( "Short TACT = " + v);

                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }
                return;
        }

        /**
         * Acquire STRING type through DaServer without reference.
         */
        private void simpleTest06()
        {
                System.out.println( "===========\n TEST 06 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");

                try
                {
                    da.setParam("BEAM=1");
                    String v = (String) da.get("TRIG:LI31:109//TACT", DaValue.Type.STRING);

                    System.out.println( "String TACT = " + v);

                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }
                return;
        }

        /**
         * Deactivate the specified TRIG device. 
         */
        void simpleTest07()
        {
                System.out.println( "===========\n TEST 07 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");

                try
                {
                    String query = "TRIG:LI31:109//TACT";

                    da.setParam("BEAM=1");

                    Short setValue = new Short((short) 0);

                    DaValue inData = new DaValue(setValue);

                    DaValue outData = da.setDaValue(query, inData);

                    short status = ((Short) outData.firstElement()).shortValue();

                    System.out.println( "String TACT = " + status);

                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }
                return;
        }

        /**
         * Reactivate the specified TRIG device. 
         */
        private void simpleTest08()
        {
                System.out.println( "===========\n TEST 08 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");

                try
                {
                    String query = "TRIG:LI31:109//TACT";

                    da.setParam("BEAM=1");

                    Short setValue = new Short((short) 1);

                    DaValue inData = new DaValue(setValue);

                    DaValue outData = da.setDaValue(query, inData);

                    short status = ((Short) outData.firstElement()).shortValue();

                    System.out.println( "String TACT = " + status);

                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }
                return;
        }

        /**
         * Set the specified variable of a specified BGRP to the new value "N".
         */
        private void simpleTest09()
        {
                System.out.println( "===========\n TEST 09 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");

                try
                {
                    String query = "BGRP//VAL";

                    da.setParam("BGRP=LCLS");
                    da.setParam("VARNAME=T_CAV");

                    String setValue = "N";

                    DaValue inData = new DaValue(setValue);

                    da.setDaValue(query, inData);
                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }
                return;
        }

        /**
         * Set the specified variable of a specified BGRP to the new value "Y".
         */
        private void simpleTest10()
        {
                System.out.println( "===========\n TEST 10 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");

                try
                {
                    String query = "BGRP//VAL";

                    da.setParam("BGRP=LCLS");

                    da.setParam("VARNAME=T_CAV");

                    String setValue = "Y";

                    DaValue inData = new DaValue(setValue);

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

			default:
				System.out.println( "No such SlcUtil test number parameter " + 
						    testNumber + "=\"" + testNumber + "\"" );
				System.out.println( "usage: java daSlcUtilTest <test_number> ..." );
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
