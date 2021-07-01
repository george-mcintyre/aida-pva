#ifndef _Included_aida_server_helper
#define _Included_aida_server_helper
#ifdef __cplusplus
extern "C" {
#endif

#include "aida_types.h"

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

