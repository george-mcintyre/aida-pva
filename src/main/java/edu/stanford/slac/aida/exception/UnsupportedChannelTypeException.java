package edu.stanford.slac.aida.exception;


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
