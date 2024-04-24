/*
 * @file
 * This class encapsulates the permissible values for the transpose element in a channel configuration file.
 */
package edu.stanford.slac.aida.lib.model;

import com.fasterxml.jackson.annotation.JsonProperty;

/*
 * @file
 * This class encapsulates the permissible values for the transpose element in a channel configuration file.
 * <p>
 * The transpose element selects the type of transposition that is required for PVAs that are submitted to AIDA-PVA.
 * It defaults to no transposition
 */
public enum TransposeMethod {
    @JsonProperty("none")
    NONE,

    @JsonProperty("flip")
    FLIP
}
