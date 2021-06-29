/**
 * Reference Server implementation: IMPLEMENT ANY METHODS THAT YOUR SERVER SUPPORTS
      **MEMBER**=SLCLIBS:AIDASHRLIB
      **ATTRIBUTES**=JNI
 */
#include <string.h>
#include <stdlib.h>
#include "reference_server.h"

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
		Field* fields = calloc(1, sizeof(Field));
		fields[0].name = "isActive";
		fields[0].label = "Device is active?";
		fields[0].description = "Device activity status.  Active if true";

		config.type = BOOLEAN;
		config.fields = fields;
		config.fieldCount = 1;
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
		item = 1;
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
		item = 0x02;
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
		item = 3;
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
		item = 4;
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
		item = 5;
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
		item = 6;
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
		item = 7.0;
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
		item = "8";
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
		booleanArray.count = 1;
		booleanArray.items = calloc(1, sizeof(unsigned char));
		((unsigned char*)(booleanArray.items))[0] = 1;
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
		byteArray.count = 1;
		byteArray.items = calloc(1, sizeof(unsigned char));
		((unsigned char*)(byteArray.items))[0] = 1;
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
		shortArray.count = 1;
		shortArray.items = calloc(1, sizeof(short));
		((short*)(shortArray.items))[0] = 2;
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
		integerArray.count = 1;
		integerArray.items = calloc(1, sizeof(int));
		((int*)(integerArray.items))[0] = 3;
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
		longArray.count = 1;
		longArray.items = calloc(1, sizeof(long));
		((long*)(longArray.items))[0] = 4;
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
		floatArray.count = 1;
		floatArray.items = calloc(1, sizeof(float));
		((float*)(floatArray.items))[0] = 5;
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
		doubleArray.count = 1;
		doubleArray.items = calloc(1, sizeof(double));
		((double*)(doubleArray.items))[0] = 6;
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
		stringArray.count = 1;
		stringArray.items = calloc(1, sizeof(char *));
		((char **)(stringArray.items))[0] = malloc(2); // one character string
		strcpy(((char**)(stringArray.items))[0], "8");
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
		table.columnCount = 8;

		// Allocate space for columns
		table.ppData = calloc(table.columnCount, sizeof(*table.ppData));

		// Do something to get rows of data ...
		table.rowCount = 1;

		// Allocate space for rows of data
		for (int column = 0; column < table.columnCount; column++) {
			switch (table.types[column]) {
			case BOOLEAN_ARRAY:
				table.ppData[column] = calloc(table.rowCount, sizeof(unsigned char));
				break;
			case BYTE_ARRAY:
				table.ppData[column] = calloc(table.rowCount, sizeof(unsigned char));
				break;
			case SHORT_ARRAY:
				table.ppData[column] = calloc(table.rowCount, sizeof(short));
				break;
			case INTEGER_ARRAY:
				table.ppData[column] = calloc(table.rowCount, sizeof(int));
				break;
			case LONG_ARRAY:
				table.ppData[column] = calloc(table.rowCount, sizeof(long));
				break;
			case FLOAT_ARRAY:
				table.ppData[column] = calloc(table.rowCount, sizeof(float));
				break;
			case DOUBLE_ARRAY:
				table.ppData[column] = calloc(table.rowCount, sizeof(double));
				break;
			case STRING_ARRAY:
				table.ppData[column] = calloc(table.rowCount, sizeof(char*));
				break;
			default: // unsupported
				fprintf( stderr, "Unsupported table column type: %d\n", table.types[column]);
				break;
			}
		}
	}

	// Add data to table
	((unsigned char*)(table.ppData[0]))[0] = 1; // Boolean
	((unsigned char*)(table.ppData[1]))[0] = 2; // Byte
	((short*)(table.ppData[2]))[0] = 3; // Short
	((int*)(table.ppData[3]))[0] = 4; // Integer
	((long*)(table.ppData[4]))[0] = 5; // Long
	((float*)(table.ppData[5]))[0] = 6; // Float
	((double*)(table.ppData[6]))[0] = 7.0; // Double

	((char**)(table.ppData[7]))[0] = malloc(2); // String (one character)
	strcpy(((char**)(table.ppData[7]))[0], "8");

	// Return the table
	return table;
}
