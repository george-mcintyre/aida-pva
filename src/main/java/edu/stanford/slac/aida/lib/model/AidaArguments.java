/*
 * @file
 * This class encapsulates an AidaArguments structure.
 */
package edu.stanford.slac.aida.lib.model;

import lombok.Data;

import java.util.ArrayList;
import java.util.List;

/**
 * This class encapsulates an {@link AidaArguments} structure.
 * <p>
 * It is used to represent all argument passed in an AIDA-PVA request.
 * @note
 * It uses the `@Data` annotation to provide all the getters and setters,
 * a constructor with all required arguments,
 * and an equals(), hashcode() and toString()  method.
 */
@Data
public class AidaArguments {
    /**
     * The list of arguments stored in this object
     */
    private final List<AidaArgument> arguments;
    /**
     * The list of floating point arguments in this list of arguments
     */
    private final List<FloatArgument> floatArguments = new ArrayList<FloatArgument>();
    /**
     * The list of double precision floating point arguments in this list of arguments
     */
    private final List<DoubleArgument> doubleArguments = new ArrayList<DoubleArgument>();

    /**
     * Constructor will take a list of arguments and encapsulate it in this object and will extract any of them,
     * that are floating points into the appropriate floatArguments or doubleArguments member.
     * @param arguments the list of argument to encapsulate
     */
    public AidaArguments(List<AidaArgument> arguments) {
        this.arguments = arguments;
        for (AidaArgument argument : arguments) {
            floatArguments.addAll(argument.getFloats());
            doubleArguments.addAll(argument.getDoubles());
        }
    }
}
