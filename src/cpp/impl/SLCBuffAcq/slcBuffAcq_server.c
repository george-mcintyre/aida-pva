/**
 * Buffered BPM Acquisition Server implementation
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stsdef.h>               /* $VMS.. status manipulation */
#include <ssdef.h>                /* SS$_NORMAL and other VMS general codes */
#include <descrip.h>              /*  for definition of $DESCRIPTOR  */

#include "aida_server_helper.h"
#include "json.h"

#include "slcBuffAcq_server.h"

#include "opdef.hc"               /* OP_BADDATA */
#include "slc_macros.h"           /* vmsstat_t, int2u, int4u, etc. */
#include "sysutil_proto.h"        /* for cvt_vms_to_ieee_flt */
#include "msg_proto.h"            /* for standalone_init */
#include "slc_types.h"            /* prev of dpslcbuffacq.h */
#include "bpmparamv.h"            /* prev of dpslcbuffacq.h */
#include "bpmparam.hc"            /* prev of bpmstruc.hc */
#include "bpmstruc.hc"            /* bpms_data_ts, prev of dpslcbuffacq.h */
#include "bpm_buff_common.h"      /* MAX_BUFF_MEAS */
#include "dpslcbuff_jni.h"        /* javah generated JNI protos */
#include "dpslcbpm.h"             /* BPM service include file, NAME_SIZE */
#include "dpslcbuff.h"            /* Suite include file for dpslcbuff */

/**
 * Initialise the service
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @throws ServerInitialisationException if the service fails to initialise
 */
void aidaServiceInit(JNIEnv* env)
{
	vmsstat_t status;

	if (!$VMS_STATUS_SUCCESS(status = DPSLCBUFF_INIT())) {
		aidaThrow(env, status, SERVER_INITIALISATION_EXCEPTION, "while initializing Buffered BPM service");
		return;
	}

	printf("Aida Buffered BPM Acquisition Service Initialised\n");
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
 * Parameters:
 *   BPMD:    Integer Bpm Measurement Definition number
 *   NRPOS:   number of pulses. Default is 1
 *   BPMS:   json.  List of BPMs to read
 *                ["BPMS:LI02:501", "BPMS:DR12:334"]
 *   DEVS:   json.  List of devices to read
 *   Note: cannot specify both BPMS and DEVS
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
	int bpmd = 0;

	int nrpos = 1;
	Value devices; // The devices to read
	DEVICE_NAME_TS deviceNames[MAX_DGRP_BPMS];
	int nDevices = 0;

	Argument argument;

	// BPMD
	argument = getArgument(arguments, "bpmd");
	if (!argument.name) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
				"'Acquisition requires a BPMD parameter");
		return table;
	}
	bpmd = getIntegerArgument(argument);

	// NRPOS
	argument = getArgument(arguments, "nrpos");
	if (argument.name) {
		nrpos = getIntegerArgument(argument);
	}

	char name[100];

	// Get the dgroup which is the first part of the uri
	strcpy(name, uri);
	char* dGroupName = strtok(name, "/");
	if (!dGroupName) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"Invalid URI while making Buffered Data acquisition");
		return table;
	}

	// BPMS or DEVS
	devices = getNamedValue(env, arguments, "bpms");
	if (devices.type == AIDA_NO_TYPE) {
		devices = getNamedValue(env, arguments, "devs");
	}
	switch (devices.type) {
	case AIDA_STRING_TYPE:
		strcpy(name, devices.value.stringValue);
		strcpy(deviceNames[0].prim_s._a, strtok(name, ":"));
		strcpy(deviceNames[0].micr_s._a, strtok(NULL, ":"));
		deviceNames[0].unit_s._i = atoi(strtok(NULL, ":"));
		nDevices = 1;
		break;
	case AIDA_JSON_TYPE:
		if (devices.value.jsonValue->type == json_array) {
			nDevices = devices.value.jsonValue->u.array.length;
			json_value** values = devices.value.jsonValue->u.array.values;
			for (int i = 0; i < nDevices; i++) {
				strcpy(name, values[i]->u.string.ptr);
				strcpy(deviceNames[i].prim_s._a, strtok(name, ":"));
				strcpy(deviceNames[i].micr_s._a, strtok(NULL, ":"));
				deviceNames[i].unit_s._i = atoi(strtok(NULL, ":"));
			}
		}
		break;
	}

	// Initialise acquisition
	if (!$VMS_STATUS_SUCCESS(status = DPSLCBUFF_ACQINIT(nDevices, nrpos))) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "while initializing Buffered BPM Acquisition");
		table.rowCount = 0;
		return table;
	}

	// Acquire BPM values
	status = DPSLCBUFF_ACQ(nDevices, deviceNames, dGroupName, bpmd, nrpos);

	if (!$VMS_STATUS_SUCCESS(status)) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "while making Buffered Data acquisition");
		table.rowCount = 0;
		return table;
	}

	table.rowCount = DPSLCBUFF_GETTABLEMROWS();


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
		case 1: // pulse id
			table.types[column] = AIDA_LONG_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(unsigned long));
			break;
		case 2: // x offsets
			table.types[column] = AIDA_FLOAT_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(float));
			break;
		case 3: // y offsets
			table.types[column] = AIDA_FLOAT_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(float));
			break;
		case 4: // tmits
			table.types[column] = AIDA_FLOAT_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(float));
			break;
		case 5: // stats
			table.types[column] = AIDA_LONG_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(unsigned long));
			break;
		case 6: // goodmeas
			table.types[column] = AIDA_BOOLEAN_ARRAY_TYPE;
			table.ppData[column] = calloc(table.rowCount, sizeof(unsigned char));
			break;
		default: // unsupported
			fprintf(stderr, "Unsupported table column type: %d\n", table.types[column]);
			break;
		}
	}

	// Get all data
	unsigned long nBpmsRead = 0;
	int2u M;
	float* floatArray;
	int* intArray;
	unsigned char* booleArray;
	unsigned long* longArray;

	// Names
	nBpmsRead = DPSLCBUFF_GETNAMES(stringArray);
	if (!nBpmsRead) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "reading BPM device names");
		return table;
	}

	// pulse ids
	longArray = (unsigned long*)(table.ppData[1]);
	nBpmsRead = DPSLCBUFF_GETPULSEIDS(longArray);
	if (!nBpmsRead) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "reading BPM pulse IDs");
		return table;
	}

	// x
	floatArray = (float*)(table.ppData[2]);
	nBpmsRead = DPSLCBUFF_GETXS(floatArray);
	if (!nBpmsRead) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "reading BPM x values");
		return table;
	}
	M = (int2u)nBpmsRead;
	CVT_VMS_TO_IEEE_FLT(floatArray, floatArray, &M);

	// y
	floatArray = (float*)(table.ppData[3]);
	nBpmsRead = DPSLCBUFF_GETYS(floatArray);
	if (!nBpmsRead) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "reading BPM y values");
		return table;
	}
	M = (int2u)nBpmsRead;
	CVT_VMS_TO_IEEE_FLT(floatArray, floatArray, &M);

	// tmit
	floatArray = (float*)(table.ppData[4]);
	nBpmsRead = DPSLCBUFF_GETTMITS(floatArray);
	if (!nBpmsRead) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "reading BPM tmit values");
		return table;
	}
	M = (int2u)nBpmsRead;
	CVT_VMS_TO_IEEE_FLT(floatArray, floatArray, &M);

	int2u* buffer = malloc(table.rowCount * sizeof(int2u));

	// stats
	intArray = (int*)(table.ppData[5]);
	nBpmsRead = DPSLCBUFF_GETSTATS(buffer);
	if (!nBpmsRead) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "reading BPM stats values");
		return table;
	}
	for (int i = 0; i < nBpmsRead; i++) {
		intArray[i] = (int)buffer[i];
	}

	// goodmeas
	booleArray = (unsigned char*)(table.ppData[6]);
	nBpmsRead = DPSLCBUFF_GETGOODMEASES(buffer);
	if (nBpmsRead != 0) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "reading BPM goodmeas values");
		return table;
	}
	for (int i = 0; i < nBpmsRead; i++) {
		booleArray[i] = (unsigned char)buffer[i];
	}

	free(buffer);

	// Terminate the acquisition
	status = DPSLCBUFF_ACQTERM();
	if (!$VMS_STATUS_SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "while terminating Buffered Data acquisition");
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

