/**
 * Klystron Server implementation
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <descrip.h>              /*  for definition of $DESCRIPTOR  */
#include <cvtdef.h>               /*  CVT$K_*, CVT$M_*  */
#include <cvtmsg.h>               /*  CVT$_NORMAL  */
#include <cvt$routines.h>         /*  CVT$CONVERT_FLOAT  */

#include "aida_server_helper.h"
#include "json.h"

#include "slcMagnet_server.h"

#include "slc_macros.h"    /* vmsstat_t, int2u, int4u, etc. */
#include "msg_proto.h"     /* for standalone_init */
#include "sysutil_proto.h" /* for cvt_vms_to_ieee_flt */

static int getSimpleMagnetArguments(JNIEnv* env, const char* uri, Value value, int* count,
		char** prim_list, char** micr_list, int** unit_list, int4u* secn, float** set_values);

static int getMagnetArguments(JNIEnv* env, const char* uri, Arguments arguments, Value value, int* count,
		char** prim_list, char** micr_list, int** unit_list, int4u* secn, float** set_values,
		char** magFunc, char** limitCheck);

/**
 * Initialise the service
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @throws ServerInitialisationException if the service fails to initialise
 */
void aidaServiceInit(JNIEnv* env)
{
	vmsstat_t status;

	if (!SUCCESS(status = DPSLCMAGNET_DB_INIT())) {
		aidaThrow(env, status, SERVER_INITIALISATION_EXCEPTION, "initialising SLC Magnet Service");
		return;
	}

	printf("Aida Magnet Service Initialised\n");
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
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	return 0;
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
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	return 0x0;
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
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	return 0;
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
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	return 0;
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
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	return 0;
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
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	return 0.0f;
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
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	return 0.0;
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
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	return NULL;
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
	Array array;
	array.count = 0;
	array.items = NULL;
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	return array;
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
	Array array;
	array.count = 0;
	array.items = NULL;
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	return array;
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
	Array array;
	array.count = 0;
	array.items = NULL;
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	return array;
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
	Array array;
	array.count = 0;
	array.items = NULL;
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	return array;
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
	Array array;
	array.count = 0;
	array.items = NULL;
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	return array;
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
	Array array;
	array.count = 0;
	array.items = NULL;
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	return array;
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
	Array array;
	array.count = 0;
	array.items = NULL;
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	return array;
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
	StringArray stringArray;
	stringArray.count = 0;
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
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
	table.columnCount = 0;
	table.rowCount = 0;

	vmsstat_t status = 0;
	char* micrPattern;
	char* unitPattern;

	Argument argument;

	// MICROS
	argument = getArgument(arguments, "micros");
	if (argument.name) {
		micrPattern = argument.value;
	}

	// UNITS
	argument = getArgument(arguments, "units");
	if (argument.name) {
		unitPattern = argument.value;
	}

	// Acquire Magnet values
	int num_magnet_pvs;
	status = DPSLCMAGNET_GET((char*)uri, micrPattern, unitPattern, &num_magnet_pvs);

	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "while reading magnet values");
		table.rowCount = 0;
		return table;
	}

	// Set columns
	table.columnCount = 2;

	// Allocate space for table data
	if (initTable(env, &table) == NULL) {
		return table;
	}

	// Allocate space for rows of data
	char** stringArray;
	stringArray = table.ppData[0];
	for (int column = 0; column < table.columnCount; column++) {
		switch (column) {
		case 0: // names
			tableStringColumn(&table, column, MAX_PMU_STRING_LEN);
			break;
		case 1: // secondary values
			tableFloatColumn(&table, column);
			break;
		default: // unsupported
			fprintf(stderr, "Unsupported table column type: %d\n", table.types[column]);
			break;
		}
	}

	// Get all data
	char* names = (char*)malloc((num_magnet_pvs * MAX_PMU_STRING_LEN) + 1);
	unsigned long nMagnetsRead = DPSLCMAGNET_GETNUMPVS();
	int2u M;
	float* floatArray;
	unsigned long* longArray;

	// Names
	DPSLCMAGNET_GETNAMES(names);
	// Copy names to allocated space
	for (int row = 0; row < nMagnetsRead; row++) {
		memcpy(stringArray[row], names + (row * MAX_PMU_STRING_LEN), MAX_PMU_STRING_LEN);
	}
	free(names);

	// Secondary values
	floatArray = (float*)(table.ppData[1]);
	DPSLCMAGNET_GETSECNVALUES(floatArray);
	M = (int2u)nMagnetsRead;
	CVT_VMS_TO_IEEE_FLT(floatArray, floatArray, &M);

	// Cleanup
	DPSLCMAGNET_GETCLEANUP();

	// All read successfully
	return table;
}

/**
 * Set a value
 * Sets specified BCON secondary values for specified array of magnet names to an array of corresponding set values.
 * The parameter `value` argument is Value containing two elements: an array of magnet names (name)
 * and an array of corresponding set values (values)
 * e.g. { "names": [], "values": [] }
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @param value to set
 * @return the table
 */
void aidaSetValue(JNIEnv* env, const char* uri, Arguments arguments, Value value)
{
	int status;
	int count;
	char* prim_list, * micr_list;
	int* unit_list;
	int4u secn;
	float* set_values;

	if (getSimpleMagnetArguments(env, uri, value, &count, &prim_list, &micr_list, &unit_list, &secn, &set_values)) {
		return;
	}

	// set the PVs specified by the lists of primary, micros, and units
	status = DPSLCMAGNET_SETCONFIG(count, prim_list, micr_list, unit_list, secn, set_values);

	free(prim_list);
	free(micr_list);
	free(unit_list);
	free(set_values);
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
	int status;
	Table table;
	memset(&table, 0, sizeof(table));
	table.columnCount = 0;

	int count;
	int4u secn;
	char* prim_list, * micr_list, * magFunc, * limitCheck;
	int* unit_list;
	float* set_values;

	if (getMagnetArguments(env, uri, arguments, value, &count, &prim_list, &micr_list, &unit_list, &secn, &set_values,
			&magFunc, &limitCheck)) {
		return table;
	}

	// set the PVs specified by the lists of primary, micros, and units
	status = DPSLCMAGNET_SET(count, prim_list, micr_list, unit_list, secn, set_values, magFunc);

	// NOW RETURN STATUS
	table.rowCount = DPSLCMAGNET_RET_SETNUMPVS();

	// Set columns
	table.columnCount = 2;

	// Allocate space for table data
	if (initTable(env, &table) == NULL) {
		return table;
	}

	// Allocate space for rows of data
	char** stringArray;
	stringArray = table.ppData[0];
	for (int column = 0; column < table.columnCount; column++) {
		switch (column) {
		case 0: // names
			tableStringColumn(&table, column, MAX_STATE_NAME_LEN + 1);
			break;
		case 1: // x
			tableFloatColumn(&table, column);
			break;
		default: // unsupported
			fprintf(stderr, "Unsupported table column type: %d\n", table.types[column]);
			break;
		}
	}

	// Names
	char* names = (char*)malloc((table.rowCount * MAX_STATE_NAME_LEN) + 1);
	DPSLCMAGNET_RET_STATENAMES(names);
	// Copy names to allocated space
	for (int row = 0; row < table.rowCount; row++) {
		memcpy(stringArray[row], names + (row * MAX_STATE_NAME_LEN), MAX_STATE_NAME_LEN);
		stringArray[row][MAX_STATE_NAME_LEN] = '\0';
	}
	free(names);

	// Status
	int2u M;
	float* floatArray = (float*)(table.ppData[1]);
	DPSLCMAGNET_RET_BACTVALUES(floatArray);
	M = (int2u)table.rowCount;
	CVT_VMS_TO_IEEE_FLT(floatArray, floatArray, &M);

	return table;
}

static int getSimpleMagnetArguments(JNIEnv* env, const char* uri, Value value, int* count,
		char** prim_list, char** micr_list, int** unit_list, int4u* secn, float** set_values)
{
	int status;
	if (value.type != AIDA_JSON_TYPE ||
			value.value.jsonValue->type != json_object ||
			value.value.jsonValue->u.object.length != 2) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION, "setting Magnet BCON values");
		return 1;
	}

	// value contains an object with two congruent arrays
	// { "names": [], "values": [] }

	json_object_entry* v0 = &(value.value.jsonValue->u.object.values[0]);
	json_object_entry* v1 = &(value.value.jsonValue->u.object.values[1]);
	if (v0->value->type != json_array || v1->value->type != json_array) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION, "setting Magnet BCON values");
		return 2;
	}

	*count = (int)v0->value->u.array.length;
	if (*count != v1->value->u.array.length) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION, "setting Magnet BCON values");
		return 3;
	}

	json_value** nameArray, ** valuesArray;

	if (strcasecmp(v0->name, "names") == 0) {
		nameArray = v0->value->u.array.values;
		valuesArray = v1->value->u.array.values;
	} else {
		nameArray = v1->value->u.array.values;
		valuesArray = v0->value->u.array.values;
	}

	// Get names array
	*prim_list = (char*)malloc((*count * MAX_PRIM_NAME_LEN) + 1);
	*micr_list = (char*)malloc((*count * MAX_MICR_NAME_LEN) + 1);
	*unit_list = (int*)malloc(*count * sizeof(int));
	char name[MAX_PRIM_NAME_LEN + MAX_MICR_NAME_LEN + sizeof(int) + 4];
	for (int i = 0; i < *count; i++) {
		strcpy(name, nameArray[i]->u.string.ptr);
		char* prim = strtok(name, ":");
		strcpy(*prim_list + (i * MAX_PRIM_NAME_LEN), prim);
		char* micr = strtok(NULL, ":");
		strcpy(*micr_list + (i * MAX_PRIM_NAME_LEN), micr);
		char* unit = strtok(NULL, ":");
		sscanf(unit, "%d", unit_list[i]);
	}

	// Get values array
	*set_values = (float*)malloc(*count * sizeof(float));
	if (strcasecmp(v0->name, "names") == 0) {
		for (int i = 0; i < *count; i++) {
			*set_values[i] = (float)valuesArray[i]->u.dbl;
			status = CVT$CONVERT_FLOAT((void*)(*set_values + i), CVT$K_IEEE_S, (void*)(*set_values + i), CVT$K_VAX_F,
					0);
			if (status != CVT$_NORMAL) {
				aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
						"Illegal floating point number was provided");
				return 4;
			}
		}
	}

	// Secondary name
	char* secondary = strstr(uri, "//") + 2;
	// TODO This seems very bizarre - copying from a string to an integer (see http://www-mcc.slac.stanford.edu/ref_0/AIDASHR/DPSLCMAGNET_JNI.C)
	*secn;
	memcpy(secn, secondary, MAX_SECN_NAME_LEN);

	return 0;
}

static int getMagnetArguments(JNIEnv* env, const char* uri, Arguments arguments, Value value, int* count,
		char** prim_list, char** micr_list, int** unit_list, int4u* secn, float** set_values,
		char** magFunc, char** limitCheck)
{
	int status;

	if (value.type != AIDA_JSON_TYPE ||
			value.value.jsonValue->type != json_object ||
			value.value.jsonValue->u.object.length != 2) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION, "setting Magnet BCON values");
		return 1;
	}

	// value contains an object with two congruent arrays
	// { "names": [], "values": [] }

	json_object_entry* v0 = &(value.value.jsonValue->u.object.values[0]);
	json_object_entry* v1 = &(value.value.jsonValue->u.object.values[1]);
	if (v0->value->type != json_array || v1->value->type != json_array) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION, "setting Magnet BCON values");
		return 2;
	}

	*count = (int)v0->value->u.array.length;
	if (*count != v1->value->u.array.length) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION, "setting Magnet BCON values");
		return 3;
	}

	json_value** nameArray, ** valuesArray;

	if (strcasecmp(v0->name, "names") == 0) {
		nameArray = v0->value->u.array.values;
		valuesArray = v1->value->u.array.values;
	} else {
		nameArray = v1->value->u.array.values;
		valuesArray = v0->value->u.array.values;
	}

	// Get names array
	*prim_list = (char*)malloc((*count * MAX_PRIM_NAME_LEN) + 1);
	*micr_list = (char*)malloc((*count * MAX_MICR_NAME_LEN) + 1);
	*unit_list = (int*)malloc(*count * sizeof(int));
	char name[MAX_PRIM_NAME_LEN + MAX_MICR_NAME_LEN + sizeof(int) + 4];
	for (int i = 0; i < *count; i++) {
		strcpy(name, nameArray[i]->u.string.ptr);
		char* prim = strtok(name, ":");
		strcpy(*prim_list + (i * MAX_PRIM_NAME_LEN), prim);
		char* micr = strtok(NULL, ":");
		strcpy(*micr_list + (i * MAX_PRIM_NAME_LEN), micr);
		char* unit = strtok(NULL, ":");
		sscanf(unit, "%d", unit_list[i]);
	}

	// Get values array
	*set_values = (float*)malloc(*count * sizeof(float));
	if (strcasecmp(v0->name, "names") == 0) {
		for (int i = 0; i < *count; i++) {
			(*set_values)[i] = (float)valuesArray[i]->u.dbl;
			status = CVT$CONVERT_FLOAT((void*)(*set_values + i), CVT$K_IEEE_S, (void*)(*set_values + i), CVT$K_VAX_F,
					0);
			if (status != CVT$_NORMAL) {
				aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
						"Illegal floating point number was provided");
				return 4;
			}
		}
	}

	// Secondary name
	char* secondary = strstr(uri, "//") + 2;
	// TODO This seems very bizarre - copying from a string to an integer (see http://www-mcc.slac.stanford.edu/ref_0/AIDASHR/DPSLCMAGNET_JNI.C)
	memcpy(secn, secondary, MAX_SECN_NAME_LEN);

	Argument argument;

	// magfunc is required variable
	argument = getArgument(arguments, "magfunc");
	if (!argument.name) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION, "magfunc");
		return 4;
	}
	*magFunc = argument.value;

	// limitcheck is an optional variable
	argument = getArgument(arguments, "limitcheck");
	*limitCheck = "ALL";
	if (argument.name) {
		*limitCheck = argument.value;
	}

	return 0;
}

