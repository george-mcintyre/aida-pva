package edu.stanford.slac.aida.lib.model;

import lombok.Data;

import java.util.List;

/**
 * This class encapsulates an {@link AidaArgument}.
 * <p>
 * It is used to represent a single argument passed in an AIDA-PVA request.
 * Note that is has a list of {@link FloatArgument} and
 * a list of {@link DoubleArgument}.  These are a list
 * of any Floats and Doubles found in the arguments received in the request.  These
 * are added verbatim without translating to string as all other parameters are.
 * <p>
 * Note that it uses the {@link lombok.Data} annotation to provide all the getters and setters.
 * a constructor with all required arguments,
 * and an equals(), hashcode() and toString()  method.
 */
@Data
public class AidaArgument {
    private final String name;
    private final String value;
    private final List<FloatArgument> floats;
    private final List<DoubleArgument> doubles;

    @Override
    public String toString() {
        return name + "=" + value;
    }
}
