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

Table setActivateValue(JNIEnv* env, const char* uri, Arguments arguments, Value value, char* pmu, char* secn);
Table setPdesValue(JNIEnv* env, const char* uri, Arguments arguments, Value value, char* pmu, char* secn);
Table setKphrValue(JNIEnv* env, const char* uri, Arguments arguments, Value value, char* pmu, char* secn);
Table setPdesOrKphrValue(JNIEnv* env, const char* uri, Value value, char* trim, char* pmu, char* secn);
void setPconOrAconValue(JNIEnv* env, const char* uri, Arguments arguments, Value value, char* pmu, char* secn);
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
	short klys_status;
	if (getKlystronStatus(env, uri, arguments, &klys_status)) {
		return 0;
	}
	return klys_status;
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
	short klys_status;
	if (getKlystronStatus(env, uri, arguments, &klys_status)) {
		return 0;
	}
	return (long)klys_status;
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
	short klys_status;
	if (getKlystronStatus(env, uri, arguments, &klys_status)) {
		return NULL;
	}
	char* string = malloc(40);
	if (klys_status) {
		strcpy(string, "activated");
	} else {
		strcpy(string, "deactivated");
	}

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
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
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
	char pmu_str[100];
	int pmuEnd = strcspn(uri, "/");
	strncpy(pmu_str, uri, pmuEnd);
	pmu_str[pmuEnd] = 0x0;

	if (endsWith(uri, "PCON")) {
		setPconOrAconValue(env, uri, arguments, value, pmu_str, "PCON");
	} else if (endsWith(uri, "ACON")) {
		setPconOrAconValue(env, uri, arguments, value, pmu_str, "ACON");
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
		Table table;
		return table;
	}
	char pmu_str[100];
	int pmuEnd = strcspn(uri, "/");
	strncpy(pmu_str, uri, pmuEnd);
	pmu_str[pmuEnd] = 0x0;

	if (endsWith(uri, "TACT")) {
		return setActivateValue(env, uri, arguments, value, pmu_str, "TACT");
	} else if (endsWith(uri, "PDES")) {
		return setPdesValue(env, uri, arguments, value, pmu_str, "PDES");
	} else if (endsWith(uri, "KPHR")) {
		return setKphrValue(env, uri, arguments, value, pmu_str, "KPHR");
	} else {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
		Table table;
		return table;
	}
}

void setPconOrAconValue(JNIEnv* env, const char* uri, Arguments arguments, Value value, char* pmu, char* secn)
{
	if (!DPSLCKLYS_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Aida access to klystron operations is not currently enabled");
		return;
	}

	if (value.type != AIDA_STRING_TYPE) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION, "Missing value to PCON or ACON value");
		return;
	}
	float floatValue;
	sscanf(value.value.stringValue, "%f", &floatValue);

	// Set the value
	vmsstat_t status = 0;
	int2u one = 1;
	float config_value_array[1];
	config_value_array[0] = floatValue;
	CVT_IEEE_TO_VMS_FLT(config_value_array, config_value_array, &one);

	status = DPSLCKLYS_SETCONFIG(pmu, config_value_array, secn);
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
	char* trim;

	if (getTrimArgument(env, arguments, &trim)) {
		Table table;
		return table;
	}

	return setPdesOrKphrValue(env, uri, value, trim, pmu, secn);
}

Table setPdesOrKphrValue(JNIEnv* env, const char* uri, Value value, char* trim, char* pmu, char* secn)
{
	Table table;
	memset(&table, 0, sizeof(table));
	table.columnCount = 8;
	float secn_value_array[1];

	if (value.type != AIDA_STRING_TYPE) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION, "Missing value to PDES or KPHR value");
		return table;
	}
	sscanf(value.value.stringValue, "%f", secn_value_array);

	// Set the value
	vmsstat_t status = 0;
	float phas_value;  /* Returned in ieee format */

	int2u one = 1;

	/* Convert the specified secondary (PDES or KPHR) value
       to VMS floating point format for DPSLCKLYS_SETTRIMPHASE.
    ----------------------------------------------------------- */
	status = CVT$CONVERT_FLOAT((void*)secn_value_array, CVT$K_IEEE_S, (void*)secn_value_array, CVT$K_VAX_F, 0);
	if (status != CVT$_NORMAL) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION, "Could not convert float");
		return table;
	}

	status = DPSLCKLYS_SETTRIMPHASE(pmu, secn, secn_value_array, trim, &phas_value);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "Could not convert float");
	}

	// return phas_value
	// Set columns
	table.columnCount = 1;
	table.rowCount = 1;

	// Allocate space for table data
	if (initTable(env, &table) == NULL) {
		return table;
	}

	// Allocate space for rows of data
	for (int column = 0; column < table.columnCount; column++) {
		switch (column) {
		case 0: // names
			tableFloatColumn(&table, column);
			break;
		default: // unsupported
			fprintf(stderr, "Unsupported table column type: %d\n", table.types[column]);
			break;
		}
	}

	// Get all data
	float* floatArray;

	// Flag
	floatArray = (float*)(table.ppData[0]);
	floatArray[0] = phas_value;

	return table;
}

Table setActivateValue(JNIEnv* env, const char* uri, Arguments arguments, Value value, char* pmu, char* secn)
{
	Table table;
	memset(&table, 0, sizeof(table));
	table.columnCount = 8;

	char* beam_c;
	char* dgrp_c;

	if (getBeamArgument(env, arguments, &beam_c) || getDGrpArgument(env, arguments, &dgrp_c)) {
		return table;
	}

	if (value.type != AIDA_STRING_TYPE) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
				"Missing value to set Activate/Deactivate status");
		return table;
	}
	short flag;
	sscanf(value.value.stringValue, "%hi", &flag);

	// Set the value
	vmsstat_t status = 0;;
	status = DPSLCKLYS_SETDEACTORREACT((char*)uri, flag, beam_c);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, NULL);
		return table;
	}

	// Get the status to return it
	short klys_status;
	status = DPSLCKLYS_GETSTATUS((char*)uri, beam_c, dgrp_c, &klys_status);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, NULL);
		return table;
	}

	// return status
	// Set columns
	table.columnCount = 1;
	table.rowCount = 1;

	// Allocate space for table data
	if (initTable(env, &table) == NULL) {
		return table;
	}

	// Allocate space for rows of data
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

	// Flag
	shortArray = (short*)(table.ppData[0]);
	shortArray[0] = klys_status;

	return table;
}

static int getKlystronStatus(JNIEnv* env, const char* uri, Arguments arguments, short* klys_status)
{
	vmsstat_t status = 0;;
	char* beam_c;
	char* dgrp_c;

	if (getBeamArgument(env, arguments, &beam_c) || getDGrpArgument(env, arguments, &dgrp_c)) {
		return 1;
	}

	status = DPSLCKLYS_GETSTATUS((char*)uri, beam_c, dgrp_c, klys_status);
	if (!SUCCESS(status)) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "failed to get klystron status");
		return 2;
	}

	return 0;
}

static int getBeamArgument(JNIEnv* env, Arguments arguments, char** beam)
{
	Argument argument = getArgument(arguments, "beam");
	if (!argument.name) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
				"'beam parameter is required");
		return 2;
	}
	*beam = argument.value;

	return 0;
}

static int getDGrpArgument(JNIEnv* env, Arguments arguments, char** dgrp)
{
	Argument argument = getArgument(arguments, "dgrp");
	if (argument.name) {
		*dgrp = argument.value;
	}

	return 0;
}

static int getTrimArgument(JNIEnv* env, Arguments arguments, char** trim)
{
	Argument argument = getArgument(arguments, "trim");
	if (argument.name) {
		*trim = argument.value;
	}

	return 0;
}

