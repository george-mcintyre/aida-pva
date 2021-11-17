/*
 * @file
 * Unsupported Channel Type Exception.
 */
package edu.stanford.slac.except;

/**
 * Unsupported Channel Type Exception.
 * Use this when the channel requested is registered with this service but a required `TYPE` parameter is missing
 * or incorrect.
 * <p>
 * 1) The channel is supported but requires a specific set `TYPE` parameters, one of which was not specified with the request
 * 2) The `TYPE` that is sent with the request is not supported
 * 3) etc
 */
public class UnsupportedChannelTypeException extends RuntimeException {
    /**
     * No Args constructor.
     * Use this constructor to raise an exception without extra information.
     * Please don't use this unless there is no alternative, add at least a simple message
     */
    public UnsupportedChannelTypeException() {
    }

    /***
     * Constructor with a message.
     * Use this constructor to create an exception that will indicate the problem using your message.
     * @param message A message explaining what the problem was
     */
    public UnsupportedChannelTypeException(String message) {
        super(message);
    }

    /**
     * Constructor with a message and the cause.
     * Use this constructor if you've caught an exception,
     * and need to rethrow it with an extra message of your own.
     *
     * @param message A message explaining what the problem was
     * @param cause   the exception that caused this problem
     */
    public UnsupportedChannelTypeException(String message, Throwable cause) {
        super(message, cause);
    }
}
