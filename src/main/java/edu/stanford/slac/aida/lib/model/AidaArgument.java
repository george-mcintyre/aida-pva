package edu.stanford.slac.aida.lib.model;

import lombok.Data;
import lombok.RequiredArgsConstructor;

import java.util.HashMap;
import java.util.Map;

@Data
@RequiredArgsConstructor
public class AidaArgument {
    private final String name;
    private final String value;
    private final Map<String, Float> floats = new HashMap<String, Float>();
    private final Map<String, Double> doubles = new HashMap<String, Double>();

    @Override
    public String toString() {
        return name + "=" + value;
    }
}
