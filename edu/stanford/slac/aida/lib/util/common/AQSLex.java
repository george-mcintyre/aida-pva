/*
 * AQSLex.java
 *
 * Created on 17 July 2002, 13:14
 */

package edu.stanford.slac.aida.lib.util.common;

import java.util.*;

/**
 * Analyses AQS (AIDA Query Strings) and returns AQSTokens corresponding to the characters
 * read. Succesive calls to the getToken() method will return the next token mark() and
 * rollback(mark) are supported with the respective methods unGetToken() rollsback to the
 * last token
 * 
 * @author George McIntyre
 * @see AQSToken
 * @version 26-Feb-2007, Bob Hall (rdh): Added new method getInstanceToken.
 */
public class AQSLex
{
        /**
         * Array of string values that are not recognised AQS terminal symbols
         */
        private String[] m_StringValues;

        /**
         * Index of last used string Value
         * 
         * @see m_stringValues
         */
        private int stringValueIndex;

        /**
         * The original given target string (AQS).
         */
        private String m_target;

        /**
         * Internal stack. Stores lookahead stack (tokens that have already been read
         * relative to the current parse context). Each token that is "unGot" is pushed to
         * this stack so that if a new token is required this is the first place to look.
         * 
         * @see forwardStack
         */
        private Stack stack;

        /**
         * Internal Stack. Stores all tokens that have already been output relative to the
         * current parse position. If one needs to go back in time, here is a good place
         * to start. To go back to a marked position simply pop values here (placing them
         * into the lookahead stack) until you get to the desired position.
         * 
         * @see stack
         */
        private Stack forwardStack;

        /**
         * Creates a new instance of AQSLex AQS lexical analyser
         * 
         * @param target
         *        AQS target string specification to be lexically analysed
         */
        public AQSLex(String target)
        {
                m_target = target;
                m_StringValues = m_target.split(createAQSRegex());

                // FUDGE: To overcome problems of regex matching
                // Manually remove the extraneous '(' and ')' from the string
                // list
                int i, p;
                for (i = 0; i < m_StringValues.length; i++)
                        if ((p = m_StringValues[i].indexOf('(')) != -1
                                || (p = m_StringValues[i].indexOf(')')) != -1)
                                m_StringValues[i] = m_StringValues[i].substring(0, p);

                stringValueIndex = 0;
                stack = new Stack();
                forwardStack = new Stack();
        }

        /**
         * Gets the next Token until end of target for instance names.  This method
         * was created to allow instance names that have non-leading parsing tokens,
         * such as "+" and "-".
         * 
         * @see AQSToken
         * @return AQSToken object containing the next token parsed from the target string
         *         or pulled from the lookahead stack
         */
        public AQSToken getInstanceToken()
        {
            String instanceStr = "";

            AQSToken aToken;

            //
            // Invoke the getToken method to get the next token, which is
            // expected to be a STRING.
            //
            aToken = getToken();
            if (aToken.token != AQSToken.Token.STRING)
            {
                return aToken; 
            }

            //
            // Loop while the next token is a STRING, the "+" character,
            // or the "-" character.  If a new token was found that was
            // one of these, concatenate its token characters to the
            // instanceStr String.
            //
            while ((aToken.token == AQSToken.Token.STRING) ||
                   (aToken.token == AQSToken.Token.PLUS) ||
                   (aToken.token == AQSToken.Token.MINUS) )
            {
                instanceStr = instanceStr + aToken.value;

                aToken = getToken();
            }

            //
            // After the loop, one more token was read that was not
            // part of the instance name.  Invoke method unGetToken
            // to put this token back on "stack" so it can be used
            // when getting the next token.
            //
            unGetToken();

            // 
            // Return an AQSToken object whose String value is the
            // instance name with possible non-leading "+" or "-"
            // characters.
            //
            AQSToken retToken = new AQSToken();

            retToken.token = AQSToken.Token.STRING;
            retToken.value = instanceStr;

            return retToken; 
        }

        /**
         * Gets the next Token until end of target.
         * 
         * @see AQSToken
         * @return AQSToken object containing the next token parsed from the target string
         *         or pulled from the lookahead stack
         */
        public AQSToken getToken()
        {
                AQSToken aToken;
                if (stack.size() == 0)
                {
                        aToken = new AQSToken();
                        aToken.token = AQSToken.Token.EOF;

                        if (m_target.length() > 0)
                        {
                                if (aToken.getToken(m_target) == AQSToken.Token.STRING)
                                        aToken.value = getNextString();
                                if (aToken.token != AQSToken.Token.EOF)
                                        m_target = m_target.substring(aToken.getValue()
                                                .length());
                        }
                } else
                        aToken = (AQSToken) stack.pop();

                return ((AQSToken) forwardStack.push(aToken));
        }

        /**
         * Get next string from string value list. Ignores blank strings created by the
         * juxtaposition of two non string tokens in the target string. This method has
         * knowledge of the method used to create the stringValue list and so acts
         * accordingly to always return the next valid string value from the stringValue
         * list.
         * 
         * @return Next valid string from stringValues. Skip over null strings. Assumes
         *         that will only be called where there are strings to be read. Will cause
         *         an out of bounds exception if too many strings are read from the
         *         stringValues array.
         *  
         */
        private String getNextString()
        {
                int len = m_StringValues.length;
                while ((stringValueIndex < len)
                        && (m_StringValues[stringValueIndex++].length() == 0))
                        ;
                return (m_StringValues[stringValueIndex - 1]);
        }

        /**
         * Unget token. Pushes the last token onto the readahead stack and continues.
         * 
         * @see stack
         */
        public void unGetToken()
        {
                stack.push(forwardStack.pop());
        }

        /**
         * Mark current point in input stream for latter rollback.
         * 
         * @return The mark ID. This is the ID to be passed to the rollback function to
         *         rollback the context to this point. In this implementation it is simply
         *         the size of the forwardStack.
         * @see stack
         * @see forwardStack
         */
        public int mark()
        {
                return (forwardStack.size());
        }

        /**
         * Rollback to given mark.
         * 
         * @param mark
         *        Mark ID. This is a valid mark identifier returned from a call to mark().
         *        It is used to locate the desired rollback context.
         */
        public void rollback(int mark)
        {
                while (!forwardStack.isEmpty() && forwardStack.size() != mark)
                        unGetToken();
        }

        /**
         * Internal method to create a regular expression that will match any AQS token.
         * This is made by concatenating all AQS tokens and backslashing any characters
         * that have special meaning to regex. It is used in implementations of AQSLex
         * where a regex evaluator (such as the split method of Java's String class) is
         * used to preparse the input string into tokens.
         * 
         * @return A regular expression that will match any AQS token. This is made by
         *         concatenating all AQS tokens and backslashing any characters that have
         *         special meaning to regex
         * @see Symbols
         */
        private String createAQSRegex()
        {
                String theAQSRegex = "";
                boolean first = true;

                for (int i = 0, len = AQSToken.Symbols.length; i < len; i++)
                {
                        if (first)
                                first = false;
                        else
                                theAQSRegex += "|";

                        theAQSRegex += backslash(AQSToken.Symbols[i]);
                }

                return theAQSRegex;
        }

        /**
         * Backslashes characters in the token String that have special meaning to regex.
         * Double backslashes real problem children. Used in conjunction
         * with createAQSRegex().
         * 
         * @param tokenString
         *        A string token to be backslashed if necessary.
         * @return The properly backslashed token string
         * @see createAQSRegex
         */
        private String backslash(String tokenString)
        {
                String backslashedString = "";
                char c;

                for (int i = 0, len = tokenString.length(); i < len; i++)
                {
                        c = tokenString.charAt(i);

                        // SINGLE BACKSLASH
                        switch (c)
                        {
                        // Characters that have special meaning to regex
                        case '\\':
                        case '{':
                        case '}':
                        case '+':
                        case '-':
                        case '[':
                        case ']':
                        case '*':
                        case '.':
                        case '$':
                        case '^':
                        case '#':
                                backslashedString += '\\'; // This is a
                                // backslash
                                // character
                                break;
                        }

                        // DOUBLE BACKSLASH (problem child)
                        // Still does not work. So I cheat and manually remove
                        // '(' and ')' from m_stringValues
                        switch (c)
                        {
                        // Characters that have special meaning to regex
                        case '(':
                        case ')':
                                backslashedString += '\\'; // This is a
                                // backslash
                                // character
                                backslashedString += '\\'; // This is a
                                // backslash for the
                                // backslash
                                // character
                                backslashedString += '\\'; // This is a
                                // backslash for the
                                // character itself
                                break;
                        }

                        backslashedString += c;
                }

                return backslashedString;
        }
}
