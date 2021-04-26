/*
 * TestParser.java
 *
 * Created on 20 July 2002, 19:36
 */

package test.java;

import edu.stanford.slac.aida.lib.da.*;
import edu.stanford.slac.err.*;   // Err singleton

/**
 * 
 * @author gsm
 * @version 28-Sep-2004 greg: Add catch from DaReference constructor for
 * UndefinedNameException, now thrown if name does not parse successfully.
 */
public class TestParser
{
        private String m_AQS;
        private static Err err_;
        

        /** Creates a new instance of TestParser */
        public TestParser(String theArgument)
        {
                m_AQS = theArgument;
        }

        /**
         * @param args
         *        the command line arguments
         */
        public static void main(String[] args)
        {
                err_ = Err.getInstance("TestParser");
                System.out.println("TEST PARSER");

                if (args.length == 0)
                {
                        System.out.println("Required parameter not specified");
                        System.out.println("Usage: java TestParser <aidaquerystring>");
                        System.exit(1);
                }

                System.out.println("Query String: " + args[0]);

                TestParser tp = new TestParser(args[0]);
                tp.test();
                System.exit(0);
        }

        public void test()
        {
                DaReference r = null;
                try
                {
                        r = new DaReference(m_AQS);
                        System.out.println(r.toString());
                } 
                catch (Exception ex)
                {
                        System.err.println(ex.getMessage() + " from DaReference(" + m_AQS + ")");
                }
        }

}