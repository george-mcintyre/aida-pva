/*
 * ExprBound.java
 *
 * Created on 19 September 2002, 17:04
 */

package edu.stanford.slac.aida.lib.util.common;

/**
 * Encapsulates a constraint to a single dimension of an array
 * 
 * @see ArrayBound
 */
public class ExprBound extends ArrayBound
{
        /**
         * Aida Expression
         */
        public AidaExpr expr;

        /** Creates a new instance of ExprBound */
        public ExprBound(AidaExpr theExpr)
        {
                type = Type.EXPR;
                expr = theExpr;
        }
}

