/*
 * @file
 * This class encapsulates the permissible values for the transcode element in a channel configuration file.
 */
package edu.stanford.slac.aida.lib.model;

import com.fasterxml.jackson.annotation.JsonProperty;

/*
 * @file
 * This class encapsulates the permissible values for the transcode element in a channel configuration file.
 * <p>
 * The transcode element selects the type of transcoding that is required for PVAs that are submitted to AIDA-PVA.
 * It defaults to no transcoding
 */
public enum TranscodingMethod {
    @JsonProperty("none")
    NONE,

    @JsonProperty("flip")
    FLIP
}
