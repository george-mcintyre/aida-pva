package test.java;



// The following libraries must me imported for all AIDA clients
// da defines objects used in AIDA clients
// util defines common objects available thoughout the AIDA system
import edu.stanford.slac.aida.lib.da.*;
import edu.stanford.slac.aida.lib.util.common.*;


/**
 * The Channel Access AIDA Data Provider Test Suite. Tests Da access to EPICS data.
 *
 * @author  11-Sep-2002 Greg White. Based on daAIDATest.java
 * @version 20-Nov-2002, Ron MacKenzie.  Change except from aida to pkg except.
 */
public class DpCaTests
{
    private DaObject da;
    
    public DpCaTests(String[] args)
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
     * Simple acquisition of DOUBLE type EPICS PV, giving type, through DaServer.
     */ 
    private void simpleTest01()
    {
        System.out.println( "============== TEST 01 ---------------");
        try
        {
            Float v = (Float)da.get("PB60:LUMVAL//VAL", DaValue.Type.FLOAT);
            System.out.println( "\t=" + v);
        }
        catch (Exception e)
        {
            System.err.println( e.getMessage() );
        }
        return;
    }
    
    /**
     * Acquire DOUBLE type EPICS PV, giving type, through
     * DaServer, with reference.
     */ 
    private void simpleTest02()
    {
        System.out.println( "============== TEST 02 ---------------");    
        try
        {
            String query = "HR81:STN:VOLT//VAL";
            DaReference r = new DaReference(query, da);
            r.setType(DaValue.Type.DOUBLE);
            Double v = (Double)da.get(r);
            
            System.out.println( query + "\t=" + v);
        }
        catch (Exception e)
        {
            System.err.println( e.getMessage() );
        }
        return;
    }

    /**
     * Acquire FLOAT type EPICS PV, giving type, through
     * DaServer, with reference.
     */ 
    private void simpleTest03()
    {
        System.out.println( "============== TEST 03 ---------------");    
        try
        {
            String query = "DF00:CAV_B:TUNER_POS//VAL";
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
     * Acquire LONG INT type EPICS PV, giving type, through
     * DaServer, with reference.
     */ 
    private void simpleTest04()
    {
        System.out.println( "============== TEST 04 ----------------"); 
        try
        {
            String query = "DF00:CAV_B:GAPV//VAL";
            
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
     * Acquire array of FLOAT type EPICS PV, giving type, through DaServer, with reference.
     */ 
    private void simpleTest05()
    {
        System.out.println( "================= TEST 05 -------------------"); 
        try
        {
            System.err.println("Expected to fail until");
            System.err.println("SLC db analogs and digitals are known to AIDA");
            System.err.println("name server");
            final int MORETHANENOUGH = 10;
            Float[] v = new Float[MORETHANENOUGH];
            String query = "LI28:CSTR:1:CRV3//VAL";
            
            DaReference r = new DaReference(query, da);
            r.setType(DaValue.Type.FLOATA); // Set Aida type to Array of FLOAT 
            v = (Float[])da.geta(r);
            
            System.out.println( query );
            for (int i = 0; i<v.length; i++) System.out.println( v[i] ); 
        }
        catch (Exception e)
        {
            System.err.println( e.getMessage() );
        }
        return;
    }
    
    /**
     * Acquire the DBR_Time structure of an EPICS PV, through Da Service
     */ 
    private void simpleTest06()
    {
        System.out.println( "============== TEST 06 ------------------"); 
        try
        {
            System.err.println("Expected to fail until DBR_ support is added"); 
            String query = "LI28:CSTR:1:CRV3//DBR_TIME";
            
            DaValue v = da.getDaValue(query);
            
            System.out.println( query + " = " + v.toString() );
 
        }
        catch (Exception e)
        {
            System.err.println( e.getMessage() );
        }
        return;
    }


    private void simpleTest11()
    {
        System.out.println( "============== TEST 11 ---------------");
        try
        {
            System.out.println (da.getDaValue ("PB60:LUMVAL//VAL"));
        }
        catch (Exception e)
        {
            System.err.println( e.getMessage() );
        }
        return;
    }
    
    private void simpleTest12()
    {
        System.out.println( "============== TEST 12 ---------------");    
        try
        {
            String query = "HR81:STN:VOLT//VAL";
            DaReference r = new DaReference(query, da);
            r.setType(DaValue.Type.DOUBLE);
            System.out.println (da.getDaValue (r));
        }
        catch (Exception e)
        {
            System.err.println( e.getMessage() );
        }
        return;
    }

    private void simpleTest13()
    {
        System.out.println( "============== TEST 13 ---------------");    
        try
        {
            String query = "DF00:CAV_B:TUNER_POS//VAL";
            DaReference r = new DaReference(query, da);
            r.setType(DaValue.Type.FLOAT);
            System.out.println (da.getDaValue (r));
        }
        catch (Exception e)
        {
            System.err.println( e.getMessage() );
        }
        return;
    }
    
    private void simpleTest14()
    {
        System.out.println( "============== TEST 14 ----------------"); 
        try
        {
            String query = "DF00:CAV_B:GAPV//VAL";
            
            DaReference r = new DaReference(query, da);
            r.setType(DaValue.Type.LONG);
            System.out.println (da.getDaValue (r));
        }
        catch (Exception e)
        {
            System.err.println( e.getMessage() );
        }
        return;
    }
    
    private void simpleTest15()
    {
        System.out.println( "================= TEST 15 -------------------"); 
        try
        {
            String query = "HB60:BCM:BKT:PH2//VAL";
            
            DaReference r = new DaReference(query, da);
            r.setType(DaValue.Type.FLOATA); // Set Aida type to Array of FLOAT
            System.out.println (da.getDaValue (r));
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

            case 11:
                simpleTest11();
                break;

            case 12:
                simpleTest12();
                break;

            case 13:
                simpleTest13();
                break;

            case 14:
                simpleTest14();
                break;

            case 15:
                simpleTest15();
                break;

            default:
                System.out.println( "No such Ca test number parameter " + testNumber + "=\"" + testNumber + "\"" );
                System.out.println( "usage: java daCaTest <test_number> ..." );
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

