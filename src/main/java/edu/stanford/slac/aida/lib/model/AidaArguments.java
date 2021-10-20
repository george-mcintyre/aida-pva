/*
 * @file
 * Class that encapsulates a collection of all request arguments to be sent to the Native Channel Provider endpoint.
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
    private final List<AidaArgument> arguments;
    private final List<FloatArgument> floatArguments = new ArrayList<FloatArgument>();
    private final List<DoubleArgument> doubleArguments = new ArrayList<DoubleArgument>();

    public AidaArguments(List<AidaArgument> arguments) {
        this.arguments = arguments;
        for (AidaArgument argument : arguments) {
            floatArguments.addAll(argument.getFloats());
            doubleArguments.addAll(argument.getDoubles());
        }
    }
}
