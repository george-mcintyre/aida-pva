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

static Table setTriggerValue(JNIEnv* env, const char* uri, Arguments arguments, Value value);
static Table setMkbValue(JNIEnv* env, const char* uri, Arguments arguments, Value value);

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
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param channelName
 * @param forGetter true to return config for getter, false for setter
 * @return the channel config.  Leave empty if you don't have any specific configuration overrides
 */
Config aidaChannelConfig(JNIEnv* env, const char* channelName, short forGetter)
{
	DEFAULT_CONFIG_REQUEST
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
	// Get the arguments
	int beam;
	if (ascanf(env, &arguments, "%d", "beam", &beam)) {
		return 0;
	}

	// Read the status
	vmsstat_t status;
	short trig_status;
	status = DPSLCUTIL_TRIG_GETSTATUS((char*)uri, beam, &trig_status);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "Unable to get beam status");
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
	// Get the arguments
	int beam;
	if (ascanf(env, &arguments, "%d", "beam", &beam)) {
		return 0;
	}

	// Read the status
	vmsstat_t status;
	short trig_status;
	status = DPSLCUTIL_TRIG_GETSTATUS((char*)uri, beam, &trig_status);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "Unable to get beam status");
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
	// Get the arguments
	int beam;

	if (ascanf(env, &arguments, "%d", "beam", &beam)) {
		return NULL;
	}

	// Read the status
	vmsstat_t status;
	short trig_status;
	status = DPSLCUTIL_TRIG_GETSTATUS((char*)uri, beam, &trig_status);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "Unable to get beam status");
		return NULL;
	}

	// Return the value
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
	UNSUPPORTED_ARRAY_REQUEST
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
	UNSUPPORTED_ARRAY_REQUEST
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
	UNSUPPORTED_ARRAY_REQUEST
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
	UNSUPPORTED_ARRAY_REQUEST
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
	UNSUPPORTED_ARRAY_REQUEST
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
	UNSUPPORTED_ARRAY_REQUEST
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
	UNSUPPORTED_ARRAY_REQUEST
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
	UNSUPPORTED_STRING_ARRAY_REQUEST
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
	UNSUPPORTED_TABLE_REQUEST
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

	// Check if operations are enabled?
	if (!DPSLCUTIL_BGRP_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Aida access to BGRP set variable operations is not currently enabled");
		return;
	}

	if (avscanf(env, &arguments, &value, "%s %s %s",
			"value", &valueString,
			"bgrp", &bgrp,
			"varname", &varname
	)) {
		return;
	}

	if (strcasecmp(valueString, "Y") != 0 && strcasecmp(valueString, "N") != 0) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
				"'BGRP Set Variable requires a AValue parameter to be 'Y' or 'N'");
		return;
	}

	// Set the value
	status = DPSLCUTIL_BGRP_SETVAR(bgrp, varname, valueString);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "Failed to set data");
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

/**
 * Set Multi-knob value
 *
 * @param env
 * @param uri
 * @param arguments
 * @param value
 * @return
 */
static Table setMkbValue(JNIEnv* env, const char* uri, Arguments arguments, Value value)
{
	// Check if operations are enabled?
	if (!DPSLCUTIL_MKB_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Aida multiknob operations are not currently enabled");
		RETURN_NULL_TABLE;
	}

	// Get arguments
	char* mkb;
	float floatValue;
	if (ascanf(env, &arguments, "%f %s",
			"value", &floatValue,
			"mkb", &mkb
	)) {
		RETURN_NULL_TABLE;
	}
	CONVERT_TO_VMS_FLOAT(floatValue)

	// Set the value
	int num_devices;
	vmsstat_t status;
	status = DPSLCUTIL_DO_MKB(mkb, &floatValue, &num_devices);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "unable to set value");
		RETURN_NULL_TABLE;
	}

	// Now get the new values to return

	// To hold the data
	char namesData[(num_devices * MAX_DEVICE_STRING_LEN)];
	float secondaryValuesData[num_devices];

	// Names
	DPSLCUTIL_MKB_GETNAMES(namesData);

	// Secondary values
	DPSLCUTIL_MKB_GETSECNVALUES(secondaryValuesData);

	// cleanup
	DPSLCUTIL_MKB_GETCLEANUP();

	// Now create table to return
	Table table = tableCreate(env, num_devices, 2);
	CHECK_EXCEPTION(table)
	tableAddFixedWidthStringColumn(env, &table, namesData, MAX_DEVICE_STRING_LEN);
	CHECK_EXCEPTION(table)
	tableAddColumn(env, &table, AIDA_FLOAT_TYPE, secondaryValuesData);

	return table;
}

/**
 * Set Trigger value
 *
 * @param env
 * @param uri
 * @param arguments
 * @param value
 * @return
 */
static Table setTriggerValue(JNIEnv* env, const char* uri, Arguments arguments, Value value)
{
	// Check if operations are enabled?
	if (!DPSLCUTIL_TRIG_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Aida access to trigger operations is not currently enabled");
		RETURN_NULL_TABLE;
	}

	// Get arguments
	int beam;
	short flag;
	if (avscanf(env, &arguments, &value, "%hd %d",
			"value", &flag,
			"beam", &beam
	)) {
		RETURN_NULL_TABLE;
	}

	// Set the trigger value
	vmsstat_t status;
	status = DPSLCUTIL_TRIG_SETDEACTORREACT((char*)uri, flag, beam);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "unable to set value");
		RETURN_NULL_TABLE;
	}

	// Read back status
	status = DPSLCUTIL_TRIG_GETSTATUS((char*)uri, beam, &flag);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "unable to read-back value");
		RETURN_NULL_TABLE;
	}

	// Now create table to return the flag
	Table table = tableCreate(env, 1, 1);
	CHECK_EXCEPTION(table)
	tableAddSingleRowShortColumn(env, &table, flag);

	return table;
}
