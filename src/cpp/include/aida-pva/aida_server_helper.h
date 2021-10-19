/** @file */
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
 * Create tracking variables so that memory can be freed with FREE_MEMORY macro.
 * Creates up to MAX_POINTERS pointers to track all memory allocations so that
 * they can be safely freed, when needed.
 * Creates local variables to store the tracking information so these macros can only
 * be used within a single block.
 */
#define TRACK_ALLOCATED_MEMORY \
    int _nAllocationsToFree = 0, _n_jsonValuesToFree = 0; \
    void *_memoryAllocationsToFree[MAX_POINTERS] ; \
    json_value *_jsonValuesToFree[MAX_POINTERS] ;

/**
 * Register this newly allocated memory so that it will be freed by FREE_MEMORY.
 */
#define TRACK_MEMORY(_ptr) \
    if (_ptr) _memoryAllocationsToFree[_nAllocationsToFree++] = (_ptr);

/**
 * Register this newly allocated json value so that it will be freed by FREE_JSON_MEMORY.
 */
#define TRACK_JSON(_ptr) \
    if (_ptr) _jsonValuesToFree[_n_jsonValuesToFree++] = (_ptr);

/**
 * Allocate memory.  Allocates memory of the given size
 *
 * @param env      The JNI environment.  Used in all functions involving JNI
 * @param _size    size of memory to allocate
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 */
#define ALLOCATE_MEMORY(_env, _size, _purpose) allocateMemory(_env, NULL, _size, false, "Could not allocate space for " _purpose)

/**
 * Allocate memory and set its contents to the given buffer of given size
 *
 * @param env      The JNI environment.  Used in all functions involving JNI
 * @param _source buffer to copy contents from
 * @param _size size of memory to allocate
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 */
#define ALLOCATE_AND_COPY_MEMORY(_env, _source, _size, _purpose) allocateMemory(_env, _source, _size, false, "Could not allocate space for " _purpose)

/**
 * Allocate memory for a string and copy the given string into this allocated space
 *
 * @param env      The JNI environment.  Used in all functions involving JNI
 * @param _string buffer to copy contents from
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 */
#define ALLOCATE_STRING(_env, _string, _purpose) ALLOCATE_AND_COPY_MEMORY(_env, _string, strlen(_string)+1, _purpose)

/**
 * Allocate space for a fixed length string and copy date from the given string into
 * the newly allocated space.  You need to specify size as one bigger than the
 * fixed length string so that it can be null terminated
 *
 * @param env      The JNI environment.  Used in all functions involving JNI
 * @param _string buffer to copy contents from
 * @param _size size of memory to allocate
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 */
#define ALLOCATE_FIXED_LENGTH_STRING(_env, _string, _size, _purpose) allocateMemory(_env, _string, _size, true, "Could not allocate space for " _purpose)

/**
 * Allocate memory.
 *
 * @param env      The JNI environment.  Used in all functions involving JNI
 * @param _var the specified variable is set to point to the allocated memory
 * @param _size size of memory to allocate
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 * @param _r the specified return value
 * @return This MACRO will return the specified return value from your function if it fails
 */
#define ALLOCATE_MEMORY_AND_ON_ERROR_RETURN(_env, _var, _size, _purpose, _r) { \
    void * _aptr = allocateMemory(_env, NULL, _size, false, "Could not allocate space for " _purpose); \
    if (!_aptr) \
        return _r; \
    (_var) = _aptr; \
}

/**
 * Allocate memory for a string and copy the given string into this allocated space
 *
 * @param env      The JNI environment.  Used in all functions involving JNI
 * @param _var the specified variable is set to point to the allocated memory
 * @param _string buffer to copy contents from
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 * @param _r the specified return value
 * @return This MACRO will return the specified return value from your function if it fails
 */
#define ALLOCATE_STRING_AND_ON_ERROR_RETURN_(_env, _var, _string, _purpose, _r) \
if (!( (_var) = ALLOCATE_STRING(_env, _string, _purpose))) { \
    return _r; \
}

/**
 * Allocate memory for a string and copy the given string into this allocated space
 * The specified variable is set to point to the allocated memory
 * The given purpose is a string that will be contained in the error message if the allocation fails
 *
 * @param env      The JNI environment.  Used in all functions involving JNI
 * @param _var the specified variable is set to point to the allocated memory
 * @param _string buffer to copy contents from
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 * @return This MACRO will return from your function if it fails
 */
#define ALLOCATE_STRING_AND_ON_ERROR_RETURN_VOID(_env, _var, _string, _purpose) \
if (!( (_var) = ALLOCATE_STRING(_env, _string, _purpose))) { \
    return; \
}

/**
 * Allocate space for a fixed length string and copy date from the given string into
 * the newly allocated space.  You need to specify size as one bigger than the
 * fixed length string so that it can be null terminated
 *
 * @param env      The JNI environment.  Used in all functions involving JNI
 * @param _var the specified variable is set to point to the allocated memory
 * @param _string buffer to copy contents from
 * @param _size size of memory to allocate
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 * @return This MACRO will return from your function if it fails
 */
#define ALLOCATE_FIXED_LENGTH_STRING_AND_ON_ERROR_RETURN_VOID(_env, _var, _string, _size, _purpose) \
if (!( (_var) = ALLOCATE_FIXED_LENGTH_STRING(_env, _string, _size, _purpose))) { \
    return; \
}

/**
 * Allocate space for a fixed length string and copy date from the given string into
 * the newly allocated space.  You need to specify size as one bigger than the
 * fixed length string so that it can be null terminated
 *
 * @param env      The JNI environment.  Used in all functions involving JNI
 * @param _var the specified variable is set to point to the allocated memory
 * @param _string buffer to copy contents from
 * @param _size size of memory to allocate
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 * @param _r the specified return value
 * @return This MACRO will return the specified return value from your function if it fails
 */
#define ALLOCATE_AND_TRACK_FIXED_LENGTH_STRING_AND_ON_ERROR_RETURN_(_env, _var, _string, _size, _purpose, _r) \
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
 * Allocate memory and add it to the tracked memory list so that it can be freed automatically later
 *
 * @param env      The JNI environment.  Used in all functions involving JNI
 * @param _var the specified variable is set to point to the allocated memory
 * @param _size size of memory to allocate
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 * @param _r the specified return value
 * @return This MACRO will return the specified return value from your function if it fails
 */
#define ALLOCATE_AND_TRACK_MEMORY_AND_ON_ERROR_RETURN_(_env, _var, _size, _purpose, _r) \
{ \
    void *_aptr = ALLOCATE_MEMORY(_env, _size, _purpose); \
    if ( !_aptr ) { \
        FREE_MEMORY \
        return _r; \
    } \
    TRACK_MEMORY(_aptr) \
    (_var) = _aptr; \
}

/**
 * Allocate memory and set its contents to the given buffer of given size
 *
 * @param env      The JNI environment.  Used in all functions involving JNI
 * @param _var the specified variable is set to point to the allocated memory
 * @param _source buffer to copy contents from
 * @param _size size of memory to allocate
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 * @param _r the specified return value
 * @return This MACRO will return the specified return value from your function if it fails
 */
#define ALLOCATE_COPY_AND_TRACK_MEMORY_AND_ON_ERROR_RETURN_(_env, _var, _source, _size, _purpose, _r) \
{ \
    void *_aptr = ALLOCATE_AND_COPY_MEMORY(_env, _source, _size, _purpose); \
    if ( !_aptr ) { \
        FREE_MEMORY \
        return _r; \
    } \
    TRACK_MEMORY(_aptr) \
    (_var) = _aptr; \
}

/**
 * Allocate and track a string
 *
 * @param env      The JNI environment.  Used in all functions involving JNI
 * @param _var the specified variable is set to point to the allocated memory
 * @param _string buffer to copy contents from
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 * @param _r the specified return value
 * @return This MACRO will return the specified return value from your function if it fails
 */
#define ALLOCATE_COPY_AND_TRACK_STRING_AND_ON_ERROR_RETURN_(_env, _var, _string, _purpose, _r) \
{ \
    void *_aptr = ALLOCATE_STRING(_env, _string, _purpose); \
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
    while ( _n_jsonValuesToFree-- > 0) { \
        if ( _jsonValuesToFree[_n_jsonValuesToFree] ) \
            json_value_free(_jsonValuesToFree[_n_jsonValuesToFree]); \
    } \
}

/**
 * Free any allocated memory
 */
#define FREE_MEMORY \
{                              \
    while ( _nAllocationsToFree-- > 0) { \
        if ( _memoryAllocationsToFree[_nAllocationsToFree])  \
            free (_memoryAllocationsToFree[_nAllocationsToFree]); \
    } \
    FREE_JSON \
}

/**
 * Free a single tracked memory allocation and remove from list
 *
 * @param _ptr name of a pointer that points to the memory to free
 */
#define FREE_TRACKED_MEMORY(_ptr) \
{ \
    if ( _ptr) {  \
    \
        bool found = false; \
        for ( int i = 0 ; i < _nAllocationsToFree; i++ ) { \
            if ( (_ptr) == _memoryAllocationsToFree[i] ) {   \
                free (_ptr);                    \
                found = true;                    \
            } \
            if ( found && i != (_nAllocationsToFree-1) )   \
                _memoryAllocationsToFree[i] = _memoryAllocationsToFree[i+1]; \
        } \
        if ( found )   \
            _nAllocationsToFree--;     \
    }\
}

/**
 * Format an error message, throw it in an exception, free any allocated memory and return the error code
 *
 * @param _exception exception to raise (string)
 * @param _errorText the text of the error to raise
 * @param _ref a string that will be substituted in message with %s
 * @param _r the specified return value
 * @return This MACRO will return the specified return value from your function
 */
#define SPRINTF_ERROR_FREE_MEMORY_AND_RETURN_(_exception, _errorText, _ref, _r) \
{ \
    char error[MAX_ERROR_TEXT_LEN + strlen(_ref)]; \
    sprintf(error, _errorText,  _ref); \
    aidaThrowNonOsException(env, _exception, error); \
    FREE_MEMORY \
    return _r; \
}

/**
 *Throw error message in an exception, free any allocated memory and return the error code
 *
 * @param _exception exception to raise (string)
 * @param _errorText the text of the error to raise
 * @param _r the specified return value
 * @return This MACRO will return the specified return value from your function
 */
#define PRINT_ERROR_FREE_MEMORY_AND_RETURN_(_exception, _errorText, _r) \
{ \
    aidaThrowNonOsException(env, _exception, _errorText); \
    FREE_MEMORY \
    return _r; \
}

/**
 * Return an empty table response.   Use this if you're implementing an api that returns
 * a Table and you've encountered an error, just after you've raised an exception.
 */
#define RETURN_NULL_TABLE \
    Table nullTable; \
    nullTable.columnCount = 0; \
    return nullTable;

/**
 * Throw unsupported channel exception and return a blank array.
 */
#define UNSUPPORTED_ARRAY_REQUEST \
    aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri); \
    Array array; \
    return array;

/**
 * Throw an unsupported channel exception and return an empty string array.
 */
#define UNSUPPORTED_STRING_ARRAY_REQUEST \
    aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri); \
    StringArray stringArray; \
    return stringArray;

/**
 * Throw an unsupported channel exception and return an empty table.
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
#define PMU_STRING_FROM_URI(_var, _uri)  \
    char _var[MAX_PMU_LEN]; \
    pmuStringFromUri(_var, _uri);

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
#define TO_DGROUP(_var, _uri) \
    char _var[MAX_URI_LEN]; \
    groupNameFromUri(_var, _uri);



/**
 * API stub for AIDA-PVA scalar requests
 *
 * @param _api the name of the API to define
 * @param _rtype the type that this API will return
 * @param _r the specified return value
 * @return This MACRO will create a function stub that will return the specified return value
 */
#define REQUEST_STUB_SCALAR(_api, _rtype, _r) \
_rtype _api(JNIEnv* env, const char* uri, Arguments arguments) \
{ \
    aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri); \
    return (_r); \
}

/**
 * aidaRequestBoolean API Stub
 */
#define REQUEST_STUB_BOOLEAN REQUEST_STUB_SCALAR(aidaRequestBoolean, int, 0)


/**
 * aidaRequestByte API Stub
 */
#define REQUEST_STUB_BYTE REQUEST_STUB_SCALAR(aidaRequestByte, char, 0x0)

/**
 * aidaRequestShort API Stub
 */
#define REQUEST_STUB_SHORT REQUEST_STUB_SCALAR(aidaRequestShort, short, 0)

/**
 * aidaRequestInteger API Stub
 */
#define REQUEST_STUB_INTEGER REQUEST_STUB_SCALAR(aidaRequestInteger, int, 0)

/**
 * aidaRequestLong API Stub
 */
#define REQUEST_STUB_LONG REQUEST_STUB_SCALAR(aidaRequestLong, long, 0)

/**
 * aidaRequestFloat API Stub
 */
#define REQUEST_STUB_FLOAT REQUEST_STUB_SCALAR(aidaRequestFloat, float, 0.0f)

/**
 * aidaRequestDouble API Stub
 */
#define REQUEST_STUB_DOUBLE REQUEST_STUB_SCALAR(aidaRequestDouble, double, 0.0)

/**
 * aidaRequestString API Stub
 */
#define REQUEST_STUB_STRING REQUEST_STUB_SCALAR(aidaRequestString, char*, NULL)

/**
 * API stub for AIDA-PVA array requests
 *
 * @param _api the name of the API to define
 * @return This MACRO will create a function stub that will return Array
 */
#define REQUEST_STUB_ARRAY(_api) \
Array _api(JNIEnv* env, const char* uri, Arguments arguments) \
{ \
    UNSUPPORTED_ARRAY_REQUEST \
}

/**
 * aidaRequestBooleanArray API Stub
 */
#define REQUEST_STUB_BOOLEAN_ARRAY REQUEST_STUB_ARRAY(aidaRequestBooleanArray)

/**
 * aidaRequestByteArray API Stub
 */
#define REQUEST_STUB_BYTE_ARRAY REQUEST_STUB_ARRAY(aidaRequestByteArray)

/**
 * aidaRequestShortArray API Stub
 */
#define REQUEST_STUB_SHORT_ARRAY REQUEST_STUB_ARRAY(aidaRequestShortArray)

/**
 * aidaRequestIntegerArray API Stub
 */
#define REQUEST_STUB_INTEGER_ARRAY REQUEST_STUB_ARRAY(aidaRequestIntegerArray)

/**
 * aidaRequestLongArray API Stub
 */
#define REQUEST_STUB_LONG_ARRAY REQUEST_STUB_ARRAY(aidaRequestLongArray)

/**
 * aidaRequestFloatArray API Stub
 */
#define REQUEST_STUB_FLOAT_ARRAY REQUEST_STUB_ARRAY(aidaRequestFloatArray)

/**
 * aidaRequestDoubleArray API Stub
 */
#define REQUEST_STUB_DOUBLE_ARRAY REQUEST_STUB_ARRAY(aidaRequestDoubleArray)


/**
 * aidaRequestStringArray API stub
 */
#define REQUEST_STUB_STRING_ARRAY \
StringArray aidaRequestStringArray(JNIEnv* env, const char* uri, Arguments arguments) \
{ \
    UNSUPPORTED_STRING_ARRAY_REQUEST \
}

/**
 * aidaRequestTable API stub
 */
#define REQUEST_STUB_TABLE \
Table aidaRequestTable(JNIEnv* env, const char* uri, Arguments arguments) \
{ \
    UNSUPPORTED_TABLE_REQUEST \
}

/**
 * aidaSetValue API stub
 */
#define SET_STUB_VOID \
void aidaSetValue(JNIEnv* env, const char* uri, Arguments arguments, Value value) \
{ \
    aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri); \
}

/**
 * aidaSetValueWithResponse API stub
 */
#define SET_STUB_TABLE \
Table aidaSetValueWithResponse(JNIEnv* env, const char* uri, Arguments arguments, Value value) \
{ \
    UNSUPPORTED_TABLE_REQUEST \
}

/**
 * Initialise the aida service.  Called once by the framework when starting up.
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 */
void aidaServiceInit(JNIEnv* env);

/**
 * Get a table of data
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the table
 */
Table aidaRequestTable(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a boolean
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean
 */
int aidaRequestBoolean(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a byte
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the byte
 */
char aidaRequestByte(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a short
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the short
 */
short aidaRequestShort(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a integer
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the integer
 */
int aidaRequestInteger(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a long
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the long
 */
long aidaRequestLong(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a float
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the float
 */
float aidaRequestFloat(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a double
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the double
 */
double aidaRequestDouble(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a string
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the string
 */
char* aidaRequestString(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a boolean array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean array
 */
Array aidaRequestBooleanArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a byte array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the byte array
 */
Array aidaRequestByteArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a short array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the short array
 */
Array aidaRequestShortArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a integer array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the integer array
 */
Array aidaRequestIntegerArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a long array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the long array
 */
Array aidaRequestLongArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a float array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the float array
 */
Array aidaRequestFloatArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a double array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the double array
 */
Array aidaRequestDoubleArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a string array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the string array
 */
StringArray aidaRequestStringArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Set a value
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @param value to set
 * @return the table
 */
void aidaSetValue(JNIEnv* env, const char* uri, Arguments arguments, Value value);

/**
 * Set a value and return a table as a response
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
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
 * @param jsonValue the json value traverse
 * @return json value
 */
json_value* getJsonRoot(json_value* jsonValue);

/**
 * Get the Display group name from a URI
 *
 * @param groupName pre-allocated space to store the group name
 * @param uri the new format AIDA PV name
 * @return EXIT_SUCCESS if all goes well EXIT_FAILURE otherwise
 */
int groupNameFromUri(char groupName[], const char* uri);

/**
 * Get secondary number from URI
 *
 * @param uri the new format AIDA PV name
 * @param secn pointer to an int to store the secondary as a number
 */
void secnFromUri(const char* uri, int4u* secn);

/**
 * Get secondary from URI.  Just points into the URI so don't go messing with it
 *
 * @param uri the new format AIDA PV name
 * @param secn pointer to an int to store the secondary as a number
 */
const char* secondaryFromUri(const char* uri);

/**
 * Get primary, micro and unit from a device name
 *
 * @param device pre-allocated space to store the device
 * @param primary pre-allocated space to store the primary
 * @param micro pre-allocated space to store the micro
 * @param unit pre-allocated space to store the unit
 */
int pmuFromDeviceName(JNIEnv* env, char* device, char* primary, char* micro, int4u* unit);

/**
 * Get the pmu part of a URI
 *
 * @param pmuString the pre-allocated space to store the pmu string
 * @param uri the new format AIDA PV name
 */
void pmuStringFromUri(char* pmuString, const char* uri);

/**
 * Convert all URIs to slac names before making queries
 *
 * @param slcName pointer to space to store the SLC name
 * @param uri the new format AIDA PV name
 */
void uriToSlcName(char slcName[MAX_URI_LEN], const char* uri);

/**
 * Convert the given URI to the legacy AIDA name for low level functions that still require it that way
 *
 * @param legacyName pointer to space to store the legacy AIDA name
 * @param uri the new format AIDA PV name
 */
void uriLegacyName(char legacyName[MAX_URI_LEN], const char* uri);

/**
 * Allocate memory and copy the source to it if specified.  If the null terminate flag is set
 * null terminate the allocate space, at the last position
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param source source of data to copy to newly allocated space, NULL to not copy
 * @param size the amount of space to allocate
 * @param nullTerminate true to null terminate
 * @param message the message to display if anything goes wrong
 * @return the allocated memory
 */
void* allocateMemory(JNIEnv* env, void* source, size_t size, bool nullTerminate, char* message);

#ifdef __cplusplus
}
#endif
#endif

