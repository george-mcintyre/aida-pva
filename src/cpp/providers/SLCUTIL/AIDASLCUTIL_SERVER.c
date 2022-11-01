/** @file
 *  @brief Utility Native Provider implementation.
 *     **MEMBER**=SLCLIBS:AIDA_PVALIB
 *     **ATTRIBUTES**=JNI,LIBR_NOGLOBAL
 */
#include <descrip.h>              /* for definition of $DESCRIPTOR  */
#include <float.h>
#include "aida_pva.h"
#include "descr.h"               /* DESCR* macros */
#include "ref.h"                  /* passing by reference macros */
#include "crrcntl_parms.h"
#include "bpmutil_proto.h"
#include "AIDASLCUTIL_SERVER.h"

static Table setTriggerValue(JNIEnv* env, const char* uri, Arguments arguments, Value value);
static Table setMkbValue(JNIEnv* env, const char* uri, Arguments arguments, Value value);
static short getTrigStatus(JNIEnv* env, const char* uri, Arguments arguments);
static void setBGrp(JNIEnv* env, Arguments arguments, Value value);
static void setFeedback(JNIEnv* env, const char* uri, Arguments arguments, Value value);
static int4u faStatus(JNIEnv* env, const char* uri, int newHSTA, BOOLEAN retrieveOnly);
static short getShortFaStatus(JNIEnv* env, const char* uri);
static char* GetStringFaStatus(JNIEnv* env, const char* uri);
static Table phaseScan(JNIEnv* env, const char* uri, Arguments arguments);
static bool getAcqParameters(JNIEnv* env, Arguments arguments, char** buttonFileName,
		char** primaryStepVariable, float* primaryLow, float* primaryHigh, int* primarySteps, float* primarySettleTime,
		float* primaryExtraSettleTime,
		char** secondaryStepVariable, float* secondaryLow, float* secondaryHigh, int* secondarySteps,
		float* secondarySettleTime, float* secondaryExtraSettleTime, short* bpmd, long* nrpos, long* dtizavg,
		char** magnetFunction);
static void
createCorrelationPlotTable(JNIEnv* env, Table* table, int totalSampleVariables, int nSteps, char* primaryStepVariable,
		bool secondaryStepVariableProvided,
		char* secondaryStepVariable, float* samples, long* samplesOk);
static int countSampleVariables(JNIEnv* env, long* nSampleVariables);

// API Stubs
VERSION("1.0.0")
REQUEST_STUB_BYTE
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
void aidaServiceInit(JNIEnv* env) {
	vmsstat_t status;
	if (!$VMS_STATUS_SUCCESS(status = init("AIDA_SLCUTIL", false))) {
		aidaThrow(env, status, SERVER_INITIALISATION_EXCEPTION, "initialising Utility Service");
	} else {
		printf("AIDA-PVA Utility Provider\n");
	}
}

/**
 * Get a short
 *
 * @param uri the uri
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param arguments the arguments
 * @return the short
 */
short aidaRequestShort(JNIEnv* env, const char* uri, Arguments arguments) {
	if (startsWith(uri, "FBCK")) {
		return getShortFaStatus(env, uri);
	} else {
		return getTrigStatus(env, uri, arguments);
	}
}

/**
 * Get an integer
 *
 * @param uri the uri
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param arguments the arguments
 * @return the integer
 */
int aidaRequestInteger(JNIEnv* env, const char* uri, Arguments arguments) {
	return getShortFaStatus(env, uri);
}

/**
 * Get a Boolean
 *
 * @param uri the uri
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param arguments the arguments
 * @return the boolean
 */
int aidaRequestBoolean(JNIEnv* env, const char* uri, Arguments arguments) {
	return getShortFaStatus(env, uri) == 0 ? false : true;
}

/**
 * Get a integer
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the integer
 */
long aidaRequestLong(JNIEnv* env, const char* uri, Arguments arguments) {
	if (startsWith(uri, "FBCK")) {
		return (long)faStatus(env, uri, 0, TRUE_B);
	} else {
		return (long)getTrigStatus(env, uri, arguments);
	}
}

/**
 * Get a string.  Allocate memory for string and it will be freed for you by framework
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the string
 */
char* aidaRequestString(JNIEnv* env, const char* uri, Arguments arguments) {
	if (startsWith(uri, "FBCK")) {
		return GetStringFaStatus(env, uri);
	} else {
		// Read the status
		short trig_status = getTrigStatus(env, uri, arguments);
		ON_EXCEPTION_RETURN_(NULL)

		if (trig_status) {
			return ALLOCATE_STRING(env, "activated", "string");
		} else {
			return ALLOCATE_STRING(env, "deactivated", "string");
		}
	}
}

/**
 * Implement Correlated Plots
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @return the table
 *
 * The `FILE` contains the name of the button file and is used
 * The `PRIMSTV` contains a structure with configuration for the primary step variable
 * The `SECNSTV` optionally contains a structure with configuration for the secondary step variable
 *
 * First the button file is loaded and then it is executed.
 * A table with the following fields is returned:
 * 		`primary`		    FLOAT_ARRAY		value of primary step variable for this data set
 * 		`secondary`	        FLOAT_ARRAY		value of secondary step variable for this data set.  Omitted if no secondary step variable specified
 * 		`samp{n}`			FLOAT_ARRAY		For each sample variable found in the given FILE a column is returned.  {n} is the sample var number
 *
 * 	The labels for each column are set to the device name
 */
Table aidaRequestTable(JNIEnv* env, const char* uri, Arguments arguments) {
	REF_DECLARE;
	DESCR_DECLARE;

	TRACK_ALLOCATED_MEMORY

	// Blank table
	Table table;
	table.columnCount = 0;

	// Get button file name
	char* buttonFileName;
	char* primaryStepVariable, * secondaryStepVariable, * magnetFunction;
	float primaryLow, primaryHigh, primarySettleTime, primaryExtraSettleTime, secondaryLow, secondaryHigh, secondarySettleTime, secondaryExtraSettleTime, settle;
	int primarySteps, secondarySteps;
	long nrpos = NRPOS_DEFAULT, dtizavg;
	short bpmd = BPMD_ROGUE;

	bool secondaryStepVariableProvided = getAcqParameters(env, arguments, &buttonFileName, &primaryStepVariable,
			&primaryLow, &primaryHigh, &primarySteps,
			&primaryExtraSettleTime, &primarySettleTime, &secondaryStepVariable, &secondaryLow, &secondaryHigh,
			&secondarySteps,
			&secondarySettleTime, &secondaryExtraSettleTime, &bpmd, &nrpos, &dtizavg, &magnetFunction);
	ON_EXCEPTION_RETURN_(table)
	TRACK_MEMORY(buttonFileName)
	TRACK_MEMORY(primaryStepVariable)
	TRACK_MEMORY(secondaryStepVariable)

	vmsstat_t status;

	// Initialize correlation plot context
	status = CRR_RESET_ALL();
	if (!SUCCESS(status)) {
		SPRINTF_ERROR_STATUS_FREE_MEMORY_AND_RETURN_(status, UNABLE_TO_GET_DATA_EXCEPTION,
				"error resetting correlation plots: %s", buttonFileName, table)
	}

	// Local constant for fortran parameter
	float wire_size = 0.0f;
	float inc = (primaryHigh - primaryLow) / (float)(primarySteps - 1);

	// Set up primary step variable
	status = CRR_SET_VAR_NAME_FREEFORM(REFINT4_1(DEFINE_STEP), REFINT4_2(1), DESCRN1(primaryStepVariable),
			REFINT4_3(0));
	if (!SUCCESS(status)) {
		SPRINTF_ERROR_STATUS_FREE_MEMORY_AND_RETURN_(status, UNABLE_TO_GET_DATA_EXCEPTION,
				"error setting primary step variable name: %s", primaryStepVariable, table)
	}
	status = CRR_SET_STEP(REFINT4_1(1), &primaryLow, &inc, &primarySteps, &primarySettleTime);
	if (!SUCCESS(status)) {
		SPRINTF_ERROR_STATUS_FREE_MEMORY_AND_RETURN_(status, UNABLE_TO_GET_DATA_EXCEPTION,
				"error setting primary step variable: %s", primaryStepVariable, table)
	}

	// Set secondary step variable
	if (secondaryStepVariableProvided) {
		status = CRR_SET_VAR_NAME_FREEFORM(REFINT4_1(DEFINE_STEP), REFINT4_2(-1), DESCRN1(secondaryStepVariable),
				REFINT4_3(0));
		if (!SUCCESS(status)) {
			SPRINTF_ERROR_STATUS_FREE_MEMORY_AND_RETURN_(status, UNABLE_TO_GET_DATA_EXCEPTION,
					"error setting secondary step variable name: %s", secondaryStepVariable, table)
		}

		inc = (secondaryHigh - secondaryLow) / (float)(secondarySteps - 1);
		status = CRR_SET_STEP(REFINT4_1(-1), &secondaryLow, &inc, &secondarySteps, &secondarySettleTime);
		if (!SUCCESS(status)) {
			SPRINTF_ERROR_STATUS_FREE_MEMORY_AND_RETURN_(status, UNABLE_TO_GET_DATA_EXCEPTION,
					"error setting secondary step variable: %s", secondaryStepVariable, table)
		}
	}

	// Make absolute file name
	char fullyQualifiedButtonFileName[strlen(buttonFileName) + strlen(SLC_BUTTON_DIR) + 1];
	sprintf(fullyQualifiedButtonFileName, "%s%s", SLC_BUTTON_DIR, buttonFileName);

	// Load button file
	status = CRR_RESTORE_BUTTON_FILE(DESCRN1(fullyQualifiedButtonFileName));
	if (!SUCCESS(status)) {
		SPRINTF_ERROR_STATUS_FREE_MEMORY_AND_RETURN_(status, UNABLE_TO_GET_DATA_EXCEPTION,
				"error loading scan parameters from button file: %s", fullyQualifiedButtonFileName, table)
	}

	// Set BPM Measurement Definition if it has been specified
	if (bpmd != BPMD_ROGUE) {
		status = BPMD_SELECT_UTIL(&bpmd, NULL, NULL, NULL, NULL, NULL);
		if (!SUCCESS(status)) {
			fprintf(stderr, "Failed to set Measurement Definition: %hd", bpmd);
			SPRINTF_ERROR_STATUS_FREE_MEMORY_AND_RETURN_(status, UNABLE_TO_GET_DATA_EXCEPTION,
					"error initializing correlation plot acquisition: %s", buttonFileName, table)
		}
	}

	long magfunc;
	memcpy(&magfunc, magnetFunction, 4);
	float settleExtra[2];
	settleExtra[0] = primaryExtraSettleTime;
	settleExtra[1] = secondaryExtraSettleTime;

	status = CRR_SET_SAMP_PARMS(&nrpos, &dtizavg, NULL, &magfunc, NULL, NULL, NULL, &settleExtra[0]);
	if (!SUCCESS(status)) {
		fprintf(stderr,
				"Failed to set sample params.  nrpos: %ld, dtizavg: %ld,  magfunc: %ld,  primSettleExtra: %f,  secnSettleExtra: %f",
				nrpos, dtizavg, magfunc, settleExtra[0], settleExtra[1]);
		SPRINTF_ERROR_STATUS_FREE_MEMORY_AND_RETURN_(status, UNABLE_TO_GET_DATA_EXCEPTION,
				"error initializing correlation plot acquisition: %s", buttonFileName, table)
	}

	status = CRR_DATA_ACQ_INIT();
	if (!SUCCESS(status)) {
		SPRINTF_ERROR_STATUS_FREE_MEMORY_AND_RETURN_(status, UNABLE_TO_GET_DATA_EXCEPTION,
				"error initializing correlation plot acquisition: %s", buttonFileName, table)
	}

	// Run Correlation plot
	status = CRR_DATA_ACQ(DESCRN1("*"));
	if (!SUCCESS(status)) {
		CRR_DATA_ACQ_DONE();
		SPRINTF_ERROR_STATUS_FREE_MEMORY_AND_RETURN_(status, UNABLE_TO_GET_DATA_EXCEPTION,
				"executing correlation plot: %s", buttonFileName, table)
	}
	printf("Scan Complete");

	// Number of Samples
	long nSampleVariables;
	if (countSampleVariables(env, &nSampleVariables)) {
		CRR_DATA_ACQ_DONE();
		return table;
	}

	// Get step count.
	int nSteps = CRR_SAMPLES();
	if (!SUCCESS(status)) {
		CRR_DATA_ACQ_DONE();
		SPRINTF_ERROR_FREE_MEMORY_AND_RETURN_(UNABLE_TO_GET_DATA_EXCEPTION,
				"error getting number of steps: %s", buttonFileName, table)
	}

	// Get total number of samples
	long totalSampleVariables = nSampleVariables + ((secondaryStepVariableProvided) ? 2 : 1);
	if (!totalSampleVariables || !nSampleVariables) {
		CRR_DATA_ACQ_DONE();
		fprintf(stderr, "Invalid number of sample variables. totalSampleVariables: %ld, nSampleVariables: %ld",
				totalSampleVariables, nSampleVariables);
		SPRINTF_ERROR_FREE_MEMORY_AND_RETURN_(UNABLE_TO_GET_DATA_EXCEPTION,
				"No sample variables have been specified in the given file: %s", buttonFileName, table)
	}

	// Query results variables
	if (!nSteps) {
		CRR_DATA_ACQ_DONE();
		SPRINTF_ERROR_FREE_MEMORY_AND_RETURN_(UNABLE_TO_GET_DATA_EXCEPTION,
				"Total number of steps derived from specified step variables is: %s", "0", table)
	}

	// Space for samples.  Organised as sets of values for all sample variables
	float* samples;
	long* samplesOk;

	printf("Tabulating Scan Results: %d x %ld\n", nSteps, totalSampleVariables);
	ALLOCATE_AND_TRACK_MEMORY_AND_ON_ERROR_RETURN_(env, samples, nSteps * totalSampleVariables * sizeof(float) * 2,
			"store acquired data", table)
	ALLOCATE_AND_TRACK_MEMORY_AND_ON_ERROR_RETURN_(env, samplesOk, nSteps * totalSampleVariables * sizeof(long),
			"store acquired data ok", table)

	// Clean up correlation plot context
	status = CRR_DATA_ACQ_DONE();
	if (!SUCCESS(status)) {
		SPRINTF_ERROR_STATUS_FREE_MEMORY_AND_RETURN_(status, UNABLE_TO_GET_DATA_EXCEPTION,
				"cleaning up after correlation plot: %s", buttonFileName, table)
	}

	// Allocate a dynamic table of with as many columns as samples and step variables
	table = tableCreateDynamic(env, nSteps, (int)totalSampleVariables * 3);
	ON_EXCEPTION_FREE_MEMORY_AND_RETURN_(table)

	// Create the correlation plot table
	createCorrelationPlotTable(env, &table, ((int)totalSampleVariables), nSteps,
			primaryStepVariable,
			secondaryStepVariableProvided, secondaryStepVariable,
			samples, samplesOk);

	// Free all allocated memory
	FREE_MEMORY

	return table;
}

/**
 * Count the number of sampled variables by excluding all those that are ZERO or SAMP
 * @param env
 * @param nSampleVariables
 */
static int countSampleVariables(JNIEnv* env, long* nSampleVariables) {
	REF_DECLARE;
	long allVars;
	*nSampleVariables = 0;
	vmsstat_t status = CRR_GET_NVAR(&allVars);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "unable to get number of vars");
		return EXIT_FAILURE;
	}

	// Loop over all variables discarding all that are ZERO
	for (long varNum = 1; varNum < allVars; varNum++) {
		long prim, micr, unit, secn, secn2, elem2;
		status = CRR_GET_VAR_NAME(REFINT4_1(DEFINE_SAMPLE), &varNum, &prim, &micr, &unit, &secn, &secn2, &elem2);
		if (!SUCCESS(status)) {
			aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "unable to get var name");
			return EXIT_FAILURE;
		}

		if (prim != ZERO_SV) {
			(*nSampleVariables)++;
		}
	}

	return EXIT_SUCCESS;
}

/**
 * Add the provided samples to the given table and create the fields, and labels as well
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param table given table
 * @param totalSampleVariables the total number of samples for which the scan was done
 * @param nSteps the number of steps over which the plot was executed
 * @param primaryStepVariable the name of the primary step variable
 * @param secondaryStepVariableProvided true if a secondary step variable was provided
 * @param secondaryStepVariable the secondary step variable
 * @param sampleDescriptors the sample descriptors in for of longs representing PRIM, MICRO, UNIT, SECN
 * @param samples array of sample values organised as vectors
 */
static void createCorrelationPlotTable(JNIEnv* env, Table* table, int totalSampleVariables, int nSteps,
		char* primaryStepVariable, bool secondaryStepVariableProvided, char* secondaryStepVariable,
		float* samples, long* samplesOk) {
	REF_DECLARE;
	char name[MAX_URI_LEN];
	char sampleFieldName[MAX_URI_LEN + 1], sampleLabelName[MAX_URI_LEN + 1];

	char* tableFieldName, * tableLabelName;
	int secondaryAdjust = (secondaryStepVariableProvided ? 1 : 0);
	vmsstat_t status;
	int samplesAdded = 0;
	for (long varNum = -secondaryAdjust; samplesAdded < totalSampleVariables; varNum++) {
		bool isTime = false, isATime = false, isSamp = false;
		long prim, micr, unit, secn, secn2, elem2;
		status = CRR_GET_VAR_NAME(REFINT4_1(DEFINE_SAMPLE), &varNum, &prim, &micr, &unit, &secn, &secn2, &elem2);
		isTime = prim == TIME_SV;
		isATime = prim == ATIME_SV;
		isSamp = prim == SAMP_SV;

		if (varNum == 0) { // Primary step variable
			tableFieldName = "primary";
			tableLabelName = primaryStepVariable;
		} else if (varNum == -1) {
			tableFieldName = "secondary";
			tableLabelName = secondaryStepVariable;
		} else {
			if (!SUCCESS(status)) {
				aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "unable to get var name");
				return;
			}

			if (prim == ZERO_SV) {
				continue;
			}

			sprintf(sampleFieldName, "sample%ld", varNum);
			tableFieldName = sampleFieldName;

			unsigned long rawName[AP_DESCSIZE];
			CRR_GET_VAR_DEF(REFINT4_1(DEFINE_SAMPLE), &varNum, (long*)&rawName[0]);
			char part[10];
//			struct dsc$descriptor desc = AP_DEV_TO_STR(&rawName[0]);
//			tableLabelName = desc.dsc$a_pointer;
//			sprintf(sampleLabelName, "sample%ld", varNum);
			sprintf(sampleLabelName, "%.4s", (char*)&rawName[0]);
			sprintf(part, "%.4s", (char*)&rawName[1]);
			if (part[0] != '-') {
				sprintf(sampleLabelName + strlen(sampleLabelName), ":%.4s", part);
				sprintf(part, "%.4s", (char*)&rawName[2]);
				if (part[0] != '-') {
					sprintf(sampleLabelName + strlen(sampleLabelName), ":%.4s", part);
					sprintf(part, "%.4s", (char*)&rawName[3]);
					if (part[0] != '-') {
						sprintf(sampleLabelName + strlen(sampleLabelName), ":%.4s", part);
						sprintf(part, "%.4s", (char*)&rawName[4]);
						if (part[0] != '-') {
							sprintf(sampleLabelName + strlen(sampleLabelName), " %.4s", part);
						}
					}
				}
			}
			tableLabelName = sampleLabelName;
		}

		// Get sample var data
		long dataOffset = table->_currentColumn * nSteps;
		long errorOffset = dataOffset + nSteps;
		if (isTime || isATime || isSamp) {
			status = CRR_GET_VAR(&varNum, &samples[dataOffset], NULL, NULL, NULL);
		} else {
			status = CRR_GET_VAR(&varNum, &samples[dataOffset], &samples[errorOffset], &samplesOk[dataOffset], NULL);
		}
		if (!SUCCESS(status)) {
			aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "unable to get sample data");
			return;
		}

		// Data column
		if (isTime || isATime) {
			tableAddColumn(env, table, AIDA_LONG_TYPE, &samples[dataOffset], false);
		} else if (isSamp) {
			tableAddColumn(env, table, AIDA_LONG_TYPE, &samples[dataOffset], false);
		} else {
			tableAddColumn(env, table, AIDA_FLOAT_TYPE, &samples[dataOffset], true);
		}
		ON_EXCEPTION_RETURN_VOID
		tableAddField(env, table, tableFieldName);
		ON_EXCEPTION_RETURN_VOID
		tableAddLabel(env, table, tableLabelName);
		ON_EXCEPTION_RETURN_VOID

		if (isTime || isATime || isSamp) {
			table->columnCount -= 2;
		} else {
			// Error column
			tableAddColumn(env, table, AIDA_FLOAT_TYPE, &samples[errorOffset], true);
			ON_EXCEPTION_RETURN_VOID
			sprintf(name, "%sError", tableFieldName);
			tableAddField(env, table, name);
			ON_EXCEPTION_RETURN_VOID
			sprintf(name, "%s Error", tableLabelName);
			tableAddLabel(env, table, name);
			ON_EXCEPTION_RETURN_VOID

			// Data OK column
			tableAddColumn(env, table, AIDA_LONG_TYPE, &samplesOk[dataOffset], false);
			ON_EXCEPTION_RETURN_VOID
			sprintf(name, "%sOk", tableFieldName);
			tableAddField(env, table, name);
			ON_EXCEPTION_RETURN_VOID
			sprintf(name, "%s Ok?", tableLabelName);
			tableAddLabel(env, table, name);
			ON_EXCEPTION_RETURN_VOID
		}

		samplesAdded++;
	}
}

/**
 * Get the user parameters required for the correlation plot functionality
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param arguments the provided user arguments
 * @param buttonFileName pointer to store pointer to allocated button file name
 * @param primaryStepVariable pointer to store the allocated step variable name
 * @param primaryLow pointer to store primary step variable starting point
 * @param primaryHigh pointer to store primary step variable end point
 * @param primarySteps pointer to store primary number of steps
 * @param primarySettleTime pointer to store optional primary settle time
 * @param secondaryStepVariable pointer to store optional secondary step variable name
 * @param secondaryLow pointer to store optional secondary step variable starting point
 * @param secondaryHigh pointer to store optional secondary step variable end point
 * @param secondarySteps pointer to store optional secondary number of steps
 * @param secondarySettleTime pointer to store optional secondary step variable settle time
 * @return true if secondary step variable has been set
 */
static bool getAcqParameters(JNIEnv* env, Arguments arguments, char** buttonFileName,
		char** primaryStepVariable, float* primaryLow, float* primaryHigh, int* primarySteps, float* primarySettleTime,
		float* primaryExtraSettleTime,
		char** secondaryStepVariable, float* secondaryLow, float* secondaryHigh, int* secondarySteps,
		float* secondarySettleTime, float* secondaryExtraSettleTime, short* bpmd, long* nrpos, long* dtizavg,
		char** magnetFunction) {
	*primarySettleTime = 0.0f;

	*secondaryStepVariable = NULL;
	*secondaryLow = FLT_MAX;
	*secondaryHigh = FLT_MAX;
	*primaryExtraSettleTime = 0.0f;
	*secondaryExtraSettleTime = 0.0f;
	*secondarySteps = -1;
	*secondarySettleTime = 0.0f;

	if (ascanf(env, &arguments, "%s  %s %f %f %d %of %of  %os %of %of %od %of %of  %ohd %old %old %os",
			"file", buttonFileName,

			"primstv.name", primaryStepVariable,
			"primstv.low", primaryLow,
			"primstv.high", primaryHigh,
			"primstv.steps", primarySteps,
			"primstv.settle", primarySettleTime,
			"primstv.extrasettle", primaryExtraSettleTime,

			"secnstv.name", secondaryStepVariable,
			"secnstv.low", secondaryLow,
			"secnstv.high", secondaryHigh,
			"secnstv.steps", secondarySteps,
			"secnstv.settle", secondarySettleTime,
			"secnstv.extrasettle", secondaryExtraSettleTime,

			"bpmd", bpmd,
			"nrpos", nrpos,
			"dtizavg", dtizavg,
			"magfunc", magnetFunction

	)) {
		return false;
	}
	bool allSecSet =
			*secondaryStepVariable && *secondaryLow != FLT_MAX && *secondaryHigh != FLT_MAX && *secondarySteps != -1;
	bool noSecSet =
			!*secondaryStepVariable && *secondaryLow == FLT_MAX && *secondaryHigh == FLT_MAX && *secondarySteps == -1;

	// All fields must be specified for the secondary or none of them
	if (!allSecSet && !noSecSet) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"You have to specify name, low, high and step for the secondary step variable.  You missed at least one");
		return false;
	}

	if (*primarySteps < 2 || (allSecSet && *secondarySteps < 2)) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Steps must be at least 2");
	}
	return allSecSet;
}

/**
 * Set a value: BGRP Set Variable or SCP transverse feedback
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @param value to set
 */
void aidaSetValue(JNIEnv* env, const char* uri, Arguments arguments, Value value) {
	if (startsWith(uri, "FBCK")) {
		setFeedback(env, uri, arguments, value);
	} else {
		setBGrp(env, arguments, value);
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
Table aidaSetValueWithResponse(JNIEnv* env, const char* uri, Arguments arguments, Value value) {
	if (strcasecmp(uri, "MKB:VAL") == 0) {
		// set Multi Knob value
		return setMkbValue(env, uri, arguments, value);
	} else {
		// set TRIGGER value
		return setTriggerValue(env, uri, arguments, value);
	}
}

/**
 * Set a value: BGRP Set Variable
 *  Parameters:
 *    BGRP    	: The BGRP name
 *    VARNAME   : BGRP variable name for the specified BGRP
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri
 * @param arguments the arguments
 * @param value to set
 */
static void setBGrp(JNIEnv* env, Arguments arguments, Value value) {
	TRACK_ALLOCATED_MEMORY

	vmsstat_t status;
	char* bgrp, * varname, * valueString;

	// Check if operations are enabled?
	if (!DPSLCUTIL_BGRP_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Aida access to BGRP set variable operations is not currently enabled");
		return;
	}

	if (avscanf(env, &arguments, &value, "%s %s %s",
			"value", &valueString,
			"bgrp", &bgrp,
			"varname", &varname
	)) {
		return;
	}

	TRACK_MEMORY(bgrp)
	TRACK_MEMORY(varname)
	TRACK_MEMORY(valueString)

	if (strcasecmp(valueString, "Y") != 0 && strcasecmp(valueString, "N") != 0) {
		FREE_MEMORY
		aidaThrowNonOsException(env, MISSING_REQUIRED_ARGUMENT_EXCEPTION,
				"'BGRP Set Variable requires Value argument to be 'Y' or 'N'");
		return;
	}

	// Set the value
	status = DPSLCUTIL_BGRP_SETVAR(bgrp, varname, valueString);
	FREE_MEMORY
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "Failed to set data");
	}
}

/**
 * Set Multi-knob value
 *
 * @param env
 * @param uri
 * @param arguments
 * @param value
 * @return
 */
static Table setMkbValue(JNIEnv* env, const char* uri, Arguments arguments, Value value) {
	TRACK_ALLOCATED_MEMORY

	// Check if operations are enabled?
	if (!DPSLCUTIL_MKB_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Aida multiknob operations are not currently enabled");
		RETURN_NULL_TABLE;
	}

	// Get arguments
	char* mkb;
	float floatValue;
	if (avscanf(env, &arguments, &value, "%f %s",
			"value", &floatValue,
			"mkb", &mkb
	)) {
		RETURN_NULL_TABLE;
	}
	TRACK_MEMORY(mkb)
	CONVERT_TO_VMS_FLOAT(&floatValue, 1)

	// Set the value
	int num_devices;
	vmsstat_t status;
	status = DPSLCUTIL_DO_MKB(mkb, &floatValue, &num_devices);
	FREE_MEMORY
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "unable to set value");
		DPSLCUTIL_MKB_GETCLEANUP();
		RETURN_NULL_TABLE;
	}

	if (DPSLCUTIL_MKB_GETABSFLAG()) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Specified multiknob file is absolute, which is not permitted");
		DPSLCUTIL_MKB_GETCLEANUP();
		RETURN_NULL_TABLE;
	}

	// Now get the new values to return

	// To hold the data
	char namesData[(num_devices * MAX_DEVICE_STRING_LEN)];
	float secondaryValuesData[num_devices];

	// Names
	DPSLCUTIL_MKB_GETNAMES(namesData);

	// Secondary values
	DPSLCUTIL_MKB_GETSECNVALUES(secondaryValuesData);

	// cleanup
	DPSLCUTIL_MKB_GETCLEANUP();

	// Now create table to return
	Table table = tableCreate(env, num_devices, 2);
	ON_EXCEPTION_RETURN_(table)
	tableAddFixedWidthStringColumn(env, &table, namesData, MAX_DEVICE_STRING_LEN);
	ON_EXCEPTION_RETURN_(table)
	tableAddColumn(env, &table, AIDA_FLOAT_TYPE, secondaryValuesData, false);

	return table;
}

/**
 * Set Trigger value
 *
 * @param env
 * @param uri
 * @param arguments
 * @param value
 * @return
 */
static Table setTriggerValue(JNIEnv* env, const char* uri, Arguments arguments, Value value) {
	// Check if operations are enabled?
	if (!DPSLCUTIL_TRIG_ACCESSENABLED()) {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Aida access to trigger operations is not currently enabled");
		RETURN_NULL_TABLE;
	}

	// Get arguments
	int beam;
	short flag;
	if (avscanf(env, &arguments, &value, "%hd %d",
			"value", &flag,
			"beam", &beam
	)) {
		RETURN_NULL_TABLE;
	}

	// Set the trigger value
	vmsstat_t status;
	TO_DGROUP(dGroupName, uri)
	status = DPSLCUTIL_TRIG_SETDEACTORREACT(dGroupName, flag, beam);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "unable to set value");
		RETURN_NULL_TABLE;
	}

	// Read back status
	status = DPSLCUTIL_TRIG_GETSTATUS(dGroupName, beam, &flag);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "unable to read-back value");
		RETURN_NULL_TABLE;
	}

	// Now create table to return the flag
	Table table = tableCreate(env, 1, 1);
	ON_EXCEPTION_RETURN_(table)
	tableAddSingleRowShortColumn(env, &table, flag);

	return table;
}

/**
 * Read the trig status and return the value
 *
 * @param env to report errors
 * @param uri the uri
 * @param arguments the arguments
 * @return the beam status
 */
static short getTrigStatus(JNIEnv* env, const char* uri, Arguments arguments) {
	// Get the arguments
	int beam;

	if (ascanf(env, &arguments, "%d", "beam", &beam)) {
		return -1;
	}

	// Read the status
	vmsstat_t status;
	short trig_status;
	TO_DGROUP(dGroupName, uri)
	status = DPSLCUTIL_TRIG_GETSTATUS((char*)dGroupName, beam, &trig_status);
	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "Unable to get beam status");
	}
	return trig_status;
}

/**
 * Get feedback status as an integer
 * @param env to report errors
 * @param uri the uri
 * @return 0 = off, 1 = compute, 2 = feedback
 */
static short getShortFaStatus(JNIEnv* env, const char* uri) {
	int4u haStatus = faStatus(env, uri, 0, TRUE_B);
	ON_EXCEPTION_RETURN_(0);
	switch (haStatus) {
	case FBCK_FEEDBACK_STATE:
		return 2;
	case FBCK_COMPUTE_STATE:
		return 1;
	case FBCK_OFF_STATE:
	case FBCK_SAMPLE_STATE:
	case FBCK_ACTUATE_STATE:
	case FBCK_DITHER_STATE:
	default:
		return 0;
	}
}

/**
 * Get the feedback status as a string
 *
 * @param env to report errors
 * @param uri the uri
 * @return sample, compute, actuate, dither, feedback or off (default)
 */
static char* GetStringFaStatus(JNIEnv* env, const char* uri) {
	int4u haStatus = faStatus(env, uri, 0, TRUE_B);
	ON_EXCEPTION_RETURN_(0);
	switch (haStatus) {
	case FBCK_SAMPLE_STATE:
		return ALLOCATE_STRING(env, "sample", "string");
	case FBCK_COMPUTE_STATE:
		return ALLOCATE_STRING(env, "compute", "string");
	case FBCK_ACTUATE_STATE:
		return ALLOCATE_STRING(env, "actuate", "string");
	case FBCK_DITHER_STATE:
		return ALLOCATE_STRING(env, "dither", "string");
	case FBCK_FEEDBACK_STATE:
		return ALLOCATE_STRING(env, "feedback", "string");
	case FBCK_OFF_STATE:
	default:
		return ALLOCATE_STRING(env, "off", "string");
	}
}

/**
 * Set SCP transverse feedback and return the prior setting
 *
 * @param env to report errors
 * @param uri the uri
 * @param newHSTA new value
 * @param retrieveOnly don't set only retrieve prior setting if true
 */
static int4u faStatus(JNIEnv* env, const char* uri, int newHSTA, BOOLEAN retrieveOnly) {
	// Get SCP transverse feedback name from uri
	DESCR_DECLARE;
	PMU_STRING_FROM_URI(pmu_str, uri)
	vmsstat_t status;
	fbckhsta_tu lastHsta;

	// Change secondary to HSTA
	printf("SLC NAME: %s", pmu_str);

	// Change the Feedback state or throw an exception on failure
	status = FA_CHANGE_HSTA(DESCRA1(pmu_str), newHSTA, retrieveOnly, &lastHsta.bit_s);
	if (!SUCCESS(status)) {
		if (retrieveOnly) {
			aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "unable to get feedback state");
		} else {
			aidaThrow(env, status, UNABLE_TO_SET_DATA_EXCEPTION, "unable to set feedback state");
		}
		return 0;
	}
	return lastHsta.lword;
}

/**
 * Sets the specified SCP transverse feedback to off, sample, compute, on, actuate, dither, or feedback
 *
 * @param env to be used to throw exceptions using aidaThrow() and aidaThrowNonOsException()
 * @param uri the uri containing the specified SCP transverse feedback to set
 * @param arguments the arguments containing the value parameter
 * @param value to set: `off`, `sample`, `compute`, `on`, `actuate`, `dither`, `feedback`, `true`, `false`, `0`, `1`, or `2`
 */
static void setFeedback(JNIEnv* env, const char* uri, Arguments arguments, Value value) {
	char* valueString;
	int newHSTA;

	// Get value string
	if (avscanf(env, &arguments, &value, "%s", "value", &valueString)) {
		return;
	}

	// Convert value string to new state
	if (strcasecmp(valueString, "OFF") == 0 || strcasecmp(valueString, "0") == 0
			|| strcasecmp(valueString, "false") == 0) {
		newHSTA = FBCK_OFF_STATE;
	} else if (strcasecmp(valueString, "SAMPLE") == 0) {
		newHSTA = FBCK_SAMPLE_STATE;
	} else if (strcasecmp(valueString, "ON") == 0 || strcasecmp(valueString, "1") == 0
			|| strcasecmp(valueString, "COMPUTE") == 0) {
		newHSTA = FBCK_COMPUTE_STATE;
	} else if (strcasecmp(valueString, "ACTUATE") == 0) {
		newHSTA = FBCK_ACTUATE_STATE;
	} else if (strcasecmp(valueString, "DITHER") == 0) {
		newHSTA = FBCK_DITHER_STATE;
	} else if (strcasecmp(valueString, "FEEDBACK") == 0 || strcasecmp(valueString, "2") == 0
			|| strcasecmp(valueString, "true") == 0) {
		newHSTA = FBCK_FEEDBACK_STATE;
	} else {
		aidaThrowNonOsException(env, UNABLE_TO_SET_DATA_EXCEPTION,
				"Unsupported value for feedback control: Must be OFF, SAMPLE, COMPUTE, ON, ACTUATE, DITHER, or FEEDBACK");
		return;
	}
	faStatus(env, uri, newHSTA, FALSE_B);
}

