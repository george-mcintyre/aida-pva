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
Table aidaRequestTable(char* uri, Arguments arguments);

/**
 * Get a boolean
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean
 */
int aidaRequestBoolean(char* uri, Arguments arguments);

/**
 * Get a byte
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the byte
 */
unsigned char aidaRequestByte(char* uri, Arguments arguments);

/**
 * Get a short
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the short
 */
short aidaRequestShort(char* uri, Arguments arguments);

/**
 * Get a integer
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the integer
 */
int aidaRequestInteger(char* uri, Arguments arguments);

/**
 * Get a long
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the long
 */
long aidaRequestLong(char* uri, Arguments arguments);

/**
 * Get a float
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the float
 */
float aidaRequestFloat(char* uri, Arguments arguments);

/**
 * Get a double
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the double
 */
double aidaRequestDouble(char* uri, Arguments arguments);

/**
 * Get a string
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the string
 */
char* aidaRequestString(char* uri, Arguments arguments);

/**
 * Get a boolean array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean array
 */
Array aidaRequestBooleanArray(char* uri, Arguments arguments);

/**
 * Get a byte array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the byte array
 */
Array aidaRequestByteArray(char* uri, Arguments arguments);

/**
 * Get a short array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the short array
 */
Array aidaRequestShortArray(char* uri, Arguments arguments);

/**
 * Get a integer array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the integer array
 */
Array aidaRequestIntegerArray(char* uri, Arguments arguments);

/**
 * Get a long array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the long array
 */
Array aidaRequestLongArray(char* uri, Arguments arguments);

/**
 * Get a float array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the float array
 */
Array aidaRequestFloatArray(char* uri, Arguments arguments);

/**
 * Get a double array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the double array
 */
Array aidaRequestDoubleArray(char* uri, Arguments arguments);

/**
 * Get a string array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the string array
 */
StringArray aidaRequestStringArray(char* uri, Arguments arguments);

#ifdef __cplusplus
}
#endif
#endif
