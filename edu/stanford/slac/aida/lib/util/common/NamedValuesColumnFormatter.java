package edu.stanford.slac.aida.lib.util.common;

import java.util.*;

public class NamedValuesColumnFormatter extends NamedValuesFormatter
{
    public String getCell (int row, int col)
    {
        if (shouldDisplayLabels())
            if (row == 0)
                return labels[col];
            else --row;

        if (row >= cells[col].length)
            return "";

        return cells[col][row];
    }


    public String getFormattedCell (int row, int col, String space)
    {
        if (isLabelCell (row, col))
            return centerText (getCell (row, col), space);
        
        return rightJustifyText (getCell (row, col), space);
    }


    public boolean isLabelCell (int row, int col)
    {
        return ((row == 0) && shouldDisplayLabels());
    }


    public void updateDimensions ()
    {
        numCols = cells.length;
        
        numRows = 0;
        for (int i = 0; i < cells.length; ++i)
            if (cells[i].length > numRows)
                numRows = cells[i].length;

        if (shouldDisplayLabels())
            ++numRows;
    }
}
