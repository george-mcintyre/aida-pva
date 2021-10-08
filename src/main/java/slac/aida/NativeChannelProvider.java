package slac.aida;

import edu.stanford.slac.aida.lib.model.AidaArgument;
import edu.stanford.slac.aida.lib.model.AidaTable;

import java.util.List;

public class NativeChannelProvider {

    /**
     * Called once to initialise the channel provider
     */
    static public native void aidaServiceInit();

    /**
     * Prototype to be implemented for Tables.  This is the most commonly implemented method.
     * Tables are simply a list of arbitrary objects.  The object is unpacked based on the
     * channel getterConfig.
     *
     * @param pvUri     the uri
     * @param arguments the arguments
     * @return a List of AidaTable to be tabulated either ROW_MAJOR (list of rows) or COLUMN_MAJOR (list of columns)
     */
    protected native AidaTable aidaRequestTable(String pvUri, List<AidaArgument> arguments);

    /**
     * Prototype to be implemented for setting values with no return value.
     *
     * @param pvUri     the uri
     * @param arguments the arguments
     */
    protected native void aidaSetValue(String pvUri, List<AidaArgument> arguments);

    /**
     * Prototype to be implemented for setting values with a Table as a return value.
     * Tables are simply a list of arbitrary objects.  The object is unpacked based on the
     * channel setterConfig.
     *
     * @param pvUri     the uri
     * @param arguments the arguments
     * @return a List of AidaTable to be tabulated either ROW_MAJOR (list of rows) or COLUMN_MAJOR (list of columns)
     */
    protected native AidaTable aidaSetValueWithResponse(String pvUri, List<AidaArgument> arguments);

    /**
     * Prototype to be implemented for Scalar Boolean
     *
     * @param pvUri     the uri
     * @param arguments the arguments
     * @return a Boolean
     */
    protected native boolean aidaRequestBoolean(String pvUri, List<AidaArgument> arguments);

    /**
     * Prototype to be implemented for Scalar Byte
     *
     * @param pvUri     the uri
     * @param arguments the arguments
     * @return a Byte
     */
    protected native byte aidaRequestByte(String pvUri, List<AidaArgument> arguments);

    /**
     * Prototype to be implemented for Scalar Short
     *
     * @param pvUri     the uri
     * @param arguments the arguments
     * @return a Short
     */
    protected native short aidaRequestShort(String pvUri, List<AidaArgument> arguments);

    /**
     * Prototype to be implemented for Scalar Integer
     *
     * @param pvUri     the uri
     * @param arguments the arguments
     * @return ab Integer
     */
    protected native int aidaRequestInteger(String pvUri, List<AidaArgument> arguments);

    /**
     * Prototype to be implemented for Scalar Long
     *
     * @param pvUri     the uri
     * @param arguments the arguments
     * @return a Long
     */
    protected native long aidaRequestLong(String pvUri, List<AidaArgument> arguments);

    /**
     * Prototype to be implemented for Scalar Float
     *
     * @param pvUri     the uri
     * @param arguments the arguments
     * @return a Float
     */
    protected native float aidaRequestFloat(String pvUri, List<AidaArgument> arguments);

    /**
     * Prototype to be implemented for Scalar Double
     *
     * @param pvUri     the uri
     * @param arguments the arguments
     * @return a Double
     */
    protected native double aidaRequestDouble(String pvUri, List<AidaArgument> arguments);

    /**
     * Prototype to be implemented for String
     *
     * @param pvUri     the uri
     * @param arguments the arguments
     * @return a String
     */
    protected native String aidaRequestString(String pvUri, List<AidaArgument> arguments);

    /**
     * Prototype to be implemented for Scalar Boolean Array
     *
     * @param pvUri     the uri
     * @param arguments the arguments
     * @return a Boolean Array
     */
    protected native boolean[] aidaRequestBooleanArray(String pvUri, List<AidaArgument> arguments);

    /**
     * Prototype to be implemented for Scalar Byte Array
     *
     * @param pvUri     the uri
     * @param arguments the arguments
     * @return a Byte Array
     */
    protected native byte[] aidaRequestByteArray(String pvUri, List<AidaArgument> arguments);

    /**
     * Prototype to be implemented for Scalar Short Array
     *
     * @param pvUri     the uri
     * @param arguments the arguments
     * @return a Short Array
     */
    protected native short[] aidaRequestShortArray(String pvUri, List<AidaArgument> arguments);

    /**
     * Prototype to be implemented for Scalar Integer Array
     *
     * @param pvUri     the uri
     * @param arguments the arguments
     * @return an Integer Array
     */
    protected native int[] aidaRequestIntegerArray(String pvUri, List<AidaArgument> arguments);

    /**
     * Prototype to be implemented for Scalar Long Array
     *
     * @param pvUri     the uri
     * @param arguments the arguments
     * @return a Long Array
     */
    protected native long[] aidaRequestLongArray(String pvUri, List<AidaArgument> arguments);

    /**
     * Prototype to be implemented for Scalar Float Array
     *
     * @param pvUri     the uri
     * @param arguments the arguments
     * @return a Float Array
     */
    protected native float[] aidaRequestFloatArray(String pvUri, List<AidaArgument> arguments);

    /**
     * Prototype to be implemented for Scalar Double Array
     *
     * @param pvUri     the uri
     * @param arguments the arguments
     * @return a Double Array
     */
    protected native double[] aidaRequestDoubleArray(String pvUri, List<AidaArgument> arguments);

    /**
     * Prototype to be implemented for String Array
     *
     * @param pvUri     the uri
     * @param arguments the arguments
     * @return a String Array
     */
    protected native String[] aidaRequestStringArray(String pvUri, List<AidaArgument> arguments);

}
