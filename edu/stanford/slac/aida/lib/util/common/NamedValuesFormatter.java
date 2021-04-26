package edu.stanford.slac.aida.lib.util.common;

import java.io.PrintStream;
import java.util.*;

public abstract class NamedValuesFormatter
{
    boolean shouldDisplayLabels_ = true;
    protected String[] labels = new String[0];
    protected String[][] cells = new String[0][];
    protected int numRows = 0;
    protected int numCols = 0;


    public static NamedValuesFormatter create (String name)
    {
        if ("rows".equals (name))
            return new NamedValuesRowFormatter();
        else
            return new NamedValuesColumnFormatter();
    }

    
    public void assignNamedValues (NamedValues namedValues)
    {
        labels = namedValues.getLabels();
        cells = namedValues.getValues();
        updateDimensions();
    }

    
    protected int getColumnWidth (int col)
    {
        String longest = "";
        for (int row = 0; row < numRows; ++row)
        {
            String text = getCell (row, col);
            if (text.length() > longest.length())
                longest = text;
        }

        return longest.length();
    }


    
    public void display (PrintStream out)
    {
        // generate formatting spaces for column widths
        String[] spaces = new String[numCols];
        for (int n = 0; n < spaces.length; ++n)
        {
            int width = getColumnWidth (n) + 2;
            StringBuffer space = new StringBuffer (width);
            for (int i = 0; i < width; ++i)
                space.append (' ');
            spaces[n] = space.toString();
        }

        for (int m = 0; m < numRows; ++m)
        {
            for (int n = 0; n < numCols; ++n)
                out.print (getFormattedCell (m, n, spaces[n]));

            out.println ("");
        }
    }


    public boolean shouldDisplayLabels ()
    {
        return shouldDisplayLabels_;
    }


    public void allowLabelDisplay (boolean yesno)
    {
        shouldDisplayLabels_ = yesno;
        updateDimensions();
    }


    public static String centerText (String text, String space)
    {
        int rem = space.length() - text.length();
        int b = rem / 2;
        int a = rem - b;
        return space.substring (0,a) + text + space.substring (0,b);
    }


    public static String leftJustifyText (String text, String space)
    {
        return text + space.substring (text.length());
    }


    public static String rightJustifyText (String text, String space)
    {
        return space.substring (text.length()) + text;
    }

    
    abstract public boolean isLabelCell (int row, int col);
    abstract public String getCell (int row, int col);
    abstract public String getFormattedCell (int row, int col, String space);
    abstract public void updateDimensions ();
}
