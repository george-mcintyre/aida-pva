package edu.stanford.slac.aida.lib.model;

import lombok.Data;

/**
 * This class encapsulates a {@link DoubleArgument}.
 * <p>
 * Whenever arguments are passed with a channel request some they are all converted to strings and converted back
 * in the Native Provider to their original types.  Floats and Doubles however are treated differently because
 * the ieee format in which Floats and Doubles are represented can, in some circumstances, hold more precision
 * than can be expressed in strings.
 * <p>
 * For this reason the original Float and Double values are transited, as-is across the EPICS network, through AIDA-PVA
 * and down to the Native Provider.
 * <p>
 * For the last leg of that journey, from AIDA-PVA to the Native Provider the double precision arguments are
 * encapsulated in a {@link DoubleArgument} object.
 * <p>
 * Note that it uses the {@link Data} annotation to provide all the getters and setters.
 * a constructor with all required arguments,
 * and an equals(), hashcode() and toString()  method.
 */
@Data
public class DoubleArgument {
    private final String name;          ///< The argument name
    private final double value;         ///< The double precision floating point argument value
}
