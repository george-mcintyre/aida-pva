/*
 * @file
 */
package edu.stanford.slac.aida.lib.model;

import lombok.Data;

import java.util.List;

/**
 * This class encapsulates an {@link AidaArgument}.
 * <p>
 * It is used to represent a single argument passed in an AIDA-PVA request.
 * Note that is has a list of {@link FloatArgument} and
 * a list of {@link DoubleArgument}.  These are a list
 * of any `Floats` and `Doubles` found in the Arguments received in a request.  These
 * are added verbatim without translating to string as all other parameters are.
 * <p>
 * @note
 * It uses the `@Data` annotation to provide all the getters and setters,
 * a constructor with all required arguments,
 * and an equals(), hashcode() and toString()  method.
 */
@Data
public class AidaArgument {
    /**
     * The name of the AidaArgument.
     */
    private final String name;
    /**
     * The value of the AidaArgument.  This is always a string representation of the value
     * and can be a json string for complex data types.
     */
    private final String value;
    /**
     * A list floating point value of the argument
     */
    private final List<FloatArgument> floats;
    private final List<DoubleArgument> doubles;

    @Override
    public String toString() {
        return name + "=" + value;
    }
}
