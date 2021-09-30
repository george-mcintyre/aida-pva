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

static int acquireBuffAcqData(JNIEnv* env,
		int nDevices, DEVICE_NAME_TS deviceNames[3000], char* dGroupName, int bpmd, int nrpos);
static int getBuffAcqData(JNIEnv* env,
		char** namesData,
		float* xData, float* yData, float* tmitData, unsigned long* pulseIdData,
		int2u* statsData, int2u* goodMeasData);
static int endAcquireBuffAcq(JNIEnv* env);

// API Stubs
REQUEST_STUB_CHANNEL_CONFIG
REQUEST_STUB_BOOLEAN
REQUEST_STUB_BYTE
REQUEST_STUB_SHORT
REQUEST_STUB_INTEGER
REQUEST_STUB_LONG
REQUEST_STUB_FLOAT
REQUEST_STUB_DOUBLE
REQUEST_STUB_STRING
REQUEST_STUB_BOOLEAN_ARRAY
REQUEST_STUB_BYTE_ARRAY
REQUEST_STUB_SHORT_ARRAY
REQUEST_STUB_INTEGER_ARRAY
REQUEST_STUB_LONG_ARRAY
REQUEST_STUB_FLOAT_ARRAY
REQUEST_STUB_DOUBLE_ARRAY
REQUEST_STUB_STRING_ARRAY
SET_STUB_VOID
SET_STUB_TABLE

/**
 * Initialise the service
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @throws ServerInitialisationException if the service fails to initialise
 */
void aidaServiceInit(JNIEnv* env)
{
	DO_STANDALONE_INIT("AIDA-PVA_SLCBUFFACQ", "Buffered BPM Acquisition",
			true,        // msg init
			true,        // slac net init
			true,        // db init
			false,       // query init
			false)       // set init
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
	unsigned int nBpms = 0, nDevs = 0;
	char** bpms, ** devices;
	DEVICE_NAME_TS deviceNames[MAX_DGRP_BPMS];
	TO_DGROUP(uri, dGroupName)

	if (ascanf(env, &arguments, "%d %od %osa %osa",
			"bpmd", &bpmd,
			"nrpos", &nrpos,
			"bpms", &bpms, &nBpms,
			"devs", &devices, &nDevs
	)) {
		RETURN_NULL_TABLE
	}

	if (nBpms && nDevs) {
		// Only one or the other not both
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Specify either DEVS or BPMS argument but not both");
		free(bpms);
		free(devices);
		RETURN_NULL_TABLE
	} else if (nBpms) {
		for (int i = 0; i < nBpms; i++) {
			pmuFromDeviceName(bpms[i],
					deviceNames[i].prim_s._a,
					deviceNames[i].micr_s._a,
					&deviceNames[i].unit_s._i);
		}
		nDevices += nBpms;
		free(bpms);
	} else if (nDevs) {
		for (int i = 0; i < nDevs; i++) {
			pmuFromDeviceName(devices[i],
					deviceNames[i].prim_s._a,
					deviceNames[i].micr_s._a,
					&deviceNames[i].unit_s._i);
		}
		nDevices += nDevs;
		free(devices);
	}

	// Acquire Data
	int rows;
	if ((rows = acquireBuffAcqData(env, nDevices, deviceNames, dGroupName, bpmd, nrpos)) == 0) {
		RETURN_NULL_TABLE
	}

	// To hold data
	char* namesData[rows];
	float xData[rows], yData[rows], tmitData[rows];
	unsigned long pulseIdData[rows];
	int2u statsData[rows], goodMeasData[rows];

	// Get Buffered Data
	if (getBuffAcqData(env, namesData, xData, yData, tmitData, pulseIdData, statsData, goodMeasData)) {
		RETURN_NULL_TABLE
	}

	// Convert Float Data
	CONVERT_FROM_VMS_FLOAT(xData, rows)
	CONVERT_FROM_VMS_FLOAT(yData, rows)
	CONVERT_FROM_VMS_FLOAT(tmitData, rows)

	// Make and output table
	Table table = tableCreate(env, rows, 7);
	CHECK_EXCEPTION(table)
	tableAddStringColumn(env, &table, namesData);
	CHECK_EXCEPTION(table)
	tableAddColumn(env, &table, AIDA_INTEGER_TYPE, pulseIdData);
	CHECK_EXCEPTION(table)
	tableAddColumn(env, &table, AIDA_FLOAT_TYPE, xData);
	CHECK_EXCEPTION(table)
	tableAddColumn(env, &table, AIDA_FLOAT_TYPE, yData);
	CHECK_EXCEPTION(table)
	tableAddColumn(env, &table, AIDA_FLOAT_TYPE, tmitData);
	CHECK_EXCEPTION(table)
	tableAddColumn(env, &table, AIDA_SHORT_TYPE, statsData);
	CHECK_EXCEPTION(table)
	tableAddColumn(env, &table, AIDA_SHORT_TYPE, goodMeasData);

	// All read successfully
	return table;
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
		endAcquireBuffAcq(env);
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
		char** namesData,
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
		return EXIT_FAILURE;
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
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

