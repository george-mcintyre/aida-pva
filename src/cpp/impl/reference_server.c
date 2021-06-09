/**
 * Reference Server implementation: IMPLEMENT ANY METHODS THAT YOUR SERVER SUPPORTS
      **MEMBER**=SLCLIBS:AIDASHRLIB
      **ATTRIBUTES**=JNI
 */
#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/reference_server.h"
#include "../include/aida_server_helper.h"

/**
 * Get channel configuration
 * @param channelName
 * @return the channel config.  Leave empty if you don't have any specific configuration overrides
 */
Config aidaChannelConfig(const char* channelName)
{
	Config config;
	memset(&config, 0, sizeof(config));

	if (strcmp(channelName, "PV1") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// example follows = ...
		Field* fields = calloc(1, sizeof(Field)); /* CALLOC-aidaChannelConfig-config.fields */
		fields[0].name = "fieldName";
		fields[0].label = "Field Value";

		config.type = STRING;
		config.layout = COLUMN_MAJOR;
 		config.fields = fields;
		config.fieldCount = 1;
	} else if (strcmp(channelName, "PV2") == 0) {
		// do something else
	}

	// Return the config
	return config;
}

/**
 * Get a table of data
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the table
 */
Table aidaRequestTable(char* uri, Arguments arguments)
{
	Table table;
	memset(&table, 0, sizeof(table));

	if (strcmp(uri, "PV1") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");

		// do something:
		// table.columnCount = 2,
		// table.rowCount = 10,
		// table.types = { INTEGER, STRING },
		// int *col1 = calloc(table.rowCount, sizeof(int))
		// char **col2 = calloc(table.rowCount, sizeof(char *))
		// ... set data in col1 and col2
		// table.ppData = {col1, col2}
		// ...

		table.columnCount = 0;
		table.rowCount = 0;

	} else if (strcmp(uri, "PV2") == 0) {
		// do something else
	}

	// Return the table
	return table;
}

/**
 * Get a boolean
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean
 */
int aidaRequestBoolean(char* uri, Arguments arguments)
{
	int item = 0;

	if (strcmp(uri, "PV1") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: item = ...
	} else if (strcmp(uri, "PV2") == 0) {
		// do something else
	}

	// Return the item
	return item;

}

/**
 * Get a byte
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the byte
 */
unsigned char aidaRequestByte(char* uri, Arguments arguments)
{
	unsigned char item = 0x0;

	if (strcmp(uri, "PV1") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: item = ...
	} else if (strcmp(uri, "PV2") == 0) {
		// do something else
	}

	// Return the item
	return item;
}

/**
 * Get a short
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the short
 */
short aidaRequestShort(char* uri, Arguments arguments)
{
	short item = 0;

	if (strcmp(uri, "PV1") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: item = ...
	} else if (strcmp(uri, "PV2") == 0) {
		// do something else
	}

	// Return the item
	return item;
}

/**
 * Get a integer
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the integer
 */
int aidaRequestInteger(char* uri, Arguments arguments)
{
	int item = 0;

	if (strcmp(uri, "PV1") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: item = ...
	} else if (strcmp(uri, "PV2") == 0) {
		// do something else
	}

	// Return the item
	return item;
}

/**
 * Get a long
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the long
 */
long aidaRequestLong(char* uri, Arguments arguments)
{
	long item = 0;

	if (strcmp(uri, "PV1") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: item = ...
	} else if (strcmp(uri, "PV2") == 0) {
		// do something else
	}

	// Return the item
	return item;
}

/**
 * Get a float
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the float
 */
float aidaRequestFloat(char* uri, Arguments arguments)
{
	float item = 0;

	if (strcmp(uri, "PV1") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: item = ...
	} else if (strcmp(uri, "PV2") == 0) {
		// do something else
	}

	// Return the item
	return item;
}

/**
 * Get a double
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the double
 */
double aidaRequestDouble(char* uri, Arguments arguments)
{
	double item = 0.0;

	if (strcmp(uri, "PV1") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: item = ...
	} else if (strcmp(uri, "PV2") == 0) {
		// do something else
	}

	// Return the item
	return item;
}

/**
 * Get a string
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the string
 */
char* aidaRequestString(char* uri, Arguments arguments)
{
	char* item = "";

	if (strcmp(uri, "PV1") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: item = ...
	} else if (strcmp(uri, "PV2") == 0) {
		// do something else
	}

	// Return the item
	return item;
}

/**
 * Get a boolean array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean array
 */


Array aidaRequestBooleanArray(char* uri, Arguments arguments)
{
	Array booleanArray;
	booleanArray.count = 0;

	if (strcmp(uri, "PV1") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: booleanArray.items = ...
	} else if (strcmp(uri, "PV2") == 0) {
		// do something else
	}

	// Return the boolean array
	return booleanArray;
}

/**
 * Get a byte array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the byte array
 */
Array aidaRequestByteArray(char* uri, Arguments arguments)
{
	Array byteArray;
	byteArray.count = 0;

	if (strcmp(uri, "PV1") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: byteArray.items = ...
	} else if (strcmp(uri, "PV2") == 0) {
		// do something else
	}

	// Return the byte array
	return byteArray;
}

/**
 * Get a short array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the short array
 */
Array aidaRequestShortArray(char* uri, Arguments arguments)
{
	Array shortArray;
	shortArray.count = 0;

	if (strcmp(uri, "PV1") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: shortArray.items = ...
	} else if (strcmp(uri, "PV2") == 0) {
		// do something else
	}

	// Return the short array
	return shortArray;
}

/**
 * Get a integer array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the integer array
 */
Array aidaRequestIntegerArray(char* uri, Arguments arguments)
{
	Array integerArray;
	integerArray.count = 0;

	if (strcmp(uri, "PV1") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: integerArray.items = ...
	} else if (strcmp(uri, "PV2") == 0) {
		// do something else
	}

	// Return the integer array
	return integerArray;
}

/**
 * Get a long array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the long array
 */
Array aidaRequestLongArray(char* uri, Arguments arguments)
{
	Array longArray;
	longArray.count = 0;

	if (strcmp(uri, "PV1") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: longArray.items = ...
	} else if (strcmp(uri, "PV2") == 0) {
		// do something else
	}

	// Return the long array
	return longArray;
}

/**
 * Get a float array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the float array
 */
Array aidaRequestFloatArray(char* uri, Arguments arguments)
{
	Array floatArray;
	floatArray.count = 0;

	if (strcmp(uri, "PV1") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: floatArray.items = ...
	} else if (strcmp(uri, "PV2") == 0) {
		// do something else
	}

	// Return the float array
	return floatArray;
}

/**
 * Get a double array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the double array
 */
Array aidaRequestDoubleArray(char* uri, Arguments arguments)
{
	Array doubleArray;
	doubleArray.count = 0;

	if (strcmp(uri, "PV1") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: doubleArray.items = ...
	} else if (strcmp(uri, "PV2") == 0) {
		// do something else
	}

	// Return the double array
	return doubleArray;
}

/**
 * Get a string array
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the string array
 */
StringArray aidaRequestStringArray(char* uri, Arguments arguments)
{
	StringArray stringArray;
	stringArray.count = 0;

	if (strcmp(uri, "PV1") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: stringArray.items = ...
	} else if (strcmp(uri, "PV2") == 0) {
		// do something else
	}

	// Return the string array
	return stringArray;
}

