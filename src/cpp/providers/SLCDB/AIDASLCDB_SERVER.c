/** @file
 *  @brief SLC Database Native Provider implementation.  This source file implements
 *  the AIDA-PVA SLC Database Provider.
 *  SLC Database provides data from the database values in the SLC Control System.
 *  It also allows the setting of float scalar database values.
 *
 *  Acquires data for the named SLC device from the SLC database. Also allows setting of float scalar database values.
 *
 *     **MEMBER**=SLCLIBS:AIDA_PVALIB
 *     **ATTRIBUTES**=JNI,LIBR_NOGLOBAL
 */
#include "aida_pva.h"
#include "AIDASLCDB_SERVER.h"

// API Stubs
VERSION("1.0.0")
SET_STUB_TABLE

/**
 * Initialise the service
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @throws ServerInitialisationException if the service fails to initialise
 */
void aidaServiceInit(JNIEnv* env)
{
	vmsstat_t status;

	if (!$VMS_STATUS_SUCCESS(status = init("AIDA_SLCDB", false))) {
		aidaThrow(env, status, SERVER_INITIALISATION_EXCEPTION, "initialising SLC Service");
	} else {
		printf("AIDA-PVA SLC Database Provider\n");
	}
}

/**
 * Get a boolean.  In fact this is implemented by getting a short and returning true if it is 0 and false otherwise
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean
 */
int aidaRequestBoolean(JNIEnv* env, const char* uri, Arguments arguments)
{
	int val = 0;
	TO_SLC_NAME(uri, slcName)
	vmsstat_t status = JNI_DBGETINT(slcName, &val);

	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "getting SLC db boolean device data");
	}

	// Return logical
	return val != 0;
}

/**
 * Get a byte
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the byte
 */
char aidaRequestByte(JNIEnv* env, const char* uri, Arguments arguments)
{
	unsigned char val = 0;
	TO_SLC_NAME(uri, slcName)
	vmsstat_t status = JNI_DBGETBYTE(slcName, &val);

	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "getting SLC db byte device data");
	}

	return (char)val;
}

/**
 * Get a short
 *
 * @param uri the uri
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param arguments the arguments
 * @return the short
 */
short aidaRequestShort(JNIEnv* env, const char* uri, Arguments arguments)
{
	short int val = 0;
	TO_SLC_NAME(uri, slcName)
	vmsstat_t status = JNI_DBGETSHORT(slcName, &val);

	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "getting SLC db short device data");
	}

	return val;
}

/**
 * Get a integer
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the integer
 */
int aidaRequestInteger(JNIEnv* env, const char* uri, Arguments arguments)
{
	int val = 0;
	TO_SLC_NAME(uri, slcName)
	vmsstat_t status = JNI_DBGETINT(slcName, &val);

	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "getting SLC db integer device data");
	}

	return val;
}

/**
 * Get a long.  In fact this is implemented by getting an integer and then converting the return value to a long
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the long
 */
long aidaRequestLong(JNIEnv* env, const char* uri, Arguments arguments)
{
	int val = 0;
	TO_SLC_NAME(uri, slcName)
	vmsstat_t status = JNI_DBGETINT(slcName, &val);

	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "getting SLC db long device data");
	}

	return val;
}

/**
 * Get a float
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the float
 */
float aidaRequestFloat(JNIEnv* env, const char* uri, Arguments arguments)
{
	float val = 0.0f; /* Returned in ieee format */
	TO_SLC_NAME(uri, slcName)
	vmsstat_t status = JNI_DBGETFLOAT(slcName, &val);

	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "getting SLC db floating point device data");
	}

	return val;
}

/**
 * Get a double.  In fact this is implemented by getting a float and converting the return to a double
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the double
 */
double aidaRequestDouble(JNIEnv* env, const char* uri, Arguments arguments)
{
	float val = 0.0f; /* Returned in ieee format */
	TO_SLC_NAME(uri, slcName)
	vmsstat_t status = JNI_DBGETFLOAT(slcName, &val);

	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "getting SLC db double device data");
	}

	return val;
}

/**
 * Get a string.  Allocate memory for string and it will be freed for you by framework
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the string
 */
char* aidaRequestString(JNIEnv* env, const char* uri, Arguments arguments)
{
	char* val = NULL;
	TO_SLC_NAME(uri, slcName)
	vmsstat_t status = JNI_DBGETSTRING(slcName, &val);

	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "getting SLC db string device data");
	}

	return val;
}

/**
 * Get a boolean array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean array
 */
Array aidaRequestBooleanArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	Array booleanArray;
	booleanArray.count = 0;
	booleanArray.items = NULL;

	TO_SLC_NAME(uri, slcName)
	vmsstat_t status = JNI_DBGETINTA(slcName, (int**)(&booleanArray.items));

	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "getting SLC db boolean array device data");
	}

	// First item is the count
	booleanArray.count = *((int*)booleanArray.items);

	// update values to contain booleans instead of integers
	for (int i = 0; i < booleanArray.count; i++) {
		((int*)booleanArray.items)[i] = ((int*)booleanArray.items)[i] != 0;
	}
	return booleanArray;
}

/**
 * Get a byte array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the byte array
 */
Array aidaRequestByteArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	Array byteArray;
	byteArray.count = 0;
	byteArray.items = NULL;

	TO_SLC_NAME(uri, slcName)
	vmsstat_t status = JNI_DBGETBYTEA(slcName, (unsigned char**)(&byteArray.items));

	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "getting SLC db byte array device data");
	}

	byteArray.count = (int)strlen(byteArray.items);
	return byteArray;
}

/**
 * Get a short array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the short array
 */
Array aidaRequestShortArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	TRACK_ALLOCATED_MEMORY
	Array shortArray;
	shortArray.count = 0;
	shortArray.items = NULL;

	TO_SLC_NAME(uri, slcName)
	vmsstat_t status = JNI_DBGETSHORTA(slcName, (short**)(&shortArray.items));
	TRACK_MEMORY(shortArray.items)

	if (!SUCCESS(status)) {
		FREE_MEMORY
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "getting SLC db short array device data");
	}

	// First item is the count
	shortArray.count = (int)*((short*)shortArray.items);

	// Copy shorts to new short array and free up returned short array
	short* shortData;
	ALLOCATE_COPY_AND_TRACK_MEMORY_AND_ON_ERROR_RETURN_(env, shortData, &((short*)shortArray.items)[1],
			shortArray.count * sizeof(short),
			"SLC db short array device data", shortArray)
	FREE_TRACKED_MEMORY(shortArray.items)
	shortArray.items = shortData;
	return shortArray;
}

/**
 * Get a integer array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the integer array
 */
Array aidaRequestIntegerArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	TRACK_ALLOCATED_MEMORY
	Array integerArray;
	integerArray.count = 0;
	integerArray.items = NULL;

	TO_SLC_NAME(uri, slcName)
	vmsstat_t status = JNI_DBGETINTA(slcName, (int**)(&integerArray.items));
	TRACK_MEMORY(integerArray.items)

	if (!SUCCESS(status)) {
		FREE_MEMORY
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "getting SLC db int array device data");
	}

	// First item is the count
	integerArray.count = *((int*)integerArray.items);

	// Copy integers to new integer array and free up returned integer array
	int* integerData;
	ALLOCATE_COPY_AND_TRACK_MEMORY_AND_ON_ERROR_RETURN_(env, integerData, &((int*)integerArray.items)[1],
			integerArray.count * sizeof(int),
			"SLC db integer array device data", integerArray)
	FREE_TRACKED_MEMORY(integerArray.items)
	integerArray.items = integerData;
	return integerArray;
}

/**
 * Get a long array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the long array
 */
Array aidaRequestLongArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	TRACK_ALLOCATED_MEMORY
	Array longArray;
	longArray.count = 0;
	longArray.items = NULL;

	TO_SLC_NAME(uri, slcName)
	vmsstat_t status = JNI_DBGETINTA(slcName, (int**)(&longArray.items));
	TRACK_MEMORY(longArray.items)

	if (!SUCCESS(status)) {
		FREE_MEMORY
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "getting SLC db long array device data");
	}

	// First item is the count
	longArray.count = *((int*)longArray.items);

	// Copy integers to new long array and free up returned integer array
	long* longData;
	ALLOCATE_AND_TRACK_MEMORY_AND_ON_ERROR_RETURN_(env, longData, longArray.count * sizeof(long),
			"SLC db long array device data", longArray)
	for (int i = 0; i < longArray.count; i++) {
		longData[i] = (long)((int*)longArray.items)[i + 1];
	}
	FREE_TRACKED_MEMORY(longArray.items)
	longArray.items = longData;
	return longArray;
}

/**
 * Get a float array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the float array
 */
Array aidaRequestFloatArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	TRACK_ALLOCATED_MEMORY
	Array floatArray;
	floatArray.count = 0;
	floatArray.items = NULL;

	TO_SLC_NAME(uri, slcName)
	vmsstat_t status = JNI_DBGETFLOATA(slcName, (float**)(&floatArray.items));
	TRACK_MEMORY(floatArray.items)

	if (!SUCCESS(status)) {
		FREE_MEMORY
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "getting SLC db float array device data");
	}

	// First item is the count
	floatArray.count = (int)*((float*)floatArray.items);

	// Copy floats to new float array and free up returned float array
	float* floatData;
	ALLOCATE_COPY_AND_TRACK_MEMORY_AND_ON_ERROR_RETURN_(env, floatData, &((float*)floatArray.items)[1],
			floatArray.count * sizeof(float),
			"SLC db float array device data", floatArray)
	FREE_TRACKED_MEMORY(floatArray.items)
	floatArray.items = floatData;
	return floatArray;
}

/**
 * Get a double array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the double array
 */
Array aidaRequestDoubleArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	TRACK_ALLOCATED_MEMORY
	Array doubleArray;
	doubleArray.count = 0;
	doubleArray.items = NULL;

	TO_SLC_NAME(uri, slcName)
	vmsstat_t status = JNI_DBGETFLOATA(slcName, (float**)(&doubleArray.items));
	TRACK_MEMORY(doubleArray.items)

	if (!SUCCESS(status)) {
		FREE_MEMORY
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "getting SLC db double array device data");
	}

	// First item is the count
	doubleArray.count = (int)*((float*)doubleArray.items);

	// Copy floats to new double array and free up returned float array
	double* doubleData;
	ALLOCATE_AND_TRACK_MEMORY_AND_ON_ERROR_RETURN_(env, doubleData, doubleArray.count * sizeof(double),
			"SLC db float array device data",
			doubleArray)
	for (int i = 0; i < doubleArray.count; i++) {
		doubleData[i] = ((float*)doubleArray.items)[i + 1];
	}
	FREE_TRACKED_MEMORY(doubleArray.items)
	doubleArray.items = doubleData;

	return doubleArray;
}

/**
 * Get a string array:
 *
 * If the primary is ASTS and the pseudo-secondary is
 * either CTRL, STAT, VSTA,
 * parse the text, color, and flag fields from the string
 * returned by aidaRequestString().  The text, color, and flag
 * substrings are then returned in an array
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the string array
 */
StringArray aidaRequestStringArray(JNIEnv* env, const char* uri, Arguments arguments)
{
	TRACK_ALLOCATED_MEMORY
	const char* secondary = secondaryFromUri(uri);
	// If not ASTS and CTRL, STAT or VSTA
	if (!startsWith(uri, "ASTS") || (strcasecmp(secondary, "CTRL") != 0 && strcasecmp(secondary, "STAT") != 0
			&& strcasecmp(secondary, "VSTA") != 0)) {
		UNSUPPORTED_STRING_ARRAY_REQUEST
	}
	StringArray stringArray;
	stringArray.count = 0;

	char* colorString = aidaRequestString(env, uri, arguments);
	ON_EXCEPTION_RETURN_(stringArray)
	TRACK_MEMORY(colorString)

	//	  The substring returned by aidaRequestString is 19
	//	  characters long and contains the text substring
	//	  in characters 0-7, the color substring in
	//	  characters 9-16, and the flag substring in
	//	  character position 18.

	// Space for pointers for each string
	stringArray.count = 3;
	ALLOCATE_AND_TRACK_MEMORY_AND_ON_ERROR_RETURN_(env, stringArray.items, stringArray.count * sizeof(char*),
			"color string array", stringArray)
	char** strings = ((char**)(stringArray.items));

	//Create a local version of the string and free the original
	char localColorString[TOTAL_PSEUDO_SECONDARY_LEN + 1];
	memset(localColorString, ' ', TOTAL_PSEUDO_SECONDARY_LEN);
	if (colorString) {
		memcpy(localColorString, colorString, strlen(colorString));
		localColorString[TOTAL_PSEUDO_SECONDARY_LEN] = 0x0;
		FREE_TRACKED_MEMORY(colorString)
	}

	ALLOCATE_AND_TRACK_FIXED_LENGTH_STRING_AND_ON_ERROR_RETURN_(env, strings[0],
			localColorString,
			TEXT_SUBSTRING + 1, "text substring in color string array", stringArray)
	ALLOCATE_AND_TRACK_FIXED_LENGTH_STRING_AND_ON_ERROR_RETURN_(env, strings[1],
			&localColorString[TEXT_SUBSTRING + 1],
			COLOR_SUBSTRING + 1, "color substring in color string array", stringArray)
	ALLOCATE_AND_TRACK_FIXED_LENGTH_STRING_AND_ON_ERROR_RETURN_(env, strings[2],
			&localColorString[TEXT_SUBSTRING + COLOR_SUBSTRING + 2],
			FLAG_SUBSTRING + 1, "flag substring in color string array", stringArray)
	return stringArray;
}

/**
 * Get a table of data
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the table
 */
Table aidaRequestTable(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Get table type parameter
	char* specifiedType;
	if (ascanf(env, &arguments, "%s", "TABLE_TYPE", &specifiedType)) {
		RETURN_NULL_TABLE
	}

	// Make a table
	Table table;
	memset(&table, 0, sizeof(table));
	table.columnCount = 0;

	// Add column based on TYPE
	if (strcasecmp(specifiedType, "FLOAT") == 0) {
		float value = aidaRequestFloat(env, uri, arguments);
		ON_EXCEPTION_RETURN_(table)
		table = tableCreate(env, 1, 1);
		ON_EXCEPTION_RETURN_(table)
		tableAddSingleRowFloatColumn(env, &table, value, true);
		ON_EXCEPTION_RETURN_(table)
	} else if (strcasecmp(specifiedType, "DOUBLE") == 0) {
		double value = aidaRequestDouble(env, uri, arguments);
		ON_EXCEPTION_RETURN_(table)
		table = tableCreate(env, 1, 1);
		ON_EXCEPTION_RETURN_(table)
		tableAddSingleRowDoubleColumn(env, &table, true, false);
		ON_EXCEPTION_RETURN_(table)
	} else if (strcasecmp(specifiedType, "BYTE") == 0) {
		char value = aidaRequestByte(env, uri, arguments);
		ON_EXCEPTION_RETURN_(table)
		table = tableCreate(env, 1, 1);
		ON_EXCEPTION_RETURN_(table)
		tableAddSingleRowByteColumn(env, &table, value);
		ON_EXCEPTION_RETURN_(table)
	} else if (strcasecmp(specifiedType, "SHORT") == 0) {
		short value = aidaRequestShort(env, uri, arguments);
		ON_EXCEPTION_RETURN_(table)
		table = tableCreate(env, 1, 1);
		ON_EXCEPTION_RETURN_(table)
		tableAddSingleRowShortColumn(env, &table, value);
		ON_EXCEPTION_RETURN_(table)
	} else if (strcasecmp(specifiedType, "INTEGER") == 0) {
		int value = aidaRequestInteger(env, uri, arguments);
		ON_EXCEPTION_RETURN_(table)
		table = tableCreate(env, 1, 1);
		ON_EXCEPTION_RETURN_(table)
		tableAddSingleRowIntegerColumn(env, &table, value);
		ON_EXCEPTION_RETURN_(table)
	} else if (strcasecmp(specifiedType, "LONG") == 0) {
		long value = aidaRequestLong(env, uri, arguments);
		ON_EXCEPTION_RETURN_(table)
		table = tableCreate(env, 1, 1);
		ON_EXCEPTION_RETURN_(table)
		tableAddSingleRowLongColumn(env, &table, value);
		ON_EXCEPTION_RETURN_(table)
	} else if (strcasecmp(specifiedType, "BOOLEAN") == 0) {
		int value = aidaRequestBoolean(env, uri, arguments);
		ON_EXCEPTION_RETURN_(table)
		table = tableCreate(env, 1, 1);
		ON_EXCEPTION_RETURN_(table)
		tableAddSingleRowBooleanColumn(env, &table, value);
		ON_EXCEPTION_RETURN_(table)
	} else if (strcasecmp(specifiedType, "STRING") == 0) {
		char* value = aidaRequestString(env, uri, arguments);
		ON_EXCEPTION_RETURN_(table)
		table = tableCreate(env, 1, 1);
		ON_EXCEPTION_RETURN_(table)
		tableAddSingleRowStringColumn(env, &table, value);
		ON_EXCEPTION_RETURN_(table)

	} else if (strcasecmp(specifiedType, "FLOAT_ARRAY") == 0) {
		Array value = aidaRequestFloatArray(env, uri, arguments);
		ON_EXCEPTION_RETURN_(table)
		table = tableCreate(env, value.count, 1);
		ON_EXCEPTION_RETURN_(table)
		tableAddColumn(env, &table, AIDA_FLOAT_ARRAY_TYPE, value.items, true);
		ON_EXCEPTION_RETURN_(table)
	} else if (strcasecmp(specifiedType, "DOUBLE_ARRAY") == 0) {
		Array value = aidaRequestDoubleArray(env, uri, arguments);
		ON_EXCEPTION_RETURN_(table)
		table = tableCreate(env, value.count, 1);
		ON_EXCEPTION_RETURN_(table)
		tableAddColumn(env, &table, AIDA_DOUBLE_ARRAY_TYPE, value.items, true);
		ON_EXCEPTION_RETURN_(table)
	} else if (strcasecmp(specifiedType, "BYTE_ARRAY") == 0) {
		Array value = aidaRequestByteArray(env, uri, arguments);
		ON_EXCEPTION_RETURN_(table)
		table = tableCreate(env, value.count, 1);
		ON_EXCEPTION_RETURN_(table)
		tableAddColumn(env, &table, AIDA_BYTE_ARRAY_TYPE, value.items, false);
		ON_EXCEPTION_RETURN_(table)
	} else if (strcasecmp(specifiedType, "SHORT_ARRAY") == 0) {
		Array value = aidaRequestShortArray(env, uri, arguments);
		ON_EXCEPTION_RETURN_(table)
		table = tableCreate(env, value.count, 1);
		ON_EXCEPTION_RETURN_(table)
		tableAddColumn(env, &table, AIDA_SHORT_ARRAY_TYPE, value.items, false);
		ON_EXCEPTION_RETURN_(table)
	} else if (strcasecmp(specifiedType, "INTEGER_ARRAY") == 0) {
		Array value = aidaRequestIntegerArray(env, uri, arguments);
		ON_EXCEPTION_RETURN_(table)
		table = tableCreate(env, value.count, 1);
		ON_EXCEPTION_RETURN_(table)
		tableAddColumn(env, &table, AIDA_INTEGER_ARRAY_TYPE, value.items, false);
		ON_EXCEPTION_RETURN_(table)
	} else if (strcasecmp(specifiedType, "LONG_ARRAY") == 0) {
		Array value = aidaRequestLongArray(env, uri, arguments);
		ON_EXCEPTION_RETURN_(table)
		table = tableCreate(env, value.count, 1);
		ON_EXCEPTION_RETURN_(table)
		tableAddColumn(env, &table, AIDA_LONG_ARRAY_TYPE, value.items, false);
		ON_EXCEPTION_RETURN_(table)
	} else if (strcasecmp(specifiedType, "BOOLEAN_ARRAY") == 0) {
		Array value = aidaRequestBooleanArray(env, uri, arguments);
		ON_EXCEPTION_RETURN_(table)
		table = tableCreate(env, value.count, 1);
		ON_EXCEPTION_RETURN_(table)
		tableAddColumn(env, &table, AIDA_BOOLEAN_ARRAY_TYPE, value.items, false);
		ON_EXCEPTION_RETURN_(table)
	}

	// Return the table
	return table;
}

/**
 * Set a value
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @param value to set
 */
void aidaSetValue(JNIEnv* env, const char* uri, Arguments arguments, Value value)
{
	TRACK_ALLOCATED_MEMORY

	if (!JNI_DBACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"Aida access for SLC Database set operations are not currently enabled");
		return;
	}

	// Get optional VALUE_TYPE parameter
	char* specifiedValueType = "FLOAT_ARRAY";
	if (ascanf(env, &arguments, "%os", "VALUE_TYPE", &specifiedValueType)) {
		return;
	}

	vmsstat_t status;
	unsigned int length;
	TO_SLC_NAME(uri, slcName)

	if (strcasecmp(specifiedValueType, "FLOAT_ARRAY") == 0) {
		// If VALUE_TYPE is FLOAT_ARRAY then set float array
		float* data;
		if (avscanf(env, &arguments, &value, "%fa", "value", &data, &length)) {
			return;
		}
		TRACK_MEMORY(data)
		CONVERT_TO_VMS_FLOAT(data, length)
		status = JNI_DBSETFLOAT(slcName, data, (int)length);
	} else if (strcasecmp(specifiedValueType, "INTEGER_ARRAY") == 0) {
		// If VALUE_TYPE is INTEGER_ARRAY then set int array
		int* data;
		if (avscanf(env, &arguments, &value, "%da", "value", &data, &length)) {
			return;
		}
		TRACK_MEMORY(data)
		status = JNI_DBSETINT(slcName, data, (int)length);
	} else {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Invalid VALUE_TYPE specified for SLC Database set operation");
		return;
	}

	FREE_MEMORY
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "setting SLC db array device data");
	}
}
