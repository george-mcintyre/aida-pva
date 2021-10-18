package edu.stanford.slac.except;


/**
 * Server initialisation error
 * User this when there is an error during the server initialisation phase
 */
public class ServerInitialisationException extends RuntimeException {
    public ServerInitialisationException() {
    }

    public ServerInitialisationException(String message) {
        super(message);
    }

    public ServerInitialisationException(String message, Throwable cause) {
        super(message, cause);
    }
}
