#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>

#include "slc_macros.h"           /* vmsstat_t, int2u, int4u, etc. */
#include "sysutil_proto.h"        /* for cvt_vms_to_ieee_flt */

#include "aida_types.h"
#include "aida_server_helper.h"

#define MIN_FLOAT_ALLOCATIONS 100

/// Definitions for ascanf, and avscanf
#define FORMAT_INTEGER 'd'
#define FORMAT_UNSIGNED_INTEGER 'u'
#define FORMAT_FLOAT 'f'
#define FORMAT_STRING 's'
#define FORMAT_BYTE 'c'
#define FORMAT_BOOLEAN 'b'

#define FORMAT_OPTIONAL_FLAG 'o'

#define FORMAT_PREFIX_SHORT 'h'
#define FORMAT_PREFIX_LONG 'l'

#define FORMAT_SUFFIX_ARRAY 'a'

#define MAX_FORMAT_SPECIFIERS 20
#define    MAX_FORMAT 8

#define ARRAY_TARGET(_cType) &((_cType *)(*arrayPtr))[i]

#define ASCANF_SET_SCALAR(_format, _cType, _jsonType, _typeName, _target) \
{ \
    _cType* ptr = (_cType*)(_target); \
    if (!valueShouldBeJson) {                                    \
        sscanf(stringValue, _format, ptr);\
    } else {                                                              \
        if (jsonRoot->type == json_integer) { \
            *ptr = (_cType)jsonRoot->u.integer; \
        } else if (jsonRoot->type == json_double) { \
            *ptr = (_cType)(jsonRoot->u.dbl); \
        } else {\
            SPRINF_ERROR_AND_FREE_MEMORY(AIDA_INTERNAL_EXCEPTION, "can't convert argument to " _typeName ": %s", "<json>", EXIT_FAILURE) \
        }\
    }\
}

#define ASCANF_SET_ARRAY(_format, _cType, _jsonType, _typeName) \
{ \
    ALLOCATE_AND_TRACK_MEMORY(env, *arrayPtr, arrayCount * sizeof(_cType), "array arguments", EXIT_FAILURE) \
    for (int i = 0; i < arrayCount; i++) { \
        jsonRoot = arrayRoot->u.array.values[i]; \
        ASCANF_SET_SCALAR(_format, _cType, _jsonType, _typeName, ARRAY_TARGET(_cType)) \
    } \
}

#define ASCANF_SET_BOOLEAN_OR_BYTE_ARRAY(_cType, _setMacro) \
{ \
    ALLOCATE_AND_TRACK_MEMORY(env, *arrayPtr, arrayCount * sizeof(_cType), "array arguments", EXIT_FAILURE) \
    for (int i = 0; i < arrayCount; i++) { \
        jsonRoot = arrayRoot->u.array.values[i]; \
        _setMacro(ARRAY_TARGET(_cType)) \
    } \
}

#define ASCANF_SET_BOOLEAN(_targetBoolean) \
{ \
    unsigned char* ptr = (unsigned char*)(_targetBoolean); \
    if (!valueShouldBeJson) {  \
        if (isdigit(*stringValue)) {\
            int number; \
            sscanf(stringValue, "%d", &number); \
            *ptr = (unsigned char)number; \
        } else  { \
            *ptr = (unsigned char)!( strcasecmp(stringValue, "n") ==0  \
                    || strcasecmp(stringValue, "0") ==0  \
                    || strcasecmp(stringValue, "no") ==0  \
                    || strcasecmp(stringValue, "false") ==0  \
                    || strcasecmp(stringValue, "f")==0 );  \
        } \
    } else {   \
        if (jsonRoot->type == json_integer) { \
            *ptr = (unsigned char)(jsonRoot->u.integer != 0); \
        } else if (jsonRoot->type == json_double) { \
            *ptr = (unsigned char)(jsonRoot->u.dbl != 0.0); \
        } else if (jsonRoot->type == json_boolean) { \
            *ptr = (unsigned char)jsonRoot->u.boolean; \
        } else if (jsonRoot->type == json_string) { \
            *ptr = (unsigned char)!(strcasecmp(jsonRoot->u.string.ptr, "false") == 0 \
                    || strcasecmp(jsonRoot->u.string.ptr, "n") == 0 \
                    || strcasecmp(jsonRoot->u.string.ptr, "0") == 0 \
                    || strcasecmp(jsonRoot->u.string.ptr, "no") == 0 \
                    || strcasecmp(jsonRoot->u.string.ptr, "f") == 0); \
        } else {  \
            SPRINF_ERROR_AND_FREE_MEMORY(AIDA_INTERNAL_EXCEPTION, "can't convert argument to boolean: %s", "<json>", EXIT_FAILURE) \
        } \
    } \
}

#define ASCANF_SET_BOOLEAN_ARRAY ASCANF_SET_BOOLEAN_OR_BYTE_ARRAY(unsigned char, ASCANF_SET_BOOLEAN)

#define ASCANF_SET_BYTE(_targetByte) \
{ \
    unsigned char* ptr = (unsigned char*)(_targetByte); \
    if (!valueShouldBeJson) { \
        *ptr = *stringValue; \
    } else {   \
        if (jsonRoot->type == json_integer) { \
            *ptr = jsonRoot->u.integer; \
        } else if (jsonRoot->type == json_string && jsonRoot->u.string.length == 1) { \
            *ptr = *jsonRoot->u.string.ptr; \
        } else { \
            SPRINF_ERROR_AND_FREE_MEMORY(AIDA_INTERNAL_EXCEPTION, "can't convert argument to byte: %s", "<json>", EXIT_FAILURE) \
        } \
    } \
}

#define ASCANF_SET_BYTE_ARRAY ASCANF_SET_BOOLEAN_OR_BYTE_ARRAY(unsigned char, ASCANF_SET_BYTE)

#define ASCANF_SET_STRING(_targetString) \
{ \
    char** ptr = (char**)(_targetString); \
    if (!valueShouldBeJson) { \
        *ptr = stringValue; \
    } else {  \
        if (aidaType == AIDA_STRING_TYPE) { \
            ALLOCATE_AND_TRACK_MEMORY(env, nextStringPosition, jsonRoot->u.string.length+1, "string arguments", EXIT_FAILURE) \
        }\
        if (jsonRoot->type == json_string) { \
            strcpy(nextStringPosition, jsonRoot->u.string.ptr); \
        } else if (jsonRoot->type == json_integer) {                \
            sprintf(nextStringPosition, "%d", jsonRoot->u.integer); \
        } else if (jsonRoot->type == json_double) {                 \
            sprintf(nextStringPosition, "%g", jsonRoot->u.dbl); \
        } else { \
            if (aidaType == AIDA_STRING_TYPE) { \
                free(nextStringPosition);\
            }\
            SPRINF_ERROR_AND_FREE_MEMORY(AIDA_INTERNAL_EXCEPTION, "can't convert argument to string: %s", "<json>", EXIT_FAILURE) \
        } \
        *ptr = nextStringPosition; \
        nextStringPosition+=strlen(nextStringPosition)+1; \
    } \
}

#define ASCANF_SET_STRING_ARRAY \
{ \
    size_t pointerSpace = arrayCount * sizeof(char*); \
    ALLOCATE_AND_TRACK_MEMORY(env, *arrayPtr, pointerSpace + totalStingLengthOf(arrayRoot) + arrayCount + 1, "string array arguments", EXIT_FAILURE) \
    nextStringPosition = ((char*)*arrayPtr) + pointerSpace; \
    for (int i = 0; i < arrayCount; i++) { \
        jsonRoot = arrayRoot->u.array.values[i]; \
        ASCANF_SET_STRING(ARRAY_TARGET(char *)) \
    } \
}

static void allocateTableColumn(JNIEnv* env, Table* table, Type aidaType, size_t elementSize);
static Type tableArrayTypeOf(Type type);
static size_t tableElementSizeOfOf(Type type);
static int vavscanf(JNIEnv* env, Arguments* arguments, Value* value, const char* formatString, va_list argp);
static Type getAidaType(JNIEnv* env, char format, short isArray, short isLong, short isShort);
static void getJsonPathElements(char* fullJsonPath, char* variableName, char** path);
static void parseFormatString(char* formatString, short formatCharsToProcess, char* format,
		short* isRequired, short* isLong, short* isShort, short* isArray);
static void expectingJsonOrValue(const char* argumentName, short isArray, short* isValue,
		short* valueShouldBeJson);
static int getFloatArgument(Arguments* arguments, char* path, float* target);
static int getDoubleArgument(Arguments* arguments, char* path, double* target);
static int getFloatArrayArgument(Arguments* arguments, char* path, float** target, unsigned int* elementCount);
static int getDoubleArrayArgument(Arguments* arguments, char* path, double** target, unsigned int* elementCount);
static FloatingPointValue* getFloatingPointValue(Arguments* arguments, char* path);
static void* _getFloatArray(Arguments* arguments, char* path, bool forFloat, unsigned int* elementCount);
static float* getFloatArray(Arguments* arguments, char* path, unsigned int* elementCount);
static double* getDoubleArray(Arguments* arguments, char* path, unsigned int* elementCount);

/**
 * ascanf, avscanf
 *
 * Synopsis
 *
 *     int ascanf(Arguments *arguments, const char *format, ...);
 *     int avscanf(Arguments *arguments, Value *value, const char *format, ...);
 *
 * Details
 * Reads data from the given arguments and stores them according to parameter format into the locations given by the additional arguments, as if scanf was used, but reading from arguments instead of the standard input (stdin).
 *
 * The additional arguments should point to already allocated objects of the type specified by their corresponding format specifier.  For strings and arrays only the pointer needs to be pre-allocated.
 *
 * The only space allocated by this function is for the array of pointers to strings or arrays.  On return strings will point to the original string in the given arguments so the strings themselves do not need to be freed by the caller.
 *
 * Note, only the provided pointer needs to be freed as only one allocation is made e.g.
 *
 *     Arguments arguments;
 *     int *intArray;
 *     ascanf(arguments "%da, "fooArray", &intArray);
 *     // Do stuff
 *     free(intArray);
 *
 * Differences from scanf
 * There are a number of other differences from scanf which are best described by example:
 *
 * 1. Scan into simple variable
 *
 *     int n;
 *     ascanf("%d", "NPOS", &n);
 *
 * You must always provide the name of the variable and the pointer to the place to put the value in pairs
 *
 * 2. Optional arguments
 * Optional arguments are specified with the o character before the format character.
 *
 *     short flag = 10;  // 10 is the default value
 *     ascanf("%ohd", "flag", &flag);
 *
 * By default all arguments are considered required unless this character is specified. For optional arguments the pointer provided must point to the default value. In the case of arrays and strings this will be copied into allocated storage that will need to be freed as normal. i.e. strings themselves don't need to be freed.
 *
 * Variable Names
 * 1. You can specify simple variable names
 *     int simpleInt;
 *     ascanf(&arguments "%d, "simple", &simpleInt);
 *
 * 1. You can specify simple names or you can use dot and square brace notation to refer to arguments that refer to json structures. e.g. given a variable named json and presented as
 *
 *     json=' { "foo": {"bar": 0} }}'
 *
 * You can specify the name as json.foo.bar to retrieve the 0 value *
 *
 * 2. Also given a variable named jsonArray and presented as
 *     jsonArray=' [ {"foo": 10}, {"bar": 20} ]'
 *
 * You can specify the name as jsonArray[1].bar to retrieve the 20 value
 *
 * 3. Finally if you use the name value in the the avscanf() function will use the supplied value to get the data for that parameter
 *     Arguments arguments;
 *     Value value;
 *     int *intArray;
 *     avscanf(&arguments &value, "%da, "fooArray", &intArray);
 *     // Do stuff
 *     free(intArray);
 *
 * Format Specifiers
 * Supported formats specifiers
 * - b : unsigned char * - extract a single byte into the corresponding variable, translate “true”, “false” etc
 * - c : char * - extract a single character into the corresponding variable
 * - d : int * - extract an integer into the corresponding variable (see l & h below).
 * - f  : float * - extract a floating point number (see l below)
 * - s : char * - extract a string of characters into allocated space and point the corresponding variable to it
 * - u : unsigned int * - extract an unsigned integer into the corresponding variable (see l & h below)
 *
 * Required flag
 * - o - optional precede the format with 'o' to indicate that the argument is optional
 *
 * Prefixes
 * - h - short * - preceding d will retrieve a short e.g. %hd
 * - l - long *, double * - preceding d will retrieve a long eg. %ld, preceding f will retrieve a double eg. %lf
 *
 * Suffixes
 * - a - extract an array of the preceding type into a block of allocated space and point the corresponding variable to it. the variable will have an extra level of indirection than the non-array version e.g. "%d" "int *" becomes "%da" "int **"
 *
 * @param env
 * @param arguments      arguments that the function processes as its source to retrieve the data.
 * @param value          value that the function processes as its source to retrieve the data
 * @param formatString   C  string that contains a format string as described above
 * @param ...            Depending on the format string, the function may expect a sequence of additional arguments,
 * 						 containing pairs of names and pointers to allocated storage (except as indicated above),
 * 						 where the interpretation of the extracted data is stored with the appropriate type.
 *                       There should be at least as many pairs of these arguments as the number of values stored
 *                       by the format specifiers.
 *                       Additional arguments are ignored by the function
 * @return EXIT_SUCCESS if all required arguments were read and no errors occurred, otherwise EXIT_FAILURE
 * @throw MissingRequiredArgument if one of the required arguments are missing
*/
int ascanf(JNIEnv* env, Arguments* arguments, const char* formatString, ...)
{
	va_list argp;
	va_start(argp, formatString);
	int status = vavscanf(env, arguments, NULL, formatString, argp);
	va_end (argp);
	return status;
}

int avscanf(JNIEnv* env, Arguments* arguments, Value* value, const char* formatString, ...)
{
	va_list argp;
	va_start(argp, formatString);
	int status = vavscanf(env, arguments, value, formatString, argp);
	va_end (argp);
	return status;
}

static int vavscanf(JNIEnv* env, Arguments* arguments, Value* value, const char* formatString, va_list argp)
{
	// Keep track of stuff to free
	TRACK_ALLOCATED_MEMORY

	if (!formatString) {
		FREE_MEMORY
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, "No format specifiers provided to ascanf");
		return EXIT_FAILURE;
	}

	// Remove white space and empty format strings, also strtok variable must not be const!
	char _formatString[strlen(formatString) + 1];
	sprintf(_formatString, "- %s", formatString);
	char* formatSpecifier = strtok(_formatString, "% ");
	int numberOfFormatStrings = 0;
	char formatSpecifiers[MAX_FORMAT_SPECIFIERS][MAX_FORMAT];
	for (int i = 0; formatSpecifier; ++i) {
		numberOfFormatStrings++;
		strcpy(formatSpecifiers[i], formatSpecifier);
		formatSpecifier = strtok(NULL, "% ");
	}

	if (!numberOfFormatStrings) {
		FREE_MEMORY
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, "No format specifiers provided to ascanf");
		return EXIT_FAILURE;
	}

	// nextStringPosition: Used to store strings in space at end of pointers to strings
	// String space is allocated as follows:
	// +------------------------+----------+----------+----------+---------+
	// | pointers to strings    | string 1 | string 2 | string 3 | ...     |
	// +------------------------+----------+----------+----------+---------+
	// So only one block of memory needs to be freed.
	// The nextStringPosition initially points to the first position after the list of pointers
	// to strings and is incremented by the string length each time
	char* nextStringPosition;

	// loop over format specifiers
	for (int formatNumber = 0; formatNumber < numberOfFormatStrings; formatNumber++) {
		formatSpecifier = formatSpecifiers[formatNumber];

		short formatSpecifierLength = (short)strlen(formatSpecifier);

		// If format specifier is empty or our artificially added prefix then silently ignore
		if (!formatSpecifierLength || *formatSpecifier == '-') {
			continue;
		}

		// extract the format, isRequiredFlag, isLong, isShort, and isArray
		char format = 0x0;
		short isRequired = true, isLong = false, isShort = false, isArray = false;

		parseFormatString(formatSpecifier, formatSpecifierLength, &format, &isRequired, &isLong, &isShort, &isArray);

		// Invalid format - if no format was specified
		if (!format) {
			SPRINF_ERROR_AND_FREE_MEMORY(AIDA_INTERNAL_EXCEPTION, "incorrect format string: %%%s", formatSpecifier,
					EXIT_FAILURE)
		}

		// get argumentName
		// This is the name of the argument that we will get the value from
		char* argumentName = va_arg (argp, char *);
		if (!argumentName) {
			SPRINF_ERROR_AND_FREE_MEMORY(AIDA_INTERNAL_EXCEPTION, "missing variable to correspond to format: %%%s",
					formatSpecifier, EXIT_FAILURE)
		}

		// Set valueShouldBeJson (if argument name should reference json
		// or if we are expecting an array or if the argument name has json path in it
		// Set isValue (if we need to get the value from the "value" argument)
		short valueShouldBeJson = false, isValue = false;
		expectingJsonOrValue(argumentName, isArray, &isValue, &valueShouldBeJson);

		// Get target variable pointer
		void* target = va_arg (argp, void *);
		void** arrayPtr = (void**)target;
		unsigned int* elementCount;
		if (isArray) {
			elementCount = va_arg (argp, unsigned int *);
			*elementCount = 0;
		}

		// Convert format, isArray, isLong, and isShort into an AIDA_TYPE
		Type aidaType = getAidaType(env, format, isArray, isLong, isShort);
		CHECK_EXCEPTION_FREE_MEMORY_AND_RETURN_(EXIT_FAILURE)

		// If this is for a FLOAT or DOUBLE then try to get ieee version if available
		if (aidaType == AIDA_FLOAT_TYPE) {
			if (getFloatArgument(arguments, argumentName, (float*)target) == EXIT_SUCCESS) {
				return EXIT_SUCCESS;
			}
		} else if (aidaType == AIDA_DOUBLE_TYPE) {
			if (getDoubleArgument(arguments, argumentName, (double*)target) == EXIT_SUCCESS) {
				return EXIT_SUCCESS;
			}
		}

		if (aidaType == AIDA_FLOAT_ARRAY_TYPE) {
			if (getFloatArrayArgument(arguments, argumentName, (float**)target, elementCount) == EXIT_SUCCESS) {
				return EXIT_SUCCESS;
			}
		} else if (aidaType == AIDA_DOUBLE_TYPE) {
			if (getDoubleArrayArgument(arguments, argumentName, (double**)target, elementCount) == EXIT_SUCCESS) {
				return EXIT_SUCCESS;
			}
		}

		// If this is for a FLOAT_ARRAY or DOUBLE_ARRAY then get the ieee version if available

		// if the argument is json then the name may contain dots and square braces,
		// so this variable contains just the argument name part at the beginning.
		// e.g. if the argumentName="bpms[0].name" then jsonArgumentName="bpms"
		char jsonArgumentName[strlen(argumentName) + 1];

		// The json path used to identify the element in the given json that we should use.
		// It is the remaining part of the argument name after the jsonArgumentName has been removed.
		// e.g. if the argumentName="bpms[0].name" then jsonPath="[0].name"
		char* jsonPath = "";

		// This is the value extracted from the provided argument that has been parsed for json
		Value elementValue;

		// get the valueString and jsonRoot
		// valueString is the string representation of the argument's value and
		// jsonRoot is the root of the parsed json version of the argument, if the argument is json
		char* stringValue = NULL;
		json_value* jsonRoot = NULL;
		json_type jsonType = json_none;

		if (valueShouldBeJson) {
			getJsonPathElements(argumentName, jsonArgumentName, &jsonPath);
		}

		// Is using value argument
		if (isValue) {
			// First check if we have had the value passed to us as an argument
			// If not then go and get it
			if (value) {
				if (isArray && value->type == AIDA_STRING_TYPE) {

					// ignore passed in value if we need an array but the user didn't give us an array
					// Just take the string and wrap it as an array
					elementValue = asArrayValue(value->value.stringValue);
					if (elementValue.type != AIDA_JSON_TYPE) {
						SPRINF_ERROR_AND_FREE_MEMORY(AIDA_INTERNAL_EXCEPTION, "Unable to make array of: %s",
								value->value.stringValue, EXIT_FAILURE)
					}
					value = &elementValue;
					TRACK_JSON(elementValue.value.jsonValue)
				}
			} else {
				if (isArray && !*jsonPath) {
					elementValue = getArrayValue(env, *arguments);
				} else {
					elementValue = getValue(env, *arguments);
				}
				CHECK_EXCEPTION_FREE_MEMORY_AND_RETURN_(EXIT_FAILURE);
				value = &elementValue;
				if (elementValue.type == AIDA_JSON_TYPE) {
					TRACK_JSON(elementValue.value.jsonValue)
					valueShouldBeJson = true;
				}
			}

			// Check if the value has been properly set
			if (value->type == AIDA_NO_TYPE) {
				if (isRequired) {
					SPRINF_ERROR_AND_FREE_MEMORY(MISSING_REQUIRED_ARGUMENT_EXCEPTION, "Missing required argument: %s",
							argumentName, EXIT_FAILURE)
				} else {
					// If this is a string and a default has been set but the optional string has not been provided
					// then allocate space for it and set it
					// so that callers can uniformly free all strings
					char* defaultString = *(char**)target;
					if (aidaType == AIDA_STRING_TYPE && defaultString) {
						char* allocatedString;
						ALLOCATE_AND_TRACK_STRING(env, allocatedString, defaultString, "default arguments",
								EXIT_FAILURE)
						*(char**)target = allocatedString;
					}
					continue;
				}
			}

			if (valueShouldBeJson) {
				jsonRoot = getJsonValue(value, jsonPath);
				jsonType = jsonRoot->type;
			} else {
				stringValue = value->value.stringValue;
			}
		} else {
			// Is not using value argument
			if (valueShouldBeJson) {
				// Should be a normal json argument
				if (isArray && !*jsonPath) {
					elementValue = getNamedArrayValue(env, *arguments, jsonArgumentName);
				} else {
					elementValue = getNamedValue(env, *arguments, jsonArgumentName);
				}
				CHECK_EXCEPTION_FREE_MEMORY_AND_RETURN_(EXIT_FAILURE);
				if (elementValue.type != AIDA_JSON_TYPE) {
					if (isRequired) {
						SPRINF_ERROR_AND_FREE_MEMORY(MISSING_REQUIRED_ARGUMENT_EXCEPTION,
								"Missing required argument: %s", argumentName, EXIT_FAILURE)
					} else {
						continue;
					}
				}
				TRACK_JSON(elementValue.value.jsonValue)
				jsonRoot = getJsonValue(&elementValue, jsonPath);
				jsonType = jsonRoot->type;
			} else {
				// Normal string argument
				Argument elementArgument = getArgument(*arguments, argumentName);
				if (!elementArgument.name) {
					if (isRequired) {
						SPRINF_ERROR_AND_FREE_MEMORY(MISSING_REQUIRED_ARGUMENT_EXCEPTION,
								"Missing required argument: %s", argumentName, EXIT_FAILURE)
					} else {
						continue;
					}
				}
				stringValue = elementArgument.value;
			}
		}

		// Note that for strings we allocate enough memory for all the pointers and for the strings as well
		// stored contiguously afterwards so that the caller simply frees the pointer-to-pointer to strings
		// +------------------------+----------+----------+----------+---------+
		// | pointers to strings    | string 1 | string 2 | string 3 | ...     |
		// +------------------------+----------+----------+----------+---------+
		// For all other arrays the allocated space is simply a contiguous set of elements of the base array type

		// From this point forwards we can be sure that all arrays have a valid arrayRoot set.
		json_value* arrayRoot = NULL;
		unsigned int arrayCount;
		if (isArray) {
			arrayRoot = jsonRoot;
			arrayCount = arrayRoot->u.array.length;
		}

		// Now we have a valueString or jsonRoot, a target to put the data and the type we want to extract!
		// based on format process the argument and/or value
		switch (aidaType) {
		case AIDA_BOOLEAN_TYPE: ASCANF_SET_BOOLEAN(target)
			break;
		case AIDA_BOOLEAN_ARRAY_TYPE: ASCANF_SET_BOOLEAN_ARRAY
			break;
		case AIDA_BYTE_TYPE: ASCANF_SET_BYTE(target)
			break;
		case AIDA_BYTE_ARRAY_TYPE: ASCANF_SET_BYTE_ARRAY
			break;
		case AIDA_SHORT_TYPE: ASCANF_SET_SCALAR("%hi", short, json_integer, "short", target)
			break;
		case AIDA_SHORT_ARRAY_TYPE: ASCANF_SET_ARRAY("%hi", short, json_integer, "short")
			break;
		case AIDA_UNSIGNED_SHORT_TYPE: ASCANF_SET_SCALAR("%hu", unsigned short, json_integer, "unsigned short", target)
			break;
		case AIDA_UNSIGNED_SHORT_ARRAY_TYPE: ASCANF_SET_ARRAY("%hu", unsigned short, json_integer, "unsigned short")
			break;
		case AIDA_INTEGER_TYPE: ASCANF_SET_SCALAR("%d", int, json_integer, "integer", target)
			break;
		case AIDA_INTEGER_ARRAY_TYPE: ASCANF_SET_ARRAY("%d", int, json_integer, "integer")
			break;
		case AIDA_UNSIGNED_INTEGER_TYPE: ASCANF_SET_SCALAR("%u", unsigned int, json_integer, "unsigned integer", target)
			break;
		case AIDA_UNSIGNED_INTEGER_ARRAY_TYPE: ASCANF_SET_ARRAY("%u", unsigned int, json_integer, "unsigned integer")
			break;
		case AIDA_LONG_TYPE: ASCANF_SET_SCALAR("%ld", long, json_integer, "long", target)
			break;
		case AIDA_LONG_ARRAY_TYPE: ASCANF_SET_ARRAY("%ld", long, json_integer, "long")
			break;
		case AIDA_UNSIGNED_LONG_TYPE: ASCANF_SET_SCALAR("%lu", unsigned long, json_integer, "unsigned long", target)
			break;
		case AIDA_UNSIGNED_LONG_ARRAY_TYPE: ASCANF_SET_ARRAY("%lu", unsigned long, json_integer, "unsigned long")
			break;
		case AIDA_FLOAT_TYPE: ASCANF_SET_SCALAR("%f", float, json_double, "float", target)
			break;
		case AIDA_FLOAT_ARRAY_TYPE: ASCANF_SET_ARRAY("%f", float, json_double, "float")
			break;
		case AIDA_DOUBLE_TYPE: ASCANF_SET_SCALAR("%lf", double, json_double, "double", target)
			break;
		case AIDA_DOUBLE_ARRAY_TYPE: ASCANF_SET_ARRAY("%lf", double, json_double, "double")
			break;
		case AIDA_STRING_TYPE: ASCANF_SET_STRING(target)
			break;
		case AIDA_STRING_ARRAY_TYPE: ASCANF_SET_STRING_ARRAY
			break;

			// Will never happen so ignore
		case AIDA_NO_TYPE:
		case AIDA_VOID_TYPE:
		case AIDA_TABLE_TYPE:
		case AIDA_JSON_TYPE:
		default:
			break;
		}

		if (isArray) {
			*elementCount = arrayCount;
		}
	}

	FREE_JSON
	return EXIT_SUCCESS;
}

/**
 * See if there is a ieee float value stored in arguments.  If so set target
 *
 * @param arguments
 * @param path path to look for in arguments
 * @param target the place to store the float
 * @return EXIT_SUCCESS if found EXIT_FAILURE if not
 */
static int getFloatArgument(Arguments* arguments, char* path, float* target)
{
	FloatingPointValue* floatingPointValue = getFloatingPointValue(arguments, path);
	if (floatingPointValue) {
		*target = floatingPointValue->value.floatValue;
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

/**
 * See if there is a ieee double value stored in arguments.  If so set target
 *
 * @param arguments
 * @param path path to look for in arguments
 * @param target the place to store the double
 * @return EXIT_SUCCESS if found EXIT_FAILURE if not
 */
static int getDoubleArgument(Arguments* arguments, char* path, double* target)
{
	FloatingPointValue* floatingPointValue = getFloatingPointValue(arguments, path);
	if (floatingPointValue) {
		*target = floatingPointValue->value.doubleValue;
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

/**
 * See if there is an ieee float array stored in arguments.  If so allocate space and
 * and set target
 *
 * @param arguments
 * @param path path to look for in arguments
 * @param target the place to store the float array
 * @return EXIT_SUCCESS if found EXIT_FAILURE if not
 */
static int getFloatArrayArgument(Arguments* arguments, char* path, float** target, unsigned int* elementCount)
{
	float* floatArray = getFloatArray(arguments, path, elementCount);
	if (floatArray) {
		*target = floatArray;
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}
/**
 * See if there is an ieee double array stored in arguments.  If so allocate space and
 * and set target
 *
 * @param arguments
 * @param path path to look for in arguments
 * @param target the place to store the double array
 * @return EXIT_SUCCESS if found EXIT_FAILURE if not
 */
static int getDoubleArrayArgument(Arguments* arguments, char* path, double** target, unsigned int* elementCount)
{
	double* doubleArray = getDoubleArray(arguments, path, elementCount);
	if (doubleArray) {
		*target = doubleArray;
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

/**
 * Get a floating point value by path.  This will look up the value in the arguments
 * by searching for one with the given path
 * @param arguments
 * @param path
 * @return NULL of not found
 */
static FloatingPointValue* getFloatingPointValue(Arguments* arguments, char* path)
{
	for (int i = 0; i < arguments->floatingPointValuesCountCount; i++) {
		if (strcasecmp(path, arguments->floatingPointValues[i].path) == 0) {
			return &arguments->floatingPointValues[i];
		}
	}
	return NULL;
}

/**
 * Get an array of floats by searching for an array rooted at path
 * Space for the array will be allocated if any are found (must be freed by caller)
 *
 * @param arguments
 * @param path
 * @return
 */
static float* getFloatArray(Arguments* arguments, char* path, unsigned int* elementCount)
{
	return (float*)_getFloatArray(arguments, path, true, elementCount);
}

/**
 * Get an array of doubles by searching for an array rooted at path
 * Space for the array will be allocated if any are found (must be freed by caller)
 *
 * @param arguments
 * @param path
 * @return
 */
static double* getDoubleArray(Arguments* arguments, char* path, unsigned int* elementCount)
{
	return (double*)_getFloatArray(arguments, path, false, elementCount);
}

static void* _getFloatArray(Arguments* arguments, char* path, bool forFloat, unsigned int* elementCount)
{
	// To store the floats/doubles found and return to caller
	void* theArray = NULL;

	// To store the calculated element path
	char elementPath[strlen(path) + 10];

	int numberOfFloatAllocated = 0, numberOfFloatsFound = 0;

	FloatingPointValue* floatingPointValue;
	sprintf(elementPath, "%s[%d]", path, numberOfFloatsFound);

	while ((floatingPointValue = getFloatingPointValue(arguments, elementPath))) {
		if (numberOfFloatAllocated == 0) {
			// allocate space
			numberOfFloatAllocated += MIN_FLOAT_ALLOCATIONS;
			theArray = calloc(numberOfFloatAllocated, forFloat ? sizeof(float) : sizeof(double));
			if (theArray == NULL) {
				return NULL;
			}
		} else if (numberOfFloatAllocated <= numberOfFloatsFound) {
			// reallocate space
			numberOfFloatAllocated += MIN_FLOAT_ALLOCATIONS;
			theArray = realloc(theArray, numberOfFloatAllocated * (forFloat ? sizeof(float) : sizeof(double)));
		}
		if (forFloat) {
			((float*)theArray)[numberOfFloatsFound] = floatingPointValue->value.floatValue;
		} else {
			((double*)theArray)[numberOfFloatsFound] = floatingPointValue->value.doubleValue;
		}
		sprintf(elementPath, "%s[%d]", path, ++numberOfFloatsFound);
	}
	*elementCount = numberOfFloatsFound;
	return theArray;
}

/**
 * Determine if we are expecting json and/or whether we are expecting a value argument to have been provided
 *
 * @param argumentName
 * @param isArray
 * @param isValue
 * @param valueShouldBeJson
 */
static void expectingJsonOrValue(const char* argumentName, short isArray, short* isValue,
		short* valueShouldBeJson)
{
	if (strncasecmp(argumentName, "value", 5) == 0) {
		(*isValue) = true;
	}

	if (isArray || strchr(argumentName, '.') != NULL || strchr(argumentName, '[') != NULL) {
		(*valueShouldBeJson) = true;
	}
}

/**
 * Parse out the format from the format specifier
 *
 * @param formatString the pointer to the format string
 * @param formatCharsToProcess count of format characters to process
 * @param format will store the one character format code
 * @param isRequired will store if this argument is required
 * @param isLong will store if the argument type is a long type
 * @param isShort will store if the argument type is a short type
 * @param isArray will store if this argument expecting an array
 */
static void parseFormatString(char* formatString, short formatCharsToProcess, char* format,
		short* isRequired, short* isLong, short* isShort, short* isArray)
{
	if (formatCharsToProcess && *formatString == FORMAT_OPTIONAL_FLAG) {
		(*isRequired) = false;
		formatCharsToProcess--;
		formatString++;
	}

	if (formatCharsToProcess) {
		if (*formatString == FORMAT_PREFIX_SHORT) {
			(*isShort) = true;
			formatCharsToProcess--;
			formatString++;
		} else if (*formatString == FORMAT_PREFIX_LONG) {
			(*isLong) = true;
			formatCharsToProcess--;
			formatString++;
		}
	}

	if (formatCharsToProcess) {
		(*format) = *formatString++;
		formatCharsToProcess--;
	}

	if (formatCharsToProcess && *formatString == FORMAT_SUFFIX_ARRAY) {
		(*isArray) = true;
	}
}

/**
 * Break a json path into the initial variable name and the remaining path.
 * e.g.
 *     value.names[1]
 * is broken into
 *     variableName = value
 *     path = names[1]
 *
 * @param fullJsonPath the full json path to break
 * @param variableName to store the variable name
 * @param path to store the extracted path
 */
static void getJsonPathElements(char* fullJsonPath, char* variableName, char** path)
{
	char* firstDot = strchr(fullJsonPath, '.');
	char* firstParen = strchr(fullJsonPath, '[');

	// If there are no path elements then return whole thing as variable and an empty path
	if (firstDot == NULL && firstParen == NULL) {
		strcpy(variableName, fullJsonPath);
		*path = &fullJsonPath[strlen(fullJsonPath)]; // The empty string
		return;
	}

	char* relativeJsonPath = (firstDot == NULL || firstParen == NULL) ?
							 MAX(firstDot, firstParen) :
							 MIN(firstParen, firstDot);
	int lenRootArgument = (int)(relativeJsonPath - fullJsonPath);

	if (*relativeJsonPath == '.') {
		relativeJsonPath++; // skip opening dot
	}
	memcpy(variableName, fullJsonPath, lenRootArgument);
	variableName[lenRootArgument] = 0x0;
	*path = relativeJsonPath;
}

/**
 * Depending on the combination of options specified in a format string determine the target aida type
 *
 * @param env jni env for throwing errors
 * @param format the format character of the format string
 * @param isArray true if the array indicator is set
 * @param isLong true if the long indicator is set
 * @param isShort true if the short indicator is set
 * @return the target aida type
 */
static Type getAidaType(JNIEnv* env, char format, short isArray, short isLong, short isShort)
{
	if (isArray) {
		if (isLong) {
			switch (format) {
			case FORMAT_INTEGER:
				return AIDA_LONG_ARRAY_TYPE;
			case FORMAT_UNSIGNED_INTEGER:
				return AIDA_UNSIGNED_LONG_ARRAY_TYPE;
			case FORMAT_FLOAT:
				return AIDA_DOUBLE_ARRAY_TYPE;
			default:
				break;
			}
		} else if (isShort) {
			switch (format) {
			case FORMAT_INTEGER:
				return AIDA_SHORT_ARRAY_TYPE;
			case FORMAT_UNSIGNED_INTEGER:
				return AIDA_UNSIGNED_SHORT_ARRAY_TYPE;
			default:
				break;
			}
		} else {
			switch (format) {
			case FORMAT_INTEGER:
				return AIDA_INTEGER_ARRAY_TYPE;
			case FORMAT_UNSIGNED_INTEGER:
				return AIDA_UNSIGNED_INTEGER_ARRAY_TYPE;
			case FORMAT_FLOAT:
				return AIDA_FLOAT_ARRAY_TYPE;
			case FORMAT_STRING:
				return AIDA_STRING_ARRAY_TYPE;
			case FORMAT_BYTE:
				return AIDA_BYTE_ARRAY_TYPE;
			case FORMAT_BOOLEAN:
				return AIDA_BOOLEAN_ARRAY_TYPE;
			default:
				break;
			}
		}
	} else {
		if (isLong) {
			switch (format) {
			case FORMAT_INTEGER:
				return AIDA_LONG_TYPE;
			case FORMAT_UNSIGNED_INTEGER:
				return AIDA_UNSIGNED_LONG_TYPE;
			case FORMAT_FLOAT:
				return AIDA_DOUBLE_TYPE;
			default:
				break;
			}
		} else if (isShort) {
			switch (format) {
			case FORMAT_INTEGER:
				return AIDA_SHORT_TYPE;
			case FORMAT_UNSIGNED_INTEGER:
				return AIDA_UNSIGNED_SHORT_TYPE;
			default:
				break;
			}
		} else {
			switch (format) {
			case FORMAT_INTEGER:
				return AIDA_INTEGER_TYPE;
			case FORMAT_UNSIGNED_INTEGER:
				return AIDA_UNSIGNED_INTEGER_TYPE;
			case FORMAT_FLOAT:
				return AIDA_FLOAT_TYPE;
			case FORMAT_STRING:
				return AIDA_STRING_TYPE;
			case FORMAT_BYTE:
				return AIDA_BYTE_TYPE;
			case FORMAT_BOOLEAN:
				return AIDA_BOOLEAN_TYPE;
			default:
				break;
			}
		}
	}

	aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, "incorrect format string passed to ascanf() or avscanf()");
	return AIDA_NO_TYPE;
}

/**
 * Make a table for return.  Specify the number of rows and columns, then call tableAddColumn() and tableAddStringColumn() to add
 * columns before returning the table
 *
 * @param env jni env for throwing errors
 * @param rows the number of rows
 * @param columns the number of columns
 * @return the newly created table
 */
Table tableCreate(JNIEnv* env, int rows, int columns)
{
	Table table;
	memset(&table, 0, sizeof(table));
	table._currentColumn = 0;  // Reset current column so that any addColumn() calls are correct
	table.columnCount = 0;

	if (rows <= 0) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, "Attempt to allocate a zero length table");
		return table;
	}

	// Allocate space for the table columns and column types
	ALLOCATE_MEMORY_OR_RETURN(env, table.ppData, columns * sizeof(void*), "table columns", table)
	ALLOCATE_MEMORY_OR_RETURN(env, table.types, columns * sizeof(Type*), "table column types", table)
	table.rowCount = rows;
	table.columnCount = columns;
	return table;
}

/**
 * This reads data from a buffer that is itself a list of pointers to strings
 * We allocate just enough space to store the strings in our table
 *
 * @param env jni env for throwing errors
 * @param table the table to add the column to
 * @param data the data to add to this column
 */
void tableAddStringColumn(JNIEnv* env, Table* table, char** data)
{
	tableAddColumn(env, table, AIDA_STRING_ARRAY_TYPE, data, false);
	CHECK_EXCEPTION_AND_RETURN_VOID

	// allocate data for each string too
	char** stringArray = table->ppData[table->_currentColumn];
	for (int row = 0; row < table->rowCount; row++, data++) {
		ALLOCATE_STRING_OR_RETURN_VOID(env, stringArray[row], *data, "table strings")
	}

	table->_currentColumn++;
}

/**
 * This reads data from an allocated space that is rows * width with each string occupying width characters
 * Though the strings are null terminated if there is space, there is no guarantee so an exact number of
 * bytes is copied.  Each string in the table is allocated maximally.
 *
 * @param env jni env for throwing errors
 * @param table the table to add the column to
 * @param data the data to add to this column, a buffer of width * table->rowCount size
 * @param width the width of the strings
 */
void tableAddFixedWidthStringColumn(JNIEnv* env, Table* table, void* data, int width)
{
	tableAddColumn(env, table, AIDA_STRING_ARRAY_TYPE, data, false);
	CHECK_EXCEPTION_AND_RETURN_VOID

	// allocate data for each string too
	char** stringArray = table->ppData[table->_currentColumn];
	char* dataPointer = (char*)data;
	for (int row = 0; row < table->rowCount; row++, dataPointer += width) {
		ALLOCATE_FIXED_LENGTH_STRING_OR_RETURN_VOID(env, stringArray[row], dataPointer, width + 1, "table strings")
		stringArray[row][width] = 0x0;
	}

	table->_currentColumn++;
}

/**
 * Add a column of arbitrary type to a table.  Add the given data to the
 * column assuming that the data has the correct number of rows for the
 * table define by table->rowCount.  Don't call this for string types!
 *
 * @param env jni env for throwing errors
 * @param table the table to add the column to
 * @param type the type of this table column
 * @param data the data to add to this column, a buffer of sizeof(type) * table->rowCount size
 * @param ieeeFormat true if the data provided is already in ieee format
 */
void tableAddColumn(JNIEnv* env, Table* table, Type type, void* data, bool ieeeFormat)
{
	// Table full?
	if (table->_currentColumn == table->columnCount) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"Internal Error: more columns added than table size");
		return;
	}

	// No Data supplied ?
	if (!data) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"Internal Error: Attempt to add column with no data");
		return;
	}

	// Correct type for tables
	type = tableArrayTypeOf(type);

	// Set column type, and allocate space
	allocateTableColumn(env, table, type, tableElementSizeOfOf(type));
	CHECK_EXCEPTION_AND_RETURN_VOID

	// Rest of processing for strings is done in addStringColumn
	if (type == AIDA_STRING_ARRAY_TYPE) {
		return;
	}

	if (!ieeeFormat) {
		// Convert float values if float array
		if (type == AIDA_FLOAT_ARRAY_TYPE) {
			CONVERT_FROM_VMS_FLOAT(((float*)data), (int2u)table->rowCount)
		}

		// Convert double values if double array
		if (type == AIDA_DOUBLE_ARRAY_TYPE) {
			CONVERT_FROM_VMS_DOUBLE((double*)data, (int2u)table->rowCount)
		}
	}

	// Add data to column
	for (int row = 0; row < table->rowCount; row++) {
		// add data and increment pointer based on type
		switch (type) {
		case AIDA_BOOLEAN_ARRAY_TYPE:
		case AIDA_BYTE_ARRAY_TYPE:
			((unsigned char*)(table->ppData[table->_currentColumn]))[row] = ((unsigned char*)data)[row];
			break;
		case AIDA_SHORT_ARRAY_TYPE:
			((short*)(table->ppData[table->_currentColumn]))[row] = ((short*)data)[row];
			break;
		case AIDA_INTEGER_ARRAY_TYPE:
			((int*)(table->ppData[table->_currentColumn]))[row] = ((int*)data)[row];
			break;
		case AIDA_LONG_ARRAY_TYPE:
			((long*)(table->ppData[table->_currentColumn]))[row] = ((long*)data)[row];
			break;
		case AIDA_FLOAT_ARRAY_TYPE:
			((float*)(table->ppData[table->_currentColumn]))[row] = ((float*)data)[row];
			break;
		case AIDA_DOUBLE_ARRAY_TYPE:
			((double*)(table->ppData[table->_currentColumn]))[row] = ((double*)data)[row];
			break;
		default:
			aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
					"Internal Error: Call to tableAddColumn() un-supported type");
			return;
		}
	}

	table->_currentColumn++;
}

/**
 * Add a float column to a table with only one row
 *
 * @param env jni env for throwing errors
 * @param table the table to add the column to
 * @param data the data to add to this column
 */
void tableAddSingleRowFloatColumn(JNIEnv* env, Table* table, float data, bool ieeeFloat)
{
	tableAddColumn(env, table, AIDA_FLOAT_TYPE, &data, ieeeFloat);
}

/**
 * Add a long column to a table with only one row
 *
 * @param env jni env for throwing errors
 * @param table the table to add the column to
 * @param data the data to add to this column
 */
void tableAddSingleRowLongColumn(JNIEnv* env, Table* table, long data)
{
	tableAddColumn(env, table, AIDA_LONG_TYPE, &data, false);
}

/**
 * Add a boolean column to a table with only one row
 *
 * @param env jni env for throwing errors
 * @param table the table to add the column to
 * @param data the data to add to this column
 */
void tableAddSingleRowBooleanColumn(JNIEnv* env, Table* table, unsigned char data)
{
	tableAddColumn(env, table, AIDA_BOOLEAN_TYPE, &data, false);
}

/**
 * Add a byte column to a table with only one row
 *
 * @param env jni env for throwing errors
 * @param table the table to add the column to
 * @param data the data to add to this column
 */
void tableAddSingleRowByteColumn(JNIEnv* env, Table* table, unsigned char data)
{
	tableAddColumn(env, table, AIDA_BYTE_TYPE, &data, false);
}

/**
 * Add a short column to a table with only one row
 *
 * @param env jni env for throwing errors
 * @param table the table to add the column to
 * @param data the data to add to this column
 */
void tableAddSingleRowShortColumn(JNIEnv* env, Table* table, short data)
{
	tableAddColumn(env, table, AIDA_SHORT_TYPE, &data, false);
}

/**
 * Add a integer column to a table with only one row
 *
 * @param env jni env for throwing errors
 * @param table the table to add the column to
 * @param data the data to add to this column
 */
void tableAddSingleRowIntegerColumn(JNIEnv* env, Table* table, int data)
{
	tableAddColumn(env, table, AIDA_INTEGER_TYPE, &data, false);
}

/**
 * Add a double column to a table with only one row
 *
 * @param env jni env for throwing errors
 * @param table the table to add the column to
 * @param data the data to add to this column
 */
void tableAddSingleRowDoubleColumn(JNIEnv* env, Table* table, double data, bool ieeeDouble)
{
	tableAddColumn(env, table, AIDA_DOUBLE_TYPE, &data, ieeeDouble);
}

/**
 * Add a string column to a table with only one row
 *
 * @param env jni env for throwing errors
 * @param table the table to add the column to
 * @param data the data to add to this column
 */
void tableAddSingleRowStringColumn(JNIEnv* env, Table* table, char* data)
{
	tableAddStringColumn(env, table, &data);
}

/**
 * Get a float from a given value
 *
 * @param value the value to extract the float from
 * @return the float
 */
float valueGetFloat(Value value)
{
	float floatValue;
	sscanf(value.value.stringValue, "%f", &floatValue);
	return floatValue;
}

/**
 * Get a short from a given value
 *
 * @param value the value to extract the short from
 * @return the short
 */
short valueGetShort(Value value)
{
	short shortValue;
	sscanf(value.value.stringValue, "%hi", &shortValue);
	return shortValue;
}

/**
 * Make a single entry json_value array from a string.  Use getJsonRoot() to get a
 * pointer to the array element directly.
 *
 * @param stringValue the string to add as the single entry in the json_value array
 * @return the single entry json_value array
 */
Value asArrayValue(char* stringValue)
{
	char arrayValueToParse[strlen(stringValue + 17)];
	if (isdigit(*stringValue)) {
		sprintf(arrayValueToParse, "{\"_array\": [%s]}", stringValue);
	} else {
		sprintf(arrayValueToParse, "{\"_array\": [\"%s\"]}", stringValue);
	}
	Value value;
	value.value.jsonValue = json_parse(arrayValueToParse, strlen(arrayValueToParse));
	if (value.value.jsonValue) {
		value.type = AIDA_JSON_TYPE;
	}
	return value;
}

/**
 * Return the corresponding array type of the given type
 *
 * @param type the given type
 * @return the corresponding array type
 */
static Type tableArrayTypeOf(Type type)
{
	switch (type) {
	case AIDA_BOOLEAN_TYPE:
		type = AIDA_BOOLEAN_ARRAY_TYPE;
		break;
	case AIDA_BYTE_TYPE:
		type = AIDA_BYTE_ARRAY_TYPE;
		break;
	case AIDA_SHORT_TYPE:
		type = AIDA_SHORT_ARRAY_TYPE;
		break;
	case AIDA_INTEGER_TYPE:
		type = AIDA_INTEGER_ARRAY_TYPE;
		break;
	case AIDA_LONG_TYPE:
		type = AIDA_LONG_ARRAY_TYPE;
		break;
	case AIDA_FLOAT_TYPE:
		type = AIDA_FLOAT_ARRAY_TYPE;
		break;
	case AIDA_DOUBLE_TYPE:
		type = AIDA_DOUBLE_ARRAY_TYPE;
		break;
	case AIDA_STRING_TYPE:
		type = AIDA_STRING_ARRAY_TYPE;
		break;
	default:
		break;
	}
	return type;
}

/**
 * Return the corresponding data element size for the given type
 *
 * @param type the given type
 * @return the corresponding data element size
 */
static size_t tableElementSizeOfOf(Type type)
{
	size_t size;

	switch (type) {
	case AIDA_BOOLEAN_TYPE:
	case AIDA_BOOLEAN_ARRAY_TYPE:
	case AIDA_BYTE_TYPE:
	case AIDA_BYTE_ARRAY_TYPE:
		size = sizeof(unsigned char);
		break;
	case AIDA_SHORT_TYPE:
	case AIDA_SHORT_ARRAY_TYPE:
		size = sizeof(short);
		break;
	case AIDA_INTEGER_TYPE:
	case AIDA_INTEGER_ARRAY_TYPE:
		size = sizeof(int);
		break;
	case AIDA_LONG_TYPE:
	case AIDA_LONG_ARRAY_TYPE:
		size = sizeof(long);
		break;
	case AIDA_FLOAT_TYPE:
	case AIDA_FLOAT_ARRAY_TYPE:
		size = sizeof(float);
		break;
	case AIDA_DOUBLE_TYPE:
	case AIDA_DOUBLE_ARRAY_TYPE:
		size = sizeof(double);
		break;
	case AIDA_STRING_TYPE:
	case AIDA_STRING_ARRAY_TYPE:
		size = sizeof(char*);
		break;

	default:
		size = 0;
	}
	return size;
}

/**
 * Add a column to a table specifying the type and element size
 *
 * @param table the table to add the column to
 * @param aidaType the type of the column to add
 * @param elementSize the size of each element
 */
static void allocateTableColumn(JNIEnv* env, Table* table, Type aidaType, size_t elementSize)
{
	table->types[table->_currentColumn] = aidaType;
	table->ppData[table->_currentColumn] = ALLOCATE_MEMORY(env, table->rowCount * elementSize, "table data");
}
