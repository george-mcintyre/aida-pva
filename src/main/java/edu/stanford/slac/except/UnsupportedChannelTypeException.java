package edu.stanford.slac.except;


public class UnsupportedChannelTypeException extends RuntimeException {
    public UnsupportedChannelTypeException() {
    }

    public UnsupportedChannelTypeException(String message) {
        super(message);
    }

    public UnsupportedChannelTypeException(String message, Throwable cause) {
        super(message, cause);
    }
}
