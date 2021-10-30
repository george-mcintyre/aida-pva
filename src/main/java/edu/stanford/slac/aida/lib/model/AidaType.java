package edu.stanford.slac.aida.lib.model;

import java.util.List;

/**
 * @file
 * This class encapsulates an AIDA-PVA type.
 * <p>
 * All the data types that can be used in AIDA-PVA are represented here.
 */
public enum AidaType {
    /*
     * API Not valid
     */
    NONE,               // Means that this getter or setter is not supported

    /*
     * No return value
     */
    VOID,               // Means that this setter does not return a value (only valid for setters)

    /*
     * Scalar return values
     */
    BOOLEAN,            // Getter returns a boolean
    BYTE,               // Getter returns a byte
    SHORT,              // Getter returns a short
    INTEGER,            // Getter returns an integer
    LONG,               // Getter returns a long
    FLOAT,              // Getter returns a float
    DOUBLE,             // Getter returns a double
    STRING,             // Getter returns a string

    /*
     * Scalar Array return values
     */
    BOOLEAN_ARRAY,      // Getter returns a boolean array
    BYTE_ARRAY,         // Getter returns a byte array
    SHORT_ARRAY,        // Getter returns a short array
    INTEGER_ARRAY,      // Getter returns an integer array
    LONG_ARRAY,         // Getter returns a long array
    FLOAT_ARRAY,        // Getter returns a float array
    DOUBLE_ARRAY,       // Getter returns a double array
    STRING_ARRAY,       // Getter returns a string array

    /*
     * Table Array return values
     */
    TABLE,              // Getter or setter returns a table

    /*
     * Return meta-types.
     * These meta-types are used for channel configuration only.
     * They cannot be used as the value of a TYPE parameter.
     *
     * When a channel is configured with any of these meta-types, all requests for that channel must be presented with TYPE parameter.
     *
     * All of these meta-types allow you to set TABLE as the value of the mandatory TYPE parameter so in the configuration,
     * you must also provide field definitions if the Channel Provider will support TABLE requests.
     */
    ANY,                // Getter or setter returns any type defined by the mandatory accompanying TYPE argument, for setters this can only be VOID or TABLE
    SCALAR,             // Constrains the TYPE parameter to be set to any scalar type or TABLE
    SCALAR_ARRAY;       // Constrains the TYPE parameter to be set to any scalar array type or TABLE

    /**
     * Given a {@link Object} this method will return the corresponding AIDA-PVA type.
     *
     * @param value java object to check
     * @return the corresponding AIDA-PVA type or null if the type is not supported by AIDA-PVA.
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
     * Given a {@link List} of homogenous objects this method will return the corresponding AIDA-PVA type of the List.
     *
     * @param values list of homogenous objects
     * @return the corresponding AIDA-PVA type of the List or null if the type is not supported
     */
    public static AidaType aidaTypeOf(List<?> values) {
        if (values == null || values.isEmpty()) {
            return null;
        }
        return arrayTypeOf(aidaTypeOf(values.get(0)));
    }

    /**
     * Get the meta-type of this aida type
     *
     * @return the meta type SCALAR, SCALAR_ARRAY, or AHY
     */
    public AidaType metaType() {
        switch (this) {
            case BOOLEAN:
            case BYTE:
            case DOUBLE:
            case FLOAT:
            case INTEGER:
            case LONG:
            case SHORT:
            case STRING:
                return SCALAR;
            case BYTE_ARRAY:
            case BOOLEAN_ARRAY:
            case DOUBLE_ARRAY:
            case FLOAT_ARRAY:
            case INTEGER_ARRAY:
            case LONG_ARRAY:
            case SHORT_ARRAY:
            case STRING_ARRAY:
                return SCALAR_ARRAY;
            case TABLE:
            default:
                return ANY;
        }
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

    /**
     * Check if the specified type is compatible with the configured type.
     *
     * @param specifiedAidaType the specified type
     * @param configuredType    the configured type
     * @return true if compatible false if not
     */
    public static boolean isSpecifiedTypeCompatibleWithConfiguredType(AidaType specifiedAidaType, AidaType configuredType) {
        if (configuredType == ANY) {
            return true;
        } else if (configuredType == SCALAR || configuredType == SCALAR_ARRAY) {
            return specifiedAidaType == TABLE || specifiedAidaType.metaType() == configuredType;
        } else
            return specifiedAidaType == configuredType;
    }
}
