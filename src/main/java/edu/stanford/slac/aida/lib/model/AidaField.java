package edu.stanford.slac.aida.lib.model;

import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.NonNull;

/**
 * This class encapsulates an {@link AidaField}.
 * <p>
 * It is used to represent a single field in {@link AidaChannelConfig#fields} that defines the configuration
 * of the ordinal TABLE column to which it is related by ordinal position in the list.
 * <p>
 * Note that it uses the {@link lombok.Data} annotation to provide all the getters and setters.
 * a constructor with all required arguments,
 * and an equals(), hashcode() and toString() method.
 * <p>
 * It also uses the {@link lombok.NoArgsConstructor} annotation to provide a constructor
 * with no arguments.
 * <p>
 */
@Data
@NoArgsConstructor
public class AidaField {
    /**
     * TABLE's are returned as Normative Type `PVStructure`'s.  see
     * {@see  <a href="{@docRoot}/docs/NormativeTypes.md">AIDA-PVA Normative Types Documentation</a> }
     * `value` field PVStructure's subfield field name
     */
    private @NonNull String name;

    /**
     * The TABLE column PVField's name
     */
    private String label;
    private String description;
    private String units;
}
