#ifndef _Included_aida_server_helper
#define _Included_aida_server_helper
#ifdef __cplusplus
extern "C" {
#endif

#include <jni.h>
#include "aida_jni_helper.h"

#include "slc_macros.h"
#include "errtranslate.h"
#include "aida_types.h"
#include "json.h"

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

#define RETURN_NULL_TABLE \
	Table nullTable; \
	return nullTable;


#define DEFAULT_CONFIG_REQUEST \
	Config config; \
	memset(&config, 0, sizeof(config)); \
	return config;

#define UNSUPPORTED_ARRAY_REQUEST \
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri); \
	Array array; \
	return array;

#define UNSUPPORTED_STRING_ARRAY_REQUEST \
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri); \
	StringArray stringArray; \
	return stringArray;

#define UNSUPPORTED_TABLE_REQUEST \
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri); \
	Table table; \
	return table;


/**
 * Initialise the aida service.  Called once by the framework when starting up.
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 */
void aidaServiceInit(JNIEnv* env);

/**
 * Get channel configuration
 * @param channelName
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @return the channel config
 */
Config aidaChannelConfig(JNIEnv* env, const char* channelName);

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
void printValue(JNIEnv* env, Value value);

/**
 * Get the json value from the given value identified by the path
 *
 * @param value the given value
 * @param path is an absolute reference to the element within the json of the given value. e.g. root.collection[0].name
 * @return the json_value
 */
json_value* getJsonValue(Value value, char* path);

/**
 * Get value from a named  argument in the provided arguments structure.
 *
 * @param env env
 * @param arguments provided arguments structure
 * @param name provided name
 * @return the extracted Value
 */
Value getNamedValue(JNIEnv* env, Arguments arguments, char* name);

char* groupNameFromUri(const char* uri);

void pmuFromUri(const char* uri, char* primary, char* micro, unsigned long* unit);

#ifdef __cplusplus
}
#endif
#endif

