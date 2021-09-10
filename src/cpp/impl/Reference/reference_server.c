/**
 * Reference Server implementation: IMPLEMENT ANY METHODS THAT YOUR SERVER SUPPORTS
      **MEMBER**=SLCLIBS:AIDASHRLIB
      **ATTRIBUTES**=JNI
 */
#include <string.h>
#include <stdlib.h>
#include "aida_server_helper.h"
#include "reference_server.h"

#include "opdef.hc"               /* General messages */
#include "slc_macros.h"           /* vmsstat_t, int2u, int4u, etc. */
#include "msg_proto.h"            /* for standalone_init */
#include "sysutil_proto.h"        /* for cvt_vms_to_ieee_flt */
#include "err_proto.h"            /* for err_translate */
#include "ref.h"                  /* Pass by ref to fortran helpers */
#include "descr.h"                /* for pass by descr macros */
#include "util_proto.h"           /* asts_color */
#include "err_facility_p.h"

/*
 * Local Static
 */
static const $DESCRIPTOR( process_name,
"AidaDbIf");

/*
* Implementation for cm log requires process name so needs to be implemented in service impl.
 */
void issue_err(char* message)
{
	DESCR_DECLARE;
	REF_DECLARE;
	char msg_c[BUFSIZ];

	strcpy(msg_c, "ReferenceService: ");            /* Prepend Aida process name to msg */
	strcat(msg_c, message);                 /* Add passed in message */

	// TODO find impl of err_text
	// err_text( REFINT4U_1(OP_MSGTXT), DESCRN1(msg_c), &process_name );
}

/**
 * Initialise the service
 * @param env to be used to throw exceptions using throw()
 * @throws ServerInitialisationException if the service fails to initialise
 */
void aidaServiceInit(JNIEnv* env)
{
	printf("Aida Service Initialised\n");
}

/**
 * Get channel configuration
 * @param channelName
 * @param env to be used to throw exceptions using throw()
 * @return the channel config.  Leave empty if you don't have any specific configuration overrides
 */
Config aidaChannelConfig(JNIEnv* env, const char* channelName)
{
	Config config;
	memset(&config, 0, sizeof(config));

	// Only modify config for attribute01
	if (startsWith(channelName, "AIDA:SAMPLE:") && endsWith(channelName, "//attribute01")) {
		Field* fields = calloc(1, sizeof(Field));
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
 * @param env to be used to throw exceptions using throw()
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
	if (argument.name != NULL) {
		item = getBooleanArgument(argument);
	}

	// Return the item
	return item;

}

/**
 * Get a byte
 *
 * @param env to be used to throw exceptions using throw()
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
	if (argument.name != NULL) {
		item |= getByteArgument(argument);
	}

	// Return the item
	return item;
}

/**
 * Get a short
 *
 * @param uri the uri
 * @param env to be used to throw exceptions using throw()
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
	if (argument.name != NULL) {
		item += getShortArgument(argument);
	}

	// Return the item
	return item;
}

/**
 * Get a integer
 *
 * @param env to be used to throw exceptions using throw()
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
	if (argument.name != NULL) {
		item += getIntegerArgument(argument);
	}

	// Return the item
	return item;
}

/**
 * Get a long
 *
 * @param env to be used to throw exceptions using throw()
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
	if (argument.name != NULL) {
		item += getLongArgument(argument);
	}

	// Return the item
	return item;
}

/**
 * Get a float
 *
 * @param env to be used to throw exceptions using throw()
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
	if (argument.name != NULL) {
		item *= getFloatArgument(argument);
	}

	// Return the item
	return item;
}

/**
 * Get a double
 *
 * @param env to be used to throw exceptions using throw()
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
	if (argument.name != NULL) {
		item *= getDoubleArgument(argument);
	}

	// Return the item
	return item;
}

/**
 * Get a string.  Allocate memory for string and it will be freed for you by framework
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the string
 */
char* aidaRequestString(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Only for attribute08
	if (!startsWith(uri, "AIDA:SAMPLE:") || !endsWith(uri, "//attribute08")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	char* item;
	char* data = "eight";
	const char* arg = "";

	// Optional Arguments
	Argument argument = getArgument(arguments, "x");
	if (argument.name != NULL) {
		arg = argument.value;
	}

	// allocate and return
	item = malloc(strlen(data) + strlen(arg) + 3);
	if (strlen(arg) == 0) {
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
 * @param env to be used to throw exceptions using throw()
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
 * @param env to be used to throw exceptions using throw()
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
 * @param env to be used to throw exceptions using throw()
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
 * @param env to be used to throw exceptions using throw()
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
 * @param env to be used to throw exceptions using throw()
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
 * @param env to be used to throw exceptions using throw()
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
 * @param env to be used to throw exceptions using throw()
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
 * @param env to be used to throw exceptions using throw()
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
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the table
 */
Table aidaRequestTable(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Only for attribute20
	if (!startsWith(uri, "AIDA:SAMPLE:") || !endsWith(uri, "//attribute20")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	Table table;
	memset(&table, 0, sizeof(table));
	table.columnCount = 8;

	// Allocate space for columns
	table.ppData = calloc(table.columnCount, sizeof(void*));
	if (table.ppData == NULL) {
		fprintf(stderr, "Unable to allocate memory for table: %ld\n", table.columnCount * sizeof(void*));
		return table;
	}

	table.types = calloc(table.columnCount, sizeof(Type*));
	if (table.types == NULL) {
		fprintf(stderr, "Unable to allocate memory for table types: %ld\n", table.columnCount * sizeof(Type*));
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