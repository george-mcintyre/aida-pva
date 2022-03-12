/** @file
 *  @brief Buffered BPM Acquisition Native Provider implementation.
 *     **MEMBER**=SLCLIBS:AIDA_PVALIB
 *     **ATTRIBUTES**=JNI,LIBR_NOGLOBAL
 */
#include "aida_pva.h"
#include "AIDASLCBPMBUFF_SERVER.h"

static int
acquireBuffAcqData(JNIEnv* env, int* rows, int nDevices, DEVICE_NAME_TS* deviceNames, char* dGroupName,
		int bpmd,
		int nrpos);
static int getBuffAcqData(JNIEnv* env,
		char** namesData,
		float* xData, float* yData, float* tmitData, unsigned long* pulseIdData,
		int2u* statsData, int2u* goodMeasData);
static int endAcquireBuffAcq(JNIEnv* env);
static int checkArguments(JNIEnv* env, int bpmd, int nrpos, int nDevices);

// API Stubs
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
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @throws ServerInitialisationException if the service fails to initialise
 */
void aidaServiceInit(JNIEnv* env)
{
	vmsstat_t status;

	if (!$VMS_STATUS_SUCCESS(status = init("AIDA_SLCBPMBUFF", true))) {
		aidaThrow(env, status, SERVER_INITIALISATION_EXCEPTION, "while initializing Buffered BPM service");
		return;
	}

	DPSLCBUFF_SETMODE();

	printf("Aida Buffered BPM Acquisition Service Initialised\n");
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
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the table
 */
Table aidaRequestTable(JNIEnv* env, const char* uri, Arguments arguments)
{
	TRACK_ALLOCATED_MEMORY

	// Get arguments
	int bpmd = BPMD_ROGUE, nrpos = NRPOS_DEFAULT, nDevices = 0;
	unsigned int nBpms = 0, nDevs = 0;
	char** bpms = NULL, ** devices = NULL;
	DEVICE_NAME_TS deviceNames[MAX_DGRP_BPMS];
	TO_DGROUP(dGroupName, uri)

	if (ascanf(env, &arguments, "%d %od %osa %osa",
			"bpmd", &bpmd,
			"nrpos", &nrpos,
			"bpms", &bpms, &nBpms,
			"devs", &devices, &nDevs
	)) {
		RETURN_NULL_TABLE
	}
	TRACK_MEMORY(bpms)
	TRACK_MEMORY(devices)

	if (nBpms && nDevs) {
		// Only one or the other not both
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Specify either DEVS or BPMS argument but not both");
		FREE_MEMORY
		RETURN_NULL_TABLE
	} else if (nBpms) {
		for (int i = 0; i < nBpms; i++) {
			if (pmuFromDeviceName(env, bpms[i],
					deviceNames[i].prim_s._a,
					deviceNames[i].micr_s._a,
					&deviceNames[i].unit_s._i)) {
				FREE_MEMORY
				RETURN_NULL_TABLE
			}
		}
		nDevices += nBpms;
		bpms = NULL;
	} else if (nDevs) {
		for (int i = 0; i < nDevs; i++) {
			if (pmuFromDeviceName(env, devices[i],
					deviceNames[i].prim_s._a,
					deviceNames[i].micr_s._a,
					&deviceNames[i].unit_s._i)) {
				FREE_MEMORY
				RETURN_NULL_TABLE
			}
		}
		nDevices += nDevs;
		devices = NULL;
	}
	FREE_MEMORY

	// Check arguments
	if (checkArguments(env, bpmd, nrpos, nDevices)) {
		RETURN_NULL_TABLE
	}

	// Acquire Data
	int rows;
	if (acquireBuffAcqData(env, &rows, nDevices, &deviceNames[0], dGroupName, bpmd, nrpos)) {
		RETURN_NULL_TABLE
	}

	// No rows
	if (!rows) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "No rows were retrieved for your query");
		RETURN_NULL_TABLE
	}

	if (rows > MAX_DGRP_BPMS * MAX_BUFF_MEAS) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Too many rows returned by this query");
		RETURN_NULL_TABLE
	}

	// To hold data
	char* namesData[rows + 1];
	float xData[rows + 1], yData[rows + 1], tmitData[rows + 1];
	unsigned long pulseIdData[rows + 1];
	int2u statsData[rows + 1], goodMeasData[rows + 1];

	// Get Buffered Data
	if (getBuffAcqData(env, namesData, xData, yData, tmitData, pulseIdData, statsData, goodMeasData)) {
		RETURN_NULL_TABLE
	}

	// Make and output table
	Table table = tableCreate(env, rows, 7);
	ON_EXCEPTION_RETURN_(table)
	tableAddStringColumn(env, &table, namesData);
	ON_EXCEPTION_RETURN_(table)
	tableAddColumn(env, &table, AIDA_INTEGER_TYPE, pulseIdData, false);
	ON_EXCEPTION_RETURN_(table)
	tableAddColumn(env, &table, AIDA_FLOAT_TYPE, xData, false);
	ON_EXCEPTION_RETURN_(table)
	tableAddColumn(env, &table, AIDA_FLOAT_TYPE, yData, false);
	ON_EXCEPTION_RETURN_(table)
	tableAddColumn(env, &table, AIDA_FLOAT_TYPE, tmitData, false);
	ON_EXCEPTION_RETURN_(table)
	tableAddColumn(env, &table, AIDA_SHORT_TYPE, statsData, false);
	ON_EXCEPTION_RETURN_(table)
	tableAddColumn(env, &table, AIDA_SHORT_TYPE, goodMeasData, false);

	endAcquireBuffAcq(env);

	// All read successfully
	return table;
}

/**
 * Check arguments
 * @param env
 * @param bpmd
 * @param nrpos
 * @param nDevices
 * @return
 */
static int checkArguments(JNIEnv* env, int bpmd, int nrpos, int nDevices)
{
	// Check that required param, bpmd, is valid.
	if (bpmd < BPMD_MIN || bpmd > BPMD_MAX) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"BPMD param is required and must be be between 1..9999");
		return EXIT_FAILURE;
	}

	// Check n is valid
	if (nrpos < NRPOS_MIN || nrpos > NRPOS_MAX) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"NRPOS, the number of pulse to acquire, must be between 1..2800");
		return EXIT_FAILURE;
	}

	// Check devices given
	if (nDevices <= 0) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "No devices to acquire were received");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
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
static int
acquireBuffAcqData(JNIEnv* env, int* rows, int nDevices, DEVICE_NAME_TS* deviceNames, char* dGroupName,
		int bpmd,
		int nrpos)
{
	vmsstat_t status;

	// Initialise acquisition
	if (!$VMS_STATUS_SUCCESS(status = DPSLCBUFF_ACQINIT(nDevices, nrpos))) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "while initializing Buffered BPM Acquisition");
		return EXIT_FAILURE;
	}

	// Acquire BPM values
	status = DPSLCBUFF_ACQ(nDevices, deviceNames, dGroupName, bpmd, nrpos);
	if (!$VMS_STATUS_SUCCESS(status)) {
		endAcquireBuffAcq(env);
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "while making Buffered Data acquisition");
		return EXIT_FAILURE;
	}

	*rows = (int)DPSLCBUFF_GETTABLEMROWS();
	return EXIT_SUCCESS;
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
	int4u nNames, nPulseId, nXdata, nYdata, ntmit, nstats, ngoodmeas;

	if (!(nNames = DPSLCBUFF_GETNAMES(namesData)) ||
			!(nPulseId = DPSLCBUFF_GETPULSEIDS(pulseIdData)) ||
			!(nXdata = DPSLCBUFF_GETXS(xData)) ||
			!(nYdata = DPSLCBUFF_GETYS(yData)) ||
			!(ntmit = DPSLCBUFF_GETTMITS(tmitData)) ||
			!(nstats = DPSLCBUFF_GETSTATS(statsData)) ||
			!(ngoodmeas = DPSLCBUFF_GETGOODMEASES(goodMeasData))
			) {
		endAcquireBuffAcq(env);
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "reading Buffered values");
		return EXIT_FAILURE;
	}

	// If rows are not homogenous
	if (!(nNames == nPulseId && nPulseId == nXdata && nXdata == nYdata && nYdata == ntmit && ntmit == nstats
			&& nstats == ngoodmeas)) {
		endAcquireBuffAcq(env);
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "non-homologous vectors of data returned");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
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

