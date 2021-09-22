package edu.stanford.slac.aida.lib.model;


import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.ArrayList;
import java.util.List;

import static edu.stanford.slac.aida.lib.model.AidaTableLayout.COLUMN_MAJOR;
import static edu.stanford.slac.aida.lib.model.AidaType.STRING;

/**
 * The channel config class controls how the provider responds to a channel request.
 * There is a channel getterConfig for the get requests (getter) and a setterConfig for the set requests (setter)
 * All setter requests are differentiated by containing a VALUE argument.
 * The default setterConfig is NONE meaning that no setter is possible.  To enable setters it must be set
 * to VOID, or any other type which will be returned.
 *
 * A request that contains the VALUE parameter is sent to the setter, otherwise it is sent to the getter.
 *
 * Type:
 *   - If the type is set to ANY then it uses the supplied TYPE parameter in the request to determine which end point to send
 *     requests to.  e.g. if TYPE=SHORT then requests are sent to aidaRequestShort().
 *   - If the type is set to NONE then no getter requests are supported for the channel
 * Fields:
 *   - If Type is TABLE then Fields contains the fields in the returned structure.  Note that the
 *     provider may only return tables of a single configuration for any one channel.  The table
 *     configuration cannot change with different parameters.  The type of data returned in a column
 *     in a table MAY change based on parameters because the table configuration is type agnostic!
 * Layout and Fields:
 *  - These fields control the table return type.  They describe whether the table will be ROW_MAJOR or COLUMN_MAJOR
 *    in its orientation.  They also describe each of the fields in the table, labels, descriptions and names.
 *
 */
@Data
@NoArgsConstructor
public class AidaChannelConfig {
    private AidaType type;
    private String description;
    private AidaTableLayout layout;
    private List<AidaField> fields;

    /**
     * To set type from a string
     *
     * @param stringType type string
     */
    public void setType(String stringType) {
        if (stringType == null) {
            this.type = STRING;
        } else {
            this.type = AidaType.valueOf(stringType);
        }
    }

    /**
     * To set layout from a string
     *
     * @param stringLayout layout string
     */
    public void setLayout(String stringLayout) {
        if (stringLayout == null) {
            this.layout = COLUMN_MAJOR;
        } else {
            this.layout = AidaTableLayout.valueOf(stringLayout);
        }
    }

    /**
     * To add a field one at a time
     *
     * @param field field to add
     */
    public void addField(AidaField field) {
        if (this.fields == null) {
            this.fields = new ArrayList<AidaField>();
        }
        this.fields.add(field);
    }
}
