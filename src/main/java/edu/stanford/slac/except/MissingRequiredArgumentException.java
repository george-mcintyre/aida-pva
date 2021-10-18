package edu.stanford.slac.except;


/**
 * Missing Required Argument Exception.
 * Use this when a required argument is missing from the submitted request
 */
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
