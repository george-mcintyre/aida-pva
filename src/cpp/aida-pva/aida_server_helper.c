#include <string.h>
#include <stsdef.h>               /* $VMS.. status manipulation */
#include <ssdef.h>                /* SS$_NORMAL and other VMS general codes */
#include <descrip.h>              /*  for definition of $DESCRIPTOR  */
#include <ctype.h>                /* isalnum, ispunct */
#include <stdlib.h>

#include "aida_types.h"
#include "json.h"
#include "aida_server_helper.h"

#include "slc_macros.h"    /* vmsstat_t, int2u, int4u, etc. */

static json_value* navigateToArrayElement(json_value* jsonValue, int index);
static json_value* navigateToObjectElement(json_value* jsonValue, const char* name);
static json_value* processArrayReference(json_value* jsonValue, const char* arrayRef);
static Value _getNamedValue(JNIEnv* env, Arguments arguments, char* name, bool forArray);

/**
 * To log any non-OS exceptions and throw back to java
 *
 * The exception is formatted in a standard way with the optionally supplied message
 * The exception is always assumed to be from the edu.stanford.slac.except package
 *
 * @param env
 * @param exception
 * @param message
 */
void aidaThrowNonOsException(JNIEnv* env, char* exception, const char* message)
{
	fprintf(stderr, "AIDA Exception: %s: %s\n", exception, message);
	aidaThrow(env, 1, exception, message);
}

/**
 * To log any exceptions and throw back to java
 *
 * The exception is formatted in a standard way using the VMS status code and its associated message
 * and the optionally supplied message
 * The exception is always assumed to be from the edu.stanford.slac.except package
 *
 * @param env
 * @param status
 * @param exception
 * @param message
 */
void aidaThrow(JNIEnv* env, vmsstat_t status, char* exception, const char* message)
{
	// Clear any exception that may be in the process of being thrown (unlikely)
	if ((*env)->ExceptionCheck(env)) {
		(*env)->ExceptionClear(env);
	}

	char vmsErrorMessage[BUFSIZ] = { '\0' };
	$DESCRIPTOR(MESSAGE, vmsErrorMessage);
	struct dsc$descriptor errorMessageDescriptor = { BUFSIZ, DSC$K_DTYPE_T, DSC$K_CLASS_S, (char*)&vmsErrorMessage };

	//	Get the message text associated with the VMS message code. if the cause is an OS error
	if (!SUCCESS(status)) {
		ERRTRANSLATE(&status, &errorMessageDescriptor);
		strncat(errorMessageDescriptor.dsc$a_pointer, "; ",
				MIN(strlen("; "), BUFSIZ - strlen(errorMessageDescriptor.dsc$a_pointer)));
	}

	// Add exception
	strncat(errorMessageDescriptor.dsc$a_pointer, exception,
			MIN(strlen(exception), BUFSIZ - strlen(errorMessageDescriptor.dsc$a_pointer)));

	// If a message is specified then append it to the vms message string
	if (message) {
		strncat(errorMessageDescriptor.dsc$a_pointer, "; ",
				MIN(strlen("; "), BUFSIZ - strlen(errorMessageDescriptor.dsc$a_pointer)));
		strncat(errorMessageDescriptor.dsc$a_pointer, message,
				MIN(strlen(message), BUFSIZ - strlen(errorMessageDescriptor.dsc$a_pointer)));
	}

	// Create the fully qualified java class name of the exception to throw
	char classToCreate[BUFSIZ] = "edu/stanford/slac/except/";
	strcat (classToCreate, exception);

	// Create the java exception class
	jclass exceptionClass;
	exceptionClass = (*env)->FindClass(env, classToCreate);
	if (!exceptionClass) {
		fprintf(stderr, "FATAL: Failed to create object of class: %s\n", classToCreate);
		exit((int)status);
	}

	// 	Throw the given exception to Java server code, giving the
	//	VMS error text and supplied message as the exception text.
	(*env)->ThrowNew(env, exceptionClass, errorMessageDescriptor.dsc$a_pointer);
}

/**
 * Check if a string ends with another string
 * @param str
 * @param suffix
 * @return true if string ends with suffix
 */
int endsWith(const char* str, char* suffix)
{
	if (!str || !suffix) {
		return false;
	}
	size_t lenstr = strlen(str);
	size_t lenSuffix = strlen(suffix);
	if (lenSuffix > lenstr)
		return false;
	return !strncasecmp(str + lenstr - lenSuffix, suffix, lenSuffix);
}

/**
 * Check if a string starts with another string
 * @param str
 * @param prefix
 * @returns true if string starts with prefix
 */
int startsWith(const char* str, char* prefix)
{
	if (!str || !prefix) {
		return false;
	}
	size_t lenstr = strlen(str);
	size_t lenPrefix = strlen(prefix);
	if (lenPrefix > lenstr)
		return false;
	return !strncasecmp(str, prefix, lenPrefix);
}

/**
 * Get a named argument
 * @param arguments arguments
 * @param name name
 * @return Argument
 */
Argument getArgument(Arguments arguments, char* name)
{
	Argument noArgument;
	memset(&noArgument, 0, sizeof(Argument));

	for (int i = 0; i < arguments.argumentCount; i++) {
		Argument argument = arguments.arguments[i];
		if (!strcasecmp(argument.name, name)) {
			if (strlen(argument.value) > 0) {
				return argument;
			}
		}
	}
	return noArgument;
}

static Value _getNamedValue(JNIEnv* env, Arguments arguments, char* name, bool forArray)
{
	Value value;
	value.type = AIDA_NO_TYPE;

	Argument valueArgument = getArgument(arguments, name);
	if (valueArgument.name && valueArgument.value) {
		// Get value to parse and trim leading space
		char* valueToParse = valueArgument.value;
		while (isspace(*valueToParse)) {
			valueToParse++;
		}

		// Json arrays can only be parsed by this parser by wrapping them in a json object, so we always
		// create {"_array": [ ... ]} and when pulling out values we always replace
		// the element "_array" by its value
		char arrayValueToParse[strlen(valueToParse + 30)];

		if (*valueToParse == '[') {
			sprintf(arrayValueToParse, "{\"_array\": %s}", valueToParse);
			valueToParse = arrayValueToParse;
		} else if (forArray) {
			if (isdigit(*valueToParse)) {
				sprintf(arrayValueToParse, "{\"_array\": [%s]}", valueToParse);
			} else {
				sprintf(arrayValueToParse, "{\"_array\": [\"%s\"]}", valueToParse);
			}
			valueToParse = arrayValueToParse;
		}

		// If this is a json string then parse it otherwise just extract the string
		if (*valueToParse == '{') {
			value.value.jsonValue = json_parse(valueToParse, strlen(valueToParse));
			if (value.value.jsonValue) {
				value.type = AIDA_JSON_TYPE;
			} else {
				aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
						"Unable to parse supplied JSON value string");
			}
		} else {
			value.type = AIDA_STRING_TYPE;
			value.value.stringValue = valueToParse;
		}
	}

	return value;
}

/**
 * Get value from a named  argument in the provided arguments structure.
 *
 * @param env env
 * @param arguments provided arguments structure
 * @param name provided name
 * @return the extracted Value
 */
Value getNamedValue(JNIEnv* env, Arguments arguments, char* name)
{
	return _getNamedValue(env, arguments, name, false);
}

/**
 * Get value from a named  argument in the provided arguments structure.
 *
 * @param env env
 * @param arguments provided arguments structure
 * @param name provided name
 * @return the extracted Value
 */
Value getNamedArrayValue(JNIEnv* env, Arguments arguments, char* name)
{
	return _getNamedValue(env, arguments, name, true);
}

/**
 * Get boolean argument
 * @param argument argument
 * @return boolean
 */
int getBooleanArgument(Argument argument)
{
	return strlen(argument.value) &&
			strcasecmp(argument.value, "false") != 0 &&
			strcmp(argument.value, "0") != 0;
}

/**
 * Get byte argument
 * @param argument argument
 * @return byte
 */
char getByteArgument(Argument argument)
{
	int scannedValue = 0;

	if (!strncmp("0x", argument.value, 2)) {
		sscanf(argument.value, "%x", &scannedValue);
	} else {
		sscanf(argument.value, "%d", &scannedValue);
	}

	return (char)scannedValue;
}

/**
 * Get short argument
 * @param argument argument
 * @return short
 */
short getShortArgument(Argument argument)
{
	short item = 0;
	sscanf(argument.value, "%hi", &item);
	return item;
}

/**
 * Get integer argument
 * @param argument argument
 * @return int
 */
int getIntegerArgument(Argument argument)
{
	int item = 0;
	sscanf(argument.value, "%d", &item);
	return item;
}

/**
 * Get long argument
 * @param argument argument
 * @return long
 */
long getLongArgument(Argument argument)
{
	long item = 0;
	sscanf(argument.value, "%ld", &item);
	return item;
}

/**
 * Get float argument
 * @param argument argument
 * @return float
 */
float getFloatArgument(Argument argument)
{
	float item = 0;
	sscanf(argument.value, "%f", &item);
	return item;
}

/**
 * Get double argument
 * @param argument argument
 * @return double
 */
double getDoubleArgument(Argument argument)
{
	double item = 0.0;
	sscanf(argument.value, "%lf", &item);
	return item;
}

/**
 * Print a value to standard output
 * @param value
 */
void printValue(Value value)
{
	if (value.type == AIDA_STRING_TYPE) {
		printf("%s\n", value.value.stringValue);
	} else if (value.type == AIDA_JSON_TYPE) {
		process_value(value.value.jsonValue, 0);
	}
}

/**
 * Get the json value from the given value identified by the path
 *
 * @param value the given value
 * @param path is an absolute reference to the element within the json of the given value. e.g. root.collection.[0].name
 * @return pointer to the json_value
 */
json_value* getJsonValue(Value* value, char* passedInPath)
{
	if (value->type != AIDA_JSON_TYPE || !value->value.jsonValue) {
		return NULL;
	}

	json_value* jsonValue = getJsonRoot(value->value.jsonValue);

	// If there is no path then we already have the json value
	if (!passedInPath || strlen(passedInPath) == 0) {
		return jsonValue;
	}

	// can't use const for strtok
	char path[strlen(passedInPath) + 1];
	strcpy(path, passedInPath);

	int len;
	char name[256];
	char* arrayRef;

	// Extract the first token
	char* token = strtok(path, ".");

	while (token) {
		if (*token == '[') {
			// if array is at top level
			jsonValue = processArrayReference(jsonValue, token);
		} else if ((arrayRef = strstr(token, "["))) {
			// element followed by array ref
			len = (int)(arrayRef - token);
			strncpy(name, token, len);
			name[len] = 0x0;
			jsonValue = navigateToObjectElement(jsonValue, name);

			jsonValue = processArrayReference(jsonValue, arrayRef);
		} else {
			// element only
			jsonValue = navigateToObjectElement(jsonValue, token);
		}

		// Next token
		token = strtok(NULL, ".");
	}

	return jsonValue;
}

/**
 * 	Skip root element if it is _array otherwise return unchanged
 *
 * 	This is because our json parser can't process arrays at the top level and so we insert
 * 	an object at the top level with an "_array" element if we find an array at the top level
 *
 * @param jsonValue
 * @return
 */
json_value* getJsonRoot(json_value* jsonValue)
{
	if (jsonValue->type == json_object && jsonValue->u.object.length == 1
			&& strcmp(jsonValue->u.object.values[0].name, "_array") == 0) {
		jsonValue = jsonValue->u.object.values[0].value;
	}
	return jsonValue;
}

/**
 * Process any array references.  Because arrays can be directly nested within other array we
 * allow up to 4 levels of nesting without any intervening object
 * @param jsonValue
 * @param arrayRef
 * @return the new position or unchanged if any  index is out of bounds or no indexes are found
 */
static json_value* processArrayReference(json_value* jsonValue, const char* arrayRef)
{
	int index[4];  // Up to 4 levels of array with no object
	int count = sscanf(arrayRef, "[%d][%d][%d][%d]", &index[0], &index[1], &index[2], &index[3]);

	while (count > 0) {
		jsonValue = navigateToArrayElement(jsonValue, index[--count]);
	}
	return jsonValue;
}

/**
 * Using the given name navigate to the named element from the current position in the json_value
 * @param jsonValue
 * @param name
 * @return the new position or unchanged if the name is not found
 */
static json_value* navigateToObjectElement(json_value* jsonValue, const char* name)
{
	if (jsonValue->type == json_object) {
		for (int i = 0; i < jsonValue->u.object.length; i++) {
			if (strcasecmp(name, jsonValue->u.object.values[i].name) == 0) {
				jsonValue = jsonValue->u.object.values[i].value;
				break;
			}
		}
	}
	return jsonValue;
}
/**
 * Using the given index navigate to the zero based index'th element of the array at the current position
 * @param jsonValue
 * @param index
 * @return the new position or unchanged if the index is out of bounds
 */
static json_value* navigateToArrayElement(json_value* jsonValue, int index)
{
	if (jsonValue->type == json_array) {
		if (jsonValue->u.array.length > index) {
			jsonValue = jsonValue->u.array.values[index];
		}
	}
	return jsonValue;
}

/**
 * Get the Display group name from a URI.  This is everything up until the last colon
 * @param uri
 * @param groupName
 * @return
 */
int groupNameFromUri(const char* uri, char groupName[])
{
	strcpy(groupName, uri);
	char* groupNameEnd = strrchr(groupName, ':');
	if (groupNameEnd) {
		*groupNameEnd = 0x0;
	}
	return EXIT_SUCCESS;
}

/**
 * Get secondary from pseudo secondary (containing a colon) number from URI
 *  e.g. `BD01:BEND:BDES`  => `BEND` as int4u
 *
 * @param uri the uri
 * @param secn pointer to an int to store the secondary as a number
 */
void secnFromUri(const char* uri, int4u* secn)
{
	char uriCopy[strlen(uri)+1];
	strcpy(uriCopy, uri);
	char* secondary = strrchr(uriCopy, ':');
	if ( secondary ) {
		uriCopy[secondary-uriCopy] = 0x0;
		secondary = strrchr(uriCopy, ':');
		if ( secondary ) {
			memcpy(secn, secondary+1, sizeof(int4u));
			return;
		}
	}
	fprintf(stderr, "Warning: Found corrupt URI when trying to extract secn: %s\n", uri);
	*secn = 0;
}

/**
 * Get secondary from URI.  Just points into the URI so don't go messing with it
 * @param uri the uri
 * @param secn pointer to an int to store the secondary as a number
 */
const char* secondaryFromUri(const char* uri)
{
	char* secondary = strrchr(uri, ':');
	if (!secondary) {
		fprintf(stderr, "Secondary not found in uri: %s\n", uri);
		return uri;
	}
	return secondary + 2;
}

/**
 * Get the pmu part of a URI
 * @param uri the uri
 * @param pmuString the pre-allocated space to store the pmu string
 */
void pmuStringFromUri(const char* uri, char pmuString[MAX_PMU_LEN])
{
	unsigned long pmuEnd = strrchr(uri, ':') - uri;
	if (pmuEnd >= MAX_URI_LEN) {
		pmuEnd = MAX_URI_LEN - 1;
	}
	strncpy(pmuString, uri, pmuEnd);
	pmuString[pmuEnd] = 0x0;
}

/**
 * Get primary, micro and unit from a device name
 *
 * @param device
 * @param primary
 * @param micro
 * @param unit
 */
void pmuFromDeviceName(char* device, char* primary, char* micro, int4u* unit)
{
	// Copy each part to the provided variables
	char* nextPart = strtok(device, ":");
	if (nextPart) {
		memcpy(primary, nextPart, PRIM_LEN);
		nextPart = strtok(NULL, ":");
		if (nextPart) {
			memcpy(micro, nextPart, MICRO_LEN);
			nextPart = strtok(NULL, ":");
			if (nextPart) {
				*unit = (int4u)atol(nextPart);
			}
		}
	}
}

/**
 * Convert all URIs to slac names before making queries
 * @param slcName
 * @param uri
 * @return
 */
void uriToSlcName(char slcName[MAX_URI_LEN], const char* uri)
{
	char* separator = strrchr(uri, ':');
	if (separator) {
		memcpy(slcName, uri, separator - uri);
		memcpy(slcName + (separator - uri), ".", 1);
		strcpy(slcName + (separator - uri) + 1, separator + 1);
	}
}

/**
 * Convert the given URI to the legacy AIDA name for low level functions that still require it that way
 *
 * @param legacyName
 * @param uri
 * @return
 */
void uriLegacyName(char legacyName[MAX_URI_LEN], const char* uri)
{
	char* firstSeparator = strchr(uri, ':');
	char* secondSeparator = strchr(firstSeparator+1, ':');
	char* lastSeparator = strrchr(uri, ':');
	// See how many colons.  If only three then this is a pseudo and we need to separate after first
	if ( lastSeparator == secondSeparator ) {
		// This has only three parts so separate at first colon
		memcpy(legacyName, uri, firstSeparator - uri);
		memcpy(legacyName + (firstSeparator - uri), "//", 2);
		strcpy(legacyName + (firstSeparator - uri) + 2, firstSeparator + 1);
	} else {
		// Normal URI separate at last colon
		memcpy(legacyName, uri, lastSeparator - uri);
		memcpy(legacyName + (lastSeparator - uri), "//", 2);
		strcpy(legacyName + (lastSeparator - uri) + 2, lastSeparator + 1);
	}
}

/**
 * Allocate memory, copy data from source while checking for errors
 *
 * @param env to throw errors
 * @param source source data
 * @param size size of data
 * @param message message to display if fails
 * @return the allocated data or null if fails
 */
void* allocateMemory(JNIEnv* env, void* source, size_t size, bool nullTerminate, char* message)
{
	void* data = malloc(size);
	if (!data) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, message);
		return NULL;
	}
	if (source) {
		memcpy(data, source, size - (nullTerminate ? 1 : 0));
		if (nullTerminate) {
			*(char*)((char*)data + size - 1) = 0x0;
		}
	}
	return data;
}


