package edu.stanford.slac.aida.lib;

import edu.stanford.slac.aida.lib.model.*;
import edu.stanford.slac.except.*;
import org.epics.nt.NTURI;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvaccess.server.rpc.RPCService;
import org.epics.pvdata.pv.*;
import org.epics.util.array.*;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.List;

import static edu.stanford.slac.aida.lib.model.AidaType.*;
import static edu.stanford.slac.aida.lib.util.AidaPVHelper.*;
import static org.epics.pvdata.pv.Status.StatusType.ERROR;
import static org.epics.pvdata.pv.Type.scalar;
import static org.epics.pvdata.pv.Type.scalarArray;

public class AidaRPCService implements RPCService {
    private final ChannelProvider aidaChannelProvider;

    public AidaRPCService(ChannelProvider aidaChannelProvider) {
        this.aidaChannelProvider = aidaChannelProvider;
    }

    /**
     * Callback when a channel is called
     *
     * @param pvUri the uri passed to the channel containing the name, query, and arguments
     * @return the result of the call
     * @throws RPCRequestException              if any error occurs formulating the request or decoding the response
     * @throws AidaInternalException            if any error occurs because of an implementation error in aida server code
     * @throws MissingRequiredArgumentException when a required argument was not supplied
     * @throws UnableToGetDataException         when server fails to retrieve data
     * @throws UnableToSetDataException         when server fails to set data
     * @throws UnsupportedChannelException      when server does not yet support the specified channel.
     *                                          Usually caused when channel matches a pattern specified in the channels.yml file
     *                                          but is not yet supported in the service implementation
     */
    public PVStructure request(PVStructure pvUri) throws RPCRequestException, UnableToGetDataException, UnsupportedChannelException, UnableToSetDataException, AidaInternalException, MissingRequiredArgumentException {
        PVStructure retVal;
        try {
            // Check that the parameter is always a normative type
            String type = pvUri.getStructure().getID();
            if (!NTURI.is_a(pvUri.getStructure())) {
                String msg = "Unable to get data, unexpected request type: " + type;
                throw new RPCRequestException(ERROR, msg);
            }

            // Retrieve the PV name
            PVString pvPathField = pvUri.getStringField("path");
            if (pvPathField == null) {
                throw new RPCRequestException(ERROR, "unable to determine the channel from the request specified: " + pvUri);
            }
            String channelName = pvPathField.get();
            if (channelName == null || channelName.length() == 0) {
                throw new RPCRequestException(ERROR, "unable to determine the channel from the request specified: <blank>");
            }

            // Retrieve arguments, if any given to this RPC PV channel.
            PVStructure pvUriQuery = pvUri.getStructureField("query");
            List<AidaArgument> arguments = getArguments(pvUriQuery);

            // Make sure that only one request occurs at a time because
            // the implementations are not thread safe.
            synchronized (this) {
                retVal = request(channelName, arguments);
            }
        } catch (RPCRequestException e) {
            throw e;
        } catch (UnableToGetDataException e) {
            throw new RPCRequestException(ERROR, e.getMessage(), e);
        } catch (UnsupportedChannelException e) {
            throw new RPCRequestException(ERROR, e.getMessage(), e);
        } catch (UnableToSetDataException e) {
            throw new RPCRequestException(ERROR, e.getMessage(), e);
        } catch (AidaInternalException e) {
            throw new RPCRequestException(ERROR, e.getMessage(), e);
        } catch (MissingRequiredArgumentException e) {
            throw new RPCRequestException(ERROR, e.getMessage(), e);
        } catch (Throwable e) {
            throw new RPCRequestException(ERROR, e.getMessage(), e);
        }

        return retVal;
    }

    /**
     * Make request to the specified channel with the uri and arguments specified
     * and return the NT_TABLE of results.
     *
     * @param channelName   channel name
     * @param argumentsList arguments if any
     * @return the structure containing the results.
     * @throws AidaInternalException            if any error occurs because of an implementation error in aida server code
     * @throws MissingRequiredArgumentException when a required argument was not supplied
     * @throws UnableToGetDataException         when server fails to retrieve data
     * @throws UnableToSetDataException         when server fails to set data
     * @throws UnsupportedChannelException      when server does not yet support the specified channel.
     *                                          Usually caused when channel matches a pattern specified in the channels.yml file
     *                                          but is not yet supported in the service implementation
     */
    private PVStructure request(String channelName, List<AidaArgument> argumentsList) throws UnableToGetDataException, UnsupportedChannelException, UnableToSetDataException, AidaInternalException, MissingRequiredArgumentException {
        AidaChannelConfig getterConfig = aidaChannelProvider.getGetterConfig(channelName);
        AidaChannelConfig setterConfig = aidaChannelProvider.getSetterConfig(channelName);

        AidaType aidaGetterType = getterConfig == null ? NONE : getterConfig.getType();
        AidaType aidaSetterType = setterConfig == null ? NONE : setterConfig.getType();

        // Get special arguments `TYPE` and `VALUE` used to determine which APIs will be called
        String typeArgument = null;
        AidaArgument valueArgument = null;
        for (AidaArgument argument : argumentsList) {
            String argumentName = argument.getName();
            if (argumentName.equalsIgnoreCase("TYPE")) {
                typeArgument = argument.getValue().toUpperCase();
            } else if (argumentName.equalsIgnoreCase("VALUE")) {
                valueArgument = argument;
            }
        }

        boolean isSetterRequest = valueArgument != null;

        // Validate arguments
        for (AidaArgument argument : argumentsList) {
            String argumentName = argument.getName();
            if (aidaGetterType != NONE && !getterConfig.getArguments().contains(argumentName.toUpperCase())) {
                throw new UnsupportedChannelException(channelName + ":  " + argumentName + " is not a valid argument for get requests to this channel");
            } else if (aidaSetterType != NONE && !setterConfig.getArguments().contains(argumentName.toUpperCase())) {
                throw new UnsupportedChannelException(channelName + ":  " + argumentName + " is not a valid argument for set requests to this channel");
            }
        }

        // See if the request type is supported for the channel
        if (isSetterRequest && NONE.equals(aidaSetterType)) {
            throw new UnsupportedChannelTypeException(channelName + argumentsList + ".  Set requests are not supported for this channel");
        } else if (!isSetterRequest && NONE.equals(aidaGetterType)) {
            throw new UnsupportedChannelTypeException(channelName + argumentsList + ".  Get requests are not supported for this channel");
        }

        // If a type has been specified then override the appropriate Config's type with the specified one
        if (typeArgument != null) {
            try {
                AidaType specifiedAidaType = AidaType.valueOf(typeArgument);

                // Check that it matches the type class from the channels file
                // if channel accepts any type or if the specified type is TABLE (which matches any configuration)
                // or if the specified type is in the class of types allowed by the channel
                if (isSetterRequest) {
                    if (!TABLE.equals(specifiedAidaType) && !VOID.equals(specifiedAidaType)) {
                        throw new UnsupportedChannelTypeException(channelName + argumentsList + ".  The type specified by the 'Type' parameter must be either VOID or TABLE for setters, but you specified " + specifiedAidaType);
                    }
                    if (specifiedAidaType.equals(aidaSetterType) || ANY.equals(aidaSetterType)) {
                        aidaSetterType = specifiedAidaType;
                    } else {
                        throw new UnsupportedChannelTypeException(channelName + argumentsList + ".  The type specified by the 'Type' parameter is not compatible with the channel's definition (" + aidaSetterType + "), you specified " + specifiedAidaType);
                    }
                } else {
                    if (ANY.equals(aidaGetterType) ||
                            ((SCALAR.equals(aidaGetterType) || SCALAR_ARRAY.equals(aidaGetterType)) &&
                                    (TABLE.equals(specifiedAidaType) || specifiedAidaType.metaType().equals(aidaGetterType))) ||
                            (!SCALAR.equals(aidaGetterType) && !SCALAR_ARRAY.equals(aidaGetterType) && aidaGetterType.equals(specifiedAidaType))
                    ) {
                        aidaGetterType = specifiedAidaType;
                    } else {
                        throw new UnsupportedChannelTypeException(channelName + argumentsList + ".  The type specified by the 'Type' parameter must be a " + aidaGetterType + ", but you specified " + specifiedAidaType);
                    }
                }
            } catch (IllegalArgumentException e) {
                throw new UnsupportedChannelTypeException(channelName + argumentsList + ".  The type specified by the 'Type' parameter is not a recognised AIDA type: " + typeArgument);
            }
        } else {
            if (isSetterRequest) {
                // If a class of types is set as the getter type, but you didn't set one then error out
                if (aidaSetterType == ANY) {
                    throw new UnsupportedChannelTypeException(channelName + argumentsList + ".  The 'Type' parameter must be set 'VOID' or 'TABLE' but you didn't specify one");
                }
            } else {
                // If a class of types is set as the getter type, but you didn't set one then error out
                switch (aidaGetterType) {
                    case SCALAR:
                    case SCALAR_ARRAY:
                    case ANY:
                        throw new UnsupportedChannelTypeException(channelName + argumentsList + ".  The 'Type' parameter must be set to a type corresponding to " + aidaGetterType + ", but you didn't specify one");
                }
            }
        }

        // Get EPICS type from Aida Type
        Type channelGetterPVClass = typeOf(aidaGetterType);

        // Get Normative Type from Aida Type
        String channelGetterType = ntTypeOf(aidaGetterType);
        String channelSetterType = ntTypeOf(aidaSetterType);

        // If channelName contains a service (<service>::channelName) then remove the service part before calling the service implementation
        int servicePrefix = channelName.indexOf("::");
        if (servicePrefix != -1) {
            channelName = channelName.substring(servicePrefix + 2);
        }

        int indexOfLastLegacySeparator = channelName.lastIndexOf("//");
        if (indexOfLastLegacySeparator != -1) {
            channelName = channelName.substring(0, indexOfLastLegacySeparator) + ":" + channelName.substring(indexOfLastLegacySeparator + 2);
        }

        if (isSetterRequest) {
            System.out.println("AIDA SetValue: " + channelName + argumentsList + " => " + aidaSetterType + (channelSetterType == null ? "" : ("::" + channelSetterType)));
        } else {
            System.out.println("AIDA GetValue: " + channelName + argumentsList + " => " + aidaGetterType + (channelGetterType == null ? "" : ("::" + channelGetterType)));
        }

        // Make an arguments object to pass to requests
        AidaArguments arguments = new AidaArguments(argumentsList);

        // Call entry point based on return type
        if (isSetterRequest) {
            if (aidaSetterType.equals(VOID)) {
                this.aidaChannelProvider.setValue(channelName, arguments);
                return NT_SCALAR_EMPTY_STRUCTURE;
            } else if (aidaSetterType.equals(TABLE)) {
                if (setterConfig.getFields() == null) {
                    throw new AidaInternalException("Table channel configured without defining fields");
                }
                List<List<Object>> returnValue = this.aidaChannelProvider.setValueWithResponse(channelName, arguments);
                return asNtTable(returnValue, setterConfig);
            } else {
                throw new UnsupportedChannelException("Setters can only be VOID or TABLE but found: " + aidaSetterType.name());
            }
        } else {
            // Otherwise, this is a regular get request
            if (aidaGetterType.equals(NONE)) {
                throw new UnsupportedChannelException("Get is not allowed for this channel" + channelName);
            }

            if (channelGetterPVClass.equals(scalar)) {
                Object returnValue = this.aidaChannelProvider.requestScalar(channelName, aidaGetterType, arguments);
                return asScalar(returnValue);
            } else if (channelGetterPVClass.equals(scalarArray)) {
                List<?> returnValue = this.aidaChannelProvider.requestScalarArray(channelName, aidaGetterType, arguments);
                return asScalarArray(returnValue);
            } else {
                if (getterConfig.getFields() == null) {
                    throw new AidaInternalException("Table channel configured without defining fields");
                }
                List<List<Object>> returnValue = this.aidaChannelProvider.requestTable(channelName, arguments);
                return asNtTable(returnValue, getterConfig);
            }
        }
    }

    /**
     * Get the arguments for the specified request
     *
     * @param pvUriQuery the specified request
     * @return the list of aida channel arguments
     * @throws RPCRequestException if there is a problem reading the arguments
     */
    private List<AidaArgument> getArguments(PVStructure pvUriQuery) throws RPCRequestException {
        List<AidaArgument> arguments = new ArrayList<AidaArgument>();
        if (pvUriQuery != null) {
            PVField[] pvFields = pvUriQuery.getPVFields();
            for (PVField field : pvFields) {
                arguments.add(getArgument(field));
            }
        }
        return arguments;
    }

    /**
     * Convert a PVField into an AidaArgument
     *
     * @param field the field to convert into an AidaArgument
     * @return the AidaArgument
     * @throws RPCRequestException if something bad happens
     */
    private AidaArgument getArgument(PVField field) throws RPCRequestException {
        String name = field.getFieldName();
        if (name == null) {
            throw new RPCRequestException(ERROR, "Invalid argument name: <blank>");
        }

        // Convert all arguments to string (json or otherwise)
        // But collect all doubles and floats unchanged in ieee format
        List<FloatArgument> floatArgumentList = new ArrayList<FloatArgument>();
        List<DoubleArgument> doubleArgumentList = new ArrayList<DoubleArgument>();
        String value = fieldToString(field, name, floatArgumentList, doubleArgumentList);

        if (value == null) {
            throw new RPCRequestException(ERROR, "Invalid argument value: <blank>");
        }

        return new AidaArgument(name, value, floatArgumentList, doubleArgumentList);
    }

    /**
     * Extract the String value from the field
     * Any time a double or float is found, add it to the appropriate map under the `fieldMap` key
     *
     * @param field              the field
     * @param fieldPath          path under which this field sits.  Used only for floats and doubles that need
     *                           to set the path so the name in the pair is prefixed with this value if set
     * @param floatArgumentList  to return the list of floats found and the paths
     * @param doubleArgumentList to return the list of doubles found and the paths
     * @return the extracted value
     * @throws RPCRequestException of the field type is unsupported
     */
    private String fieldToString(PVField field, String fieldPath, List<FloatArgument> floatArgumentList, List<DoubleArgument> doubleArgumentList) throws RPCRequestException {
        String value;
        if (field instanceof PVBoolean) {
            value = Boolean.toString(((PVBoolean) field).get());
        } else if (field instanceof PVByte) {
            value = Byte.toString(((PVByte) field).get());
        } else if (field instanceof PVUByte) {
            value = Byte.toString(((PVUByte) field).get());
        } else if (field instanceof PVDouble) {
            double d = ((PVDouble) field).get();
            doubleArgumentList.add(new DoubleArgument(fieldPath, d));
            value = Double.toString(d);
        } else if (field instanceof PVFloat) {
            float f = ((PVFloat) field).get();
            floatArgumentList.add(new FloatArgument(fieldPath, f));
            value = Float.toString(f);
        } else if (field instanceof PVInt) {
            value = Integer.toString(((PVInt) field).get());
        } else if (field instanceof PVLong) {
            value = Long.toString(((PVLong) field).get());
        } else if (field instanceof PVShort) {
            value = Short.toString(((PVShort) field).get());
        } else if (field instanceof PVString) {
            if (isSubField(fieldPath)) {
                // Subfield strings are always quoted
                value = "\"" + ((PVString) field).get() + "\"";
            } else {
                // Single top level can be left without quotes
                value = ((PVString) field).get();
            }
        } else if (field instanceof PVStructure) {
            // For structures then we need to recurse for each field
            PVStructure structure = ((PVStructure) field);
            StringBuilder structureBuilder = new StringBuilder();
            structureBuilder.append("{");
            boolean firstTime = true;
            for (PVField subField : structure.getPVFields()) {
                String elementName = subField.getFieldName();
                String elementValue = fieldToString(subField, fieldPath + "." + elementName, floatArgumentList, doubleArgumentList);
                if (!firstTime) {
                    structureBuilder.append(", ");
                }
                firstTime = false;

                structureBuilder.append("\"");
                structureBuilder.append(elementName);
                structureBuilder.append("\": ");
                structureBuilder.append(elementValue);
            }
            structureBuilder.append("}");
            value = structureBuilder.toString();
        } else if (field instanceof PVArray) {
            PVArray array = ((PVArray) field);
            StringBuilder arrayBuilder = new StringBuilder();
            arrayBuilder.append("[");
            boolean firstTime = true;
            List<String> arrayList = arrayFields(array, fieldPath, floatArgumentList, doubleArgumentList);
            for (String subValue : arrayList) {
                if (!firstTime) {
                    arrayBuilder.append(", ");
                }
                firstTime = false;
                arrayBuilder.append(subValue);
            }
            arrayBuilder.append("]");
            value = arrayBuilder.toString();
        } else {
            throw new RPCRequestException(ERROR, "Invalid argument value: can only accept scalar, scalar array, structure or structure array");
        }
        return value;
    }

    /**
     * Extract array fields
     *
     * @param array              any PV array
     * @param fieldPath          path under which this field sits
     * @param floatArgumentList  to return the list of floats found and the paths
     * @param doubleArgumentList to return the list of doubles found and the paths
     * @return the list of strings for this array
     * @throws RPCRequestException when there is an unsupported type
     */
    private List<String> arrayFields(PVArray array, String fieldPath, List<FloatArgument> floatArgumentList, List<DoubleArgument> doubleArgumentList) throws RPCRequestException {
        List<String> arrayList = new ArrayList<String>();

        if (array instanceof PVBooleanArray) {
            // For boolean arrays
            BooleanArrayData data = new BooleanArrayData();
            int offset = 0, len = array.getLength();
            while (offset < len) {
                int num = ((PVBooleanArray) array).get(offset, (len - offset), data);
                for (int i = 0; i < num; i++) {
                    arrayList.add(Boolean.toString(data.data[offset + i]));
                }
                offset += num;
            }
        } else if (array instanceof PVByteArray) {
            // For byte arrays
            IteratorByte it = ((PVByteArray) array).get().iterator();
            while (it.hasNext()) {
                arrayList.add(Byte.toString(it.nextByte()));
            }
        } else if (array instanceof PVDoubleArray) {
            // For double arrays
            IteratorDouble it = ((PVDoubleArray) array).get().iterator();
            int i = 0;
            while (it.hasNext()) {
                double d = it.nextDouble();
                doubleArgumentList.add(new DoubleArgument(fieldPath + "[" + i++ + "]", d));
                arrayList.add(Double.toString(d));
            }
        } else if (array instanceof PVFloatArray) {
            // For float arrays
            IteratorFloat it = ((PVFloatArray) array).get().iterator();
            int i = 0;
            while (it.hasNext()) {
                float f = it.nextFloat();
                floatArgumentList.add(new FloatArgument(fieldPath + "[" + i++ + "]", f));
                arrayList.add(Float.toString(f));
            }
        } else if (array instanceof PVIntArray) {
            // For integer arrays
            IteratorInteger it = ((PVIntArray) array).get().iterator();
            while (it.hasNext()) {
                arrayList.add(Integer.toString(it.nextInt()));
            }
        } else if (array instanceof PVLongArray) {
            // For long arrays
            IteratorLong it = ((PVLongArray) array).get().iterator();
            while (it.hasNext()) {
                arrayList.add(Long.toString(it.nextLong()));
            }
        } else if (array instanceof PVShortArray) {
            // For short arrays
            IteratorShort it = ((PVShortArray) array).get().iterator();
            while (it.hasNext()) {
                arrayList.add(Short.toString(it.nextShort()));
            }
        } else if (array instanceof PVStringArray) {
            // For string arrays
            StringArrayData data = new StringArrayData();
            int offset = 0, len = array.getLength();
            while (offset < len) {
                int num = ((PVStringArray) array).get(offset, (len - offset), data);
                for (int i = 0; i < num; i++) {
                    arrayList.add("\"" + data.data[offset + i] + "\"");
                }
                offset += num;
            }
        } else if (array instanceof PVStructureArray) {
            // For structure arrays then recurse to unpack each field
            StructureArrayData data = new StructureArrayData();
            int offset = 0, len = array.getLength();
            while (offset < len) {
                int num = ((PVStructureArray) array).get(offset, (len - offset), data);
                for (int i = 0; i < num; i++) {
                    arrayList.add(fieldToString((data.data[offset + i]), fieldPath + "[" + offset + i + "]", floatArgumentList, doubleArgumentList));
                }
                offset += num;
            }
        }
        return arrayList;
    }

    private boolean isSubField(String fieldPath) {
        return fieldPath.contains(".") || fieldPath.contains("[");
    }

    /**
     * Is the given string numeric
     * Test by trying to convert to all numeric types and if any succeeds then return true
     *
     * @param strNum the string to check
     * @return true if the string is a number false otherwise
     */
    private boolean isNumeric(String strNum) {
        if (strNum == null) {
            return false;
        }

        try {
            Integer.parseInt(strNum);
            return true;
        } catch (NumberFormatException ignored) {
        }

        try {
            Float.parseFloat(strNum);
            return true;
        } catch (NumberFormatException ignored) {
        }
        try {
            Double.parseDouble(strNum);
            return true;
        } catch (NumberFormatException ignored) {
        }
        try {
            Long.parseLong(strNum);
            return true;
        } catch (NumberFormatException ignored) {
        }
        try {
            new BigInteger(strNum);
            return true;
        } catch (NumberFormatException ignored) {
        }
        return false;
    }
}
