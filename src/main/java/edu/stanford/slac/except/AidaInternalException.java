package edu.stanford.slac.except;


public class AidaInternalException extends RuntimeException {
    public AidaInternalException() {
    }

    public AidaInternalException(String message) {
        super(message);
    }

    public AidaInternalException(String message, Throwable cause) {
        super(message, cause);
    }
}
