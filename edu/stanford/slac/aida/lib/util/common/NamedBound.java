/*
 * NamedBound.java
 *
 * Created on 19 September 2002, 16:55
 */
package edu.stanford.slac.aida.lib.util.common;

import java.util.*;

/**
 * Encapsulates a constraint to a single dimension of an array
 * 
 * @see ArrayBound
 */
public class NamedBound extends ArrayBound

{
        /**
         * Names List
         */
        public Vector names;

        /**
         * Constructs a Named Bound from an array of strings
         * 
         * @param theNames
         *        the list of string names of referenced elements
         */
        public NamedBound(String[] theNames)
        {
                type = Type.NAMED;
                names = new Vector();
                for (int i = 0; i < theNames.length; i++)
                        names.addElement(theNames[i]);
        }

        /**
         * Constructs a Named Bound from a single string
         * 
         * @param theName
         *        the string names of referenced element
         */
        public NamedBound(String theName)
        {
                type = Type.NAMED;
                names = new Vector();
                names.addElement(theName);
        }

        /**
         * Adds a Named Bound
         * 
         * @param theName
         *        the string names of referenced element
         */
        public void addName(String theName)
        {
                names.addElement(theName);
        }
}