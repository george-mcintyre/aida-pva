/*
 * DpASQToken.java
 *
 * Created on 17 July 2002, 13:20
 */

package edu.stanford.slac.aida.lib.util.common;

import edu.stanford.slac.err.*;			// Err issuance singleton and API.

/**
 * Token object. This class encapsulates an AQS token. It defines all terminal
 * symbols and allows representation of the token by integer identifier as well
 * as giving access to the actual string token.
 * 
 * @author George McIntyre
 * @version 29-Sep-2004, Greg White: Added exception logging.
 */
public class AQSToken
{
        // Definition of macros representing all keyword and symbol strings

        /**
         * Separator between Instance and Attribute
         */
        public static String SEPARATOR = "//";

        /**
         * User defined array open specifier
         */
        public static String ARRAY_OPEN = "[";

        /**
         * User defined array close specifier
         */
        public static String ARRAY_CLOSE = "]";

        /**
         * User defined block scope delimiter open
         */
        public static String STRUCT_OPEN = "{";

        /**
         * User defined block scope delimiter close
         */
        public static String STRUCT_CLOSE = "}";

        /**
         * Operator for assigning a name to an element in a user defined
         * structure
         */
        public static String NAMED_ELEMENT = "=";

        /**
         * Lexical element for separating adjacent elements in a user defined
         * structure
         */
        public static String STRUCT_SEPARATOR = ",";

        /**
         * Minus sign. Used for ranges and for simple expressions involving
         * subtraction
         */
        public static String MINUS = "-";

        /**
         * Plus sign. Used for simple expressions involving addition
         */
        public static String PLUS = "+";

        /**
         * Open paretheses. Used in simple expressions to force evaluation
         * precedence
         */
        public static String OPEN_PAREN = "(";

        /**
         * Close paretheses. Used in simple expressions to force evaluation
         * precedence
         */
        public static String CLOSE_PAREN = ")";

        /**
         * Array of keyword/symbol strings for translation between code & string
         * representations NB: Strings MUST be ordered such that entries that
         * are prefixes of other entries follow the prefixed string (not
         * necessarily adjacent). This is because of the way that the efficient
         * matcher in getToken() works Very important: This table must match
         * exactly the following table containing the token codes. Adding to
         * this table requires an obligatory addition to the tokens table
         */
        public static String[] Symbols = { SEPARATOR, // Separator between
                                                      // instance and attribute
                ARRAY_OPEN, // Open array specifier
                ARRAY_CLOSE, // Close array specifier
                STRUCT_OPEN, // Open struct element delimiter
                STRUCT_CLOSE, // Close struct element delimiter
                NAMED_ELEMENT, // Named element operator
                STRUCT_SEPARATOR, // Separator bewteen adjacent structure
                                  // elements
                MINUS, // Minus sign also bounds separator
                PLUS, // Plus sign
                OPEN_PAREN, // open simple expression
                CLOSE_PAREN, // close simple expression
                // Keywords
                "#LENGTH", // Length of this dimension of the array result
                "#TYPE", // Type of element
                "#NAME" // Name of element
        };

        /**
         * Array of keyword/symbol codes for translation between code & string
         * representations Very important: This table must match exactly the
         * previous table containing the strings corsponding to these codes.
         * Adding to this table requires an obligatory addition to the strings
         * table
         */
        public static int[] Tokens = { Token.SEPARATOR, Token.ARRAY_OPEN, Token.ARRAY_CLOSE,
                Token.STRUCT_OPEN, Token.STRUCT_CLOSE, Token.NAMED_ELEMENT,
                Token.STRUCT_SEPARATOR, Token.MINUS, Token.PLUS, Token.OPEN_PAREN,
                Token.CLOSE_PAREN, Token.KEYWORD_LEN, Token.KEYWORD_TYPE, Token.KEYWORD_NAME };

        /**
         * Token codes: This table gives numeric codes to the terminal lexical
         * elements
         */
        public static class Token
        {
                /**
                 * End of Input
                 */
                public static final int EOF = -1;

                /**
                 * String
                 */
                public static final int STRING = 0;

                /**
                 * Separator between instance and attribute
                 */
                public static final int SEPARATOR = 1;

                /**
                 * Open array specifier
                 */
                public static final int ARRAY_OPEN = 2;

                /**
                 * Close array specifier
                 */
                public static final int ARRAY_CLOSE = 3;

                /**
                 * Open struct element delimiter
                 */
                public static final int STRUCT_OPEN = 4;

                /**
                 * Close struct element delimiter
                 */
                public static final int STRUCT_CLOSE = 5;

                /**
                 * Element name assignment operator
                 */
                public static final int NAMED_ELEMENT = 6;

                /**
                 * Separator bewteen adjacent structure elements
                 */
                public static final int STRUCT_SEPARATOR = 7;

                /**
                 * Plus sign
                 */
                public static final int MINUS = 8;

                /**
                 * Minus sign also bounds separator
                 */
                public static final int PLUS = 9;

                /**
                 * open simple expression
                 */
                public static final int OPEN_PAREN = 10;

                /**
                 * close simple expression
                 */
                public static final int CLOSE_PAREN = 11;

                /**
                 * Length of element
                 */
                public static final int KEYWORD_LEN = 12;

                /**
                 * Type of element
                 */
                public static final int KEYWORD_TYPE = 13;

                /**
                 * Name of element
                 */
                public static final int KEYWORD_NAME = 14;
        }

        /**
         * This token's actual token code
         */
        public int token;

        /**
         * This token's actual string value
         */
        public String value;

        /**
         * Creates a new instance of ASQToken
         */
        public AQSToken()
        {
                token = Token.EOF;
                value = null;
        }

        /**
         * Gets the next token from the given string buffer.
         * 
         * @param theBuffer
         *        The string buffer to tokenize
         * @return Token.EOF: The buffer has been exhausted Token.code: The
         *         token code corresponding to the first recognised token in the
         *         given buffer Token.STRING: Othewise the initial portion of
         *         the given buffer is returned up to the first recognised token
         *         or the end of the buffer
         */
        public int getToken(String theBuffer)
        {
                // For all significant problems (not EOF)
                if (theBuffer.length() == 0)
                        return Token.EOF;

                // If you have eliminated the possibility of any known tokens
                for (int i = 0, len = Symbols.length; i < len; i++)
                        if (theBuffer.startsWith(Symbols[i]))
                        {
                                value = Symbols[i];
                                return (token = Tokens[i]);
                        }

                // Whatever is left is a String (however improbable)
                return (token = Token.STRING); // value set from buffer by
                                               // caller
        }

        /**
         * Get the string value of given particular token code
         * 
         * @return String value of this token. The fixed string value of the
         *         AIDA lexical symbol.
         * @param theToken
         *        A valid Token code
         */
        public static String getValue(int theToken)
        {
                try
                {
                        return (Symbols[theToken - 1]);
                }
                catch (Exception ex)
                {
                        throw (IllegalArgumentException) Err.getInstance().log(
                                new IllegalArgumentException("Invalid Syntax in query").
                                initCause(ex));
                }
        }

        /**
         * Get the string value of a this token
         * 
         * @return String value of this token. Either the fixed string value of
         *         the AIDA lexical symbol or the string found in the input
         *         stream corresponding to the Token.STRING.
         */
        public String getValue()
        {
                return value;
        }

}