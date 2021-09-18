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

#include "slcBuffAcq_server.h"

static int getBuffAcqArguments(JNIEnv* env, const char* uri, Arguments arguments,
		int* bpmd, int* nrpos, DEVICE_NAME_TS deviceNames[MAX_DGRP_BPMS], int* nDevices, char** dGroupName);
static int acquireBuffAcqData(JNIEnv* env,
		int nDevices, DEVICE_NAME_TS deviceNames[3000], char* dGroupName, int bpmd, int nrpos);
static int getBuffAcqData(JNIEnv* env,
		char **namesData,
		float* xData, float* yData, float* tmitData, unsigned long* pulseIdData,
		int2u* statsData, int2u* goodMeasData);
static int endAcquireBuffAcq(JNIEnv* env);

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
	// Get arguments
	int bpmd = 0, nrpos = 1, nDevices = 0;
	DEVICE_NAME_TS deviceNames[MAX_DGRP_BPMS];
	char* dGroupName;
	if (getBuffAcqArguments(env, uri, arguments, &bpmd, &nrpos, deviceNames, &nDevices, &dGroupName)) {
		Table table;
		return table;
	}

	// Acquire Data
	int rows;
	if ((rows = acquireBuffAcqData(env, nDevices, deviceNames, dGroupName, bpmd, nrpos)) == 0) {
		Table table;
		return table;
	}

	// To hold data
	char *namesData[rows];
	float xData[rows], yData[rows], tmitData[rows];
	unsigned long pulseIdData[rows];
	int2u statsData[rows], goodMeasData[rows];

	// Get Buffered Data
	if (!(rows = getBuffAcqData(env, namesData, xData, yData, tmitData, pulseIdData, statsData, goodMeasData))) {
		Table table;
		return table;
	}

	// Make and output table
	Table table = makeTable(env, rows, 7);
	CHECK_EXCEPTION(table)
	addStringColumn(env, &table, namesData);
	CHECK_EXCEPTION(table)
	addColumn(env, &table, AIDA_INTEGER_TYPE, pulseIdData);
	CHECK_EXCEPTION(table)
	addColumn(env, &table, AIDA_FLOAT_TYPE, xData);
	CHECK_EXCEPTION(table)
	addColumn(env, &table, AIDA_FLOAT_TYPE, yData);
	CHECK_EXCEPTION(table)
	addColumn(env, &table, AIDA_FLOAT_TYPE, tmitData);
	CHECK_EXCEPTION(table)
	addColumn(env, &table, AIDA_SHORT_TYPE, statsData);
	CHECK_EXCEPTION(table)
	addColumn(env, &table, AIDA_SHORT_TYPE, goodMeasData);

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

static int
getBuffAcqArguments(JNIEnv* env, const char* uri, Arguments arguments, int* bpmd, int* nrpos,
		DEVICE_NAME_TS deviceNames[MAX_DGRP_BPMS], int* nDevices, char** dGroupName)
{
	Argument argument;

	// BPMD
	argument = getArgument(arguments, "bpmd");
	if (!argument.name) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
				"'Acquisition requires a BPMD parameter");
		return 1;
	}
	*bpmd = getIntegerArgument(argument);

	// NRPOS
	argument = getArgument(arguments, "nrpos");
	if (argument.name) {
		*nrpos = getIntegerArgument(argument);
	}

	// Get the dgroup which is the first part of the uri
	*dGroupName = groupNameFromUri(uri);

	// BPMS or DEVS
	Value devices;
	devices = getNamedValue(env, arguments, "bpms");
	if (devices.type == AIDA_NO_TYPE) {
		devices = getNamedValue(env, arguments, "devs");
	}
	switch (devices.type) {
	case AIDA_STRING_TYPE:
		pmuFromUri(devices.value.stringValue,
				deviceNames[0].prim_s._a,
				deviceNames[0].micr_s._a,
				&deviceNames[0].unit_s._i);
		*nDevices = 1;
		break;
	case AIDA_JSON_TYPE:
		if (devices.value.jsonValue->type == json_array) {
			*nDevices = (int)devices.value.jsonValue->u.array.length;
			json_value** values = devices.value.jsonValue->u.array.values;
			for (int i = 0; i < *nDevices; i++) {
				pmuFromUri(values[i]->u.string.ptr,
						deviceNames[i].prim_s._a,
						deviceNames[i].micr_s._a,
						&deviceNames[i].unit_s._i);
			}
		}
		break;
	}

	return 0;
}

/**
 * Acquire Buffered Data and return the number of readings returned
 *
 * @param env
 * @param nDevices
 * @param deviceNames
 * @param dGroupName
 * @param bpmd
 * @param nrpos
 * @return
 */
static int acquireBuffAcqData(JNIEnv* env,
		int nDevices, DEVICE_NAME_TS deviceNames[3000], char* dGroupName, int bpmd, int nrpos)
{
	vmsstat_t status;

	// Initialise acquisition
	if (!$VMS_STATUS_SUCCESS(status = DPSLCBUFF_ACQINIT(nDevices, nrpos))) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "while initializing Buffered BPM Acquisition");
		return 0;
	}

	// Acquire BPM values
	status = DPSLCBUFF_ACQ(nDevices, deviceNames, dGroupName, bpmd, nrpos);
	if (!$VMS_STATUS_SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "while making Buffered Data acquisition");
		return 0;
	}

	return (int)DPSLCBUFF_GETTABLEMROWS();
}

/**
 * Get the Buffered Data
 *
 * @param env
 * @param namesData
 * @param xData
 * @param yData
 * @param tmitData
 * @param pulseIdData
 * @param statsData
 * @param goodMeasData
 * @return
 */
static int
getBuffAcqData(JNIEnv* env,
		char **namesData,
		float* xData, float* yData, float* tmitData,
		unsigned long* pulseIdData, int2u* statsData, int2u* goodMeasData)
{
	if (!DPSLCBUFF_GETNAMES(namesData) ||
			!DPSLCBUFF_GETPULSEIDS(pulseIdData) ||
			!DPSLCBUFF_GETXS(xData) ||
			!DPSLCBUFF_GETYS(yData) ||
			!DPSLCBUFF_GETTMITS(tmitData) ||
			!DPSLCBUFF_GETSTATS(statsData) ||
			!DPSLCBUFF_GETGOODMEASES(goodMeasData)
			) {
		endAcquireBuffAcq(env);
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "reading Buffered values");
		return 0;
	}

	return endAcquireBuffAcq(env);
}

/**
 * End Buffered data acquisition
 *
 * @param env
 * @return
 */
static int endAcquireBuffAcq(JNIEnv* env)
{
	vmsstat_t status = 0;

	// Terminate the acquisition
	if (!$VMS_STATUS_SUCCESS(status = DPSLCBUFF_ACQTERM())) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "while terminating Buffered Data acquisition");
		return 0;
	}
	return 1;
}

