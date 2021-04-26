package edu.stanford.slac.aida.lib.util.common;

import edu.stanford.slac.except.*;
import java.text.DateFormat;
import java.text.SimpleDateFormat;


/** Adds knowledge of AIDA-specific date specs to standard date format.
 */
public class AidaDateFormat extends SimpleDateFormat
{
    public void applyPattern (String pattern)
        throws IllegalArgumentException
    {
        if ("DATEFORM0".equals (pattern))
        {
            // dd-mmm-yyyy hh:mm:ss
            super.applyPattern ("dd-MMM-yyyy hh:mm:ss");
        }
        else if ("MMDDYYYY".equals (pattern))
        {
            // mm/dd/yyyy hh:mm:ss
            super.applyPattern ("MM/dd/yyyy hh:mm:ss");
        }
        else if ("MMDDYYYY_FRAC".equals (pattern))
        {
            // mm/dd/yyyy hh:mm:ss.ff
            super.applyPattern ("MM/dd/yyyy hh:mm:ss.SS");
        }
            
        else super.applyPattern (pattern);
    }
}
