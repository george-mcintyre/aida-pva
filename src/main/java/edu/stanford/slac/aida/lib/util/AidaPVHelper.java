package edu.stanford.slac.aida.lib.util;

import edu.stanford.slac.aida.lib.model.AidaChannelConfig;
import edu.stanford.slac.aida.lib.model.AidaField;
import edu.stanford.slac.aida.lib.model.AidaType;
import lombok.NonNull;
import org.epics.pvaccess.PVFactory;
import org.epics.pvdata.factory.FieldFactory;
import org.epics.pvdata.pv.*;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import static edu.stanford.slac.aida.lib.model.AidaTableLayout.ROW_MAJOR;
import static edu.stanford.slac.aida.lib.model.AidaType.STRING;
import static edu.stanford.slac.aida.lib.model.AidaType.aidaTypeOf;
import static org.epics.pvdata.pv.ScalarType.pvString;

public class AidaPVHelper {
    private static final String NTSCALAR_ID = "epics:nt/NTScalar:1.0";
    private static final String NTSCALARARRAY_ID = "epics:nt/NTScalarArray:1.0";
    private static final String NTTABLE_ID = "epics:nt/NTTable:1.0";

    public static final String NT_FIELD_NAME = "value";
    public static final String NT_LABELS_NAME = "labels";
    private static final String[] NT_TABLE_TOP_STRUCTURE_NAMES = {NT_LABELS_NAME, NT_FIELD_NAME};
    private static final String[] NT_SCALAR_TOP_STRUCTURE_NAME = {NT_FIELD_NAME};

    public static final String[] EMPTY_FIELD_NAMES = new String[0];
    public static final Field[] EMPTY_FIELD_DEFINITIONS = new Field[0];

    /**
     * Empty fields for an empty NT_TABLE
     */
    private static final Field[] NT_TABLE_EMPTY_FIELDS = new Field[]{
            FieldFactory.getFieldCreate().createScalarArray(pvString),
            FieldFactory.getFieldCreate().createStructure(EMPTY_FIELD_NAMES, EMPTY_FIELD_DEFINITIONS)
    };

    /**
     * Empty field for an empty NT_SCALAR_ARRAY.  We use string as it doesn't matter as it will be empty
     */
    private static final Field[] NT_SCALAR_ARRAY_EMPTY_FIELD = new Field[]{
            FieldFactory.getFieldCreate().createScalarArray(pvString)
    };

    /**
     * Empty field for an empty NT_SCALAR  We use string as it doesn't matter as it will be empty
     */
    private static final Field[] NT_SCALAR_EMPTY_FIELD = new Field[]{
            FieldFactory.getFieldCreate().createScalar(pvString)
    };

    /**
     * Empty table structure when no data is returned
     */
    public static final PVStructure NT_SCALAR_EMPTY_STRUCTURE =
            PVFactory.getPVDataCreate()
                    .createPVStructure(
                            FieldFactory.getFieldCreate()
                                    // Structure with one element: label and empty scalar field definition
                                    .createStructure(NTSCALAR_ID, NT_SCALAR_TOP_STRUCTURE_NAME, NT_SCALAR_EMPTY_FIELD));


    /**
     * Empty scalar array when no data is returned
     */
    public static final PVStructure NT_SCALAR_ARRAY_EMPTY_STRUCTURE =
            PVFactory.getPVDataCreate()
                    .createPVStructure(
                            FieldFactory.getFieldCreate()
                                    // Structure with one element: label and empty scalar array field definition
                                    .createStructure(NTSCALARARRAY_ID, NT_SCALAR_TOP_STRUCTURE_NAME, NT_SCALAR_ARRAY_EMPTY_FIELD));


    /**
     * Empty table structure when no data is returned
     */
    public static final PVStructure NT_TABLE_EMPTY_STRUCTURE =
            PVFactory.getPVDataCreate()
                    .createPVStructure(
                            FieldFactory.getFieldCreate()
                                    // Structure with two elements: labels and field definitions
                                    .createStructure(NTTABLE_ID, NT_TABLE_TOP_STRUCTURE_NAMES, NT_TABLE_EMPTY_FIELDS));


    /**
     * Set field value inside a given structure.
     *
     * @param structure the given structure
     * @param fieldName field name string.
     * @param value     the value to set in the field
     */
    public static void setValue(@NonNull PVStructure structure, @NonNull String fieldName, Object value) {
        AidaType aidaType = aidaTypeOf(value);
        if (aidaType != null) {
            setValue(structure, fieldName, value, aidaType);
        }
    }

    /**
     * Set field value inside a given structure.
     *
     * @param structure the given structure
     * @param fieldName field name string.  optionally containing dot notation to indicate sub-structures
     * @param value     the value to set in the field
     * @param aidaType  the aida type of the value to set
     */
    private static void setValue(@NonNull PVStructure structure, @NonNull String fieldName, Object value, @NonNull AidaType aidaType) {
        // Get sub-fields if required
        String[] fieldParts = fieldName.split("\\.");
        // For all sub-fields get sub-structures
        for (int i = 0; i < fieldParts.length - 1; i++) {
            structure = structure.getStructureField(fieldParts[i]);
            fieldName = fieldParts[i + 1];
        }

        switch (aidaType) {
            case BOOLEAN:
                structure.getBooleanField(fieldName).put((Boolean) value);
                break;
            case BYTE:
                structure.getByteField(fieldName).put((Byte) value);
                break;
            case DOUBLE:
                structure.getDoubleField(fieldName).put((Double) value);
                break;
            case FLOAT:
                structure.getFloatField(fieldName).put((Float) value);
                break;
            case INTEGER:
                structure.getIntField(fieldName).put((Integer) value);
                break;
            case LONG:
                structure.getLongField(fieldName).put((Long) value);
                break;
            case SHORT:
                structure.getShortField(fieldName).put((Short) value);
                break;
            case STRING:
                structure.getStringField(fieldName).put((String) value);
                break;
        }
    }

    /**
     * Set values for the given pv structure for the specified field name
     *
     * @param structure the given structure
     * @param fieldName the specified field name
     * @param values    the values to set
     */
    private static void setValues(@NonNull PVStructure structure, @NonNull String fieldName, @NonNull List<?> values) {
        AidaType aidaType = aidaTypeOf(values);
        if (aidaType != null) {
            setValues(structure, fieldName, values, aidaType);
        }
    }

    /**
     * Set field values inside a given structure.
     *
     * @param structure the given structure
     * @param fieldName field name string.
     * @param values    the values to set in the field
     * @param aidaType  the aida type of the value to set
     */
    private static void setValues(@NonNull PVStructure structure, @NonNull String fieldName, @NonNull List<?> values, @NonNull AidaType aidaType) {
        // Get sub-fields if required
        String[] fieldParts = fieldName.split("\\.");
        // For all sub-fields get sub-structures
        for (int i = 0; i < fieldParts.length - 1; i++) {
            structure = structure.getStructureField(fieldParts[i]);
            fieldName = fieldParts[i + 1];
        }

        int valuesCount = values.size();
        Object[] valuesArray = values.toArray();
        PVScalarArray scalarArray = structure.getScalarArrayField(fieldName, scalarTypeOf(aidaType));
        scalarArray.setCapacity(valuesCount);

        switch (aidaType) {
            case BOOLEAN_ARRAY: {
                boolean[] primitiveArray = new boolean[valuesCount];
                for (int i = 0; i < valuesCount; i++)
                    primitiveArray[i] = (Boolean) valuesArray[i];

                ((PVBooleanArray) scalarArray).put(0, valuesCount, primitiveArray, 0);
                break;
            }
            case BYTE_ARRAY: {
                byte[] primitiveArray = new byte[valuesCount];
                for (int i = 0; i < valuesCount; i++)
                    primitiveArray[i] = (Byte) valuesArray[i];

                ((PVByteArray) scalarArray).put(0, valuesCount, primitiveArray, 0);
                break;
            }
            case DOUBLE_ARRAY: {
                double[] primitiveArray = new double[valuesCount];
                for (int i = 0; i < valuesCount; i++)
                    primitiveArray[i] = (Double) valuesArray[i];

                ((PVDoubleArray) scalarArray).put(0, valuesCount, primitiveArray, 0);
                break;
            }
            case FLOAT_ARRAY: {
                float[] primitiveArray = new float[valuesCount];
                for (int i = 0; i < valuesCount; i++)
                    primitiveArray[i] = (Float) valuesArray[i];

                ((PVFloatArray) scalarArray).put(0, valuesCount, primitiveArray, 0);
                break;
            }
            case INTEGER_ARRAY: {
                int[] primitiveArray = new int[valuesCount];
                for (int i = 0; i < valuesCount; i++)
                    primitiveArray[i] = (Integer) valuesArray[i];

                ((PVIntArray) scalarArray).put(0, valuesCount, primitiveArray, 0);
                break;
            }
            case LONG_ARRAY: {
                long[] primitiveArray = new long[valuesCount];
                for (int i = 0; i < valuesCount; i++)
                    primitiveArray[i] = (Long) valuesArray[i];

                ((PVLongArray) scalarArray).put(0, valuesCount, primitiveArray, 0);
                break;
            }
            case SHORT_ARRAY: {
                short[] primitiveArray = new short[valuesCount];
                for (int i = 0; i < valuesCount; i++)
                    primitiveArray[i] = (Short) valuesArray[i];

                ((PVShortArray) scalarArray).put(0, valuesCount, primitiveArray, 0);
                break;
            }
            case STRING_ARRAY: {
                String[] primitiveArray = new String[valuesCount];
                for (int i = 0; i < valuesCount; i++)
                    primitiveArray[i] = (String) valuesArray[i];

                ((PVStringArray) scalarArray).put(0, valuesCount, primitiveArray, 0);
                break;
            }
        }
        scalarArray.setImmutable();
    }

    /**
     * Convert a value to a PVStructure containing an NT_SCALAR
     * <p>
     * The value is one of the supported scalar types Boolean, Byte, Short, Int, Long, Float, Double or String
     *
     * @param value supported scalar value
     * @return PVStructure containing an NT_SCALAR with the value
     */
    public static PVStructure asScalar(Object value, AidaChannelConfig aidaChannelConfig) {
        if (value == null) {
            return NT_SCALAR_EMPTY_STRUCTURE;
        }

        AidaType aidaType = aidaTypeOf(value);
        if (aidaType == null) {
            // TODO log error
            return NT_SCALAR_EMPTY_STRUCTURE;
        }

        ScalarType scalarType = scalarTypeOf(aidaType);
        Field[] scalarField = new Field[]{
                FieldFactory.getFieldCreate().createScalar(scalarType)
        };

        PVStructure retVal = PVFactory.getPVDataCreate()
                .createPVStructure(
                        FieldFactory.getFieldCreate()
                                // Structure with one element: label and empty scalar field definition
                                .createStructure(NTSCALAR_ID, NT_SCALAR_TOP_STRUCTURE_NAME, scalarField));

        // Set value
        setValue(retVal, NT_FIELD_NAME, value, aidaType);

        return retVal;
    }

    /**
     * Convert a list of values to a PVStructure containing an NT_SCALAR_ARRAY
     *
     * @param values the list of values to convert
     * @return PVStructure containing an NT_SCALAR_ARRAY with the values
     */
    public static PVStructure asScalarArray(List<?> values, AidaChannelConfig aidaChannelConfig) {
        AidaType aidaType = aidaTypeOf(values);
        if (aidaType == null) {
            return NT_SCALAR_ARRAY_EMPTY_STRUCTURE;
        }

        ScalarType scalarType = scalarTypeOf(aidaType);

        Field[] scalarArrayField = new Field[]{
                FieldFactory.getFieldCreate().createScalarArray(scalarType)
        };

        PVStructure retVal = PVFactory.getPVDataCreate()
                .createPVStructure(
                        FieldFactory.getFieldCreate()
                                // Structure with one element: label and empty scalar field definition
                                .createStructure(NTSCALARARRAY_ID, NT_SCALAR_TOP_STRUCTURE_NAME, scalarArrayField));


        // Set value
        setValues(retVal, NT_FIELD_NAME, values, aidaType);

        return retVal;
    }


    /**
     * Convert a list of values to a PVStructure containing an NT_TABLE
     * <p>
     * The list is a list of columns or a list of rows.  The config.layout is used to determine whether
     * the lists are ROW_MAJOR or COLUMN_MAJOR (default)
     * <p>
     * The columns/rows are pulled out out the list and mapped to the NT_TABLE columns
     *
     * @param values the list of values
     * @return the returned PVStructure containing the NT_TABLE
     */
    public static PVStructure asNtTable(List<List<Object>> values, AidaChannelConfig aidaChannelConfig) {
        if (values == null || values.isEmpty() || values.get(0).isEmpty()) {
            return NT_TABLE_EMPTY_STRUCTURE;
        }

        // If layout is row-major then transpose the data first before processing
        if (aidaChannelConfig.getLayout().equals(ROW_MAJOR)) {
            values = transpose(values);
        }

        // Track fields, field names, labels and types
        List<Field> pvFields = new ArrayList<Field>();
        List<String> fieldNames = new ArrayList<String>();
        List<String> labels = new ArrayList<String>();
        List<AidaType> aidaTypes = new ArrayList<AidaType>();

        // get fields, field names, labels and types
        getFieldsNamesLabelsAndTypes(values, aidaChannelConfig, pvFields, fieldNames, labels, aidaTypes);

        // Create top level NT_TABLE fields (labels and value{column1[], column2[], ...})
        Field scalarArray = FieldFactory.getFieldCreate().createScalarArray(pvString);
        String [] fieldNameArray = new String[fieldNames.size()];
        for ( int i = 0; i < fieldNames.size() ; i++) {
            fieldNameArray[i] = fieldNames.get(i);
        }
        Field [] fieldArray = new Field[pvFields.size()];
        for ( int i = 0; i < pvFields.size() ; i++) {
            fieldArray[i] = pvFields.get(i);
        }
        Field structure = FieldFactory.getFieldCreate().createStructure(fieldNameArray, fieldArray);

        Field[] topFields = new Field[]{scalarArray, structure};

        // Create the top structure that will be returned (labels and values)
        PVStructure retVal = PVFactory.getPVDataCreate()
                .createPVStructure(
                        FieldFactory.getFieldCreate()
                                .createStructure(NTTABLE_ID, NT_TABLE_TOP_STRUCTURE_NAMES, topFields));

        // Set the field labels
        setValues(retVal, NT_LABELS_NAME, labels, STRING);

        // Set array values
        for (int i = 0; i < fieldNames.size(); i++) {
            String fieldName = NT_FIELD_NAME + "." + fieldNames.get(i);
            List<?> columnValues = values.get(i);
            AidaType aidaType = aidaTypes.get(i);

            setValues(retVal, fieldName, columnValues, aidaType);
        }

        return retVal;
    }

    /**
     * Get the fields, field names, labels and types from the supplied values and config
     *
     * @param values                the supplied values
     * @param channelConfig         the config
     * @param fieldsToPopulate      the fields to populate - provide an empty list
     * @param fieldNamesToPopulate  the field names to populate - provide an empty list
     * @param fieldLabelsToPopulate the labels to populate - provide an empty list
     * @param fieldTypesToPopulate  the types to populate - provide an empty list
     */
    private static void getFieldsNamesLabelsAndTypes(
            List<List<Object>> values, AidaChannelConfig channelConfig,
            List<Field> fieldsToPopulate, List<String> fieldNamesToPopulate, List<String> fieldLabelsToPopulate, List<AidaType> fieldTypesToPopulate) {

        // Loop over values and channel config simultaneously
        Iterator<AidaField> configIterator = channelConfig.getFields().listIterator();
        for (List<?> column : values) {
            AidaType aidaType = aidaTypeOf(column);
            ScalarType scalarType = scalarTypeOf(aidaType);
            fieldsToPopulate.add(FieldFactory.getFieldCreate().createScalarArray(scalarType));

            if (!configIterator.hasNext()) {
                break;
            }
            AidaField fieldConfig = configIterator.next();

            // Get type, field name and label
            // TODO description and units
            fieldTypesToPopulate.add(aidaType);
            fieldNamesToPopulate.add(fieldConfig.getName());
            fieldLabelsToPopulate.add(fieldConfig.getLabel());
        }
    }

    /**
     * transpose a table represented as rows of data to a table represented as columns of data
     *
     * @param rows rows of data
     * @return columns of data
     */
    private static List<List<Object>> transpose(List<List<Object>> rows) {
        List<List<Object>> columns = new ArrayList<List<Object>>();

        for (int rowNumber = 0; rowNumber < rows.size(); rowNumber++) {
            List<?> row = rows.get(rowNumber);
            for (int columnNumber = 0; columnNumber < rows.size(); columnNumber++) {
                Object value = row.get(columnNumber);

                if (rowNumber == 0) {
                    columns.add(new ArrayList<Object>());
                }
                // Add row value to the correct column list
                columns.get(columnNumber).add(value);
            }
        }
        return columns;
    }

    /**
     * Determine the epics type of a java object of a supported type
     *
     * @param value java object of supported type
     * @return the epics scalar type
     */
    public static Type typeOf(Object value) {
        return typeOf(aidaTypeOf(value));
    }

    /**
     * Determine the epics scalar type of a java object of a supported type
     *
     * @param value java object of supported type
     * @return the epics scalar type
     */
    public static ScalarType scalarTypeOf(Object value) {
        return scalarTypeOf(aidaTypeOf(value));
    }

    /**
     * Determine the epics type of a list of java objects of supported types
     *
     * @param values list of values
     * @return scalar type of values
     */
    public static Type typeOf(List<?> values) {
        return typeOf(aidaTypeOf(values));
    }

    /**
     * Determine the epics scalar type of a list of java objects of supported types
     *
     * @param values list of values
     * @return scalar type of values
     */
    public static ScalarType scalarTypeOf(List<?> values) {
        return scalarTypeOf(aidaTypeOf(values));
    }

    /**
     * Determine the epics type based on the aida type
     *
     * @param aidaType aida type
     * @return epics type
     */
    public static Type typeOf(AidaType aidaType) {
        switch (aidaType) {
            case BOOLEAN:
            case BYTE:
            case SHORT:
            case INTEGER:
            case LONG:
            case FLOAT:
            case DOUBLE:
            case STRING:
            case SCALAR:
                return Type.scalar;
            case BOOLEAN_ARRAY:
            case BYTE_ARRAY:
            case SHORT_ARRAY:
            case INTEGER_ARRAY:
            case LONG_ARRAY:
            case FLOAT_ARRAY:
            case DOUBLE_ARRAY:
            case STRING_ARRAY:
            case SCALAR_ARRAY:
                return Type.scalarArray;
            case TABLE:
            case ANY:
                return Type.structure;
            default:
                return null;
        }
    }

    /**
     * Determine the epics scalar type based on the aida type
     *
     * @param aidaType aida type
     * @return epics scalar type
     */
    public static ScalarType scalarTypeOf(AidaType aidaType) {
        switch (aidaType) {
            case BOOLEAN:
            case BOOLEAN_ARRAY:
                return ScalarType.pvBoolean;
            case BYTE:
            case BYTE_ARRAY:
                return ScalarType.pvByte;
            case DOUBLE:
            case DOUBLE_ARRAY:
                return ScalarType.pvDouble;
            case FLOAT:
            case FLOAT_ARRAY:
                return ScalarType.pvFloat;
            case INTEGER:
            case INTEGER_ARRAY:
                return ScalarType.pvInt;
            case LONG:
            case LONG_ARRAY:
                return ScalarType.pvLong;
            case SHORT:
            case SHORT_ARRAY:
                return ScalarType.pvShort;
            case STRING:
            case STRING_ARRAY:
                return ScalarType.pvString;
            case TABLE:
            default:
                return null;
        }
    }
}

