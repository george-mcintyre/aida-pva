package edu.stanford.slac.aida.lib.model;


import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.List;

@Data
@NoArgsConstructor
public class AidaChannelConfig {
    private AidaType type;
    private String description;
    private AidaTableLayout layout;
    private List<AidaField> fields;
}
