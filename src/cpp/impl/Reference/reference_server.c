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
	printf("Aida SLC Reference Service Initialised\n");
}

/**
 * Get a boolean.
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean
 */
int aidaRequestBoolean(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Only for attribute01
	if (!endsWith(uri, ":attribute32") && !endsWith(uri, ":attribute01")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	int item;

	// Optional Arguments
	unsigned char x = 0x1;
	ascanf(env, &arguments, "%ob", "x", &x);
	item = x;

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
	if (!endsWith(uri, ":attribute32") && !endsWith(uri, ":attribute02")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	char item = 0x02;

	// Optional Arguments
	unsigned char x = 0x0;
	ascanf(env, &arguments, "%oc", "x", &x);
	item |= x;

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
	if (!endsWith(uri, ":attribute32") && !endsWith(uri, ":attribute03")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	short item = 3;

	// Optional Arguments
	short x = 0;
	ascanf(env, &arguments, "ohd", "x", &x);
	item += x;

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
	if (!endsWith(uri, ":attribute32") && !endsWith(uri, ":attribute04")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	int item = 4;

	// Optional Arguments
	int x = 0;
	ascanf(env, &arguments, "%od", "x", &x);
	item += x;

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
	if (!endsWith(uri, ":attribute32") && !endsWith(uri, ":attribute05")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	long item = 5;

	// Optional Arguments
	long x = 0;
	ascanf(env, &arguments, "%old", "x", &x);
	item += x;

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
	if (!endsWith(uri, ":attribute32") && !endsWith(uri, ":attribute06")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	float item = 6.6f;

	// Optional Arguments
	float x = 1.0f;
	ascanf(env, &arguments, "%of", "x", &x);
	item *= x;

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
	if (!endsWith(uri, ":attribute32") && !endsWith(uri, ":attribute07")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	double item = 7.7;

	// Optional Arguments
	double x = 1.0;
	ascanf(env, &arguments, "%olf", "x", &x);
	item *= x;

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
	if (!endsWith(uri, ":attribute32") && !endsWith(uri, ":attribute08")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
		return NULL;
	}

	char* item;
	char* data = "eight";

	// Optional Arguments
	char* x = "";
	ascanf(env, &arguments, "%os", "x", &x);

	// allocate and return
	ALLOCATE_MEMORY_OR_RETURN(env, item, strlen(data) + strlen(x) + 3, "string", NULL)

	if (!strlen(x)) {
		sprintf(item, "%s", data);
	} else {
		sprintf(item, "%s: %s", data, x);
	}
	free(x);

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
	if (!endsWith(uri, ":attribute32") && !endsWith(uri, ":attribute11")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	Array booleanArray;
	booleanArray.count = 1;
	ALLOCATE_MEMORY_OR_RETURN(env, booleanArray.items, sizeof(unsigned char), "boolean array", booleanArray)
	((unsigned char*)(booleanArray.items))[0] = 1;

	// Optional Arguments
	unsigned char* x;
	unsigned int count = 0;
	if (!ascanf(env, &arguments, "%oba", "x", &x, &count) && count) {
		free(booleanArray.items);
		booleanArray.count = (int)count;
		booleanArray.items = x;
	}

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
	if (!endsWith(uri, ":attribute32") && !endsWith(uri, ":attribute12")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	Array byteArray;
	byteArray.count = 1;
	ALLOCATE_MEMORY_OR_RETURN(env, byteArray.items, sizeof(unsigned char), "byte array", byteArray)
	((unsigned char*)(byteArray.items))[0] = 12;

	// Optional Arguments
	unsigned char* x;
	unsigned int count = 0;
	if (!ascanf(env, &arguments, "%oca", "x", &x, &count) && count) {
		free(byteArray.items);
		byteArray.count = (int)count;
		byteArray.items = x;
		for (int i = 0; i < count; i++) {
			((unsigned char*)byteArray.items)[i] |= 12;
		}
	}

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
	if (!endsWith(uri, ":attribute32") && !endsWith(uri, ":attribute13")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	Array shortArray;
	shortArray.count = 1;
	ALLOCATE_MEMORY_OR_RETURN(env, shortArray.items, sizeof(short), "short array", shortArray)
	((short*)(shortArray.items))[0] = 13;

	// Optional Arguments
	short* x;
	unsigned int count = 0;
	if (!ascanf(env, &arguments, "%ohda", "x", &x, &count) && count) {
		free(shortArray.items);
		shortArray.count = (int)count;
		shortArray.items = x;
		for (int i = 0; i < count; i++) {
			((short*)shortArray.items)[i] += 13;
		}
	}

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
	if (!endsWith(uri, ":attribute32") && !endsWith(uri, ":attribute14")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	Array integerArray;
	integerArray.count = 1;
	ALLOCATE_MEMORY_OR_RETURN(env, integerArray.items, sizeof(int), "integer array", integerArray)
	((int*)(integerArray.items))[0] = 14;

	// Optional Arguments
	int* x;
	unsigned int count = 0;
	if (!ascanf(env, &arguments, "%oda", "x", &x, &count) && count) {
		free(integerArray.items);
		integerArray.count = (int)count;
		integerArray.items = x;
		for (int i = 0; i < count; i++) {
			((int*)integerArray.items)[i] += 14;
		}
	}

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
	if (!endsWith(uri, ":attribute32") && !endsWith(uri, ":attribute15")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	Array longArray;
	longArray.count = 1;
	ALLOCATE_MEMORY_OR_RETURN(env, longArray.items, sizeof(long), "long array", longArray)
	((long*)(longArray.items))[0] = 15;

	// Optional Arguments
	long* x;
	unsigned int count = 0;
	if (!ascanf(env, &arguments, "%olda", "x", &x, &count) && count) {
		free(longArray.items);
		longArray.count = (int)count;
		longArray.items = x;
		for (int i = 0; i < count; i++) {
			((long*)longArray.items)[i] += 15;
		}
	}

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
	if (!endsWith(uri, ":attribute32") && !endsWith(uri, ":attribute16")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	Array floatArray;
	floatArray.count = 1;
	ALLOCATE_MEMORY_OR_RETURN(env, floatArray.items, sizeof(float), "float array", floatArray)
	((float*)(floatArray.items))[0] = 16.6f;

	// Optional Arguments
	float* x;
	unsigned int count = 0;
	if (!ascanf(env, &arguments, "%ofa", "x", &x, &count) && count) {
		free(floatArray.items);
		floatArray.count = (int)count;
		floatArray.items = x;
		for (int i = 0; i < count; i++) {
			((float*)floatArray.items)[i] *= 16.6f;
		}
	}

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
	if (!endsWith(uri, ":attribute32") && !endsWith(uri, ":attribute17")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	Array doubleArray;
	doubleArray.count = 1;
	ALLOCATE_MEMORY_OR_RETURN(env, doubleArray.items, sizeof(double), "double array", doubleArray)
	((double*)(doubleArray.items))[0] = 17.7;

	// Optional Arguments
	double* x;
	unsigned int count = 0;
	if (!ascanf(env, &arguments, "%olfa", "x", &x, &count) && count) {
		free(doubleArray.items);
		doubleArray.count = (int)count;
		doubleArray.items = x;
		for (int i = 0; i < count; i++) {
			((double*)doubleArray.items)[i] *= 17.7;
		}
	}

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
	if (!endsWith(uri, ":attribute32") && !endsWith(uri, ":attribute18")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	}

	StringArray stringArray;
	stringArray.count = 1;
	ALLOCATE_MEMORY_OR_RETURN(env, stringArray.items, sizeof(char*), "string array", stringArray);
	ALLOCATE_STRING_OR_RETURN(env, stringArray.items[0], "eighteen", "string in string array", stringArray);

	// Optional Arguments
	char** x;
	unsigned int count = 0;
	if (!ascanf(env, &arguments, "%osa", "x", &x, &count) && count) {
		free(stringArray.items);
		stringArray.count = (int)count;
		stringArray.items = x;
	}

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
	// Only for attribute20
	if (!endsWith(uri, ":attribute32") && !endsWith(uri, ":attribute20")) {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
		RETURN_NULL_TABLE;
	}

	// Optional Arguments
	unsigned char xBoolean = 0x1, xByte = 0;
	short xShort = 0;
	int xInteger = 0;
	long xLong = 0;
	float xFloat = 1.0f;
	double xDouble = 1.0;
	char* xString = NULL;
	if (ascanf(env, &arguments, "%ob %oc %ohd %od %old %of %olf %os",
			"x.boolean", &xBoolean,
			"x.byte", &xByte,
			"x.short", &xShort,
			"x.integer", &xInteger,
			"x.long", &xLong,
			"x.float", &xFloat,
			"x.double", &xDouble,
			"x.string", &xString)) {
		RETURN_NULL_TABLE
	}

	Table table = tableCreate(env, 1, 8);
	CHECK_EXCEPTION_AND_RETURN_(table)
	tableAddSingleRowBooleanColumn(env, &table, xBoolean);
	CHECK_EXCEPTION_AND_RETURN_(table)
	tableAddSingleRowByteColumn(env, &table, 2 | xByte);
	CHECK_EXCEPTION_AND_RETURN_(table)
	tableAddSingleRowShortColumn(env, &table, 3 + xShort);
	CHECK_EXCEPTION_AND_RETURN_(table)
	tableAddSingleRowIntegerColumn(env, &table, 4 + xInteger);
	CHECK_EXCEPTION_AND_RETURN_(table)
	tableAddSingleRowLongColumn(env, &table, 5 + xLong);
	CHECK_EXCEPTION_AND_RETURN_(table)
	tableAddSingleRowFloatColumn(env, &table, 6.6f * xFloat, true);
	CHECK_EXCEPTION_AND_RETURN_(table)
	tableAddSingleRowDoubleColumn(env, &table, 7.7 * xDouble, true);
	CHECK_EXCEPTION_AND_RETURN_(table)
	tableAddSingleRowStringColumn(env, &table, xString ? xString : "eight");

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
	// Mandatory Value Argument
	unsigned char v;
	avscanf(env, &arguments, &value, "%b", "value", &v);

	Table table = tableCreate(env, 1, 1);
	CHECK_EXCEPTION_AND_RETURN_(table)
	tableAddSingleRowBooleanColumn(env, &table, v);

	// Return the table
	return table;
}

