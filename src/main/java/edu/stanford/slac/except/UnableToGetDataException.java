package edu.stanford.slac.except;


/**
 * Unable to get data exception
 * Use this when there is an error that occurs during a get operation
 */
public class UnableToGetDataException extends RuntimeException {
    public UnableToGetDataException() {
    }

    public UnableToGetDataException(String message) {
        super(message);
    }

    public UnableToGetDataException(String message, Throwable cause) {
        super(message, cause);
    }
}
