package edu.stanford.slac.aida.lib.model;


import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.ArrayList;
import java.util.List;

@Data
@NoArgsConstructor
public class AidaChannelConfig {
    private AidaType type;
    private String description;
    private AidaTableLayout layout;
    private List<AidaField> fields;

    public void setType(AidaType type) {
        this.type = type;
    }

    public void setLayout(AidaTableLayout layout) {
        this.layout = layout;
    }

    /**
     * To set type from a string
     * @param stringType type string
     */
    public void setType(String stringType) {
        this.type = AidaType.valueOf(stringType);
    }

    /**
     * To set layout from a string
     * @param stringLayout layout string
     */
    public void setLayout(String stringLayout) {
        this.layout = AidaTableLayout.valueOf(stringLayout);
    }

    /**
     * To add a field one at a time
     * @param field field to add
     */
    public void addField(AidaField field) {
        if ( this.fields == null ) {
            this.fields = new ArrayList<AidaField>();
        }
        this.fields.add(field);
    }
}
