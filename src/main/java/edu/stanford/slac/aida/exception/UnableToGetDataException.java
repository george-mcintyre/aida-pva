package edu.stanford.slac.aida.exception;


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
