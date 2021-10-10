package edu.stanford.slac.aida.lib.model;

import lombok.Data;
import lombok.RequiredArgsConstructor;

import java.util.List;

@Data
@RequiredArgsConstructor
public class AidaArgument {
    private final String name;
    private final String value;
    private final List<FloatArgument> floats;
    private final List<DoubleArgument> doubles;

    @Override
    public String toString() {
        return name + "=" + value;
    }
}
