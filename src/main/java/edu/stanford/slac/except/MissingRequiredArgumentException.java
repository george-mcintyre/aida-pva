/*
 * @file
 * Missing Required Argument Exception.
 */
package edu.stanford.slac.except;

/**
 * Missing Required Argument Exception.
 * Use this when a required argument is missing from the submitted request.
 * Raising this exception will notify the client that it can retry the request
 * with the missing argument.
 */
public class MissingRequiredArgumentException extends RuntimeException {
    /**
     * No Args constructor.
     * Use this constructor to raise an exception without extra information.
     */
    public MissingRequiredArgumentException() {
    }

    /***
     * Constructor with a message.
     * Use this constructor to create an exception that will indicate the problem using your message.
     * @param message A message explaining which argument was missing
     */
    public MissingRequiredArgumentException(String message) {
        super(message);
    }

    /**
     * Constructor with a message and the cause.
     * Use this constructor if you've caught an exception and need to rethrow it with an extra message of your own.
     * For example if your code is not checking parameters but the code you call raises an exception
     * that it is missing a parameter, you can use this constructor.
     *
     * @param message A message explaining which argument was missing
     * @param cause   the exception that caused this problem
     */
    public MissingRequiredArgumentException(String message, Throwable cause) {
        super(message, cause);
    }
}
