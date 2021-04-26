package edu.stanford.slac.aida.lib.util.common;

import java.util.*;
import org.omg.CORBA.*;
import org.omg.CORBA.portable.*;
import edu.stanford.slac.err.*;			// Error issuance API

/**
 * <code>DaValue</code> is the AIDA overloaded data type, similar to a CORBA
 * Any but easier to use. <code>DaValue</code> makes it easy to construct and
 * pass complex data types through Aida. For instance a <code>DaValue</code>
 * may be used by an Aida Data Provider servant as a convenient way to construct
 * structured data to be returned by the servant's get - Any method. Also
 * <code>DaValue</code> may be returned by methods in <code>DaObject</code>
 * or <code>DaReference</code> that return data values.
 * 
 * <code>DaValue</code> extends the Java Vector class, and is organised as
 * follows:
 * <ol>
 * <li>If this DaValue's type is STRUCT - then the elements() of this vector
 * are themselves DaValues.
 * <li>If this DaValue's type is an Array type - the elements() of this vector
 * are objects of the base type of the array. toArray() returns an array of
 * these base types.
 * <li>If this DaValue's type is a base type - this vector contains exactly one
 * element of that type. firstElement() returns the object of this base type.
 * <li>Arrays of structures are represented as an array of DaValues containing
 * structures.
 * </ol>
 * 
 * See the dpTest Data Provider get - Any method for example of using
 * <code>DaValue</code> on a server, and test.Tests for example of the use of
 * DaValue in an Aida client.
 * 
 * @author gsm
 * @version 29-Sep-2004, Greg White (greg): Added exception handling
 * @version 15-Jun-2004, Greg White (greg): Bugfix for changed length array of strings.
 */
public class DaValue extends Vector implements Cloneable
{
    private static final boolean DEBUG = false;     // Adds runtime diagnostics
        
    private static Err err_;			// Error issuance object

    private static ORB orb;                		// Utility orb, for creating Anys

    private static final String[][] DAO_PROPS = {
        { "org.omg.CORBA.ORBClass", "com.ooc.CORBA.ORB" },
        { "org.omg.CORBA.ORBSingletonClass", "com.ooc.CORBA.ORBSingleton" } };

    private static boolean initialized = false;

    /**
     * The type of this vector of the DaValue. This is used as the
     * discriminator when evaluating a DaValue.
     */
    public int type; // The type of this node of the DaValue

    /**
     * Some DaValues can be named, notably when they are the elements of the
     * structure form of DaValues. In this case this field stores the name
     * of this element. Applications should use setName(String) to set the
     * name for a node even though the name field is public. Null means that
     * this vector is unnamed.
     */
    public String name; // The name if specified for this node of the
    // DaValue

    /**
     * These are the types understood by AIDA. Simple Types Arrays of Simple
     * types Structures (elements are a variable length list of any of the
     * previous types Any - A CORBA Any An array of Any
     * 
     * Values of these tokens must match those used in the Database as type
     * codes Loosly based on the CORBA type codes
     */
    public static class Type
    {
        /**
         * AIDA Structure
         */
        public final static int STRUCT = 0; // TCKind._tk_any heterogen

        /**
         * CORBA Any
         */
        public final static int ANY = 98; // TCKind._tk_any;

        /**
         * Array of CORBA Any
         */
        public final static int ANYA = 99; // TCKind._tk_any Array;
        // homogen

        /**
         * AIDA Boolean
         */
        public final static int BOOLEAN = 1; // TCKind._tk_boolean;

        /**9abfmpvx
         * AIDA Byte
         */
        public final static int BYTE = 2; // TCKind.tk_octet

        /**
         * Aida Character
         */
        public final static int CHAR = 3; // TCKind._tk_char;

        /**
         * AIDA Double
         */
        public final static int DOUBLE = 4; // TCKind._tk_double;

        /**
         * AIDA Float
         */
        public final static int FLOAT = 5; // TCKind._tk_float;

        /**
         * AIDA Long
         */
        public final static int LONG = 6; // TCKind._tk_long;

        /**
         * AIDA Long Double (Same as Double)
         */
        public final static int LONGDOUBLE = 7; // TCKind._tk_longdouble;

        /**
         * AIDA Long Long
         */
        public final static int LONGLONG = 8; // TCKind._tk_longlong;

        /**
         * AIDA Short
         */
        public final static int SHORT = 9; // TCKind._tk_short;

        /**
         * AIDA String
         */
        public final static int STRING = 10; // TCKind._tk_string;

        /**
         * AIDA Unsigned Long (same as Long)
         */
        public final static int ULONG = 11; // TCKind._tk_ulong;

        /**
         * AIDA Unsigned Long Long (Same as Long Long)
         */
        public final static int ULONGLONG = 12; // TCKind._tk_ulonglong;

        /**
         * AIDA Unsigned Short
         */
        public final static int USHORT = 13; // TCKind._tk_ushort;

        /**
         * AIDA Wide Character (Same as Character)
         */
        public final static int WCHAR = 14; // TCKind._tk_wchar;

        /**
         * AIDA Wide String (Same as String)
         */
        public final static int WSTRING = 15; // TCKind._tk_wstring;

        /**
         * AIDA Boolean Array
         */
        public final static int BOOLEANA = 51; // TCKind._tk_boolean;

        /**
         * AIDA Byte Array
         */
        public final static int BYTEA = 52; // TCKind.tk_octet

        /**
         * AIDA Character Array
         */
        public final static int CHARA = 53; // TCKind._tk_char;

        /**
         * AIDA Double Array
         */
        public final static int DOUBLEA = 54; // TCKind._tk_double;

        /**
         * AIDA Float Array
         */
        public final static int FLOATA = 55; // TCKind._tk_float;

        /**
         * AIDA Long Array
         */
        public final static int LONGA = 56; // TCKind._tk_long;

        /**
         * AIDA Long Double Array (Same As Double Array)
         */
        public final static int LONGDOUBLEA = 57; // TCKind._tk_longdouble;

        /**
         * AIDA Long Long Array (Same as Long Array)
         */
        public final static int LONGLONGA = 58; // TCKind._tk_longlong;

        /**
         * AIDA Short Array
         */
        public final static int SHORTA = 59; // TCKind._tk_short;

        /**
         * AIDA String Array
         */
        public final static int STRINGA = 60; // TCKind._tk_string;

        /**
         * AIDA Unsigned Long Array (Same as Long Array)
         */
        public final static int ULONGA = 61; // TCKind._tk_ulong;

        /**
         * AIDA Unsigned Long Long Array (Same as Long Array)
         */
        public final static int ULONGLONGA = 62; // TCKind._tk_ulonglong;

        /**
         * AIDA Unsigned Short Array
         */
        public final static int USHORTA = 63; // TCKind._tk_ushort;

        /**
         * AIDA Wide Character Array (Same as Character Array)
         */
        public final static int WCHARA = 64; // TCKind._tk_wchar;

        /**
         * AIDA Wide String Array (Same as String Array)
         */
        public final static int WSTRINGA = 65; // TCKind._tk_wstring;

        public final int code;

        public Type (int code)
        {
            this.code = code;
        }
    }

    /**
     * Create an empty DaValue
     */
    public DaValue()
    {
        super();
        init();
    }

    /**
     * Create an empty DaValue of specified name and type
     */
    public DaValue (String name, Type type)
    {
        super();
        init();

        if (name != null)
            this.name = new String (name);
        this.type = type.code;
    }

    /**
     * As with the super class, this constructor creates a DaValue with
     * initial capacity for storing the given number of elements in its
     * Vector. Do not confuse with the Constructor taking a Long argument.
     * 
     * @param initialCapacity
     *        Initial capacity. Vector initially has capacity for this
     *        number of elements
     * @see Vector(int)
     */
    public DaValue(int initialCapacity)
    {
        super(initialCapacity);
        init();
    }

    /**
     * As with the super class, this constructor creates a DaValue with
     * initial capacity for storing the given number of elements in its
     * Vector and capacity increases in increments of the given capacity
     * Increment
     * 
     * @param initialCapacity
     *        The initial capacity. Vector initially has capacity for this
     *        number of elements
     * @param capacityIncrement
     *        Each time capacity needs to be increased increase by this
     *        amount.
     * @see Vector.Vector(int, int)
     */
    public DaValue(int initialCapacity, int capacityIncrement)
    {
        super(initialCapacity, capacityIncrement);
        init();
    }

    /**
     * Initialisation for all new DaValues. sets up for issuing error messages,
     * inits the fields which say what this DaValue describes, and initializes 
     * the utility ORB, making sure ORBACUS is used rather than default Java orb.
     */
    private void init()
    {
        err_ = Err.getInstance();          // Get error issuance object
        name = null;
        type = -1;
        Properties m_props;
        m_props = System.getProperties();  // Get CORBA init properties
        if (!initialized)
        {
            for (int i = 0; i < DAO_PROPS.length; i++)
                m_props.put(DAO_PROPS[i][0], DAO_PROPS[i][1]);
            orb = ORB.init(new String[] { "" }, m_props); // as ORBACUS
            initialized = true;
        }
    }

    /**
     * Construct a DaValue from a Boolean
     * 
     * @param theValue
     *        Boolean value to be copied into the new DaValue's first and
     *        only element
     */
    public DaValue(Boolean theValue)
    {
        super();
        init();
        type = Type.BOOLEAN; // Boolean
        addElement(theValue);
    }

    /**
     * Construct a DaValue from a Byte
     * 
     * @param theValue
     *        Byte value to be copied into the new DaValue's first and only
     *        element
     */
    public DaValue(Byte theValue)
    {
        super();
        init();
        type = Type.BYTE; // Byte
        addElement(theValue);
    }

    /**
     * Construct a DaValue from a Character
     * 
     * @param theValue
     *        Character value to be copied into the new DaValue's first and
     *        only element
     */
    public DaValue(Character theValue)
    {
        super();
        init();
        type = Type.CHAR; // Character
        addElement(theValue);
    }

    /**
     * Construct a DaValue from a Double
     * 
     * @param theValue
     *        Double value to be copied into the new DaValue's first and
     *        only element
     */
    public DaValue(Double theValue)
    {
        super();
        init();
        type = Type.DOUBLE; // Double
        addElement(theValue);
    }

    /**
     * Construct a DaValue from a Float
     * 
     * @param theValue
     *        Float value to be copied into the new DaValue's first and only
     *        element
     */
    public DaValue(Float theValue)
    {
        super();
        init();
        type = Type.FLOAT; // Float
        addElement(theValue);
    }

    /**
     * Construct a DaValue from a Long
     * 
     * @param theValue
     *        Long value to be copied into the new DaValue's first and only
     *        element
     */
    public DaValue(Long theValue)
    {
        super();
        init();
        type = Type.LONG; // Long
        addElement(theValue);
    }

    /**
     * Construct a DaValue from a Short
     * 
     * @param theValue
     *        Short value to be copied into the new DaValue's first and only
     *        element
     */
    public DaValue(Short theValue)
    {
        super();
        init();
        type = Type.SHORT; // Short
        addElement(theValue);
    }

    /**
     * Construct a DaValue from a String
     * 
     * @param theValue
     *        String value to be copied into the new DaValue's first and
     *        only element
     */
    public DaValue(String theValue)
    {
        super();
        init();
        type = Type.STRING; // String
        addElement(theValue);
    }

    /**
     * Construct a DaValue from an Any
     * 
     * @param a
     *        CORBA Any that will be used to construct this new DaValue
     */
    public DaValue(Any a)
    {
        super();
        init();
        fromAny(this, a);
    }

    /**
     * Construct a DaValue from an Array of boolean
     * 
     * @param theArray
     *        boolean array whose elements are copied into corresponding
     *        Boolean elements in the Vector of this new DaValue
     */
    public DaValue(boolean[] theArray)
    {
        super();
        init();
        type = Type.BOOLEANA; // Array of boolean
        for (int i = 0; i < theArray.length; i++)
            addElement(new Boolean(theArray[i]));
    }

    /**
     * Construct a DaValue from an Array of byte
     * 
     * @param theArray
     *        byte array whose elements are copied into corresponding Byte
     *        elements in the Vector of this new DaValue
     */
    public DaValue(byte[] theArray)
    {
        super();
        init();
        type = Type.BYTEA; // Array of byte
        for (int i = 0; i < theArray.length; i++)
            addElement(new Byte(theArray[i]));
    }

    /**
     * Construct a DaValue from an Array of char
     * 
     * @param theArray
     *        char array whose elements are copied into corresponding
     *        Character elements in the Vector of this new DaValue
     */
    public DaValue(char[] theArray)
    {
        super();
        init();
        type = Type.CHARA; // Array of char
        for (int i = 0; i < theArray.length; i++)
            addElement(new Character(theArray[i]));
    }

    /**
     * Construct a DaValue from an Array of doubles
     * 
     * @param theArray
     *        double array whose elements are copied into corresponding
     *        Double elements in the Vector of this new DaValue
     */
    public DaValue(double[] theArray)
    {
        super();
        init();
        type = Type.DOUBLEA; // Array
        for (int i = 0; i < theArray.length; i++)
            addElement(new Double(theArray[i]));
    }

    /**
     * Construct a DaValue from an Array of floats
     * 
     * @param theArray
     *        float array whose elements are copied into corresponding Float
     *        elements in the Vector of this new DaValue
     */
    public DaValue(float[] theArray)
    {
        super();
        init();
        type = Type.FLOATA; // Array
        for (int i = 0; i < theArray.length; i++)
            addElement(new Float(theArray[i]));
    }

    /**
     * Construct a DaValue from an Array of long
     * 
     * @param theArray
     *        long array whose elements are copied into corresponding Long
     *        elements in the Vector of this new DaValue
     */
    public DaValue(long[] theArray)
    {
        super();
        init();
        type = Type.LONGA; // Array
        for (int i = 0; i < theArray.length; i++)
            addElement(new Long(theArray[i]));
    }

    /**
     * Construct a DaValue from an Array of integers
     * 
     * @param theArray
     *        int array whose elements are copied into corresponding Long
     *        elements in the Vector of this new DaValue
     */
    public DaValue(int[] theArray)
    {
        super();
        init();
        type = Type.LONGA; // Array
        for (int i = 0; i < theArray.length; i++)
            addElement(new Long(theArray[i]));
    }

    /**
     * Construct a DaValue from an Array of shorts
     * 
     * @param theArray
     *        short array whose elements are copied into corresponding Short
     *        elements in the Vector of this new DaValue
     */
    public DaValue(short[] theArray)
    {
        super();
        init();
        type = Type.SHORTA; // Array of short
        for (int i = 0; i < theArray.length; i++)
            addElement(new Short(theArray[i]));
    }

    /**
     * Construct a DaValue from an Array of String objects
     * 
     * @param theArray
     *        String array whose elements are copied into corresponding
     *        String elements in the Vector of this new DaValue
     */
    public DaValue(String[] theArray)
    {
        super();
        init();
        addArray(theArray, Type.STRINGA);
    }

    /**
     * Construct a DaValue from an Array of Boolean objects
     * 
     * @param theArray
     *        Boolean array whose elements are copied into corresponding
     *        Boolean elements in the Vector of this new DaValue
     */
    public DaValue(Boolean[] theArray)
    {
        super();
        init();
        addArray(theArray, Type.BOOLEANA);
    }

    /**
     * Construct a DaValue from an Array of byte
     * 
     * @param theArray
     *        Byte array whose elements are copied into corresponding Byte
     *        elements in the Vector of this new DaValue
     */
    public DaValue(Byte[] theArray)
    {
        super();
        init();
        addArray(theArray, Type.BYTEA);
    }

    /**
     * Construct a DaValue from an Array of Character objects
     * 
     * @param theArray
     *        Character array whose elements are copied into corresponding
     *        Character elements in the Vector of this new DaValue
     */
    public DaValue(Character[] theArray)
    {
        super();
        init();
        addArray(theArray, Type.CHARA);
    }

    /**
     * Construct a DaValue from an Array of Double objects
     * 
     * @param theArray
     *        Double array whose elements are copied into corresponding
     *        Double elements in the Vector of this new DaValue
     */
    public DaValue(Double[] theArray)
    {
        super();
        init();
        addArray(theArray, Type.DOUBLEA);
    }

    /**
     * Construct a DaValue from an Array of Float objects
     * 
     * @param theArray
     *        Float array whose elements are copied into corresponding Float
     *        elements in the Vector of this new DaValue
     */
    public DaValue(Float[] theArray)
    {
        super();
        init();
        addArray(theArray, Type.FLOATA);
    }

    /**
     * Construct a DaValue from an Array of Long objects
     * 
     * @param theArray
     *        Long array whose elements are copied into corresponding Long
     *        elements in the Vector of this new DaValue
     */
    public DaValue(Long[] theArray)
    {
        super();
        init();
        addArray(theArray, Type.LONGA);
    }

    /**
     * Construct a DaValue from an Array of Integer objects
     * 
     * @param theArray
     *        Integer array whose elements are copied into corresponding
     *        Long elements in the Vector of this new DaValue
     */
    public DaValue(Integer[] theArray)
    {
        super();
        init();
        type = Type.LONGA; // Array
        for (int i = 0; i < theArray.length; i++)
            addElement(new Long(theArray[i].longValue()));
    }

    /**
     * Construct a DaValue from an Array of Short objects
     * 
     * @param theArray
     *        Short array whose elements are copied into corresponding Short
     *        elements in the Vector of this new DaValue
     */
    public DaValue(Short[] theArray)
    {
        super();
        init();
        addArray(theArray, Type.SHORTA);
    }

    /**
     * Construct a DaValue from an arbitrary Array of objects
     * 
     * @param o
     *        arbitrary array whose elements are copied into corresponding
     *        elements in the Vector of this DaValue
     * @param theType
     *        the type of the elements of the array
     */
    public void addArray(java.lang.Object [] o, int theType)
    {
        type = theType;
        addArray(this, o);
    }

    /**
     * Add arbitrary array to given DaValue. Caller is expected to set the
     * type for given DaValue
     * 
     * @param v
     *        given DaValue. Elements in given array are added to this
     *        DaValue
     * @param o
     *        arbitrary array whose elements are copied into corresponding
     *        elements in the Vector of this DaValue
     */
    private void addArray(DaValue v, java.lang.Object [] a)
    {
        for (int i = 0; i < a.length; i++)
            v.addElement(a[i]);
    }

    /**
     * Set the name of this vector of the DaValue
     * 
     * @param theName
     *        name to set for this vector
     */
    public void setName(String theName)
    {
        name = theName;
    }



    /**
       Returns the basic element of this DaValue that is considered the
       "primary" one.  Currently, this translates to the first scalar or
       vector of basic types discovered by iterating through the contents.
       In effect, this means that the first such element inserted into the
       container has primacy.
    */
    public DaValue getPrimaryElement ()
    {
        if (type != Type.STRUCT)
            return this;

        return ((DaValue)firstElement()).getPrimaryElement();
    }

    

    /**
     * Static method that fills a DaValue from a CORBA Any that contains a
     * HOMOgenous array of Any's or strings. The function is HETERO because
     * the Any's contains heterogenous types/structures.
     * 
     * @param v
     *        DaValue to store decoded CORBA Any
     * @param a
     *        CORBA Any to decode into the DaValue.
     * @param tc
     *        CORBA TypeCode discriminator that describes the given CORBA
     *        Any
     * @return DaValue that represents decoded CORBA Any
     * @throws IllegalArgumentException
     * 		  Error in string comparison operation on TypeCode.id
     */
    public static DaValue fromAnyHeteroArray(DaValue v, Any a, TypeCode tc)
    {
        boolean isIdlStruct = false;
        try
        { // TODO: do without string compare
            if (DEBUG)
                System.out.println("In fromAnyHeteroArray");
            if (isIdlStruct = tc.id().substring(0, 4).equals("IDL:"))
                v.type = Type.STRUCT;
            else if (tc.id().equalsIgnoreCase("AIDA:Struct/1.0"))
                v.type = Type.STRUCT;
            else
                v.type = Type.STRINGA;
        }
        catch (Exception ex)
        {
            throw (IllegalArgumentException) 
                err_.log(new IllegalArgumentException().initCause(ex), 
                         " when determining DaValue type");
        }

        try
        {
            org.omg.CORBA.portable.InputStream inputStream = a
                .create_input_stream();
            int count = tc.member_count();
            for (int i = 0; count-- > 0; i++)
            {
                // if (v.type == DaSeqValue.Type.STRINGA)
                //    v.addElement(inputStream.read_string()));
                if (isIdlStruct || v.type == DaValue.Type.STRINGA)
                {
                    DaValue d = new DaValue(tc.member_type(i), inputStream);
                    d.name = tc.member_name(i);
                    v.addElement(d);
                } else
                {
                    DaValue d = new DaValue(inputStream.read_any());
                    d.name = tc.member_name(i);
                    v.addElement(d);
                }
            }
        }
        catch (Exception ex)
        {
            throw (IllegalArgumentException) 
                err_.log(new IllegalArgumentException().initCause(ex), 
                         " while unpacking structure into DaValue");
        }
        return v;
    }

    /**
     * Constuctor from a structure member
     * 
     * @param m
     *        StructureMember
     * @return new DaValue
     */
    public DaValue(TypeCode tc, org.omg.CORBA.portable.InputStream inputStream)
    {
        TCKind tk = tc.kind();
        int tv = tk.value();
        if (DEBUG)
            System.out.println("In DaValue(TypeCode, inputStream");
        try
        {
            switch (tv)
            {
                // Base types
            case TCKind._tk_boolean:
                type = DaValue.Type.BOOLEAN;
                addElement(new Boolean(inputStream.read_boolean()));
                break;
            case TCKind._tk_octet:
                type = DaValue.Type.BYTE;
                addElement(new Byte(inputStream.read_octet()));
                break;
            case TCKind._tk_char:
                type = DaValue.Type.CHAR;
                addElement(new Character(inputStream.read_char()));
                break;
            case TCKind._tk_double:
                type = DaValue.Type.DOUBLE;
                addElement(new Double(inputStream.read_double()));
                break;
            case TCKind._tk_float:
                type = DaValue.Type.FLOAT;
                addElement(new Float(inputStream.read_float()));
                break;
            case TCKind._tk_long:
                type = DaValue.Type.LONG;
                addElement(new Long(inputStream.read_long()));
                break;
            case TCKind._tk_longdouble:
                type = DaValue.Type.LONGDOUBLE;
                addElement(new Double(inputStream.read_double()));
                break;
            case TCKind._tk_longlong:
                type = DaValue.Type.LONGLONG;
                addElement(new Long(inputStream.read_long()));
                break;
            case TCKind._tk_short:
                type = DaValue.Type.SHORT;
                addElement(new Short(inputStream.read_short()));
                break;
            case TCKind._tk_string:
                type = DaValue.Type.STRING;
                addElement(new String(inputStream.read_string()));
                break;
            case TCKind._tk_ulong:
                type = DaValue.Type.ULONG;
                addElement(new Long(inputStream.read_ulong()));
                break;
            case TCKind._tk_ulonglong:
                type = DaValue.Type.ULONGLONG;
                addElement(new Long(inputStream.read_ulonglong()));
                break;
            case TCKind._tk_ushort:
                type = DaValue.Type.USHORT;
                addElement(new Short(inputStream.read_ushort()));
                break;
            case TCKind._tk_wchar:
                type = DaValue.Type.WCHAR;
                addElement(new Character(inputStream.read_wchar()));
                break;
            case TCKind._tk_wstring:
                type = DaValue.Type.WSTRING;
                addElement(new String(inputStream.read_wstring()));
                break;
                // Multitype array
                //		case TCKind._tk_struct:
                //		    fromAnyHeteroArray(this, a, tc);
                //                  break;
                // Monotype array
            case TCKind._tk_array:
            case TCKind._tk_alias:
                fromStreamArray(this, inputStream, tc);
                break;
                // TODO: Untested
            case TCKind._tk_any:
                fromAny(this, inputStream.read_any());
                break;
                // TODO: Unsupported types
                // To support all types of Any including IDL
                // defined structures
                // need to include support for the following
                // types
            case TCKind._tk_enum:
            case TCKind._tk_except:
            case TCKind._tk_fixed:
            case TCKind._tk_null:
            case TCKind._tk_native:
            case TCKind._tk_objref:
            case TCKind._tk_value:
            case TCKind._tk_value_box:
            case TCKind._tk_union:
            case TCKind._tk_void:
            case TCKind._tk_sequence:
            default:
                throw new UnsupportedOperationException(
                    "Unsupported type found in Any to DaValue conversion");
         
            }
        }
        catch (Exception ex)
        {
            throw (IllegalArgumentException) 
                err_.log( new IllegalArgumentException().initCause(ex),
                          "Constructing DaValue from inputstream");
        }
    }

    /**
     * Static method that fills a DaValue from a CORBA Any that contains a
     * HOMOgenous array of Basic types.
     * 
     * @param v
     *        DaValue to store decoded CORBA Any
     * @param a
     *        CORBA Any to decode into the DaValue.
     * @param tc
     *        CORBA TypeCode discriminator that describes the given CORBA
     *        Any
     * @return DaValue that represents decoded CORBA Any
     */
    static public void fromStreamArray(DaValue v,
                                       org.omg.CORBA.portable.InputStream inputStream, TypeCode ptc)
    {
        if (DEBUG)
            System.out.println("In fromStreamArray");
        int i;
        try
        {
            TypeCode tc = ptc.content_type().content_type();
            TCKind tk = tc.kind();
            int tv = tk.value();
            switch (tv)
            {
            case TCKind._tk_boolean:
            {
                v.type = DaValue.Type.BOOLEANA;
                boolean[] ary = BooleanSeqHelper.read(inputStream);
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Boolean(ary[i]));
                break;
            }
            case TCKind._tk_octet:
            {
                v.type = DaValue.Type.BYTEA;
                byte[] ary = OctetSeqHelper.read(inputStream);
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Byte(ary[i]));
                break;
            }
            case TCKind._tk_char:
            {
                v.type = DaValue.Type.CHARA;
                char[] ary = CharSeqHelper.read(inputStream);
                inputStream.read_char_array(ary, 0, tc.length());
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Character(ary[i]));
                break;
            }
            case TCKind._tk_double:
            {
                v.type = DaValue.Type.DOUBLEA;
                double[] ary = DoubleSeqHelper.read(inputStream);
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Double(ary[i]));
                break;
            }
            case TCKind._tk_float:
            {
                v.type = DaValue.Type.FLOATA;
                float[] ary = FloatSeqHelper.read(inputStream);
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Float(ary[i]));
                break;
            }
            case TCKind._tk_long:
            {
                v.type = DaValue.Type.LONGA;
                int[] ary = LongSeqHelper.read(inputStream);
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Long(ary[i]));
                break;
            }
            case TCKind._tk_longdouble:
            {
                v.type = DaValue.Type.LONGDOUBLEA;
                double[] ary = DoubleSeqHelper.read(inputStream);
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Double(ary[i]));
                break;
            }
            case TCKind._tk_longlong:
            {
                v.type = DaValue.Type.LONGLONGA;
                long[] ary = LongLongSeqHelper.read(inputStream);
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Long(ary[i]));
                break;
            }
            case TCKind._tk_short:
            {
                v.type = DaValue.Type.SHORTA;
                short[] ary = ShortSeqHelper.read(inputStream);
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Short(ary[i]));
                break;
            }
            case TCKind._tk_ulong:
            {
                v.type = DaValue.Type.ULONGA;
                int[] ary = ULongSeqHelper.read(inputStream);
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Long(ary[i]));
                break;
            }
            case TCKind._tk_ulonglong:
            {
                v.type = DaValue.Type.ULONGLONGA;
                long[] ary = ULongLongSeqHelper.read(inputStream);
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Long(ary[i]));
                break;
            }
            case TCKind._tk_ushort:
            {
                v.type = DaValue.Type.USHORTA;
                short[] ary = UShortSeqHelper.read(inputStream);
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Short(ary[i]));
                break;
            }
            case TCKind._tk_wchar:
            {
                v.type = DaValue.Type.WCHARA;
                char[] ary = WCharSeqHelper.read(inputStream);
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Character(ary[i]));
                break;
            }
            case TCKind._tk_string:
                v.addArray(StringSeqHelper.read(inputStream),
                           DaValue.Type.STRINGA);
                break;
                // TOTO: Untested
            case TCKind._tk_any:
            {
                v.type = DaValue.Type.STRUCT;
                Any[] ary = AnySeqHelper.read(inputStream);
                for (i = 0; i < ary.length; i++)
                    v.addElement(fromAny(new DaValue(), ary[i]));
                break;
            }
            // Unsupported types
            // To support all types of Any including IDL
            // defined structures
            // need to include support for the following
            // types
            case TCKind._tk_alias:
            case TCKind._tk_enum:
            case TCKind._tk_except:
            case TCKind._tk_fixed:
            case TCKind._tk_null:
            case TCKind._tk_native:
            case TCKind._tk_objref:
            case TCKind._tk_value:
            case TCKind._tk_value_box:
            case TCKind._tk_union:
            case TCKind._tk_void:
            case TCKind._tk_sequence:
            default:
                throw new UnsupportedOperationException(
                    "Unsupported TCKind found in TypeCode stream:" + tc.id());
                                   
            }
        }
        catch (Exception ex)
        {
            throw (IllegalArgumentException) 
                err_.log( (new IllegalArgumentException().initCause(ex)),
                          "Constructing DaValue from inputstream given typecode" + 
                          ": TypeCode = " + ptc.toString() +
                          " while unpacking array into DaValue");
        }
    }

    /**
     * Static method that fills a DaValue from a CORBA Any that contains a
     * HOMOgenous array of Basic types.
     * 
     * @param v
     *        DaValue to store decoded CORBA Any
     * @param a
     *        CORBA Any to decode into the DaValue.
     * @param tc
     *        CORBA TypeCode discriminator that describes the given CORBA
     *        Any
     * @return DaValue that represents decoded CORBA Any
     */
    public static DaValue fromAnyHomoArray(DaValue v, Any a, TypeCode tc)
    {
        if (DEBUG)
            System.out.println("In fromAnyHomoArray");
        int i;
        try
        {
            org.omg.CORBA.portable.InputStream inputStream = a
                .create_input_stream();
            int tv = tc.content_type().kind().value();
            switch (tv)
            {
            case TCKind._tk_boolean:
            {
                v.type = DaValue.Type.BOOLEANA;
                boolean[] ary = new boolean[tc.length()];
                inputStream.read_boolean_array(ary, 0, tc.length());
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Boolean(ary[i]));
                break;
            }
            case TCKind._tk_octet:
            {
                v.type = DaValue.Type.BYTEA;
                byte[] ary = new byte[tc.length()];
                inputStream.read_octet_array(ary, 0, tc.length());
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Byte(ary[i]));
                break;
            }
            case TCKind._tk_char:
            {
                v.type = DaValue.Type.CHARA;
                char[] ary = new char[tc.length()];
                inputStream.read_char_array(ary, 0, tc.length());
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Character(ary[i]));
                break;
            }
            case TCKind._tk_double:
            {
                v.type = DaValue.Type.DOUBLEA;
                double[] ary = new double[tc.length()];
                inputStream.read_double_array(ary, 0, tc.length());
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Double(ary[i]));
                break;
            }
            case TCKind._tk_float:
            {
                v.type = DaValue.Type.FLOATA;
                float[] ary = new float[tc.length()];
                inputStream.read_float_array(ary, 0, tc.length());
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Float(ary[i]));
                break;
            }
            case TCKind._tk_long:
            {
                v.type = DaValue.Type.LONGA;
                int[] ary = new int[tc.length()];
                inputStream.read_long_array(ary, 0, tc.length());
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Long(ary[i]));
                break;
            }
            case TCKind._tk_longdouble:
            {
                v.type = DaValue.Type.LONGDOUBLEA;
                double[] ary = new double[tc.length()];
                inputStream.read_double_array(ary, 0, tc.length());
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Double(ary[i]));
                break;
            }
            case TCKind._tk_longlong:
            {
                v.type = DaValue.Type.LONGLONGA;
                long[] ary = new long[tc.length()];
                inputStream.read_longlong_array(ary, 0, tc.length());
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Long(ary[i]));
                break;
            }
            case TCKind._tk_short:
            {
                v.type = DaValue.Type.SHORTA;
                short[] ary = new short[tc.length()];
                inputStream.read_short_array(ary, 0, tc.length());
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Short(ary[i]));
                break;
            }
            case TCKind._tk_string:
            {
                if (DEBUG)
                    System.out.println("Trying to extract string");
                v.addArray(StringSeqHelper.extract(a),
                           DaValue.Type.STRINGA);
                v.type = DaValue.Type.STRINGA;
                // v.addElement( a.extract_string() );
                break;
            }
            case TCKind._tk_ulong:
            {
                v.type = DaValue.Type.ULONGA;
                int[] ary = new int[tc.length()];
                inputStream.read_ulong_array(ary, 0, tc.length());
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Long(ary[i]));
                break;
            }
            case TCKind._tk_ulonglong:
            {
                v.type = DaValue.Type.ULONGLONGA;
                long[] ary = new long[tc.length()];
                inputStream.read_ulonglong_array(ary, 0, tc.length());
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Long(ary[i]));
                break;
            }
            case TCKind._tk_ushort:
            {
                v.type = DaValue.Type.USHORTA;
                short[] ary = new short[tc.length()];
                inputStream.read_ushort_array(ary, 0, tc.length());
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Short(ary[i]));
                break;
            }
            case TCKind._tk_wchar:
            {
                v.type = DaValue.Type.WCHARA;
                char[] ary = new char[tc.length()];
                inputStream.read_wchar_array(ary, 0, tc.length());
                for (i = 0; i < ary.length; i++)
                    v.addElement(new Character(ary[i]));
                break;
            }
            case TCKind._tk_wstring:
            {
                v.type = DaValue.Type.STRING;
                v.addElement(a.extract_wstring());
                break;
            }
            // Unsupported types
            // To support all types of Any including IDL
            // defined structures
            // need to include support for the following
            // types
            case TCKind._tk_alias:
            case TCKind._tk_any:
            case TCKind._tk_enum:
            case TCKind._tk_except:
            case TCKind._tk_fixed:
            case TCKind._tk_null:
            case TCKind._tk_native:
            case TCKind._tk_objref:
            case TCKind._tk_value:
            case TCKind._tk_value_box:
            case TCKind._tk_union:
            case TCKind._tk_void:
            case TCKind._tk_sequence:
            default: 
                throw new UnsupportedOperationException(
                    "Unsupported TCKind found in TypeCode stream:" + tc.id());
            }
        }
        catch (Exception ex)
        {
            throw (IllegalArgumentException) 
                err_.log( (new IllegalArgumentException().initCause(ex)),
                          "Constructing DaValue from inputstream given typecode" + 
                          ": TypeCode = " + tc.toString() +
                          " while unpacking Any homogeneous array into DaValue");
        }
        return v;
    }

    /**
     * Static method that fills a DaValue from a CORBA Any. Used with the
     * Any constructor to recursively add elements from an Any. This method
     * understands Any's that are composed as follows: A node can contain
     * either a simple type, a structure, or an array. Simple types are
     * terminal and once processed execution pops a level. Arrays
     * (TCKind._tk_array) have a fixed length and are monotype and are
     * streamed into variables of corresponding type (NB arrays of
     * structures are not allowed). They will appear as a structure of
     * structures. Structures (TCKind._tk_struct) are structures with
     * elements that are of any type including Any (recursive)
     * 
     * @param v
     *        DaValue to store decoded CORBA Any
     * @param a
     *        CORBA Any to decode into the DaValue.
     * @return DaValue that represents decoded CORBA Any
     */
    public static DaValue fromAny(DaValue v, Any a)
    {
        if (DEBUG)
            System.out.println("In fromAny");
        TypeCode tc = a.type();
        TCKind tk = tc.kind();
        int tv = tk.value();
        try
        {
            switch (tv)
            {
                // Base types
            case TCKind._tk_boolean:
                v.type = DaValue.Type.BOOLEAN;
                v.addElement(new Boolean(a.extract_boolean()));
                break;
            case TCKind._tk_octet:
                v.type = DaValue.Type.BYTE;
                v.addElement(new Byte(a.extract_octet()));
                break;
            case TCKind._tk_char:
                v.type = DaValue.Type.CHAR;
                v.addElement(new Character(a.extract_char()));
                break;
            case TCKind._tk_double:
                v.type = DaValue.Type.DOUBLE;
                v.addElement(new Double(a.extract_double()));
                break;
            case TCKind._tk_float:
                v.type = DaValue.Type.FLOAT;
                v.addElement(new Float(a.extract_float()));
                break;
            case TCKind._tk_long:
                v.type = DaValue.Type.LONG;
                v.addElement(new Long(a.extract_long()));
                break;
            case TCKind._tk_longdouble:
                v.type = DaValue.Type.LONGDOUBLE;
                v.addElement(new Double(a.extract_double()));
                break;
            case TCKind._tk_longlong:
                v.type = DaValue.Type.LONGLONG;
                v.addElement(new Long(a.extract_longlong()));
                break;
            case TCKind._tk_short:
                v.type = DaValue.Type.SHORT;
                v.addElement(new Short(a.extract_short()));
                break;
            case TCKind._tk_string:
                v.type = DaValue.Type.STRING;
                v.addElement(new String(a.extract_string()));
                break;
            case TCKind._tk_ulong:
                v.type = DaValue.Type.ULONG;
                v.addElement(new Long(a.extract_ulong()));
                break;
            case TCKind._tk_ulonglong:
                v.type = DaValue.Type.ULONGLONG;
                v.addElement(new Long(a.extract_ulonglong()));
                break;
            case TCKind._tk_ushort:
                v.type = DaValue.Type.USHORT;
                v.addElement(new Short(a.extract_ushort()));
                break;
            case TCKind._tk_wchar:
                v.type = DaValue.Type.WCHAR;
                v.addElement(new Character(a.extract_wchar()));
                break;
            case TCKind._tk_wstring:
                v.type = DaValue.Type.WSTRING;
                v.addElement(new String(a.extract_wstring()));
                break;
                // Multitype array
            case TCKind._tk_struct:
                return fromAnyHeteroArray(v, a, tc);
                // Monotype array
            case TCKind._tk_array:
                return fromAnyHomoArray(v, a, tc);
            case TCKind._tk_alias:
                return fromAnyHomoArray(v, a, tc.content_type());
                // Unsupported types.
                // To support all types of Any including IDL
                // defined structures need to include support for the following
                // types

            case TCKind._tk_any:
            case TCKind._tk_enum:
            case TCKind._tk_except:
            case TCKind._tk_fixed:
            case TCKind._tk_null:
            case TCKind._tk_native:
            case TCKind._tk_objref:
            case TCKind._tk_value:
            case TCKind._tk_value_box:
            case TCKind._tk_union:
            case TCKind._tk_void:
            case TCKind._tk_sequence:
            default:
                throw new UnsupportedOperationException(
                    "Unsupported TCKind found in TypeCode Any:" + tc.id());
            }
        }
        catch (Exception ex)
        {
            throw (IllegalArgumentException) 
                err_.log( (new IllegalArgumentException().initCause(ex)),
                          "Constructing DaValue from Any encountered unsupported " + 
                          "TypeCode = " + tc.toString());
        }
        return v;
    }

    /**
     * Create an Any from this DaValue Encodes this DaValue into a CORBA Any
     * 
     * @return Returns a new CORBA Any that is constituted from the elements
     *         of this DaValue
     */
    public Any toAny()
    {
        if (DEBUG)
            System.out.println("In Any toAny()");
        Any a = orb.create_any();
        return toAny(this, a);
    }

    /**
     * Static method to create an Any that represents an arbitrary DaValue
     * 
     * @param v
     *        DaValue to decode
     * @param a
     *        CORBA Any to write decoded DaValue into.
     * @return Returns a new CORBA Any that is constituted of the arbitrary
     *         contents of this DaValue's String array
     */
    public static Any toAny(DaValue v, Any a)
    {
        if (DEBUG)
            System.out.println("In static toAny");
        switch (v.type)
        {
            // Structure
        case Type.STRUCT:
            return DaValue.toAnyStructure(v, a);
            // String array
        case Type.STRINGA:
        case Type.WSTRINGA:
            return DaValue.toAnyStringArray(v, a);
            // Basic or Array type
        default:
            return DaValue.toAnyBasicOrArray(v, a);
        }
    }

    /**
     * Static method to create an Any that represents a DaValue structure
     * 
     * @param v
     *        DaValue to decode
     * @param a
     *        CORBA Any to write decoded DaValue into.
     * @return Returns a new CORBA Any that is constituted from the elements
     *         of this DaValue structure
     */
    private static Any toAnyStructure(DaValue v, Any a)
    {
        if (DEBUG)
            System.out.println("In toAnyStructure");
        DaValue d; // Scratch DaValue
        int dLen = v.size(); // Elements in this vector
        int i = 0;

        // Create a new CORBA structure to hold the DaValue Structure
        String typeName = "IDL:DaValue";
        try
        {
            StructMember[] members = new StructMember[dLen];
            OutputStream outputStream = a.create_output_stream();

            // For each member add it to the structure
            for (Enumeration e = v.elements(); e.hasMoreElements(); i++)
            {
                d = (DaValue) e.nextElement();
                Any ta = DaValue.toAny(d, orb.create_any());
                outputStream.write_any(ta);
                members[i] = new StructMember();
                members[i].name = (d.name == null) ? ("unnamed" + i) : d.name;
//                members[i].name = "dv_" + (new Integer(i)) + "_";
//                if (d.name != null) members[i].name += d.name;
                members[i].type = orb.get_primitive_tc(TCKind.tk_any);
                members[i].type_def = null;

                typeName += ":" + members[i].name;
            }
            // Read descriptive type code into any
            String name = (v.name == null) ? "unnamed" : v.name;
            TypeCode tc = orb.create_struct_tc(typeName, name, members);

            org.omg.CORBA.portable.InputStream in = outputStream
                .create_input_stream();
            outputStream.flush();
            a.type(tc);
            a.read_value(in, tc);
            outputStream.close();
        }
        catch (Exception ex)
        {
            throw (IllegalArgumentException)
                err_.log( new IllegalArgumentException("DaValue").initCause(ex), 
                          "Constructing Any from DaValue of structures");
        }
        return a;
    }

    /**
     * Static method to create an Any that represents a DaValue String Array
     * 
     * @param v
     *        DaValue to decode
     * @param a
     *        CORBA Any to write decoded DaValue into.
     * @return Returns a new CORBA Any that is constituted from the Strings
     *         of this DaValue's String array
     */
    static int toAnyStringcount = 0;

    private static Any toAnyStringArray(DaValue v, Any a)
    {
        if (DEBUG)
            System.out.println("In toAnyStringArray");
        // ORB orb = ORB.init();
        int dLen = v.size(); // Elements in this vector
        String[] strings = new String[dLen];
        int i = 0;
        try
        {
            for (Enumeration e = v.elements(); e.hasMoreElements(); i++)
            {
                String elem = (String) e.nextElement();
                strings[i] = elem;
            }
            StringSeqHelper.insert(a, strings);
        }
        catch (Exception ex)
        {
            throw (IllegalArgumentException)
                err_.log( new IllegalArgumentException("DaValue").initCause(ex),        
                          "Constructing Any from DaValue of Strings");
        }
        return a;
    }

    /**
     * Static method to create an Any that represents a DaValue Basic or
     * Array type
     * 
     * @param v
     *        DaValue to decode
     * @param a
     *        CORBA Any to write decoded DaValue into.
     * @return Returns a new CORBA Any
     */
    private static Any toAnyBasicOrArray(DaValue v, Any a)
    {
        switch (v.type)
        {
        case Type.BOOLEAN:
            a.insert_boolean(((Boolean) v.firstElement()).booleanValue());
            break;
        case Type.BYTE:
            a.insert_octet(((Byte) v.firstElement()).byteValue());
            break;
        case Type.CHAR:
            a.insert_char(((Character) v.firstElement()).charValue());
            break;
        case Type.DOUBLE:
            a.insert_double(((Double) v.firstElement()).doubleValue());
            break;
        case Type.FLOAT:
            a.insert_float(((Float) v.firstElement()).floatValue());
            break;
        case Type.LONG:
            a.insert_long(((Long) v.firstElement()).intValue());
            break;
        case Type.LONGDOUBLE:
            a.insert_double(((Double) v.firstElement()).doubleValue());
            break;
        case Type.LONGLONG:
            a.insert_longlong(((Long) v.firstElement()).longValue());
            break;
        case Type.SHORT:
            a.insert_short(((Short) v.firstElement()).shortValue());
            break;
        case Type.STRING:
            a.insert_string((String) v.firstElement());
            break;
        case Type.ULONG:
            a.insert_ulong(((Long) v.firstElement()).intValue());
            break;
        case Type.ULONGLONG:
            a.insert_ulonglong(((Long) v.firstElement()).longValue());
            break;
        case Type.USHORT:
            a.insert_short(((Short) v.firstElement()).shortValue());
            break;
        case Type.WCHAR:
            a.insert_wchar(((Character) v.firstElement()).charValue());
            break;
        case Type.WSTRING:
            a.insert_wstring((String) v.firstElement());
            break;
            // Array type
        default:
            return toAnyArray(v, a);
        }
        return a;
    }

    /**
     * Static method to create an Any that represents a DaValue Array type
     * 
     * @param v
     *        DaValue to decode
     * @param a
     *        CORBA Any to write decoded DaValue into.
     * @return Returns a new CORBA Any
     */
    private static Any toAnyArray(DaValue v, Any a)
    {
        if (DEBUG)
            System.out.println("In toAnyArray");
        int i = 0;

        try
        {
            org.omg.CORBA.portable.OutputStream outputStream = null;
            outputStream = a.create_output_stream();
            switch (v.type)
            {
            case Type.BOOLEANA:
            {
                boolean[] ary = new boolean[v.size()];
                for (Enumeration e = v.elements(); e.hasMoreElements();)
                    ary[i++] = ((Boolean) e.nextElement())
                        .booleanValue();
                outputStream.write_boolean_array(ary, 0, v.size());
            }
            break;
            case Type.BYTEA:
            {
                byte[] ary = new byte[v.size()];
                for (Enumeration e = v.elements(); e.hasMoreElements();)
                    ary[i++] = ((Byte) e.nextElement()).byteValue();
                outputStream.write_octet_array(ary, 0, v.size());
            }
            break;
            case Type.CHARA:
            {
                char[] ary = new char[v.size()];
                for (Enumeration e = v.elements(); e.hasMoreElements();)
                    ary[i++] = ((Character) e.nextElement())
                        .charValue();
                outputStream.write_char_array(ary, 0, v.size());
            }
            break;
            case Type.DOUBLEA:
            case Type.LONGDOUBLEA:
            {
                double[] ary = new double[v.size()];
                for (Enumeration e = v.elements(); e.hasMoreElements();)
                    ary[i++] = ((Double) e.nextElement())
                        .doubleValue();
                outputStream.write_double_array(ary, 0, v.size());
            }
            break;
            case Type.FLOATA:
            {
                float[] ary = new float[v.size()];
                for (Enumeration e = v.elements(); e.hasMoreElements();)
                    ary[i++] = ((Float) e.nextElement())
                        .floatValue();
                outputStream.write_float_array(ary, 0, v.size());
            }
            break;
            case Type.LONGA:
            {
                int[] ary = new int[v.size()];
                for (Enumeration e = v.elements(); e.hasMoreElements();)
                    ary[i++] = ((Long) e.nextElement()).intValue();
                outputStream.write_long_array(ary, 0, v.size());
            }
            break;
            case Type.LONGLONGA:
            {
                long[] ary = new long[v.size()];
                for (Enumeration e = v.elements(); e.hasMoreElements();)
                    ary[i++] = ((Long) e.nextElement()).longValue();
                outputStream.write_longlong_array(ary, 0, v.size());
            }
            break;
            case Type.SHORTA:
            {
                short[] ary = new short[v.size()];
                for (Enumeration e = v.elements(); e.hasMoreElements();)
                    ary[i++] = ((Short) e.nextElement())
                        .shortValue();
                outputStream.write_short_array(ary, 0, v.size());
            }
            break;
            case Type.ULONGA:
            {
                int[] ary = new int[v.size()];
                for (Enumeration e = v.elements(); e.hasMoreElements();)
                    ary[i++] = ((Short) e.nextElement()).intValue();
                outputStream.write_ulong_array(ary, 0, v.size());
            }
            break;
            case Type.ULONGLONGA:
            {
                long[] ary = new long[v.size()];
                for (Enumeration e = v.elements(); e.hasMoreElements();)
                    ary[i++] = ((Long) e.nextElement()).longValue();
                outputStream.write_ulonglong_array(ary, 0, v.size());
            }
            break;
            case Type.USHORTA:
            {
                short[] ary = new short[v.size()];
                for (Enumeration e = v.elements(); e.hasMoreElements();)
                    ary[i++] = ((Short) e.nextElement())
                        .shortValue();
                outputStream.write_ushort_array(ary, 0, v.size());
            }
            break;
            case Type.WCHARA:
            {
                char[] ary = new char[v.size()];
                for (Enumeration e = v.elements(); e.hasMoreElements();)
                    ary[i++] = ((Character) e.nextElement())
                        .charValue();
                outputStream.write_wchar_array(ary, 0, v.size());
            }
            break;
            default:
                throw new UnsupportedOperationException(
                    "Missing DaValue.Type conversion");
                                
            }
            a.read_value(outputStream.create_input_stream(), orb.create_array_tc(v
                                                                                 .size(), orb.get_primitive_tc(toCORBATCKind(v.type))));
            outputStream.flush();
            outputStream.close();
        }
        catch (Exception ex)
        {
            throw (IllegalArgumentException)
                err_.log( new IllegalArgumentException("DaValue").initCause(ex), 
                          "Constructing Any from DaValue of array");
        }
        return a;
    }

    /**
     * Return the corresponding TCKind value for the given DaValue type
     * 
     * @param theType
     *        the given DaValue type
     * @return The corresponding CORBA TCKind
     */
    private static TCKind toCORBATCKind(int theType)
    {
        switch (theType)
        {
        case Type.STRUCT:
            return TCKind.tk_struct;
        case Type.ANY:
            return TCKind.tk_any;
        case Type.BOOLEAN:
        case Type.BOOLEANA:
            return TCKind.tk_boolean;
        case Type.BYTE:
        case Type.BYTEA:
            return TCKind.tk_octet;
        case Type.CHAR:
        case Type.CHARA:
            return TCKind.tk_char;
        case Type.DOUBLE:
        case Type.DOUBLEA:
            return TCKind.tk_double;
        case Type.FLOAT:
        case Type.FLOATA:
            return TCKind.tk_float;
        case Type.LONG:
        case Type.LONGA:
            return TCKind.tk_long;
        case Type.LONGDOUBLE:
        case Type.LONGDOUBLEA:
            return TCKind.tk_longdouble;
        case Type.LONGLONG:
        case Type.LONGLONGA:
            return TCKind.tk_longlong;
        case Type.SHORT:
        case Type.SHORTA:
            return TCKind.tk_short;
        case Type.STRING:
        case Type.STRINGA:
            return TCKind.tk_string;
        case Type.ULONG:
        case Type.ULONGA:
            return TCKind.tk_ulong;
        case Type.ULONGLONG:
        case Type.ULONGLONGA:
            return TCKind.tk_ulonglong;
        case Type.USHORT:
        case Type.USHORTA:
            return TCKind.tk_ushort;
        case Type.WCHAR:
        case Type.WCHARA:
            return TCKind.tk_wchar;
        case Type.WSTRING:
        case Type.WSTRINGA:
            return TCKind.tk_wstring;
        }
        throw (IllegalArgumentException)
            err_.log( new IllegalArgumentException("DaValue.Type"), 
                      "Converting DaValue.Type to TCKind");
    }



    static public boolean[] numbersToBooleans (Vector numbers)
    {
        boolean[] a = new boolean[numbers.size()];
        for (int i = 0; i < a.length; ++i) {
            Number n = (Number)numbers.elementAt (i);
            a[i] = (n.doubleValue() != 0);
        }
        return a;
    }
    static public byte[] numbersToBytes (Vector numbers)
    {
        byte[] a = new byte[numbers.size()];
        for (int i = 0; i < a.length; ++i) {
            Number n = (Number)numbers.elementAt (i);
            a[i] = n.byteValue();
            if (a[i] != n.doubleValue())
                throw new UnsupportedOperationException (
                    "Numeric conversion loses information");
        }
        return a;
    }
    static public short[] numbersToShorts (Vector numbers)
    {
        short[] a = new short[numbers.size()];
        for (int i = 0; i < a.length; ++i) {
            Number n = (Number)numbers.elementAt (i);
            a[i] = n.shortValue();
            if (a[i] != n.doubleValue())
                throw new UnsupportedOperationException (
                    "Numeric conversion loses information");
        }
        return a;
    }
    static public int[] numbersToInts (Vector numbers)
    {
        int[] a = new int[numbers.size()];
        for (int i = 0; i < a.length; ++i) {
            Number n = (Number)numbers.elementAt (i);
            a[i] = n.intValue();
            if (a[i] != n.doubleValue())
                throw new UnsupportedOperationException (
                    "Numeric conversion loses information");
        }
        return a;
    }
    static public long[] numbersToLongs (Vector numbers)
    {
        long[] a = new long[numbers.size()];
        for (int i = 0; i < a.length; ++i) {
            Number n = (Number)numbers.elementAt (i);
            a[i] = n.longValue();
            if (a[i] != n.doubleValue())
                throw new UnsupportedOperationException (
                    "Numeric conversion loses information");
        }
        return a;
    }
    static public float[] numbersToFloats (Vector numbers)
    {
        float[] a = new float[numbers.size()];
        for (int i = 0; i < a.length; ++i) {
            Number n = (Number)numbers.elementAt (i);
            a[i] = n.floatValue();
            // sloppy check for (unexpected) loss of information
            if (Math.abs (n.doubleValue() - a[i]) > 1e-6) {
                System.out.println (
                    "n.doubleValue() - n.floatValue() = " +
                    new Double (n.doubleValue() - n.floatValue()));
                throw new UnsupportedOperationException (
                    "Numeric conversion loses information");
            }
        }
        return a;
    }
    static public double[] numbersToDoubles (Vector numbers)
    {
        double[] a = new double[numbers.size()];
        for (int i = 0; i < a.length; ++i) {
            Number n = (Number)numbers.elementAt (i);
            a[i] = n.doubleValue();
        }
        return a;
    }
    static public Vector stringsToFloatNumbers (Vector strings)
    {
        Vector numbers = new Vector (strings.size());
        for (int i = 0; i < strings.size(); ++i) {
            // for the time being, don't worry about longs
            numbers.addElement (new Float ((String)strings.elementAt (i)));
        }
        return numbers;
    }
    static public Vector stringsToDoubleNumbers (Vector strings)
    {
        Vector numbers = new Vector (strings.size());
        for (int i = 0; i < strings.size(); ++i) {
            // for the time being, don't worry about longs
            numbers.addElement (new Double ((String)strings.elementAt (i)));
        }
        return numbers;
    }
    static public boolean[] stringsToBooleans (Vector strings)
    {
        boolean[] a = new boolean[strings.size()];
        for (int i = 0; i < a.length; ++i) {
            a[i] =
                (new Boolean ((String)strings.elementAt (i))).booleanValue();
        }
        return a;
    }
    static public Vector booleansToNumbers (Vector booleans)
    {
        Vector numbers = new Vector (booleans.size());
        for (int i = 0; i < booleans.size(); ++i) {
            // for the time being, don't worry about longs
            numbers.addElement (
                new Byte(((Boolean)booleans.elementAt (i)).booleanValue()?
                         (byte)1 : (byte)0));
        }
        return numbers;
    }


    public boolean isNumeric ()
    {
        return (isShort() || isInt() || isLong() || isFloat() || isDouble());
    }


    public boolean isBoolean ()
    {
        return (type == Type.BOOLEAN) || (type == Type.BOOLEANA);
    }

    public boolean getBoolean (int i)
    {
        return ((Boolean)elementAt(i)).booleanValue();
    }

    public boolean[] getBooleans ()
    {
        boolean[] a = new boolean[size()];
        for (int i = 0; i < a.length; ++i)
            a[i] = getBoolean (i);
        return a;         
    }

    public boolean[] getAsBooleans ()
    {
        if (isNumeric())
            return numbersToBooleans (this);
        if (isString())
            return stringsToBooleans (this);

        return getBooleans();
    }


    public boolean isByte ()
    {
        return (type == Type.BYTE) || (type == Type.BYTEA);        
    }
    
    public byte getByte (int i)
    {
        return ((Byte)elementAt(i)).byteValue();
    }

    public byte[] getBytes ()
    {
        byte[] a = new byte[size()];
        for (int i = 0; i < a.length; ++i)
            a[i] = getByte (i);
        return a;
    }

    public byte[] getAsBytes ()
    {
        Vector n = this;
        
        if (isString())
            n = stringsToDoubleNumbers (this);
        else if (isBoolean())
            n = booleansToNumbers (this);
        
        return numbersToBytes (n);
    }

    
    public boolean isShort ()
    {
        return (type == Type.SHORT) || (type == Type.SHORTA);
    }

    public short getShort (int i)
    {
        return ((Short)elementAt(i)).shortValue();
    }

    public short[] getShorts ()
    {
        short[] a = new short[size()];
        for (int i = 0; i < a.length; ++i)
            a[i] = getShort (i);
        return a;
    }

    public short[] getAsShorts ()
    {
        Vector n = this;
        
        if (isString())
            n = stringsToDoubleNumbers (this);
        else if (isBoolean())
            n = booleansToNumbers (this);
        
        return numbersToShorts (n);
    }

    
    public boolean isInt ()
    {
        return (isShort() || isLong());
    }

    public int getInt (int i)
    {
        if (isShort())
            return (int)getShort (i);
        else
            return (int)getLong (i);
    }

    public int getInt ()
    {
        return getInt (0);
    }

    public int[] getInts ()
    {
        int[] a = new int[size()];
        for (int i = 0; i < a.length; ++i)
            a[i] = getInt (i);
        return a;
    }

    public int[] getAsInts ()
    {
        Vector n = this;
        
        if (isString())
            n = stringsToDoubleNumbers (this);
        else if (isBoolean())
            n = booleansToNumbers (this);
        
        return numbersToInts (n);
    }

    
    public boolean isLong ()
    {
        return (type == Type.LONG) || (type == Type.LONGA);
    }

    public long getLong (int i)
    {
        return ((Long)elementAt(i)).longValue();
    }

    public long getLong ()
    {
        return getLong (0);
    }

    public long[] getLongs ()
    {
        long[] a = new long[size()];
        for (int i = 0; i < a.length; ++i)
            a[i] = getLong (i);
        return a;
    }

    public long[] getAsLongs ()
    {
        Vector n = this;
        
        if (isString())
            n = stringsToDoubleNumbers (this);
        else if (isBoolean())
            n = booleansToNumbers (this);
        
        return numbersToLongs (n);
    }

    
    public boolean isFloat ()
    {
        return (type == Type.FLOAT) || (type == Type.FLOATA);
    }

    public float getFloat (int i)
    {
        return ((Float)elementAt(i)).floatValue();
    }

    public float getFloat ()
    {
        return getFloat (0);
    }

    public float[] getFloats ()
    {
        float[] a = new float[size()];
        for (int i = 0; i < a.length; ++i)
            a[i] = getFloat (i);
        return a;
    }

    public float[] getAsFloats ()
    {
        Vector n = this;
        
        if (isString())
            n = stringsToFloatNumbers (this);
        else if (isBoolean())
            n = booleansToNumbers (this);
        
        return numbersToFloats (n);
    }

    
    public boolean isDouble ()
    {
        return (type == Type.DOUBLE) || (type == Type.DOUBLEA);
    }

    public double getDouble (int i)
    {
        return ((Double)elementAt(i)).doubleValue();
    }

    public double getDouble ()
    {
        return getDouble (0);
    }

    public double[] getDoubles ()
    {
        double[] a = new double[size()];
        for (int i = 0; i < a.length; ++i)
            a[i] = getDouble (i);
        return a;
    }

    public double[] getAsDoubles ()
    {
        Vector n = this;
        
        if (isString())
            n = stringsToDoubleNumbers (this);
        else if (isBoolean())
            n = booleansToNumbers (this);
        
        return numbersToDoubles (n);
    }

    
    public boolean isString ()
    {
        return (type == Type.STRING) || (type == Type.STRINGA);
    }

    public String getString (int i)
    {
        return elementAt(i).toString();
    }

    public String getString ()
    {
        return getString (0);
    }

    public String[] getStrings ()
    {
        String[] a = new String[size()];
        for (int i = 0; i < a.length; ++i)
            a[i] = getString (i);
        return a;
    }    

    public String[] getAsStrings ()
    {
        String[] a = new String[size()];
        for (int i = 0; i < a.length; ++i)
            a[i] = (elementAt (i)).toString();
        return a;
    }


    public NamedValues getNamedValues ()
    {
        NamedValues namedValues = new NamedValues ();
        generateNamedValues (namedValues, "");
        return namedValues;
    }


    protected void generateNamedValues (
        NamedValues namedValues,
        String prefix)
    {
        // base case: an array or scalar of basic type
        if (type != Type.STRUCT)
        {
            namedValues.add (prefix + name, this);
        }
        else
        {
            if (name != null)
                prefix = prefix + name + ".";
            
            Enumeration e = elements();
            while (e.hasMoreElements())
            {
                DaValue dv = (DaValue)e.nextElement();
                dv.generateNamedValues (namedValues, prefix);
            }
        }
    }
}

