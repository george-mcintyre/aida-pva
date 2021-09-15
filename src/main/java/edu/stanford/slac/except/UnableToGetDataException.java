package edu.stanford.slac.except;


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
