package edu.stanford.slac.aida.lib.model;


import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.ArrayList;
import java.util.List;

import static edu.stanford.slac.aida.lib.model.AidaTableLayout.COLUMN_MAJOR;
import static edu.stanford.slac.aida.lib.model.AidaType.STRING;

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
        if ( stringType == null ) {
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
        if ( stringLayout == null ) {
            this.layout =COLUMN_MAJOR;
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
