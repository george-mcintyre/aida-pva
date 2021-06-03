package edu.stanford.slac.aida.lib.model;

import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.NonNull;

@Data
@NoArgsConstructor
public class AidaField {
    private @NonNull String name;
    private String label;
    private String description;
    private String units;
}
