package edu.stanford.slac.except;

/**
 * @file
 * Aida Internal Exception.
 * Use this when an anomaly is detected that is independent of the request being made.
 * i.e. a programming error or an unsupported edge case.
 */
public class AidaInternalException extends RuntimeException {
    /**
     * No Args constructor.
     * Use this constructor to raise an exception without extra information.
     */
    public AidaInternalException() {
    }

    /**
     * Constructor with a message.
     * Use this constructor to create an exception that will indicate the problem using your message.
     * @param message A message explaining what the problem was
     */
    public AidaInternalException(String message) {
        super(message);
    }

    /**
     * Constructor with a message and the cause.
     * Use this constructor if you've caught an exception and need to rethrow it with an extra message of your own.
     *
     * @param message A message explaining what the problem was
     * @param cause   the exception that caused this problem
     */
    public AidaInternalException(String message, Throwable cause) {
        super(message, cause);
    }
}
