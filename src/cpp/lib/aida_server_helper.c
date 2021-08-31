#include <string.h>
#include <stsdef.h>               /* $VMS.. status manipulation */
#include <ssdef.h>                /* SS$_NORMAL and other VMS general codes */
#include <descrip.h>              /*  for definition of $DESCRIPTOR  */
#include <ctype.h>                /* isalnum, ispunct */

#include "aida_types.h"
#include "aida_server_helper.h"

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
void aidaThrow(JNIEnv* env, int4u status, char* exception, const char* message)
{
	char vmsErrorMessage[256] = { '\0' };
	$DESCRIPTOR(MESSAGE, vmsErrorMessage);
	struct dsc$descriptor errorMessageDescriptor = { 256, DSC$K_DTYPE_T, DSC$K_CLASS_S, (char*)&vmsErrorMessage };

	//	Get the message text associated with the VMS message code.
	ERRTRANSLATE(&status, &errorMessageDescriptor);

	// If a message is specified then append it to the vms message string
	if (message) {
		strncat(errorMessageDescriptor.dsc$a_pointer, "; ",
				MIN(strlen("; "), 256 - strlen(errorMessageDescriptor.dsc$a_pointer)));
		strncat(errorMessageDescriptor.dsc$a_pointer, message,
				MIN(strlen(message), 256 - strlen(errorMessageDescriptor.dsc$a_pointer)));
	}

	// Log error message to cm log
	issue_err(errorMessageDescriptor.dsc$a_pointer);

	// Create the fully qualified java class name of the exception to throw
	char classToCreate[256] = "edu/stanford/slac/aida/exception/";
	strcat (classToCreate, exception);

	// Clear any exception that may be in the process of being thrown (unlikely)
	(*env)->ExceptionClear(env);

	// Create the java exception class
	jclass exceptionClass;
	exceptionClass = (*env)->FindClass(env, classToCreate);
	if (NULL == exceptionClass) {
		fprintf(stderr, "Failed to create object of class: %s\n", classToCreate);
		return;
	}

	// 	Throw the given exception to Java server code, giving the
	//	VMS error text and supplied message as the exception text.
	(*env)->ThrowNew(env, exceptionClass, errorMessageDescriptor.dsc$a_pointer);
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
		if (strcmp(argument.name, name) == 0) {
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
