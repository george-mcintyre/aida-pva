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
 * UNIX data provider server is not functioning correctly.
 *
 * An error code is returned with each bit (a power of 2) indicating whether
 * one of the Aida UNIX data providers is functioning correctly (value "0") or
 * not (value "1").  If an exception is not thrown as the result of making
 * a request to an Aida data provider, the data provider is presumed to be
 * functioning correctly.
 *
 * @author  22-Jan-2008 Bob Hall
 * @version 26-Jun-2008 Bob Hall Added support for new Aida Rdb server.
 * @version 21-Jul-2008 Bob Hall Added support for new Aida Model server.
 * @version 17-Nov-2008 Bob Hall Changed date/time for LCLS Channel Archiver test.
 * @version 09-Apr-2010 Bob Hall Changed to use NLCTA PV for Non-LCLS Channel Access test.
 * @version 02-Feb-2011 Bob Hall Added support for new FACET Channel Archiver server.
 */
public class AidaUnixDpServerStatus 
{
    static Err err;                             // Err issuance singleton.

    private DaObject da;

    public AidaUnixDpServerStatus()
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
// Determine whether an Aida Channel Access data provider is
// functioning correctly.
//
    public int getChannelAccessStatus(String query)
    {
        int status;

        status = 1;

        try
        {
            da.reset();

            Double v = (Double) da.get(query, DaValue.Type.DOUBLE);
            status = 0;
        }
        catch (Exception e)
        {
            //System.err.println( e.getMessage() + " caused by " + e );
        }

        return status;
    }

//
// Determine whether an Aida Channel Archiver data provider is
// functioning correctly.
//
    public int getChannelArchiverStatus(String query, String startTime, String endTime)
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
// Determine whether the Aida Rdb data provider is functioning correctly.
//
    public int getRdbStatus(String query)
    {
        int status;

        status = 1;

        try
        {
            da.reset();

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
// Determine whether an Aida Model data provider is
// functioning correctly.
//
    public int getModelStatus(String query, String type)
    {
        int status;

        status = 1;

        try
        {
            da.reset();
            da.setParam("TYPE", type);

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
 
        err = Err.getInstance("AidaUnixDpServerStatus");

        AidaUnixDpServerStatus s = new AidaUnixDpServerStatus();

        // Get the status of the LCLS Channel Access server.
        int lclsChannelAccessStatus = s.getChannelAccessStatus("VPIO:IN20:111:VRAW//VAL");

        System.out.println("lclsChannelAccessStatus = " + lclsChannelAccessStatus);

        dpServerStatus += lclsChannelAccessStatus;

        // Get the status of the Non-LCLS Channel Access server.
        int nonLclsChannelAccessStatus = s.getChannelAccessStatus("TRS1:AS2:LOST:PAVE//VAL");

        System.out.println("nonLclsChannelAccessStatus = " + nonLclsChannelAccessStatus);

        dpServerStatus += nonLclsChannelAccessStatus * 2;

        // Get the status of the LCLS Channel Archiver server.
        int lclsChannelArchiverStatus = s.getChannelArchiverStatus("VPIO:IN20:111:VRAW//HIST.lcls",
            "11/17/2008 10:00:00", "11/17/2008 10:10:00");

        System.out.println("lclsChannelArchiverStatus = " + lclsChannelArchiverStatus);

        dpServerStatus += lclsChannelArchiverStatus * 4;

        // Get the status of the NLCTA Channel Archiver server.
        int nlctaChannelArchiverStatus = s.getChannelArchiverStatus("TRS1:AS2:LOST:PAVE//HIST.nlcta",
            "08/27/2007 18:00:00", "08/27/2007 18:20:00");

        System.out.println("nlctaChannelArchiverStatus = " + nlctaChannelArchiverStatus);

        dpServerStatus += nlctaChannelArchiverStatus * 8;

        // Get the status of the PEPII Channel Archiver server.
        int pepiiChannelArchiverStatus = s.getChannelArchiverStatus("HR81:CAV2:VACM//HIST.pepii",
            "08/27/2007 18:00:00", "08/27/2007 18:10:00");

        System.out.println("pepiiChannelArchiverStatus = " + pepiiChannelArchiverStatus);

        dpServerStatus += pepiiChannelArchiverStatus * 16;

        // Get the status of the Aida Rdb server.
        int rdbStatus = s.getRdbStatus("LCLS//BSAnames");

        System.out.println("rdbStatus = " + rdbStatus);

        dpServerStatus += rdbStatus * 32;

        // Get the status of the Aida Model server.
        int modelStatus = s.getModelStatus("BPMS:IN20:235//twiss", "DESIGN");

        System.out.println("modelStatus = " + modelStatus);

        dpServerStatus += modelStatus * 64;

        // Get the status of the FACET Channel Archiver server.
        int facetChannelArchiverStatus = s.getChannelArchiverStatus("LI20:ASTS:VP10//HIST.facet",
            "01/31/2011 18:00:00", "01/31/2011 18:10:00");

        System.out.println("facetChannelArchiverStatus = " + facetChannelArchiverStatus);

        dpServerStatus += facetChannelArchiverStatus * 128;

        System.exit(dpServerStatus);
    }
}
