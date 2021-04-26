package edu.stanford.slac.aida.lib.util.common;

/**
 * Encapsulates a constraint to a single dimension of an array
 * 
 * @see ArrayBound
 */
public class SimpleBound extends ArrayBound
{
        /**
         * Index. Index of selected element in this vector
         */
        public int index;

        /**
         * Constructs a Simple Bound
         * 
         * @param theIndex
         *        The index of the indexed element
         */
        public SimpleBound(int theIndex)
        {
                type = Type.SIMPLE;
                index = theIndex;
        }
}

