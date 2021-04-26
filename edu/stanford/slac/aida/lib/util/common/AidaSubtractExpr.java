/*
 * AidaExpr.java
 *
 * Created on 19 September 2002, 17:00
 */

package edu.stanford.slac.aida.lib.util.common;

/**
 * Encapsulates an Aida subtraction expression. Can be ( expr - expr )
 */
public class AidaSubtractExpr extends AidaExpr
{
        /**
         * Left hand side of subtraction
         */
        AidaExpr lhs;

        /**
         * Right hand side of subtraction
         */
        AidaExpr rhs;

        /** Creates a new instance of AidaSubtractExpr */
        public AidaSubtractExpr(AidaExpr theLHS, AidaExpr theRHS)
        {
                lhs = theLHS;
                rhs = theRHS;
                type = Type.SUBTRACT;
        }
}
