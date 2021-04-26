/*
  This is a simple tool that allows fetching arbitrary aida data via options
  specified on command-line.
 */
package edu.stanford.slac.aida.tool.command;

import edu.stanford.slac.aida.lib.da.*;
import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.err.Err;
import edu.stanford.slac.except.*;

import java.util.*;


public class AidaFetcher
{
    interface Fetcher
    {
        public DaValue fetch ()
            throws UnableToGetDataException, UndefinedNameException;
    }

    
    class DaFetcher implements Fetcher
    {
        private DaObject da;
        private String target;
    
        DaFetcher (String target, String params)
            throws AidaInitException
        {
            da = new DaObject();
            this.target = target;
            da.setParam (params);
        }

        public DaValue fetch ()
            throws UnableToGetDataException,
            UndefinedNameException
        {
            return da.getDaValue (target);
        }
    }

    
    class DirectFetcher extends DirectVariable implements Fetcher
    {
        DirectFetcher (String dpName, String target, String[] params)
            throws AidaInitException
        {
            super (dpName, target);
            setReadArgs (params);
        }

        public DaValue fetch ()
            throws UnableToGetDataException,
            UndefinedNameException
        {
            return read();
        }
    }


    
    Fetcher fetcher = null;

    
    public AidaFetcher (AidaCommandLine cmdline)
        throws AidaInitException,
        BadArguments
    {
        String dataProviderName = cmdline.getOption ("dp");
        String target = cmdline.getArg (0);
        
        if (dataProviderName != null) {
            fetcher = new DirectFetcher (
                dataProviderName, target, cmdline.getParametersAsArray());
        }
        else {
            fetcher = new DaFetcher (target, cmdline.getParametersAsString());
        }

        cmdline.assertNoUnusedOptions();
    }


    public DaValue fetch ()
        throws UnableToGetDataException,
        UndefinedNameException
    {
        return fetcher.fetch();
    }


    public static void usage ()
    {
        System.out.println (
            "Usage: [options] <target>"
            + "\n" +
            "  options"
            + "\n" +
            /*
            "    -dp=<name>          talk to data provider named <name>,"
            + "\n" +
            "                        completely bypassing DaServer middleman."
            + "\n" +
            */
            "    -Dxx=yy             set AIDA parameter \"xx\" to value \"yy\""
            + "\n" +
            "                        this  may be used repeatedly."
            + "\n" +
            "    -format=rows        display output table in row order"
            + "\n" +
            "    -format=columns     display output table in column order"
            + "\n" +
            "    -labels             display labels for rows/columns"
            + "\n" +
            "  target                aida object name"
            + "\n");
        System.exit (1);
    }

    
    public static void main (String[] args)
    {
        try
        {
            AidaCommandLine cmdline = new AidaCommandLine (args);
            if ((cmdline.argCount() < 1) || cmdline.isOptionSet ("help"))
                usage();

            // need to instantiate Err singleton for AIDA
            Err.getInstance ("AidaFetcher");
            
            String format = cmdline.getOption ("format");
            if (format == null) format = "default";
            NamedValuesFormatter formatter =
                NamedValuesFormatter.create (format);
            String labels = cmdline.getOption ("labels");
            formatter.allowLabelDisplay (labels != null);
                
            DaValue dv = (new AidaFetcher (cmdline)).fetch();
            formatter.assignNamedValues (dv.getNamedValues());
            formatter.display (System.out);
        }
        catch (Exception e)
        {
            System.out.println ("Exception: " + e.getMessage());
            //e.printStackTrace();
            System.exit (2);
        }

        System.exit (0);
    }
}
