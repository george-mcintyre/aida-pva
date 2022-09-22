/** @file
 *  @brief This file contain functions that should be
 *  used directly by Native Channel Providers.
 *  These functions provide all of the features related to processing of Arguments and Table
 *  that allow the Channel Provider to implement its service.
 *
 *     **MEMBER**=SLCLIBS:AIDA_PVALIB
 *     **ATTRIBUTES**=JNI
 *
 *  - ascanf() and avscanf() to scan arguments into provided variables.
 *  - Table creation and building functions: tableAddColumn(),
 *    tableAddSingleRowFloatColumn(),
 *    tableAddSingleRowLongColumn(),
 *    tableAddSingleRowBooleanColumn(),
 *    tableAddSingleRowByteColumn(),
 *    tableAddSingleRowShortColumn(),
 *    tableAddSingleRowIntegerColumn(),
 *    tableAddSingleRowDoubleColumn(),
 *    tableAddSingleRowStringColumn(),
 *    tableAddFixedWidthStringColumn(),
 *    tableAddStringColumn()
 */
#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>

#include "slc_macros.h"           /* vmsstat_t, int2u, int4u, etc. */
#include "sysutil_proto.h"        /* for cvt_vms_to_ieee_flt */

#include "aida_pva_jni_helper.h"
#include "aida_pva_server_helper.h"
#include "aida_pva_types_helper.h"

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
static int getBooleanValue(char* stringValue);
static Value asArrayValue(char* stringValue);

/**
 * ascanf(), avscanf()
 *
 * @paragraph Synopsis
 * @code
 *     int ascanf(Arguments *arguments, const char *format, ...);
 *     int avscanf(Arguments *arguments, Value *value, const char *format, ...);
 * @endcode
 *
 * @paragraph Details
 * Reads data from the given @p arguments and stores them according
 * to parameter format into the locations given by the additional arguments,
 * as if scanf() was used, but reading from arguments instead of the standard input (stdin).
 *
 * @see avscanf() for full details.
 *
 * @param env            The JNI environment.  Used in all functions involving JNI
 * @param arguments      Arguments that the function processes as its source to retrieve the data.
 * @param formatString   C string that contains a format string as described above
 * @param ...            Depending on the format string, the function may expect a sequence of additional arguments,
 * 						 containing pairs of names and pointers to allocated storage (except as indicated above),
 * 						 where the interpretation of the extracted data is stored with the appropriate type.
 *                       There should be at least as many pairs of these arguments as the number of values stored
 *                       by the format specifiers.
 *                       Additional arguments are ignored by the function
 * @return `EXIT_SUCCESS` if all required arguments were read and no errors occurred, otherwise `EXIT_FAILURE`
 * @throw MissingRequiredArgumentException if one of the required arguments are missing
*/
int ascanf(JNIEnv* env, Arguments* arguments, const char* formatString, ...)
{
	va_list argp;
	va_start(argp, formatString);
	int status = vavscanf(env, arguments, NULL, formatString, argp);
	va_end (argp);
	return status;
}

/**
 * ascanf(), avscanf()
 *
 * @paragraph Synopsis
 * @code
 *     int ascanf(Arguments *arguments, const char *format, ...);
 *     int avscanf(Arguments *arguments, Value *value, const char *format, ...);
 * @endcode
 *
 * @paragraph Details
 * Reads data from the given @p arguments and stores them according
 * to parameter format into the locations given by the additional arguments,
 * as if scanf() was used, but reading from arguments instead of the standard input (stdin).
 *
 * The additional arguments should point to already allocated objects of
 * the type specified by their corresponding format specifier.
 * For strings and arrays only the pointer needs to be pre-allocated.
 *
 * The only space allocated by this function is for the strings or arrays.  So callers
 * should only free strings and arrays.  Even if you provide a default value for a string
 * the pointer will be allocated memory on exit from the function, so even then you need to free it.
 *
 * @note
 * Only the provided pointer needs to be freed as only one allocation is made e.g.
 * @code
 *     Arguments arguments;
 *     int *intArray;
 *     ascanf(arguments "%da, "fooArray", &intArray);
 *     // Do stuff
 *     free(intArray);
 * @endcode
 *
 * String space is allocated as follows:
 * @code
 * +------------------------+----------+----------+----------+---------+
 * | pointers to strings    | string 1 | string 2 | string 3 | ...     |
 * +------------------------+----------+----------+----------+---------+
 * @endcode
 *
 * @paragraph dts Differences to scanf()
 * ___
 * There are a number of differences from scanf() which are best described by example:
 *
 * @paragraph gd General differences
 * -# Scan into simple variable/
 * @code
 *     int n;
 *     ascanf("%d", "NPOS", &n);
 * @endcode
 * You must always provide the name of the variable and the pointer to the place to put the value in pairs
 * -# Optional arguments.
 * Optional arguments are specified with the **o** character before the format character.
 * @code
 *     short flag = 10;  // 10 is the default value
 *     ascanf("%ohd", "flag", &flag);
 * @endcode
 *
 * By default all arguments referenced by format specifications are considered required
 * unless the format specification character is preceded by **o**.
 * For optional arguments the pointer provided must point to the default value.
 * In the case of arrays and strings this will be copied into allocated storage that
 * will need to be freed as normal.
 *
 * @paragraph an Argument names
 * -# You can specify simple argument names to search for.  These will simply find the named argument and extract its value.
 * @code
 *     int simpleInt;
 *     ascanf(&arguments "%d, "simple", &simpleInt);
 * @endcode
 * -# You can also use dot and square brace notation to refer to complex arguments that are either arrays or complex objects.
 *  Values that are buried deep inside the json structures can be referenced in this way. e.g., given a
 *  variable named `json` and presented as:
 * @code
 *     json='{"foo": {"bar": 0}}}'
 * @endcode
 * You can specify the @p name as `"json.foo.bar"` to retrieve the `0` value.
 * -# Also given a variable named `jsonArray` and presented as
 * @code
 *     jsonArray='[{"foo": 10}, {"bar": 20}]'
 * @endcode
 * You can specify the #p name as `"jsonArray[1].bar"` to retrieve the `20` value.
 * -# Finally if you use the @p name `"value"`, then the avscanf() function
 * will use the supplied @p value parameter to get the data for that parameter
 * @code
 *     Arguments arguments;
 *     Value value;
 *     int *intArray;
 *     avscanf(&arguments &value, "%da, "fooArray", &intArray);
 *     // Do stuff
 *     free(intArray);
 * @endcode
 *
 * @paragraph fs Format Specifiers
 * ___
 * @paragraph sfs Type specifiers
 * - **b** : `unsigned char *` - interpret the input as a boolean, then extract a single byte into the corresponding variable.
 *   - The following translate to `true`
 *     - not `0`  - integer, short, long,
 *     - not `0.0` - float, double
 *     - `“true”` - char string
 *     - `“t”`  - char string
 *     - `“yes”` - char string
 *     - `“y”`  - char string
 *     - `!0` - short, int, long, float, or double
 *   - The following translate to `false`
 *     - `0`  - integer, short, long,
 *     - `0.0` - float, double
 *     - `“false”` - char string
 *     - `“f”`- char string
 *     - `“no”`- char string
 *     - `“n”`- char string
 *     - `0` - short, int, long, float, or double
 * - **c** : `char *` - extract a single character into the corresponding variable.
 * - **d** : `int *` - extract an integer into the corresponding variable (see **l** & **h** below).
 * - **f** : `float *` - extract a floating point number (see **l** below).
 * - **s** : `char *` - extract a string of characters into allocated space and point the corresponding variable to it.
 * - **u** : `unsigned int *` - extract an unsigned integer into the corresponding variable (see **l** & **h** below).
 *
 * @paragraph rf Required flag
 * - **o** - optional precede the format with this to indicate that the argument is optional.
 *
 * @paragraph Prefixes
 * - **h** - `short *` - preceding **d** will retrieve a short e.g. "%hd".
 * - **l** - `long *`, double * - preceding **d** will retrieve a long eg. `"%ld"`; preceding **f** will retrieve a double eg. `"%lf"`.
 *
 * @paragraph Suffixes
 * - **a** - extract an array of the preceding type into a block of allocated space and point the corresponding variable to it.
 *   The variable will have an extra level of indirection than the non-array version.
 *   e.g.,
 *   @code
 *       int i;
 *       ascanf(..., "%d", &i);
 *   @endcode
 *   becomes
 *   @code
 *       int *ia, n;
 *       ascanf(..., "%da", &ia, &n);
 *   @endcode
 *   Also, you need to provide an extra parameter for each format containing an **a** suffix to hold the count of array
 *   elements found.  The pointer will point to an `int`.
 *
 *
 * @param env            The JNI environment.  Used in all functions involving JNI
 * @param arguments      Arguments that the function processes as its source to retrieve the data.
 * @param value      	 For the avscanf() form this parameter holds the parsed Value given to the Channel Provider endpoint.
 * @param formatString   C string that contains a format string as described above
 * @param ...            Depending on the format string, the function may expect a sequence of additional arguments,
 * 						 containing pairs of names and pointers to allocated storage (except as indicated above),
 * 						 where the interpretation of the extracted data is stored with the appropriate type.
 *                       There should be at least as many pairs of these arguments as the number of references
 *                       in the format specifiers.
 *                       Additional arguments are ignored by the function
 * @return `EXIT_SUCCESS` if all required arguments were read and no errors occurred, otherwise `EXIT_FAILURE`
 * @throw MissingRequiredArgumentException if one of the required arguments are missing
*/
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
			SPRINTF_ERROR_FREE_MEMORY_AND_RETURN_(AIDA_INTERNAL_EXCEPTION, "incorrect format string: %%%s", formatSpecifier,
					EXIT_FAILURE)
		}

		// get argumentName
		// This is the name of the argument that we will get the value from
		char* argumentName = va_arg (argp, char *);
		if (!argumentName) {
			SPRINTF_ERROR_FREE_MEMORY_AND_RETURN_(AIDA_INTERNAL_EXCEPTION, "missing variable to correspond to format: %%%s",
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
		ON_EXCEPTION_FREE_MEMORY_AND_RETURN_(EXIT_FAILURE)

		// If this is for a FLOAT or DOUBLE then try to get ieee version if available
		float floatTarget;
		float* floatArrayTarget;
		double doubleTarget;
		double* doubleArrayTarget;
		if (aidaType == AIDA_FLOAT_TYPE || aidaType == AIDA_DOUBLE_TYPE) {
			// Try getting double first
			if (getDoubleArgument(arguments, argumentName, (double*)&doubleTarget) == EXIT_SUCCESS) {
				if (aidaType == AIDA_FLOAT_TYPE) {  // use for float if that's what you need
					*((float*)target) = (float)doubleTarget;
				} else {
					*((double*)target) = doubleTarget;
				};
				continue;
			}
			// Try float
			if (getFloatArgument(arguments, argumentName, (float*)&floatTarget) == EXIT_SUCCESS) {
				if (aidaType == AIDA_DOUBLE_TYPE) {  // use for double if that's what you need
					*((double*)target) = (double)floatTarget;
				} else {
					*((float*)target) = floatTarget;
				};
				continue;
			}
		}

		if (aidaType == AIDA_FLOAT_ARRAY_TYPE || aidaType == AIDA_DOUBLE_ARRAY_TYPE) {
			// Try double array first
			if (getDoubleArrayArgument(arguments, argumentName, (double**)&doubleArrayTarget, elementCount)
					== EXIT_SUCCESS) {
				TRACK_MEMORY(doubleArrayTarget)
				if (aidaType == AIDA_FLOAT_ARRAY_TYPE) {  // use for float if that's what you need
					// Allocate a new array and copy values
					floatArrayTarget = calloc(*elementCount, sizeof(float));
					TRACK_MEMORY(floatArrayTarget)
					if (!floatArrayTarget) {
						PRINT_ERROR_FREE_MEMORY_AND_RETURN_(AIDA_INTERNAL_EXCEPTION,
								"Could not allocate memory for float argument",
								EXIT_FAILURE)
					}
					*((float**)target) = floatArrayTarget;
					for (int i = 0; i < *elementCount; i++) {
						floatArrayTarget[i] = (float)doubleArrayTarget[i];
					}
					FREE_TRACKED_MEMORY(doubleArrayTarget)
				} else {
					*((double**)target) = doubleArrayTarget;
				};
				continue;
			}

			// Then try float array
			if (getFloatArrayArgument(arguments, argumentName, (float**)&floatArrayTarget, elementCount)
					== EXIT_SUCCESS) {
				TRACK_MEMORY(floatArrayTarget)
				if (aidaType == AIDA_DOUBLE_ARRAY_TYPE) {  // use for double if that's what you need
					// Allocate a new array and copy values
					doubleArrayTarget = calloc(*elementCount, sizeof(double));
					TRACK_MEMORY(doubleArrayTarget)
					if (!doubleArrayTarget) {
						PRINT_ERROR_FREE_MEMORY_AND_RETURN_(AIDA_INTERNAL_EXCEPTION,
								"Could not allocate memory for double argument",
								EXIT_FAILURE)
					}
					*((double**)target) = doubleArrayTarget;
					for (int i = 0; i < *elementCount; i++) {
						doubleArrayTarget[i] = (double)floatArrayTarget[i];
					}
					FREE_TRACKED_MEMORY(floatArrayTarget)
				} else {
					*((float**)target) = floatArrayTarget;
				};
				continue;
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
						SPRINTF_ERROR_FREE_MEMORY_AND_RETURN_(AIDA_INTERNAL_EXCEPTION, "Unable to make array of: %s",
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
				ON_EXCEPTION_FREE_MEMORY_AND_RETURN_(EXIT_FAILURE);
				value = &elementValue;
				if (elementValue.type == AIDA_JSON_TYPE) {
					TRACK_JSON(elementValue.value.jsonValue)
					valueShouldBeJson = true;
				}
			}

			// Check if the value has been properly set
			if (value->type == AIDA_NO_TYPE) {
				if (isRequired) {
					SPRINTF_ERROR_FREE_MEMORY_AND_RETURN_(MISSING_REQUIRED_ARGUMENT_EXCEPTION, "Missing required argument: %s",
							argumentName, EXIT_FAILURE)
				} else {
					// If this is a string and a default has been set but the optional string has not been provided
					// then allocate space for it and set it
					// so that callers can uniformly free all strings
					char* defaultString = *(char**)target;
					if (aidaType == AIDA_STRING_TYPE && defaultString) {
						char* allocatedString;
						ALLOCATE_COPY_AND_TRACK_STRING_AND_ON_ERROR_RETURN_(
								env, allocatedString, defaultString, "default arguments", EXIT_FAILURE)
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
				ON_EXCEPTION_FREE_MEMORY_AND_RETURN_(EXIT_FAILURE);
				if (elementValue.type != AIDA_JSON_TYPE) {
					if (isRequired) {
						SPRINTF_ERROR_FREE_MEMORY_AND_RETURN_(MISSING_REQUIRED_ARGUMENT_EXCEPTION,
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
						SPRINTF_ERROR_FREE_MEMORY_AND_RETURN_(MISSING_REQUIRED_ARGUMENT_EXCEPTION,
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
			if (jsonType != json_array) {
				SPRINTF_ERROR_FREE_MEMORY_AND_RETURN_(UNABLE_TO_GET_DATA_EXCEPTION,
						"Array expected but found scalar or string: %s", argumentName, EXIT_FAILURE)
			}
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
 * Make a Table for return to client.  This is the first call that needs to be made to return a Table.
 * This will create a Table with the specified the number of rows and columns.
 * You need to call tableAddColumn(), tableAddStringColumn(), or any of the
 * other special `tableAdd` functions to add
 * columns to the Table before returning it.
 *
 * @param env            The JNI environment.  Used in all functions involving JNI.
 * @param rows           the number of rows to create the Table with.
 * @param columns        the number of columns to create the Table with,
 * @return the newly created Table
 *
 * @see
 * tableAddColumn(),
 * tableAddStringColumn()
 * tableAddFixedWidthStringColumn(),
 * tableAddSingleRowBooleanColumn(),
 * tableAddSingleRowByteColumn(),
 * tableAddSingleRowShortColumn(),
 * tableAddSingleRowIntegerColumn(),
 * tableAddSingleRowLongColumn(),
 * tableAddSingleRowFloatColumn(),
 * tableAddSingleRowDoubleColumn(),
 * tableAddSingleRowStringColumn(),
 *
 * @paragraph Example
 *
 * Create a two column, two row Table, add data, and return.
 * @code
 * int rows = 2, columns = 2;
 * float xData[rows] = { 1.0f, 2.0f }, yData[rows] = { 7.0f, 8.0f };
 *
 * Table table = tableCreate(env, rows, columns);
 * ON_EXCEPTION_AND_RETURN_(table)
 * tableAddColumn(env, &table, AIDA_FLOAT_TYPE, xData, true);
 * ON_EXCEPTION_AND_RETURN_(table)
 * tableAddColumn(env, &table, AIDA_FLOAT_TYPE, yData, true);
 * ON_EXCEPTION_AND_RETURN_(table)
 * return table;
 * @endcode
 * @note
 * You need to call ON_EXCEPTION_AND_RETURN_(table) after each call to make
 * sure that no exception was raised.
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
	ALLOCATE_MEMORY_AND_ON_ERROR_RETURN_(env, table.ppData, columns * sizeof(void*), "table columns", table)
	ALLOCATE_MEMORY_AND_ON_ERROR_RETURN_(env, table.types, columns * sizeof(Type*), "table column types", table)
	table.rowCount = rows;
	table.columnCount = columns;
	return table;
}

/**
 * Add a column of arbitrary type to a Table.  Add the given data to the
 * column assuming that the data has a number of rows that corresponds to the
 * Table's rowCount.  Memory will be allocated for the data of the column so
 * the @p data buffer presented can be freed up after calling this function.
 *
 * @note
 * Don't call this to add strings to the Table.  Use tableAddStringColumn() for that.
 *
 * The framework will release all memory associated with
 * a Table when you return from your API implementation.
 *
 * @param env            The JNI environment.  Used in all functions involving JNI.
 * @param table          the Table to add the column to.
 * @param type           the type of this Table column.
 * @param data           the data to add to this column, a buffer of `sizeof(type) * table->rowCount` size.
 * @param ieeeFormat     true if the data provided is already in ieee format.  If the data is not in ieee format,
 *        usually because it has been retrieved from some backend system, this function will convert it to ieee format
 *        unless this parameter is set to true.
 *
 * @see
 * tableCreate(),
 * tableAddStringColumn()
 * tableAddFixedWidthStringColumn(),
 * tableAddSingleRowBooleanColumn(),
 * tableAddSingleRowByteColumn(),
 * tableAddSingleRowShortColumn(),
 * tableAddSingleRowIntegerColumn(),
 * tableAddSingleRowLongColumn(),
 * tableAddSingleRowFloatColumn(),
 * tableAddSingleRowDoubleColumn(),
 * tableAddSingleRowStringColumn(),
 *
 * @paragraph Example
 *
 * Create a two column, two row Table, add data, and return.
 * @code
 * int rows = 2, columns = 2;
 * float xData[rows] = { 1.0f, 2.0f }, yData[rows] = { 7.0f, 8.0f };
 *
 * Table table = tableCreate(env, rows, columns);
 * ON_EXCEPTION_AND_RETURN_(table)
 * tableAddColumn(env, &table, AIDA_FLOAT_TYPE, xData, true);
 * ON_EXCEPTION_AND_RETURN_(table)
 * tableAddColumn(env, &table, AIDA_FLOAT_TYPE, yData, true);
 * ON_EXCEPTION_AND_RETURN_(table)
 * return table;
 * @endcode
 * @note
 * You need to call ON_EXCEPTION_AND_RETURN_(table) after each call to make
 * sure that no exception was raised.
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
	ON_EXCEPTION_RETURN_VOID

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
 * Add a String column to the given Table.
 * This reads data from a buffer that is itself a list of pointers to strings.
 * We allocate just enough space to store the strings in our Table.  This is allocated
 * in one buffer so there is only one pointer to release.
 *
 * @note
 * The framework will release all memory associated with
 * a Table when you return from your API implementation.
 *
 * @param env            The JNI environment.  Used in all functions involving JNI.
 * @param table          the Table to add the string column to.
 * @param data           the data to add to this column, a buffer of `sizeof(char *) * table->rowCount` size.
 *     This will contain the strings to be added to the Table.
 *
 * @see
 * tableCreate(),
 * tableAddColumn(),
 * tableAddFixedWidthStringColumn(),
 * tableAddSingleRowBooleanColumn(),
 * tableAddSingleRowByteColumn(),
 * tableAddSingleRowShortColumn(),
 * tableAddSingleRowIntegerColumn(),
 * tableAddSingleRowLongColumn(),
 * tableAddSingleRowFloatColumn(),
 * tableAddSingleRowDoubleColumn(),
 * tableAddSingleRowStringColumn(),
 *
 * @paragraph Example
 *
 * Create a single column, one row Table, add data, and return.
 * @code
 * int rows = 1, columns = 1;
 * char* namesData[rows];
 * namesData[0] = "NAME";
 *
 * Table table = tableCreate(env, rows, columns);
 * ON_EXCEPTION_AND_RETURN_(table)
 * tableAddStringColumn(env, &table, namesData);
 * ON_EXCEPTION_AND_RETURN_(table)
 * return table;
 * @endcode
 * @note
 * You need to call ON_EXCEPTION_AND_RETURN_(table) after each call to make
 * sure that no exception was raised.
 */
void tableAddStringColumn(JNIEnv* env, Table* table, char** data)
{
	tableAddColumn(env, table, AIDA_STRING_ARRAY_TYPE, data, false);
	ON_EXCEPTION_RETURN_VOID

	// allocate data for each string too
	char** stringArray = table->ppData[table->_currentColumn];
	for (int row = 0; row < table->rowCount; row++, data++) {
		ALLOCATE_STRING_AND_ON_ERROR_RETURN_VOID(env, stringArray[row], *data, "table strings")
	}

	table->_currentColumn++;
}

/**
 * This reads data from an allocated space that is rows * width with each string occupying width characters
 * Though the strings are null terminated if there is space, there is no guarantee so an exact number of
 * bytes is copied.  Each string in the Table is allocated maximally.
 *
 * @note
 * The framework will release all memory associated with
 * a Table when you return from your API implementation.
 *
 * @param env            The JNI environment.  Used in all functions involving JNI.
 * @param table          The Table to add the column to.
 * @param data           The data to add to this column, a pointer to `char` buffer containing
 *                       the fixed length strings.  The strings are arranged in
 *                       contiguous blocks @p width long.
 * @param width 		 the width of the strings
 *
 * @see
 * tableCreate(),
 * tableAddColumn(),
 * tableAddStringColumn()
 * tableAddSingleRowBooleanColumn(),
 * tableAddSingleRowByteColumn(),
 * tableAddSingleRowShortColumn(),
 * tableAddSingleRowIntegerColumn(),
 * tableAddSingleRowLongColumn(),
 * tableAddSingleRowFloatColumn(),
 * tableAddSingleRowDoubleColumn(),
 * tableAddSingleRowStringColumn(),
 */
void tableAddFixedWidthStringColumn(JNIEnv* env, Table* table, char* data, int width)
{
	tableAddColumn(env, table, AIDA_STRING_ARRAY_TYPE, data, false);
	ON_EXCEPTION_RETURN_VOID

	// allocate data for each string too
	char** stringArray = table->ppData[table->_currentColumn];
	char* dataPointer = (char*)data;
	for (int row = 0; row < table->rowCount; row++, dataPointer += width) {
		ALLOCATE_FIXED_LENGTH_STRING_AND_ON_ERROR_RETURN_VOID(env, stringArray[row], dataPointer, width + 1, "table strings")
		stringArray[row][width] = 0x0;
	}

	table->_currentColumn++;
}

/**
 * Add a boolean column to a Table with only one row.  This function will allocate
 * the required memory for the single `unsigned char` that is required.
 *
 * @note
 * The framework will release all memory associated with
 * a Table when you return from your API implementation.
 *
 * @param env            The JNI environment.  Used in all functions involving JNI.
 * @param table          The Table to add the column to.
 * @param data           The data to add to this column, a pointer to an `unsigned char`.
 *
 * @see
 * tableCreate(),
 * tableAddColumn(),
 * tableAddStringColumn()
 * tableAddFixedWidthStringColumn(),
 * tableAddSingleRowByteColumn(),
 * tableAddSingleRowShortColumn(),
 * tableAddSingleRowIntegerColumn(),
 * tableAddSingleRowLongColumn(),
 * tableAddSingleRowFloatColumn(),
 * tableAddSingleRowDoubleColumn(),
 * tableAddSingleRowStringColumn(),
 */
void tableAddSingleRowBooleanColumn(JNIEnv* env, Table* table, unsigned char data)
{
	tableAddColumn(env, table, AIDA_BOOLEAN_TYPE, &data, false);
}

/**
 * Add a byte column to a Table with only one row
 *
 * @note
 * The framework will release all memory associated with
 * a Table when you return from your API implementation.
 *
 * @param env            The JNI environment.  Used in all functions involving JNI.
 * @param table          The Table to add the column to.
 * @param data           The data to add to this column, an `unsigned char`.
 *
 * @see
 * tableCreate(),
 * tableAddColumn(),
 * tableAddStringColumn()
 * tableAddFixedWidthStringColumn(),
 * tableAddSingleRowBooleanColumn(),
 * tableAddSingleRowShortColumn(),
 * tableAddSingleRowIntegerColumn(),
 * tableAddSingleRowLongColumn(),
 * tableAddSingleRowFloatColumn(),
 * tableAddSingleRowDoubleColumn(),
 * tableAddSingleRowStringColumn(),
 */
void tableAddSingleRowByteColumn(JNIEnv* env, Table* table, unsigned char data)
{
	tableAddColumn(env, table, AIDA_BYTE_TYPE, &data, false);
}

/**
 * Add a short column to a Table with only one row.
 *
 * @note
 * The framework will release all memory associated with
 * a Table when you return from your API implementation.
 *
 * @param env            The JNI environment.  Used in all functions involving JNI.
 * @param table          The Table to add the column to.
 * @param data           The data to add to this column, a `short`.
 *
 * @see
 * tableCreate(),
 * tableAddColumn(),
 * tableAddStringColumn()
 * tableAddFixedWidthStringColumn(),
 * tableAddSingleRowBooleanColumn(),
 * tableAddSingleRowByteColumn(),
 * tableAddSingleRowIntegerColumn(),
 * tableAddSingleRowLongColumn(),
 * tableAddSingleRowFloatColumn(),
 * tableAddSingleRowDoubleColumn(),
 * tableAddSingleRowStringColumn(),
 */
void tableAddSingleRowShortColumn(JNIEnv* env, Table* table, short data)
{
	tableAddColumn(env, table, AIDA_SHORT_TYPE, &data, false);
}

/**
 * Add a integer column to a Table with only one row.
 *
 * @note
 * The framework will release all memory associated with
 * a Table when you return from your API implementation.
 *
 * @param env            The JNI environment.  Used in all functions involving JNI.
 * @param table          the Table to add the column to.
 * @param data           the data to add to this column, an `int`
 *
 * @see
 * tableCreate(),
 * tableAddColumn(),
 * tableAddStringColumn()
 * tableAddFixedWidthStringColumn(),
 * tableAddSingleRowBooleanColumn(),
 * tableAddSingleRowByteColumn(),
 * tableAddSingleRowShortColumn(),
 * tableAddSingleRowLongColumn(),
 * tableAddSingleRowFloatColumn(),
 * tableAddSingleRowDoubleColumn(),
 * tableAddSingleRowStringColumn(),
 */
void tableAddSingleRowIntegerColumn(JNIEnv* env, Table* table, int data)
{
	tableAddColumn(env, table, AIDA_INTEGER_TYPE, &data, false);
}

/**
 * Add a long column to a Table with only one row
 *
 *
 * @note
 * The framework will release all memory associated with
 * a Table when you return from your API implementation.
 *
 * @param env            The JNI environment.  Used in all functions involving JNI.
 * @param table          the Table to add the column to.
 * @param data           the data to add to this column.  A `long` value
 *
 * @see
 * tableCreate(),
 * tableAddColumn(),
 * tableAddStringColumn()
 * tableAddFixedWidthStringColumn(),
 * tableAddSingleRowBooleanColumn(),
 * tableAddSingleRowByteColumn(),
 * tableAddSingleRowShortColumn(),
 * tableAddSingleRowIntegerColumn(),
 * tableAddSingleRowFloatColumn(),
 * tableAddSingleRowDoubleColumn(),
 * tableAddSingleRowStringColumn(),
 */
void tableAddSingleRowLongColumn(JNIEnv* env, Table* table, long data)
{
	tableAddColumn(env, table, AIDA_LONG_TYPE, &data, false);
}

/**
 * Add a float column to a Table with only one row
 *
 * @note
 * The framework will release all memory associated with
 * a Table when you return from your API implementation.
 *
 * @param env            The JNI environment.  Used in all functions involving JNI.
 * @param table          The Table to add the column to.
 * @param data           The data to add to this column, a `float`.
 * @param ieeeFloat	     True if the data is in ieee format, otherwise the given floating point
 * 						 number is converted from VMS to ieee format.
 *
 * @see
 * tableCreate(),
 * tableAddColumn(),
 * tableAddStringColumn()
 * tableAddFixedWidthStringColumn(),
 * tableAddSingleRowBooleanColumn(),
 * tableAddSingleRowByteColumn(),
 * tableAddSingleRowShortColumn(),
 * tableAddSingleRowIntegerColumn(),
 * tableAddSingleRowLongColumn(),
 * tableAddSingleRowDoubleColumn(),
 * tableAddSingleRowStringColumn(),
 */
void tableAddSingleRowFloatColumn(JNIEnv* env, Table* table, float data, bool ieeeFloat)
{
	tableAddColumn(env, table, AIDA_FLOAT_TYPE, &data, ieeeFloat);
}

/**
 * Add a double column to a Table with only one row.
 *
 * @note
 * The framework will release all memory associated with
 * a Table when you return from your API implementation.
 *
 * @param env            The JNI environment.  Used in all functions involving JNI.
 * @param table          The Table to add the column to.
 * @param data           The data to add to this column, a `double`.
 * @param ieeeDouble	 True if the data is in ieee format, otherwise the double precision floating point
 * 						 number is converted from VMS to ieee format.
 *
 * @see
 * tableCreate(),
 * tableAddColumn(),
 * tableAddStringColumn()
 * tableAddFixedWidthStringColumn(),
 * tableAddSingleRowBooleanColumn(),
 * tableAddSingleRowByteColumn(),
 * tableAddSingleRowShortColumn(),
 * tableAddSingleRowIntegerColumn(),
 * tableAddSingleRowLongColumn(),
 * tableAddSingleRowFloatColumn(),
 * tableAddSingleRowStringColumn(),
 */
void tableAddSingleRowDoubleColumn(JNIEnv* env, Table* table, double data, bool ieeeDouble)
{
	tableAddColumn(env, table, AIDA_DOUBLE_TYPE, &data, ieeeDouble);
}

/**
 * Add a string column to a Table with only one row.  This is a shortcut
 * Table function that simplifies adding a string to a Table with only one row.
 *
 * @note
 * The framework will release all memory associated with
 * a Table when you return from your API implementation.
 *
 * @param env            The JNI environment.  Used in all functions involving JNI.
 * @param table          the Table to add the column to.
 * @param data           the data to add to this column.  A single string.
 *
 * @see
 * tableCreate(),
 * tableAddColumn(),
 * tableAddStringColumn()
 * tableAddFixedWidthStringColumn(),
 * tableAddSingleRowBooleanColumn(),
 * tableAddSingleRowByteColumn(),
 * tableAddSingleRowShortColumn(),
 * tableAddSingleRowIntegerColumn(),
 * tableAddSingleRowLongColumn(),
 * tableAddSingleRowFloatColumn(),
 * tableAddSingleRowDoubleColumn(),
 */
void tableAddSingleRowStringColumn(JNIEnv* env, Table* table, char* data)
{
	tableAddStringColumn(env, table, &data);
}

/**
 * See if there is a ieee float value stored in arguments.  If so set target
 *
 * @param arguments the arguments to scan for the float.
 * @param path path to look for in arguments. The path can be a simple path that is just the argument name.
 *             But it can also use dot notation to reference the value deep inside json.
 *             e.g. `"json.foo.bar"`
 * @param target the place to store the float if found.
 * @return `EXIT_SUCCESS` if found `EXIT_FAILURE` if not.
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
 * See if there is a ieee double value stored in arguments.  If so set target.
 *
 * @param arguments the arguments to scan for the double.
 * @param path path to look for in arguments. The path can be a simple path that is just the argument name.
 *             But it can also use dot notation to reference the value deep inside json.
 *             e.g. `"json.foo.bar"`
 * @param target the place to store the double.
 * @return `EXIT_SUCCESS` if found `EXIT_FAILURE` if not.
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
 * and set target.
 *
 * @param arguments the arguments to scan for the float array.
 * @param path path to look for in arguments. The path can be a simple path that is just the argument name.
 *             But it can also use dot notation to reference the value deep inside json.
 *             e.g. `"jsonArray[1].bar"`
 * @param target the place to store the float array.
 * @return `EXIT_SUCCESS` if found `EXIT_FAILURE` if not.
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
 * and set target.
 *
 * @param arguments the arguments to scan for the double array.
 * @param path path to look for in arguments. The path can be a simple path that is just the argument name.
 *             But it can also use dot notation to reference the value deep inside json.
 *             e.g. `"jsonArray[1].bar"`
 * @param target the place to store the double array.
 * @return `EXIT_SUCCESS` if found `EXIT_FAILURE` if not.
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
 * by searching for one with the given path.
 *
 * @param arguments the arguments to scan for the floating point value.
 * @param path path to look for in arguments. The path can be a simple path that is just the argument name.
 *             But it can also use dot notation to reference the value deep inside json.
 *             e.g. `"jsonArray[1].bar"`
 * @return NULL of not found.
 */
static FloatingPointValue* getFloatingPointValue(Arguments* arguments, char* path)
{
	for (int i = 0; i < arguments->floatingPointValuesCount; i++) {
		if (strcasecmp(path, arguments->floatingPointValues[i].path) == 0) {
			return &arguments->floatingPointValues[i];
		}
	}
	return NULL;
}

/**
 * Get an array of floats by searching for an array rooted at path.
 * Space for the array will be allocated if any are found (must be freed by caller).
 *
 * @param arguments the arguments to scan for the floating point array.
 * @param path path to look for in arguments. The path can be a simple path that is just the argument name.
 *             But it can also use dot notation to reference the value deep inside json.
 *             e.g. `"jsonArray[1].bar"`
 * @return NULL of not found.
 */
static float* getFloatArray(Arguments* arguments, char* path, unsigned int* elementCount)
{
	return (float*)_getFloatArray(arguments, path, true, elementCount);
}

/**
 * Get an array of doubles by searching for an array rooted at path
 * Space for the array will be allocated if any are found (must be freed by caller)
 *
 * @param arguments the arguments to scan for the double precision floating point array.
 * @param path path to look for in arguments. The path can be a simple path that is just the argument name.
 *             But it can also use dot notation to reference the value deep inside json.
 *             e.g. `"jsonArray[1].bar"`
 * @return NULL of not found.
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
		// If the floating point found is the same we're looking for ...
		if (floatingPointValue->isFloat == forFloat) {
			if (numberOfFloatAllocated == 0) { // allocate space if none allocated
				numberOfFloatAllocated += MIN_FLOAT_ALLOCATIONS;
				theArray = calloc(numberOfFloatAllocated, forFloat ? sizeof(float) : sizeof(double));
				if (theArray == NULL) {
					return NULL;
				}
			} else if (numberOfFloatAllocated
					<= numberOfFloatsFound) { // reallocate space is allocated space is used up
				numberOfFloatAllocated += MIN_FLOAT_ALLOCATIONS;
				theArray = realloc(theArray, numberOfFloatAllocated * (forFloat ? sizeof(float) : sizeof(double)));
			}

			// Get the floating point value and store it in the target array
			if (forFloat) {
				((float*)theArray)[numberOfFloatsFound] = floatingPointValue->value.floatValue;
			} else {
				((double*)theArray)[numberOfFloatsFound] = floatingPointValue->value.doubleValue;
			}
		}

		// Next element path
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
	if (strcasecmp(argumentName, "value") == 0) {
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
 * @param env            The JNI environment.  Used in all functions involving JNI
 * @param format 		 the format character of the format string
 * @param isArray 		 true if the array indicator is set
 * @param isLong 		 true if the long indicator is set
 * @param isShort 		 true if the short indicator is set
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
 * Make a single entry json_value array from a string.  Use getJsonRoot() to get a
 * pointer to the array element directly.
 *
 * @param stringValue the string to add as the single entry in the json_value array
 * @return the single entry json_value array
 */
Value asArrayValue(char* stringValue)
{
	char arrayValueToParse[
			strlen(stringValue) + 20]; // Length of string plus strlen => {"_array": ["_"]} <= plus a couple of bytes
	if (isdigit(*stringValue)) {
		sprintf(arrayValueToParse, "{\"_array\": [%s]}", stringValue);
	} else {
		sprintf(arrayValueToParse, "{\"_array\": [\"%s\"]}", stringValue);
	}
	Value value;
	value.type = AIDA_NO_TYPE;
	value.value.jsonValue = json_parse(arrayValueToParse, strlen(arrayValueToParse));
	if (value.value.jsonValue) {
		value.type = AIDA_JSON_TYPE;
	}
	return value;
}

/**
 * Return the corresponding array type of the given type.
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
 * Add a column to a Table specifying the type and element size
 *
 * @param table the Table to add the column to
 * @param aidaType the type of the column to add
 * @param elementSize the size of each element
 */
static void allocateTableColumn(JNIEnv* env, Table* table, Type aidaType, size_t elementSize)
{
	table->types[table->_currentColumn] = aidaType;
	table->ppData[table->_currentColumn] = ALLOCATE_MEMORY(env, table->rowCount * elementSize, "table data");
}

/**
 * Determine if the given string value is a boolean value
 * @param stringValue string value
 * @return boolean or -1 if it is not a boolean value
 */
static int getBooleanValue(char* stringValue)
{
	if (strcasecmp(stringValue, "y") == 0
			|| strcasecmp(stringValue, "1") == 0
			|| strcasecmp(stringValue, "yes") == 0
			|| strcasecmp(stringValue, "true") == 0
			|| strcasecmp(stringValue, "t") == 0) {
		return true;
	}
	if (strcasecmp(stringValue, "n") == 0
			|| strcasecmp(stringValue, "0") == 0
			|| strcasecmp(stringValue, "no") == 0
			|| strcasecmp(stringValue, "false") == 0
			|| strcasecmp(stringValue, "f") == 0) {
		return false;
	}
	return -1;
}
