/** @file
 *  @brief The Header File for the Native Channel Provider AIDA-PVA type related functions.
 *
 *  **CMS**=C_INC
 */

#ifndef _Included_aida_types_helper
#define _Included_aida_types_helper
#ifdef __cplusplus
extern "C" {
#endif

#include "aida_pva.h"

/**
 * Minimum number of floating point allocations.  When allocating space for floating point numbers the framework allocates in batches and then reallocates if the batch is full.  This is the minimum batch size.
 */
#define MIN_FLOAT_ALLOCATIONS 100

/**
 * int format definition character for ascanf() and avscanf().
 */
#define FORMAT_INTEGER 'd'
/**
 * unsigned int format definition character for ascanf() and avscanf().
 */
#define FORMAT_UNSIGNED_INTEGER 'u'
/**
 * float format definition character for ascanf() and avscanf().
 */
#define FORMAT_FLOAT 'f'
/**
 * string format definition character for ascanf() and avscanf().
 */
#define FORMAT_STRING 's'
/**
 * byte format definition character for ascanf() and avscanf().
 */
#define FORMAT_BYTE 'c'
/**
 * boolean format definition character for ascanf() and avscanf().
 */
#define FORMAT_BOOLEAN 'b'

/**
 * optional format definition character for ascanf() and avscanf().
 */
#define FORMAT_OPTIONAL_FLAG 'o'

/**
 * short format definition character for ascanf() and avscanf().
 */
#define FORMAT_PREFIX_SHORT 'h'
/**
 * long format definition character for ascanf() and avscanf().
 */
#define FORMAT_PREFIX_LONG 'l'

/**
 * array format definition character for ascanf() and avscanf().
 */
#define FORMAT_SUFFIX_ARRAY 'a'

/**
 * Maximum number of permitted format specifiers for ascanf() and avscanf().
 */
#define MAX_FORMAT_SPECIFIERS 20
/**
 * Maximum length of s single format specifier for ascanf() and avscanf().
 */
#define MAX_FORMAT 8
/**
 * Used internally to formulate a correctly cast pointer to the array being constructed.
 */
#define ARRAY_TARGET(_cType) &((_cType *)(*arrayPtr))[i]
/**
 * Macro used internally to set a scalar value for ascanf() and avscanf().
 */
#define ASCANF_SET_SCALAR(_format, _cType, _jsonType, _typeName, _target) \
{ \
    _cType* ptr = (_cType*)(_target); \
    if (!valueShouldBeJson) { \
        if ( sscanf(stringValue, _format, ptr) == 0 ) { \
            SPRINTF_ERROR_FREE_MEMORY_AND_RETURN_(AIDA_INTERNAL_EXCEPTION, "can't convert argument \"%s\" to " _typeName, stringValue, EXIT_FAILURE) \
        }\
    } else {  \
        if (jsonRoot->type == json_integer) { \
            *ptr = (_cType)jsonRoot->u.integer; \
        } else if (jsonRoot->type == json_double) { \
            *ptr = (_cType)(jsonRoot->u.dbl); \
        } else if (jsonRoot->type == json_string) { \
            if ( sscanf(jsonRoot->u.string.ptr, _format, ptr) == 0 ) {    \
                SPRINTF_ERROR_FREE_MEMORY_AND_RETURN_(AIDA_INTERNAL_EXCEPTION, "can't convert argument \"%s\" to " _typeName, jsonRoot->u.string.ptr, EXIT_FAILURE) \
            } \
        } else { \
            PRINT_ERROR_FREE_MEMORY_AND_RETURN_(AIDA_INTERNAL_EXCEPTION, "can't convert argument to " _typeName ": <json>", EXIT_FAILURE) \
        }\
    }\
}

/**
 * Macro used internally to set a scalar boolean for ascanf() and avscanf().
 */
#define ASCANF_SET_BOOLEAN(_targetBoolean) \
{ \
    unsigned char* ptr = (unsigned char*)(_targetBoolean); \
    if (!valueShouldBeJson) {  \
        if (isdigit(*stringValue)) {\
            double number; \
            sscanf(stringValue, "%lg", &number); \
            *ptr = (unsigned char)((number == 0.0) ? 0 : 1); \
        } else  { \
            int _val = getBooleanValue(stringValue); \
            if ( _val == -1 ) { \
                SPRINTF_ERROR_FREE_MEMORY_AND_RETURN_(AIDA_INTERNAL_EXCEPTION, "can't convert argument to boolean: %s", stringValue, EXIT_FAILURE) \
            } else { \
                *ptr = _val;  \
            } \
        } \
    } else {   \
        if (jsonRoot->type == json_integer) { \
            *ptr = (unsigned char)(jsonRoot->u.integer != 0); \
        } else if (jsonRoot->type == json_double) { \
            *ptr = (unsigned char)(jsonRoot->u.dbl != 0.0); \
        } else if (jsonRoot->type == json_boolean) { \
            *ptr = (unsigned char)jsonRoot->u.boolean; \
        } else if (jsonRoot->type == json_string) { \
            int _val = getBooleanValue(jsonRoot->u.string.ptr); \
            if ( _val == -1 ) { \
                SPRINTF_ERROR_FREE_MEMORY_AND_RETURN_(AIDA_INTERNAL_EXCEPTION, "can't convert argument to boolean: %s", jsonRoot->u.string.ptr, EXIT_FAILURE) \
            } else { \
                *ptr = _val;  \
            } \
        } else {  \
            PRINT_ERROR_FREE_MEMORY_AND_RETURN_(AIDA_INTERNAL_EXCEPTION, "can't convert argument to boolean: <json>", EXIT_FAILURE) \
        } \
    } \
}

/**
 * Macro used internally to set a scalar byte for ascanf() and avscanf().
 */
#define ASCANF_SET_BYTE(_targetByte) \
{ \
    unsigned char* ptr = (unsigned char*)(_targetByte); \
    if (!valueShouldBeJson) { \
        *ptr = *stringValue; \
    } else {   \
        if (jsonRoot->type == json_integer) { \
            *ptr = (int)(jsonRoot->u.integer & 0XFF) ; \
        } else if (jsonRoot->type == json_string && jsonRoot->u.string.length == 1) { \
            *ptr = *jsonRoot->u.string.ptr; \
        } else { \
            PRINT_ERROR_FREE_MEMORY_AND_RETURN_(AIDA_INTERNAL_EXCEPTION, "can't convert argument to byte: <json>", EXIT_FAILURE) \
        } \
    } \
}

/**
 * Macro used internally to set a string for ascanf() and avscanf().
 */
#define ASCANF_SET_STRING(_targetString) \
{ \
    char** ptr = (char**)(_targetString); \
    if (!valueShouldBeJson) { \
        *ptr = stringValue; \
    } else {  \
        if (aidaType == AIDA_STRING_TYPE) { \
            ALLOCATE_AND_TRACK_MEMORY_AND_ON_ERROR_RETURN_(env, nextStringPosition, jsonRoot->u.string.length+1, "string arguments", EXIT_FAILURE) \
        }\
        if (jsonRoot->type == json_string) { \
            strcpy(nextStringPosition, jsonRoot->u.string.ptr); \
        } else if (jsonRoot->type == json_integer) {                \
            sprintf(nextStringPosition, "%ld", jsonRoot->u.integer); \
        } else if (jsonRoot->type == json_double) {                 \
            sprintf(nextStringPosition, "%g", jsonRoot->u.dbl); \
        } else { \
            if (aidaType == AIDA_STRING_TYPE) { \
                free(nextStringPosition);\
            }\
            PRINT_ERROR_FREE_MEMORY_AND_RETURN_(AIDA_INTERNAL_EXCEPTION, "can't convert argument to string: <json>", EXIT_FAILURE) \
        } \
        *ptr = nextStringPosition; \
        nextStringPosition+=strlen(nextStringPosition)+1; \
    } \
}

/**
 * Macro used internally to set an array for ascanf() and avscanf().
 */
#define ASCANF_SET_ARRAY(_format, _cType, _jsonType, _typeName) \
{ \
    ALLOCATE_AND_TRACK_MEMORY_AND_ON_ERROR_RETURN_(env, *arrayPtr, arrayCount * sizeof(_cType), "array arguments", EXIT_FAILURE) \
    for (int i = 0; i < arrayCount; i++) { \
        jsonRoot = arrayRoot->u.array.values[i]; \
        ASCANF_SET_SCALAR(_format, _cType, _jsonType, _typeName, ARRAY_TARGET(_cType)) \
    } \
}

/**
 * Macro used internally for setting a boolean array or a byte array for ascanf() and avscanf().
 */
#define ASCANF_SET_BOOLEAN_OR_BYTE_ARRAY(_cType, _setMacro) \
{ \
    ALLOCATE_AND_TRACK_MEMORY_AND_ON_ERROR_RETURN_(env, *arrayPtr, arrayCount * sizeof(_cType), "array arguments", EXIT_FAILURE) \
    for (int i = 0; i < arrayCount; i++) { \
        jsonRoot = arrayRoot->u.array.values[i]; \
        _setMacro(ARRAY_TARGET(_cType)) \
    } \
}

/**
 * Macro used to internally for setting a boolean array for ascanf() and avscanf().
 */
#define ASCANF_SET_BOOLEAN_ARRAY ASCANF_SET_BOOLEAN_OR_BYTE_ARRAY(unsigned char, ASCANF_SET_BOOLEAN)

/**
 * Macro used internally for setting a byte array for ascanf() and avscanf().
 */
#define ASCANF_SET_BYTE_ARRAY ASCANF_SET_BOOLEAN_OR_BYTE_ARRAY(unsigned char, ASCANF_SET_BYTE)

/**
 * Macro used internally to set a string array for ascanf() and avscanf().
 */
#define ASCANF_SET_STRING_ARRAY \
{ \
    size_t pointerSpace = arrayCount * sizeof(char*); \
    ALLOCATE_AND_TRACK_MEMORY_AND_ON_ERROR_RETURN_(env, *arrayPtr, pointerSpace + totalStingLengthOf(arrayRoot) + arrayCount + 1, "string array arguments", EXIT_FAILURE) \
    nextStringPosition = ((char*)*arrayPtr) + pointerSpace; \
    for (int i = 0; i < arrayCount; i++) { \
        jsonRoot = arrayRoot->u.array.values[i]; \
        ASCANF_SET_STRING(ARRAY_TARGET(char *)) \
    } \
}
#ifdef __cplusplus
}
#endif
#endif

