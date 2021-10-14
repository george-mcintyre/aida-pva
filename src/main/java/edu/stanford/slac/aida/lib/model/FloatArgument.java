package edu.stanford.slac.aida.lib.model;

import lombok.Data;

/**
 * This class encapsulates an {@link AidaConfigGroup}.
 * <p>
 * It is used to represent the configuration that will apply to a single group of {@link AidaChannel}s.
 * <p>
 * The {@link AidaConfigGroup} class controls how the AIDA-PVA Channel Provider responds to a channel request.
 * <p>
 * There is a {@link AidaConfigGroup#getterConfig} for the `get` requests,
 * and a {@link AidaConfigGroup#setterConfig} for the `set` requests.
 * <p>
 * The List of {@link AidaConfigGroup#channels} are the names of the channels that will use the specified config
 * <p>
 * Note that it uses the {@link lombok.Data} annotation to provide all the getters and setters.
 * a constructor with all required arguments,
 * and an equals(), hashcode() and toString()  method.
 * <p>
 * It also uses the {@link lombok.NoArgsConstructor} annotation to provide a constructor
 * with no arguments.
 * <p>
 */
@Data
public class FloatArgument {
    private final String name;
    private final float value;
}
