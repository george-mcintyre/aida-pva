#include <string.h>
#include "../include/aida_types.h"
#include "../include/aida_server_helper.h"

/**
 * Get a named boolean argument
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
			item = argument.value.boolean_value;
			break;
		}
	}
	return item;
}

/**
 * Get a named byte argument
 * @param arguments arguments
 * @param name name
 * @return byte
 */
unsigned char getByteArgument(Arguments arguments, char* name)
{
	unsigned char item = 0;
	for (int i = 0; i < arguments.argumentCount; i++) {
		Argument argument = arguments.arguments[i];
		if (strcmp(argument.name, name) == 0) {
			item = argument.value.byte_value;
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
			item = argument.value.short_value;
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
	for (int i = 0; i < arguments.argumentCount; i++) {
		Argument argument = arguments.arguments[i];
		if (strcmp(argument.name, name) == 0) {
			item = argument.value.integer_value;
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
			item = argument.value.long_value;
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
			item = argument.value.float_value;
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
			item = argument.value.double_value;
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
char* getStringArgument(Arguments arguments, char* name)
{
	char* item = NULL;
	for (int i = 0; i < arguments.argumentCount; i++) {
		Argument argument = arguments.arguments[i];
		if (strcmp(argument.name, name) == 0) {
			item = argument.value.string_value;
			break;
		}
	}
	return item;
}
