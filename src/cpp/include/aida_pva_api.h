/** @file
 *  @brief The Header File for the Provider API and Stubs MACROS.
 *
 */
#ifndef _Included_aida_pva_api
#define _Included_aida_pva_api
#ifdef __cplusplus
extern "C" {
#endif

#include <jni.h>
#include <descrip.h>              /* for definition of $DESCRIPTOR  */
#include <stdbool.h>
#include <stsdef.h>               /* Macros for handling VMS status */

#include "slc_macros.h"
#include "msg_proto.h"            /* for standalone_init */
#include "errtranslate.h"
#include "aida_pva_json.h"
#include "ref.h"                  /* passing by reference macros */
#include "process_parm.h"

#include "aida_pva.h"

/**
 * Initialise the aida service.  Called once by the framework when starting up.
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 */
void aidaServiceInit(JNIEnv* env);

/**
 * Get a table of data.
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the table
 */
Table aidaRequestTable(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a boolean.
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean
 */
int aidaRequestBoolean(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a byte.
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
 *
 *  **CMS**=C_INC
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
 * Return an empty table response.   Use this if you're implementing an api that returns
 * a Table and you've encountered an error, just after you've raised an exception.
 */
#define RETURN_NULL_TABLE \
    Table nullTable; \
    nullTable.columnCount = 0; \
    return nullTable;

#ifdef __cplusplus
}
#endif
#endif

