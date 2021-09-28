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
 * Get channel configuration
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param channelName
 * @param forGetter true to return config for getter, false for setter
 * @return the channel config.  Leave empty if you don't have any specific configuration overrides
 */
Config aidaChannelConfig(JNIEnv* env, const char* channelName, short forGetter)
{
	DEFAULT_CONFIG_REQUEST
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
	UNSUPPORTED_ARRAY_REQUEST
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
	UNSUPPORTED_ARRAY_REQUEST
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
	UNSUPPORTED_ARRAY_REQUEST
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
	UNSUPPORTED_ARRAY_REQUEST
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
	UNSUPPORTED_ARRAY_REQUEST
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
	UNSUPPORTED_ARRAY_REQUEST
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
	UNSUPPORTED_ARRAY_REQUEST
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
	UNSUPPORTED_STRING_ARRAY_REQUEST
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
	tableAddSingleRowDoubleColumn(env, &table, meas_abs_freq);

	// Return table
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

	CONVERT_FROM_VMS_DOUBLE(&resulting_abs_freq, 1)

	// Now create table to return
	Table table = tableCreate(env, 1, 1);
	CHECK_EXCEPTION(table)
	tableAddSingleRowDoubleColumn(env, &table, resulting_abs_freq);

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

