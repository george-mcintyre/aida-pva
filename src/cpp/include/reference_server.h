#ifndef _Included_reference_server
#define _Included_reference_server
#ifdef __cplusplus
extern "C" {
#endif

#include "aida_types.h"

/**
 * Get channel configuration
 * @param channelName
 * @return the channel config
 */
Config aidaChannelConfig(const char* channelName);

/**
 * Get a table of data
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the table
 */
Table aidaRequestTable(const char* uri, Arguments arguments);

/**
 * Get a boolean
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean
 */
int aidaRequestBoolean(const char* uri, Arguments arguments);

/**
 * Get a byte
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the byte
 */
char aidaRequestByte(const char* uri, Arguments arguments);

/**
 * Get a short
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the short
 */
short aidaRequestShort(const char* uri, Arguments arguments);

/**
 * Get a integer
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the integer
 */
int aidaRequestInteger(const char* uri, Arguments arguments);

/**
 * Get a long
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the long
 */
long aidaRequestLong(const char* uri, Arguments arguments);

/**
 * Get a float
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the float
 */
float aidaRequestFloat(const char* uri, Arguments arguments);

/**
 * Get a double
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the double
 */
double aidaRequestDouble(const char* uri, Arguments arguments);

/**
 * Get a string
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the string
 */
char* aidaRequestString(const char* uri, Arguments arguments);

/**
 * Get a boolean array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean array
 */
Array aidaRequestBooleanArray(const char* uri, Arguments arguments);

/**
 * Get a byte array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the byte array
 */
Array aidaRequestByteArray(const char* uri, Arguments arguments);

/**
 * Get a short array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the short array
 */
Array aidaRequestShortArray(const char* uri, Arguments arguments);

/**
 * Get a integer array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the integer array
 */
Array aidaRequestIntegerArray(const char* uri, Arguments arguments);

/**
 * Get a long array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the long array
 */
Array aidaRequestLongArray(const char* uri, Arguments arguments);

/**
 * Get a float array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the float array
 */
Array aidaRequestFloatArray(const char* uri, Arguments arguments);

/**
 * Get a double array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the double array
 */
Array aidaRequestDoubleArray(const char* uri, Arguments arguments);

/**
 * Get a string array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the string array
 */
StringArray aidaRequestStringArray(const char* uri, Arguments arguments);

#ifdef __cplusplus
}
#endif
#endif
