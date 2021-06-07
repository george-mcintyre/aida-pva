package edu.stanford.slac.aida.lib.model;

import java.util.List;

public enum AidaType {
    BOOLEAN,
    BYTE,
    SHORT,
    INTEGER,
    LONG,
    FLOAT,
    DOUBLE,
    STRING,
    BOOLEAN_ARRAY,
    BYTE_ARRAY,
    SHORT_ARRAY,
    INTEGER_ARRAY,
    LONG_ARRAY,
    FLOAT_ARRAY,
    DOUBLE_ARRAY,
    STRING_ARRAY,
    TABLE;

    /**
     * Determine the aida type of a java object of a supported type.  Return null if type not supported
     *
     * @param value java object to check
     * @return the aida type
     */
    public static AidaType aidaTypeOf(Object value) {
        if (value instanceof Boolean) {
            return BOOLEAN;
        }
        if (value instanceof Byte) {
            return BYTE;
        }
        if (value instanceof Short) {
            return SHORT;
        }
        if (value instanceof Integer) {
            return INTEGER;
        }
        if (value instanceof Long) {
            return LONG;
        }
        if (value instanceof Float) {
            return FLOAT;
        }
        if (value instanceof Double) {
            return DOUBLE;
        }
        if (value instanceof String) {
            return STRING;
        }
        return null;
    }

    /**
     * Determine the aida type of a list of java objects of supported types
     *
     * @param values list of values
     * @return the aida type
     */
    public static AidaType aidaTypeOf(List<?> values) {
        if (values == null || values.isEmpty()) {
            return null;
        }
        return arrayTypeOf(aidaTypeOf(values.get(0)));
    }

    /**
     * Return the corresponding array type for the given scalar type
     *
     * @param aidaType the given scalar type
     * @return the corresponding array type
     */
    private static AidaType arrayTypeOf(AidaType aidaType) {
        switch (aidaType) {
            case BOOLEAN:
                return BOOLEAN_ARRAY;
            case BYTE:
                return BYTE_ARRAY;
            case DOUBLE:
                return DOUBLE_ARRAY;
            case FLOAT:
                return FLOAT_ARRAY;
            case INTEGER:
                return INTEGER_ARRAY;
            case LONG:
                return LONG_ARRAY;
            case SHORT:
                return SHORT_ARRAY;
            case STRING:
                return STRING_ARRAY;
            case TABLE:
            default:
                return null;
        }
    }

}