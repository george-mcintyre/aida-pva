#include <string.h>
#include "../include/aida_types.h"
#include "../include/aida_server_helper.h"

/**
 * Get a named boolean argument
 *
 * @param arguments arguments
 * @param name name
 * @return boolean
 */
int getBooleanArgument(Arguments arguments, char* name)
{
	int item = 0;
	for (int i = 0; i < arguments.argumentCount; i++) {
		Argument argument = arguments.arguments[i];
		if (strcmp(argument.name, name) == 0) {
			// set true if non-empty string and not 'FALSE', 'false', 'False', or '0'
			if (strlen(argument.value) > 0) {
				item = strlen(argument.value) &&
						!strcmp(argument.value, "FALSE") &&
						!strcmp(argument.value, "false") &&
						!strcmp(argument.value, "False") &&
						!strcmp(argument.value, "0");
			}
			break;
		}
	}
	return item;
}

/**
 * Get a named byte argument
 *
 * @param arguments arguments
 * @param name name
 * @return byte
 */
unsigned char getByteArgument(Arguments arguments, char* name)
{
	unsigned char item = 0;
	unsigned int scannedValue = 0;
	for (int i = 0; i < arguments.argumentCount; i++) {
		Argument argument = arguments.arguments[i];
		if (strcmp(argument.name, name) == 0) {
			if (strlen(argument.value) > 0) {
				if ( !strncmp('0x', argument.value, 2)) {
					sscanf(argument.value, "%x", &scannedValue);
				} else {
					sscanf(argument.value, "%d", &scannedValue);
				}
				item = scannedValue;
			}
			break;
		}
	}
	return item;
}

/**
 * Get a named short argument
 * @param arguments arguments
 * @param name name
 * @return short
 */
short getShortArgument(Arguments arguments, char* name)
{
	short item = 0;
	for (int i = 0; i < arguments.argumentCount; i++) {
		Argument argument = arguments.arguments[i];
		if (strcmp(argument.name, name) == 0) {
			if (strlen(argument.value) > 0) {
				sscanf(argument.value, "%hi", &item);
			}
			break;
		}
	}
	return item;
}

/**
 * Get a named integer argument
 * @param arguments arguments
 * @param name name
 * @return int
 */
int getIntegerArgument(Arguments arguments, char* name)
{
	int item = 0;
	unsigned int scannedValue = 0;
	for (int i = 0; i < arguments.argumentCount; i++) {
		Argument argument = arguments.arguments[i];
		if (strcmp(argument.name, name) == 0) {
			if (strlen(argument.value) > 0) {
				sscanf(argument.value, "%d", &item);
			}
			break;
		}
	}
	return item;
}

/**
 * Get a named long argument
 * @param arguments arguments
 * @param name name
 * @return long
 */
long getLongArgument(Arguments arguments, char* name)
{
	long item = 0;
	for (int i = 0; i < arguments.argumentCount; i++) {
		Argument argument = arguments.arguments[i];
		if (strcmp(argument.name, name) == 0) {
			if (strlen(argument.value) > 0) {
				sscanf(argument.value, "%ld", &item);
			}
			break;
		}
	}
	return item;
}

/**
 * Get a named float argument
 * @param arguments arguments
 * @param name name
 * @return float
 */
float getFloatArgument(Arguments arguments, char* name)
{
	float item = 0;
	for (int i = 0; i < arguments.argumentCount; i++) {
		Argument argument = arguments.arguments[i];
		if (strcmp(argument.name, name) == 0) {
			if (strlen(argument.value) > 0) {
				sscanf(argument.value, "%f", &item);
			}
			break;
		}
	}
	return item;
}

/**
 * Get a named double argument
 * @param arguments arguments
 * @param name name
 * @return double
 */
double getDoubleArgument(Arguments arguments, char* name)
{
	double item = 0.0;
	for (int i = 0; i < arguments.argumentCount; i++) {
		Argument argument = arguments.arguments[i];
		if (strcmp(argument.name, name) == 0) {
			if (strlen(argument.value) > 0) {
				sscanf(argument.value, "%lf", &item);
			}
			break;
		}
	}
	return item;
}

/**
 * Get a named string argument
 * @param arguments arguments
 * @param name name
 * @return string
 */
const char* getStringArgument(Arguments arguments, char* name)
{
	for (int i = 0; i < arguments.argumentCount; i++) {
		Argument argument = arguments.arguments[i];
		if (strcmp(argument.name, name) == 0) {
			return argument.value;
		}
	}
	return NULL;
}
