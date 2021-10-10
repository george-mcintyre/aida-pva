package edu.stanford.slac.aida.lib.model;

import lombok.Data;

import java.util.ArrayList;
import java.util.List;

@Data
public class AidaArguments {
    private final List<AidaArgument> arguments;

    /**
     * Get the list of all floats in one list
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
     * Get the list of all doubles in one list
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
