package edu.stanford.slac.except;


/**
 * Aida Internal Exception
 * Use this when an anomaly is detected that is independent of the request being made.
 * i.e. a programming error or an unsupported edge case.
 */
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
