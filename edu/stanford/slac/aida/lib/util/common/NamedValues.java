package edu.stanford.slac.aida.lib.util.common;

import java.util.*;

public class NamedValues
{
    protected Vector labels = new Vector();
    protected Vector values = new Vector();

    
    public void add (String name, Vector value)
    {
        labels.addElement (name);
        String[] array = new String[value.size()];
        for (int i = 0; i < array.length; ++i)
            array[i] = value.get(i).toString();
        
        values.addElement (array);
    }

    public String[] getLabels ()
    {
        return (String[])labels.toArray (new String[labels.size()]);
    }

    public String[][] getValues ()
    {
        return (String[][])values.toArray (new String[values.size()][]);
    }
}
