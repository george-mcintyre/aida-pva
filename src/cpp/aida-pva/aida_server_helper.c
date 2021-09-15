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
		exit(status);
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
		return 0;
	}
	size_t lenstr = strlen(str);
	size_t lenSuffix = strlen(suffix);
	if (lenSuffix > lenstr)
		return 0;
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
		return 0;
	}
	size_t lenstr = strlen(str);
	size_t lenPrefix = strlen(prefix);
	if (lenPrefix > lenstr)
		return 0;
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
	char item = 0;
	int scannedValue = 0;

	if (!strncmp("0x", argument.value, 2)) {
		sscanf(argument.value, "%x", &scannedValue);
	} else {
		sscanf(argument.value, "%d", &scannedValue);
	}
	item = (char)scannedValue;

	return item;
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
 * @param env
 * @param value
 */
void printValue(JNIEnv* env, Value value)
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
json_value* getJsonValue(Value value, char* path)
{
	if (value.type != AIDA_JSON_TYPE) {
		return NULL;
	}

	json_value* jsonValue = value.value.jsonValue;

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

