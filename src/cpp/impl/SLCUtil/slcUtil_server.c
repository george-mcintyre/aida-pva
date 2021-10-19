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
static short getTrigStatus(JNIEnv * env, const char* uri, Arguments arguments);

// API Stubs
REQUEST_STUB_BOOLEAN
REQUEST_STUB_BYTE
REQUEST_STUB_LONG
REQUEST_STUB_FLOAT
REQUEST_STUB_DOUBLE
REQUEST_STUB_BOOLEAN_ARRAY
REQUEST_STUB_BYTE_ARRAY
REQUEST_STUB_SHORT_ARRAY
REQUEST_STUB_INTEGER_ARRAY
REQUEST_STUB_LONG_ARRAY
REQUEST_STUB_FLOAT_ARRAY
REQUEST_STUB_DOUBLE_ARRAY
REQUEST_STUB_STRING_ARRAY
REQUEST_STUB_TABLE

/**
 * Initialise the service
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
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
 * Get a short
 *
 * @param uri the uri
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param arguments the arguments
 * @return the short
 */
short aidaRequestShort(JNIEnv* env, const char* uri, Arguments arguments)
{
	return getTrigStatus(env, uri, arguments);
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
	return (int)getTrigStatus(env, uri, arguments);
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
	// Read the status
	short trig_status = getTrigStatus(env, uri, arguments);
	ON_EXCEPTION_RETURN_(NULL)

	if (trig_status) {
		return ALLOCATE_STRING(env, "activated","string");
	} else {
		return ALLOCATE_STRING(env, "deactivated", "string");
	}
}

/**
 * Set a value: BGRP Set Variable
 *  Parameters:
 *    BGRP    	: The BGRP name
 *    VARNAME   : BGRP variable name for the specified BGRP
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @param value to set
 * @return the table
 */
void aidaSetValue(JNIEnv* env, const char* uri, Arguments arguments, Value value)
{
	TRACK_ALLOCATED_MEMORY

	vmsstat_t status;
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

	TRACK_MEMORY(bgrp)
	TRACK_MEMORY(varname)
	TRACK_MEMORY(valueString)

	if (strcasecmp(valueString, "Y") != 0 && strcasecmp(valueString, "N") != 0) {
		FREE_MEMORY
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
				"'BGRP Set Variable requires Value argument to be 'Y' or 'N'");
		return;
	}

	// Set the value
	status = DPSLCUTIL_BGRP_SETVAR(bgrp, varname, valueString);
	FREE_MEMORY
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "Failed to set data");
	}
}

/**
 * Set a value and return a table as a response
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @param value to set
 * @return a table
 */
Table aidaSetValueWithResponse(JNIEnv* env, const char* uri, Arguments arguments, Value value)
{
	if (strcasecmp(uri, "MKB:VAL") == 0) {
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
	if (avscanf(env, &arguments, &value, "%f %s",
			"value", &floatValue,
			"mkb", &mkb
	)) {
		RETURN_NULL_TABLE;
	}
	CONVERT_TO_VMS_FLOAT(&floatValue, 1)

	// Set the value
	int num_devices;
	vmsstat_t status;
	status = DPSLCUTIL_DO_MKB(mkb, &floatValue, &num_devices);
	free(mkb);
	mkb = NULL;
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "unable to set value");
		RETURN_NULL_TABLE;
	}

	if ( DPSLCUTIL_MKB_GETABSFLAG() ) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION, "Specified multiknob file is absolute, which is not permitted");
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
	ON_EXCEPTION_RETURN_(table)
	tableAddFixedWidthStringColumn(env, &table, namesData, MAX_DEVICE_STRING_LEN);
	ON_EXCEPTION_RETURN_(table)
	tableAddColumn(env, &table, AIDA_FLOAT_TYPE, secondaryValuesData, false);

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
	TO_DGROUP(dGroupName, uri)
	status = DPSLCUTIL_TRIG_SETDEACTORREACT(dGroupName, flag, beam);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "unable to set value");
		RETURN_NULL_TABLE;
	}

	// Read back status
	status = DPSLCUTIL_TRIG_GETSTATUS(dGroupName, beam, &flag);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "unable to read-back value");
		RETURN_NULL_TABLE;
	}

	// Now create table to return the flag
	Table table = tableCreate(env, 1, 1);
	ON_EXCEPTION_RETURN_(table)
	tableAddSingleRowShortColumn(env, &table, flag);

	return table;
}

/**
 * Read the trig status and return the value
 *
 * @param env to report errors
 * @param uri the uri
 * @param arguments the arguments
 * @return the beam status
 */
static short getTrigStatus(JNIEnv * env, const char* uri, Arguments arguments)
{
	// Get the arguments
	int beam;

	if (ascanf(env, &arguments, "%d", "beam", &beam)) {
		return -1;
	}

	// Read the status
	vmsstat_t status;
	short trig_status;
	TO_DGROUP(dGroupName, uri)
	status = DPSLCUTIL_TRIG_GETSTATUS((char*)dGroupName, beam, &trig_status);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "Unable to get beam status");
	}
	return trig_status;
}
