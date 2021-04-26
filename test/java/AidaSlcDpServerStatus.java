package test.java;

import java.util.*;

// The following libraries must me imported for all AIDA clients
// da defines objects used in AIDA clients
// util defines common objects available thoughout the AIDA system
import edu.stanford.slac.aida.lib.da.*;
import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.except.*;
import edu.stanford.slac.err.*;              // Error handling API (singleton).

/**
 * This AIDA data provider server status program determines whether any Aida 
 * SLC (VMS) Aida data provider server is not functioning correctly.
 *
 * An error code is returned with each bit (a power of 2) indicating whether
 * one of the Aida SLC data providers is functioning correctly (value "0") or
 * not (value "1").  If an exception is not thrown as the result of making
 * a request to an Aida data provider, the data provider is presumed to be
 * functioning correctly.
 *
 * @author  22-Jan-2008 Bob Hall
 *
 * @version 16-May-2008, Bob Hall (rdh) Changed to no longer check the Master Oscillator, that was only used by PEPII.
 * @version 18-Apr-2008, Bob Hall (rdh) Changed Aida BPM test to use a LCLS dgrp rather than a PEPII one.
 * @version 29-Apr-2010, Bob Hall (rdh) Changed Aida BPM test to use a NLCTA dgrp rather than a LCLS one since the LCLS request caused warning messages.
 * 
 */
public class AidaSlcDpServerStatus 
{
    static Err err;                             // Err issuance singleton.

    private DaObject da;

    public AidaSlcDpServerStatus()
    {
        try
        {
            da = new DaObject();
        }
        catch (Exception e)
        {
            System.err.println( e.getMessage() );
            e.printStackTrace();
        }
    }

    //
    // Determine whether the Aida SLC Database data provider is
    // functioning correctly.
    //
    public int getSlcDbStatus(String query)
    {
        int status;

        status = 1;

        try
        {
            da.getDaValue(query);
            status = 0;
        }
        catch (Exception e)
        {
            //System.err.println( e.getMessage() + " caused by " + e );
        }

        return status;
    }

    //
    // Determine whether the Aida SLC History data provider is
    // functioning correctly.
    //
    public int getSlcHistStatus(String query, String startTime, String endTime)
    {
        int status;

        status = 1;

        try
        {
            da.reset();
            da.setParam("STARTTIME", startTime);
            da.setParam("ENDTIME",   endTime);

            da.getDaValue(query);
            status = 0;
        }
        catch (Exception e)
        {
            //System.err.println( e.getMessage() + " caused by " + e );
        }

        return status;
    }

    //
    // Determine whether the Aida SLC BPM data provider is
    // functioning correctly.
    //
   public int getSlcBpmStatus(String query, String bpmd, String cnftype,
        String cnfnum, String n, String sortorder)
    {
        int status;

        status = 1;

        try
        {
            da.reset();
            da.setParam("BPMD", bpmd);
            da.setParam("CNFTYPE", cnftype);
            da.setParam("CNFNUM", cnfnum);
            da.setParam("N", n);
            da.setParam("SORTORDER", sortorder);

            da.getDaValue(query);
            status = 0;
        }
        catch (Exception e)
        {
            //System.err.println( e.getMessage() + " caused by " + e );
        }

        return status;
    }


    //
    // Determine whether the Aida SLC Magnet data provider is
    // functioning correctly.
    //
    public int getSlcMagnetStatus(String query)
    {
        int status;

        status = 1;

        try
        {
            da.getDaValue(query);
            status = 0;
        }
        catch (Exception e)
        {
            //System.err.println( e.getMessage() + " caused by " + e );
        }

        return status;
    }

    //
    // Determine whether the Aida SLC Utility data provider is
    // functioning correctly.
    //
    public int getSlcUtilStatus(String query, String beam)
    {
        int status;

        status = 1;

        try
        {
            da.reset();
            da.setParam("BEAM", beam);

            Short v = (Short) da.get(query, DaValue.Type.SHORT);
            status = 0;
        }
        catch (Exception e)
        {
            //System.err.println( e.getMessage() + " caused by " + e );
        }

        return status;
    }

    //
    // Determine whether the Aida SLC Master Oscillator data provider is
    // functioning correctly.
    //
    public int getSlcMoscStatus(String query)
    {
        int status;

        status = 1;

        try
        {
            da.getDaValue(query);
            status = 0;
        }
        catch (Exception e)
        {
            //System.err.println( e.getMessage() + " caused by " + e );
        }

        return status;
    }

    //
    // Determine whether the Aida SLC Klystron data provider is
    // functioning correctly.
    //
    public int getSlcKlysStatus(String query, String beam)
    {
        int status;

        status = 1;

        try
        {
            da.reset();
            da.setParam("BEAM", beam);

            Short v = (Short) da.get(query, DaValue.Type.SHORT);
            status = 0;
        }
        catch (Exception e)
        {
            //System.err.println( e.getMessage() + " caused by " + e );
        }

        return status;
    }

    //
    // Determine whether the Aida SLC Model data provider is
    // functioning correctly.
    //
    public int getSlcModelStatus(String query, String mode, String dgrp)
    {
        int status;

        status = 1;

        try
        {
            da.reset();
            da.setParam("MODE", mode);
            da.setParam("DGRP", dgrp);

            da.getDaValue(query);
            status = 0;
        }
        catch (Exception e)
        {
            //System.err.println( e.getMessage() + " caused by " + e );
        }

        return status;
    }

    public static void main(String args[])
    {
        int dpServerStatus = 0;
 
        err = Err.getInstance("AidaSlcDpServerStatus");

        AidaSlcDpServerStatus s = new AidaSlcDpServerStatus();

        // Get the status of the SLC Database server.
        int slcDbStatus = s.getSlcDbStatus("XCOR:LI03:120//LEFF");

        System.out.println("slcDbStatus = " + slcDbStatus);

        dpServerStatus += slcDbStatus;

        // Get the status of the SLC History server.
        int slcHistStatus = s.getSlcHistStatus("XCOR:LI03:120//BACT.HIST",
            "01/01/2008 00:00:00", "01/20/2008 23:59:59");

        System.out.println("slcHistStatus = " + slcHistStatus);

        dpServerStatus += slcHistStatus * 2;

        // Get the status of the SLC BPM server through the use of a NLCTA BPM request.
        int slcBpmStatus = s.getSlcBpmStatus("TAXXALL//BPMS", "41", "NONE",
            "0", "5", "1");

        System.out.println("slcBpmStatus = " + slcBpmStatus);

        dpServerStatus += slcBpmStatus * 4;

        // Get the status of the SLC Magnet server.
        int slcMagnetStatus = s.getSlcMagnetStatus("LINAC//XCOR:BDES");

        System.out.println("slcMagnetStatus = " + slcMagnetStatus);

        dpServerStatus += slcMagnetStatus * 8;

        // Get the status of the SLC Utility server.
        int slcUtilStatus = s.getSlcUtilStatus("TRIG:LI27:215//TACT", "1");

        System.out.println("slcUtilStatus = " + slcUtilStatus);

        dpServerStatus += slcUtilStatus * 16;

        // Get the status of the SLC Master Oscillator server.
        // ***COMMENTED OUT.  No longer checking the status of the SLC
        // Master Oscillator, that was only being used by PEPII.
        //int slcMoscStatus = s.getSlcMoscStatus("MASTEROSC//VAL");
        int slcMoscStatus = 0;

        System.out.println("slcMoscStatus = " + slcMoscStatus);

        dpServerStatus += slcMoscStatus * 32;

        // Get the status of the SLC Klystron server.
        int slcKlysStatus = s.getSlcKlysStatus("KLYS:LI17:31//TACT", "10");

        System.out.println("slcKlysStatus = " + slcKlysStatus);

        dpServerStatus += slcKlysStatus * 64;

        // Get the status of the SLC Model server.
        int slcModelStatus = s.getSlcModelStatus("BPMS:PR02:8032//twiss", "7", "NULL");

        System.out.println("slcModelStatus = " + slcModelStatus);

        dpServerStatus += slcModelStatus * 128;

        System.exit(dpServerStatus);
    }
}
