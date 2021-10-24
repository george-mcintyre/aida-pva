/** @file
 *  @brief The Header File for the Native Channel Provider Server Helper functions.
 *       **CMS**=C_INC
 */
#ifndef _Included_aida_server_helper
#define _Included_aida_server_helper
#ifdef __cplusplus
extern "C" {
#endif

#include "aida_pva.h"

/**
 * Get a named argument
 * @param arguments arguments
 * @param name name
 * @return Argument
 */
Argument getArgument(Arguments arguments, char* name);

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

#ifdef __cplusplus
}
#endif
#endif

