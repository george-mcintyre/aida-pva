package edu.stanford.slac.except;


/**
 * Unsupported Channel Type Exception
 * Use this when the channel requested is registered with this service but a required TYPE parameter is missing
 * or incorrect.
 * <p>
 * 1) The channel is supported but requires a specific set TYPE parameters, one of which was not specified with the request
 * 2) The TYPE that is sent with the request is not supported
 * 3) etc
 */
public class UnsupportedChannelTypeException extends RuntimeException {
    public UnsupportedChannelTypeException() {
    }

    public UnsupportedChannelTypeException(String message) {
        super(message);
    }

    public UnsupportedChannelTypeException(String message, Throwable cause) {
        super(message, cause);
    }
}
