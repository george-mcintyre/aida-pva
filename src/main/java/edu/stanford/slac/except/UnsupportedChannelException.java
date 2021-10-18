package edu.stanford.slac.except;


/**
 * Unsupported Channel Exception
 * Use this when the channel requested is registered with this service but the way it is accessed
 * is incorrect.
 * <p>
 * 1) The channel is within the namespace of channels served by this provider but support is not yet implemented
 * 2) The get/set operation is not supported,
 * 3) etc
 */
public class UnsupportedChannelException extends RuntimeException {
    public UnsupportedChannelException() {
    }

    public UnsupportedChannelException(String message) {
        super(message);
    }

    public UnsupportedChannelException(String message, Throwable cause) {
        super(message, cause);
    }
}
