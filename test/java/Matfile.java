package test.java;

// Utility for the Performance tests to manage the matlab output file.

import java.io.*;
import java.util.*;
import java.text.*;

    public class Matfile 
    {
	private PrintStream ps;
    /*
    ** If file exists, open it and append current time else, created and write description & time.
    */
	Matfile (String filename, String Desc, String comment)
	{
	    String fpname = "/u/cd/rcs/Matlab/" + filename;
	    try
	    {
		boolean file_created = false;
		File f = new File(fpname);
		if (!f.exists())
		{
		    f.createNewFile();
		    file_created = true;
		}
		/* Append to file of already exists */
		FileOutputStream fos = new FileOutputStream (fpname, true);
		ps = new PrintStream( fos );
		if (file_created)
		    ps.println(Desc);
		String datestring = DateFormat.getDateTimeInstance().format(new Date());
		ps.println(datestring);
		ps.println(comment);
 
	    }
	    catch (Exception e)
	    {
		System.err.println( e.getMessage() + " exception in matfile");
	    }
	}

	void print (double tval)
	{
	    ps.print(tval);  ps.print (" ");
	}

	void println ()
	{
	    ps.println();
	}

	void close ()
	{
	    ps.close();
	}
    }
