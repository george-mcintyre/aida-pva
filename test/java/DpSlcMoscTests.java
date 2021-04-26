package test.java;

// The following libraries must me imported for all AIDA clients
// da defines objects used in AIDA clients
// util defines common objects available thoughout the AIDA system

import java.util.*;

import org.omg.CORBA.*;

import edu.stanford.slac.aida.lib.da.*;
import edu.stanford.slac.aida.lib.util.common.*;


/**
 * The SLC AIDA Test Suite. Tests Da access to SLC Master Oscillator data.
 *
 * @author  08-Jan-2007 Bob Hall. Based on DpSlcTests.java
 */
public class DpSlcMoscTests
{    
	private DaObject da;
    
	public DpSlcMoscTests(String[] args)
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
         * Test of Master Oscillator "get" method using DaObject getDaValue
         * (through DaServer).
         */
        private void simpleTest01()
        {
                System.out.println( "===========\n TEST 01 \n-------------");

                try
                {
                    String query = "MASTEROSC//VAL";

                    DaValue outData = da.getDaValue(query);

                    System.out.println(outData.toString());
                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }
                return;
        }

        /**
         * Test of Master Oscillator "get" method giving type through
         * DaServer.
         */
        private void simpleTest02()
        {
                System.out.println( "===========\n TEST 02 \n-------------");

                try
                {
                    String query = "MASTEROSC//VAL";

                    Double dv = (Double)da.get(query, DaValue.Type.DOUBLE);

                    System.out.println(dv.toString());
                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }
                return;
        }

        /**
         * Test of Master Oscillator "get" method using DaReference.
         */
        private void simpleTest03()
        {
                System.out.println( "===========\n TEST 03 \n-------------");

                try
                {
                    String query = "MASTEROSC//VAL";

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
         * Test of Master Oscillator "set" method using DaObject setDaValue
         * through DaServer). Set value is relative frequency.
         */
        private void simpleTest04()
        {
                System.out.println( "===========\n TEST 04 \n-------------");

                try
                {
                    String query = "MASTEROSC//VAL";

                    Float setValue = new Float(0.328f);

                    DaValue inData = new DaValue(setValue);

                    DaValue outData = da.setDaValue(query, inData);

                    double resultingAbsFreq =
                        ((Double) outData.firstElement()).doubleValue();

                    System.out.println("resultingAbsFreq = " + resultingAbsFreq);
                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }
                return;
        }

        /**
         * Test of Master Oscillator "set" method using DaObject setDaValue
         * through DaServer).   Set value is relative energy.  Ring is HER.
         */
        private void simpleTest05()
        {
                System.out.println( "===========\n TEST 05 \n-------------");

                try
                {
                    String query = "MASTEROSC//VAL";

                    da.setParam("UNITS", "ENERGY");
                    da.setParam("RING", "HER");

                    Float setValue = new Float(1.0f);

                    DaValue inData = new DaValue(setValue);

                    DaValue outData = da.setDaValue(query, inData);

                    double resultingAbsFreq =
                        ((Double) outData.firstElement()).doubleValue();

                    System.out.println("resultingAbsFreq = " + resultingAbsFreq);
                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }
                return;
        }

        /**
         * Test of Master Oscillator "set" method using DaObject setDaValue
         * through DaServer).   Set value is relative energy.  Ring is HER.
         */
        private void simpleTest06()
        {
                System.out.println( "===========\n TEST 06 \n-------------");

                try
                {
                    String query = "MASTEROSC//VAL";

                    da.setParam("UNITS", "ENERGY");
                    da.setParam("RING", "LER");

                    Float setValue = new Float(1.0f);

                    DaValue inData = new DaValue(setValue);

                    DaValue outData = da.setDaValue(query, inData);

                    double resultingAbsFreq =
                        ((Double) outData.firstElement()).doubleValue();

                    System.out.println("resultingAbsFreq = " + resultingAbsFreq);
                }
                catch (Exception e)
                {
                        System.err.println( e.getMessage() );
                }
                return;
        }

        private void simpleTest07()
        {
                System.out.println( "===========\n TEST 07 \n-------------");

                try
                {
                    String query = "MASTEROSC//VAL";

                    Float setValue = new Float(0.328f);

                    DaValue inData = new DaValue(setValue);

                    AnyHolder paramData = new AnyHolder(inData.toAny());

                    DaReference r = new DaReference(query, da);

                    Any a = r.set(paramData);

                    DaValue outData = new DaValue(a);

                    double resultingAbsFreq =
                        ((Double) outData.firstElement()).doubleValue();

                    System.out.println("resultingAbsFreq = " + resultingAbsFreq);
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

			default:
				System.out.println( "No such SlcMosc test number parameter " + 
						    testNumber + "=\"" + testNumber + "\"" );
				System.out.println( "usage: java daSlcMoscTest <test_number> ..." );
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
