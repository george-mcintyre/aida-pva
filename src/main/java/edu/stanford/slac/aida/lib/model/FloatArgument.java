/*
 * @file
 */
package edu.stanford.slac.aida.lib.model;

import lombok.Data;

/**
 * This class encapsulates an {@link FloatArgument}.
 * <p>
 * Whenever arguments are passed with a channel request some they are all converted to string and converted back
 * in the Native Provider to their original types.  Floats and Doubles however are treated differently because
 * the ieee format in which Floats and Doubles are represented can, in some circumstances, hold more precision
 * than can be expressed in strings.
 * <p>
 * For this reason the original Float and Double values are transited, as-is across the EPICS network, through AIDA-PVA
 * and down to the Native Provider.
 * <p>
 * For the last leg of that journey, from AIDA-PVA to the Native Provider, the float arguments are
 * encapsulated in a {@link FloatArgument} object.
 * @note
 * It uses the `@Data` annotation to provide all the getters and setters,
 * a constructor with all required arguments,
 * and an equals(), hashcode() and toString()  method.
 */
@Data
public class FloatArgument {
    /**
     * The argument name
     */
    private final String name;
    /**
     * The floating point argument value
     */
    private final float value;
}
