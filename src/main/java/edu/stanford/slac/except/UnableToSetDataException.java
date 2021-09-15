package edu.stanford.slac.except;


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
