/**
 * Reference Server implementation: IMPLEMENT ANY METHODS THAT YOUR SERVER SUPPORTS
      **MEMBER**=SLCLIBS:AIDASHRLIB
      **ATTRIBUTES**=JNI
 */
#include <string.h>
#include <stdlib.h>

#include "aida_server_helper.h"
#include "json.h"

#include "reference_server.h"

#include "slc_macros.h"           /* vmsstat_t, int2u, int4u, etc. */

/*
 * Local Static
 */
static const $DESCRIPTOR( process_name, "AidaDbIf");

/**
 * Initialise the service
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @throws ServerInitialisationException if the service fails to initialise
 */
void aidaServiceInit(JNIEnv* env)
{
	printf("Aida Service Initialised\n");
}

/**
 * Get channel configuration
 * @param channelName
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @return the channel config.  Leave empty if you don't have any specific configuration overrides
 */
Config aidaChannelConfig(JNIEnv* env, const char* channelName)
{
	Config config;
	memset(&config, 0, sizeof(config));

	// Only modify config for attribute01
	if (startsWith(channelName, "AIDA:SAMPLE:") && endsWith(channelName, "//attribute01")) {
		Field* fields = calloc(1, sizeof(Field));
		if (!fields) {
			aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "unable to allocate space for config field");
			return config;
		}
		fields[0].name = "isActive";
		fields[0].label = "Device is active?";
		fields[0].description = "Device activity status.  Active if true";

		config.type = AIDA_BOOLEAN_TYPE;
		config.fields = fields;
		config.fieldCount = 1;
	}

	// Return the config
	return config;
}

/**
 * Get a boolean
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean
 */
int aidaRequestBoolean(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Only for attribute01
	if (!startsWith(uri, "AIDA:SAMPLE:") || !endsWith(uri, "//attribute01")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	int item = 1;

	// Optional Arguments
	Argument argument = getArgument(arguments, "x");
	if (argument.name) {
		item = getBooleanArgument(argument);
	}

	// Return the item
	return item;

}

/**
 * Get a byte
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the byte
 */
char aidaRequestByte(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Only for attribute02
	if (!startsWith(uri, "AIDA:SAMPLE:") || !endsWith(uri, "//attribute02")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	char item = 0x02;

	// Optional Arguments
	Argument argument = getArgument(arguments, "x");
	if (argument.name) {
		item |= getByteArgument(argument);
	}

	// Return the item
	return item;
}

/**
 * Get a short
 *
 * @param uri the uri
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param arguments the arguments
 * @return the short
 */
short aidaRequestShort(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Only for attribute03
	if (!startsWith(uri, "AIDA:SAMPLE:") || !endsWith(uri, "//attribute03")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	short item = 3;

	// Optional Arguments
	Argument argument = getArgument(arguments, "x");
	if (argument.name) {
		item += getShortArgument(argument);
	}

	// Return the item
	return item;
}

/**
 * Get a integer
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the integer
 */
int aidaRequestInteger(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Only for attribute04
	if (!startsWith(uri, "AIDA:SAMPLE:") || !endsWith(uri, "//attribute04")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	int item = 4;

	// Optional Arguments
	Argument argument = getArgument(arguments, "x");
	if (argument.name) {
		item += getIntegerArgument(argument);
	}

	// Return the item
	return item;
}

/**
 * Get a long
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the long
 */
long aidaRequestLong(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Only for attribute05
	if (!startsWith(uri, "AIDA:SAMPLE:") || !endsWith(uri, "//attribute05")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	long item = 5;

	// Optional Arguments
	Argument argument = getArgument(arguments, "x");
	if (argument.name) {
		item += getLongArgument(argument);
	}

	// Return the item
	return item;
}

/**
 * Get a float
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the float
 */
float aidaRequestFloat(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Only for attribute06
	if (!startsWith(uri, "AIDA:SAMPLE:") || !endsWith(uri, "//attribute06")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	float item = 6.6f;

	// Optional Arguments
	Argument argument = getArgument(arguments, "x");
	if (argument.name) {
		item *= getFloatArgument(argument);
	}

	// Return the item
	return item;
}

/**
 * Get a double
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the double
 */
double aidaRequestDouble(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Only for attribute07
	if (!startsWith(uri, "AIDA:SAMPLE:") || !endsWith(uri, "//attribute07")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	double item = 7.7;

	// Optional Arguments
	Argument argument = getArgument(arguments, "x");
	if (argument.name) {
		item *= getDoubleArgument(argument);
	}

	// Return the item
	return item;
}

/**
 * Get a string.  Allocate memory for string and it will be freed for you by framework
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the string
 */
char* aidaRequestString(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Only for attribute08
	if (!startsWith(uri, "AIDA:SAMPLE:") || !endsWith(uri, "//attribute08")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
		return NULL;
	}

	char* item;
	char* data = "eight";
	const char* arg = "";

	// Optional Arguments
	Argument argument = getArgument(arguments, "x");
	if (argument.name) {
		arg = argument.value;
	}

	// allocate and return
	item = malloc(strlen(data) + strlen(arg) + 3);
	if (!item) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "unable to allocate memory for string");
		return NULL;
	}

	if (!strlen(arg)) {
		sprintf(item, "%s", data);
	} else {
		sprintf(item, "%s: %s", arg, data);
	}

	// Return the item
	return item;
}

/**
 * Get a boolean array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean array
 */
Array aidaRequestBooleanArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Only for attribute11
	if (!startsWith(uri, "AIDA:SAMPLE:") || !endsWith(uri, "//attribute11")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	Array booleanArray;
	booleanArray.count = 1;
	booleanArray.items = calloc(1, sizeof(unsigned char));
	((unsigned char*)(booleanArray.items))[0] = 1;

	// Return the boolean array
	return booleanArray;
}

/**
 * Get a byte array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the byte array
 */
Array aidaRequestByteArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Only for attribute12
	if (!startsWith(uri, "AIDA:SAMPLE:") || !endsWith(uri, "//attribute12")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	Array byteArray;
	byteArray.count = 1;
	byteArray.items = calloc(1, sizeof(unsigned char));
	((unsigned char*)(byteArray.items))[0] = 12;

	// Return the byte array
	return byteArray;
}

/**
 * Get a short array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the short array
 */
Array aidaRequestShortArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Only for attribute13
	if (!startsWith(uri, "AIDA:SAMPLE:") || !endsWith(uri, "//attribute13")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	Array shortArray;
	shortArray.count = 1;
	shortArray.items = calloc(1, sizeof(short));
	((short*)(shortArray.items))[0] = 13;

	// Return the short array
	return shortArray;
}

/**
 * Get a integer array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the integer array
 */
Array aidaRequestIntegerArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Only for attribute14
	if (!startsWith(uri, "AIDA:SAMPLE:") || !endsWith(uri, "//attribute14")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	Array integerArray;
	integerArray.count = 1;
	integerArray.items = calloc(1, sizeof(int));
	((int*)(integerArray.items))[0] = 14;

	// Return the integer array
	return integerArray;
}

/**
 * Get a long array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the long array
 */
Array aidaRequestLongArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Only for attribute15
	if (!startsWith(uri, "AIDA:SAMPLE:") || !endsWith(uri, "//attribute15")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	Array longArray;
	longArray.count = 1;
	longArray.items = calloc(1, sizeof(long));
	((long*)(longArray.items))[0] = 15;

	// Return the long array
	return longArray;
}

/**
 * Get a float array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the float array
 */
Array aidaRequestFloatArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Only for attribute16
	if (!startsWith(uri, "AIDA:SAMPLE:") || !endsWith(uri, "//attribute16")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	Array floatArray;
	floatArray.count = 1;
	floatArray.items = calloc(1, sizeof(float));
	((float*)(floatArray.items))[0] = 16.6f;

	// Return the float array
	return floatArray;
}

/**
 * Get a double array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the double array
 */
Array aidaRequestDoubleArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Only for attribute17
	if (!startsWith(uri, "AIDA:SAMPLE:") || !endsWith(uri, "//attribute17")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	Array doubleArray;
	doubleArray.count = 1;
	doubleArray.items = calloc(1, sizeof(double));
	((double*)(doubleArray.items))[0] = 17.7;

	// Return the double array
	return doubleArray;
}

/**
 * Get a string array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the string array
 */
StringArray aidaRequestStringArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Only for attribute18
	if (!startsWith(uri, "AIDA:SAMPLE:") || !endsWith(uri, "//attribute18")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	StringArray stringArray;
	stringArray.count = 1;
	stringArray.items = calloc(1, sizeof(char*));
	((char**)(stringArray.items))[0] = malloc(9); // one character string
	strcpy(((char**)(stringArray.items))[0], "eighteen");

	// Return the string array
	return stringArray;
}

/**
 * Get a table of data
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the table
 */
Table aidaRequestTable(JNIEnv* env, const char* uri, Arguments arguments)
{
	Table table;
	memset(&table, 0, sizeof(table));
	table.columnCount = 8;

	// Only for attribute20
	if (!startsWith(uri, "AIDA:SAMPLE:") || !endsWith(uri, "//attribute20")) {
		table.columnCount = 0;
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
		return table;
	}

	// Allocate space for columns
	table.ppData = calloc(table.columnCount, sizeof(void*));
	if (!table.ppData) {
		table.columnCount = 0;
		char errorString[BUFSIZ];
		sprintf(errorString, "Unable to allocate memory for table: %ld", table.columnCount * sizeof(void*));
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
		return table;
	}

	table.types = calloc(table.columnCount, sizeof(Type*));
	if (!table.types) {
		char errorString[BUFSIZ];
		sprintf(errorString, "Unable to allocate memory for table types: %ld", table.columnCount * sizeof(Type*));
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
		return table;
	}

	// Do something to get rows of data ...
	table.rowCount = 1;

	// Allocate space for rows of data
	for (int column = 0; column < table.columnCount; column++) {
		switch (column) {
		case 0:
			table.types[column] = AIDA_BOOLEAN_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(unsigned char));
			break;
		case 1:
			table.types[column] = AIDA_BYTE_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(unsigned char));
			break;
		case 2:
			table.types[column] = AIDA_SHORT_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(short));
			break;
		case 3:
			table.types[column] = AIDA_INTEGER_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(int));
			break;
		case 4:
			table.types[column] = AIDA_LONG_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(long));
			break;
		case 5:
			table.types[column] = AIDA_FLOAT_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(float));
			break;
		case 6:
			table.types[column] = AIDA_DOUBLE_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(double));
			break;
		case 7:
			table.types[column] = AIDA_STRING_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(char*));
			break;
		default: // unsupported
			fprintf(stderr, "Unsupported table column type: %d\n", table.types[column]);
			break;
		}
	}

	// Add data to table
	((unsigned char*)(table.ppData[0]))[0] = 1; // Boolean
	((unsigned char*)(table.ppData[1]))[0] = 2; // Byte
	((short*)(table.ppData[2]))[0] = 3; // Short
	((int*)(table.ppData[3]))[0] = 4; // Integer
	((long*)(table.ppData[4]))[0] = 5; // Long
	((float*)(table.ppData[5]))[0] = 6.6f; // Float
	((double*)(table.ppData[6]))[0] = 7.7; // Double

	((char**)(table.ppData[7]))[0] = malloc(6); // String (one character)
	strcpy(((char**)(table.ppData[7]))[0], "eight");

	// Return the table
	return table;
}

/**
 * Set a value
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @param value to set
 * @return the table
 */
void aidaSetValue(JNIEnv* env, const char* uri, Arguments arguments, Value value)
{
	// Do set value logic ...
	printValue(env, value);

	char path[] = "foo[0].bar";
	json_value* jsonValue = getJsonValue(value, path);
	if (jsonValue) {
		printf("Value foo[0].bar = %d\n", jsonValue->u.integer);
	}
}

/**
 * Set a value and return a table as a response
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @param value to set
 * @return a table
 */
Table aidaSetValueWithResponse(JNIEnv* env, const char* uri, Arguments arguments, Value value)
{
	// Do set value logic ...
	printValue(env, value);

	char path[] = "foo[0].bar";
	json_value* jsonValue = getJsonValue(value, path);
	if (jsonValue) {
		printf("Value foo[0].bar = %d\n", jsonValue->u.integer);
	}

	// Return status in table
	Table table;
	memset(&table, 0, sizeof(table));
	table.columnCount = 1;

	// Allocate space for columns
	table.ppData = calloc(table.columnCount, sizeof(void*));
	if (!table.ppData) {
		char errorString[BUFSIZ];
		sprintf(errorString, "Unable to allocate memory for table: %ld", table.columnCount * sizeof(void*));
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
	}

	table.types = calloc(table.columnCount, sizeof(Type*));
	if (!table.types) {
		char errorString[BUFSIZ];
		sprintf(errorString, "Unable to allocate memory for table types: %ld", table.columnCount * sizeof(Type*));
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
	}

	// Do something to get rows of data ...
	table.rowCount = 1;

	// Allocate space for rows of data
	table.types[0] = AIDA_BOOLEAN_ARRAY_TYPE;
	table.ppData[0] = calloc(table.rowCount, sizeof(unsigned char));

	// Add data to table
	((unsigned char*)(table.ppData[0]))[0] = 1; // Boolean

	// Return the table
	return table;
}

