package test.java;

import edu.stanford.slac.err.*;
import edu.stanford.slac.aida.lib.da.*;             // AIDA Api
import edu.stanford.slac.aida.lib.util.common.*;    // Aida utilities

/**
 * Defines tests which exercise error condition handling in Aida.
 * 
 * Usage: aidatest daErrTest {1,2,3...}1+
 *
 * @author 05-Sep-2004, Greg White.
 * @version 15-Sep-2004, Greg White (greg): Convert use of Err for singleton re-write.
 */
public class ErrTests
{
	private DaObject da;
	static Err err_;                        // Error issuance singleton.

	public ErrTests (String [] args)
	{
		try 
		{
                        // Define the DaObject against which queries are made.
			da = new DaObject(args);
			err_ = Err.getInstance(); // Get Err issuance reference.
		}
		catch ( Exception e ) 
		{
			System.err.println( e.getMessage() );
			e.printStackTrace();
		}
	}

	public void run(int testNumber )
	{
		try 
		{
			switch ( testNumber )
			{
			case 1:
				Test01();
				break;

			case 2:
				Test02();
				break;

			case 3:
				Test03();
				break;

			case 4:
				Test04();
				break;
								
			case 5:
				Test05();
				break;

			default:
				System.out.println( "No such test number parameter" + 
						    testNumber + 
						    "=\"" + testNumber + "\"" );
				System.out.println( "Usage: java daErrTest " +
						    "{<test_number>}" );
			}
		}
		catch (Exception e)
		{
			System.out.println( "Uncaught exception: Executing test " + 
					    testNumber);
			System.err.println( e.getMessage() );
		}
		return;
	}

	/**
	 * Test behaviour when Entity not in Directory Service; get via
	 * DaServer, by typed get. Should cause UndefinedNameException to
	 * be logged and thrown by Name Server, causing
	 * UnableToGetDataException to be logged and thrown by DaServer,
	 * which in turn should be caught here.
	 */
	public void Test01()
	{
		try
		{
			System.out.println( "TEST 01: ");
			System.out.println( "=============================");
			System.out.println(
				"da.get(\"TESTUnknownEntity//VAL\", DaValue.Type.DOUBLE);");
			Double v = (Double)da.get("TESTUnknownEntity//VAL", 
						  DaValue.Type.DOUBLE);		

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println( e.toString() );
		}
		return;
	}

	/**
	 * Test behaviour when Entity not in Directory Service; get via
	 * DaServer, by getDaValue with parameters. Should cause
	 * UndefinedNameException to be logged and thrown by Name Server,
	 * causing UnableToGetDataException to be logged (including the
	 * parameters given here) and thrown by DaServer, which in turn should
	 * be caught here.
	 */
	public void Test02()
	{
		try
		{
			System.out.println( "TEST 02:");
			System.out.println( "=============================");

			System.out.println( 
				"da.setParam(\"STARTTIME\", \"2001-09-06 12:04\")" );
			da.setParam("STARTTIME", "2001-09-06 12:04");

			System.out.println("da.setParam(\"ENDTIME\", \"2001-09-06 12:11\")" );
			da.setParam("ENDTIME", "2001-09-06 12:11");

			System.out.println( "da.getDaValue(\"TESTUnknownEntity//VAL\")" );
			DaValue v = da.getDaValue("TESTUnknownEntity//VAL");

			System.out.println( "\t=" + v.toString() );

			System.out.println( "da.reset()" );
			da.reset();

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println( e.toString() );
		}
		return;
	}

	/**
	 * Test behaviour when Entity not in Directory Service; by
	 * reference.  Should cause UndefinedNameException to be logged
	 * and thrown by Name Server in DaReference constructor, and
	 * proceed no further.
	 */
	public void Test03()
	{
		try
		{
			System.out.println( "TEST 03:");
			System.out.println( "=============================");

			System.out.println( 
				"DaReference r = DaReference(\"TESTNotInDirService//VAL\",da)"  );
			DaReference r = new DaReference("TESTNotInDirService//VAL", da);

			System.out.println( "r.setType(DaValue.Type.FLOAT)" );
			r.setType(DaValue.Type.FLOAT);

			System.out.println( "da.get(r)" );
			Float v = (Float)da.get(r);				
			System.out.println( "\t=" + v);

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println( e.toString() );
		}
		return;
	}

	/**
	 * Test behaviour when DaReference not compiled before get.  
	 * Should recieve IllegalArgumentException (after it was logged
	 * by _DaReference::getDaValue - which recall is executed in 
	 * this process.
	 */
	public void Test04()
	{
		try
		{
			System.out.println( "TEST 04:");
			System.out.println( "=============================");

			System.out.println( "DaReference r = new DaReference(\"TEST//VAL\")");
			DaReference r = new DaReference("TEST//VAL");				

			System.out.println("r.setParam(\"STARTTIME\", \"2001-09-06 12:04\")");
			r.setParam("STARTTIME", "2001-09-06 12:04");

			System.out.println( "r.setParam(\"ENDTIME\", \"2001-09-06 12:10\")" );
			r.setParam("ENDTIME", "2001-09-06 12:10");

			System.out.println( "Ommitted r.compile(da)!" );
			// r.compile(da);

			System.out.println( "r.getDaValue()" );
			DaValue v = r.getDaValue();					

			System.out.println( "\t=" + v.toString() );

			System.out.println( "=============================");
		}
		catch (Exception e)
		{
			System.out.println( e.toString() );
		}
		return;
	}
		
	/**
	 * Test behaviour when names of incorrect syntax are acquired.
	 */
	public void Test05()
	{
		try
		{
			System.out.println( "TEST 05: Test syntax error detection");
			System.out.println( "=============================");

			System.out.println( "DaReference r = new DaReference(\"TEST\")");
			DaReference r = new DaReference("TEST");
		}
		catch ( Exception e)
		{
			System.out.println( e.getMessage());
		}
		        
		try
		{
			System.out.println( "DaReference r = new DaReference(\"TEST/\")");
			DaReference r = new DaReference("TEST/");
		}
		catch ( Exception e)
		{
			System.out.println( e.getMessage());
		}
		        
		try
		{
			System.out.println( "DaReference r = new DaReference(\"TEST//\")");
			DaReference r = new DaReference("TEST//");
		}
		catch ( Exception e)
		{
			System.out.println( e.getMessage());
		}

		try
		{
			System.out.println( "DaReference r = new DaReference(\"//TEST");
			DaReference r = new DaReference("//TEST");
		}
		catch ( Exception e)
		{
			System.out.println( e.getMessage());
		}
	}
}
