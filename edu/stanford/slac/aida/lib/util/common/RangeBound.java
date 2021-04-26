/*
 * RangeBound.java
 *
 * Created on 19 September 2002, 16:52
 */

package edu.stanford.slac.aida.lib.util.common;

/**
 * Encapsulates a constraint to a single dimension of an array
 * 
 * @see ArrayBound
 */
public class RangeBound extends ArrayBound
{
        /**
         * Lower bound
         */
        public ExprBound lowerBound;

        /**
         * Upper Bound.
         */
        public ExprBound upperBound;

        /**
         * Constructs a Range Bound
         * 
         * @param theLowerBound
         *        The lower boundary
         * @param theUpperBound
         *        The upper boundary
         */
        public RangeBound(ExprBound theLowerBound, ExprBound theUpperBound)
        {
                type = Type.RANGE;
                lowerBound = theLowerBound;
                upperBound = theUpperBound;
        }
}

