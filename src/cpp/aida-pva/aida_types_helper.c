#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "slc_macros.h"           /* vmsstat_t, int2u, int4u, etc. */
#include "sysutil_proto.h"        /* for cvt_vms_to_ieee_flt */

#include "aida_types.h"
#include "aida_server_helper.h"

#define MAX_ARGUMENT_NAME_LENGTH 50

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

#define ARRAY_TARGET(_cType) &((_cType *)(*arrayPtr))[i]

#define ASCANF_SET_SCALAR(_format, _cType, _jsonType, _typeName, _target) \
{ \
    _cType* ptr = (_cType*)(_target); \
    if (!isJson) {                                    \
        sscanf(valueString, _format, ptr);\
    } else {                                                              \
        if (jsonRoot->type == json_integer) { \
            *ptr = (_cType)jsonRoot->u.integer; \
		} else if (jsonRoot->type == json_double) { \
            *ptr = (_cType)(jsonRoot->u.dbl); \
        } else {\
			json_value_free(arrayRoot); \
            aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, "can't convert argument to " _typeName );\
            return EXIT_FAILURE;\
        }\
    }\
}\

#define ASCANF_SET_BOOLEAN(_targetBoolean) \
{ \
    unsigned char* ptr = (unsigned char*)(_targetBoolean); \
    if (!isJson) {               \
        if (isdigit(*valueString)) {\
            int number;                  \
            sscanf(valueString, "%d", &number); \
            *ptr = (unsigned char)number; \
        } else  { \
            *ptr = (unsigned char)!( strcasecmp(valueString, "n") ==0           \
					|| strcasecmp(valueString, "0") ==0          \
					|| strcasecmp(valueString, "no") ==0          \
					|| strcasecmp(valueString, "false") ==0        \
					|| strcasecmp(valueString, "f")==0 );  \
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
			json_value_free(arrayRoot); \
            aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, "can't convert argument to boolean"); \
            return EXIT_FAILURE; \
        } \
    } \
}

#define ASCANF_SET_BYTE(_targetByte) \
{ \
    unsigned char* ptr = (unsigned char*)(_targetByte); \
    if (!isJson) { \
        *ptr = *valueString; \
    } else {   \
        if (jsonRoot->type == json_integer) { \
            *ptr = jsonRoot->u.integer; \
		} else if (jsonRoot->type == json_string && jsonRoot->u.string.length == 1) { \
            *ptr = *jsonRoot->u.string.ptr; \
        } else { \
			json_value_free(arrayRoot); \
            aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, "can't convert argument to byte"); \
            return EXIT_FAILURE; \
        } \
    } \
}

#define ASCANF_SET_STRING(_targetString) \
{ \
    char** ptr = (char**)(_targetString); \
    if (!isJson) { \
        *ptr = valueString; \
    } else {  \
		if (aidaType == AIDA_STRING_TYPE) { \
			nextStringPosition = malloc(jsonRoot->u.string.length+1); \
		}\
		if (jsonRoot->type == json_string) { \
			strcpy(nextStringPosition, jsonRoot->u.string.ptr); \
		} else if (jsonRoot->type == json_integer) {                \
			sprintf(nextStringPosition, "%d", jsonRoot->u.integer); \
		} else if (jsonRoot->type == json_double) {                 \
			sprintf(nextStringPosition, "%g", jsonRoot->u.dbl); \
		} else { \
			if (aidaType == AIDA_STRING_TYPE) { \
				free(nextStringPosition); \
			}\
			json_value_free(arrayRoot); \
			aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, "can't convert argument to string"); \
			return EXIT_FAILURE; \
		} \
		*ptr = nextStringPosition; \
		nextStringPosition+=strlen(nextStringPosition)+1; \
    } \
}

static void tableColumn(Table* table, int columnNumber, Type aidaType, size_t elementSize);
static Type tableArrayTypeOf(Type type);
static size_t tableElementSizeOfOf(Type type);
static int vavscanf(JNIEnv* env, Arguments* arguments, Value* value, const char* formatString, va_list argp);
static Type fToType(JNIEnv* env, char format, short isArray, short isLong, short isShort);
static int getJsonPathElements(char* fullJsonPath, char variableName[MAX_ARGUMENT_NAME_LENGTH], char** path);

/**
 * Convert Type to string name of Type e.g. AIDA_BOOLEAN_TYPE returns "BOOLEAN"
 *
 * @param type type
 * @return string
 */
jstring toTypeString(JNIEnv* env, Type type)
{
	switch (type) {
	case AIDA_BOOLEAN_TYPE :
		return (*env)->NewStringUTF(env, "BOOLEAN");
	case AIDA_BYTE_TYPE :
		return (*env)->NewStringUTF(env, "BYTE");
	case AIDA_SHORT_TYPE :
		return (*env)->NewStringUTF(env, "SHORT");
	case AIDA_INTEGER_TYPE :
		return (*env)->NewStringUTF(env, "INTEGER");
	case AIDA_LONG_TYPE :
		return (*env)->NewStringUTF(env, "LONG");
	case AIDA_FLOAT_TYPE :
		return (*env)->NewStringUTF(env, "FLOAT");
	case AIDA_DOUBLE_TYPE :
		return (*env)->NewStringUTF(env, "DOUBLE");
	case AIDA_STRING_TYPE :
		return (*env)->NewStringUTF(env, "STRING");
	case AIDA_BOOLEAN_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "BOOLEAN_ARRAY");
	case AIDA_BYTE_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "BYTE_ARRAY");
	case AIDA_SHORT_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "SHORT_ARRAY");
	case AIDA_INTEGER_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "INTEGER_ARRAY");
	case AIDA_LONG_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "LONG_ARRAY");
	case AIDA_FLOAT_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "FLOAT_ARRAY");
	case AIDA_DOUBLE_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "DOUBLE_ARRAY");
	case AIDA_STRING_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "STRING_ARRAY");
	case AIDA_TABLE_TYPE :
	default :
		return (*env)->NewStringUTF(env, "TABLE");
	}
}

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
 * - d : int * - extract an integer into the corresponding variable (see l & h below).
 * - u : unsigned int * - extract an unsigned integer into the corresponding variable (see l & h below)
 * - f  : float * - extract a floating point number (see l below)
 * - s : char * - extract a string of characters into allocated space and point the corresponding variable to it
 * - c : char * - extract a single character into the corresponding variable
 * - b : unsigned char * - extract a single byte into the corresponding variable, translate “true”, “false” etc
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
	// Remove white space and empty format strings, also don't mess with the strtok variable!
	char localFormatString[200];
	strcpy(localFormatString, formatString);
	char* formatSpecifier = strtok(localFormatString, "%");
	if (!formatSpecifier) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, "No format specifiers found");
		return EXIT_FAILURE;
	}

	// loop over format specifiers

	char* nextStringPosition; // used to store strings in space at end of pointers to strings
	for (; formatSpecifier != NULL; formatSpecifier = strtok(NULL, "%")) {
		char* thisFormatSpecifier = formatSpecifier;
		short lenFormatSpecifier = (short)strlen(thisFormatSpecifier);
		while (isspace(*thisFormatSpecifier)) {
			thisFormatSpecifier++;
			lenFormatSpecifier--;
		}
		if (!lenFormatSpecifier) {
			continue;
		}

		// extract the isRequiredFlag, format, isLong, isShort, and isArray
		short isRequiredFlag = 1, isLong = 0, isShort = 0, isArray = 0;
		char format = 0x0;

		if (lenFormatSpecifier > 1 && *thisFormatSpecifier == FORMAT_OPTIONAL_FLAG) {
			isRequiredFlag = 0;
			lenFormatSpecifier--;
			thisFormatSpecifier++;
		}

		if (lenFormatSpecifier > 1) {
			if (*thisFormatSpecifier == FORMAT_PREFIX_SHORT) {
				isShort = 1;
				lenFormatSpecifier--;
				thisFormatSpecifier++;
			} else if (*thisFormatSpecifier == FORMAT_PREFIX_LONG) {
				isLong = 1;
				lenFormatSpecifier--;
				thisFormatSpecifier++;
			}
		}

		if (lenFormatSpecifier > 0) {
			format = *thisFormatSpecifier++;
			lenFormatSpecifier--;
		}

		if (lenFormatSpecifier > 0 && *thisFormatSpecifier == FORMAT_SUFFIX_ARRAY) {
			isArray = 1;
		}

		// Invalid format
		if (!format) {
			aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION,
					"incorrect format string passed to ascanf() or avscanf()");
			return EXIT_FAILURE;
		}

		// get argumentName, and set isJson and isValue flags appropriately
		// Also get the valueString and json root
		char* argumentName = va_arg (argp, char *), * valueString = NULL;
		if (!argumentName) {
			aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION,
					"call to ascanf() missing variable to correspond to format");
			return EXIT_FAILURE;
		}

		short isJson = 0, isValue = 0;
		json_value* jsonRoot = NULL;
		json_type jsonType = json_none;

		if (strncasecmp(argumentName, "value", 5) == 0) {
			isValue = 1;
		}

		if (strchr(argumentName, '.') != NULL || strchr(argumentName, '[') != NULL) {
			isJson = 1;
		}

		// Get target variable pointer
		void* target = va_arg (argp, void *);
		void** arrayPtr = (void**)target;
		unsigned int* elementCount;
		if (isArray) {
			elementCount = va_arg (argp, unsigned int *);
			*elementCount = 0;
		}

		// Convert format, isArray, isLong, and isShort into an AIDA_TYPE
		Type aidaType = fToType(env, format, isArray, isLong, isShort);
		CHECK_EXCEPTION(EXIT_FAILURE);

		char variableName[MAX_ARGUMENT_NAME_LENGTH];
		char* path;
		if (!isValue) {
			if (!isJson) {
				Argument elementArgument = getArgument(*arguments, argumentName);
				if (!elementArgument.name) {
					if (isRequiredFlag) {
						aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
								"Missing required argument");
						return EXIT_FAILURE;
					} else {
						continue;
					}
				}
				valueString = elementArgument.value;
			} else {
				getJsonPathElements(argumentName, variableName, &path);
				Value elementValue = getNamedValue(env, *arguments, variableName);
				CHECK_EXCEPTION(EXIT_FAILURE);
				if (elementValue.type == AIDA_NO_TYPE) {
					if (isRequiredFlag) {
						aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
								"Missing required argument");
						return EXIT_FAILURE;
					} else {
						continue;
					}
				}
				jsonRoot = getJsonValue(elementValue, path);
				jsonType = jsonRoot->type;
			}
		} else {
			Value elementValue;
			if (value == NULL) {
				elementValue = getValue(env, *arguments);
				CHECK_EXCEPTION(EXIT_FAILURE);
				value = &elementValue;
			}

			if (value->type == AIDA_NO_TYPE) {
				if (isRequiredFlag) {
					aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
							"Missing required argument");
					return EXIT_FAILURE;
				} else {
					continue;
				}
			}

			if (!isJson) {
				valueString = value->value.stringValue;
			} else {
				getJsonPathElements(argumentName, variableName, &path);
				jsonRoot = getJsonValue(*value, path);
				jsonType = jsonRoot->type;
			}
		}

		// If this is an array type then parse out the array from the string into the jsonRoot.
		// Note that for strings we allocate enough memory for all the pointers and for the strings as well
		// stored contiguously afterwards so that the caller simply frees the pointer-to-pointer to strings
		json_value* arrayRoot;
		unsigned int arrayCount;
		if (isArray && !isJson) {
			jsonRoot = json_parse(valueString, strlen(valueString));
			isJson = 1;
			if (jsonRoot == NULL || jsonRoot->type != json_array) {
				json_value_free(jsonRoot);
				aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, "can't extract array from argument");
				return EXIT_FAILURE;
			}
			jsonType = jsonRoot->u.array.values[0]->type;
		}
		if (isArray) {
			arrayRoot = jsonRoot;
			arrayCount = arrayRoot->u.array.length;
		}

		// Now we have a valueString or jsonRoot, a target to put the data and the type we want to extract!

		// based on format process the argument and or value
		switch (aidaType) {
		case AIDA_BOOLEAN_TYPE: ASCANF_SET_BOOLEAN(target)
			break;
		case AIDA_BOOLEAN_ARRAY_TYPE: {
			*arrayPtr = calloc(arrayCount, sizeof(unsigned char));
			for (int i = 0; i < arrayCount; i++) {
				jsonRoot = arrayRoot->u.array.values[i];
				ASCANF_SET_BOOLEAN(ARRAY_TARGET(unsigned char))
			}
			break;
		}
		case AIDA_BYTE_TYPE: ASCANF_SET_BYTE(target)
			break;
		case AIDA_BYTE_ARRAY_TYPE: {
			*arrayPtr = calloc(arrayCount, sizeof(unsigned char));
			for (int i = 0; i < arrayCount; i++) {
				jsonRoot = arrayRoot->u.array.values[i];
				ASCANF_SET_BYTE(ARRAY_TARGET(unsigned char))
			}
			break;
		}
		case AIDA_SHORT_TYPE: ASCANF_SET_SCALAR("%hi", short, json_integer, "short", target)
			break;
		case AIDA_SHORT_ARRAY_TYPE: {
			*arrayPtr = calloc(arrayCount, sizeof(short));
			for (int i = 0; i < arrayCount; i++) {
				jsonRoot = arrayRoot->u.array.values[i];
				ASCANF_SET_SCALAR("%hi", short, json_integer, "short", ARRAY_TARGET(short))
			}
			break;
		}
		case AIDA_UNSIGNED_SHORT_TYPE: ASCANF_SET_SCALAR("%hu", unsigned short, json_integer, "unsigned short", target)
			break;
		case AIDA_UNSIGNED_SHORT_ARRAY_TYPE: {
			*arrayPtr = calloc(arrayCount, sizeof(unsigned short));
			for (int i = 0; i < arrayCount; i++) {
				jsonRoot = arrayRoot->u.array.values[i];
				ASCANF_SET_SCALAR("%hu", unsigned short, json_integer, "unsigned short", ARRAY_TARGET(unsigned short))
			}
			break;
		}
		case AIDA_INTEGER_TYPE: ASCANF_SET_SCALAR("%d", int, json_integer, "integer", target)
			break;
		case AIDA_INTEGER_ARRAY_TYPE: {
			*arrayPtr = calloc(arrayCount, sizeof(int));
			for (int i = 0; i < arrayCount; i++) {
				jsonRoot = arrayRoot->u.array.values[i];
				ASCANF_SET_SCALAR("%d", int, json_integer, "integer", ARRAY_TARGET(int))
			}
			break;
		}
		case AIDA_UNSIGNED_INTEGER_TYPE: ASCANF_SET_SCALAR("%u", unsigned int, json_integer, "unsigned integer", target)
			break;
		case AIDA_UNSIGNED_INTEGER_ARRAY_TYPE: {
			*arrayPtr = calloc(arrayCount, sizeof(unsigned int));
			for (int i = 0; i < arrayCount; i++) {
				jsonRoot = arrayRoot->u.array.values[i];
				ASCANF_SET_SCALAR("%d", unsigned int, json_integer, "unsigned integer", ARRAY_TARGET(unsigned int))
			}
			break;
		}
		case AIDA_LONG_TYPE: ASCANF_SET_SCALAR("%ld", long, json_integer, "long", target)
			break;
		case AIDA_LONG_ARRAY_TYPE: {
			*arrayPtr = calloc(arrayCount, sizeof(long));
			for (int i = 0; i < arrayCount; i++) {
				jsonRoot = arrayRoot->u.array.values[i];
				ASCANF_SET_SCALAR("%ld", long, json_integer, "long", ARRAY_TARGET(long))
			}
			break;
		}
		case AIDA_UNSIGNED_LONG_TYPE: ASCANF_SET_SCALAR("%lu", unsigned long, json_integer, "unsigned long", target)
			break;
		case AIDA_UNSIGNED_LONG_ARRAY_TYPE: {
			*arrayPtr = calloc(arrayCount, sizeof(unsigned long));
			for (int i = 0; i < arrayCount; i++) {
				jsonRoot = arrayRoot->u.array.values[i];
				ASCANF_SET_SCALAR("%lu", unsigned long, json_integer, "unsigned long", ARRAY_TARGET(unsigned long))
			}
			break;
		}
		case AIDA_FLOAT_TYPE: ASCANF_SET_SCALAR("%f", float, json_double, "float", target)
			break;
		case AIDA_FLOAT_ARRAY_TYPE: {
			*arrayPtr = calloc(arrayCount, sizeof(float));
			for (int i = 0; i < arrayCount; i++) {
				jsonRoot = arrayRoot->u.array.values[i];
				ASCANF_SET_SCALAR("%f", float, json_double, "float", ARRAY_TARGET(float))
			}
			break;
		}
		case AIDA_DOUBLE_TYPE: ASCANF_SET_SCALAR("%lf", double, json_double, "double", target)
			break;
		case AIDA_DOUBLE_ARRAY_TYPE: {
			*arrayPtr = calloc(arrayCount, sizeof(double));
			for (int i = 0; i < arrayCount; i++) {
				jsonRoot = arrayRoot->u.array.values[i];
				ASCANF_SET_SCALAR("%lf", double, json_double, "double", ARRAY_TARGET(double))
			}
			break;
		}
		case AIDA_STRING_TYPE: ASCANF_SET_STRING(target)
			break;
		case AIDA_STRING_ARRAY_TYPE: {
			// Enough space for pointers and data
			size_t pointerSpace = arrayCount * sizeof(char*);
			*arrayPtr = malloc(pointerSpace + totalStingLengthOf(arrayRoot) + arrayCount + 1);
			nextStringPosition = ((char*)*arrayPtr) + pointerSpace;
			for (int i = 0; i < arrayCount; i++) {
				jsonRoot = arrayRoot->u.array.values[i];
				ASCANF_SET_STRING(ARRAY_TARGET(char *))
			}
			break;
		}
		case AIDA_NO_TYPE:
		default:
			return (EXIT_FAILURE); // Exception was raised
		}

		if (isArray) {
			*elementCount = arrayCount;
		}

		// Free any json that has been parsed in this iteration unless it is from the value which will be freed later
		if (!isValue && arrayRoot) {
			json_value_free(arrayRoot);
		}
	}

	return EXIT_SUCCESS;
}

/**
 * Break a json path into the initial variable name and the remaining path
 *
 * @param fullJsonPath
 * @param variableName
 * @param path
 * @return
 */
static int getJsonPathElements(char* fullJsonPath, char variableName[MAX_ARGUMENT_NAME_LENGTH], char** path)
{
	char* firstDot = strchr(fullJsonPath, '.');
	char* firstParen = strchr(fullJsonPath, '[');
	char* relativeJsonPath = (firstDot == NULL || firstParen == NULL) ?
							 MAX(firstDot, firstParen) :
							 MIN(firstParen, firstDot);
	int lenRootArgument = (int)(relativeJsonPath - fullJsonPath);
	if (lenRootArgument < MAX_ARGUMENT_NAME_LENGTH) {
		if (*relativeJsonPath == '.') {
			relativeJsonPath++; // skip opening dot
		}
		memcpy(variableName, fullJsonPath, lenRootArgument);
		variableName[lenRootArgument] = 0x0;
		*path = relativeJsonPath;
	} else {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

/**
 * Depending on the combination of options specified in an element of a format string determine the target aida type
 *
 * @param env
 * @param format
 * @param isArray
 * @param isLong
 * @param isShort
 * @return
 */
static Type fToType(JNIEnv* env, char format, short isArray, short isLong, short isShort)
{
	if (!isArray) {
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
	} else {
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
	}

	aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, "incorrect format string passed to ascanf() or avscanf()");
	return AIDA_NO_TYPE;
}

/**
 * Make a table for return.  Specify the number of columns then call tableAddColumn() and tableAddStringColumn() to add
 * columns before returning the table
 *
 * @param env
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

	// Allocate space for the table columns
	table.ppData = calloc(columns, sizeof(void*));
	if (!table.ppData) {
		char errorString[BUFSIZ];
		sprintf(errorString, "Unable to allocate memory for table: %ld", columns * sizeof(void*));
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, errorString);
		return table;
	}

	// Allocate space for the column types
	table.types = calloc(columns, sizeof(Type*));
	if (!table.types) {
		free(table.ppData);
		char errorString[BUFSIZ];
		sprintf(errorString, "Unable to allocate memory for table types: %ld", columns * sizeof(Type*));
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, errorString);
		return table;
	}

	table.rowCount = rows;
	table.columnCount = columns;
	return table;
}

/**
 * This reads data from an that is itself a list of pointers to strings
 * We allocate just enough space to store the strings in our table
 *
 * @param env
 * @param table
 * @param data
 */
void tableAddStringColumn(JNIEnv* env, Table* table, char** data)
{
	tableAddColumn(env, table, AIDA_STRING_ARRAY_TYPE, data);
	CHECK_EXCEPTION_VOID

	// allocate data for each string too
	char** stringArray = table->ppData[table->_currentColumn];
	for (int row = 0; row < table->rowCount; row++, data++) {
		unsigned long len = strlen(*data);
		stringArray[row] = malloc(len + 1);
		strncpy(stringArray[row], *data, len + 1);
	}

	table->_currentColumn++;
}

/**
 * This reads data from an allocated space that is rows * width with each string occupying width characters
 * Though the strings are null terminated if there is space, there is no guarantee so an exact number of
 * bytes is copied.  Each string in the table is allocated maximally.
 *
 * @param env
 * @param table
 * @param data
 * @param width
 */
void tableAddFixedWidthStringColumn(JNIEnv* env, Table* table, void* data, int width)
{
	tableAddColumn(env, table, AIDA_STRING_ARRAY_TYPE, data);
	CHECK_EXCEPTION_VOID

	// allocate data for each string too
	char** stringArray = table->ppData[table->_currentColumn];
	char* dataPointer = (char*)data;
	for (int row = 0; row < table->rowCount; row++, dataPointer += width) {
		stringArray[row] = malloc(width + 1);
		strncpy(stringArray[row], dataPointer, width);
		stringArray[row][width] = 0x0;
	}

	table->_currentColumn++;
}

void tableAddColumn(JNIEnv* env, Table* table, Type type, void* data)
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
	tableColumn(table, table->_currentColumn, type, tableElementSizeOfOf(type));

	// Rest of processing for strings is done in addStringColumn
	if (type == AIDA_STRING_ARRAY_TYPE) {
		return;
	}

	// Convert float values if float array
	if (type == AIDA_FLOAT_ARRAY_TYPE) {
		int2u M = (int2u)table->rowCount;
		float* floatArray = ((float*)(table->ppData[table->_currentColumn]));
		CVT_VMS_TO_IEEE_FLT(floatArray, floatArray, &M);
	}

	// Convert double values if double array
	if (type == AIDA_DOUBLE_ARRAY_TYPE) {
		int2u M = (int2u)table->rowCount;
		double* doubleArray = ((double*)(table->ppData[table->_currentColumn]));
		CVT_VMS_TO_IEEE_DBL(doubleArray, doubleArray, &M);
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

void tableAddSingleRowFloatColumn(JNIEnv* env, Table* table, float data)
{
	tableAddColumn(env, table, AIDA_FLOAT_TYPE, &data);
}

void tableAddSingleRowLongColumn(JNIEnv* env, Table* table, long data)
{
	tableAddColumn(env, table, AIDA_LONG_TYPE, &data);
}

void tableAddSingleRowBooleanColumn(JNIEnv* env, Table* table, unsigned char data)
{
	tableAddColumn(env, table, AIDA_BOOLEAN_TYPE, &data);
}

void tableAddSingleRowByteColumn(JNIEnv* env, Table* table, unsigned char data)
{
	tableAddColumn(env, table, AIDA_BYTE_TYPE, &data);
}

void tableAddSingleRowShortColumn(JNIEnv* env, Table* table, short data)
{
	tableAddColumn(env, table, AIDA_SHORT_TYPE, &data);
}

void tableAddSingleRowIntegerColumn(JNIEnv* env, Table* table, int data)
{
	tableAddColumn(env, table, AIDA_INTEGER_TYPE, &data);
}

void tableAddSingleRowDoubleColumn(JNIEnv* env, Table* table, double data)
{
	tableAddColumn(env, table, AIDA_DOUBLE_TYPE, &data);
}

void tableAddSingleRowStringColumn(JNIEnv* env, Table* table, char* data)
{
	tableAddStringColumn(env, table, &data);
}

static void tableColumn(Table* table, int columnNumber, Type aidaType, size_t elementSize)
{
	table->types[columnNumber] = aidaType;
	table->ppData[columnNumber] = calloc(table->rowCount, elementSize);
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

float valueGetFloat(Value value)
{
	float floatValue;
	sscanf(value.value.stringValue, "%f", &floatValue);
	return floatValue;
}

short valueGetShort(Value value)
{
	short shortValue;
	sscanf(value.value.stringValue, "%hi", &shortValue);
	return shortValue;
}

/**
 * Convert Layout to string name of Layout e.g. AIDA_ROW_MAJOR_LAYOUT returns "ROW_MAJOR"
 * @param layout layout
 * @return string
 */
jstring toLayoutString(JNIEnv* env, Layout layout)
{
	switch (layout) {
	case AIDA_ROW_MAJOR_LAYOUT:
		return (*env)->NewStringUTF(env, "ROW_MAJOR");
	case AIDA_COLUMN_MAJOR_LAYOUT:
	default:
		return (*env)->NewStringUTF(env, "COLUMN_MAJOR");
	}
}
