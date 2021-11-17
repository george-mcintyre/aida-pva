/*
 * @file
 * Class to create string for startup banner.
 */
package edu.stanford.slac.aida.lib.util;

/**
 * Class to create string for startup banner.
 * This class has one and only one job - Create a banner to be displayed the logs.
 * <p>
 * Go here for generator to try out different banner styles if you're bored with this one :)
 * https://www.messletters.com/en/big-text/
 * <p>
 * Style used: `univers`
 */
public class AidaPva {
    /**
     * This static method will return a string banner to be displayed in the logs at startup
     *
     * @return the string banner to be displayed in the logs
     */
    public static String banner() {
        return "                                                                                                       \n" +
                "       db         88  88888888ba,           db                                                         \n" +
                "      d88b        88  88      `\"8b         d88b                                                        \n" +
                "     d8'`8b       88  88        `8b       d8'`8b                                                       \n" +
                "    d8'  `8b      88  88         88      d8'  `8b                8b,dPPYba,   8b       d8  ,adPPYYba,  \n" +
                "   d8YaaaaY8b     88  88         88     d8YaaaaY8b     aaaaaaaa  88P'    \"8a  `8b     d8'  \"\"     `Y8  \n" +
                "  d8\"\"\"\"\"\"\"\"8b    88  88         8P    d8\"\"\"\"\"\"\"\"8b    \"\"\"\"\"\"\"\"  88       d8   `8b   d8'   ,adPPPPP88  \n" +
                " d8'        `8b   88  88      .a8P    d8'        `8b             88b,   ,a8\"    `8b,d8'    88,    ,88  \n" +
                "d8'          `8b  88  88888888Y\"'    d8'          `8b            88`YbbdP\"'       \"8\"      `\"8bbdP\"Y8  \n" +
                "                                                                 88                                    \n" +
                "                                                                 88                                    ";
    }
}
