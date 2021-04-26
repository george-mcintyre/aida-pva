package test.java;

// The following libraries must me imported for all AIDA clients
// da defines objects used in AIDA clients
// util defines common objects available thoughout the AIDA system

import java.util.*;

import org.omg.CORBA.*;

import edu.stanford.slac.aida.lib.da.*;
import edu.stanford.slac.aida.lib.util.common.*;


/**
 * The SLC AIDA Test Suite. Tests Da access to SLC Klystron data.
 *
 * @author  01-May-2007 Bob Hall. Based on DpSlcTests.java
 */
public class DpSlcKlysTests
{    
	private DaObject da;
    
	public DpSlcKlysTests(String[] args)
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
         * Acquire SHORT type through DaServer without reference.
         */
        private void simpleTest01()
        {
                System.out.println( "===========\n TEST 01 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");

                try
                {
                    da.setParam("BEAM=8");
                    da.setParam("DGRP=DEV_DGRP");
                    Short v = (Short) da.get("KLYS:LI31:31//TACT", DaValue.Type.SHORT);

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
        private void simpleTest02()
        {
                System.out.println( "===========\n TEST 02 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");

                try
                {
                    da.setParam("BEAM=8");
                    da.setParam("DGRP=DEV_DGRP");
                    String v = (String) da.get("KLYS:LI31:31//TACT", DaValue.Type.STRING);

                    System.out.println( "String TACT = " + v);

                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }
                return;
        }

        /**
         * Deactivate the specified klystron. 
         */
        private void simpleTest03()
        {
                System.out.println( "===========\n TEST 03 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");

                try
                {
                    String query = "KLYS:LI31:31//TACT";

                    da.setParam("BEAM=8");
                    da.setParam("DGRP=DEV_DGRP");

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
         * Reactivate the specified klystron. 
         */
        private void simpleTest04()
        {
                System.out.println( "===========\n TEST 04 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");

                try
                {
                    String query = "KLYS:LI31:31//TACT";

                    da.setParam("BEAM=8");
                    da.setParam("DGRP=DEV_DGRP");

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
         * Set a subbooster or klystron PDES value and trim it. 
         */
        private void simpleTest05()
        {
                System.out.println( "===========\n TEST 05 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");

                Float pdesValue = new Float(0.0f);

                try
                {
                    String defaultPmuString = "KLYS:LI31:31";

                    System.out.print(
                       "Enter subbooster or klystron PV name [enter C/R for " + 
                       defaultPmuString +  " ]:");

                    java.io.BufferedReader br;
                    br = new java.io.BufferedReader(new java.io.InputStreamReader(System.in));
                    String pmuString = br.readLine();
                    if (pmuString.length() == 0) pmuString = defaultPmuString;

                    String defaultPdesString = "90.0";

                    System.out.print(
                       "Enter subbooster or klystron PDES value [enter C/R for " + 
                       defaultPdesString +  " ]:");

                    br = new java.io.BufferedReader(new java.io.InputStreamReader(System.in));
                    String pdesString = br.readLine();
                    if (pdesString.length() == 0) pdesString = defaultPdesString;

                    try
                    {
                        pdesValue = Float.valueOf(pdesString.trim()); 
                    }
                    catch (NumberFormatException nfe)
                    {
                        System.out.println("NumberFormatException: " + nfe.getMessage());
                    }
 
                    String query = pmuString + "//PDES";

                    DaValue inData = new DaValue(pdesValue);

                    DaValue outData = da.setDaValue(query, inData);

                    float phasValue = ((Float) outData.firstElement()).floatValue();

                    System.out.println(pmuString + "  PHAS = " + phasValue);

                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }
                return;
        }

        /**
         * Set a subbooster or klystron PDES value without the trim operation.
         */
        private void simpleTest06()
        {
                System.out.println( "===========\n TEST 06 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");

                Float pdesValue = new Float(0.0f);

                try
                {
                    String defaultPmuString = "KLYS:LI31:31";

                    System.out.print(
                       "Enter subbooster or klystron PV name [enter C/R for " + 
                       defaultPmuString +  " ]:");

                    java.io.BufferedReader br;
                    br = new java.io.BufferedReader(new java.io.InputStreamReader(System.in));
                    String pmuString = br.readLine();
                    if (pmuString.length() == 0) pmuString = defaultPmuString;

                    String defaultPdesString = "90.0";

                    System.out.print(
                       "Enter subbooster or klystron PDES value [enter C/R for " + 
                       defaultPdesString +  " ]:");

                    br = new java.io.BufferedReader(new java.io.InputStreamReader(System.in));
                    String pdesString = br.readLine();
                    if (pdesString.length() == 0) pdesString = defaultPdesString;

                    try
                    {
                        pdesValue = Float.valueOf(pdesString.trim()); 
                    }
                    catch (NumberFormatException nfe)
                    {
                        System.out.println("NumberFormatException: " + nfe.getMessage());
                    }
 
                    String query = pmuString + "//PDES";

                    DaValue inData = new DaValue(pdesValue);

                    da.setParam("TRIM=NO");

                    DaValue outData = da.setDaValue(query, inData);

                    float phasValue = ((Float) outData.firstElement()).floatValue();

                    System.out.println(pmuString + "  PHAS = " + phasValue);

                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }
                return;
        }

        /**
         * Set a subbooster or klystron KPHR value.
         */
        private void simpleTest07()
        {
                System.out.println( "===========\n TEST 07 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");

                Float kphrValue = new Float(0.0f);

                try
                {
                    String defaultPmuString = "KLYS:LI31:31";

                    System.out.print(
                       "Enter subbooster or klystron PV name [enter C/R for " + 
                       defaultPmuString +  " ]:");

                    java.io.BufferedReader br;
                    br = new java.io.BufferedReader(new java.io.InputStreamReader(System.in));
                    String pmuString = br.readLine();
                    if (pmuString.length() == 0) pmuString = defaultPmuString;

                    String defaultKphrString = "60.0";

                    System.out.print(
                       "Enter subbooster or klystron KPHR value [enter C/R for " + 
                       defaultKphrString +  " ]:");

                    br = new java.io.BufferedReader(new java.io.InputStreamReader(System.in));
                    String kphrString = br.readLine();
                    if (kphrString.length() == 0) kphrString = defaultKphrString;

                    try
                    {
                        kphrValue = Float.valueOf(kphrString.trim()); 
                    }
                    catch (NumberFormatException nfe)
                    {
                        System.out.println("NumberFormatException: " + nfe.getMessage());
                    }
 
                    String query = pmuString + "//KPHR";

                    DaValue inData = new DaValue(kphrValue);

                    DaValue outData = da.setDaValue(query, inData);

                    float phasValue = ((Float) outData.firstElement()).floatValue();

                    System.out.println(pmuString + "  PHAS = " + phasValue);

                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }
                return;
        }

        /**
         * Set configuration secondary PCON value. 
         */
        private void simpleTest08()
        {
                System.out.println( "===========\n TEST 08 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");

                try
                {
                    String query = "KLYS:LI31:31//PCON";

                    Float configValue = new Float(5.0f);

                    DaValue inData = new DaValue(configValue);

                    da.setDaValue(query, inData);

                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }
                return;
        }

        /**
         * Set configuration secondary ACON value. 
         */
        private void simpleTest09()
        {
                System.out.println( "===========\n TEST 09 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");

                try
                {
                    String query = "KLYS:LI31:31//ACON";

                    Float configValue = new Float(5.0f);

                    DaValue inData = new DaValue(configValue);

                    da.setDaValue(query, inData);

                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }
                return;
        }

        private void simpleTest10()
        {
                System.out.println( "===========\n TEST 10 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");

                String query = "KLYS:LI31:31//PDES";

                try
                {
                    Float v = (Float)da.get(query, DaValue.Type.FLOAT);
                    System.out.println(query + " = " + v);

                    DaValue v1 = da.getDaValue(query);
                    System.out.println( "DaValue " + query + " = " + v1.toString());
                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }

                return;
        }

        private void simpleTest11()
        {
                System.out.println( "===========\n TEST 11 \n-------------");


                String query = "KLYS:LI31:31//KPHR";

                try
                {
                    Float v = (Float)da.get(query, DaValue.Type.FLOAT);
                    System.out.println(query + " = " + v);

                    DaValue v1 = da.getDaValue(query);
                    System.out.println( "DaValue " + query + " = " + v1.toString());
                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }

                return;
        }

        /**
         * Error reactivate klystron test with invalid beam code. 
         */
        private void simpleTest12()
        {
                System.out.println( "===========\n TEST 12 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");
                System.out.println( "Error test");

                try
                {
                    String query = "KLYS:LI31:31//TACT";

                    da.setParam("BEAM=XYZ");
                    da.setParam("DGRP=DEV_DGRP");

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

        private void simpleTest13()
        {
                System.out.println( "===========\n TEST 13 \n-------------");
                System.out.println( "This test should only be run in the PROD environment");

                try
                {
                    da.setParam("BEAM=1");
                    da.setParam("DGRP=LIN_KLYS");
                    Short v = (Short) da.get("KLYS:LI24:71//TACT", DaValue.Type.SHORT);

                    System.out.println( "Short TACT = " + v);

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

			case 12:
				simpleTest12();
				break;

			case 13:
				simpleTest13();
				break;

			default:
				System.out.println( "No such SlcKlys test number parameter " + 
						    testNumber + "=\"" + testNumber + "\"" );
				System.out.println( "usage: java daSlcKlysTest <test_number> ..." );
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
