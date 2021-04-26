package test.java;

import	org.omg.CORBA.*;
import edu.stanford.slac.err.*;
import edu.stanford.slac.aida.lib.util.common.*;
/**
 * Unit tests DaValue, the Aida class which encapsulates aggregate data
 * data. DaTest uses DaValue in teh same way an Aida Data Provider might
 * use it, in this case like a History data provider, it puts fake history
 * into a DaValue, and then retrieves from the DaValue. It so exercises 
 * DaValue's handling of a structure of arrays.
 * @author greg
 *
 */
public class DaTest {
	
    static Err err_;

	/**
     * Static Test data returned by dpTest
     */ 
    private static String timeList [] =
    {
	"2001-09-06 12:02:22",
	"2001-09-06 12:03:42",
	"2001-09-06 12:04:23",
	"2001-09-06 12:05:33",
	"2001-09-06 12:06:54",
	"2001-09-06 12:06:22",
	"2001-09-06 12:07:56",
	"2001-09-06 12:09:22",
	"2001-09-06 12:10:23",
	"2001-09-06 12:11:46",
	"2001-09-06 12:14:23"
    };
    private static double valueList [] =
    {
	1232.3434,
	1233.0121,
	1212.3234,
	1255.7565,
	1455.7454,
	1343.0923,
	1232.4656,
	1233.4544,
	1234.6565,
	1223.3434,
	1201.2323
    };

    private String startTime, endTime;

    public static void main(String[] args) 
    {    
		err_ = Err.getInstance("daErrTest");

        DaTest test1 = new DaTest("2001-09-06 12:04:00", "2001-09-06 12:07:00");
        Any a1 = test1.runner();
        DaValue undo = new DaValue(a1);
        System.out.println(undo.toString());
    
        DaTest test2 = new DaTest("2001-09-06 12:04:00", "2001-09-06 12:05:00");
        a1 = test2.runner();
        DaValue undo2 = new DaValue(a1);
        System.out.println(undo2.toString());
    }


    public DaTest( String _startTime, String _endTime )
    {
    		startTime = _startTime;
    		endTime = _endTime;
    }
    
    public Any runner()
    {
		
//		This is the return object, this could also be a CORBA Any
		DaValue ret = new DaValue();
		ret.type = DaValue.Type.STRUCT;

	    // Otherwise return data that matches parameters
	    DaValue times = new DaValue();
	    DaValue values= new DaValue();
	    
	    times.type = DaValue.Type.STRINGA;
	    values.type = DaValue.Type.DOUBLEA;
	    
	    int i;
	    for ( i = 0 ; (i < timeList.length) && (i < valueList.length) ; i++ )
	    {
	    	if ( (( startTime == null) || (startTime.compareTo(timeList[i]) <= 0)) &&
	    			(( endTime == null) || (endTime.compareTo(timeList[i]) >= 0 )) )
	    	{
	    		times.addElement(timeList[i]);
	    		values.addElement(new Double(valueList[i]));
	    	}
	    }
	    ret.addElement(times);
	    ret.addElement(values);

	    return ret.toAny();   		
    }
}
