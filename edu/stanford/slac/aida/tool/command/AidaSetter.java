/*
  This is a simple tool that allows sending DaValue-encapsulated data to aida
  services that implement the "set" method, via options specified on
  command-line.
 */
package edu.stanford.slac.aida.tool.command;

import edu.stanford.slac.aida.lib.da.*;
import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.err.Err;
import edu.stanford.slac.except.*;
import java.util.*;
import org.omg.CORBA.*;


public class AidaSetter
{
    interface Setter
    {
        void set (DaValue dv)
            throws UnableToSetDataException,
            UndefinedNameException;
    }

    
    class DaSetter implements Setter
    {
        DaObject da;
        String target;
        
        public DaSetter (String target, String params)
            throws AidaInitException
        {
            da = new DaObject();
            this.target = new String (target);
            da.setParam (params);
        }

        public void set (DaValue dv)
            throws UnableToSetDataException
        {
            da.setDaValue (target, dv);
        }
    }


    class DirectSetter extends DirectVariable implements Setter
    {
        public DirectSetter (String dpName, String target, String[] params)
            throws AidaInitException
        {
            super (dpName, target);
            setWriteArgs (params);
        }

        public void set (DaValue dv)
            throws UnableToSetDataException,
            UndefinedNameException
        {
            write (dv);
        }
    }



    Setter setter = null;
    DaValue goalValue = null;


    public AidaSetter (AidaCommandLine cmdline)
        throws AidaInitException,
        BadArguments
    {
        String dataProviderName = cmdline.getOption ("dp");
        String target = cmdline.getArg (0);
        goalValue = cmdline.parseValueSpec (1);

        cmdline.assertNoUnusedOptions();
        
        if (dataProviderName != null) {
            setter = new DirectSetter (
                dataProviderName, target, cmdline.getParametersAsArray());
        }
        else {
            setter = new DaSetter (target, cmdline.getParametersAsString());
        }
    }


   public void set ()
       throws UnableToSetDataException,
       UndefinedNameException
    {
        setter.set (goalValue);
    }
    

            
    public static void usage ()
    {
        System.out.println (
            "Usage: [options] <target> <desired value>"
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
            "                        this option may be used repeatedly."
            + "\n" +
            "  target                aida object name"
            + "\n" +
            "  desired value"
            + "\n" +
            "        unnamed:        <scalar> or <vector>"
            + "\n" +
            "        named:          /<name> <scalar> or /<name> <vector> "
            + "\n" +
            "        * scalars are individual values."
            + "\n" +
            "        * vectors are sequences of white-spaces separated values."
            + "\n" +
            "        * multiple named values may appear on command-line."
            + "\n");
        
        System.exit (1);
    }


    public static void main (String[] args)
    {
        try
        {
            AidaCommandLine cmdline = new AidaCommandLine (args);

            // at minimum, must specify target and value
            if ((cmdline.argCount() < 2) || cmdline.isOptionSet ("help"))
                usage();

           // need to instantiate Err singleton for AIDA
            Err.getInstance ("AidaSetter");

            (new AidaSetter (cmdline)).set();
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
