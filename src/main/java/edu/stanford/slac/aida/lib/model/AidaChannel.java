package edu.stanford.slac.aida.lib.model;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.NonNull;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class AidaChannel {
    private @NonNull String channel;
    private AidaChannelConfig getterConfig;
    private AidaChannelConfig setterConfig;
}
