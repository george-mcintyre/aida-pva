/*
 * @file
 */
package edu.stanford.slac.except;

/**
 * Server initialisation error.
 * User this when there is an error during the server initialisation phase.
 * This is a fatal error and the service should stop when this exception is received.
 */
public class ServerInitialisationException extends RuntimeException {
    /**
     * No Args constructor.
     * Use this constructor to raise an exception without extra information.
     * Do not use this constructor unless it can't be helped.
     */
    public ServerInitialisationException() {
    }

    /***
     * Constructor with a message.
     * Use this constructor to create an exception that will indicate the problem using your message.
     * @param message A message explaining what the problem was
     */
    public ServerInitialisationException(String message) {
        super(message);
    }

    /**
     * Constructor with a message and the cause.
     * Use this constructor if you've caught an exception and need to rethrow it with an extra message of your own.
     *
     * @param message A message explaining what the problem was
     * @param cause   the exception that caused this problem
     */
    public ServerInitialisationException(String message, Throwable cause) {
        super(message, cause);
    }
}
