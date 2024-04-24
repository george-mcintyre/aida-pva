/*
 * @file
 * To handle transposition within PV name strings by selected methods.
 */
package edu.stanford.slac.aida.lib.model;

import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static edu.stanford.slac.aida.lib.model.TransposeMethod.NONE;
import static edu.stanford.slac.aida.lib.model.TransposeMethod.FLIP;

/**
 * This class handles the transposition of PV strings based on the given transpose method.
 */
public class TransposeHandler {
    /**
     * This interface represents a transposer, which is responsible for transposing PV
     * strings based on a specific transpose method.
     */
    public interface Transposer {
        String apply(final String pvName);
    }

    /**
     * Represents a map that associates each TransposeMethod with a corresponding Transposer.
     */
    private static final Map<TransposeMethod, Transposer> transposeMethodTransposerMap = createMap();

    /**
     * This method creates a map that associates each TransposeMethod with a corresponding Transposer.
     *
     * @return A map that associates each TransposeMethod with a corresponding Transposer.
     */
    private static Map<TransposeMethod, Transposer> createMap() {
        final Map<TransposeMethod, Transposer> transposeMethodTransposerMap = new HashMap<TransposeMethod, Transposer>();

        // The null transpose method that does no transposition at all, final should make optimiser create a noop
        transposeMethodTransposerMap.put(NONE, new Transposer() {
            public String apply(final String pvName) {
                return pvName;
            }
        });

        // The flip transformation method.  This is an involution function, in that it can be called to make the
        // transposition in both directions.  i.e. when registering the names and when handling requests, call the same
        // function.
        //
        // 1. If the given pvName does not contain the pattern `{primary}:{micro}:{unit}` then return pvName unchanged
        // 2. If the string starts with the pattern `{prefix}::` then return that part of the string unchanged
        //    but continue with the remaining transposition on the rest of the string.
        // 3. Extract the {primary} and {micro} parts of the string and transpose their positions to create a
        //    new string and return this string, prepended by the prefix pattern if one was found.
        transposeMethodTransposerMap.put(FLIP, new Transposer() {
            public String apply(final String pvName) {

                    // Pattern for matching {prefix}::{primary}:{micro}:{unit} or {prefix}::{primary}:{micro}:{unit} followed by any characters
                    final Pattern pattern = Pattern.compile("^(.*::)?(.*):(.*):([^:]*)(:.*)?$");
                    final Matcher matcher = pattern.matcher(pvName);

                    // If pvName has the format {prefix}::{primary}:{micro}:{unit} or {prefix}::{primary}:{micro}:{unit} followed by any characters
                    if (matcher.matches()) {
                        final String prefix = matcher.group(1) != null ? matcher.group(1) : ""; // group 1 is the prefix
                        final String primary = matcher.group(2); // group 2 is the primary
                        final String micro = matcher.group(3); // group 3 is the micro
                        final String unit = matcher.group(4); // group 4 is the unit
                        final String remainder = matcher.group(5) != null ? matcher.group(5) : ""; // group 5 is the remainder of the pv name

                        // Create the new transposed string
                        return prefix + micro + ":" + primary + ":" + unit + remainder;
                    } else {
                        // If pvName does not match the patterns, return it unchanged.
                        return pvName;
                    }
                }
        });

        return transposeMethodTransposerMap;
    }

    public static String transpose(String pvName, TransposeMethod method) {
        return transposeMethodTransposerMap.get(method).apply(pvName);
    }
}
