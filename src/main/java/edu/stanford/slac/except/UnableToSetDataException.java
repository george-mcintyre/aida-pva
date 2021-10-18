package edu.stanford.slac.except;


/**
 * Unable to set data exception
 * Use this when an error occurs during a set operation
 */
public class UnableToSetDataException extends RuntimeException {
    public UnableToSetDataException() {
    }

    public UnableToSetDataException(String message) {
        super(message);
    }

    public UnableToSetDataException(String message, Throwable cause) {
        super(message, cause);
    }
}
