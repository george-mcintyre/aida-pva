#ifndef _Included_aida_server_helper
#define _Included_aida_server_helper
#ifdef __cplusplus
extern "C" {
#endif

#include "aida_types.h"

/**
 * Get a named boolean argument
 * @param arguments arguments
 * @param name name
 * @return boolean
 */
int getBooleanArgument(Arguments arguments, char* name);

/**
 * Get a named byte argument
 * @param arguments arguments
 * @param name name
 * @return byte
 */
unsigned char getByteArgument(Arguments arguments, char* name);

/**
 * Get a named short argument
 * @param arguments arguments
 * @param name name
 * @return short
 */
short getShortArgument(Arguments arguments, char* name);

/**
 * Get a named integer argument
 * @param arguments arguments
 * @param name name
 * @return int
 */
int getIntegerArgument(Arguments arguments, char* name);

/**
 * Get a named long argument
 * @param arguments arguments
 * @param name name
 * @return long
 */
long getLongArgument(Arguments arguments, char* name);

/**
 * Get a named float argument
 * @param arguments arguments
 * @param name name
 * @return float
 */
float getFloatArgument(Arguments arguments, char* name);

/**
 * Get a named double argument
 * @param arguments arguments
 * @param name name
 * @return double
 */
double getDoubleArgument(Arguments arguments, char* name);

/**
 * Get a named string argument
 * @param arguments arguments
 * @param name name
 * @return string
 */
const char* getStringArgument(Arguments arguments, char* name);

#ifdef __cplusplus
}
#endif
#endif

