/**
 * Reference Server implementation: IMPLEMENT ANY METHODS THAT YOUR SERVER SUPPORTS
      **MEMBER**=SLCLIBS:AIDASHRLIB
      **ATTRIBUTES**=JNI
 */
#include <string.h>
#include <stdlib.h>
#include "../include/reference_server.h"

/**
 * Get channel configuration
 * @param channelName
 * @return the channel config.  Leave empty if you don't have any specific configuration overrides
 */
Config aidaChannelConfig(const char* channelName)
{
	Config config;
	memset(&config, 0, sizeof(config));

	if (strcmp(channelName, "AIDA:SAMPLE:DEVICE01:attribute01") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// example follows = ...
		Field* fields = calloc(1, sizeof(Field)); /* CALLOC-aidaChannelConfig-config.fields */
		fields[0].name = "fieldName";
		fields[0].label = "Field Value";

		config.type = STRING;
		config.layout = COLUMN_MAJOR;
 		config.fields = fields;
		config.fieldCount = 1;
	} else if (strcmp(channelName, "AIDA:SAMPLE:DEVICE01:attribute02") == 0) {
		// do something else
	}

	// Return the config
	return config;
}

/**
 * Get a boolean
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean
 */
int aidaRequestBoolean(const char* uri, Arguments arguments)
{
	int item = 0;

	if (strcmp(uri, "AIDA:SAMPLE:DEVICE01:attribute01") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: item = ...
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
char aidaRequestByte(const char* uri, Arguments arguments)
{
	char item = 0x0;

	if (strcmp(uri, "AIDA:SAMPLE:DEVICE01:attribute02") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: item = ...
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
short aidaRequestShort(const char* uri, Arguments arguments)
{
	short item = 0;

	if (strcmp(uri, "AIDA:SAMPLE:DEVICE01:attribute03") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: item = ...
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
int aidaRequestInteger(const char* uri, Arguments arguments)
{
	int item = 0;

	if (strcmp(uri, "AIDA:SAMPLE:DEVICE01:attribute04") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: item = ...
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
long aidaRequestLong(const char* uri, Arguments arguments)
{
	long item = 0;

	if (strcmp(uri, "AIDA:SAMPLE:DEVICE01:attribute05") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: item = ...
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
float aidaRequestFloat(const char* uri, Arguments arguments)
{
	float item = 0;

	if (strcmp(uri, "AIDA:SAMPLE:DEVICE01:attribute06") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: item = ...
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
double aidaRequestDouble(const char* uri, Arguments arguments)
{
	double item = 0.0;

	if (strcmp(uri, "AIDA:SAMPLE:DEVICE01:attribute07") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: item = ...
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
char* aidaRequestString(const char* uri, Arguments arguments)
{
	char* item = "";

	if (strcmp(uri, "AIDA:SAMPLE:DEVICE01:attribute08") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: item = ...
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


Array aidaRequestBooleanArray(const char* uri, Arguments arguments)
{
	Array booleanArray;
	booleanArray.count = 0;

	if (strcmp(uri, "AIDA:SAMPLE:DEVICE01:attribute11") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: booleanArray.items = ...
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
Array aidaRequestByteArray(const char* uri, Arguments arguments)
{
	Array byteArray;
	byteArray.count = 0;

	if (strcmp(uri, "AIDA:SAMPLE:DEVICE01:attribute12") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: byteArray.items = ...
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
Array aidaRequestShortArray(const char* uri, Arguments arguments)
{
	Array shortArray;
	shortArray.count = 0;

	if (strcmp(uri, "AIDA:SAMPLE:DEVICE01:attribute13") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: shortArray.items = ...
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
Array aidaRequestIntegerArray(const char* uri, Arguments arguments)
{
	Array integerArray;
	integerArray.count = 0;

	if (strcmp(uri, "AIDA:SAMPLE:DEVICE01:attribute14") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: integerArray.items = ...
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
Array aidaRequestLongArray(const char* uri, Arguments arguments)
{
	Array longArray;
	longArray.count = 0;

	if (strcmp(uri, "AIDA:SAMPLE:DEVICE01:attribute15") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: longArray.items = ...
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
Array aidaRequestFloatArray(const char* uri, Arguments arguments)
{
	Array floatArray;
	floatArray.count = 0;

	if (strcmp(uri, "AIDA:SAMPLE:DEVICE01:attribute16") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: floatArray.items = ...
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
Array aidaRequestDoubleArray(const char* uri, Arguments arguments)
{
	Array doubleArray;
	doubleArray.count = 0;

	if (strcmp(uri, "AIDA:SAMPLE:DEVICE01:attribute17") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: doubleArray.items = ...
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
StringArray aidaRequestStringArray(const char* uri, Arguments arguments)
{
	StringArray stringArray;
	stringArray.count = 0;

	if (strcmp(uri, "AIDA:SAMPLE:DEVICE01:attribute18") == 0) {
		// int param1 = getIntegerArgument(arguments, "x");
		// do something: stringArray.items = ...
	}

	// Return the string array
	return stringArray;
}


/**
 * Get a table of data
 *
 * @param uri the uri
 * @param arguments the arguments
 * @return the table
 */
Table aidaRequestTable(const char* uri, Arguments arguments)
{
	Table table;
	memset(&table, 0, sizeof(table));

	if (strcmp(uri, "AIDA:SAMPLE:DEVICE01:attribute20") == 0) {
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
	}

	// Return the table
	return table;
}
