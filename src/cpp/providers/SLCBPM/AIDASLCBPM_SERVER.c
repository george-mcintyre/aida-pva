/** @file
 *  @brief BPM Native Provider implementation.
 *     **MEMBER**=SLCLIBS:AIDASLCBPMLIB
 *     **ATTRIBUTES**=JNI
 */
#include "aida_pva.h"
#include "AIDASLCBPM_SERVER.h"

static int checkArguments(JNIEnv* env, int bpmd, int navg, int cnfnum, int sortOrder, int cnftype);
static int acquireBpmData(JNIEnv* env, int* rows, int bpmd, int n, int cnftype, int cnfnum, int sortOrder);
static int getBpmData(JNIEnv* env,
		char namesData[MAX_DGRP_BPMS][NAME_SIZE], float* xData, float* yData, float* tmitData, float* zData,
		unsigned long* hstasData, unsigned long* statsData);
static int endAcquireBpmData(JNIEnv* env);

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

	if (!$VMS_STATUS_SUCCESS(status = init("AIDA_SLCBPM", true))) {
		aidaThrow(env, status, SERVER_INITIALISATION_EXCEPTION, "initialising SLC BPM Service");
		return;
	}

	printf("Aida BPM Service Initialised\n");
}

/**
 * Get a table of BPM data
 * Start acquisition
 * Get data
 * Construct table
 * end acquisition
 * Return data
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the table
 */
Table aidaRequestTable(JNIEnv* env, const char* uri, Arguments arguments)
{
	// Get arguments
	int bpmd, navg = NAVG, cnfnum = BPMD_ROGUE, sortOrder = SORTORDER_DISPLAY, cnftype = CNFTYPE_NONE;
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
			cnftype = CNFTYPE_NONE;
		} else if (strcasecmp(cfnTypeString, "GOLD") == 0) {
			cnftype = CNFTYPE_GOLD;
		} else if (strcasecmp(cfnTypeString, "LOADED") == 0) {
			cnftype = CNFTYPE_LOADED;
		} else if (strcasecmp(cfnTypeString, "SCRATCH") == 0) {
			cnftype = CNFTYPE_SCRATCH;
		} else if (strcasecmp(cfnTypeString, "NORMAL") == 0) {
			cnftype = CNFTYPE_NORMAL;
		} else if (strcasecmp(cfnTypeString, "TEMPORARY") == 0) {
			cnftype = CNFTYPE_TEMPORARY;
		} else {
			free(cfnTypeString);
			cfnTypeString = NULL;
			aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "CNFTYPE argument not recognised");
			RETURN_NULL_TABLE
		}

		// Check that if a cnftype that requires a config number to be given
		// that one was given.
		if ((strcasecmp(cfnTypeString, "SCRATCH") == 0 ||
				strcasecmp(cfnTypeString, "NORMAL") == 0 ||
				strcasecmp(cfnTypeString, "TEMPORARY") == 0) && cnfnum <= BPMD_ROGUE) {
			free(cfnTypeString);
			cfnTypeString = NULL;
			aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
					"A CNFNUM argument (>0) is required with the CFNTYPE argument given");
			RETURN_NULL_TABLE
		}

	}

	// Check parameters
	if (checkArguments(env, bpmd, navg, cnfnum, sortOrder, cnftype)) {
		RETURN_NULL_TABLE
	}

	// Acquire BPM Data
	int rows;
	if (acquireBpmData(env, &rows, bpmd, navg, cnftype, cnfnum, sortOrder)) {
		RETURN_NULL_TABLE
	}

	// No rows
	if (!rows) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "No rows were retrieved for your query");
		RETURN_NULL_TABLE
	}

	if (rows > MAX_DGRP_BPMS) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Too many rows returned by this query");
		RETURN_NULL_TABLE
	}

	// To hold data
	char namesData[rows + 1][NAME_SIZE];
	float xData[rows + 1], yData[rows + 1], tmitData[rows + 1], zData[rows + 1];
	unsigned long hstasData[rows + 1], statsData[rows + 1];

	// Get BPM data
	if (getBpmData(env, namesData, xData, yData, tmitData, zData, hstasData, statsData)) {
		RETURN_NULL_TABLE
	}

	// Make and output table
	Table table = tableCreate(env, rows, 7);
	ON_EXCEPTION_RETURN_(table)
	tableAddFixedWidthStringColumn(env, &table, (char*)namesData, NAME_SIZE);
	ON_EXCEPTION_RETURN_(table)
	tableAddColumn(env, &table, AIDA_FLOAT_TYPE, xData, false);
	ON_EXCEPTION_RETURN_(table)
	tableAddColumn(env, &table, AIDA_FLOAT_TYPE, yData, false);
	ON_EXCEPTION_RETURN_(table)
	tableAddColumn(env, &table, AIDA_FLOAT_TYPE, tmitData, false);
	ON_EXCEPTION_RETURN_(table)
	tableAddColumn(env, &table, AIDA_FLOAT_TYPE, zData, false);
	ON_EXCEPTION_RETURN_(table)
	tableAddColumn(env, &table, AIDA_INTEGER_TYPE, hstasData, false);
	ON_EXCEPTION_RETURN_(table)
	tableAddColumn(env, &table, AIDA_INTEGER_TYPE, statsData, false);

	endAcquireBpmData(env);

	// All read successfully
	return table;
}

/**
 * Check the arguments throwing an exception if there are any problems
 * @param env
 * @param bpmd
 * @param navg
 * @param cnfnum
 * @param sortOrder
 * @param cnftype
 * @return
 */
static int checkArguments(JNIEnv* env, int bpmd, int navg, int cnfnum, int sortOrder, int cnftype)
{
	if (bpmd <= 0) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"(+ve) BPMD param is required for acq of BPM orbits");
		return EXIT_FAILURE;
	}

	// Check n is rational
	if (navg <= 0 || navg > 10000) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "N argument (N avg / N turns) is out of range");
		return EXIT_FAILURE;
	}

	// Check config number. 0 was the init rogue value and is therefore
	// a permitted value now (meaning an absolute orbit is wanted). If
	// a value was given it must be > 0.
	if (cnfnum < BPMD_ROGUE) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "CNFNUM argument out of range");
		return EXIT_FAILURE;
	}

	// Check sort order
	if (sortOrder != SORTORDER_Z && sortOrder != SORTORDER_DISPLAY) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "SORTORDER argument is not a recognised value");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/**
 * Acquire BPM Data and return the number of readings returned in rows
 *
 * @param env
 * @param rows
 * @param bpmd
 * @param n
 * @param cnftype
 * @param cnfnum
 * @param sortOrder
 * @return
 */
static int acquireBpmData(JNIEnv* env, int* rows, int bpmd, int n, int cnftype, int cnfnum, int sortOrder)
{
	vmsstat_t status = 0;
	int4u bpmCount = 0;

	// Initialise acquisition
	status = DPSLCBPM_ACQINIT();
	if (!$VMS_STATUS_SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "while initializing BPM acquisition");
		return EXIT_FAILURE;
	}

	// Acquire BPM values
	status = DPSLCBPM_BPMACQ(&bpmCount, bpmd, n, cnftype, cnfnum);
	if (!$VMS_STATUS_SUCCESS(status)) {
		endAcquireBpmData(env);
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "while making BPM acquisition");
		return EXIT_FAILURE;
	}

	/* Reorder the BPM data by Z if that was required */
	if (sortOrder == SORTORDER_Z) {
		DPSLCBPM_SORTBYZ(bpmCount);
	}

	*rows = bpmCount;
	if (bpmCount == 0) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
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
	int4u rows, rowsy;
	int2u rowsnames, rowsz, rowstmit, rowshsta, rowsstat;

	// data that returns a status
	status = DPSLCBPM_GETX(&rows, xData);
	CHECK_VMS_STATUS
	status = DPSLCBPM_GETY(&rowsy, yData);
	CHECK_VMS_STATUS

	// data that return rows read
	if (!(rowsnames = DPSLCBPM_GETNAMES(namesData)) ||
			!(rowstmit = DPSLCBPM_GETTMIT(tmitData)) ||
			!(rowsz = DPSLCBPM_GETZ(zData)) ||
			!(rowshsta = DPSLCBPM_GETHSTA(hstasData)) ||
			!(rowsstat = DPSLCBPM_GETSTAT(statsData))
			) {
		endAcquireBpmData(env);
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "reading BPM values");
		return EXIT_FAILURE;
	}

	// If rows are not homogenous
	if (!(rows == rowsy && rowsy == rowsnames && rowsnames == rowsz && rowsz == rowstmit && rowstmit == rowshsta
			&& rowshsta == rowsstat)) {
		endAcquireBpmData(env);
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "non-homologous vectors of data returned");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
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

