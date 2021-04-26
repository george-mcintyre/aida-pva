package test.java;

// The following libraries must me imported for all AIDA clients
// da defines objects used in AIDA clients
// util defines common objects available thoughout the AIDA system
import edu.stanford.slac.aida.lib.da.*;
import edu.stanford.slac.aida.lib.util.common.*;


/**
 * The SLC AIDA Test Suite. Tests Da access to SLC data.
 *
 * @author  11-Sep-2002 Robert C. Sass. Based on dpCaTests.java
 * @version 20-Nov-2002, ???Ron MacKenzie.  Change except from aida to pkg except.
 * @version 14-Oct-2004, Bob Hall.  Added many new tests for newly implemented functions.
 * @version 24-Apr-2006, Bob Hall.  Added a new test for ASTS channel names.
 * @version 09-Jun-2006, Bob Hall.  Added testing for ASTS channels with STAT.
 */
public class DpSlcTests
{    
	private DaObject da;
    
	public DpSlcTests(String[] args)
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
	 * Acquire FLOAT type SLC PMUS, giving type, through DaServer, without reference.
	 */ 
	private void simpleTest01()
	{
		System.out.println( "===========\n TEST 01 \n-------------");

		try
		{
			Float v = (Float)da.get("XCOR:LI03:120//LEFF", DaValue.Type.FLOAT);
			System.out.println( "Float BACT=" + v);
			Double dv = (Double)da.get("XCOR:LI03:120//LEFF", DaValue.Type.DOUBLE);
			System.out.println( "Double BACT=" + dv);

			Boolean bool = (Boolean)da.get("XCOR:LI03:120//VSTA", 
						       DaValue.Type.BOOLEAN);
			System.out.println( "Boolean VSTA = " + bool); 
			Short s = (Short)da.get("XCOR:LI03:120//SETL", DaValue.Type.SHORT);
			System.out.println( "Short SETL=" + s);
			Byte b = (Byte)da.get("XCOR:LI03:120//ADCP", DaValue.Type.BYTE);
			System.out.println( "Byte ADCP=" + b);

			Character c = (Character)da.get("XCOR:LI03:120//ADCP", 
							DaValue.Type.CHAR);
			System.out.println( "Char ADCP=" + c);
			Character wc = (Character)da.get("XCOR:LI03:120//ADCP", 
							 DaValue.Type.WCHAR);
			System.out.println( "Wchar ADCP=" + wc);

			String st = (String)da.get("XCOR:LI03:120//PSNM", 
						   DaValue.Type.STRING);
			System.out.println( "String PSNM=" + st);
			String wst = (String)da.get("XCOR:LI03:120//PSNM", 
						    DaValue.Type.WSTRING);
			System.out.println( "WString PSNM=" + wst);

			Byte[] ba =(Byte[])da.geta("XCOR:LI03:120//PSNM",DaValue.Type.BYTEA);
			System.out.println( "Byte array PSNM =");
			for (int i = 0; i < ba.length; i++)
			{
				System.out.println(ba[i]);
			}

			Character[] ca =(Character[]) da.geta("XCOR:LI03:120//PSNM",
							      DaValue.Type.CHARA);
			System.out.println( "Char array PSNM =");
			for (int i = 0; i < ca.length; i++)
			{
				System.out.println(ca[i]);
			}
		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}
		return;
	}
    
	/**
	 * Acquire FLOAT type SLC PMUS, giving type, through DaServer, with reference.
	 */ 
	private void simpleTest02()
	{
		System.out.println( "=============\n TEST 02 \n-------------");    
		try
		{
			String query = "XCOR:LI03:120//BACT";
			DaReference r = new DaReference(query, da);
			r.setType(DaValue.Type.FLOAT);
			Float v = (Float)da.get(r);
            
			System.out.println( query + "\t=" + v);
		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}
		return;
	}
    
	/**
	 * Acquire LONG INT type SLC PMUS, giving type, through DaServer, with reference.
	 */ 
	private void simpleTest03()
	{
		System.out.println( "=============\n TEST 03 \n------------"); 
		try
		{
			String query = "XCOR:LI03:120//VSTA";
            
			DaReference r = new DaReference(query, da);
			r.setType(DaValue.Type.LONG);
			Long v = (Long)da.get(r);
            
			System.out.println( query + "\t=" + v);   
		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}
		return;
	}
    
	/**
	 * Acquire array of FLOAT type SLC PMUS, giving type, through DaServer, with reference.
	 */ 
	private void simpleTest04()
	{
		System.out.println( "=============\n TEST 04 \n-------------"); 
		try
		{
			final int MORETHANENOUGH = 100;
			Float[] v = new Float[MORETHANENOUGH];
			String query = "XCOR:LI03:120//IMMS";
            
			DaReference r = new DaReference(query, da);
			r.setType(DaValue.Type.FLOATA); // Set Aida type to Array of FLOAT 
			v = (Float[])da.geta(r);
            
			System.out.println( query );

			// Note v.length is set to length returned by the data provider.
			for (int i = 0; i < v.length; i++)
			{
				System.out.println( v[i] );
			}
		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}
		return;
	}

	/**
	 * Acquire array of DOUBLE type SLC PMUS, giving type, through DaServer, with reference.
	 */ 
	private void simpleTest05()
	{
		System.out.println( "==============\n TEST 05 \n------------"); 
		try
		{
			final int MORETHANENOUGH = 100;
			Double[] v = new Double[MORETHANENOUGH];
			String query = "XCOR:LI03:120//IMMS";
            
			DaReference r = new DaReference(query, da);
			r.setType(DaValue.Type.DOUBLEA); // Set Aida type to Array of DOUBLE 
			v = (Double[])da.geta(r);
            
			System.out.println( query );

			// Note v.length is set to length returned by the data provider.
			for (int i = 0; i < v.length; i++)
			{
				System.out.println( v[i] );
			}
		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}
		return;
	}

	/**
	 * Acquire array of SHORT type SLC PMUS, giving type, through DaServer, with reference.
	 */ 
	private void simpleTest06()
	{
		System.out.println( "===============\n TEST 06 \n--------------"); 
		try
		{
			final int MORETHANENOUGH = 100;
			Short[] v = new Short[MORETHANENOUGH];
			String query = "XCOR:LI03:120//NSCY";
            
			DaReference r = new DaReference(query, da);
			r.setType(DaValue.Type.SHORTA); // Set Aida type to Array of SHORT
			v = (Short[])da.geta(r);
            
			System.out.println( query );

			// Note v.length is set to length returned by the data provider.
			for (int i = 0; i < v.length; i++)
			{
				System.out.println( v[i] );
			}
		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}
		return;
	}

	/**
	 * Acquire array of LONG type SLC PMUS, giving type, through DaServer, with reference.
	 */ 
	private void simpleTest07()
	{
		System.out.println( "================\n TEST 07 \n---------------"); 
		try
		{
			final int MORETHANENOUGH = 100;
			Long[] v = new Long[MORETHANENOUGH];
			String query = "XCOR:LI03:120//RAMP";
            
			DaReference r = new DaReference(query, da);
			r.setType(DaValue.Type.LONGA); // Set Aida type to Array of LONG 
			v = (Long[])da.geta(r);
            
			System.out.println( query );

			// Note v.length is set to length returned by the data provider.
			for (int i = 0; i < v.length; i++)
			{
				System.out.println( v[i] );
			}
		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}
		return;
	}

	/**
	 * Acquire array of LONG type SLC PMUS, giving type, through DaServer, with reference.
	 */ 
	private void simpleTest08()
	{
		System.out.println( "==============\n TEST 08 \n--------------"); 
		try
		{
			final int MORETHANENOUGH = 100;
			Boolean[] v = new Boolean[MORETHANENOUGH];
			String query = "XCOR:LI03:120//RAMP";
            
			DaReference r = new DaReference(query, da);
			r.setType(DaValue.Type.BOOLEANA); // Set Aida type to Array of BOOLEAN 
			v = (Boolean[])da.geta(r);
            
			System.out.println( query );

			// Note v.length is set to length returned by the data provider.
			for (int i = 0; i < v.length; i++)
			{
				System.out.println( v[i] );
			}
		}
		catch (Exception e)
		{
			System.err.println( e.getMessage() );
		}
		return;
	}

	/**
	 * Acquire FLOAT type SLC Database PV using DaReference.getDaValue().
	 */
	private void simpleTest09()
	{
		System.out.println( "===============\n TEST 09 \n---------------");
		try
		{
			System.out.println("Getting data for XCOR:LI03:120//LEFF");
			String query = "XCOR:LI03:120//LEFF";
			DaReference r = new DaReference(query, da);

			r.setType(DaValue.Type.FLOAT);

			r.compile(da);
			DaValue v = r.getDaValue();

			System.out.println(v.toString());

		}
		catch (Exception e)
		{
			System.err.println("catch exception message = " +  e.toString() );
		}
		return;
	}

	/**
	 * Loping acquire FLOAT type SLC Database PV using DaReference.getDaValue().
	 * Can be used to test persistency of SlcDp.
	 */
	private void simpleTest10()
	{
		System.out.println( "===============\n TEST 10 \n---------------");
		int ACQS = 10;     // Make 10 iterations
		int PAUSE = 10000; // .. pausing 10 seconds between each iteration
		try
		{
			String query = "XCOR:LI03:120//LEFF";
			System.out.println("Looping acqusition of " + query);
			System.out.println("Iterations = " + ACQS +
					   " pausing " + PAUSE/1000 + " secs between each.");

			DaReference r = new DaReference(query, da);
			r.setType(DaValue.Type.FLOAT);
			r.compile(da);
			for (int i=0; i<ACQS; i++)
			{
				try 
				{
					DaValue v = r.getDaValue();
					System.out.println(v.toString());
				}
				catch ( Exception e )
				{
					System.err.println("Caught " + e.toString() );
				}
				Thread.sleep(PAUSE);
			}
		}
		catch (Exception e)
		{
			System.err.println("Caught " +  e.toString() );
		}
		return;
	}

        /**
         * ASTS channel name test
         */
        private void simpleTest11()
        {
            System.out.println( "=============================\n TEST 11 \n-----------------------------");
            try
            {
                Float f = (Float)da.get("ASTS:LI00:ISOPLAN2//DATA", DaValue.Type.FLOAT);
                System.out.println( "Float ASTS:LI00:ISOPLAN2//DATA = " + f);

                Float f2 = (Float)da.get("ASTS:PR02:VP3012//DATA", DaValue.Type.FLOAT);
                System.out.println( "Float ASTS:PR02:VP3012//DATA = " + f2);

                Short s = (Short)da.get("ASTS:PR02:VP3012//STAT", DaValue.Type.SHORT);
                System.out.println( "Short ASTS:PR02:VP3012//STAT = " + s);

                Float f3 = (Float)da.get("ASTS:PR02:T2092BLW//LIM1", DaValue.Type.FLOAT);
                System.out.println( "Float ASTS:PR02:T2092BLW//LIM1 = " + f3);

                Float f4 = (Float)da.get("ASTS:PR02:T2092BLW//LIM2", DaValue.Type.FLOAT);
                System.out.println( "Float ASTS:PR02:T2092BLW//LIM2 = " + f4);

                Float[] fa =(Float[])da.geta("ASTS:PR02:T2092BLW//LIMS",DaValue.Type.FLOATA);
                System.out.println("ASTS:PR02:T2092BLW//LIMS float array = ");
                for (int i = 0; i < fa.length; i++)
                {
                    System.out.println(fa[i]);
                }

                Float[] fa2 =(Float[])da.geta("ASTS:PR02:T2092QUA//SCAL",DaValue.Type.FLOATA);
                System.out.println("ASTS:PR02:T2092QUA//SCAL float array = ");
                for (int i = 0; i < fa2.length; i++)
                {
                    System.out.println(fa2[i]);
                }

                Float av5 = (Float)da.get("ASTS:PR02:T2092QUA//RAW", DaValue.Type.FLOAT);
                System.out.println( "Float ASTS:PR02:T2092BLW//RAW = " + av5);

                String st = (String)da.get("ASTS:PR02:T2092QUA//STAT", 
                    DaValue.Type.STRING);
                System.out.println( "String STAT = " + st);

            }
            catch (Exception e)
            {
                System.err.println( e.getMessage() );
            }
            return;
        }

        /**
         * ASTS channel name with one of the pseudo-secondaries STAT, CTRL, or VSTA and
         * using DaValue to obtain separate text, color, and flag strings.
         */
        private void simpleTest12()
        {
            System.out.println( "=============================\n TEST 12 \n-----------------------------");

            try
            {
                String query = "ASTS:PR02:T2092QUA//STAT";

                DaValue v = da.getDaValue(query);

                System.out.println(v.toString()); 

                String text = v.elementAt(0).toString();
                String color = v.elementAt(1).toString();
                String flag = v.elementAt(2).toString();

                System.out.println("text  = " + text);
                System.out.println("color = " + color);
                System.out.println("flag  = " + flag);

            }
            catch (Exception e)
            {
                System.err.println( e.getMessage() );
            }
            return;
        }

        /**
         * Set test.
         */
        private void simpleTest13()
        {
                System.out.println( "===========\n TEST 13 \n-------------");
                System.out.println( "This test should only be run in the DEV environment");

                try
                {
                    String query = "XCOR:LI31:41//BCON";

                    Float floatValue = new Float(5.0f);

                    DaValue inData = new DaValue(floatValue);

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

			case 12:
				simpleTest12();
				break;

			case 13:
				simpleTest13();
				break;
                    
			default:
				System.out.println( "No such Slc test number parameter " + 
						    testNumber + "=\"" + testNumber + "\"" );
				System.out.println( "usage: java daSlcTest <test_number> ..." );
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
