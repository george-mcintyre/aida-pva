/**
 * BPM Server implementation
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stsdef.h>               /* $VMS.. status manipulation */
#include <ssdef.h>                /* SS$_NORMAL and other VMS general codes */
#include <descrip.h>              /*  for definition of $DESCRIPTOR  */

#include "aida_server_helper.h"
#include "json.h"

#include "slcBpm_server.h"

#include "slc_macros.h"           /* vmsstat_t, int2u, int4u, etc. */
#include "sysutil_proto.h"        /* for cvt_vms_to_ieee_flt */
#include "msg_proto.h"            /* for standalone_init */
#include "slc_types.h"            /* prev of dpslcbpm.h */
#include "bpmparamv.h"            /* prev of dpslcbpm.h */
#include "bpmparam.hc"            /* prev of bpmstruc.hc */
#include "bpmstruc.hc"            /* bpms_data_ts, prev of dpslcbpm.h */
#include "dpslcbpm.h"             /* Suite include file for dpslcbpm */
#include "errtranslate.h"

/**
 * Initialise the service
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @throws ServerInitialisationException if the service fails to initialise
 */
void aidaServiceInit(JNIEnv* env)
{
	vmsstat_t status;

	if (!$VMS_STATUS_SUCCESS(status = DPSLCBPM_INIT())) {
		aidaThrow(env, status, SERVER_INITIALISATION_EXCEPTION, "initialising SLC BPM Service");
		return;
	}

	printf("Aida BPM Service Initialised\n");
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
 * Get a table of BPM data
 * Start acquisition
 * Get data
 * Construct table
 * end acquisition
 * Return data
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @return the table
 */
Table aidaRequestTable(JNIEnv* env, const char* uri, Arguments arguments)
{
	char pvname[NAME_MAX];

	Table table;
	memset(&table, 0, sizeof(table));
	table.columnCount = 0;
	table.rowCount = 0;

	vmsstat_t status = 0;
	int bpmd = 0;

	int n = 1;
	int cnfnum = 1;
	int sortOrder = 2;
	int cnftype = 0;

	Argument argument;

	// BPMD
	argument = getArgument(arguments, "bpmd");
	if (!argument.name) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
				"'Acquisition requires a BPMD parameter");
		return table;
	}
	bpmd = getIntegerArgument(argument);

	// N
	argument = getArgument(arguments, "n");
	if (argument.name) {
		n = getIntegerArgument(argument);
	}

	// cnfnum
	argument = getArgument(arguments, "cnfnum");
	if (argument.name) {
		cnfnum = getIntegerArgument(argument);
	}

	// sort order
	argument = getArgument(arguments, "sortOrder");
	if (argument.name) {
		sortOrder = getIntegerArgument(argument);
	}

	// cnftype
	argument = getArgument(arguments, "cnftype");
	//         cnftype                The acquisition "configuration" type. If
	//                                0 (NONE) no bpm config is used; cnfnum ignored.
	//                                1 (GOLD) "gold" config is used; cnfnum ignored.
	//                                2 (LOADED). Last loaded used; cnfnum ignored.
	//                                3 (SCRATCH). Scratch config <cnfnum> is used.
	//                                4 (NORMAL). Normal config <cnfnum> is used.
	if (argument.name) {
		if (strcasecmp(argument.value, "NONE") == 0) {
		} else if (strcasecmp(argument.value, "GOLD") == 0) {
			cnftype = 0;
		} else if (strcasecmp(argument.value, "LOADED") == 0) {
			cnftype = 1;
		} else if (strcasecmp(argument.value, "SCRATCH") == 0) {
			cnftype = 2;
		} else if (strcasecmp(argument.value, "NORMAL") == 0) {
			cnftype = 4;
		}
	}

	// Initialise acquisition
	if (!$VMS_STATUS_SUCCESS(status = DPSLCBPM_ACQINIT())) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "while initializing BPM acquisition");
		table.rowCount = 0;
		return table;
	}

	// Acquire BPM values
	unsigned long bpmCount;
	status = DPSLCBPM_BPMACQ(&bpmCount, bpmd, n, cnftype, cnfnum);
	table.rowCount = (int)bpmCount;

	/* Check status from above call (will ThrowNew if status is bad) */
	if (!$VMS_STATUS_SUCCESS(status)) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "while making BPM acquisition");
		table.rowCount = 0;
		return table;
	}

	/* Reorder the BPM data by Z if that was required */
	// TODO check that value of edu_stanford_slac_aida_dp_dpSlcBpm_DpSlcBpmI_0005fimpl_SORTORDER_Z is 2
	if (sortOrder == 2) {
		DPSLCBPM_SORTBYZ(bpmCount);
	}

	// Set columns
	table.columnCount = 7;

	// Allocate space for table data
	table.ppData = calloc(table.columnCount, sizeof(void*));
	if (!table.ppData) {
		table.columnCount = 0;
		char errorString[BUFSIZ];
		sprintf(errorString, "Unable to allocate memory for table: %ld", table.columnCount * sizeof(void*));
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
		return table;
	}

	table.types = calloc(table.columnCount, sizeof(Type*));
	if (!table.types) {
		char errorString[BUFSIZ];
		sprintf(errorString, "Unable to allocate memory for table types: %ld", table.columnCount * sizeof(Type*));
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
		return table;
	}

	// Allocate space for rows of data
	char** stringArray;
	stringArray = table.ppData[0];
	for (int column = 0; column < table.columnCount; column++) {
		switch (column) {
		case 0: // names
			table.types[column] = AIDA_STRING_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(char*));
			// allocate data for each string too
			for (int row = 0; row < table.rowCount; row++) {
				stringArray[row] = malloc(NAME_SIZE);
			}
			break;
		case 1: // x
			table.types[column] = AIDA_FLOAT_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(float));
			break;
		case 2: // y
			table.types[column] = AIDA_FLOAT_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(float));
			break;
		case 3: // tmits
			table.types[column] = AIDA_FLOAT_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(float));
			break;
		case 4: // z
			table.types[column] = AIDA_FLOAT_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(float));
			break;
		case 5: // hstas
			table.types[column] = AIDA_LONG_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(long));
			break;
		case 6: // stats
			table.types[column] = AIDA_LONG_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(long));
			break;
		default: // unsupported
			fprintf(stderr, "Unsupported table column type: %d\n", table.types[column]);
			break;
		}
	}

	// Get all data
	char names[MAX_DGRP_BPMS][NAME_SIZE];
	unsigned long nBpmsRead = 0;
	int2u M;
	float* floatArray;
	unsigned long* longArray;

	// Names
	nBpmsRead = DPSLCBPM_GETNAMES(names);
	if (!nBpmsRead) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "reading BPM names");
		return table;
	}
	// Copy names to allocated space
	for (int row = 0; row < nBpmsRead; row++) {
		strcpy(stringArray[row], names[row]);
	}

	// x
	floatArray = (float*)(table.ppData[1]);
	status = DPSLCBPM_GETX(&nBpmsRead, floatArray);
	if (!$VMS_STATUS_SUCCESS(status)) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "reading BPM x values");
		return table;
	}
	M = (int2u)nBpmsRead;
	CVT_VMS_TO_IEEE_FLT(floatArray, floatArray, &M);

	// y
	floatArray = (float*)(table.ppData[2]);
	status = DPSLCBPM_GETY(&nBpmsRead, floatArray);
	if (!$VMS_STATUS_SUCCESS(status)) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "reading BPM y values");
		return table;
	}
	M = (int2u)nBpmsRead;
	CVT_VMS_TO_IEEE_FLT(floatArray, floatArray, &M);

	// tmit
	floatArray = (float*)(table.ppData[3]);
	nBpmsRead = DPSLCBPM_GETTMIT(floatArray);
	if (!nBpmsRead) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "reading BPM tmit values");
		return table;
	}
	M = (int2u)nBpmsRead;
	CVT_VMS_TO_IEEE_FLT(floatArray, floatArray, &M);

	// z
	floatArray = (float*)(table.ppData[4]);
	nBpmsRead = DPSLCBPM_GETZ(floatArray);
	if (!nBpmsRead) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "reading BPM z values");
		return table;
	}
	M = (int2u)nBpmsRead;
	CVT_VMS_TO_IEEE_FLT(floatArray, floatArray, &M);

	// hstas
	longArray = (unsigned long*)(table.ppData[5]);
	nBpmsRead = DPSLCBPM_GETHSTA(longArray);
	if (!nBpmsRead) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "reading BPM htas values");
		return table;
	}

	// stats
	longArray = (unsigned long*)(table.ppData[6]);
	nBpmsRead = DPSLCBPM_GETSTAT(longArray);
	if (nBpmsRead != 0) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "reading BPM stats values");
		return table;
	}

	// Terminate the acquisition
	status = DPSLCBPM_ACQTERM();
	if (!$VMS_STATUS_SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "while terminating BPM acquisition");
	}

	// All read successfully
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
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
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
	Table table;
	memset(&table, 0, sizeof(table));
	table.columnCount = 0;
	aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
	return table;
}

