package edu.stanford.slac.aida.lib.model;

import lombok.Data;

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
}
