/*
 * AidaExpr.java
 *
 * Created on 19 September 2002, 17:00
 */

package edu.stanford.slac.aida.lib.util.common;

/**
 * Encapsulates an Aida sum expression. Can be expr + expr
 */
public class AidaSumExpr extends AidaExpr
{
        /**
         * Left hand side of addition
         */
        AidaExpr lhs;

        /**
         * Right hand side of addition
         */
        AidaExpr rhs;

        /** Creates a new instance of AidaExpr */
        public AidaSumExpr(AidaExpr theLHS, AidaExpr theRHS)
        {
                lhs = theLHS;
                rhs = theRHS;
                type = Type.ADD;
        }

}
