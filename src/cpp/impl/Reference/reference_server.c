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

	// Manual test for parameters.
	// Simply enter parameters a, b, c, d, e, f, g, h, i, j, k
	// Defaults are all 0 zo any values you see are the ones you sent

	int intVar = 0, * intArray = NULL;
	unsigned int unsignedIntVar = 0, * unsignedIntArray= NULL;
	float floatVar = 0.0f, * floatArray= NULL;
	char byteVar = '0', * byteArray= NULL; // only first value works
	unsigned char boolVar = 0, * boolArray= NULL;
	char* stringVar = NULL, ** stringArray= NULL;
	short shortVar = 0, * shortArray= NULL;
	unsigned short unsignedShortVar = 0, * unsignedShortArray= NULL;
	long longVar = 0L, * longArray= NULL;
	unsigned long unsignedLongVar = 0L, * unsignedLongArray= NULL;
	double doubleVar = 0.0, * doubleArray= NULL;

	unsigned int intArrayCount, unsignedIntArrayCount, floatArrayCount, byteArrayCount, boolArrayCount, stringArrayCount, shortArrayCount, unsignedShortArrayCount, longArrayCount, unsignedLongArrayCount, doubleArrayCount;

	if (ascanf(env, &arguments,
			"%od   %ou   %of   %oc   %ob   %os "
			"%ohd  %ohu "
			"%old  %olu  %olf"
			"%oda  %oua  %ofa  %oca  %oba  %osa"
			"%ohda %ohua "
			"%olda %olua %olfa",
			"int", &intVar,
			"uInt", &unsignedIntVar,
			"float", &floatVar,
			"byte", &byteVar,
			"bool", &boolVar,
			"string", &stringVar,
			"short", &shortVar,
			"uShort", &unsignedShortVar,
			"long", &longVar,
			"uLong", &unsignedLongVar,
			"double", &doubleVar,
			"intA", &intArray, &intArrayCount,
			"uIntA", &unsignedIntArray, &unsignedIntArrayCount,
			"floatA", &floatArray, &floatArrayCount,
			"byteA", &byteArray, &byteArrayCount,
			"boolA", &boolArray, &boolArrayCount,
			"stringA", &stringArray, &stringArrayCount,
			"shortA", &shortArray, &shortArrayCount,
			"uShortA", &unsignedShortArray, &unsignedShortArrayCount,
			"longA", &longArray, &longArrayCount,
			"uLongA", &unsignedLongArray, &unsignedLongArrayCount,
			"doubleA", &doubleArray, &doubleArrayCount
	)) {
		// Do some error stuff
		printf("Parsing error in parameters.\n");
	} else {
		// Print values
		printf("int=%d, ", intVar);
		printf("uInt=%u, ", unsignedIntVar);
		printf("float=%g, ", floatVar);
		printf("byte=%c, ", byteVar);
		printf("bool=%d, ", boolVar);
		if (stringVar) printf("string=%s, ", stringVar);
		printf("short=%hd, ", shortVar);
		printf("uShort=%hu, ", unsignedShortVar);
		printf("long=%ld, ", longVar);
		printf("uLong=%lu, ", unsignedLongVar);
		printf("double=%g, ", doubleVar);

		if (intArrayCount) { for (int index = 0; index < intArrayCount; index++) printf("intA[%d]=%d, ", index, intArray[index]); printf("\n");}
		if (unsignedIntArrayCount) { for (int index = 0; index < unsignedIntArrayCount; index++) printf("uIntA[%d]=%u, ", index, unsignedIntArray[index]); printf("\n");}
		if (floatArrayCount) { for (int index = 0; index < floatArrayCount; index++) printf("floatA[%d]=%g, ", index, floatArray[index]); printf("\n");}
		if (byteArrayCount) { for (int index = 0; index < byteArrayCount; index++) printf("byteA[%d]=%c, ", index, byteArray[index]); printf("\n");}
		if (boolArrayCount) { for (int index = 0; index < boolArrayCount; index++) printf("boolA[%d]=%d, ", index, boolArray[index]); printf("\n");}
		if (stringArrayCount) { for (int index = 0; index < stringArrayCount; index++) printf("stringA[%d]=%s, ", index, stringArray[index]); printf("\n");}
		if (shortArrayCount) { for (int index = 0; index < shortArrayCount; index++) printf("shortA[%d]=%hd, ",index,  shortArray[index]); printf("\n");}
		if (unsignedShortArrayCount) { for (int index = 0; index < unsignedShortArrayCount; index++) printf("uShortA[%d]=%hu, ",index,  unsignedShortArray[index]); printf("\n");}
		if (longArrayCount) { for (int index = 0; index < longArrayCount; index++) printf("longA[%d]=%ld, ",index,  longArray[index]); printf("\n");}
		if (unsignedLongArrayCount) { for (int index = 0; index < unsignedLongArrayCount; index++) printf("uLongA[%d]=%lu, ",index,  unsignedLongArray[index]); printf("\n");}
		if (doubleArrayCount) { for (int index = 0; index < doubleArrayCount; index++) printf("doubleA[%d]=%g, ",index,  doubleArray[index]); printf("\n");}
	}

	// Free allocated stuff
	// Strings
	if (stringVar) {
		free(stringVar);
	}

	// Arrays
	if (intArrayCount) free(intArray);
	if (unsignedIntArrayCount) free(unsignedIntArray);
	if (floatArrayCount) free(floatArray);
	if (byteArrayCount) free(byteArray);
	if (boolArrayCount) free(boolArray);
	if (stringArrayCount) free(stringArray);
	if (shortArrayCount) free(shortArray);
	if (unsignedShortArrayCount) free(unsignedShortArray);
	if (longArrayCount) free(longArray);
	if (unsignedLongArrayCount) free(unsignedLongArray);
	if (doubleArrayCount) free(doubleArray);


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

	table = tableCreate(env, 1, 8);
	CHECK_EXCEPTION(table)
	tableAddSingleRowBooleanColumn(env, &table, 1);
	CHECK_EXCEPTION(table)
	tableAddSingleRowByteColumn(env, &table, 2);
	CHECK_EXCEPTION(table)
	tableAddSingleRowShortColumn(env, &table, 3);
	CHECK_EXCEPTION(table)
	tableAddSingleRowIntegerColumn(env, &table, 4);
	CHECK_EXCEPTION(table)
	tableAddSingleRowLongColumn(env, &table, 5);
	CHECK_EXCEPTION(table)
	tableAddSingleRowFloatColumn(env, &table, 6.6f);
	CHECK_EXCEPTION(table)
	tableAddSingleRowDoubleColumn(env, &table, 7.7);
	CHECK_EXCEPTION(table)
	tableAddSingleRowStringColumn(env, &table, "eight");

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

	Table table = tableCreate(env, 1, 1);
	CHECK_EXCEPTION(table)
	tableAddSingleRowBooleanColumn(env, &table, 1);

	// Return the table
	return table;
}

