/**
 * Master Oscillator Server implementation
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <descrip.h>              /*  for definition of $DESCRIPTOR  */
#include <stdbool.h>

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
 * @param channelName
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @return the channel config.  Leave empty if you don't have any specific configuration overrides
 */
Config aidaChannelConfig(JNIEnv* env, const char* channelName)
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
	vmsstat_t status = 0;

	if (!DPSLCMOSC_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"Aida access to Master Oscillator is not currently enabled");
		return 0.0;
	}

	double meas_abs_freq;

	int2u one = 1;
	status = DPSLCMOSC_MEASMASTEROSC(&meas_abs_freq);
	if (!SUCCESS(status)) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Unable to get oscillator frequency");
		return 0.0;
	}
	CVT_VMS_TO_IEEE_DBL(&meas_abs_freq, &meas_abs_freq, &one);

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
	Table table;
	memset(&table, 0, sizeof(table));
	table.columnCount = 0;

	vmsstat_t status = 0;

	if (!DPSLCMOSC_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"Aida access to Master Oscillator is not currently enabled");
		return table;
	}

	double meas_abs_freq;

	int2u one = 1;
	status = DPSLCMOSC_MEASMASTEROSC(&meas_abs_freq);
	if (!SUCCESS(status)) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Unable to get oscillator frequency");
		return table;
	}
	CVT_VMS_TO_IEEE_DBL(&meas_abs_freq, &meas_abs_freq, &one);

	// Now create table to return

	// Set columns
	table.columnCount = 1;
	table.rowCount = 1;

	// Allocate space for table data
	if (initTable(env, &table) == NULL) {
		return table;
	}

	// Allocate space for rows of data
	for (int column = 0; column < table.columnCount; column++) {
		switch (column) {
		case 0: // names
			tableDoubleColumn(&table, column);
			break;
		default: // unsupported
			fprintf(stderr, "Unsupported table column type: %d\n", table.types[column]);
			break;
		}
	}

	// Get all data
	double* doubleArray;

	// Secondary values
	doubleArray = (double*)(table.ppData[0]);
	doubleArray[0] = meas_abs_freq;

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
	vmsstat_t status = 0;

	if (!DPSLCMOSC_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Aida access to Master Oscillator is not currently enabled");
		return table;
	}

	// Get Unit, Ring and Value
	float floatValue;
	double resulting_abs_freq;
	char* units;
	char* ring;

	if (getMoscArguments(env, arguments, value, &units, &ring, &floatValue)) {
		return table;
	}

	// Set value
	int2u one = 1;
	status = DPSLCMOSC_SETMASTEROSC(&floatValue, units, ring, &resulting_abs_freq);
	if (!SUCCESS(status))
	{
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Unable to set master oscilator frequency");
		return table;
	}
	CVT_VMS_TO_IEEE_DBL(&resulting_abs_freq, &resulting_abs_freq, &one);

	// Now create table to return

	// Set columns
	table.columnCount = 1;
	table.rowCount = 1;

	// Allocate space for table data
	if (initTable(env, &table) == NULL) {
		return table;
	}

	// Allocate space for rows of data
	for (int column = 0; column < table.columnCount; column++) {
		switch (column) {
		case 0: // names
			tableDoubleColumn(&table, column);
			break;
		default: // unsupported
			fprintf(stderr, "Unsupported table column type: %d\n", table.types[column]);
			break;
		}
	}

	// Get all data
	double* doubleArray;

	// Secondary values
	doubleArray = (double*)(table.ppData[0]);
	doubleArray[0] = resulting_abs_freq;

	return table;
}

static int getMoscArguments(JNIEnv* env, Arguments arguments, Value value, char** units, char** ring, float* floatValue)
{
	if (value.type != AIDA_STRING_TYPE) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
				"Master Oscillator Set Variable requires a AValue parameter to set to a float");
		return 1;
	}
	sscanf(value.value.stringValue, "%f", floatValue);

	Argument argument = getArgument(arguments, "units");
	*units = "FREQUENCY";
	if (argument.name) {
		*units = argument.value;
	}

	argument = getArgument(arguments, "ring");
	*ring = NULL;
	if (argument.name) {
		*ring = argument.value;
	}

	if (strcasecmp(*units, "ENERGY") == 0 && *ring == NULL ) {
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
				"Master Oscillator Set Variable requires a RING parameter if the UNITS are ENERGY");
		return 2;
	}

	return 0;
}

