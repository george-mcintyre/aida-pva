/*
 * @file
 * Unsupported Channel Exception.
 * Use this when the channel requested is registered with this service but the way it is accessed
 * is incorrect.
 */
package edu.stanford.slac.except;

/**
 * Unsupported Channel Exception.
 * Use this when the channel requested is registered with this service but the way it is accessed
 * is incorrect.
 * <p>
 * 1) The channel is within the namespace of channels served by this provider but support is not yet implemented
 * 2) The get/set operation is not supported,
 * 3) etc
 */
public class UnsupportedChannelException extends RuntimeException {
    /**
     * No Args constructor.
     * Use this constructor to raise an exception without extra information.
     */
    public UnsupportedChannelException() {
    }

    /***
     * Constructor with a message.
     * Use this constructor to create an exception that will indicate the problem using your message.
     * @param message A message explaining what the problem was
     */
    public UnsupportedChannelException(String message) {
        super(message);
    }

    /**
     * Constructor with a message and the cause.
     * Use this constructor if you've caught an exception and need to rethrow it with an extra message of your own.
     *
     * @param message A message explaining what the problem was
     * @param cause   the exception that caused this problem
     */
    public UnsupportedChannelException(String message, Throwable cause) {
        super(message, cause);
    }
}
