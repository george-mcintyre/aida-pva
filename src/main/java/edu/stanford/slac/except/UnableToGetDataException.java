package edu.stanford.slac.except;

/**
 * @file
 * Unable to get data exception.
 * This Exception is a general exception to be used in most cases when there's a problem satisfying a `get` request.
 * If this is a `set` request use the {@link UnableToSetDataException} exception instead.
 * <p>
 * This can be used when there are problems calling the lower level Channel Provider module.
 * It can also be used if the set of arguments given are not coherent or are out or bounds.
 * <p>
 * Use this when there is an error that occurs during a get operation
 */
public class UnableToGetDataException extends RuntimeException {
    /**
     * No Args constructor.
     * Use this constructor to raise an exception without extra information.
     * Please don't use this unless there is no alternative, add at least a simple message
     */
    public UnableToGetDataException() {
    }

    /***
     * Constructor with a message.
     * Use this constructor to create an exception that will indicate the problem using your message.
     * @param message A message explaining what the problem was
     */
    public UnableToGetDataException(String message) {
        super(message);
    }

    /**
     * Constructor with a message and the cause.
     * Use this constructor if you've caught an exception,
     * and need to rethrow it with an extra message of your own.
     *
     * @param message A message explaining what the problem was
     * @param cause   the exception that caused this problem
     */
    public UnableToGetDataException(String message, Throwable cause) {
        super(message, cause);
    }
}
