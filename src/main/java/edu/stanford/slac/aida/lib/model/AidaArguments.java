package edu.stanford.slac.aida.lib.model;

import lombok.Data;

import java.util.ArrayList;
import java.util.List;

/**
 * This class encapsulates an {@link AidaArguments} structure.
 * <p>
 * It is used to represent all argument passed in an AIDA-PVA request.
 * <p>
 * Note that it uses the {@link lombok.Data} annotation to provide all the getters and setters.
 * a constructor with all required arguments,
 * and an equals(), hashcode() and toString()  method.
 */
@Data
public class AidaArguments {
    private final List<AidaArgument> arguments;

    /**
     * Get the list of all {@link FloatArgument} in one list
     * This is only called from the Native Channel Provider.  So don't remove it even though it's not called from Java
     *
     * @return consolidated list of all float arguments
     */
    public List<FloatArgument> getFloatArguments() {
        List<FloatArgument> floatArguments = new ArrayList<FloatArgument>();
        for (AidaArgument argument : arguments) {
            floatArguments.addAll(argument.getFloats());
        }
        return floatArguments;
    }

    /**
     * Get the list of all {@link DoubleArgument} in one list
     * This is only called from the Native Channel Provider.  So don't remove it even though it's not called from Java
     *
     * @return consolidated list of all double arguments
     */
    public List<DoubleArgument> getDoubleArguments() {
        List<DoubleArgument> doubleArguments = new ArrayList<DoubleArgument>();
        for (AidaArgument argument : arguments) {
            doubleArguments.addAll(argument.getDoubles());
        }
        return doubleArguments;
    }
}
