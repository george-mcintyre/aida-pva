package edu.stanford.slac.aida.lib.model;

import lombok.Data;

@Data
public class AidaArgument {
    private final String name;
    private final Object value;
}