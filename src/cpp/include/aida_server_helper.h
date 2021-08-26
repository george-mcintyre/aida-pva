#ifndef _Included_aida_server_helper
#define _Included_aida_server_helper
#ifdef __cplusplus
extern "C" {
#endif

#include <jni.h>

#include "aida_types.h"

// Supported exceptions
#define SERVER_INITIALISATION_EXCEPTION "ServerInitialisationException"
#define UNABLE_TO_GET_DATA_EXCEPTION "UnableToGetDataException"

/**
 * To log any exceptions and throw back to java
 *
 * The exception is formatted in a standard way using the VMS status code and its associated message
 * and the optionally supplied message
 * The exception is always assumed to be from the edu.stanford.slac.aida.exception package
 *
 * @param env
 * @param status
 * @param exception
 * @param message
 */
void throw(JNIEnv* env, int4u status, char* exception, const char* message);

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

#ifdef __cplusplus
}
#endif
#endif

