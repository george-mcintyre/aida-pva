#ifndef _Included_aida_server_helper
#define _Included_aida_server_helper
#ifdef __cplusplus
extern "C" {
#endif

#include <jni.h>

#include "slc_macros.h"
#include "errtranslate.h"
#include "aida_types.h"

/**
 * Issues a given error message to SLC error log, which is passed on to cmlog.
 * @param message
 */
void issue_err( char* message );

// Override prototypes of externals to uppercase names, since compile.com
// adds cc/names=UPPERCASE on compiles by default, but if the ATTRIBUTE=JNI
// is in effect (as is for this module), then it's /names=AS_IS.

void ERRTRANSLATE(const unsigned long int* errcode_p, struct dsc$descriptor* msgdsc_ps);

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
void aidaThrow(JNIEnv* env, int4u status, char* exception, const char* message);

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

