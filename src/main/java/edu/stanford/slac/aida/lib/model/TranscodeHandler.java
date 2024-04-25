/*
 * @file
 * To handle transcoding within PV name strings by selected methods.
 */
package edu.stanford.slac.aida.lib.model;

import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static edu.stanford.slac.aida.lib.model.TranscodingMethod.NONE;
import static edu.stanford.slac.aida.lib.model.TranscodingMethod.FLIP;

/**
 * This class handles the transcoding of PV strings based on the given transcoding method.
 */
public class TranscodeHandler {
    /**
     * This interface represents a transcoder, which is responsible for transcoding PV
     * strings based on a specific transcoding method.
     */
    public interface Transcoder {
        String apply(final String pvName);
    }

    /**
     * Represents a map that associates each TranscodingMethod with a corresponding Transcoder.
     */
    private static final Map<TranscodingMethod, Transcoder> transcodingMethodTranscoderMap = createMap();

    /**
     * This method creates a map that associates each TranscodingMethod with a corresponding Transcoder.
     *
     * @return A map that associates each TranscodingMethod with a corresponding Transcoder.
     */
    private static Map<TranscodingMethod, Transcoder> createMap() {
        final Map<TranscodingMethod, Transcoder> transcodingMethodTranscoderMap = new HashMap<TranscodingMethod, Transcoder>();

        // The null transcoding method that does no transcoding at all, final should make optimiser create a noop
        transcodingMethodTranscoderMap.put(NONE, new Transcoder() {
            public String apply(final String pvName) {
                return pvName;
            }
        });

        // The flip transformation method.  This is an involution function, in that it can be called to make the
        // transcoding in both directions.  i.e. when registering the names and when handling requests, call the same
        // function.
        //
        // 1. If the given pvName does not contain the pattern `{primary}:{micro}:{unit}` then return pvName unchanged
        // 2. If the string starts with the pattern `{prefix}::` then return that part of the string unchanged
        //    but continue with the remaining transcoding on the rest of the string.
        // 3. Extract the {primary} and {micro} parts of the string and transpose their positions to create a
        //    new string and return this string, prepended by the prefix pattern if one was found.
        transcodingMethodTranscoderMap.put(FLIP, new Transcoder() {
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

                        // Create the new transcoded string
                        return prefix + micro + ":" + primary + ":" + unit + remainder;
                    } else {
                        // If pvName does not match the patterns, return it unchanged.
                        return pvName;
                    }
                }
        });

        return transcodingMethodTranscoderMap;
    }

    public static String transcode(String pvName, TranscodingMethod method) {
        return transcodingMethodTranscoderMap.get(method).apply(pvName);
    }
}
