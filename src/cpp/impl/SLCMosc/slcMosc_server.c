/**
 * Master Oscillator Server implementation
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <descrip.h>              /*  for definition of $DESCRIPTOR  */

#include "aida_server_helper.h"
#include "json.h"

#include "slc_macros.h"    /* vmsstat_t, int2u, int4u, etc. */
#include "msg_proto.h"     /* for standalone_init */
#include "sysutil_proto.h" /* for cvt_vms_to_ieee_flt */

#include "slcMosc_server.h"

static int getMoscArguments(JNIEnv* env, Arguments arguments, Value value, char** units, char** ring, float* floatValue);

// API Stubs
REQUEST_STUB_CHANNEL_CONFIG
REQUEST_STUB_BOOLEAN
REQUEST_STUB_BYTE
REQUEST_STUB_SHORT
REQUEST_STUB_INTEGER
REQUEST_STUB_LONG
REQUEST_STUB_FLOAT
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

/**
 * Initialise the service
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @throws ServerInitialisationException if the service fails to initialise
 */
void aidaServiceInit(JNIEnv* env)
{
	vmsstat_t status;

	if (!SUCCESS(status = DPSLCMOSC_DB_INIT())) {
		aidaThrow(env, status, SERVER_INITIALISATION_EXCEPTION, "initialising Master Oscillator Service");
		return;
	}

	printf("Aida Master Oscillator Service Initialised\n");
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
	// Check if operations are enabled?
	if (!DPSLCMOSC_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"Aida access to Master Oscillator is not currently enabled");
		return 0.0;
	}

	// Read value
	double meas_abs_freq;
	vmsstat_t status;
	status = DPSLCMOSC_MEASMASTEROSC(&meas_abs_freq);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "Unable to get oscillator frequency");
		return 0.0;
	}
	CONVERT_FROM_VMS_DOUBLE(&meas_abs_freq, 1)

	return meas_abs_freq;
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
	// Create table to return value
	Table table = tableCreate(env, 1, 1);
	CHECK_EXCEPTION(table)

	// Get the value
	double meas_abs_freq = aidaRequestDouble(env, uri, arguments);
	CHECK_EXCEPTION(table)

	// Add value to table
	tableAddSingleRowDoubleColumn(env, &table, meas_abs_freq, false);

	// Return table
	return table;
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
	// Keep track of stuff to free
	TRACK_ALLOCATED_MEMORY

	// Check if operations are enabled?
	if (!DPSLCMOSC_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Aida access to Master Oscillator is not currently enabled");
		RETURN_NULL_TABLE;
	}

	// Get arguments
	float floatValue;
	char* units;
	char* ring;
	int getArgStatus =  getMoscArguments(env, arguments, value, &units, &ring, &floatValue);
	TRACK_MEMORY(units)
	TRACK_MEMORY(ring)
	if (getArgStatus) {
		FREE_MEMORY
		RETURN_NULL_TABLE
	}

	// Set value
	vmsstat_t status;
	double resulting_abs_freq; // Value read back
	CONVERT_TO_VMS_FLOAT(&floatValue, 1)
	status = DPSLCMOSC_SETMASTEROSC(&floatValue, units, ring, &resulting_abs_freq);
	FREE_MEMORY
	if (!SUCCESS(status))
	{
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "Unable to set Master Oscillator frequency");
		RETURN_NULL_TABLE
	}

	// Now create table to return
	Table table = tableCreate(env, 1, 1);
	CHECK_EXCEPTION(table)
	tableAddSingleRowDoubleColumn(env, &table, resulting_abs_freq, false);

	return table;
}

/**
 * Get all  Master Oscillator arguments
 *
 * @param env
 * @param arguments
 * @param value
 * @param units
 * @param ring
 * @param floatValue
 * @return true if successful
 */
static int getMoscArguments(JNIEnv* env, Arguments arguments, Value value, char** units, char** ring, float* floatValue)
{
	*units = "FREQUENCY";
	*ring = NULL;
	if (avscanf(env, &arguments, &value, "%f %os %os",
			"value", floatValue,
			"units", units,
			"ring", ring
			)) {
		return EXIT_FAILURE;
	}

	// If the units is set to ENERGY then we *do* require the ring parameter
	if (strcasecmp(*units, "ENERGY") == 0 && *ring == NULL ) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
				"Master Oscillator Set Variable requires a RING parameter if the UNITS are ENERGY");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

