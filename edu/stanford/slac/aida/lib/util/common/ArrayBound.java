/*
 * ArrayBound.java
 *
 * Created on 19 September 2002, 16:44
 */

package edu.stanford.slac.aida.lib.util.common;

/**
 * Base class that encapsulates a constraint to a single dimension of an array
 * 
 * @author gsm
 */
public abstract class ArrayBound
{
        /**
         * Valid types for the Array bounds
         * 
         * @see ArrayBound
         */
        public static class Type
        {
                /**
                 * Simple bound. Single numerical value refering to a specific element in
                 * this vector eg. x[10]
                 */
                public static final int SIMPLE = 0;

                /**
                 * Range bound. Single Range refering to a specific range of elements in
                 * this vector eg. x[10-20]
                 */
                public static final int RANGE = 1;

                /**
                 * Named bound. Sting specifier(s) that reference(s) the named element(s)
                 * in this vector eg. elements[x,y] elements named x and y
                 */
                public static final int NAMED = 2;

                /**
                 * Expression bounds. Complex ranges containing references to AIDA
                 * veriables which can be part of simple expressions eg.
                 * elements[(#LENGTH-1)-#LENGTH] last two elements eg. elements[#LENGTH]
                 * last two elements
                 */
                public static final int EXPR = 3;
        }

        /**
         * Type of this array bound
         */
        public int type;
}