/*
 * _DaReference.java
 *
 * Created on 08 July 2002, 05:30
 *
 */

package edu.stanford.slac.aida.lib.util.common;

import java.util.*;
import org.omg.CORBA.*;
import edu.stanford.slac.aida.lib.dp.*;
import edu.stanford.slac.err.*; // Message logging API.
import edu.stanford.slac.except.*; // General Exceptions

/**
 * <code>_DaReference</code> represents a reference to an Aida variable as a
 * parsed class that can be easily interchanged with the value's string
 * representation. By the variable's string representation is meant just the
 * query string that gets the variable's value, the AIDA Query Specification
 * 
 * Constructors parse the Query String using a AQSParser and create this
 * _DaReference. toString() creates the AQS string representation of the
 * reference specification.
 * 
 * Each element of a <code>_DaReference</code> must be one instance of:
 * <ul>
 * <li>a simple type (containing the instance attribute strings)
 * <li>an array type (containing the instance attribute strings and some array
 * delimiters)
 * <li>a structure (containing vector elements that are other _DaReferences)
 * </ul>
 * 
 * <h4>Lifecycle Construction</h4>
 * Creation with a string creates an underlying object and also parses the given
 * string into this object. If the object is created empty then setTarget() must
 * be called to set and parse the query string.
 * 
 * <h4>Parsing</h4>
 * In the parsed state of the object, it can be compiled, but cannot yet be
 * executed. The syntax of the query has been checked against the AQS language
 * specification and the corresponding shape of object created.
 * 
 * <h4>Compiled</h4>
 * After it has been compiled, all references to AIDA objects have been resolved
 * into respective callable interfaces.
 * 
 * <h4>Execution</h4>
 * Various getxxx() methods offer execution of the compiled reference.
 * 
 * <h4>Notes on get() methods</h4>
 * All the get functions defined are similar. They all have four main entry
 * points
 * <ul>
 * <li><code>get()</code>- No parameters, retrieves parameters stored in
 * this Reference and calls <code>get(ref, args)</code>.
 * <li><code>get(da)</code>- With a _DaObject, simply calls the get(da,
 * args) with args set to null
 * <li><code>get(da, args)</code>- With _DaObject and args array, if not
 * compiled the reference is compiled then calls <code>get(ref, args)</code>
 * with args set to the combined args in the DaObject and the given args.
 * <li><code>get(args)</code>- Just args array, simply call get(ref, args)
 * with ref set to this reference object
 * </ul>
 * Finally, the worker function <code>get(ref, args)</code>- this is the call
 * that does the actual get. The given reference must be pre-compiled. This
 * template is repeated for each get type.
 * 
 * 
 * @author George McIntyre
 * @version 04-Dec-2006, Bob Hall (rdh): Removed set method logging that has now
 *          been moved to DaObject, where the identification of the requestor
 *          is possible for DaServer requests.
 * @version 17-Nov-2006, Bob Hall (rdh): Added new set method with no parameters.
 * @version 02-Oct-2006, Bob Hall (rdh): Added String array args parameter
 *          to set methods.  Also added set method logging.
 * @version 21-Sep-2006, Bob Hall (rdh): Added set methods.
 * @version 27-Sep-2004, Greg White (greg): Change get methods to catching all
 *          exceptions but only throwing UnableToGetDataException, so can catch
 *          CORBA exceptions and uncache AidaRef in case of CORBA failure.
 * @version 15-Sep-2004, Greg White (greg): Add error logging.
 */
public class _DaReference extends Vector
{
        /** List of (parm, value) pairs: global for all levels */
        public Hashtable m_args = new Hashtable();

        /**
         * Permisible types of Reference vector
         */
        public static class Type
        {

                /**
                 * This vector contains no elements, instead the instance,
                 * attribute fields are set. This is a simple type of query
                 * containing a single instance attribute pair with optional
                 * modifiers. Even though this is a simple type of query it may
                 * return an array or struct.
                 */
                public static final int SIMPLE = 0;

                /**
                 * This vector contains no elements, instead the instance,
                 * attribute, bounds, etc fields are set. This is a simple type
                 * of query containing a single instance attribute pair with
                 * additional array constraints. This is an array type of query
                 * so it should return an array and it is a runtime error if it
                 * returns a structure.
                 */
                public static final int ARRAY = 1;

                /**
                 * This vector contains elements that are themselves
                 * _DaReferences. The sub references determine which queries
                 * will be run and the ordering of results and filling of the
                 * user defined structure.
                 */
                public static final int STRUCT = 2;
        }

        /**
         * Fields that define this level in the reference Instance string
         */
        public String instance = null; // Instance

        /**
         * Attribute string
         */
        public String attribute = null; // Attribute

        /**
         * Type of this vector: Simple, Array or Structure
         */
        public int type = Type.SIMPLE; // Type of this node

        /**
         * Name of this vector
         */
        public String name = null; // Name of this node

        /**
         * Array bounds of this vector. This is a list of ArrayBound entries.
         * They are used in user queries for arrays and structures, to limit
         * returned elements to the specified boundaries. Elements in bounds are
         * ArrayBound.
         * 
         * @see ArrayBound
         */
        public Vector bounds = new Vector();

        /**
         * When the reference is compiled this holds the AidaObjectRef returned
         * by AIDA NameServer for this vector of the _DaReference. It also holds
         * the converted target string which should be used instead of the
         * original target string when the query represented by this
         * _DaReference is executed.
         */
        public AidaObjectRef m_ref = null;

        /**
         * When the reference is compiled, this holds the AidaObjectI returned
         * by the AIDA NameServer for this vector of the _DaReference. It is the
         * actual callable interface to the AIDA object.
         */
        public AidaObjectI m_refI = null;

        /**
         * Is this _DaReference compiled. True if yes.
         */
        boolean fCompiled = false;

        /**
         * Internal flag used when executing toString and toNewString to let
         * recursive calls know which to use.
         */
        private boolean fUseNewString = false;

        /**
         * Server lookup cache. This is the list of instances that have already
         * been looked up. As well as the time the lookup was made. It employs
         * an optimistic lookup invalidation policy which tries to access the
         * server first, and then if it fails, replaces the cached entry with a
         * newly looked up one.
         * 
         * The cache is contructed with an instance as the key. The value is
         * another Hashtable containing the attribute as the key. The value is
         * the AidaObjectRef.
         * 
         * Note that m_cache here, is just a reference to a Hashtable cache
         * member of the API Object that is using this <code>_DaReference</code>,
         * either <code>DaReference</code> or <code>DaServerI_impl</code>
         * (for DaObject). _DaReference manages the cache owned by those other
         * objects.
         */
        private Hashtable m_cache = null;

        /**
         * Error Issuance API
         */
        protected static Err m_err;

        /**
         * Creates a new empty instance instance of a _DaReference object
         */
        public _DaReference()
        {
                super();
                init();
        }

        /**
         * Creates a new instance of the reference initialised with the given
         * target string The target string is parsed for correctness as per the
         * AIDA query specification
         * 
         * @param target
         *        String target specification in AQS
         */
        public _DaReference(String target) // TODO: Change UndefinedName to
                // SyntaxException to disambiguate
                throws UndefinedNameException
        {
                super();
                init();
                AQSParser parser = new AQSParser(this, target, m_args);
        }

        /**
         * Creates a new instance of the reference, initialised with the given
         * target string and argument list. The target string is parsed for
         * correctness per the AIDA query specification
         * 
         * @param target
         *        String target specification in AQS
         * @param args
         *        String array containing Parameter, Value pairs
         */
        public _DaReference(String target, String[] args) throws UndefinedNameException
        {
                super();
                init();
                addArgs(args);
                AQSParser parser = new AQSParser(this, target, m_args);
        }

        /**
         * Creates a new instance of the reference, initialised with the given
         * target string and <code>DaObject</code>. The target string is
         * parsed for correctness per the AIDA query specification and the
         * parsed structure is compiled.
         * 
         * @param target
         *        String target specification in AQS
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @throws UndefinedNameException
         *         _DaObject failed to compile, probably bad Directory Service
         *         data
         */
        public _DaReference(String target, _DaObject da) throws UndefinedNameException
        // TODO: Change UndefinedName to SyntaxException to disambiguate
        {
                super();
                init();
                AQSParser parser = new AQSParser(this, target, m_args);
                compile(da);
        }

        /**
         * Creates a new instance of the reference, initialised with the given
         * target string, <code>DaObject</code> and argument list. The target
         * string is parsed for correctness as the AIDA query specification. The
         * parsed structure is then compiled.
         * 
         * @param target
         *        String target specification in AQS
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @param args
         *        String array containing Parameter-Value, pairs
         */
        public _DaReference(String target, _DaObject da, String[] args)
                throws UndefinedNameException
        // TODO: Change UndefinedName to SyntaxException to disambiguate
        {
                super();
                init();
                addArgs(args);
                AQSParser parser = new AQSParser(this, target, m_args);
                compile(da);
        }

        /**
         * Creates a new instance of the reference, initialised with the given
         * target string, <code>DaObject</code> and argument list, together
         * with a reference to a <code>Hashtable</code> cache of
         * <code>DaReferences</code> a user program may wish to keep for
         * itself. The target string is parsed for correctness per the AIDA
         * query specification. The parsed structure is then compiled.
         * 
         * @param target
         *        String target specification in AQS
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @param args
         *        String array containing Parameter, Value pairs
         * @param cache
         *        A reference to an external lookup cache to be used in
         *        compilation
         * @throws UndefinedNameException
         *         _DaObject failed to compile, probably bad Directory Service
         *         data
         */
        public _DaReference(String target, _DaObject da, String[] args, Hashtable cache)
                throws UndefinedNameException
        // TODO: Change UndefinedName to SyntaxException to disambiguate problem that
        // a name is not in teh directory service from a syntactically invalid name.
        {
                super();
                init();
                m_cache = cache;
                addArgs(args);
                AQSParser parser = new AQSParser(this, target, m_args);
                compile(da);
        }

        /**
         * Adds arguments to the internal arguments table
         * 
         * @param args
         *        String array containing Parameter, Value pairs
         */
        private void addArgs(String[] args)
        {
                for (int i = 0; (i + 1) < args.length; i += 2)
                        if (args[i].compareTo("null") != 0)
                                m_args.put(args[i], args[i + 1]);
        }

        /**
         * Initialise this _DaReference with the given target string. The target
         * string is parsed for correctness as per the AIDA query specification
         * 
         * @param target
         *        String target specification in AQS
         */
        public void setTarget(String target)
                throws UndefinedNameException
        {
                AQSParser parser = new AQSParser(this, target, m_args);
        }

        /**
         * Initialization common to all constructors.
         */
        protected void init()
        {
                m_err = Err.getInstance(); // Get logging singleton.
        }

        /**
         * Is this a simple (non user defined structure) reference?
         * 
         * @return Returns true if the type is not STRUCT
         */
        public boolean isSimpleReference()
        {
                return (type == Type.SIMPLE || type == Type.ARRAY);
        }

        /**
         * Return a string representation of the AQS represented by this
         * _DaReference object
         * 
         * @return A string representation of the AQS represented by this
         *         _DaReference
         */
        public String toString()
        {
                String thisString = "";

                fUseNewString = false;
                switch (type)
                {
                case Type.ARRAY:
                        thisString += instanceAttribute();
                        break;

                case Type.STRUCT:
                        thisString += structSpecifier();
                        break;

                case Type.SIMPLE:
                default:
                        thisString += instanceAttribute();
                }

                thisString += arraySpecifier();

                return (thisString);
        }

        /**
         * Return a new string representation of the AQS represented by this
         * <code> _DaReference</code> object. For compiled queries return the
         * converted target string and the pre-pended service chain (removing
         * the first service)
         * 
         * @return A string representation of the converted target string
         *         associated with this _DaReference. It is not necessary for
         *         the result to correspond to AQS as long as the receiving
         *         service is expecting this.
         */
        public String toNewString()
        {
                String thisString = "";
                String serviceList = "";
                fUseNewString = true;

                if (m_ref != null)
                {
                        int len = m_ref.sor.length;

                        for (int i = 1; i < len; i++)
                                serviceList += "{AIDA:" + m_ref.sor[i] + "//}";
                }

                switch (type)
                {
                case Type.ARRAY:
                        thisString += serviceList
                                + ((m_ref == null) ? instanceAttribute() : m_ref.data);
                        break;

                case Type.STRUCT:
                        thisString += serviceList + structSpecifier();
                        break;

                case Type.SIMPLE:
                default:
                        thisString += serviceList
                                + ((m_ref == null) ? instanceAttribute() : m_ref.data);

                }

                thisString += arraySpecifier();

                return (thisString);
        }

        /**
         * Lookup this query in the query cache, if a query cache has been
         * previosuly assigned.
         * 
         * @param query
         *        the query to lookup
         * @return reference object or null if query not in cache
         */

        private AidaObjectRef lookup(String query)
        {
                if (m_cache == null)
                        return null;
                else
                        return ((AidaObjectRef) (m_cache.get(query)));
        }

        /**
         * put this query in the query cache if given
         * 
         * @param query
         *        the query to cache
         * @param ref
         *        the reference to store with the query
         * @version 30-Sep-2004, Greg White (Greg): Remove all caching until
         * can work out how to reaquire an IOR and retry a get method all inside the
         * same get.
         * @version 05-Sep-2004, Greg White: don't put() if query is null.
         */
        private void cache(String query, AidaObjectRef ref)
        {
                // Remove caching, to see if can fix bug that a dp server restart
                // requires a DaServer restart
                // if (m_cache != null && query != null && ref != null)
                //        m_cache.put(query, ref);
        }

        /**
         * remove/invalidate this query from the query cache
         * 
         * @param query
         *        the query to cache
         * @param ref
         *        the reference to remove
         */
        public void remove(String query)
        {
                if (m_cache != null)
                        m_cache.remove(query);
        }

        /**
         * Compile this _DaReference to resolve all references
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @throws UndefinedNameException
         *         _DaObject failed to compile, probably bad Directory Service
         *         data
         * @throws IllegalStateException
         *         The Data Provider server for the data object encapsulated by
         *         this object, is not up or not registered or for some other
         *         reason has null IOR; or that the data item is otherwise
         *         invalid; or if Aida server IOR can't be narrowed.
         */
        public void compile(_DaObject da)
                throws UndefinedNameException
        {
                if (!fCompiled)
                {
                        switch (type)
                        {
                        case Type.SIMPLE:
                        case Type.ARRAY:
                        {
                                String query = instance + AQSToken.SEPARATOR + attribute;

                                // Get the reference to the actual
                                // server for this data item
                                if ((m_ref = lookup(query)) == null)
                                        cache(query, m_ref = da.getTargetRef(query));

                                // If no server IOR, means that the
                                // item is invalid or the server is
                                // down.
                                if (m_ref.sor.length == 0)
                                {
                                        // TODO: probably better to throw a
                                        // checked exception
                                        // here, and modify callers.
                                        throw (IllegalStateException) m_err
                                                .log(
                                                        new IllegalStateException(),
                                                        query
                                                                + ":It's dead Jim...(the reference points to a data item who's CORBA IOR is unknown");
                                } else
                                        // Otherwise get the callable
                                        // interface to the server
                                        m_refI = da.getAidaServerI(m_ref.sor[0]);

                                if (m_ref.count > 1)
                                        type = Type.ARRAY;
                                break;
                        }

                        case Type.STRUCT:
                                // For structures compile each of the
                                // sub elements
                                for (Enumeration e = elements(); e.hasMoreElements();)
                                {
                                        _DaReference r = (_DaReference) (e.nextElement());
                                        r.m_args = m_args;
                                        r.compile(da);
                                }
                                break;
                        }
                }

                fCompiled = true;
        }

        /**
         * Retrieve this reference returning an object directly. See comments in
         * header for the programming pattern for all get functions.
         * 
         * @return Returns a language dependent class object (eg Long)
         *         containing the instantaneous result of executing the query.
         */
        public java.lang.Object get()
                throws UndefinedNameException, UnableToGetDataException
        {
                return get(this, getArgs());
        }

        /**
         * Retrieve this reference returning an object directly Compile if
         * necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return Returns a language dependent class object (eg Boolean)
         *         containing the instantaneous result of executing the query.
         */
        public java.lang.Object get(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                return get(da, (String[]) null);
        }

        /**
         * Retrieve this reference returning an object directly Compile if
         * necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return Returns a language dependent class object (eg Boolean)
         *         containing the instantaneous result of executing the query.
         */
        public java.lang.Object get(_DaObject da, String[] args)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return get(this, da.concatArgs(args));
        }

        /**
         * Retrieve this reference with the given arguments returning an object
         * directly
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return Returns a language dependent class object (eg Boolean)
         *         containing the instantaneous result of executing the query.
         */
        public java.lang.Object get(String[] args)
                throws UndefinedNameException, UnableToGetDataException
        {
                return get(this, args);
        }

        /**
         * Static method to run the given reference with given arguments
         * returning an object directly
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return Returns a language dependent class object (eg Boolean)
         *         containing the instantaneous result of executing the query.
         * @throws UndefinedNameException
         *         The reference contains a name that is either unknown to, or
         *         poorly specified in, the Directory Service.
         * @throws UnableToGetDataException
         *         An error occurred on the data provider which meant that data
         *         could not be returned
         * @throws IllegalArgumentException
         *         The reference is unknown <code>DaValue.Type</code>, or of
         *         <code>DaValue.Type</code> STRUCT or ARRAY, which is not
         *         allowed for the highest level of the returned object.
         */
        public static java.lang.Object get(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                switch (r.type)
                {
                case Type.STRUCT:
                        throw new IllegalArgumentException(
                                "Can't return a structure as an object directly");

                case Type.ARRAY:
                        throw new IllegalArgumentException(
                                "Can't return an array as a simple object directly");

                case Type.SIMPLE:
                        try
                        {
                                switch (r.m_ref.type)
                                {
                                case DaValue.Type.BOOLEAN:
                                        return (new Boolean(get_boolean(r, args)));
                                case DaValue.Type.BYTE:
                                        return (new Byte(get_byte(r, args)));
                                case DaValue.Type.CHAR:
                                        return (new Character(get_char(r, args)));
                                case DaValue.Type.DOUBLE:
                                        return (new Double(get_double(r, args)));
                                case DaValue.Type.FLOAT:
                                        return (new Float(get_float(r, args)));
                                case DaValue.Type.LONG:
                                        return (new Long(get_long(r, args)));
                                case DaValue.Type.LONGDOUBLE:
                                        return (new Double(get_double(r, args)));
                                case DaValue.Type.LONGLONG:
                                        return (new Long(get_long(r, args)));
                                case DaValue.Type.SHORT:
                                        return (new Short(get_short(r, args)));
                                case DaValue.Type.STRING:
                                        return (get_string(r, args));
                                case DaValue.Type.ULONG:
                                        return (new Long(get_long(r, args)));
                                case DaValue.Type.ULONGLONG:
                                        return (new Long(get_long(r, args)));
                                case DaValue.Type.USHORT:
                                        return (new Short(get_short(r, args)));
                                case DaValue.Type.WCHAR:
                                        return (new Character(get_char(r, args)));
                                case DaValue.Type.WSTRING:
                                        return (get_string(r, args));
                                default:
                                        throw new IllegalArgumentException(
                                                "Unrecognized type in get()");
                                }
                        }
                        catch (UnableToGetDataException e)
                        {
                                // NOTE: This method is made to catch
                                // and rethrow UndefinedNameException
                                // to up-translate the exceptions from
                                // below, eg CORBA error occurs
                                // the error log only gets a CORBA
                                // error. Disconcerting to user.
                                //								
                                m_err.log(e, r.toNewString() + " by reference");
                                throw e;
                        }
                }
                return null; // Should be unreachable.
        }

        /**
         * Retrieve this reference returning an object array
         * 
         * @return Returns an array of language dependent class objects (eg
         *         Boolean []) containing the instantaneous result of executing
         *         the query.
         */
        public java.lang.Object[] geta()
                throws UndefinedNameException, UnableToGetDataException
        {
                return geta(this, getArgs());
        }

        /**
         * Retrieve this reference returning an object array Compile if
         * necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return Returns an array of language dependent class objects (eg
         *         Boolean []) containing the instantaneous result of executing
         *         the query.
         */
        public java.lang.Object[] geta(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                return geta(da, null);
        }

        /**
         * Retrieve this reference returning an object array Compile if
         * necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return Returns an array of language dependent class objects (eg
         *         Boolean []) containing the instantaneous result of executing
         *         the query.
         */
        public java.lang.Object[] geta(_DaObject da, String[] args)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return geta(this, da.concatArgs(args));
        }

        /**
         * Retrieve this reference with the given arguments returning an object
         * array.
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return Returns an array of language dependent class objects (eg
         *         Boolean []) containing the instantaneous result of executing
         *         the query.
         */
        public java.lang.Object[] geta(String[] args)
                throws UndefinedNameException, UnableToGetDataException
        {
                return geta(this, args);
        }

        /**
         * Retrieve given reference with given arguments returning an object
         * array
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return Returns an array of language dependent class objects (eg
         *         Boolean []) containing the instantaneous result of executing
         *         the query.
         * @throws IllegalArgumentException
         *         The reference is unknown <code>DaValue.Type</code>, or of
         *         <code>DaValue.Type</code> STRUCT, which is not allowed for
         *         the highest level of the returned object.
         */
        public static java.lang.Object[] geta(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                switch (r.type)
                {
                case Type.STRUCT:
                        throw new IllegalArgumentException(
                                "Can't return a structure as an object directly");

                case Type.ARRAY:
                case Type.SIMPLE:
                        try
                        {
                                switch (r.m_ref.type)
                                {
                                case DaValue.Type.BOOLEANA:
                                {
                                        boolean[] ary = get_booleana(r, args);
                                        Boolean[] newAry = new Boolean[ary.length];
                                        for (int i = 0; i < ary.length; i++)
                                                newAry[i] = new Boolean(ary[i]);
                                        return (newAry);
                                }
                                case DaValue.Type.BYTEA:
                                {
                                        byte[] ary = get_bytea(r, args);
                                        Byte[] newAry = new Byte[ary.length];
                                        for (int i = 0; i < ary.length; i++)
                                                newAry[i] = new Byte(ary[i]);
                                        return (newAry);
                                }

                                case DaValue.Type.CHARA:
                                {
                                        char[] ary = get_chara(r, args);
                                        Character[] newAry = new Character[ary.length];
                                        for (int i = 0; i < ary.length; i++)
                                                newAry[i] = new Character(ary[i]);
                                        return (newAry);
                                }

                                case DaValue.Type.DOUBLEA:
                                {
                                        double[] ary = get_doublea(r, args);
                                        Double[] newAry = new Double[ary.length];
                                        for (int i = 0; i < ary.length; i++)
                                                newAry[i] = new Double(ary[i]);
                                        return (newAry);
                                }

                                case DaValue.Type.FLOATA:
                                {
                                        float[] ary = get_floata(r, args);
                                        Float[] newAry = new Float[ary.length];
                                        for (int i = 0; i < ary.length; i++)
                                                newAry[i] = new Float(ary[i]);
                                        return (newAry);
                                }

                                case DaValue.Type.LONGA:
                                {
                                        int[] ary = get_longa(r, args);
                                        Long[] newAry = new Long[ary.length];
                                        for (int i = 0; i < ary.length; i++)
                                                newAry[i] = new Long(ary[i]);
                                        return (newAry);
                                }

                                case DaValue.Type.LONGDOUBLEA:
                                {
                                        double[] ary = get_doublea(r, args);
                                        Double[] newAry = new Double[ary.length];
                                        for (int i = 0; i < ary.length; i++)
                                                newAry[i] = new Double(ary[i]);
                                        return (newAry);
                                }

                                case DaValue.Type.LONGLONGA:
                                {
                                        int[] ary = get_longa(r, args);
                                        Long[] newAry = new Long[ary.length];
                                        for (int i = 0; i < ary.length; i++)
                                                newAry[i] = new Long(ary[i]);
                                        return (newAry);
                                }

                                case DaValue.Type.SHORTA:
                                {
                                        short[] ary = get_shorta(r, args);
                                        Short[] newAry = new Short[ary.length];
                                        for (int i = 0; i < ary.length; i++)
                                                newAry[i] = new Short(ary[i]);
                                        return (newAry);
                                }

                                case DaValue.Type.STRINGA:
                                        return (get_stringa(r, args));

                                case DaValue.Type.ULONGA:
                                {
                                        int[] ary = get_longa(r, args);
                                        Long[] newAry = new Long[ary.length];
                                        for (int i = 0; i < ary.length; i++)
                                                newAry[i] = new Long(ary[i]);
                                        return (newAry);
                                }

                                case DaValue.Type.ULONGLONGA:
                                {
                                        int[] ary = get_longa(r, args);
                                        Long[] newAry = new Long[ary.length];
                                        for (int i = 0; i < ary.length; i++)
                                                newAry[i] = new Long(ary[i]);
                                        return (newAry);
                                }

                                case DaValue.Type.USHORTA:
                                {
                                        short[] ary = get_shorta(r, args);
                                        Short[] newAry = new Short[ary.length];
                                        for (int i = 0; i < ary.length; i++)
                                                newAry[i] = new Short(ary[i]);
                                        return (newAry);
                                }

                                case DaValue.Type.WCHARA:
                                {
                                        char[] ary = get_chara(r, args);
                                        Character[] newAry = new Character[ary.length];
                                        for (int i = 0; i < ary.length; i++)
                                                newAry[i] = new Character(ary[i]);
                                        return (newAry);
                                }

                                case DaValue.Type.WSTRINGA:
                                        return (get_stringa(r, args));
                                }
                        }
                        catch (UnableToGetDataException e)
                        {
                                // NOTE: This method is made to catch
                                // and rethrow UndefinedNameException
                                // to up-translate the exceptions from
                                // below, eg CORBA error occurs
                                // the error log only gets a CORBA
                                // error. Disconcerting to user.
                                //								
                                m_err.log(e, r.toNewString() + " by reference");
                                throw e;
                        }
                default:
                        throw new IllegalArgumentException("Unrecognized type in get()");
                }

        }

        /**
         * Retrieve this reference returning an Any
         * 
         * @return Returns a CORBA Any containing the instantaneous result of
         *         executing the query.
         */
        public Any getAny()
                throws UndefinedNameException, UnableToGetDataException
        {
                return getAny(this, getArgs());
        }

        /**
         * Retrieve this reference returning an Any Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return Returns a CORBA Any containing the instantaneous result of
         *         executing the query.
         */
        public Any getAny(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                return getAny(da, getArgs());
        }

        /**
         * Retrieve this reference returning an Any Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return Returns a CORBA Any containing the instantaneous result of
         *         executing the query.
         */
        public Any getAny(_DaObject da, String[] args)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return getAny(this, da.concatArgs(args));
        }

        /**
         * Retrieve this reference with the given arguments
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query.
         * @return Returns a CORBA Any containing the instantaneous result of
         *         executing the query.
         */
        public Any getAny(String[] args)
                throws UndefinedNameException, UnableToGetDataException
        {
                return getAny(this, args);
        }

        /**
         * Static method to run given reference with given arguments returning
         * an Any
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return Returns a CORBA Any containing the instantaneous result of
         *         executing the query.
         * @throws IllegalArgumentException
         *         The reference argument has not been compiled
         * @throws UnableToGetDataException
         *         Passed through from remote get method
         */
        public static Any getAny(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                if (!r.fCompiled)
                {
                        throw (IllegalArgumentException) m_err
                                .log(new IllegalArgumentException("DaReference not compiled"));
                }

                switch (r.type)
                {
                // If this is a user defined structure then evaluate the
                // reference as a DaValue and then recreate an Any when
                // done
                case Type.STRUCT:
                {
                        DaValue d = new DaValue();
                        d.type = DaValue.Type.STRUCT;
                        for (Enumeration e = r.elements(); e.hasMoreElements();)
                                d.addElement(((_DaReference) e.nextElement()).getDaValue(args));
                        return d.toAny();
                }
                case Type.ARRAY: // TODO: Process constrained ARRAY differently 
                case Type.SIMPLE:
                        return (r.m_refI.get(r.toNewString(), args));
                default:
                        throw (IllegalArgumentException) m_err
                                .log(new IllegalArgumentException(
                                        "_DaReference::type has unrecognized value."
                                                + "Must be DaValue.Type.STRUCT, ARRAY or SIMPLE"));
                }
        }

        /**
         * Performs set operation with no specified parameter, returning an Any
         * 
         * @return Returns a CORBA Any containing the result of the set operation.
         */
        public Any set()
                throws UndefinedNameException, UnableToSetDataException
        {
            float [] dummyValue = {0.0f};

            DaValue d = new DaValue(dummyValue);

            AnyHolder anyHolder = new AnyHolder(d.toAny());

            Any a = set(this, getArgs(), anyHolder);

            return (a);
        }


        /**
         * Performs set operation with information in AnyHolder, returning an Any
         * 
         * @param anyHolder
         *        An AnyHolder container for an Any.  AnyHolder must be used
         *        rather than Any for in/out parameters.
         * @return Returns a CORBA Any containing the result of the set operation.
         */
        public Any set(AnyHolder anyHolder)
                throws UndefinedNameException, UnableToSetDataException
        {
            Any a = set(this, getArgs(), anyHolder);

            return (a);
        }

        /**
         * Static method to run set operation with given arguments returning
         * an Any
         * 
         * @param r
         *        given _DaReference object
         * @param anyHolder
         *        An AnyHolder container for an Any.  AnyHolder must be used
         *        rather than Any for in/out parameters.
         * @return Returns a CORBA Any containing the result of the set operation.
         * @throws UnableToSetDataException
         *         Passed through from remote get method
         */
        public static Any set(_DaReference r, String[] args, AnyHolder anyHolder)
                throws UnableToSetDataException
        {
            if (!r.fCompiled)
            {
                throw (IllegalArgumentException) m_err
                    .log(new IllegalArgumentException("DaReference not compiled"));
            }

            r.m_refI.set(r.toNewString(), args, anyHolder);

            return (anyHolder.value);
        }

        /**
         * Retrieve this reference returning a DaValue.
         * 
         * @return Returns a DaValue containing the instantaneous result of
         *         executing the query.
         * @throws UnableToGetDataException
         *         Passed through from remote get method
         */
        public DaValue getDaValue()
                throws UnableToGetDataException
        {
                return getDaValue(this, getArgs());
        }

        /**
         * Retrieve this reference returning a DaValue. Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return Returns a DaValue containing the instantaneous result of
         *         executing the query.
         */
        public DaValue getDaValue(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                return getDaValue(da, getArgs());
        }

        /**
         * Retrieve this reference returning a DaValue. Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return Returns a DaValue containing the instantaneous result of
         *         executing the query.
         * @throws UndefinedNameException
         *         _DaObject failed to compile, probably bad Directory Service
         *         data
         * @throws UnableToGetDataException
         *         Passed through from remote get method
         */
        public DaValue getDaValue(_DaObject da, String[] args)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return getDaValue(this, da.concatArgs(args));
        }

        /**
         * Retrieve this reference with the given arguments.
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return Returns a DaValue containing the instantaneous result of
         *         executing the query.
         */
        public DaValue getDaValue(String[] args)
                throws UnableToGetDataException
        {
                return getDaValue(this, args);
        }

        /**
         * Retrieve given reference with given arguments
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return Returns a DaValue containing the instantaneous result of
         *         executing the query.
         * @throws IllegalArgumentException
         *         The reference argument has not been compiled
         */
        public static DaValue getDaValue(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                if (!r.fCompiled)
                {
                        throw (IllegalArgumentException) m_err
                                .log(new IllegalArgumentException("DaReference not compiled"));
                }

                if (r.type == Type.STRUCT)
                        return getDaValuefromStruct(r, args);
                else if (r.m_ref.type < DaValue.Type.BOOLEANA)
                        return getDaValuefromBasic(r, args);
                else
                        return getDaValuefromArray(r, args);
        }

        /**
         * Internal method to get DaValues when it is already determined that
         * the given _DaReference is a user defined structure.
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return Returns a DaValue containing the instantaneous result of
         *         executing the query.
         */
        private static DaValue getDaValuefromStruct(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                DaValue d = new DaValue();

                d.type = DaValue.Type.STRUCT;
                for (Enumeration e = r.elements(); e.hasMoreElements();)
                        d.addElement(((_DaReference) e.nextElement()).getDaValue(args));

                return d;
        }

        /**
         * Internal method to get DaValues when it is already determined that
         * the given _DaReference is a basic type or array of basic types.
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return Returns a DaValue containing the instantaneous result of
         *         executing the query.
         */
        private static DaValue getDaValuefromBasic(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                try
                {
                        switch (r.m_ref.type)
                        {
                        case DaValue.Type.BOOLEAN:
                                return (new DaValue(new Boolean(r.m_refI.get_boolean(r
                                        .toNewString(), args))));
                        case DaValue.Type.BYTE:
                                return (new DaValue(new Byte(r.m_refI.get_byte(r.toNewString(),
                                        args))));
                        case DaValue.Type.CHAR:
                                return (new DaValue(new Character(r.m_refI.get_char(r
                                        .toNewString(), args))));
                        case DaValue.Type.DOUBLE:
                                return (new DaValue(new Double(r.m_refI.get_double(r
                                        .toNewString(), args))));
                        case DaValue.Type.FLOAT:
                                return (new DaValue(new Float(r.m_refI.get_float(r
                                        .toNewString(), args))));
                        case DaValue.Type.LONG:
                                return (new DaValue(new Long(r.m_refI.get_long(r.toNewString(),
                                        args))));
                        case DaValue.Type.LONGDOUBLE:
                                return (new DaValue(new Double(r.m_refI.get_double(r
                                        .toNewString(), args))));
                        case DaValue.Type.LONGLONG:
                                return (new DaValue(new Long(r.m_refI.get_long(r.toNewString(),
                                        args))));
                        case DaValue.Type.SHORT:
                                return (new DaValue(new Short(r.m_refI.get_short(r
                                        .toNewString(), args))));
                        case DaValue.Type.STRING:
                                return (new DaValue(r.m_refI.get_string(r.toNewString(), args)));
                        case DaValue.Type.ULONG:
                                return (new DaValue(new Long(r.m_refI.get_long(r.toNewString(),
                                        args))));
                        case DaValue.Type.ULONGLONG:
                                return (new DaValue(new Long(r.m_refI.get_long(r.toNewString(),
                                        args))));
                        case DaValue.Type.USHORT:
                                return (new DaValue(new Short(r.m_refI.get_short(r
                                        .toNewString(), args))));
                        case DaValue.Type.WCHAR:
                                return (new DaValue(new Character(r.m_refI.get_char(r
                                        .toNewString(), args))));
                        case DaValue.Type.WSTRING:
                                return (new DaValue(r.m_refI.get_string(r.toNewString(), args)));

                        // if type is not known get as an Any
                        default:
                                return (new DaValue(r.m_refI.get(r.toNewString(), args)));
                        }
                }
                catch (UnableToGetDataException e)
                {
                        m_err.log(e, r.toNewString() + " by reference");
                        r.remove(r.toNewString()); // Remove this cache entry
                        throw e;
                }
        }

        /**
         * Internal method to get DaValues when it is already determined that
         * the given _DaReference is a user constrained array.
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return Returns a DaValue containing the instantaneous result of
         *         executing the query.
         */
        private static DaValue getDaValuefromArray(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                try
                {
                        switch (r.m_ref.type)
                        {
                        case DaValue.Type.BOOLEANA:
                                return (new DaValue(r.m_refI
                                        .get_booleana(r.toNewString(), args)));
                        case DaValue.Type.BYTEA:
                                return (new DaValue(r.m_refI.get_bytea(r.toNewString(), args)));
                        case DaValue.Type.CHARA:
                                return (new DaValue(r.m_refI.get_chara(r.toNewString(), args)));
                        case DaValue.Type.DOUBLEA:
                                return (new DaValue(r.m_refI.get_doublea(r.toNewString(), args)));
                        case DaValue.Type.FLOATA:
                                return (new DaValue(r.m_refI.get_floata(r.toNewString(), args)));
                        case DaValue.Type.LONGA:
                                return (new DaValue(r.m_refI.get_longa(r.toNewString(), args)));
                        case DaValue.Type.LONGDOUBLEA:
                                return (new DaValue(r.m_refI.get_doublea(r.toNewString(), args)));
                        case DaValue.Type.LONGLONGA:
                                return (new DaValue(r.m_refI.get_longa(r.toNewString(), args)));
                        case DaValue.Type.SHORTA:
                                return (new DaValue(r.m_refI.get_shorta(r.toNewString(), args)));
                        case DaValue.Type.STRINGA:
                                return (new DaValue(r.m_refI.get_stringa(r.toNewString(), args)));
                        case DaValue.Type.ULONGA:
                                return (new DaValue(r.m_refI.get_longa(r.toNewString(), args)));
                        case DaValue.Type.ULONGLONGA:
                                return (new DaValue(r.m_refI.get_longa(r.toNewString(), args)));
                        case DaValue.Type.USHORTA:
                                return (new DaValue(r.m_refI.get_shorta(r.toNewString(), args)));
                        case DaValue.Type.WCHARA:
                                return (new DaValue(r.m_refI.get_chara(r.toNewString(), args)));
                        case DaValue.Type.WSTRINGA:
                                return (new DaValue(r.m_refI.get_stringa(r.toNewString(), args)));

                        // if type is not known get as an Any
                        default:
                                return (new DaValue(r.m_refI.get(r.toNewString(), args)));
                        }
                }
                catch (UnableToGetDataException e)
                {
                        m_err.log(e, r.toNewString() + " by reference");
                        r.remove(r.toNewString()); // Remove this cache entry
                        throw e;
                }
        }

        /**
         * Return outer-level arguments of this _DaReference. Only works for
         * outer level _DaReference
         * 
         * @return Arguments tied to this _DaReference
         */
        public String[] getArgs()
        {
                int i = 0;
                String[] args = null;

                if (!m_args.isEmpty())
                        args = new String[m_args.size() * 2];
                for (Enumeration e = m_args.keys(); e.hasMoreElements();)
                {
                        String param;
                        args[i++] = param = (String) e.nextElement();
                        args[i++] = (String) m_args.get(param);
                }
                if (args == null)
                {
                        args = new String[2];
                        args[0] = "null"; // at least one parameter needed for
                        // CORBA
                        args[1] = " "; // at least one parameter needed for
                        // CORBA
                }
                return args;
        }

        /**
         * Internal method for getting the Instance/Attribute string from this
         * vector of a _DaReference.
         * 
         * @return Instance {separator} Attribute
         */
        private String unAdornedInstanceAttribute()
        {
                return (((instance == null) ? "" : instance) + AQSToken.SEPARATOR + ((attribute == null) ? ""
                        : attribute));
        }

        /**
         * Internal method for getting the Instance/Attribute string from this
         * vector of a _DaReference including name and enclosing braces.
         * 
         * @return Name {assignment_operator} {open_brace} Instance {separator}
         *         Attribute {close_brace}
         */
        private String adornedInstanceAttribute()
        {
                return (((name == null) ? "" : (name + AQSToken.NAMED_ELEMENT))
                        + AQSToken.STRUCT_OPEN + unAdornedInstanceAttribute() + AQSToken.STRUCT_CLOSE);
        }

        /**
         * Internal method for getting the Instance/Attribute string from this
         * vector of a _DaReference including name and enclosing braces where
         * necessary.
         * 
         * @return Name {assignment_operator} {open_brace} Instance {separator}
         *         Attribute {close_brace}
         */
        private String instanceAttribute()
        {
                if (name == null)
                        return unAdornedInstanceAttribute();
                else
                        return adornedInstanceAttribute();
        }

        /**
         * Internal method for getting the array specifier string from this
         * vector of a _DaReference.
         * 
         * @return {open_square_braces} LowerBound {hiphen} UpperBound
         *         {open_square_braces} ...
         */
        private String arraySpecifier()
        {
                String anArraySpecifier = "";
                for (Enumeration e = bounds.elements(); e.hasMoreElements();)
                {
                        ArrayBound theseBounds = (ArrayBound) e.nextElement();
                        anArraySpecifier += AQSToken.ARRAY_OPEN;
                        switch (theseBounds.type)
                        {
                        case ArrayBound.Type.SIMPLE:
                                anArraySpecifier += ((SimpleBound) theseBounds).index;
                                break;

                        case ArrayBound.Type.NAMED:
                                for (Enumeration names = ((NamedBound) theseBounds).names
                                        .elements(); names.hasMoreElements();)
                                {
                                        anArraySpecifier += names.nextElement();
                                        if (names.hasMoreElements())
                                                anArraySpecifier += ",";
                                }
                                break;

                        case ArrayBound.Type.RANGE:
                                anArraySpecifier += arrayBoundSpecifier(((RangeBound) theseBounds).lowerBound.expr);
                                if (((RangeBound) theseBounds).upperBound != null)
                                {
                                        anArraySpecifier += AQSToken.MINUS;
                                        anArraySpecifier += arrayBoundSpecifier(((RangeBound) theseBounds).upperBound.expr);
                                }
                                break;
                        }
                        anArraySpecifier += AQSToken.ARRAY_CLOSE;
                }

                return anArraySpecifier;
        }

        /**
         * Internal method for getting a single bound of an array specifier
         * string from the given AidaExpr
         * 
         * @return String representation of this bound
         */
        private String arrayBoundSpecifier(AidaExpr b)
        {
                if (b == null)
                        return ("");

                switch (b.type)
                {
                case AidaExpr.Type.LITERAL:
                        return (((AidaLiteral) b).literal.toString());

                case AidaExpr.Type.VARIABLE:
                        return (((AidaVariable) b).name);

                case AidaExpr.Type.ADD:
                        return (AQSToken.OPEN_PAREN
                                + arrayBoundSpecifier(((AidaSumExpr) b).lhs) + AQSToken.PLUS
                                + arrayBoundSpecifier(((AidaSumExpr) b).rhs) + AQSToken.CLOSE_PAREN);

                case AidaExpr.Type.SUBTRACT:
                        return (AQSToken.OPEN_PAREN
                                + arrayBoundSpecifier(((AidaSubtractExpr) b).lhs)
                                + AQSToken.MINUS
                                + arrayBoundSpecifier(((AidaSubtractExpr) b).rhs) + AQSToken.CLOSE_PAREN);
                }
                return "";
        }

        /**
         * Internal method for getting the user defined structure specifier
         * string from this vector of a _DaReference.
         * 
         * @return AQS structure
         */
        private String structSpecifier()
        {
                if (name == null)
                        return unAdornedStructSpecifier();
                else
                        return adornedStructSpecifier();
        }

        /**
         * Internal method for getting the user defined structure specifier
         * string from this vector of a _DaReference.
         * 
         * @return AQS structure
         */
        private String unAdornedStructSpecifier()
        {
                String aStructSpecifier = "";
                boolean first = true;
                for (Enumeration e = elements(); e.hasMoreElements();)
                {
                        _DaReference areference = (_DaReference) e.nextElement();

                        if (first)
                                first = false;
                        else
                                aStructSpecifier += AQSToken.STRUCT_SEPARATOR;

                        if (fUseNewString)
                                aStructSpecifier += areference.toNewString();
                        else
                                aStructSpecifier += areference.toString();
                }

                return aStructSpecifier;
        }

        /**
         * Internal method for getting the user defined structure specifier
         * string from this vector of a _DaReference.
         * 
         * @return AQS structure
         */
        private String adornedStructSpecifier()
        {
                return (((name == null) ? "" : (name + AQSToken.NAMED_ELEMENT))
                        + AQSToken.STRUCT_OPEN + unAdornedStructSpecifier() + AQSToken.STRUCT_CLOSE);
        }

        // Getting elementary intrinsic types
        // 
        /**
         * Retrieve this reference
         * 
         * @return intrinsic boolean
         */
        public boolean get_boolean()
                throws UnableToGetDataException
        {
                return get_boolean(this, getArgs());
        }

        /**
         * Retrieve this reference Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return intrinsic boolean
         * @throws UndefinedNameException
         *         _DaObject failed to compile, probably bad Directory Service
         *         data
         */
        public boolean get_boolean(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return get_boolean(this, getArgs());
        }

        /**
         * Retrieve this reference
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic boolean
         */
        public boolean get_boolean(String[] args)
                throws UnableToGetDataException
        {
                return get_boolean(this, args);
        }

        /**
         * Retrieve this reference
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic boolean
         * @throws IllegalArgumentException
         *         _DaReference not compiled before calling get method
         */
        public static boolean get_boolean(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                String query = r.toNewString();
                if (!r.fCompiled)
                {
                        throw (IllegalArgumentException) m_err
                                .log(new IllegalArgumentException("DaReference not compiled"));
                }

                try
                {
                        return (r.m_refI.get_boolean(query, args));
                }
                catch (Exception ex)
                {
                        r.remove(query); // Remove this cache entry
                        m_err.log(ex, query + printArgs(args));
                        throw new UnableToGetDataException(ex.getMessage());
                }
        }

        /**
         * Retrieve this reference.
         * 
         * @return intrinsic byte
         */
        public byte get_byte()
                throws UnableToGetDataException
        {
                return get_byte(this, getArgs());
        }

        /**
         * Retrieve this reference Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return intrinsic byte
         */
        public byte get_byte(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return get_byte(this, getArgs());
        }

        /**
         * Retrieve this reference
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic byte
         */
        public byte get_byte(String[] args)
                throws UnableToGetDataException
        {
                return get_byte(this, args);
        }

        /**
         * Retrieve this reference
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic byte
         * @throws IllegalArgumentException
         *         _DaReference not compiled before calling get method
         */
        public static byte get_byte(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                String query = r.toNewString();
                if (!r.fCompiled)
                {
                        throw (IllegalArgumentException) m_err
                                .log(new IllegalArgumentException("DaReference not compiled"));
                }

                try
                {
                        return (r.m_refI.get_byte(r.toNewString(), args));
                }
                catch (Exception ex)
                {
                        r.remove(query); // Remove this cache entry
                        m_err.log(ex, query + printArgs(args));
                        throw new UnableToGetDataException(ex.getMessage());
                }
        }

        /**
         * Retrieve this reference.
         * 
         * @return intrinsic char
         */
        public char get_char()
                throws UnableToGetDataException
        {
                return get_char(this, getArgs());
        }

        /**
         * Retrieve this reference Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return intrinsic char
         */
        public char get_char(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return get_char(this, getArgs());
        }

        /**
         * Retrieve this reference
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic char
         */
        public char get_char(String[] args)
                throws UnableToGetDataException
        {
                return get_char(this, args);
        }

        /**
         * Retrieve this reference
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic char
         * @throws IllegalArgumentException
         *         _DaReference not compiled before calling get method
         */
        public static char get_char(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                String query = r.toNewString();
                if (!r.fCompiled)
                {
                        throw (IllegalArgumentException) m_err
                                .log(new IllegalArgumentException("DaReference not compiled"));
                }

                try
                {
                        return (r.m_refI.get_char(r.toNewString(), args));
                }
                catch (Exception ex)
                {
                        r.remove(query); // Remove this cache entry
                        m_err.log(ex, query + printArgs(args));
                        throw new UnableToGetDataException(ex.getMessage());
                }
        }

        /**
         * Retrieve this reference
         * 
         * @return intrinsic double
         */
        public double get_double()
                throws UnableToGetDataException
        {
                return get_double(this, getArgs());
        }

        /**
         * Retrieve this reference Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return intrinsic double
         */
        public double get_double(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return get_double(this, getArgs());
        }

        /**
         * Retrieve this reference
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic double
         */
        public double get_double(String[] args)
                throws UnableToGetDataException
        {
                return get_double(this, args);
        }

        /**
         * Retrieve this reference
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic double
         * @throws IllegalArgumentException
         *         _DaReference not compiled before calling get method
         */
        public static double get_double(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                String query = r.toNewString();
                if (!r.fCompiled)
                {
                        throw (IllegalArgumentException) m_err
                                .log(new IllegalArgumentException("DaReference not compiled"));
                }

                try
                {
                        return (r.m_refI.get_double(query, args));
                }
                catch (Exception ex)
                {
                        r.remove(query); // Remove this cache entry
                        m_err.log(ex, query + printArgs(args));
                        throw new UnableToGetDataException(ex.getMessage());
                }
        }

        /**
         * Retrieve this reference
         * 
         * @return intrinsic float
         */
        public float get_float()
                throws UnableToGetDataException
        {
                return get_float(this, getArgs());
        }

        /**
         * Retrieve this reference Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return intrinsic float
         */
        public float get_float(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return get_float(this, getArgs());
        }

        /**
         * Retrieve this reference
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic float
         */
        public float get_float(String[] args)
                throws UnableToGetDataException
        {
                return get_float(this, args);
        }

        /**
         * Retrieve this reference
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic float
         * @throws IllegalArgumentException
         *         _DaReference not compiled before calling get method
         */
        public static float get_float(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                String query = r.toNewString();
                if (!r.fCompiled)
                {
                        throw (IllegalArgumentException) m_err
                                .log(new IllegalArgumentException("DaReference not compiled"));
                }

                try
                {
                        return (r.m_refI.get_float(query, args));
                }
                catch (Exception ex)
                {
                        r.remove(query); // Remove this cache entry
                        m_err.log(ex, query + printArgs(args));
                        throw new UnableToGetDataException(ex.getMessage());
                }
        }

        /**
         * Retrieve this reference
         * 
         * @return intrinsic long
         */
        public long get_long()
                throws UnableToGetDataException
        {
                return get_long(this, getArgs());
        }

        /**
         * Retrieve this reference Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return intrinsic long
         */
        public long get_long(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return get_long(this, getArgs());
        }

        /**
         * Retrieve this reference
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic long
         */
        public long get_long(String[] args)
                throws UnableToGetDataException
        {
                return get_long(this, args);
        }

        /**
         * Retrieve this reference
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic long
         * @throws IllegalArgumentException
         *         _DaReference not compiled before calling get method
         */
        public static long get_long(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                String query = r.toNewString();
                if (!r.fCompiled)
                {
                        throw (IllegalArgumentException) m_err
                                .log(new IllegalArgumentException("DaReference not compiled"));
                }

                try
                {
                        return (r.m_refI.get_long(query, args));
                }
                catch (Exception ex)
                {
                        r.remove(query); // Remove this cache entry
                        m_err.log(ex, query + printArgs(args));
                        throw new UnableToGetDataException(ex.getMessage());
                }
        }

        /**
         * Retrieve this reference
         * 
         * @return intrinsic short
         */
        public short get_short()
                throws UnableToGetDataException
        {
                return get_short(this, getArgs());
        }

        /**
         * Retrieve this reference Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return intrinsic short
         */
        public short get_short(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return get_short(this, getArgs());
        }

        /**
         * Retrieve this reference
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic short
         */
        public short get_short(String[] args)
                throws UnableToGetDataException
        {
                return get_short(this, args);
        }

        /**
         * Retrieve this reference
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic short
         * @throws IllegalArgumentException
         *         _DaReference not compiled before calling get method
         */
        public static short get_short(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                String query = r.toNewString();
                if (!r.fCompiled)
                {
                        throw (IllegalArgumentException) m_err
                                .log(new IllegalArgumentException("DaReference not compiled"));
                }

                try
                {
                        return (r.m_refI.get_short(query, args));
                }
                catch (Exception ex)
                {
                        r.remove(query); // Remove this cache entry
                        m_err.log(ex, query + printArgs(args));
                        throw new UnableToGetDataException(ex.getMessage());
                }
        }

        /**
         * Retrieve this reference
         * 
         * @return intrinsic String object
         */
        public String get_string()
                throws UnableToGetDataException
        {
                return get_string(this, getArgs());
        }

        /**
         * Retrieve this reference Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return intrinsic String object
         */
        public String get_string(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return get_string(this, getArgs());
        }

        /**
         * Retrieve this reference
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic String object
         */
        public String get_string(String[] args)
                throws UnableToGetDataException
        {
                return get_string(this, args);
        }

        /**
         * Retrieve this reference
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic String object
         * @throws IllegalArgumentException
         *         _DaReference not compiled before calling get method
         */
        public static String get_string(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                String query = r.toNewString();
                if (!r.fCompiled)
                {
                        throw (IllegalArgumentException) m_err
                                .log(new IllegalArgumentException("DaReference not compiled"));
                }

                try
                {
                        return (r.m_refI.get_string(query, args));
                }
                catch (Exception ex)
                {
                        r.remove(query); // Remove this cache entry
                        m_err.log(ex, query + printArgs(args));
                        throw new UnableToGetDataException(ex.getMessage());
                }
        }

        /**
         * Retrieve this reference
         * 
         * @return intrinsic boolean array
         */
        public boolean[] get_booleana()
                throws UnableToGetDataException
        {
                return get_booleana(this, getArgs());
        }

        /**
         * Retrieve this reference Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return intrinsic boolean array
         */
        public boolean[] get_booleana(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return get_booleana(this, getArgs());
        }

        /**
         * Retrieve this reference
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic boolean array
         */
        public boolean[] get_booleana(String[] args)
                throws UnableToGetDataException
        {
                return get_booleana(this, args);
        }

        /**
         * Retrieve this reference
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic boolean array
         * @throws IllegalArgumentException
         *         _DaReference not compiled before calling get method
         */
        public static boolean[] get_booleana(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                String query = r.toNewString();
                if (!r.fCompiled)
                {
                        throw (IllegalArgumentException) m_err
                                .log(new IllegalArgumentException("DaReference not compiled"));
                }

                try
                {
                        return (r.m_refI.get_booleana(query, args));
                }
                catch (Exception ex)
                {
                        r.remove(query); // Remove this cache entry
                        m_err.log(ex, query + printArgs(args));
                        throw new UnableToGetDataException(ex.getMessage());
                }
        }

        /**
         * Retrieve this reference
         * 
         * @return intrinsic byte array
         */
        public byte[] get_bytea()
                throws UnableToGetDataException
        {
                return get_bytea(this, getArgs());
        }

        /**
         * Retrieve this reference Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return intrinsic byte array
         */
        public byte[] get_bytea(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return get_bytea(this, getArgs());
        }

        /**
         * Retrieve this reference
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic byte array
         */
        public byte[] get_bytea(String[] args)
                throws UnableToGetDataException
        {
                return get_bytea(this, args);
        }

        /**
         * Retrieve this reference
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic byte array
         * @throws IllegalArgumentException
         *         _DaReference not compiled before calling get method
         */
        public static byte[] get_bytea(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                String query = r.toNewString();
                if (!r.fCompiled)
                {
                        throw (IllegalArgumentException) m_err
                                .log(new IllegalArgumentException("DaReference not compiled"));
                }

                try
                {
                        return (r.m_refI.get_bytea(query, args));
                }
                catch (Exception ex)
                {
                        r.remove(query); // Remove this cache entry
                        m_err.log(ex, query + printArgs(args));
                        throw new UnableToGetDataException(ex.getMessage());
                }
        }

        /**
         * Retrieve this reference
         * 
         * @return intrinsic char array
         */
        public char[] get_chara()
                throws UnableToGetDataException
        {
                return get_chara(this, getArgs());
        }

        /**
         * Retrieve this reference Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return intrinsic char array
         */
        public char[] get_chara(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return get_chara(this, getArgs());
        }

        /**
         * Retrieve this reference
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic char array
         */
        public char[] get_chara(String[] args)
                throws UnableToGetDataException
        {
                return get_chara(this, args);
        }

        /**
         * Retrieve this reference
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic char array
         * @throws IllegalArgumentException
         *         _DaReference not compiled before calling get method
         */
        public static char[] get_chara(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                String query = r.toNewString();
                if (!r.fCompiled)
                {
                        throw (IllegalArgumentException) m_err
                                .log(new IllegalArgumentException("DaReference not compiled"));
                }

                try
                {
                        return (r.m_refI.get_chara(query, args));
                }
                catch (Exception ex)
                {
                        r.remove(query); // Remove this cache entry
                        m_err.log(ex, query + printArgs(args));
                        throw new UnableToGetDataException(ex.getMessage());
                }
        }

        /**
         * Retrieve this reference
         * 
         * @return intrinsic double array
         */
        public double[] get_doublea()
                throws UnableToGetDataException
        {
                return get_doublea(this, getArgs());
        }

        /**
         * Retrieve this reference Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return intrinsic double array
         */
        public double[] get_doublea(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return get_doublea(this, getArgs());
        }

        /**
         * Retrieve this reference
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic double array
         */
        public double[] get_doublea(String[] args)
                throws UnableToGetDataException
        {
                return get_doublea(this, args);
        }

        /**
         * Retrieve this reference
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic double array
         * @throws IllegalArgumentException
         *         _DaReference not compiled before calling get method
         */
        public static double[] get_doublea(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                String query = r.toNewString();
                if (!r.fCompiled)
                {
                        throw (IllegalArgumentException) m_err
                                .log(new IllegalArgumentException("DaReference not compiled"));
                }

                try
                {
                        return (r.m_refI.get_doublea(query, args));
                }
                catch (Exception ex)
                {
                        r.remove(query); // Remove this cache entry
                        m_err.log(ex, query + printArgs(args));
                        throw new UnableToGetDataException(ex.getMessage());
                }
        }

        /**
         * Retrieve this reference
         * 
         * @return intrinsic float array
         */
        public float[] get_floata()
                throws UnableToGetDataException
        {
                return get_floata(this, getArgs());
        }

        /**
         * Retrieve this reference Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return intrinsic float array
         */
        public float[] get_floata(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return get_floata(this, getArgs());
        }

        /**
         * Retrieve this reference
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic float array
         */
        public float[] get_floata(String[] args)
                throws UnableToGetDataException
        {
                return get_floata(this, args);
        }

        /**
         * Retrieve this reference
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic float array
         * @throws IllegalArgumentException
         *         _DaReference not compiled before calling get method
         */
        public static float[] get_floata(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                String query = r.toNewString();
                if (!r.fCompiled)
                {
                        throw (IllegalArgumentException) m_err
                                .log(new IllegalArgumentException("DaReference not compiled"));
                }

                try
                {
                        return (r.m_refI.get_floata(query, args));
                }
                catch (Exception ex)
                {
                        r.remove(query); // Remove this cache entry
                        m_err.log(ex, query + printArgs(args));
                        throw new UnableToGetDataException(ex.getMessage());
                }
        }

        /**
         * Retrieve this reference
         * 
         * @return intrinsic long array
         */
        public int[] get_longa()
                throws UnableToGetDataException
        {
                return get_longa(this, getArgs());
        }

        /**
         * Retrieve this reference Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return intrinsic long array
         */
        public int[] get_longa(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return get_longa(this, getArgs());
        }

        /**
         * Retrieve this reference
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic long array
         */
        public int[] get_longa(String[] args)
                throws UnableToGetDataException
        {
                return get_longa(this, args);
        }

        /**
         * Retrieve this reference
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic long array
         * @throws IllegalArgumentException
         *         _DaReference not compiled before calling get method
         */
        public static int[] get_longa(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                String query = r.toNewString();
                if (!r.fCompiled)
                {
                        throw (IllegalArgumentException) m_err
                                .log(new IllegalArgumentException("DaReference not compiled"));
                }

                try
                {
                        return (r.m_refI.get_longa(query, args));
                }
                catch (Exception ex)
                {
                        r.remove(query); // Remove this cache entry
                        m_err.log(ex, query + printArgs(args));
                        throw new UnableToGetDataException(ex.getMessage());
                }
        }

        /**
         * Retrieve this reference
         * 
         * @return intrinsic short array
         */
        public short[] get_shorta()
                throws UnableToGetDataException
        {
                return get_shorta(this, getArgs());
        }

        /**
         * Retrieve this reference Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return intrinsic short array
         */
        public short[] get_shorta(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return get_shorta(this, getArgs());
        }

        /**
         * Retrieve this reference
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic short array
         */
        public short[] get_shorta(String[] args)
                throws UnableToGetDataException
        {
                return get_shorta(this, args);
        }

        /**
         * Retrieve this reference
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic short array
         * @throws IllegalArgumentException
         *         _DaReference not compiled before calling get method
         */
        public static short[] get_shorta(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                String query = r.toNewString();
                if (!r.fCompiled)
                {
                        throw (IllegalArgumentException) m_err
                                .log(new IllegalArgumentException("DaReference not compiled"));
                }

                try
                {
                        return (r.m_refI.get_shorta(query, args));
                }
                catch (Exception ex)
                {
                        r.remove(query); // Remove this cache entry
                        m_err.log(ex, query + printArgs(args));
                        throw new UnableToGetDataException(ex.getMessage());
                }
        }

        /**
         * Retrieve this reference
         * 
         * @return intrinsic String object array
         */
        public String[] get_stringa()
                throws UnableToGetDataException
        {
                return get_stringa(this, getArgs());
        }

        /**
         * Retrieve this reference Compile if necessary
         * 
         * @param da
         *        Data Access Object used to access AIDA to compile the
         *        _DaReference
         * @return intrinsic String object array
         */
        public String[] get_stringa(_DaObject da)
                throws UndefinedNameException, UnableToGetDataException
        {
                if (!fCompiled)
                        compile(da);
                return get_stringa(this, getArgs());
        }

        /**
         * Retrieve this reference
         * 
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic String object array
         */
        public String[] get_stringa(String[] args)
                throws UnableToGetDataException
        {
                return get_stringa(this, args);
        }

        /**
         * Retrieve this reference
         * 
         * @param r
         *        given _DaReference object
         * @param args
         *        Arguments to be added to any Data Access Object parameters and
         *        sent to the service to add context to the query
         * @return intrinsic String object array
         * @throws IllegalArgumentException
         *         _DaReference not compiled before calling get method
         */
        public static String[] get_stringa(_DaReference r, String[] args)
                throws UnableToGetDataException
        {
                String query = r.toNewString();
                if (!r.fCompiled)
                {
                        throw (IllegalArgumentException) m_err
                                .log(new IllegalArgumentException("DaReference not compiled"));
                }

                try
                {
                        return (r.m_refI.get_stringa(query, args));
                }
                catch (Exception ex)
                {
                        r.remove(query); // Remove this cache entry
                        m_err.log(ex, query + printArgs(args));
                        throw new UnableToGetDataException(ex.getMessage());
                }
        }

        /**
         * Converts the String array args to a single String suitable for
         * printing.
         * 
         * @param args
         *        The parameters client program supplied to a DaObject method
         * @return A single String, formed by concatenation of String[] args.
         */
        private static String printArgs(String[] args)
        {
                String s = ""; // Ret var: args as a single string.

                // nArgs is the effective number of of parameters. The AIDA
                // CORBA API results in at least 1 arg always, because the
                // AidaObject API does not define versions of each API method
                // with no args arg, so CORBA must supply the args arg. The
                // Aida convention is that in this case args[0] is set to teh
                // String "null". That is args[0]==null means no parameters
                // were given to the AIDA API method.
                int nArgs = ((args[0].compareTo("null") == 0) ? 0 : args.length);
                for (int i = 0; i < nArgs; i++)
                        s = s + " " + args[i];
                return s;
        }

}
