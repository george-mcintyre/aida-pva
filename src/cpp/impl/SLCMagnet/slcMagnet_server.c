/**
 * Magnet Server implementation
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <descrip.h>              /*  for definition of $DESCRIPTOR  */
#include <cvtdef.h>               /*  CVT$K_*, CVT$M_*  */
#include <stdbool.h>               /*  CVT$K_*, CVT$M_*  */
#include <cvtmsg.h>               /*  CVT$_NORMAL  */
#include <cvt$routines.h>         /*  CVT$CONVERT_FLOAT  */

#include "aida_server_helper.h"
#include "json.h"

#include "slcMagnet_server.h"

#include "slc_macros.h"    /* vmsstat_t, int2u, int4u, etc. */
#include "msg_proto.h"     /* for standalone_init */
#include "sysutil_proto.h" /* for cvt_vms_to_ieee_flt */

static int
getBaseMagnetArguments(JNIEnv* env, const char* uri, Arguments arguments, Value value, int* count, char** prim_list,
		char** micr_list, int** unit_list, int4u* secn, float** set_values, char** name_validity);
static int
getMagnetArguments(JNIEnv* env, const char* uri, Arguments arguments, Value value, int* count, char** prim_list,
		char** micr_list, int** unit_list, int4u* secn, float** set_values, char** magFunc, char** limitCheck,
		char** name_validity);
static void magnetDataCleanUp(char* primaryList, char* microList, int* unitList, float* setValues, char* magnetFunction,
		char* limitCheck, char* name_validity);
static bool isAllValid(int count, const char* name_validity);
static void getInvalidNames(char *dst, int count, char *names[], const char* name_validity);

/**
 * Initialise the service
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @throws ServerInitialisationException if the service fails to initialise
 */
void aidaServiceInit(JNIEnv* env)
{
	vmsstat_t status;
	if (!SUCCESS(status = DPSLCMAGNET_DB_INIT())) {
		aidaThrow(env, status, SERVER_INITIALISATION_EXCEPTION, "initialising SLC Magnet Service");
		return;
	}

	printf("Aida Magnet Service Initialised\n");
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
	// Get arguments
	char* micrPattern = NULL, * unitPattern = NULL;
	if (ascanf(env, &arguments, "%os %os",
			"micros", &micrPattern,
			"units", &unitPattern
	)) {
		RETURN_NULL_TABLE;
	}

	// Acquire Magnet values
	int numMagnetPvs;
	vmsstat_t status = DPSLCMAGNET_GET((char*)uri, (micrPattern ? micrPattern : ""), (unitPattern ? unitPattern : ""),
			&numMagnetPvs);
	if (micrPattern) {
		free(micrPattern);
	}
	if (unitPattern) {
		free(unitPattern);
	}
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "while reading magnet values");
		RETURN_NULL_TABLE;
	}

	// To hold the data
	char namesData[(numMagnetPvs * MAX_PMU_STRING_LEN) + 1];
	float secondaryValues[numMagnetPvs];

	// Get Names
	DPSLCMAGNET_GETNAMES(namesData);

	// Get Secondary values
	DPSLCMAGNET_GETSECNVALUES(secondaryValues);

	// Cleanup
	DPSLCMAGNET_GETCLEANUP();

	// Make table and return results
	Table table = tableCreate(env, numMagnetPvs, 2);
	CHECK_EXCEPTION(table)
	tableAddFixedWidthStringColumn(env, &table, namesData, MAX_PMU_STRING_LEN);
	CHECK_EXCEPTION(table)
	tableAddColumn(env, &table, AIDA_FLOAT_TYPE, secondaryValues);

	// All read successfully
	return table;
}

/**
 * Set a value
 * Sets specified BCON secondary values for specified array of magnet names to an array of corresponding set values.
 * The parameter `value` argument is Value containing two elements: an array of magnet names (name)
 * and an array of corresponding set values (values)
 * e.g. { "names": [], "values": [] }
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @param uri the uri
 * @param arguments the arguments
 * @param value to set
 * @return the table
 */
void aidaSetValue(JNIEnv* env, const char* uri, Arguments arguments, Value value)
{
	if (!DPSLCMAGNET_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Aida magnet set operations are not currently enabled");
		return;
	}

	// Get the arguments
	int count;

	char* primaryList = NULL, * microList = NULL, * name_validity = NULL;
	int* unitList = NULL;

	int4u secn;
	float* setValues = NULL;

	if (getBaseMagnetArguments(env, uri,
			arguments, value, &count, &primaryList, &microList, &unitList, &secn, &setValues, &name_validity)) {
		magnetDataCleanUp(primaryList, microList, unitList, setValues, NULL, NULL, name_validity);
		return;
	}

	// set the PVs specified by the lists of primary, micros, and units
	vmsstat_t status;
	status = DPSLCMAGNET_SETCONFIG(count, primaryList, microList, unitList, secn, setValues);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "while setting magnet values");
	}

	// Clean up
	DPSLCMAGNET_SETCLEANUP();

	// Cleanup
	magnetDataCleanUp(primaryList, microList, unitList, setValues, NULL, NULL, name_validity);
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
	if (!DPSLCMAGNET_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Aida magnet set operations are not currently enabled");
		RETURN_NULL_TABLE;
	}

	// Get arguments
	int count;
	int4u secondary;
	char* primaryList, * microList, * magnetFunction, * limitCheck, * name_validity;
	int* unitList;
	float* setValues;
	int limitCheckAll = true;
	if (getMagnetArguments(env, uri, arguments, value, &count, &primaryList, &microList, &unitList, &secondary,
			&setValues,
			&magnetFunction, &limitCheck, &name_validity)) {
		 magnetDataCleanUp(primaryList, microList, unitList, setValues, magnetFunction, limitCheck, name_validity);
		RETURN_NULL_TABLE;
	}

	if (strcasecmp(magnetFunction, "TRIM") != 0 && strcasecmp(magnetFunction, "PTRB") != 0
			&& strcasecmp(magnetFunction, "NOFUNC") != 0) {
		 magnetDataCleanUp(primaryList, microList, unitList, setValues, magnetFunction, limitCheck, name_validity);
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION, "MAGFUNC parameter must be TRIM, PTRB or NOFUNC");
		RETURN_NULL_TABLE;
	}

	if (limitCheck && strcasecmp(limitCheck, "ALL") != 0 && strcasecmp(magnetFunction, "SOME") != 0) {
		 magnetDataCleanUp(primaryList, microList, unitList, setValues, magnetFunction, limitCheck, name_validity);
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION, "LIMITCHECK parameter must be ALL or SOME");
		RETURN_NULL_TABLE;
	}

	if (limitCheck) {
		limitCheckAll = false;
	}

	// Check limits
	bool withinLimits[count], allSetValuesWithinLimits;  // true if within limits
	int numPairsWithinLimits = 0;
	float magnetLimits[count * 2];
	DPSLCMAGNET_RET_MAGLIMITS(count, &magnetLimits[0]);
	CONVERT_FROM_VMS_FLOAT(&magnetLimits[0])
	for (int i = 0; i < count; i++) {
		float lowerLimit = magnetLimits[i * 2];
		float upperLimit = magnetLimits[i * 2 + 1];
		if ((setValues[i] < lowerLimit) || (setValues[i] > upperLimit)) {
			withinLimits[i] = false;
			allSetValuesWithinLimits = false;
		} else {
			withinLimits[i] = true;
			numPairsWithinLimits++;
		}
	}
	if (!allSetValuesWithinLimits && limitCheckAll || !numPairsWithinLimits) {
		magnetDataCleanUp(primaryList, microList, unitList, setValues, magnetFunction, limitCheck, name_validity);
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Some or all of the set values are outside of the limits");
		RETURN_NULL_TABLE;
	}

	// Use only the values that are within limits
	char limitedPrimList[(numPairsWithinLimits * MAX_PRIM_NAME_LEN) + 1];
	char limitedMicrList[(numPairsWithinLimits * MAX_PRIM_NAME_LEN) + 1];
	int limitedUnitList[numPairsWithinLimits];
	float limitedValuesList[numPairsWithinLimits];

	int limitedCounter = 0;
	for (int i = 0; i < count; i++) {
		if (withinLimits[i]) {
			strcpy((limitedPrimList + limitedCounter * MAX_PRIM_NAME_LEN), (primaryList + i * MAX_PRIM_NAME_LEN));
			strcpy((limitedMicrList + limitedCounter * MAX_PRIM_NAME_LEN), (microList + i * MAX_PRIM_NAME_LEN));
			limitedUnitList[limitedCounter] = unitList[i];
			limitedValuesList[limitedCounter] = setValues[i];
			limitedCounter++;
		}
	}

	// set the PVs specified by the lists of primary, micros, and units
	vmsstat_t status;
	status = DPSLCMAGNET_SET(numPairsWithinLimits, limitedPrimList, limitedMicrList, limitedUnitList, secondary,
			limitedValuesList, magnetFunction);
	magnetDataCleanUp(primaryList, microList, unitList, setValues, magnetFunction, limitCheck, name_validity);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "while setting magnet values");
		RETURN_NULL_TABLE;
	}

	// To hold data
	int rows = DPSLCMAGNET_RET_SETNUMPVS();
	char namesData[(rows * MAX_STATE_NAME_LEN) + 1];
	float bactData[rows];

	// Get data
	DPSLCMAGNET_RET_STATENAMES(namesData);
	DPSLCMAGNET_RET_BACTVALUES(bactData);

	// Clean up
	DPSLCMAGNET_SETCLEANUP();

	Table table = tableCreate(env, rows, 2);
	CHECK_EXCEPTION(table)
	tableAddFixedWidthStringColumn(env, &table, namesData, MAX_STATE_NAME_LEN);
	CHECK_EXCEPTION(table)
	tableAddColumn(env, &table, AIDA_FLOAT_TYPE, bactData);

	return table;
}

/**
 * Get basic magnet arguments
 *
 * @param env
 * @param uri
 * @param value
 * @param count
 * @param prim_list
 * @param micr_list
 * @param unit_list
 * @param secn
 * @param set_values
 * @return
 */
static int
getBaseMagnetArguments(JNIEnv* env, const char* uri, Arguments arguments, Value value, int* count, char** prim_list,
		char** micr_list, int** unit_list, int4u* secn, float** set_values, char** name_validity)
{
	*prim_list = NULL;
	*micr_list = NULL;
	*unit_list = NULL;
	*set_values = NULL;
	*name_validity = NULL;

	unsigned int nNames, nValues;
	char** names = NULL;

	// Get congruent list of values to set, name and value pairs
	// value contains an object with two congruent arrays
	if (avscanf(env, &arguments, &value, " %sa %fa",
			"value.names", &names, &nNames,
			"value.values", set_values, &nValues
	)) {
		return EXIT_FAILURE;
	}

	// Find out how many values were supplied and check that both lists are the same length
	*count = (int)nNames;
	if (nNames != nValues) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION, "supplied lists are not the same length");
		return EXIT_FAILURE;
	}

	// Get names array - and break up names into their components for other lists
	*prim_list = (char*)malloc((*count * MAX_PRIM_NAME_LEN) + 1);
	*micr_list = (char*)malloc((*count * MAX_MICR_NAME_LEN) + 1);
	*unit_list = (int*)malloc(*count * sizeof(unsigned long));
	unsigned long longUnitList[*count];
	for (int i = 0; i < nNames; i++) {
		pmuFromUri(names[i], *prim_list + i * MAX_PRIM_NAME_LEN, *micr_list + i * MAX_PRIM_NAME_LEN, &longUnitList[i]);
		((int*)*unit_list)[i] = (int)longUnitList[i];
	}
	// Secondary name
	secnFromUri(uri, secn);

	// Call names validate to see which names are valid
	*name_validity = (char*)malloc((*count * MAX_VALIDITY_STRING_LEN) + 1);
	DPSLCMAGNET_SETNAMESVALIDATE(*count, *prim_list, *micr_list, *unit_list, *secn, *name_validity);

	// Check if all names are valid
	if (!isAllValid(*count, *name_validity)) {
		char invalidNames[MAX_URI_LEN * nNames + 1];
		getInvalidNames(invalidNames, *count, names, *name_validity);
		free(names);
		SPRINF_ERROR(UNABLE_TO_SET_DATA_EXCEPTION, "Some of the names were not valid: %s", invalidNames, EXIT_FAILURE)
	}

	// Free up the names as we don't need them anymore as they have been validated
	free(names);

	// Get convert values to VMS floats
	for (int i = 0; i < nValues; i++) {
		CONVERT_TO_VMS_FLOAT(set_values[i]);
	}

	return EXIT_SUCCESS;
}

/**
 * Get full set of arguments
 *
 * @param env
 * @param uri
 * @param arguments
 * @param value
 * @param count
 * @param prim_list
 * @param micr_list
 * @param unit_list
 * @param secn
 * @param set_values
 * @param magFunc
 * @param limitCheck
 * @return
 */
static int
getMagnetArguments(JNIEnv* env, const char* uri, Arguments arguments, Value value, int* count, char** prim_list,
		char** micr_list, int** unit_list, int4u* secn, float** set_values, char** magFunc, char** limitCheck,
		char** name_validity)
{
	*magFunc = NULL;
	*limitCheck = NULL;
	if (getBaseMagnetArguments(env, uri, arguments, value, count, prim_list, micr_list, unit_list, secn, set_values,
			name_validity)) {
		return EXIT_FAILURE;
	}

	if (ascanf(env, &arguments, "%s %os",
			"magfunc", magFunc,
			"limitcheck", limitCheck
	)) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/**
 * Clean up data space allocated for magnet functions
 *
 * @param primaryList
 * @param microList
 * @param unitList
 * @param setValues
 */
static void magnetDataCleanUp(char* primaryList, char* microList, int* unitList, float* setValues, char* magnetFunction,
		char* limitCheck, char* name_validity)
{
	// Clean up
	if (primaryList) {
		free(primaryList);
	}
	if (microList) {
		free(microList);
	}
	if (unitList) {
		free(unitList);
	}
	if (setValues) {
		free(setValues);
	}
	if (magnetFunction) {
		free(magnetFunction);
	}
	if (limitCheck) {
		free(limitCheck);
	}
	if (name_validity) {
		free(name_validity);
	}
}

/**
 * Check if all the names are valid.  String will be Success if valid
 *
 * @param count
 * @param name_validity
 * @return true if all are valid
 */
bool isAllValid(int count, const char* name_validity)
{
	if (!name_validity) {
		return false;
	}

	for (int i = 0; i < count; i++) {
		if (strncasecmp(name_validity + (i * MAX_VALIDITY_STRING_LEN), "Success", strlen("Success")) != 0) {
			return false;
		}
	}

	return true;
}

/**
 * Get a string listing the invalid names into a pre allocated buffer
 * @param dst pre-allocated buffer
 * @param count the number if names in the provided array
 * @param names the array of pointers to names
 * @param name_validity the name validity structure returned from the back end
 */
static void getInvalidNames(char *dst, int count, char *names[], const char* name_validity)
{
	if (!name_validity || !names ) {
		return;
	}

	for (int i = 0; i < count; i++) {
		if (strncasecmp(name_validity + (i * MAX_VALIDITY_STRING_LEN), "Success", strlen("Success")) != 0) {
			strcat(dst, names[i]);
			if ( i != count-1 ) {
				strcat(dst, ", ");
			}
		}
	}

	return;
}

