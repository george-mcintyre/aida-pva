/**
 * Klystron Server implementation
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <descrip.h>              /*  for definition of $DESCRIPTOR  */

#include <cvtdef.h>             /* CVT$K_*, CVT$M_* */
#include <cvtmsg.h>             /* CVT$_NORMAL */
#include <cvt$routines.h>       /* CVT$CONVERT_FLOAT */

#include "aida_server_helper.h"
#include "json.h"

#include "slc_macros.h"    /* vmsstat_t, int2u, int4u, etc. */
#include "msg_proto.h"     /* for standalone_init */
#include "sysutil_proto.h" /* for cvt_vms_to_ieee_flt */

#include "slcKlys_server.h"

static int getBeamArgument(JNIEnv* env, Arguments arguments, char** beam);
static int getDGrpArgument(JNIEnv* env, Arguments arguments, char** dgrp);
static int getTrimArgument(JNIEnv* env, Arguments arguments, char** trim);
static int getKlystronStatus(JNIEnv* env, const char* uri, Arguments arguments, short* klys_status);
static int klystronStatus(JNIEnv* env, char slcName[30], char* beam_c, char* dgrp_c, short* klys_status);

Table setActivateValue(JNIEnv* env, const char* uri, Arguments arguments, Value value);
Table setPdesValue(JNIEnv* env, const char* uri, Arguments arguments, Value value, char* pmu, char* secn);
Table setKphrValue(JNIEnv* env, const char* uri, Arguments arguments, Value value, char* pmu, char* secn);
Table setPdesOrKphrValue(JNIEnv* env, const char* uri, Value value, char* trim, char* pmu, char* secn);
void setPconOrAconValue(JNIEnv* env, Value value, char* pmu, char* secn);

/**
 * Initialise the service
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @throws ServerInitialisationException if the service fails to initialise
 */
void aidaServiceInit(JNIEnv* env)
{
	vmsstat_t status;

	if (!SUCCESS(status = DPSLCKLYS_DB_INIT())) {
		aidaThrow(env, status, SERVER_INITIALISATION_EXCEPTION, "initialising Klystron Service");
		return;
	}

	printf("Aida Klystron Service Initialised\n");
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
	short klystronStatus;
	if (getKlystronStatus(env, uri, arguments, &klystronStatus)) {
		return 0;
	}
	return klystronStatus;
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
	short klystronStatus;
	if (getKlystronStatus(env, uri, arguments, &klystronStatus)) {
		return 0;
	}
	return (long)klystronStatus;
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
	short klystronStatus;
	if (getKlystronStatus(env, uri, arguments, &klystronStatus)) {
		return NULL;
	}

	char* stringKlystronStatus = malloc(40);
	if (!stringKlystronStatus) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Can't allocate space for string");
		return NULL;
	}

	if (klystronStatus) {
		strcpy(stringKlystronStatus, "activated");
	} else {
		strcpy(stringKlystronStatus, "deactivated");
	}

	return stringKlystronStatus;
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
	if (!DPSLCKLYS_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Aida access to klystron operations is not currently enabled");
		return;
	}

	PMU_STRING_FROM_URI(uri, pmu_str)

	if (endsWith(uri, "PCON")) {
		setPconOrAconValue(env, value, pmu_str, "PCON");
	} else if (endsWith(uri, "ACON")) {
		setPconOrAconValue(env, value, pmu_str, "ACON");
	} else {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
		return;
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
	if (!DPSLCKLYS_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Aida access to klystron operations is not currently enabled");
		RETURN_NULL_TABLE
	}

	PMU_STRING_FROM_URI(uri, pmu_str)

	if (endsWith(uri, "TACT")) {
		return setActivateValue(env, uri, arguments, value);
	} else if (endsWith(uri, "PDES")) {
		return setPdesValue(env, uri, arguments, value, pmu_str, "PDES");
	} else if (endsWith(uri, "KPHR")) {
		return setKphrValue(env, uri, arguments, value, pmu_str, "KPHR");
	} else {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
		RETURN_NULL_TABLE
	}
}

void setPconOrAconValue(JNIEnv* env, Value value, char* pmu, char* secn)
{
	if (value.type != AIDA_STRING_TYPE) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION, "Missing value to PCON or ACON value");
		return;
	}

	float floatValue = valueGetFloat(value);

	// Set the value
	vmsstat_t status;
	CONVERT_TO_VMS_FLOAT(&floatValue, 1)

	status = DPSLCKLYS_SETCONFIG(pmu, &floatValue, secn);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, MISSING_REQUIRED_ARGUMENT_EXCEPTION, NULL);
	}
}

Table setKphrValue(JNIEnv* env, const char* uri, Arguments arguments, Value value, char* pmu, char* secn)
{
	return setPdesOrKphrValue(env, uri, value, NULL, pmu, secn);
}

Table setPdesValue(JNIEnv* env, const char* uri, Arguments arguments, Value value, char* pmu, char* secn)
{
	char* trim = NULL;

	getTrimArgument(env, arguments, &trim);
	return setPdesOrKphrValue(env, uri, value, trim ? trim : "NO", pmu, secn);
}

Table setPdesOrKphrValue(JNIEnv* env, const char* uri, Value value, char* trim, char* pmu, char* secn)
{
	float secn_value_array[1];

	if (value.type != AIDA_STRING_TYPE) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION, "Missing value to PDES or KPHR value");
		RETURN_NULL_TABLE
	}
	secn_value_array[0] = valueGetShort(value);

	// Set the value
	float phas_value;  /* Returned in ieee format */
	CONVERT_TO_VMS_FLOAT(&secn_value_array[0], 1)
	vmsstat_t status = DPSLCKLYS_SETTRIMPHASE(pmu, secn, secn_value_array, trim, &phas_value);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "Could not convert float");
		RETURN_NULL_TABLE;
	}

	Table table = tableCreate(env, 1, 1);
	CHECK_EXCEPTION(table)
	tableAddSingleRowFloatColumn(env, &table, phas_value);

	return table;
}

Table setActivateValue(JNIEnv* env, const char* uri, Arguments arguments, Value value)
{
	// Keep track of stuff to free
	TRACK_ALLOCATED_MEMORY

	// Validate and get value
	if (value.type != AIDA_STRING_TYPE
			|| (strcmp(value.value.stringValue, "1") != 0 && strcmp(value.value.stringValue, "0") != 0)) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION, "Supplied value must be 0 or 1");
		RETURN_NULL_TABLE;
	}
	int isActivateOperation = (*value.value.stringValue == '1') ? 1 : 0;

	// Get the arguments
	char* beam_c;
	char* dgrp_c = NULL;

	if (ascanf(env, &arguments, "%s %os",
			"beam", &beam_c,
			"dgrp", &dgrp_c
	)) {
		RETURN_NULL_TABLE
	}
	PMU_STRING_FROM_URI(uri, shortSlcName)

	TRACK_MEMORY(beam_c)
	if (dgrp_c)
		TRACK_MEMORY(dgrp_c)

	// Get the status
	short klys_status;
	int callStatus = klystronStatus(env, shortSlcName, beam_c, dgrp_c, &klys_status);
	if (callStatus) {
		FREE_MEMORY
		RETURN_NULL_TABLE
	}

	short isInAccelerateState = (short)(klys_status & LINKLYSTA_ACCEL);
	short isInStandByState = (short)(klys_status & LINKLYSTA_STANDBY);

	// If trying to activate but not in standby state
	if ( isActivateOperation && !isInStandByState ) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION, "Cannot reactivate klystron when not in standby state");
		FREE_MEMORY
		RETURN_NULL_TABLE
	}

	// If trying to deactivate but not in accelerate state
	if ( !isActivateOperation && !isInAccelerateState ) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION, "Cannot deactivate klystron when not in accelerate state");
		FREE_MEMORY
		RETURN_NULL_TABLE
	}

	// Set the new value
	printf("Parameters: %s, %d, %s\n", shortSlcName, isActivateOperation, beam_c);
	int status = DPSLCKLYS_SETDEACTORREACT(shortSlcName, isActivateOperation, beam_c);
	if (!SUCCESS(status))
	{
		FREE_MEMORY
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "Could not set activation state" );
		RETURN_NULL_TABLE
	}

	// Get the status to return it
	callStatus = klystronStatus(env, shortSlcName, beam_c, dgrp_c, &klys_status);
	FREE_MEMORY
	if (callStatus) {
		RETURN_NULL_TABLE
	}

	// Create table for return value
	Table table = tableCreate(env, 1, 1);
	CHECK_EXCEPTION(table)
	tableAddSingleRowShortColumn(env, &table, klys_status);

	return table;
}

/**
 * Get Status
 *
 * @param env
 * @param uri
 * @param arguments
 * @param klys_status
 * @return
 */
static int getKlystronStatus(JNIEnv* env, const char* uri, Arguments arguments, short* klys_status)
{
	char* beam_c;
	char* dgrp_c = NULL;

	if (ascanf(env, &arguments, "%s %os",
			"beam", &beam_c,
			"dgrp", &dgrp_c
	)) {
		return EXIT_FAILURE;
	}

	TO_SLAC_NAME
	int returnStatus = klystronStatus(env, slcName, beam_c, dgrp_c, klys_status);

	free(beam_c);
	if (dgrp_c)
		free(dgrp_c);

	return returnStatus;
}

/**
 * Get Status
 *
 * @param env
 * @param uri
 * @param arguments
 * @param klys_status
 * @return
 */
static int klystronStatus(JNIEnv* env, char slcName[30], char* beam_c, char* dgrp_c, short* klys_status)
{
	vmsstat_t status = DPSLCKLYS_GETSTATUS(slcName, beam_c, dgrp_c ? dgrp_c : "LIN_KLYS", klys_status);
	if (!SUCCESS(status)) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "failed to get klystron status");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/**
 * Get Trim Argument
 *
 * @param env
 * @param arguments
 * @param trim
 * @return
 */
static int getTrimArgument(JNIEnv* env, Arguments arguments, char** trim)
{
	Argument argument = getArgument(arguments, "trim");
	if (argument.name) {
		*trim = argument.value;
	}

	return 0;
}

