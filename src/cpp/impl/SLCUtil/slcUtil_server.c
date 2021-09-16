/**
 * Utility Server implementation
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <descrip.h>              /*  for definition of $DESCRIPTOR  */

#include <cvtdef.h>             /* CVT$K_*, CVT$M_* */
#include <cvtmsg.h>             /* CVT$_NORMAL */
#include <cvt$routines.h>       /* CVT$CONVERT_FLOAT */
#include <stdbool.h>

#include "aida_server_helper.h"
#include "json.h"

#include "slc_macros.h"    /* vmsstat_t, int2u, int4u, etc. */
#include "msg_proto.h"     /* for standalone_init */
#include "sysutil_proto.h" /* for cvt_vms_to_ieee_flt */

#include "slcUtil_server.h"

static int
getBgrpArguments(JNIEnv* env, Arguments arguments, Value value, char** bgrp, char** varname, char** valueString);
static Table setTriggerValue(JNIEnv* env, const char* uri, Arguments arguments, Value value);
static Table setMkbValue(JNIEnv* env, const char* uri, Arguments arguments, Value value);
static int getMkbArguments(JNIEnv* env, Arguments arguments, Value value, char** mkb, float* floatValue);
static bool getTrigArguments(JNIEnv* env, Arguments arguments, Value value, int* beam, short* flag);
static bool getTrigBeamArgument(JNIEnv* env, Arguments arguments, int* beam);

/**
 * Initialise the service
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @throws ServerInitialisationException if the service fails to initialise
 */
void aidaServiceInit(JNIEnv* env)
{
	vmsstat_t status;
	if (!SUCCESS(status = DPSLCUTIL_DB_INIT())) {
		aidaThrow(env, status, SERVER_INITIALISATION_EXCEPTION, "initialising Utility Service");
		return;
	}

	printf("Aida Utility Service Initialised\n");
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
	vmsstat_t status = 0;
	short trig_status;
	int beam;

	if (getTrigBeamArgument(env, arguments, &beam)) {
		return 0;
	}

	status = DPSLCUTIL_TRIG_GETSTATUS((char*)uri, beam, &trig_status);
	if (!SUCCESS(status)) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"Unable to get beam status");
	}

	return trig_status;
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
	vmsstat_t status = 0;
	short trig_status;
	int beam;

	if (getTrigBeamArgument(env, arguments, &beam)) {
		return 0;
	}

	status = DPSLCUTIL_TRIG_GETSTATUS((char*)uri, beam, &trig_status);
	if (!SUCCESS(status)) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"Unable to get beam status");
	}

	return (int)trig_status;
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
	vmsstat_t status = 0;
	short trig_status;
	int beam;

	if (getTrigBeamArgument(env, arguments, &beam)) {
		return NULL;
	}

	status = DPSLCUTIL_TRIG_GETSTATUS((char*)uri, beam, &trig_status);
	if (!SUCCESS(status)) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"Unable to get beam status");
		return NULL;
	}

	char* string = malloc(15);
	if (trig_status) {
		strcpy(string, "activated");
	} else {
		strcpy(string, "deactivated");
	}

	return string;
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
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	return table;
}

/**
 * Set a value: BGRP Set Variable
 *  Parameters:
 *    BGRP    	: The BGRP name
 *    VARNAME   : BGRP variable name for the specified BGRP
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @param value to set
 * @return the table
 */
void aidaSetValue(JNIEnv* env, const char* uri, Arguments arguments, Value value)
{
	vmsstat_t status = 0;
	char* bgrp, * varname, * valueString;

	if (!DPSLCUTIL_BGRP_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Aida access to BGRP set variable operations is not currently enabled");
		return;
	}

	if (getBgrpArguments(env, arguments, value, &bgrp, &varname, &valueString)) {
		return;
	}

	status = DPSLCUTIL_BGRP_SETVAR(bgrp, varname, valueString);
	if (!SUCCESS(status)) {
		if ((status == MEMORY_ALLOCATE_FAIL) ||
				(status == NO_BGRP_NAMES) ||
				(status == BGRP_NAME_NOT_FOUND) ||
				(status == NO_BGRP_VARIABLES) ||
				(status == BGRP_VARIABLE_NOT_FOUND)) {
			aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION,
					"Failed to set data");
		} else {
			aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION,
					"Failed to set data");
		}
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
	if (strcasecmp(uri, "MKB//VAL") == 0) {
		// set Multi Knob value
		return setMkbValue(env, uri, arguments, value);
	} else {
		// set TRIGGER value
		return setTriggerValue(env, uri, arguments, value);
	}
}

static Table setMkbValue(JNIEnv* env, const char* uri, Arguments arguments, Value value)
{
	Table table;
	memset(&table, 0, sizeof(table));
	table.columnCount = 0;
	vmsstat_t status = 0;

	if (!DPSLCUTIL_MKB_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Aida multiknob operations are not currently enabled");
		return table;
	}

	char* mkb;
	float valueFloat;

	if (getMkbArguments(env, arguments, value, &mkb, &valueFloat)) {
		return table;
	}

	int num_devices;
	float relative_delta_array[1];
	int2u one = 1;

	/* CVT_IEEE_TO_VMS_FLT(relative_delta_array, relative_delta_array, &one);*/
	status = CVT$CONVERT_FLOAT((void*)&valueFloat, CVT$K_IEEE_S, (void*)&valueFloat, CVT$K_VAX_F, 0);
	if (status != CVT$_NORMAL) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"can't convert floating point numbers to the right format");
		return table;
	}

	status = DPSLCUTIL_DO_MKB(mkb, &valueFloat, &num_devices);
	if (!SUCCESS(status)) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"unable to set value");
		return table;
	}

	// Now create table to return

	// Set columns
	table.columnCount = 2;
	table.rowCount = num_devices;

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
			tableStringColumn(&table, column, MAX_DEVICE_STRING_LEN);
			break;
		case 1: // values
			tableFloatColumn(&table, column);
			break;
		default: // unsupported
			fprintf(stderr, "Unsupported table column type: %d\n", table.types[column]);
			break;
		}
	}

	// Get all data
	char* names = (char*)malloc((num_devices * MAX_DEVICE_STRING_LEN) + 1);
	int2u M;
	float* floatArray;
	unsigned long* longArray;

	// Names
	DPSLCUTIL_MKB_GETNAMES(names);
	// Copy names to allocated space
	for (int row = 0; row < num_devices; row++) {
		memcpy(stringArray[row], names + (row * MAX_DEVICE_STRING_LEN), MAX_DEVICE_STRING_LEN);
	}
	free(names);

	// Secondary values
	floatArray = (float*)(table.ppData[1]);
	DPSLCUTIL_MKB_GETSECNVALUES(floatArray);
	M = (int2u)num_devices;
	CVT_VMS_TO_IEEE_FLT(floatArray, floatArray, &M);

	// cleanup
	DPSLCUTIL_MKB_GETCLEANUP();

	return table;
}

static Table setTriggerValue(JNIEnv* env, const char* uri, Arguments arguments, Value value)
{
	Table table;
	memset(&table, 0, sizeof(table));
	table.columnCount = 0;
	vmsstat_t status = 0;

	if (!DPSLCUTIL_TRIG_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Aida access to trigger operations is not currently enabled");
		return table;
	}

	int beam;
	short flag;
	if (getTrigArguments(env, arguments, value, &beam, &flag)) {
		return table;
	}

	int num_devices;
	float relative_delta_array[1];
	int2u one = 1;

	status = DPSLCUTIL_TRIG_SETDEACTORREACT((char *)uri, flag, beam);
	if (!SUCCESS(status)) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"unable to set value");
		return table;
	}

	// Read back status
	status = DPSLCUTIL_TRIG_GETSTATUS((char *)uri, beam, &flag);
	if (!SUCCESS(status)) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"unable to read-back value");
		return table;
	}

	// Now create table to return the flag

	// Set columns
	table.columnCount = 1;
	table.rowCount = 1;

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
			tableShortColumn(&table, column);
			break;
		default: // unsupported
			fprintf(stderr, "Unsupported table column type: %d\n", table.types[column]);
			break;
		}
	}

	// Get all data
	short* shortArray;

	// Secondary values
	shortArray = (short*)(table.ppData[0]);
	shortArray[0] = flag;

	return table;
}

static bool getTrigArguments(JNIEnv* env, Arguments arguments, Value value, int* beam, short* flag)
{
	if (value.type != AIDA_STRING_TYPE) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
				"Trig Set Variable requires a AValue parameter to set to a short");
		return 1;
	}
	sscanf(value.value.stringValue, "%hi", flag);

	getTrigBeamArgument(env, arguments, beam);

	return 0;
}

static bool getTrigBeamArgument(JNIEnv* env, Arguments arguments, int* beam)
{
	Argument argument = getArgument(arguments, "beam");
	if (!argument.name) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
				"'Trig Set Variable requires a beam parameter");
		return 2;
	}
	*beam = getIntegerArgument(argument);

	return 0;
}

static int getMkbArguments(JNIEnv* env, Arguments arguments, Value value, char** mkb, float* floatValue)
{
	if (value.type != AIDA_STRING_TYPE) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
				"'MKB Set Variable requires a AValue parameter to set to a float");
		return 1;
	}
	sscanf(value.value.stringValue, "%f", floatValue);

	Argument argument = getArgument(arguments, "mkb");
	if (!argument.name) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
				"'MKB Set Variable requires a MKB parameter");
		return 2;
	}
	*mkb = argument.value;

	return 0;
}

static int
getBgrpArguments(JNIEnv* env, Arguments arguments, Value value, char** bgrp, char** varname, char** valueString)
{
	if (value.type != AIDA_STRING_TYPE) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
				"'BGRP Set Variable requires a AValue parameter to be 'Y' or 'N'");
		return 1;
	}
	*valueString = value.value.stringValue;
	if (strcasecmp(*valueString, "Y") != 0 && strcasecmp(*valueString, "N") != 0) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
				"'BGRP Set Variable requires a AValue parameter to be 'Y' or 'N'");
		return 2;
	}

	Argument argument = getArgument(arguments, "bgrp");
	if (!argument.name) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
				"'BGRP Set Variable requires a BGRP parameter");
		return 3;
	}
	*bgrp = argument.value;

	argument = getArgument(arguments, "varname");
	if (!argument.name) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
				"'BGRP Set Variable requires a VARNAME parameter");
		return 4;
	}
	*varname = argument.value;
	return 0;
}

