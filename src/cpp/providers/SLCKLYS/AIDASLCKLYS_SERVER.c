/** @file
 *  @brief Klystron Native Provider implementation.
 *     **MEMBER**=SLCLIBS:AIDA_PVALIB
 *     **ATTRIBUTES**=JNI,LIBR_NOGLOBAL
 */
#include "aida_pva.h"
#include "AIDASLCKLYS_SERVER.h"

static int getKlystronStatus(JNIEnv* env, const char* uri, Arguments arguments, short* klys_status);
static int klystronStatus(JNIEnv* env, char slcName[30], char* beam_c, char* dgrp_c, short* klys_status);

static Table setActivateValue(JNIEnv* env, const char* uri, Arguments arguments, Value value);
static Table setPdesValue(JNIEnv* env, const char* uri, Arguments arguments, Value value,
		char* pmu, char* secn);
static Table setKphrValue(JNIEnv* env, const char* uri, Arguments arguments, Value value,
		char* pmu, char* secn);
static Table setPdesOrKphrValue(JNIEnv* env, const char* uri, Arguments arguments, Value value,
		char* trim, char* pmu, char* secn);
static void setPconOrAconValue(JNIEnv* env, Arguments arguments, Value value, char* pmu, char* secn);

// API Stubs
REQUEST_STUB_BOOLEAN
REQUEST_STUB_BYTE
REQUEST_STUB_INTEGER
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

/**
 * Initialise the service
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @throws ServerInitialisationException if the service fails to initialise
 */
void aidaServiceInit(JNIEnv* env)
{
	vmsstat_t status;

	if (!$VMS_STATUS_SUCCESS(status = init("AIDA_SLCKLYS", false))) {
		aidaThrow(env, status, SERVER_INITIALISATION_EXCEPTION, "initialising Klystron Service");
		return;
	}

	printf("Aida Klystron Service Initialised\n");
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
	short klystronStatus;
	if (getKlystronStatus(env, uri, arguments, &klystronStatus)) {
		return -1;
	}
	return klystronStatus;
}

/**
 * Get a long
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the long
 */
long aidaRequestLong(JNIEnv* env, const char* uri, Arguments arguments)
{
	short klystronStatus;
	if (getKlystronStatus(env, uri, arguments, &klystronStatus)) {
		return -1;
	}
	return (long)klystronStatus;
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
	short klystronStatus;
	if (getKlystronStatus(env, uri, arguments, &klystronStatus)) {
		return NULL;
	}

	if (klystronStatus) {
		return ALLOCATE_STRING(env, "activated", "string");
	} else {
		return ALLOCATE_STRING(env, "deactivated", "string");
	}
}

/**
 * Get a table of data
 */
Table aidaRequestTable(JNIEnv* env, const char* uri, Arguments arguments)
{
	short klystronStatus;
	if (getKlystronStatus(env, uri, arguments, &klystronStatus)) {
		RETURN_NULL_TABLE
	}

	bool isInAccelerateState = (short)(klystronStatus & LINKLYSTA_ACCEL) ? true : false;
	bool isInStandByState = (short)(klystronStatus & LINKLYSTA_STANDBY) ? true : false;
	bool isInBadState = (short)(klystronStatus & LINKLYSTA_BAD) ? true : false;
	bool isSledTuned = (short)(klystronStatus & LINKLYSTA_SLED_TUNED) ? true : false;
	bool isSleded = (short)(klystronStatus & LINKLYSTA_SLEDED) ? true : false;
	bool isPampl = (short)(klystronStatus & LINKLYSTA_PAMPL) ? true : false;
	bool isPphas = (short)(klystronStatus & LINKLYSTA_PPHAS) ? true : false;

	Table table = tableCreate(env, 1, 7);
	ON_EXCEPTION_RETURN_(table)
	tableAddSingleRowBooleanColumn(env, &table, isInAccelerateState);
	ON_EXCEPTION_RETURN_(table)
	tableAddSingleRowBooleanColumn(env, &table, isInStandByState);
	ON_EXCEPTION_RETURN_(table)
	tableAddSingleRowBooleanColumn(env, &table, isInBadState);
	ON_EXCEPTION_RETURN_(table)
	tableAddSingleRowBooleanColumn(env, &table, isSledTuned);
	ON_EXCEPTION_RETURN_(table)
	tableAddSingleRowBooleanColumn(env, &table, isSleded);
	ON_EXCEPTION_RETURN_(table)
	tableAddSingleRowBooleanColumn(env, &table, isPampl);
	ON_EXCEPTION_RETURN_(table)
	tableAddSingleRowBooleanColumn(env, &table, isPphas);

	return table;
}

/**
 * Set a value
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @param value to set
 */
void aidaSetValue(JNIEnv* env, const char* uri, Arguments arguments, Value value)
{
	if (!DPSLCKLYS_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Aida access to klystron operations is not currently enabled");
		return;
	}

	PMU_STRING_FROM_URI(pmu_str, uri)

	if (endsWith(uri, "PCON")) {
		setPconOrAconValue(env, arguments, value, pmu_str, "PCON");
	} else if (endsWith(uri, "ACON")) {
		setPconOrAconValue(env, arguments, value, pmu_str, "ACON");
	} else {
		aidaThrowNonOsException(env, UNSUPPORTED_CHANNEL_EXCEPTION, uri);
		return;
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
	if (!DPSLCKLYS_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Aida access to klystron operations is not currently enabled");
		RETURN_NULL_TABLE
	}

	PMU_STRING_FROM_URI(pmu_str, uri)

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

static void setPconOrAconValue(JNIEnv* env, Arguments arguments, Value value, char* pmu, char* secn)
{
	if (value.type != AIDA_STRING_TYPE) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION, "Missing value to PCON or ACON value");
		return;
	}

	float floatValue;
	if (avscanf(env, &arguments, &value, "%f", "value", &floatValue)) {
		return;
	}

	// Set the value
	vmsstat_t status;
	CONVERT_TO_VMS_FLOAT(&floatValue, 1)

	status = DPSLCKLYS_SETCONFIG(pmu, &floatValue, secn);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, MISSING_REQUIRED_ARGUMENT_EXCEPTION, NULL);
	}
}

static Table setKphrValue(JNIEnv* env, const char* uri, Arguments arguments, Value value, char* pmu, char* secn)
{
	return setPdesOrKphrValue(env, uri, arguments, value, NULL, pmu, secn);
}

static Table setPdesValue(JNIEnv* env, const char* uri, Arguments arguments, Value value, char* pmu, char* secn)
{
	unsigned char trim = true;

	if (ascanf(env, &arguments, "%ob", "TRIM", &trim)) {
		RETURN_NULL_TABLE
	}
	return setPdesOrKphrValue(env, uri, arguments, value, trim ? "YES" : "NO", pmu, secn);
}

static Table
setPdesOrKphrValue(JNIEnv* env, const char* uri, Arguments arguments, Value value, char* trim, char* pmu, char* secn)
{
	// Get the value to set
	float secnValue;
	if ( avscanf(env, &arguments, &value, "%f", "value", &secnValue) ) {
		RETURN_NULL_TABLE;
	}

	// Set the value
	float phas_value;  /* Returned in ieee format */
	CONVERT_TO_VMS_FLOAT(&secnValue, 1)
	vmsstat_t status = DPSLCKLYS_SETTRIMPHASE(pmu, secn, &secnValue, trim, &phas_value);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "Error setting value");
		RETURN_NULL_TABLE;
	}

	Table table = tableCreate(env, 1, 1);
	ON_EXCEPTION_RETURN_(table)
	tableAddSingleRowFloatColumn(env, &table, phas_value, false);

	return table;
}

static Table setActivateValue(JNIEnv* env, const char* uri, Arguments arguments, Value value)
{
	// Keep track of stuff to free
	TRACK_ALLOCATED_MEMORY

	// Get the arguments
	char* beam_c;
	char* dgrp_c = NULL;
	unsigned char isActivateOperation;

	if (avscanf(env, &arguments, &value, "%b %s %os",
			"value", &isActivateOperation,
			"beam", &beam_c,
			"dgrp", &dgrp_c
	)) {
		RETURN_NULL_TABLE
	}
	PMU_STRING_FROM_URI(shortSlcName, uri)

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
	if (isActivateOperation && !isInStandByState) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Cannot reactivate klystron when not in standby state");
		FREE_MEMORY
		RETURN_NULL_TABLE
	}

	// If trying to deactivate but not in accelerate state
	if (!isActivateOperation && !isInAccelerateState) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Cannot deactivate klystron when not in accelerate state");
		FREE_MEMORY
		RETURN_NULL_TABLE
	}

	// Set the new value
	int status = DPSLCKLYS_SETDEACTORREACT(shortSlcName, isActivateOperation, beam_c);
	if (!SUCCESS(status)) {
		FREE_MEMORY
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "Could not set activation state");
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
	ON_EXCEPTION_RETURN_(table)
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

	TO_SLC_NAME(uri, slcName)
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
