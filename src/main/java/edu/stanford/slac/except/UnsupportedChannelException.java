package edu.stanford.slac.except;


public class UnsupportedChannelException extends RuntimeException {
    public UnsupportedChannelException() {
    }

    public UnsupportedChannelException(String message) {
        super(message);
    }

    public UnsupportedChannelException(String message, Throwable cause) {
        super(message, cause);
    }
}
