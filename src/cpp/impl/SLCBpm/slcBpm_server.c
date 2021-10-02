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

#include "slc_macros.h"           /* vmsstat_t, int2u, int4u, etc. */
#include "sysutil_proto.h"        /* for cvt_vms_to_ieee_flt */
#include "msg_proto.h"            /* for standalone_init */
#include "slc_types.h"            /* prev of dpslcbpm.h */
#include "bpmparamv.h"            /* prev of dpslcbpm.h */
#include "bpmparam.hc"            /* prev of bpmstruc.hc */
#include "bpmstruc.hc"            /* bpms_data_ts, prev of dpslcbpm.h */
#include "dpslcbpm.h"             /* Suite include file for dpslcbpm */

#include "slcBpm_server.h"

static int acquireBpmData(JNIEnv* env,
		int bpmd, int n, int cnftype, int cnfnum, int sortOrder);
static int getBpmData(JNIEnv* env,
		char namesData[MAX_DGRP_BPMS][NAME_SIZE], float* xData, float* yData, float* tmitData, float* zData,
		unsigned long* hstasData, unsigned long* statsData);
static int endAcquireBpmData(JNIEnv* env);

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
//	DO_GENERAL_INIT("AIDA-PVA_DPSLCBPM", AIDAPVA_SLCBPMID, "BPM",
//			true,       // msg init
//			true        // slac net init
//	)
	DO_STANDALONE_INIT("AIDA-PVA_DPSLCBPM", "BPM",
			true,        // msg init
			true,        // slac net init
			true,        // db init
			false,       // query init
			false)       // set init
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
	// Get arguments
	int bpmd, navg = 1, cnfnum = 1, sortOrder = SORTORDER_DISPLAY, cnftype = 0;
	char* cfnTypeString = NULL;

	if (ascanf(env, &arguments, "%d %od %od %od %os",
			"bpmd", &bpmd,
			"n", &navg,
			"cnfnum", &cnfnum,
			"sortOrder", &sortOrder,
			"cnftype", &cfnTypeString
	)) {
		RETURN_NULL_TABLE
	}

	// If cfnType was set then set cnftype variable appropriately
	if (cfnTypeString) {
		if (strcasecmp(cfnTypeString, "NONE") == 0) {
			cnftype = 0;
		} else if (strcasecmp(cfnTypeString, "GOLD") == 0) {
			cnftype = 1;
		} else if (strcasecmp(cfnTypeString, "LOADED") == 0) {
			cnftype = 2;
		} else if (strcasecmp(cfnTypeString, "SCRATCH") == 0) {
			cnftype = 3;
		} else if (strcasecmp(cfnTypeString, "NORMAL") == 0) {
			cnftype = 4;
		} else if (strcasecmp(cfnTypeString, "TEMPORARY") == 0) {
			cnftype = 5;
		}
		free(cfnTypeString);
	}

	// Acquire BPM Data
	int rows;
	if ((rows = acquireBpmData(env, bpmd, navg, cnftype, cnfnum, sortOrder)) == 0) {
		RETURN_NULL_TABLE
	}

	// To hold data
	char namesData[MAX_DGRP_BPMS][NAME_SIZE];
	float xData[rows], yData[rows], tmitData[rows], zData[rows];
	unsigned long hstasData[rows], statsData[rows];

	// Get BPM data
	if (getBpmData(env, namesData, xData, yData, tmitData, zData, hstasData, statsData)) {
		RETURN_NULL_TABLE
	}

	// Make and output table
	Table table = tableCreate(env, rows, 7);
	CHECK_EXCEPTION(table)
	tableAddFixedWidthStringColumn(env, &table, namesData, NAME_SIZE);
	CHECK_EXCEPTION(table)
	tableAddColumn(env, &table, AIDA_FLOAT_TYPE, xData);
	CHECK_EXCEPTION(table)
	tableAddColumn(env, &table, AIDA_FLOAT_TYPE, yData);
	CHECK_EXCEPTION(table)
	tableAddColumn(env, &table, AIDA_FLOAT_TYPE, tmitData);
	CHECK_EXCEPTION(table)
	tableAddColumn(env, &table, AIDA_FLOAT_TYPE, zData);
	CHECK_EXCEPTION(table)
	tableAddColumn(env, &table, AIDA_INTEGER_TYPE, hstasData);
	CHECK_EXCEPTION(table)
	tableAddColumn(env, &table, AIDA_INTEGER_TYPE, statsData);

	// All read successfully
	return table;
}

/**
 * Acquire BPM Data and return the number of readings returned
 *
 * @param env
 * @param bpmd
 * @param n
 * @param cnftype
 * @param cnfnum
 * @param sortOrder
 * @return
 */
static int acquireBpmData(JNIEnv* env, int bpmd, int n, int cnftype, int cnfnum, int sortOrder)
{
	vmsstat_t status = 0;
	int4u bpmCount = 0;

	// Initialise acquisition
	if (!$VMS_STATUS_SUCCESS(status = DPSLCBPM_ACQINIT())) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "while initializing BPM acquisition");
		return 0;
	}

	// Acquire BPM values
	// This often fails first time so if it fails just retry
	status = DPSLCBPM_BPMACQ(&bpmCount, bpmd, n, cnftype, cnfnum);
	if (!$VMS_STATUS_SUCCESS(status)) {
		endAcquireBpmData(env);
		// Retry Once
		// Reinitialise acquisition
		if (!$VMS_STATUS_SUCCESS(status = DPSLCBPM_ACQINIT())) {
			aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "while initializing BPM acquisition");
			return 0;
		}

		// Retry acquire BPM values
		status = DPSLCBPM_BPMACQ(&bpmCount, bpmd, n, cnftype, cnfnum);
		if (!$VMS_STATUS_SUCCESS(status)) {
			endAcquireBpmData(env);
			aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "while making BPM acquisition");
			return 0;
		}
	}

	/* Reorder the BPM data by Z if that was required */
	if (sortOrder == SORTORDER_Z) {
		DPSLCBPM_SORTBYZ(bpmCount);
	}

	return (int)bpmCount;
}

/**
 * Get BPM Data and return true if read correctly
 *
 * @param env
 * @param namesData
 * @param xData
 * @param yData
 * @param tmitData
 * @param zData
 * @param hstasData
 * @param statsData
 * @return
 */
static int getBpmData(JNIEnv* env,
		char namesData[MAX_DGRP_BPMS][NAME_SIZE], float* xData, float* yData, float* tmitData, float* zData,
		int4u* hstasData, int4u* statsData)
{
	vmsstat_t status;
	int4u rows;

	// data that returns a status
	status = DPSLCBPM_GETX(&rows, xData);
	CHECK_VMS_STATUS
	status = DPSLCBPM_GETY(&rows, yData);
	CHECK_VMS_STATUS

	int2u n = rows;

	// data that return rows read
	if (!DPSLCBPM_GETNAMES(namesData) ||
			!DPSLCBPM_GETTMIT(tmitData) ||
			!DPSLCBPM_GETZ(zData) ||
			!DPSLCBPM_GETHSTA(hstasData) ||
			!DPSLCBPM_GETSTAT(statsData)
			) {
		endAcquireBpmData(env);
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "reading BPM values");
		return EXIT_FAILURE;
	}

	// Convert all floating point data
	CONVERT_FROM_VMS_FLOAT(xData, n)
	CONVERT_FROM_VMS_FLOAT(yData, n)
	CONVERT_FROM_VMS_FLOAT(tmitData, n)
	CONVERT_FROM_VMS_FLOAT(zData, n)

	return endAcquireBpmData(env);
}

/**
 * End BPM Data acquisition
 *
 * @param env
 * @return
 */
static int endAcquireBpmData(JNIEnv* env)
{
	vmsstat_t status = 0;

	// Terminate the acquisition
	if (!$VMS_STATUS_SUCCESS(status = DPSLCBPM_ACQTERM())) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "while terminating BPM acquisition");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

