package edu.stanford.slac.aida.lib.model;

import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.NonNull;
import org.epics.pvdata.pv.PVScalarArray;
import org.epics.pvdata.pv.PVStructure;
import org.epics.pvdata.pv.ScalarArray;
import org.epics.pvdata.pv.ScalarType;

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
 */
@Data
@NoArgsConstructor
public class AidaField {
    /**
     * TABLE's are returned as Normative Type {@link PVStructure}s.
     * {@see <a href="{@docRoot}/docs/NormativeTypes.md">AIDA-PVA Normative Types Documentation</a>}
     * <p>
     * It contains a `value` field which is also a {@link PVStructure}, which contains subfields for each `TABLE` column
     * which are simple {@link ScalarArray} of the column's data.
     * <p>
     * This {@link AidaField#name} relates to the name of one of those subfields.
     */
    private @NonNull String name;

    /**
     * TABLE's are returned as Normative Type {@link PVStructure}s.
     * {@see <a href="{@docRoot}/docs/NormativeTypes.md">AIDA-PVA Normative Types Documentation</a>}
     * <p>
     * It contains a `labels` field which is a list of {@link ScalarType#pvString}, which are labels for each `TABLE` column.
     * <p>
     * This {@link AidaField#label} gives the value of the label for this column in the `TABLE`.
     */

    private String label;

    /**
     * TABLE's are returned as Normative Type {@link PVStructure}s.
     * {@see <a href="{@docRoot}/docs/NormativeTypes.md">AIDA-PVA Normative Types Documentation</a>}
     * <p>
     * But TABLEs don't contain {@link PVScalarArray} which contains a `display` subfield
     * which stores `description` and `units` for a field.  So we can't currently use the `description`
     * and `units` that are configured for TABLE columns and thus field is ignored.
     * <p>
     * In the future it will be used to set the description of the TABLE column
     */
    private String description;

    /**
     * TABLE's are returned as Normative Type `PVStructures`.
     * {@see <a href="{@docRoot}/docs/NormativeTypes.md">AIDA-PVA Normative Types Documentation</a>}
     * <p>
     * But TABLEs don't contain {@link PVScalarArray} which contains a `display` subfield
     * which stores `description` and `units` for a field.  So we can't currently use the `description`
     * and `units` that are configured for TABLE columns and thus field is ignored.
     * <p>
     * In the future it will be used to set the units of the TABLE column
     */
    private String units;
}
