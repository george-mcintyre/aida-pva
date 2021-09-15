package edu.stanford.slac.except;


public class MissingRequiredArgumentException extends RuntimeException {
    public MissingRequiredArgumentException() {
    }

    public MissingRequiredArgumentException(String message) {
        super(message);
    }

    public MissingRequiredArgumentException(String message, Throwable cause) {
        super(message, cause);
    }
}
