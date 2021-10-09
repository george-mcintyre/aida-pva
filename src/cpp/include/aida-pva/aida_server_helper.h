#ifndef _Included_aida_server_helper
#define _Included_aida_server_helper
#ifdef __cplusplus
extern "C" {
#endif

#include <jni.h>
#include <descrip.h>              /* for definition of $DESCRIPTOR  */
#include <stdbool.h>
#include <stsdef.h>               /* Macros for handling VMS status */
#include "aida_jni_helper.h"

#include "slc_macros.h"
#include "msg_proto.h"            /* for standalone_init */
#include "errtranslate.h"
#include "aida_types.h"
#include "json.h"
#include "ref.h"                  /* passing by reference macros */
#include "process_parm.h"

// Override prototypes of externals to uppercase names, since compile.com
// adds cc/names=UPPERCASE on compiles by default, but if the ATTRIBUTE=JNI
// is in effect (as is for this module), then it's /names=AS_IS.

void ERRTRANSLATE(const unsigned long int* errcode_p, struct dsc$descriptor* msgdsc_ps);

// Supported exceptions
#define AIDA_INTERNAL_EXCEPTION "AidaInternalException"
#define SERVER_INITIALISATION_EXCEPTION "ServerInitialisationException"
#define UNABLE_TO_GET_DATA_EXCEPTION "UnableToGetDataException"
#define UNABLE_TO_SET_DATA_EXCEPTION "UnableToSetDataException"
#define UNSUPPORTED_CHANNEL_EXCEPTION "UnsupportedChannelException"
#define MISSING_REQUIRED_ARGUMENT_EXCEPTION "MissingRequiredArgumentException"

#define MAX_ERROR_TEXT_LEN 100
#define MAX_POINTERS 100
#define MAX_PMU_LEN 18
#define MAX_URI_LEN 30
#define PRIM_LEN 4
#define MICRO_LEN 4

/**
 * Create tracking variables so that memory can be freed with FREE_MEMORY
 */
#define TRACK_ALLOCATED_MEMORY \
    int nAllocationsToFree = 0, nJsonValuesToFree = 0; \
    void *memoryAllocationsToFree[MAX_POINTERS] ; \
    json_value *jsonValuesToFree[MAX_POINTERS] ;

/**
 * Register this newly allocated memory so that it will be freed by FREE_MEMORY
 */
#define TRACK_MEMORY(_ptr) \
    if (_ptr) memoryAllocationsToFree[nAllocationsToFree++] = (_ptr);

/**
 * Register this newly allocated json value so that it will be freed by FREE_JSON_MEMORY
 */
#define TRACK_JSON(_ptr) \
    if (_ptr) jsonValuesToFree[nJsonValuesToFree++] = (_ptr);

#define ALLOCATE_MEMORY(_env, _size, _purpose) allocateMemory(_env, NULL, _size, false, "Could not allocate space for " _purpose)
#define ALLOCATE_MEMORY_OR_RETURN(_env, _var, _size, _purpose, _r) { \
    void * _aptr = allocateMemory(_env, NULL, _size, false, "Could not allocate space for " _purpose); \
    if (!_aptr) \
        return _r; \
    (_var) = _aptr; \
}

#define ALLOCATE_AND_SET_MEMORY(_env, _source, _size, _purpose) allocateMemory(_env, _source, _size, false, "Could not allocate space for " _purpose)
#define ALLOCATE_AND_SET_MEMORY_OR_RETURN_VOID(_env, _var, _source, _size, _purpose) \
if (!( (_var) = ALLOCATE_AND_SET_MEMORY(_env, _source, _size, false, _purpose))) { \
    return; \
}

#define ALLOCATE_STRING(_env, _string, _purpose) ALLOCATE_AND_SET_MEMORY(_env, _string, strlen(_string)+1, _purpose)
#define ALLOCATE_FIXED_LENGTH_STRING(_env, _string, _size, _purpose) allocateMemory(_env, _string, _size, true, "Could not allocate space for " _purpose)
#define ALLOCATE_STRING_OR_RETURN_VOID(_env, _var, _string, _purpose) \
if (!( (_var) = ALLOCATE_STRING(_env, _string, _purpose))) { \
    return; \
}
#define ALLOCATE_FIXED_LENGTH_STRING_OR_RETURN_VOID(_env, _var, _string, _size, _purpose) \
if (!( (_var) = ALLOCATE_FIXED_LENGTH_STRING(_env, _string, _size, _purpose))) { \
    return; \
}

#define ALLOCATE_STRING_OR_RETURN(_env, _var, _string, _purpose, _r) \
if (!( (_var) = ALLOCATE_STRING(_env, _string, _purpose))) { \
    return _r; \
}

#define ALLOCATE_AND_TRACK_MEMORY(_env, _var, _size, _purpose, _r) \
{ \
    void *_aptr = ALLOCATE_MEMORY(_env, _size, _purpose); \
    if ( !_aptr ) { \
        FREE_MEMORY \
        return _r; \
    } \
    TRACK_MEMORY(_aptr) \
    (_var) = _aptr; \
}

#define ALLOCATE_SET_AND_TRACK_MEMORY(_env, _var, _source, _size, _purpose, _r) \
{ \
    void *_aptr = ALLOCATE_AND_SET_MEMORY(_env, _source, _size, _purpose); \
    if ( !_aptr ) { \
        FREE_MEMORY \
        return _r; \
    } \
    TRACK_MEMORY(_aptr) \
    (_var) = _aptr; \
}

#define ALLOCATE_AND_TRACK_STRING(_env, _var, _string, _purpose, _r) \
{ \
    void *_aptr = ALLOCATE_STRING(_env, _string, _purpose); \
    if ( !_aptr ) { \
        FREE_MEMORY \
        return _r; \
    } \
    TRACK_MEMORY(_aptr) \
    (_var) = _aptr; \
}

#define ALLOCATE_AND_TRACK_FIXED_LENGTH_STRING(_env, _var, _string, _size, _purpose, _r) \
{ \
    void *_aptr = ALLOCATE_FIXED_LENGTH_STRING(_env, _string, _size, _purpose); \
    if ( !_aptr ) { \
        FREE_MEMORY \
        return _r; \
    } \
    TRACK_MEMORY(_aptr) \
    (_var) = _aptr; \
}

/**
 * Free any allocated json memory
 */
#define FREE_JSON \
{                              \
    while ( nJsonValuesToFree-- > 0) { \
        if ( jsonValuesToFree[nJsonValuesToFree] ) \
            json_value_free(jsonValuesToFree[nJsonValuesToFree]); \
    } \
}

/**
 * Free any allocated memory
 */
#define FREE_MEMORY \
{                              \
    while ( nAllocationsToFree-- > 0) { \
        if ( memoryAllocationsToFree[nAllocationsToFree])  \
            free (memoryAllocationsToFree[nAllocationsToFree]); \
    } \
    FREE_JSON \
}

/**
 * Free a single tracked memory allocation and remove from list
 */
#define FREE_TRACKED_MEMORY(_ptr) \
{ \
    if ( _ptr) {  \
    \
        bool found = false; \
        for ( int i = 0 ; i < nAllocationsToFree; i++ ) { \
            if ( (_ptr) == memoryAllocationsToFree[i] ) {   \
                free (_ptr);                    \
                found = true;                    \
            } \
            if ( found && i != (nAllocationsToFree-1) )   \
                memoryAllocationsToFree[i] = memoryAllocationsToFree[i+1]; \
        } \
        if ( found )   \
            nAllocationsToFree--;     \
    }\
}

/**
 * Format an error message, throw it in an exception, and return the error code
 */
#define SPRINF_ERROR(_exception, _errorText, _ref, _r) \
{ \
    char error[MAX_ERROR_TEXT_LEN + strlen(_ref)]; \
    sprintf(error, _errorText,  _ref); \
    aidaThrowNonOsException(env, _exception, error); \
    return _r; \
}

/**
 * Format an error message, throw it in an exception, free any allocated memory and return the error code
 */
#define SPRINF_ERROR_AND_FREE_MEMORY(_exception, _errorText, _ref, _r) \
{ \
    char error[MAX_ERROR_TEXT_LEN + strlen(_ref)]; \
    sprintf(error, _errorText,  _ref); \
    aidaThrowNonOsException(env, _exception, error); \
    FREE_MEMORY \
    return _r; \
}

/**
 * Return an empty table response
 */
#define RETURN_NULL_TABLE \
    Table nullTable; \
    nullTable.columnCount = 0; \
    return nullTable;

/**
 * Throw unsupported channel exception and return a blank array
 */
#define UNSUPPORTED_ARRAY_REQUEST \
    aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri); \
    Array array; \
    return array;

/**
 * Throw an unsupported channel exception and return an empty string array
 */
#define UNSUPPORTED_STRING_ARRAY_REQUEST \
    aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri); \
    StringArray stringArray; \
    return stringArray;

/**
 * Throw an unsupported channel exception and return an empty table
 */
#define UNSUPPORTED_TABLE_REQUEST \
    aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri); \
    Table table; \
    return table;

/**
 * Get a PMU (Primary-Micro-Unit) string from the supplied URI
 * @param _uri the uri to pull the pmu string from
 * @param _var the name of the variable to store the resulting pmu string
 */
#define PMU_STRING_FROM_URI(_uri, _var)  \
    char _var[MAX_PMU_LEN]; \
    pmuStringFromUri(_uri, _var);

/**
 * Get a slcName from the provided uri and store it in the given variable name
 * A slcName is the URI with the last ':' converted to a dot
 *
 * @param _uri the uri to pull the slcName from
 * @param _var the name of the variable to store the resulting slcName in
 */
#define TO_SLC_NAME(_uri, _var)\
    char _var[MAX_URI_LEN]; \
    uriToSlcName(_var, _uri);

/**
 * Get a legacy AIDA name from the provided uri and store it in the given variable name
 *
 * @param _uri the uri to pull the slcName from
 * @param _var the name of the variable to store the resulting slcName in
 */
#define TO_LEGACY_NAME(_uri, _var)\
    char _var[MAX_URI_LEN]; \
    uriLegacyName(_var, _uri);

/**
 * Get a display group name from the provided uri and store it in the given variable name
 * A display group name  is the the part of the URI before the last ':'
 *
 * @param _uri the uri to pull the dgroup from
 * @param _var the name of the variable to store the resulting dgroup in
 */
#define TO_DGROUP(_uri, _var) \
    char _var[MAX_URI_LEN]; \
    groupNameFromUri(_uri, _var);

#define REQUEST_STUB_ARRAY(_api) \
Array _api(JNIEnv* env, const char* uri, Arguments arguments) \
{ \
    UNSUPPORTED_ARRAY_REQUEST \
}

#define REQUEST_STUB_BOOLEAN \
int aidaRequestBoolean(JNIEnv* env, const char* uri, Arguments arguments) \
{ \
    aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri); \
    return 0; \
}

#define REQUEST_STUB_BYTE \
char aidaRequestByte(JNIEnv* env, const char* uri, Arguments arguments) \
{ \
    aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri); \
    return 0x0; \
}

#define REQUEST_STUB_SHORT \
short aidaRequestShort(JNIEnv* env, const char* uri, Arguments arguments) \
{ \
    aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri); \
    return 0; \
}

#define REQUEST_STUB_INTEGER \
int aidaRequestInteger(JNIEnv* env, const char* uri, Arguments arguments) \
{ \
    aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri); \
    return 0; \
}

#define REQUEST_STUB_LONG \
long aidaRequestLong(JNIEnv* env, const char* uri, Arguments arguments) \
{ \
    aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri); \
    return 0; \
}

#define REQUEST_STUB_FLOAT \
float aidaRequestFloat(JNIEnv* env, const char* uri, Arguments arguments) \
{ \
    aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri); \
    return 0.0f; \
}

#define REQUEST_STUB_DOUBLE \
double aidaRequestDouble(JNIEnv* env, const char* uri, Arguments arguments) \
{ \
    aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri); \
    return 0.0; \
}

#define REQUEST_STUB_STRING \
char* aidaRequestString(JNIEnv* env, const char* uri, Arguments arguments) \
{ \
    aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri); \
    return NULL; \
}

#define REQUEST_STUB_BOOLEAN_ARRAY REQUEST_STUB_ARRAY(aidaRequestBooleanArray)

#define REQUEST_STUB_BYTE_ARRAY REQUEST_STUB_ARRAY(aidaRequestByteArray)

#define REQUEST_STUB_SHORT_ARRAY REQUEST_STUB_ARRAY(aidaRequestShortArray)

#define REQUEST_STUB_INTEGER_ARRAY REQUEST_STUB_ARRAY(aidaRequestIntegerArray)

#define REQUEST_STUB_LONG_ARRAY REQUEST_STUB_ARRAY(aidaRequestLongArray)

#define REQUEST_STUB_FLOAT_ARRAY REQUEST_STUB_ARRAY(aidaRequestFloatArray)

#define REQUEST_STUB_DOUBLE_ARRAY REQUEST_STUB_ARRAY(aidaRequestDoubleArray)

#define REQUEST_STUB_STRING_ARRAY \
StringArray aidaRequestStringArray(JNIEnv* env, const char* uri, Arguments arguments) \
{ \
    UNSUPPORTED_STRING_ARRAY_REQUEST \
}

#define REQUEST_STUB_TABLE \
Table aidaRequestTable(JNIEnv* env, const char* uri, Arguments arguments) \
{ \
    UNSUPPORTED_TABLE_REQUEST \
}

#define SET_STUB_VOID \
void aidaSetValue(JNIEnv* env, const char* uri, Arguments arguments, Value value) \
{ \
    aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri); \
}

#define SET_STUB_TABLE \
Table aidaSetValueWithResponse(JNIEnv* env, const char* uri, Arguments arguments, Value value) \
{ \
    UNSUPPORTED_TABLE_REQUEST \
}

/**
 * Initialise the aida service.  Called once by the framework when starting up.
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 */
void aidaServiceInit(JNIEnv* env);

/**
 * Get a table of data
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the table
 */
Table aidaRequestTable(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a boolean
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean
 */
int aidaRequestBoolean(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a byte
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the byte
 */
char aidaRequestByte(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a short
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the short
 */
short aidaRequestShort(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a integer
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the integer
 */
int aidaRequestInteger(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a long
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the long
 */
long aidaRequestLong(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a float
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the float
 */
float aidaRequestFloat(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a double
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the double
 */
double aidaRequestDouble(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a string
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the string
 */
char* aidaRequestString(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a boolean array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean array
 */
Array aidaRequestBooleanArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a byte array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the byte array
 */
Array aidaRequestByteArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a short array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the short array
 */
Array aidaRequestShortArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a integer array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the integer array
 */
Array aidaRequestIntegerArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a long array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the long array
 */
Array aidaRequestLongArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a float array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the float array
 */
Array aidaRequestFloatArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a double array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the double array
 */
Array aidaRequestDoubleArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a string array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the string array
 */
StringArray aidaRequestStringArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Set a value
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @param value to set
 * @return the table
 */
void aidaSetValue(JNIEnv* env, const char* uri, Arguments arguments, Value value);

/**
 * Set a value and return a table as a response
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @param value to set
 * @return a table
 */
Table aidaSetValueWithResponse(JNIEnv* env, const char* uri, Arguments arguments, Value value);

/**
 * To log any exceptions and throw back to java
 *
 * The exception is formatted in a standard way using the VMS status code and its associated message
 * and the optionally supplied message
 * The exception is always assumed to be from the edu.stanford.slac.except package
 *
 * @param env
 * @param status
 * @param exception
 * @param message
 */
void aidaThrow(JNIEnv* env, vmsstat_t status, char* exception, const char* message);

/**
 * To log any non-OS exceptions and throw back to java
 *
 * The exception is formatted in a standard way with the optionally supplied message
 * The exception is always assumed to be from the edu.stanford.slac.except package
 *
 * @param env
 * @param exception
 * @param message
 */
void aidaThrowNonOsException(JNIEnv* env, char* exception, const char* message);

/**
 * Check if a string ends with another string
 * @param str
 * @param suffix
 * @return true if string ends with suffix
 */
int endsWith(const char* str, char* suffix);

/**
 * Check if a string starts with another string
 * @param str
 * @param prefix
 * @returns true if string starts with prefix
 */
int startsWith(const char* str, char* prefix);

/**
 * Get a named argument
 * @param arguments arguments
 * @param name name
 * @return Argument
 */
Argument getArgument(Arguments arguments, char* name);

/**
 * Get boolean argument
 * @param argument argument
 * @return boolean
 */
int getBooleanArgument(Argument argument);

/**
 * Get byte argument
 * @param argument argument
 * @return byte
 */
char getByteArgument(Argument argument);

/**
 * Get short argument
 * @param argument argument
 * @return short
 */
short getShortArgument(Argument argument);

/**
 * Get integer argument
 * @param argument argument
 * @return int
 */
int getIntegerArgument(Argument argument);

/**
 * Get long argument
 * @param argument argument
 * @return long
 */
long getLongArgument(Argument argument);

/**
 * Get float argument
 * @param argument argument
 * @return float
 */
float getFloatArgument(Argument argument);

/**
 * Get double argument
 * @param argument argument
 * @return double
 */
double getDoubleArgument(Argument argument);

/**
 * Print a value to standard output
 * @param env
 * @param value
 */
void printValue(Value value);

/**
 * Get the json value from the given value identified by the path
 *
 * @param value the given value
 * @param path is an absolute reference to the element within the json of the given value. e.g. root.collection[0].name
 * @return the json_value
 */
json_value* getJsonValue(Value* value, char* path);

/**
 * Get value from a named  argument in the provided arguments structure.
 *
 * @param env env
 * @param arguments provided arguments structure
 * @param name provided name
 * @return the extracted Value
 */
Value getNamedValue(JNIEnv* env, Arguments arguments, char* name);

/**
 * Get array value from a named  argument in the provided arguments structure.
 *
 * @param env env
 * @param arguments provided arguments structure
 * @param name provided name
 * @return the extracted Value
 */
Value getNamedArrayValue(JNIEnv* env, Arguments arguments, char* name);

/**
 * 	Skip root element if it is _array otherwise return unchanged
 *
 * 	This is because our json parser can't process arrays at the top level and so we insert
 * 	an object at the top level with an "_array" element if we find an array at the top level
 *
 * @param jsonValue
 * @return
 */
json_value* getJsonRoot(json_value* jsonValue);

/**
 * Get the Display group name from a URI
 * @param uri
 * @param groupName
 * @return
 */
int groupNameFromUri(const char* uri, char groupName[]);

/**
 * Get secondary number from URI
 * @param uri the uri
 * @param secn pointer to an int to store the secondary as a number
 */
void secnFromUri(const char* uri, int4u* secn);

/**
 * Get secondary from URI.  Just points into the URI so don't go messing with it
 * @param uri the uri
 * @param secn pointer to an int to store the secondary as a number
 */
const char* secondaryFromUri(const char* uri);

/**
 * Get primary, micro and unit from a device name
 *
 * @param device
 * @param primary
 * @param micro
 * @param unit
 */
void pmuFromDeviceName(char* device, char* primary, char* micro, int4u* unit);

/**
 * Get the pmu part of a URI
 * @param uri the uri
 * @param pmuString the pre-allocated space to store the pmu string
 */
void pmuStringFromUri(const char* uri, char* pmuString);

/**
 * Convert all URIs to slac names before making queries
 * @param slcName
 * @param uri
 * @return
 */
void uriToSlcName(char slcName[MAX_URI_LEN], const char* uri);

/**
 * Convert the given URI to the legacy AIDA name for low level functions that still require it that way
 *
 * @param legacyName
 * @param uri
 * @return
 */
void uriLegacyName(char legacyName[MAX_URI_LEN], const char* uri);

void* allocateMemory(JNIEnv* env, void* source, size_t size, bool nullTerminate, char* message);

#ifdef __cplusplus
}
#endif
#endif

