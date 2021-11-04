package edu.stanford.slac.aida.lib.util;

import edu.stanford.slac.aida.lib.model.*;
import edu.stanford.slac.except.AidaInternalException;
import lombok.NonNull;
import org.epics.pvaccess.PVFactory;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.factory.FieldFactory;
import org.epics.pvdata.pv.*;
import org.epics.util.array.*;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import static edu.stanford.slac.aida.lib.model.AidaType.STRING_ARRAY;
import static edu.stanford.slac.aida.lib.model.AidaType.aidaTypeOf;
import static org.epics.pvdata.pv.ScalarType.pvString;
import static org.epics.pvdata.pv.Status.StatusType.ERROR;

/**
 * @file
 * Helper functions for PVData types used to construct and decode messages conforming to the Normative Types scheme.
 * This class provides many functions that help with processing EPICS `Process Variables`.
 * APDA-PVA receives data from EPICS in {@link PVField} and
 * {@link PVStructure}, and we send data back to the EPICS network
 * using the these same data types.
 * <p>
 * This class provides functions that convert to and from AIDA-PVA java types, and enables creation of the
 * structures and norms required to reply to, and decipher EPICS network requests and data.
 * <p>
 * Furthermore, we use Normative Types to represent data in a standard way.  This is layered on top
 * of Process Variables.   So this class also provides constants and standard structures for use with
 * Normative Types.
 * <a href="{@docRoot}/docs/2_2_Normative_Types.md">AIDA-PVA Normative Types Documentation</a>
 */
public class AidaPVHelper {
    /**
     * Each top level {@link PVStructure} in the Normative Type scheme
     * has a {@link ScalarType#pvString} {@link PVField}
     * named "ID" that contains a special identifier that is used to determine the contents of the PVStructure
     * <p>
     * This static constant is the string used to identify {@link PVStructure}s
     * containing {@link PVScalar} data representing and NTScalar type.
     */
    private static final String NTSCALAR_ID = "epics:nt/NTScalar:1.0";

    /**
     * Each top level {@link PVStructure} in the Normative Type scheme
     * has a {@link ScalarType#pvString} {@link PVField}
     * named "ID" that contains a special identifier that is used to determine the contents of the PVStructure
     * <p>
     * This static constant is the string used to identify {@link PVStructure}s
     * containing {@link PVScalarArray} data representing and NTScalarArray type.
     */
    private static final String NTSCALARARRAY_ID = "epics:nt/NTScalarArray:1.0";

    /**
     * Each top level {@link PVStructure} in the Normative Type scheme
     * has a {@link ScalarType#pvString} {@link PVField}
     * named "ID" that contains a special identifier that is used to determine the contents of the PVStructure
     * <p>
     * This static constant is the string used to identify {@link PVStructure}s
     * containing {@link PVStructure} data representing and NTTable type.
     */
    private static final String NTTABLE_ID = "epics:nt/NTTable:1.0";

    /**
     * The Normative Type specification defines the name of the field containing the value part
     * of its main types (Scalar, ScalarArray, and Table) as "value".
     */
    public static final String NT_FIELD_NAME = "value";

    /**
     * The Normative Type specification defines the name of the field containing the labels part
     * of its Table type as "labels".
     */
    public static final String NT_LABELS_NAME = "labels";

    /**
     * These are the names of the elements in the top level of the Normative Type Table structure
     */
    private static final String[] NT_TABLE_TOP_STRUCTURE_NAMES = {NT_LABELS_NAME, NT_FIELD_NAME};

    /**
     * These are the fields at the top level of the Normative Type Scalar Structure
     */
    private static final String[] NT_SCALAR_TOP_STRUCTURE_NAME = {NT_FIELD_NAME};

    /**
     * If you want to add no field names to a structure then use this shortcut
     */
    public static final String[] EMPTY_FIELD_NAMES = new String[0];

    /**
     * If you want to add no fields to a structure then use this shortcut
     */
    public static final Field[] EMPTY_FIELD_DEFINITIONS = new Field[0];

    /**
     * Shortcut for a complete empty {@link Field} for an empty NTScalar
     * We use string as it doesn't matter as it will be empty
     */
    private static final Field[] NT_SCALAR_EMPTY_FIELD = new Field[]{
            FieldFactory.getFieldCreate().createScalar(pvString)
    };

    /**
     * Shortcut for a complete empty {@link Field} for an empty NTScalarArray.
     * We use string as it doesn't matter as it will be empty
     */
    private static final Field[] NT_SCALAR_ARRAY_EMPTY_FIELD = new Field[]{
            FieldFactory.getFieldCreate().createScalarArray(pvString)
    };

    /**
     * Shortcut for complete empty {@link Field}s for an empty NTTable structure
     */
    private static final Field[] NT_TABLE_EMPTY_FIELDS = new Field[]{
            FieldFactory.getFieldCreate().createScalarArray(pvString),
            FieldFactory.getFieldCreate().createStructure(EMPTY_FIELD_NAMES, EMPTY_FIELD_DEFINITIONS)
    };

    /**
     * Shortcut for a complete empty NTTable {@link PVStructure} for when no data is returned from the Channel Provider
     * from a request
     */
    public static final PVStructure NT_SCALAR_EMPTY_STRUCTURE =
            PVFactory.getPVDataCreate()
                    .createPVStructure(
                            FieldFactory.getFieldCreate()
                                    // Structure with one element: label and empty scalar field definition
                                    .createStructure(NTSCALAR_ID, NT_SCALAR_TOP_STRUCTURE_NAME, NT_SCALAR_EMPTY_FIELD));


    /**
     * Shortcut for a complete empty {@link PVStructure} containing an NTScalarArray when no data is returned
     */
    public static final PVStructure NT_SCALAR_ARRAY_EMPTY_STRUCTURE =
            PVFactory.getPVDataCreate()
                    .createPVStructure(
                            FieldFactory.getFieldCreate()
                                    // Structure with one element: label and empty scalar array field definition
                                    .createStructure(NTSCALARARRAY_ID, NT_SCALAR_TOP_STRUCTURE_NAME, NT_SCALAR_ARRAY_EMPTY_FIELD));


    /**
     * Shortcut for a complete empty {@link PVStructure} containing an NTTable when no data is returned
     */
    public static final PVStructure NT_TABLE_EMPTY_STRUCTURE =
            PVFactory.getPVDataCreate()
                    .createPVStructure(
                            FieldFactory.getFieldCreate()
                                    // Structure with two elements: labels and field definitions
                                    .createStructure(NTTABLE_ID, NT_TABLE_TOP_STRUCTURE_NAMES, NT_TABLE_EMPTY_FIELDS));

    /**
     * Creating a {@link PVStructure} is a two stage process.  First you need to create its
     * structure, then you fill that structure with data.  The name of the field in any
     * {@link PVStructure} where the value is stored is always called "value".
     * <p>
     * This method allows you to set the value field in an already created {@link PVStructure} to the given value.
     *
     * @param structure the given structure.
     * @param value     the value to set in the value field.
     * @param aidaType  the {@link AidaType} of the value to set.
     */
    private static void setValue(@NonNull PVStructure structure, Object value, @NonNull AidaType aidaType) {
        PVField field = structure.getSubField(NT_FIELD_NAME);
        switch (aidaType) {
            case BOOLEAN:
                ((PVBoolean) field).put((Boolean) value);
                break;
            case BYTE:
                ((PVByte) field).put((Byte) value);
                break;
            case DOUBLE:
                ((PVDouble) field).put((Double) value);
                break;
            case FLOAT:
                ((PVFloat) field).put((Float) value);
                break;
            case INTEGER:
                ((PVInt) field).put((Integer) value);
                break;
            case LONG:
                ((PVLong) field).put((Long) value);
                break;
            case SHORT:
                ((PVShort) field).put((Short) value);
                break;
            case STRING:
                ((PVString) field).put((String) value);
                break;
        }

        // Change the status of the field to be immutable meaning that its value can no longer be changed
        field.setImmutable();
    }

    /**
     * Creating a {@link PVStructure} is a two stage process.  First you need to create its
     * structure, then you fill that structure with data.
     * <p>
     * This method allows you to set the value of any array field in an already
     * created {@link PVStructure} to the given list of values.
     * <p>
     * The `fieldName` parameter can be a path from the root of the structure down to the array that the values will be stored in.
     * So it may be of the form "value.listName" which would store the values in the array called "listName"
     * which is in the root structure field called "value".
     *
     * @param structure the given structure.
     * @param fieldName the name of the array field.
     * @param values    the values to set in the field.
     * @param aidaType  the {@link AidaType} of the values to set.
     */
    @SuppressWarnings("unchecked")
    private static void setValues(@NonNull PVStructure structure, @NonNull String fieldName, @NonNull List<?> values, @NonNull AidaType aidaType) {
        // If the `fieldName` parameter is specified as a path then find the target structure where the values
        // have to eventually be set
        String[] fieldParts = fieldName.split("\\.");
        // For all subfields get substructures
        for (int i = 0; i < fieldParts.length - 1; i++) {
            structure = structure.getStructureField(fieldParts[i]);
            fieldName = fieldParts[i + 1];
        }

        // Now we have the target `structure` that we want to set `fieldName` to `values`

        // Find out how many values there are
        int valuesCount = values.size();

        // Locate the array that was defined in the first stage of
        // this {@link PVStructure} creation
        // and set the capacity to the number of values we need to store
        PVScalarArray scalarArray = structure.getScalarArrayField(fieldName, scalarTypeOf(aidaType));
        scalarArray.setCapacity(valuesCount);

        // Depending on the type of the values to add, call the correct method to add the values
        // In each case we can't add boxed values, so we convert to a primitive array first
        // then add that primitive array to the structure's field.
        switch (aidaType) {
            case BOOLEAN_ARRAY: {
                ((PVBooleanArray) scalarArray).put(0, valuesCount, toPrimitiveBooleanArray(((List<Boolean>) values)), 0);
                break;
            }
            case BYTE_ARRAY: {
                ((PVByteArray) scalarArray).put(0, valuesCount, toPrimitiveByteArray((List<Byte>) values), 0);
                break;
            }
            case INTEGER_ARRAY: {
                ((PVIntArray) scalarArray).put(0, valuesCount, toPrimitiveIntArray((List<Integer>) values), 0);
                break;
            }
            case SHORT_ARRAY: {
                ((PVShortArray) scalarArray).put(0, valuesCount, toPrimitiveShortArray((List<Short>) values), 0);
                break;
            }
            case LONG_ARRAY: {
                ((PVLongArray) scalarArray).put(0, valuesCount, toPrimitiveLongArray((List<Long>) values), 0);
                break;
            }
            case FLOAT_ARRAY: {
                ((PVFloatArray) scalarArray).put(0, valuesCount, toPrimitiveFloatArray((List<Float>) values), 0);
                break;
            }
            case DOUBLE_ARRAY: {
                ((PVDoubleArray) scalarArray).put(0, valuesCount, toPrimitiveDoubleArray((List<Double>) values), 0);
                break;
            }
            case STRING_ARRAY: {
                ((PVStringArray) scalarArray).put(0, valuesCount, toStringArray((List<String>) values), 0);
                break;
            }
        }

        // Change the status of the field to be immutable meaning that its value can no longer be changed
        scalarArray.setImmutable();
    }

    /**
     * Convert an arbitrary value to a {@link PVStructure} containing an `NTScalar`
     * <p>
     * The value must be the java equivalent of one of the supported AIDA-PVA scalar types
     * - {@link Boolean}, {@link Byte}, {@link Short},
     * - {@link Integer}, {@link Long}, {@link Float},
     * - {@link Double} or {@link String}
     *
     * @param value java equivalent of one of the supported AIDA-PVA scalar types
     * @return {@link PVStructure} containing an NTScalar with the value
     */
    public static PVStructure asScalar(Object value) {
        // Null values are returned as an empty {@link PVStructure}
        if (value == null) {
            return NT_SCALAR_EMPTY_STRUCTURE;
        }

        // Find out the AIDA-PVA type that corresponds to the given value
        AidaType aidaType = aidaTypeOf(value);
        if (aidaType == null) {
            throw new AidaInternalException("Could not determine type of returned value: " + value);
        }

        // And find out the corresponding PVScalar type that it needs to converted into
        ScalarType scalarType = scalarTypeOf(aidaType);

        // Create the field of that type to be added to the {@link PVStructure} we return
        Field[] scalarField = new Field[]{
                FieldFactory.getFieldCreate().createScalar(scalarType)
        };

        // Create a new {@link PVStructure} to return containing the field we created
        PVStructure retVal = PVFactory.getPVDataCreate()
                .createPVStructure(
                        FieldFactory.getFieldCreate()
                                // Structure with one element: label and empty scalar field definition
                                .createStructure(NTSCALAR_ID, NT_SCALAR_TOP_STRUCTURE_NAME, scalarField));

        // In the second phase of {@link PVStructure} creation we now set the field value
        // Note that the field name to set is always called "value" for NT types
        setValue(retVal, value, aidaType);

        return retVal;
    }

    /**
     * Convert an arbitrary list of values to a {@link PVStructure} containing an `NTScalarArray`
     * <p>
     * The value must be the java {@link List} of a type equivalent of one of the supported AIDA-PVA scalar types
     * - {@link Boolean}, {@link Byte}, {@link Short},
     * - {@link Integer}, {@link Long}, {@link Float},
     * - {@link Double} or {@link String}
     *
     * @param values the list of values to convert
     * @return {@link PVStructure} containing an `NTScalarArray` with the values
     */
    public static PVStructure asScalarArray(List<?> values) {
        // Null values are returned as an empty {@link PVStructure}
        if (values == null) {
            return NT_SCALAR_ARRAY_EMPTY_STRUCTURE;
        }

        // Find out the AIDA-PVA type that corresponds to the given values
        AidaType aidaType = aidaTypeOf(values);
        if (aidaType == null) {
            return NT_SCALAR_ARRAY_EMPTY_STRUCTURE;
        }

        // And find out the corresponding PVScalar type that it needs to converted into
        ScalarType scalarType = scalarTypeOf(aidaType);

        // Create the field of that type to be added to the {@link PVStructure} we return
        Field[] scalarArrayField = new Field[]{
                FieldFactory.getFieldCreate().createScalarArray(scalarType)
        };

        // Create a new {@link PVStructure} to return containing the field we created
        PVStructure retVal = PVFactory.getPVDataCreate()
                .createPVStructure(
                        FieldFactory.getFieldCreate()
                                // Structure with one element: label and empty scalar field definition
                                .createStructure(NTSCALARARRAY_ID, NT_SCALAR_TOP_STRUCTURE_NAME, scalarArrayField));


        // In the second phase of {@link PVStructure} creation we now set the field value
        // Note that the field name to set is always called "value" for NT types
        setValues(retVal, NT_FIELD_NAME, values, aidaType);

        return retVal;
    }


    /**
     * Convert an arbitrary list of homogeneously sized columns of values to a {@link PVStructure} containing an `NTTable`
     * The value of each of the columns must be a java {@link List} of a type equivalent of one of the supported AIDA-PVA scalar types
     * - {@link Boolean}, {@link Byte}, {@link Short},
     * - {@link Integer}, {@link Long}, {@link Float},
     * - {@link Double} or {@link String}
     *
     * @param values            the list of values
     * @param aidaChannelConfig the configuration
     * @return the returned PVStructure containing the NT_TABLE
     */
    public static PVStructure asNtTable(List<List<Object>> values, AidaChannelOperationConfig aidaChannelConfig) {
        // If there is nothing to add or that the list is empty or if the columns are empty return an empty
        //  {@link PVStructure}
        if (values == null || values.isEmpty() || values.get(0).isEmpty()) {
            return NT_TABLE_EMPTY_STRUCTURE;
        }

        // If the lists are not homogeneously sized then raise an exception
        int columnSize = values.get(0).size();
        for (List<Object> column : values) {
            if (column.size() != columnSize) {
                throw new AidaInternalException("Columns in NTTable not homogeneously sized. Normal size: " + columnSize + ", One column size: " + column.size());
            }
        }

        // The field names, labels and types for an NTTable come from the Channel Configuration specified in the
        // Channel Configuration File loaded when the service initialises.
        // We need to retrieve these configured values from the given configuration associated with the
        // request we're processing
        List<String> fieldNames = new ArrayList<String>();
        List<String> labels = new ArrayList<String>();
        List<AidaType> aidaTypes = new ArrayList<AidaType>();
        // We need to create a set of fields that we will fill with these values
        List<Field> pvFields = new ArrayList<Field>();
        setFieldsWithNamesLabelsAndTypesFromConfig(pvFields, aidaChannelConfig, values, fieldNames, labels, aidaTypes);

        /// FIRST STAGE {@link PVStructure} creation: Structure

        // List of strings that will contain the labels names
        Field labelsArray = FieldFactory.getFieldCreate().createScalarArray(pvString);

        // Create the second level {@link Field} that will conta
        // top fields, this one holding the structure of the rest of the NTTable
        //  - fieldNames = PVScalarArray containing pvstring and
        //  - columns of values = A set of Fields that are each PVScalarArrays
        Field structure = FieldFactory.getFieldCreate().createStructure(toStringArray(fieldNames), toFieldArray(pvFields));

        // Create the two top fields of the
        Field[] topFields = new Field[]{labelsArray, structure};

        // Create the top {@link PVStructure} that will be returned (labels and values)
        PVStructure retVal = PVFactory.getPVDataCreate()
                .createPVStructure(
                        FieldFactory.getFieldCreate()
                                .createStructure(NTTABLE_ID, NT_TABLE_TOP_STRUCTURE_NAMES, topFields));


        /// SECOND STAGE {@link PVStructure} creation: Set values

        // Set the field labels
        setValues(retVal, NT_LABELS_NAME, labels, STRING_ARRAY);

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
     * @param fieldsToPopulate      the fields to populate - provide an empty list
     * @param channelConfig         the config
     * @param values                the supplied values
     * @param fieldNamesToPopulate  the field names to populate - provide an empty list
     * @param fieldLabelsToPopulate the labels to populate - provide an empty list
     * @param fieldTypesToPopulate  the types to populate - provide an empty list
     */
    private static void setFieldsWithNamesLabelsAndTypesFromConfig(
            List<Field> fieldsToPopulate, AidaChannelOperationConfig channelConfig, List<List<Object>> values,
            List<String> fieldNamesToPopulate, List<String> fieldLabelsToPopulate, List<AidaType> fieldTypesToPopulate) {

        // Loop over values and fields simultaneously
        Iterator<AidaField> fieldIterator = channelConfig.getFields().listIterator();
        for (List<?> column : values) {
            AidaType aidaType = aidaTypeOf(column);
            ScalarType scalarType = scalarTypeOf(aidaType);
            fieldsToPopulate.add(FieldFactory.getFieldCreate().createScalarArray(scalarType));

            if (!fieldIterator.hasNext()) {
                break;
            }
            AidaField fieldConfig = fieldIterator.next();

            // Get type, field name and label
            // TODO description and units
            fieldTypesToPopulate.add(aidaType);
            fieldNamesToPopulate.add(fieldConfig.getName());
            String units = fieldConfig.getUnits();
            fieldLabelsToPopulate.add(fieldConfig.getLabel() + ((units == null || units.length() == 0) ? "" : " (" + units + ")"));
        }
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
     * Determine the Normative type based on the aida type
     *
     * @param aidaType aida type
     * @return Normative Type String
     */
    public static String ntTypeOf(AidaType aidaType) {
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
                return NTSCALAR_ID;
            case BOOLEAN_ARRAY:
            case BYTE_ARRAY:
            case SHORT_ARRAY:
            case INTEGER_ARRAY:
            case LONG_ARRAY:
            case FLOAT_ARRAY:
            case DOUBLE_ARRAY:
            case STRING_ARRAY:
            case SCALAR_ARRAY:
                return NTSCALARARRAY_ID;
            case TABLE:
            case ANY:
                return NTTABLE_ID;
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
            case INTEGER:
            case INTEGER_ARRAY:
                return ScalarType.pvInt;
            case SHORT:
            case SHORT_ARRAY:
                return ScalarType.pvShort;
            case LONG:
            case LONG_ARRAY:
                return ScalarType.pvLong;
            case DOUBLE:
            case DOUBLE_ARRAY:
                return ScalarType.pvDouble;
            case FLOAT:
            case FLOAT_ARRAY:
                return ScalarType.pvFloat;
            case STRING:
            case STRING_ARRAY:
                return ScalarType.pvString;
            case TABLE:
            default:
                return null;
        }
    }

    /**
     * Extract the String value from the field
     * Any time a double or float is found, add it to the appropriate map under the `fieldPath` key
     *
     * @param field              the field
     * @param fieldPath          path under which this field sits.  Used only for floats and doubles that need
     *                           to set the path so the name in the pair is prefixed with this value if set
     * @param floatArgumentList  to return the list of floats found and the paths
     * @param doubleArgumentList to return the list of doubles found and the paths
     * @return the extracted value
     * @throws RPCRequestException of the field type is unsupported
     */
    public static String fieldToString(PVField field, String fieldPath, List<FloatArgument> floatArgumentList, List<DoubleArgument> doubleArgumentList) throws RPCRequestException {
        String value;

        if (field instanceof PVBoolean) {
            value = Boolean.toString(((PVBoolean) field).get());
        } else if (field instanceof PVByte) {
            value = Byte.toString(((PVByte) field).get());
        } else if (field instanceof PVUByte) {
            value = Byte.toString(((PVUByte) field).get());
        } else if (field instanceof PVShort) {
            value = Short.toString(((PVShort) field).get());
        } else if (field instanceof PVInt) {
            value = Integer.toString(((PVInt) field).get());
        } else if (field instanceof PVLong) {
            value = Long.toString(((PVLong) field).get());
        } else if (field instanceof PVFloat) { // For float store the ieee value in the list as well
            value = floatFieldToString((PVFloat) field, fieldPath, floatArgumentList);
        } else if (field instanceof PVDouble) { // For double store the ieee value in the list as well
            value = doubleFieldToString((PVDouble) field, fieldPath, doubleArgumentList);
        } else if (field instanceof PVString) { // For strings quote them is they are subfields but leave them untouched if not
            value = stringFieldToString((PVString) field, fieldPath);
        } else if (field instanceof PVStructure) { // For structures recurse for each of the fields
            value = structureFieldToString((PVStructure) field, fieldPath, floatArgumentList, doubleArgumentList);
        } else if (field instanceof PVArray) { // For arrays recurse for each of the array elements
            value = arrayFieldToString((PVArray) field, fieldPath, floatArgumentList, doubleArgumentList);
        } else {
            throw new RPCRequestException(ERROR, "Invalid argument value: can only accept scalar, scalar array, structure or structure array");
        }
        return value;
    }

    /**
     * Convert the {@link PVArray} field to the json string representation of an array of that field,
     * storing any ieee values found unchanged in the given {@link FloatArgument}and {@link DoubleArgument} lists
     *
     * @param field              the {@link PVArray} field
     * @param fieldPath          the absolute path to the field from the root
     * @param floatArgumentList  the {@link FloatArgument} list to store any ieee values
     * @param doubleArgumentList the {@link DoubleArgument} list to store any ieee values
     * @return the json string representation of the {@link PVArray}
     * @throws RPCRequestException if anything bad happens
     */
    private static String arrayFieldToString(PVArray field, String fieldPath, List<FloatArgument> floatArgumentList, List<DoubleArgument> doubleArgumentList) throws RPCRequestException {
        String value;
        StringBuilder arrayBuilder = new StringBuilder();

        arrayBuilder.append("["); // Start of array

        boolean firstTime = true;

        // Convert all array entries to Strings, recurse to get any floating point numbers into the appropriate lists
        List<String> arrayList = arrayFieldToStringList(field, fieldPath, floatArgumentList, doubleArgumentList);

        for (String subValue : arrayList) {  // For each string value in the retrieved list
            // Comma separate entries
            if (!firstTime) {
                arrayBuilder.append(", ");
            }
            firstTime = false;

            // Add to list
            arrayBuilder.append(subValue);
        }

        arrayBuilder.append("]"); // End of array

        value = arrayBuilder.toString();
        return value;
    }

    /**
     * Convert the {@link PVStructure} field to the json string representation of a structure of that field,
     * storing any ieee values found unchanged in the given {@link FloatArgument}and {@link DoubleArgument} lists
     *
     * @param field              the {@link PVStructure} field
     * @param fieldPath          the absolute path to the field from the root
     * @param floatArgumentList  the {@link FloatArgument} list to store any ieee values
     * @param doubleArgumentList the {@link DoubleArgument} list to store any ieee values
     * @return the json string representation of the {@link PVStructure}
     * @throws RPCRequestException if anything bad happens
     */
    public static String structureFieldToString(PVStructure field, String fieldPath, List<FloatArgument> floatArgumentList, List<DoubleArgument> doubleArgumentList) throws RPCRequestException {
        String value;
        StringBuilder structureBuilder = new StringBuilder();

        structureBuilder.append("{");  // Start of structure

        boolean firstTime = true;
        for (PVField subField : field.getPVFields()) { // For each field in structure
            String elementName = subField.getFieldName(); // get the field name
            String absolutePathToElement = fieldPath + "." + elementName; // This is the absolute path to the element from the root of the structure

            // And recurse into the field to get the string representation of it - of course any floats it contains will be stored in the appropriate lists
            String elementValue = fieldToString(subField, absolutePathToElement, floatArgumentList, doubleArgumentList);
            addStructureElementValue(structureBuilder, elementName, elementValue, firstTime);
            firstTime = false;
        }

        structureBuilder.append("}"); // End of structure

        value = structureBuilder.toString();
        return value;
    }

    /**
     * Add element value to the given {@link StringBuilder}
     *
     * @param structureBuilder the given {@link StringBuilder}
     * @param elementName      the element name to add
     * @param elementValue     the element value to add
     * @param firstElement     is this the first element added to the structure
     */
    private static void addStructureElementValue(StringBuilder structureBuilder, String elementName, String elementValue, boolean firstElement) {
        // Comma separate entries in the structure
        if (!firstElement) {
            structureBuilder.append(", ");
        }

        // Add the element name and the element value
        structureBuilder.append("\"");
        structureBuilder.append(elementName);
        structureBuilder.append("\": ");
        structureBuilder.append(elementValue);
    }

    /**
     * Extract array fields as a list of strings. recurse as necessary.
     *
     * @param array              any PV array
     * @param fieldPath          the absolute path to the field from the root
     * @param floatArgumentList  to return the list of floats found and the paths
     * @param doubleArgumentList to return the list of doubles found and the paths
     * @return the list of strings for this array
     * @throws RPCRequestException when there is an unsupported type
     */
    public static List<String> arrayFieldToStringList(PVArray array, String fieldPath, List<FloatArgument> floatArgumentList, List<DoubleArgument> doubleArgumentList) throws RPCRequestException {
        List<String> arrayList = new ArrayList<String>();

        if (array instanceof PVBooleanArray) {
            // For boolean arrays
            booleanArrayToString((PVBooleanArray) array, arrayList);

        } else if (array instanceof PVByteArray) {
            // For byte arrays
            byteArrayToString((PVByteArray) array, arrayList);

        } else if (array instanceof PVShortArray) {
            // For short arrays
            shortArrayToString((PVShortArray) array, arrayList);

        } else if (array instanceof PVIntArray) {
            // For integer arrays
            integerArrayToString((PVIntArray) array, arrayList);

        } else if (array instanceof PVLongArray) {
            // For long arrays
            longArrayToString((PVLongArray) array, arrayList);

        } else if (array instanceof PVFloatArray) {
            // For float arrays
            floatArrayToString((PVFloatArray) array, fieldPath, floatArgumentList, arrayList);

        } else if (array instanceof PVDoubleArray) {
            // For double arrays
            doubleArrayToString((PVDoubleArray) array, fieldPath, doubleArgumentList, arrayList);

        } else if (array instanceof PVStringArray) {
            // For string arrays
            stringArrayToString(array, arrayList);

        } else if (array instanceof PVStructureArray) {
            // For structure arrays then recurse to unpack each field
            structureArrayToString(array, fieldPath, floatArgumentList, doubleArgumentList, arrayList);
        }
        return arrayList;
    }

    /**
     * Convert a List of Fields to an array of Fields
     *
     * @param fields List of Fields
     * @return an array of Fields
     */
    private static Field[] toFieldArray(List<Field> fields) {
        int fieldCount = fields.size();
        Field[] fieldArray = new Field[fieldCount];
        for (int i = 0; i < fieldCount; i++) {
            fieldArray[i] = fields.get(i);
        }
        return fieldArray;
    }

    /**
     * True if the given absolute path to the field is not at the root
     *
     * @param fieldPath the absolute path to the field from the root
     * @return True if the given absolute path to the field is not at the root
     */
    private static boolean isSubField(String fieldPath) {
        return fieldPath.contains(".") || fieldPath.contains("[");
    }

    /**
     * Convert a List of Boolean to an array of primitive booleans
     *
     * @param values List of Booleans
     * @return an array of primitive booleans
     */
    private static boolean[] toPrimitiveBooleanArray(List<Boolean> values) {
        int valuesCount = values.size();
        boolean[] primitiveArray = new boolean[valuesCount];
        for (int i = 0; i < valuesCount; i++)
            primitiveArray[i] = values.get(i);
        return primitiveArray;
    }

    /**
     * Convert the given {@link PVBooleanArray} to {@link String}s, adding them to the provided {@link String} list
     *
     * @param array      the given {@link PVBooleanArray}
     * @param stringList the provided {@link String} list
     */
    private static void booleanArrayToString(PVBooleanArray array, List<String> stringList) {
        BooleanArrayData data = new BooleanArrayData();
        int offset = 0, len = array.getLength();
        while (offset < len) {
            int num = array.get(offset, (len - offset), data);
            for (int i = 0; i < num; i++) {
                stringList.add(Boolean.toString(data.data[offset + i]));
            }
            offset += num;
        }
    }

    /**
     * Convert a List of Byte to an array of primitive bytes
     *
     * @param values List of Bytes
     * @return an array of primitive bytes
     */
    private static byte[] toPrimitiveByteArray(List<Byte> values) {
        int valuesCount = values.size();
        byte[] primitiveArray = new byte[valuesCount];
        for (int i = 0; i < valuesCount; i++)
            primitiveArray[i] = values.get(i);
        return primitiveArray;
    }

    /**
     * Convert the given {@link PVByteArray} to {@link String}s, adding them to the provided {@link String} list
     *
     * @param array      the given {@link PVByteArray}
     * @param stringList the provided {@link String} list
     */
    private static void byteArrayToString(PVByteArray array, List<String> stringList) {
        IteratorByte it = array.get().iterator();
        while (it.hasNext()) {
            stringList.add(Byte.toString(it.nextByte()));
        }
    }

    /**
     * Convert a List of Short to an array of primitive shorts
     *
     * @param values List of Shorts
     * @return an array of primitive shorts
     */
    private static short[] toPrimitiveShortArray(List<Short> values) {
        int valuesCount = values.size();
        short[] primitiveArray = new short[valuesCount];
        for (int i = 0; i < valuesCount; i++)
            primitiveArray[i] = values.get(i);
        return primitiveArray;
    }

    /**
     * Convert the given {@link PVShortArray} to {@link String}s, adding them to the provided {@link String} list
     *
     * @param array      the given {@link PVShortArray}
     * @param stringList the provided {@link String} list
     */
    private static void shortArrayToString(PVShortArray array, List<String> stringList) {
        IteratorShort it = array.get().iterator();
        while (it.hasNext()) {
            stringList.add(Short.toString(it.nextShort()));
        }
    }

    /**
     * Convert a List of Integer to an array of primitive ints
     *
     * @param values List of Integers
     * @return an array of primitive ints
     */
    private static int[] toPrimitiveIntArray(List<Integer> values) {
        int valuesCount = values.size();
        int[] primitiveArray = new int[valuesCount];
        for (int i = 0; i < valuesCount; i++)
            primitiveArray[i] = values.get(i);
        return primitiveArray;
    }

    /**
     * Convert the given {@link PVIntArray} to {@link String}s, adding them to the provided {@link String} list
     *
     * @param array      the given {@link PVIntArray}
     * @param stringList the provided {@link String} list
     */
    private static void integerArrayToString(PVIntArray array, List<String> stringList) {
        IteratorInteger it = array.get().iterator();
        while (it.hasNext()) {
            stringList.add(Integer.toString(it.nextInt()));
        }
    }

    /**
     * Convert a List of Long to an array of primitive longs
     *
     * @param values List of Longs
     * @return an array of primitive longs
     */
    private static long[] toPrimitiveLongArray(List<Long> values) {
        int valuesCount = values.size();
        long[] primitiveArray = new long[valuesCount];
        for (int i = 0; i < valuesCount; i++)
            primitiveArray[i] = values.get(i);
        return primitiveArray;
    }

    /**
     * Convert the given {@link PVLongArray} to {@link String}s, adding them to the provided {@link String} list
     *
     * @param array      the given {@link PVLongArray}
     * @param stringList the provided {@link String} list
     */
    private static void longArrayToString(PVLongArray array, List<String> stringList) {
        IteratorLong it = array.get().iterator();
        while (it.hasNext()) {
            stringList.add(Long.toString(it.nextLong()));
        }
    }

    /**
     * Convert a float field to a string but also store the ieee value unchanged in the given {@link FloatArgument} list
     *
     * @param field             the float field
     * @param fieldPath         the absolute path to the field from the root
     * @param floatArgumentList the {@link FloatArgument} list to store the ieee value
     * @return the string representation of the floating point number
     */
    private static String floatFieldToString(PVFloat field, String fieldPath, List<FloatArgument> floatArgumentList) {
        String value;
        float f = field.get();
        value = Float.toString(f);
        floatArgumentList.add(new FloatArgument(fieldPath, f));
        return value;
    }

    /**
     * Convert a List of Float to an array of primitive floats
     *
     * @param values List of Floats
     * @return an array of primitive floats
     */
    private static float[] toPrimitiveFloatArray(List<Float> values) {
        int valuesCount = values.size();
        float[] primitiveArray = new float[valuesCount];
        for (int i = 0; i < valuesCount; i++)
            primitiveArray[i] = values.get(i);
        return primitiveArray;
    }

    /**
     * Convert the given {@link PVFloatArray} to {@link String}s, adding them to the provided {@link String} list
     * Also add all floats found to the provided {@link FloatArgument} list
     *
     * @param array             the given {@link PVFloatArray}
     * @param fieldPath         the absolute path to the field from the root
     * @param floatArgumentList the provided {@link FloatArgument} list
     * @param stringList        the provided  {@link String} list
     */
    private static void floatArrayToString(PVFloatArray array, String fieldPath, List<FloatArgument> floatArgumentList, List<String> stringList) {
        IteratorFloat it = array.get().iterator();
        int i = 0;
        while (it.hasNext()) {
            float f = it.nextFloat();
            floatArgumentList.add(new FloatArgument(fieldPath + "[" + i++ + "]", f));
            stringList.add(Float.toString(f));
        }
    }

    /**
     * Convert a double field to a string but also store the ieee value unchanged in the given {@link DoubleArgument} list
     *
     * @param field              the double field
     * @param fieldPath          the absolute path to the field from the root
     * @param doubleArgumentList the {@link DoubleArgument} list to store the ieee value
     * @return the string representation of the floating point number
     */
    private static String doubleFieldToString(PVDouble field, String fieldPath, List<DoubleArgument> doubleArgumentList) {
        String value;
        double d = field.get();
        value = Double.toString(d);
        doubleArgumentList.add(new DoubleArgument(fieldPath, d));
        return value;
    }

    /**
     * Convert a List of Double to an array of primitive doubles
     *
     * @param values List of Doubles
     * @return an array of primitive doubles
     */
    private static double[] toPrimitiveDoubleArray(List<Double> values) {
        int valuesCount = values.size();
        double[] primitiveArray = new double[valuesCount];
        for (int i = 0; i < valuesCount; i++)
            primitiveArray[i] = values.get(i);
        return primitiveArray;
    }

    /**
     * Convert the given {@link PVDoubleArray} to {@link String}s, adding them to the provided {@link String} list
     * Also add all doubles found to the provided {@link DoubleArgument} list
     *
     * @param array              the given {@link PVDoubleArray}
     * @param fieldPath          the absolute path to the field from the root
     * @param doubleArgumentList the provided {@link DoubleArgument} list
     * @param stringList         the provided  {@link String} list
     */
    private static void doubleArrayToString(PVDoubleArray array, String fieldPath, List<DoubleArgument> doubleArgumentList, List<String> stringList) {
        IteratorDouble it = array.get().iterator();
        int i = 0;
        while (it.hasNext()) {
            double d = it.nextDouble();
            doubleArgumentList.add(new DoubleArgument(fieldPath + "[" + i++ + "]", d));
            stringList.add(Double.toString(d));
        }
    }

    /**
     * Convert a string field to a string but quote it if it is not at the root level
     *
     * @param field     string field
     * @param fieldPath the absolute path to the field from the root
     * @return the string
     */
    private static String stringFieldToString(PVString field, String fieldPath) {
        String value;
        if (isSubField(fieldPath)) {
            // Subfield strings are always quoted
            value = "\"" + field.get() + "\"";
        } else {
            // Single top level can be left without quotes
            value = field.get();
        }
        return value;
    }

    /**
     * Convert a List of String to an array of strings
     *
     * @param strings List of Strings
     * @return an array of strings
     */
    private static String[] toStringArray(List<String> strings) {
        int valuesCount = strings.size();
        String[] stringArray = new String[valuesCount];
        for (int i = 0; i < valuesCount; i++) {
            stringArray[i] = strings.get(i);
        }
        return stringArray;
    }

    /**
     * Convert the given {@link PVArray} to {@link String}s, adding them to the provided {@link String} list
     *
     * @param array      the given {@link PVArray}
     * @param stringList the provided  {@link String} list
     */
    private static void stringArrayToString(PVArray array, List<String> stringList) {
        StringArrayData data = new StringArrayData();
        int offset = 0, len = array.getLength();
        while (offset < len) {
            int num = ((PVStringArray) array).get(offset, (len - offset), data);
            for (int i = 0; i < num; i++) {
                stringList.add("\"" + data.data[offset + i] + "\"");
            }
            offset += num;
        }
    }


    /**
     * Convert the given {@link PVArray} which represents a structure, to {@link String}s, adding them to the provided {@link String} list,
     * Also add all floats found to the provided {@link FloatArgument} list,
     * and add all doubles found to the provided {@link DoubleArgument} list
     *
     * @param array              the given {@link PVDoubleArray}
     * @param fieldPath          the absolute path to the field from the root
     * @param floatArgumentList  the provided {@link FloatArgument} list
     * @param doubleArgumentList the provided {@link DoubleArgument} list
     * @param stringList         the provided  {@link String} list
     */
    private static void structureArrayToString(PVArray array, String fieldPath, List<FloatArgument> floatArgumentList, List<DoubleArgument> doubleArgumentList, List<String> stringList) throws RPCRequestException {
        StructureArrayData data = new StructureArrayData();
        int offset = 0, len = array.getLength();
        while (offset < len) {
            int num = ((PVStructureArray) array).get(offset, (len - offset), data);
            for (int i = 0; i < num; i++) {
                stringList.add(fieldToString((data.data[offset + i]), fieldPath + "[" + offset + i + "]", floatArgumentList, doubleArgumentList));
            }
            offset += num;
        }
    }
}

