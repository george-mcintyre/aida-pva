#include <string.h>
#include "aida_types.h"
#include "aida_server_helper.h"

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
