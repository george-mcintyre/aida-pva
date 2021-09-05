package edu.stanford.slac.aida.exception;


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
