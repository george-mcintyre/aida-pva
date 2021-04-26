/*
 * AidaExpr.java
 *
 * Created on 19 September 2002, 17:00
 */

package edu.stanford.slac.aida.lib.util.common;

/**
 * Encapsulates an Aida literal value.
 */
public class AidaLiteral extends AidaExpr
{
        /**
         * Type of this Literal in DaValue.Type Codes Valid codes are all the non Array or
         * compound (Any, Structure) types Anything that can be stored in a
         * java.lang.Object
         */
        public int subType;

        public Object literal;

        /** Creates a new instance of AidaLiteral */
        public AidaLiteral(Object theValue, int theType)
        {
                literal = theValue;
                subType = theType;
                type = Type.LITERAL;
        }
}

