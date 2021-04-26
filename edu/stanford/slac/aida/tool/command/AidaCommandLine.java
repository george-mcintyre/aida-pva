package edu.stanford.slac.aida.tool.command;

import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.except.*;
import java.util.*;


public class AidaCommandLine
{
    static final String DASH = "-";
    static final String EQ = "=";


    private Vector params = new Vector();
    private Map options = new HashMap();
    private Set used = new HashSet();
    private String[] args = null;


    public AidaCommandLine (String[] args)
        throws BadArguments
    {
        this.args = args;
        parseAndRemoveOptions();
    }


    /**
       Returns value associated with specified option name.  Options that were
       specified without values return an empty string.  If the requested
       option was not parsed from the command line, then a null is returned.

       @param name the name of the option to find
       
     */
    public String getOption (String name)
    {
        used.add (name);
        
        // we don't put nulls in options, so null return -> non-existant key
        return (String)options.get (name);
    }


    public boolean isOptionSet (String name)
    {
        return (getOption (name) != null);
    }


    /**
       Returns current args list.  May be invalidated by subsequent calls
       to methods that modify the argument list.
     */
    public String getArg (int idx)
    {
        return args[idx];
    }


    public int argCount ()
    {
        return args.length;
    }
    
    
    /**
       Removes options of the form -name=value or -name from the provided
       array, returning a modified array less those elements.  As options are
       removed from the array, they are inserted into a hash map.

       @param args the list of command line arguments
       @param supported list of options that are supported
    */
    void parseAndRemoveOptions ()
        throws BadArguments
    {
        Vector nargs = new Vector ();
        for (int i = 0; i < args.length; ++i)
        {
            String arg = args[i];

            boolean isNumber = true;
            try {
                Double.parseDouble (arg);
            } catch (Exception e) {
                isNumber = false;
            }
            

            // does it look like an option?
            if (!isNumber && arg.startsWith (DASH))
            {
                String tag = arg.substring (DASH.length());
                String val = "";
                boolean isParam = false;

                int idx = tag.indexOf (EQ, DASH.length());

                // value specified?
                if (idx > 0)
                {
                    val = tag.substring (idx + 1);
                    tag = tag.substring (0, idx);

                    // if it's of the form -Dxx=yy, then it's an AIDA method
                    // argument parameter
                    if (tag.startsWith ("D")) {
                        tag = tag.substring (1);
                        isParam = true;
                        params.addElement (tag);
                        params.addElement (val);
                    }
                }

                if (!isParam)
                    options.put (tag, val);
            }

            // not an option
            else nargs.addElement (arg);
        }

        // replace original args with processed list
        args = (String[])nargs.toArray (new String[nargs.size()]);
    }


    /**
       Returns values specified as -Dxx=yy on command-line
     */
    String[] getParametersAsArray ()
        throws BadArguments
    {
        String[] pa = new String[params.size()];
        for (int i = 0; i < pa.length; ++i)
            pa[i] = (String)params.get (i);
        return pa;
    }


    /**
       Returns values specified as -Dxx=yy on command-line
     */
    String getParametersAsString ()
    {
        StringBuffer sb = new StringBuffer();
        
        Enumeration e = params.elements();
        while (e.hasMoreElements())
        {
            sb.append (e.nextElement());
            sb.append ('=');
            sb.append (e.nextElement());
            sb.append (';');
        }

        return sb.toString();
    }


    /**
       Generates DaValue by parsing arguments starting at specified index as
       elements of one unnanamed value, or of several named values.  The
       syntax is <value>+ for unnamed, (/<name> <value>+)+ for named.

       @param idx the index in args of the first element comprising value
    */
    DaValue parseValueSpec (int idx)
    {
        DaValue.Type STRINGA = new DaValue.Type (DaValue.Type.STRINGA);
        DaValue.Type DOUBLEA = new DaValue.Type (DaValue.Type.DOUBLEA);
        DaValue.Type SHORTA = new DaValue.Type (DaValue.Type.SHORTA);
        DaValue.Type STRUCT = new DaValue.Type (DaValue.Type.STRUCT);

        DaValue values = null;
        DaValue value = null;
        for (int i = idx; i < args.length; ++i)
        {
            String arg = args[i];

            if (arg.startsWith ("/"))
            {
                if (values == null)
                    values = new DaValue ("values", STRUCT);
                
                String name = arg.substring ("/".length());
                value = new DaValue (name, STRINGA);
                values.addElement (value);
            }

            else
            {
                if (value == null)
                    value = new DaValue ("value", STRINGA);
                value.addElement (arg);
            }
        }

        
        DaValue dv = (values == null)? value : values;
        if (dv == null)
            dv = new DaValue();

        return dv;        
    }


    /**
       Throws a BadArguments exception if some options were parsed but never
       used.
     */
    void assertNoUnusedOptions ()
        throws BadArguments
    {
        Iterator it = options.keySet().iterator();
        while (it.hasNext())
        {
            String s = (String)it.next();
            if (!used.contains (s))
                throw new BadArguments ("Unused option: " + s);
        }
    }
}
