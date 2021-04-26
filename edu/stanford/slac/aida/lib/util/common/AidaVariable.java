/*
 * AidaExpr.java
 *
 * Created on 19 September 2002, 17:00
 */
package edu.stanford.slac.aida.lib.util.common;

/**
 * Encapsulates an Aida expression. Can be aida_symbol, where <aida_symbol> 
 * is one of the symbols defined in the AQS spec e.g. '#LENGTH'
 */
public class AidaVariable extends AidaExpr
{
        /**
         * Name of aida variable
         */
        String name;

        /** Creates a new instance of AidaExpr */
        public AidaVariable(String theVariable)
        {
                name = theVariable;
                type = Type.VARIABLE;
        }
}

