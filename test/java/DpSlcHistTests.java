package test.java;


// The following libraries must me imported for all AIDA clients
// da defines objects used in AIDA clients
// util defines common objects available thoughout the AIDA system

import java.util.*;

import org.omg.CORBA.*;

import edu.stanford.slac.aida.lib.da.*;
import edu.stanford.slac.aida.lib.util.common.*;

// The DpSlcHist histData for helper classes are now in dpSlcHist
// instead of dpChads.  However, they should be the same as dpChads.
import edu.stanford.slac.aida.dp.dpSlcHist.*;

/**
 * The SLC Hist AIDA Data Provider Test Suite. Tests Aida access to SLC history data.
 *
 * @see daDpSlcHistTest interactive runner
 * @author  10-Aug-2004 Bob Hall. Based on DpSlcTests.java
 * @version 25-Jul-2006 Bob Hall. Added display of new unixtime field.
 * @version 01-Nov-2004 Bob Hall. Added new tests. 
 */
public class DpSlcHistTests
{    
    private DaObject da;
    
    public DpSlcHistTests(String[] args)
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
     * Acquire FLOAT type SLC History PV, giving type, through DaServer, with reference.
     */ 
    private void simpleTest01()
    {
        System.out.println( "=============================\n TEST 01 \n-----------------------------");
        try
        {
            System.out.println("Getting data for XCOR:LI03:120//BACT.HIST");
            String query = "XCOR:LI03:120//BACT.HIST";
            DaReference r = new DaReference(query, da);

            r.setParam("STARTTIME", "07/30/2004 00:00:00");
            r.setParam("ENDTIME",   "07/31/2004 00:00:00");

            System.out.println("making call to getAny");

            Any anyData = r.getAny();

            System.out.println("returned from getAny");

            histData f = histDataHelper.extract(anyData);
            System.out.println( "# entries returned = " +
                                             f.d.length);

            System.out.println(query);
            if (f.d.length > 0) {
                for ( int ii = 0; ii < f.d.length ; ii++ ) {
                    System.out.println(
                                       "\t" + f.s[ii] + "\t" + f.d[ii] +
                                       "\t" + f.unixtime[ii]);
                }
            }

        }
        catch (Exception e)
        {
            System.err.println("catch exception message = " +  e.toString() );
        }
        return;
    }

    /**
     * Acquire FLOAT type SLC History PV, giving type, through DaServer, with reference.
     */ 
    private void simpleTest02()
    {
        System.out.println( "=============================\n TEST 02 \n-----------------------------");
        try
        {
            System.out.println("Getting data for ASTS:LI01:7:DATA//QW244.HIST");
            String query = "ASTS:LI01:7:DATA//QW244.HIST";
            DaReference r = new DaReference(query, da);

            r.setParam("STARTTIME", "09/16/2004 00:00:00");
            r.setParam("ENDTIME",   "09/17/2004 00:00:00");

            System.out.println("making call to getAny");

            Any anyData = r.getAny();

            System.out.println("returned from getAny");

            histData f = histDataHelper.extract(anyData);
            System.out.println( "# entries returned = " +
                                             f.d.length);

            System.out.println(query);
            if (f.d.length > 0) {
                for ( int ii = 0; ii < f.d.length ; ii++ ) {
                    System.out.println(
                                       "\t" + f.s[ii] + "\t" + f.d[ii] +
                                       "\t" + f.unixtime[ii]);
                }
            }

        }
        catch (Exception e)
        {
            System.err.println("catch exception message = " +  e.toString() );
        }
        return;
    }

    /**
     * Acquire FLOAT type SLC History PV, giving type, through DaServer, with reference.
     */ 
    private void simpleTest03()
    {
        System.out.println( "=============================\n TEST 03 \n-----------------------------");
        try
        {
            System.out.println("Getting data for XCOR:LI03:120//BACT.HIST");
            String query = "XCOR:LI03:120//BACT.HIST";
            DaReference r = new DaReference(query, da);

            r.setParam("STARTTIME", "07/30/2004 00:00:00");
            r.setParam("ENDTIME",   "07/31/2004 00:00:00");
            r.setParam("DATEFORMAT", "MMDDYYYY"); 

            System.out.println("making call to getAny");

            Any anyData = r.getAny();

            System.out.println("returned from getAny");

            histData f = histDataHelper.extract(anyData);
            System.out.println( "# entries returned = " +
                                             f.d.length);

            System.out.println(query);
            if (f.d.length > 0) {
                for ( int ii = 0; ii < f.d.length ; ii++ ) {
                    System.out.println(
                                       "\t" + f.s[ii] + "\t" + f.d[ii] +
                                       "\t" + f.unixtime[ii]);
                }
            }

        }
        catch (Exception e)
        {
            System.err.println("catch exception message = " +  e.toString() );
        }
        return;
    }

    /**
     * Acquire FLOAT type SLC History PV, giving type, through DaServer, with reference.
     */ 
    private void simpleTest04()
    {
        System.out.println( "=============================\n TEST 04 \n-----------------------------");

        // Prompt for the PV, the start time, and the end time.
        // Allow the user to enter carriage return for default values.

        System.out.print(
            "Enter PV name [enter C/R for XCOR:LI03:120//BACT.HIST ]:");

        try
        {
            java.io.BufferedReader br;
            br = new java.io.BufferedReader(
                     new java.io.InputStreamReader(System.in));
            String query = br.readLine();
            if (query.length() == 0) query = "XCOR:LI03:120//BACT.HIST";
 
            System.out.println("Getting data for " + query);
            DaReference r = new DaReference(query, da);

            System.out.print(
                "Enter START time [enter C/R to use 07/30/2004 00:00:00 ]: ");
            String query2 = br.readLine();
            if (query2.length() == 0) query2 = "07/30/2004 00:00:00";
            r.setParam("STARTTIME", query2);

            System.out.print(
                "Enter   END time [enter C/R to use 07/31/2004 00:00:00 ]: ");
            String query3 = br.readLine();
            if (query3.length() == 0) query3 = "07/31/2004 00:00:00";
            r.setParam("ENDTIME", query3);

            System.out.println("making call to getAny");

            Any anyData = r.getAny();

            System.out.println("returned from getAny");

            histData f = histDataHelper.extract(anyData);
            System.out.println( "# entries returned = " +
                                             f.d.length);

            System.out.println(query);
            if (f.d.length > 0) {
                for ( int ii = 0; ii < f.d.length ; ii++ ) {
                    System.out.println(
                                       "\t" + f.s[ii] + "\t" + f.d[ii] +
                                       "\t" + f.unixtime[ii]);
                }
            }

        }
        catch (Exception e)
        {
            System.err.println("catch exception message = " +  e.toString() );
        }
        return;
    }

    /**
     * Acquire DOUBLE type SLC History PV, giving type, through DaServer, with reference.
     */ 
    private void simpleTest05()
    {
        System.out.println( "=============================\n TEST 05 \n-----------------------------");
        try
        {
            final int MAX_DOUBLE_LEN = 300;
            Double[] v = new Double[MAX_DOUBLE_LEN];

            System.out.println("Getting data for XCOR:LI03:120//BACT.HIST");
            String query = "XCOR:LI03:120//BACT.HIST";
            DaReference r = new DaReference(query, da);

            r.setParam("STARTTIME", "07/30/2004 00:00:00");
            r.setParam("ENDTIME",   "07/31/2004 00:00:00");

            r.setType(DaValue.Type.DOUBLEA);

            System.out.println("making call to get double");

            v = (Double[])da.geta(r);

            System.out.println("returned from get double");

            System.out.println(query);

            // Note that v.length is set to length returned by the data
            // provider.
            for (int i = 0; i < v.length; i++)
            {
                System.out.println(v[i]);
            }

        }
        catch (Exception e)
        {
            System.err.println("catch exception message = " +  e.toString() );
        }
        return;
    }

    /**
     * Acquire FLOAT type SLC History PV, giving type, through DaServer, with reference.
     */ 
    private void simpleTest06()
    {
        System.out.println( "=============================\n TEST 06 \n-----------------------------");
        try
        {
            final int MAX_FLOAT_LEN = 300;
            Float[] v = new Float[MAX_FLOAT_LEN];

            System.out.println("Getting data for XCOR:LI03:120//BACT.HIST");
            String query = "XCOR:LI03:120//BACT.HIST";
            DaReference r = new DaReference(query, da);

            r.setParam("STARTTIME", "07/30/2004 00:00:00");
            r.setParam("ENDTIME",   "07/31/2004 00:00:00");

            r.setType(DaValue.Type.FLOATA);

            System.out.println("making call to get float");

            v = (Float[])da.geta(r);

            System.out.println("returned from get float");

            System.out.println(query);

            // Note that v.length is set to length returned by the data
            // provider.
            for (int i = 0; i < v.length; i++)
            {
                System.out.println(v[i]);
            }

        }
        catch (Exception e)
        {
            System.err.println("catch exception message = " +  e.toString() );
        }
        return;
    }

    /**
     * Acquire FLOAT type SLC History PV using DaValue.getAny().
     */ 
    private void simpleTest07()
    {
        System.out.println( "=============================\n TEST 07 \n-----------------------------");
        try
        {
            System.out.println("Getting data for XCOR:LI03:120//BACT.HIST");
            String query = "XCOR:LI03:120//BACT.HIST";

            da.setParam("STARTTIME", "07/30/2004 00:00:00");
            da.setParam("ENDTIME",   "07/31/2004 00:00:00");

            System.out.println("making call to getAny");

            Any anyData = da.getAny(query);

            System.out.println("returned from getAny");

            histData f = histDataHelper.extract(anyData);
            System.out.println( "# entries returned = " +
                                             f.d.length);

            System.out.println(query);
            if (f.d.length > 0) {
                for ( int ii = 0; ii < f.d.length ; ii++ ) {
                    System.out.println(
                                       "\t" + f.s[ii] + "\t" + f.d[ii] +
                                       "\t" + f.unixtime[ii]);
                }
            }

        }
        catch (Exception e)
        {
            System.err.println("catch exception message = " +  e.toString() );
        }
        return;
    }

    /**
     * Acquire FLOAT type SLC History PV using DaReference.getDaValue().
     */ 
    private void simpleTest08()
    {
        System.out.println( "=============================\n TEST 08 \n-----------------------------");
        try
        {
            System.out.println("Getting data for XCOR:LI03:120//BACT.HIST");
            String query = "XCOR:LI03:120//BACT.HIST";
            DaReference r = new DaReference(query, da);

            r.setParam("STARTTIME", "07/30/2004 00:00:00");
            r.setParam("ENDTIME",   "07/31/2004 00:00:00");

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
     * Acquire FLOAT type SLC History PV using DaValue.getDaValue().
     */ 
    private void simpleTest09()
    {
        System.out.println( "=============================\n TEST 09 \n-----------------------------");
        try
        {
            System.out.println("Getting data for XCOR:LI03:120//BACT.HIST");
            String query = "XCOR:LI03:120//BACT.HIST";

            da.setParam("STARTTIME", "07/30/2004 00:00:00");
            da.setParam("ENDTIME",   "07/31/2004 00:00:00");

            DaValue v = da.getDaValue(query);

            System.out.println(v.toString()); 
        }
        catch (Exception e)
        {
            System.err.println("catch exception message = " +  e.toString() );
        }
        return;
    }

    /**
     * Acquire FLOAT type SLC BPM Sampler History using DaValue.getDaValue().
     */ 
    private void simpleTest10()
    {
        System.out.println( "================\n TEST 10 \n-----------------------");
        try
        {
	    da.reset();
            da.setParam("STARTTIME", "04/15/2004 00:00:00");
            da.setParam("ENDTIME",   "04/17/2004 00:00:00");

            String query = "BPMS:PR02:8032//X.HIST";
            System.out.println("Getting data for "+query);
            DaValue v = da.getDaValue(query);

            System.out.println(v.toString()); 
        }
        catch (Exception e)
        {
            System.err.println("catch exception message = " +  e.toString() );
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
                    System.out.println( "No such SlcHist test number parameter " + testNumber + "=\"" + testNumber + "\"" );
                    System.out.println( "usage: java daSlcHistTest <test_number> ..." );
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
