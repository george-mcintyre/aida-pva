/*
 * AQSParser.java
 *
 * Created on 17 July 2002, 11:31
 */

package edu.stanford.slac.aida.lib.util.common;

import java.util.*;
import edu.stanford.slac.err.*;
import edu.stanford.slac.except.*;

/**
 * Parser object that represents an AQS Parser. An AQS parser understands the
 * AIDA Query Specification and can parse it into a _DaReference.
 * 
 * @author George McIntyre
 * @version 26-Feb-2007 Bob Hall (rdh): Changed to use new AQSLex method
            getInstanceToken instead of getToken for instance names to allow
            names that have non-leading parsing tokens such as "+" and "-".
 * @version 28-Sep-2004 Greg White (greg): Add UndefinedNameException to constructor
 * @see AQSLex
 * @see _DaReference
 */
public class AQSParser
{
        /**
         * Lexical analyser object to be used to analyse the input text stream
         * 
         * @see class AQSLex
         */
        private AQSLex m_lex;

        /**
         * Given _DaReference to parse the AQS target string into.
         */
        private _DaReference m_reference;

        /**
         * Lexical analyser object to be used to analyse the input text stream
         * 
         * @see class AQSLex
         */
        private Hashtable m_args;

        /**
         * Creates a new instance of AQSParser
         * 
         * @param referenceObject
         *        _DaReference object to parse the target string into
         * @param targetString
         *        given AQS target string to be parsed
         * @throws UndefinedNameException
         */
        public AQSParser(_DaReference referenceObject, String targetString, Hashtable args)
        	   throws UndefinedNameException	// TODO: Should throw SyntaxException to dissambiguate from mismatch with Directory Service.
        {
                m_args = args; // Args to set for each sub element in reference
                m_reference = referenceObject;
                m_lex = new AQSLex(targetString);
                if ((m_reference = parseQuery(m_reference)) == null)
                {
                        Err err = Err.getInstance();
                        throw (UndefinedNameException) 
                        	err.log(new UndefinedNameException("Syntax Error in: '" + 
                        	        targetString + "'"));
                }
        }

        /**
         * Starting point for parse function. query := simple_query |
         * user_defined_structure This method parses the remaining tokens in the
         * lexical analyser object to see if they correspond to an AQS Query.
         * 
         * @return A valid _DaReference: A reference object that is the parsed
         *         version of the lexical context's query. All relevant tokens
         *         are consumed. null: This is not a valid AQS query. The
         *         lexical context is left unchanged.
         * @param reference
         *        The reference object to parse the query into.
         */

        private _DaReference parseQuery(_DaReference reference)
        {
                _DaReference t;
                if ((t = parseSimpleQuery(reference)) == null) // Simple query
                        t = parseUDS(reference); // user defined structure

                return (t);
        }

        /**
         * user_defined_structure := complex_query [ ?,? user_defined_structure ]
         * This method parses the remaining tokens in the lexical analyser
         * object to see if they correspond to an AQS user_defined_structure.
         * 
         * @return A valid _DaReference: A reference object that is the parsed
         *         version of the lexical context's user_defined_structure. All
         *         relevant tokens are consumed. null: This is not a valid AQS
         *         user_defined_structure . The lexical context is left
         *         unchanged.
         * @param reference
         *        The reference object to parse the query into.
         */

        private _DaReference parseUDS(_DaReference reference)
        {
                _DaReference t;
                if ((t = parseComplexQuery(new _DaReference())) == null)
                        return null;

                t.m_args = m_args; // add global args to subquery
                reference.type = _DaReference.Type.STRUCT;
                reference.addElement(t);

                // Optional user defined query
                if (m_lex.getToken().token == AQSToken.Token.STRUCT_SEPARATOR)
                        parseUDS(reference); // get follow on
                else
                        m_lex.unGetToken(); // Put back last incorrect token

                return (reference);
        }

        /**
         * simple_query := data_reference [aida_symbol_spec ] This method parses
         * the remaining tokens in the lexical analyser object to see if they
         * correspond to an AQS simple_query. AIDA symbols can be thought of as
         * preprocessor symbols. They are things like #COUNT, #LENGTH or #TYPE
         * etc that can be added to a target to get meta information about it.
         * 
         * @return A valid _DaReference: A reference object that is the parsed
         *         version of the lexical context's simple_query. All relevant
         *         tokens are consumed. null: This is not a valid AQS
         *         simple_query. The lexical context is left unchanged.
         * @param reference
         *        The reference object to parse the query into.
         */

        private _DaReference parseSimpleQuery(_DaReference reference)
        {
                _DaReference t;
                if ((t = parseDataReference(reference)) == null)
                        return null;

                return (reference);
        }

        /**
         * data_reference := instance ?//? [ attribute_specification ] This
         * method parses the remaining tokens in the lexical analyser object to
         * see if they correspond to an AQS data_reference.
         * 
         * @return A valid _DaReference: A reference object that is the parsed
         *         version of the lexical context's data_reference. All relevant
         *         tokens are consumed. null: This is not a valid AQS
         *         data_reference. The lexical context is left unchanged.
         * @param reference
         *        The reference object to parse the query into.
         */

        private _DaReference parseDataReference(_DaReference reference)
        {
                // mark for subsequent rollback
                int m = m_lex.mark();

                AQSToken t;

                // instance.  The new AQSLex getInstanceToken method is used
                // instead of method getToken to allow instance names that
                // have non-leading parsing tokens, such as "+" and "-".
                if ((t = m_lex.getInstanceToken()).token != AQSToken.Token.STRING)
                {
                        m_lex.rollback(m);
                        return null;
                } else
                        reference.instance = t.value;

                // separator
                if ((t = m_lex.getToken()).token != AQSToken.Token.SEPARATOR)
                {
                        m_lex.rollback(m);
                        return null;
                }

                // attribute
                if ((t = m_lex.getToken()).token != AQSToken.Token.STRING)
                {
                        m_lex.rollback(m);
                        return null;
                }

                reference.attribute = t.value;
                reference.type = _DaReference.Type.SIMPLE;

                // Optional Array specifiers
                m = m_lex.mark(); // Mark point where non array version ends
                if (parseRangeSpecification(reference) == null)
                        m_lex.rollback(m);

                return (reference);
        }

        /**
         * range_specification := '[' <string_range>|<numerical_range>|
         * <bound>']' {<range_specification>} This method parses the
         * remaining tokens in the lexical analyser object to see if they
         * correspond to an AQS range specification. Examples of valid range
         * specifications are all of the following: [x,y,y] - Elements x, y and
         * z of structure (valid only for structures) [1-4] - Elements 1 to 4 of
         * the the array or structure [1-3][4] - Elements 1 to 3 of first
         * dimension of array or structure and element 4 of second [#LENGTH-4] -
         * Element which is the 4th from the last element in the array or
         * structure
         * 
         * @return A valid _DaReference: A reference object that is the parsed
         *         version of the lexical context and containing the range
         *         specification. All relevant tokens are consumed. null: This
         *         is not a valid AQS rance specification. The lexical context
         *         is left unchanged.
         * @param reference
         *        The reference object to parse the query into.
         */

        private ArrayBound parseRangeSpecification(_DaReference r)
        {
                // mark for subsequent rollback
                int m = m_lex.mark();
                ArrayBound b;

                AQSToken t;

                // First range specification
                if (m_lex.getToken().token != AQSToken.Token.ARRAY_OPEN
                                || ((b = parseStringRange()) == null
                                                && (b = parseNumericalRange()) == null && 
                                                (b = parseBound()) == null)
                                || m_lex.getToken().token != AQSToken.Token.ARRAY_CLOSE)
                {
                        m_lex.rollback(m);
                        return null;
                }

                r.bounds.addElement(b); // Add the range spec just found
                m = m_lex.mark(); // Mark point where we have at least one range
                                  // specification

                // Optional Array specifiers
                if (parseRangeSpecification(r) == null)
                        m_lex.rollback(m);

                return (b);
        }

        /**
         * Is the given string a number
         * 
         * @param theString
         *        String to test
         * @return true if given string contains a valid number false if not
         */
        private boolean isNumber(String theString)
        {
                try
                {
                        Integer.parseInt(theString);
                } catch (NumberFormatException e)
                {
                        return false;
                }
                return true;
        }

        /**
         * String range_specification :=<string>{ ,<string>} This method
         * parses the remaining tokens in the lexical analyser object to see if
         * they correspond to an AQS string range specification. Examples of
         * valid range specifications are all of the following: x,y,y - Elements
         * x, y and z of structure (valid only for structures)
         * 
         * @return A valid NamedBound: A NameBound object that is the parsed
         *         version of the lexical context and containing the string
         *         range specification. All relevant tokens are consumed. null:
         *         This is not a valid AQS rance specification. The lexical
         *         context is left unchanged.
         */

        private NamedBound parseStringRange()
        {
                // mark for subsequent rollback
                int m = m_lex.mark();

                AQSToken t;
                NamedBound b;

                // First string range specification
                if ((t = m_lex.getToken()).token != AQSToken.Token.STRING || isNumber(t.value))
                {
                        m_lex.rollback(m);
                        return null;
                }

                b = new NamedBound(t.value);
                m = m_lex.mark(); // Mark point where we have at least one range
                                  // specification

                // Optional Array specifiers
                while (m_lex.getToken().token == AQSToken.Token.STRUCT_SEPARATOR)
                {
                        if ((t = m_lex.getToken()).token != AQSToken.Token.STRING
                                        || isNumber(t.value))
                                break;

                        m = m_lex.mark(); // Mark point where we have at least
                                          // one range specification
                        b.addName(t.value);
                }
                m_lex.rollback(m);

                return (b);
        }

        /**
         * Numerical range_specification :=<bound>[ - [<bound>] ] | - [
         * <bound>] This method parses the remaining tokens in the lexical
         * analyser object to see if they correspond to an AQS numerical range
         * specification. Examples of valid range specifications are all of the
         * following: 1-10 - Elements 1 to 10 of arrays or structures 4 -
         * Elements number 4 of arrays or structures -8 - All elements upto and
         * including 8 of arrays or structures 3- - All elements 3 and above of
         * arrays or structures
         * 
         * @return A valid RangeBound: A RangeBound object that is the parsed
         *         version of the lexical context and containing the numerical
         *         range specification. All relevant tokens are consumed. null:
         *         This is not a valid AQS numerical range specification. The
         *         lexical context is left unchanged.
         */
        private RangeBound parseNumericalRange()
        {
                // mark for subsequent rollback
                int m = m_lex.mark();

                AQSToken t;
                ExprBound b1 = null;

                // Read ahead
                t = m_lex.getToken();
                m_lex.unGetToken();

                // Is this a range specification?
                if ((b1 = parseBound()) == null && // Not Expression
                                (t.token != AQSToken.Token.MINUS) // and Not Minus
                )
                {
                        m_lex.rollback(m);
                        return null;
                }

                // If there is only one bound return it now
                if ((t = m_lex.getToken()).token != AQSToken.Token.MINUS) // Not Minus
                {
                        m_lex.unGetToken();
                        return (new RangeBound(b1, null));
                }

                // Second part of range specification
                return (new RangeBound(b1, parseBound()));
        }

        /**
         * Single bound_specification := 0 |<positive_integer>|
         * <aida_symbol_spec>This method parses the remaining tokens in the
         * lexical analyser object to see if they correspond to an AQS bound
         * specification. Examples of valid bound specifications are all of the
         * following: 0 - Element 0 4 - Element 4 or Aida Symbol Expressions of
         * the form: #LENGTH - Number of elements in this vector (#LENGTH-1) -
         * Expression denoting one less than the number of elements in this
         * vector
         * 
         * @return A valid ExprBound: A ExprBound object that is the parsed
         *         version of the lexical context and containing the bound
         *         specification. All relevant tokens are consumed. null: This
         *         is not a valid AQS bound specification. The lexical context
         *         is left unchanged.
         */
        private ExprBound parseBound()
        {
                AidaExpr e = parseBoundExpr();
                if (e == null)
                        return (null);
                else
                        return (new ExprBound(e));
        }

        /**
         * Single bound_specification := 0 |<positive_integer>|
         * <aida_symbol_spec>This method parses the remaining tokens in the
         * lexical analyser object to see if they correspond to an AQS bound
         * specification. Examples of valid bound specifications are all of the
         * following: 0 - Element 0 4 - Element 4 or Aida Symbol Expressions of
         * the form: #LENGTH - Number of elements in this vector (#LENGTH-1) -
         * Expression denoting one less than the number of elements in this
         * vector
         * 
         * @return A valid AidaExpr: A AidaExpr object that is the parsed
         *         version of the lexical context and containing the bound
         *         specification. All relevant tokens are consumed. null: This
         *         is not a valid AQS bound specification. The lexical context
         *         is left unchanged.
         */
        private AidaExpr parseBoundExpr()
        {
                // mark for subsequent rollback
                int m = m_lex.mark();

                AQSToken t = m_lex.getToken();

                // Single Number
                if (t.token == AQSToken.Token.STRING && isNumber(t.value)) // Number
                        return (new AidaLiteral(Integer.valueOf(t.value), DaValue.Type.LONG));

                // Single Aida Symbol
                switch (t.token)
                {
                        // Valid for bound specification
                        case AQSToken.Token.KEYWORD_LEN:
                                return (new AidaVariable(t.value));

                        // Invalid for bound specification
                        case AQSToken.Token.KEYWORD_TYPE:
                        case AQSToken.Token.KEYWORD_NAME:
                                m_lex.unGetToken();
                                return null;
                }

                // Aida Symbol Specification
                AidaExpr e;
                if (t.token == AQSToken.Token.OPEN_PAREN && (e = parseExpr()) != null
                                && (t = m_lex.getToken()).token == AQSToken.Token.CLOSE_PAREN)
                        return (e);

                // Otherwise this is not a bound
                m_lex.rollback(m);
                return null;
        }

        /**
         * Aida symbol_expression :=<aida_symbol>+<number>|<number>+
         * <aida_symbol>|<aida_symbol>-<number>|<number>-<aida_symbol>
         * This method parses the remaining tokens in the lexical analyser
         * object to see if they correspond to an AQS symbol expression.
         * Examples of valid expressions are all of the following: #LENGTH-1 -
         * Expression denoting one less than the number of elements in this
         * vector 10+#LENGTH
         * 
         * @return A valid AidaExpr: A AidaExpr object that is the parsed
         *         version of the lexical context and containing the Aida
         *         expression. All relevant tokens are consumed. null: This is
         *         not a valid AQS Aida Expression. The lexical context is left
         *         unchanged.
         */
        private AidaExpr parseExpr()
        {
                // mark for subsequent rollback
                int m = m_lex.mark();

                AQSToken t;
                AidaExpr e1, e2;

                if (((e1 = parseBoundExpr()) != null)
                                && (((t = m_lex.getToken()).token == AQSToken.Token.PLUS) || 
                                        (t.token == AQSToken.Token.MINUS))
                                && ((e2 = parseBoundExpr()) != null))
                {
                        if (t.token == AQSToken.Token.PLUS)
                                return (new AidaSumExpr(e1, e2));
                        else
                                return (new AidaSubtractExpr(e1, e2));
                }

                // Otherwise this is not an Aida expression
                m_lex.rollback(m);
                return null;
        }

        /**
         * complex_query := [<string>?=? ] { query } This method parses the
         * remaining tokens in the lexical analyser object to see if they
         * correspond to an AQS complex_query.
         * 
         * @return A valid _DaReference: A reference object that is the parsed
         *         version of the lexical context's complex_query. All relevant
         *         tokens are consumed. null: This is not a valid AQS
         *         complex_query. The lexical context is left unchanged.
         * @param reference
         *        The reference object to parse the query into.
         */

        private _DaReference parseComplexQuery(_DaReference reference)
        {
                // mark for subsequent rollback
                int m = m_lex.mark();

                AQSToken t;
                String name = null;

                // name
                if ((t = m_lex.getToken()).token == AQSToken.Token.STRING)
                {
                        if (m_lex.getToken().token != AQSToken.Token.NAMED_ELEMENT)
                        {
                                m_lex.rollback(m);
                                return null;
                        }
                        name = t.value;
                } else
                        m_lex.unGetToken(); // string

                // open curly braces
                if ((t = m_lex.getToken()).token != AQSToken.Token.STRUCT_OPEN
                                || parseQuery(reference) == null
                                || (t = m_lex.getToken()).token != AQSToken.Token.STRUCT_CLOSE)
                {
                        m_lex.rollback(m);
                        return null;
                }

                reference.name = name;
                return (reference);
        }
}
