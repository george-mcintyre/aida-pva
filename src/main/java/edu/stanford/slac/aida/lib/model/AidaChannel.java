package edu.stanford.slac.aida.lib.model;

import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.NonNull;

@Data
@NoArgsConstructor
public class AidaChannel {
    private @NonNull String channel;
    private AidaChannelConfig getterConfig;
    private AidaChannelConfig setterConfig;
}
