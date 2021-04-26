/*
 * AidaExpr.java
 *
 * Created on 19 September 2002, 17:00
 */

package edu.stanford.slac.aida.lib.util.common;

/**
 * Encapsulates an Aida expression of form literal | variable | expr + expr ) | expr - expr )
 */
public abstract class AidaExpr
{
        /**
         * Valid types for the Aida Expression
         */
        public static class Type
        {
                /**
                 * Simple bound. Single numerical value refering to a specific element in
                 * this vector eg. x[10]
                 */
                public static final int LITERAL = 0;

                /**
                 * Range bound. Single Range refering to a specific range of elements in
                 * this vector eg. x[10-20]
                 */
                public static final int VARIABLE = 1;

                /**
                 * Named bound. Sting specifier(s) that reference(s) the named element(s)
                 * in this vector eg. elements[x,y] elements named x and y
                 */
                public static final int ADD = 2;

                /**
                 * Expression bounds. Complex ranges containing references to AIDA
                 * veriables which can be part of simple expressions eg.
                 * elements[(#LENGTH-1)-#LENGTH] last two elements eg. elements[#LENGTH]
                 * last two elements
                 */
                public static final int SUBTRACT = 3;
        }

        /**
         * Type of this Expression
         */
        public int type;

}

