package edu.stanford.slac.aida.lib.model;

import lombok.Data;

/**
 * @file
 * This class encapsulates an argument that is a double precision floating point value.
 * <p>
 * Whenever arguments are passed with a channel they are all converted to string, and converted back
 * in the AIDA-PVA Module to their original types.  Floats and Doubles however are treated differently because
 * the ieee format in which Floats and Doubles are represented can, in some circumstances, hold more precision
 * than can be expressed in strings.
 * <p>
 * For this reason the original Float and Double values are transited, as-is across the EPICS network, through the
 * AIDA-PVA SERVICE and down to the AIDA-PVA Module.
 * <p>
 * For the last leg of that journey, from AIDA-PVA SERVICE to the AIDA-PVA Module, the double precision arguments are
 * encapsulated in a {@link DoubleArgument} object.
 * @note
 * It uses the `@Data` annotation to provide all the getters and setters,
 * a constructor with all required arguments,
 * and an equals(), hashcode() and toString()  method.
 */
@Data
public class DoubleArgument {
    private final String name;          ///< The argument name
    private final double value;         ///< The double precision floating point argument value
}
