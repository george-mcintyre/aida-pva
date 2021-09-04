#ifndef _Included_slc_server
#define _Included_slc_server
#ifdef __cplusplus
extern "C" {
#endif

#include "aida_types.h"

#define DB_GET_C \
	status = DBgetC(((void*)(&dbDataReturnBuffer)), prim_i, micr_i, unit_i, secn_i, NULL);

#define DB_GET_ASTS_C \
	status = DBget_astsC((void*)&dbDataReturnBuffer, micr_i, unit_i, secn_i, astsChannelName);

#define DB_GET_LIST \
	status = DBlist_astsC(((void*)(&dbListDataBuffer)), micr_i, unit_i, secn_i, astsChannelName);

#define DB_LIST_C \
	status = DBlistC(((void*)(&dbListDataBuffer)), prim_i, micr_i, unit_i, secn_i, NULL);

#define DB_GET_LIST_C \
	status = DBlgetC(((void*)(&dbDataReturnBuffer)), ((void*)(&dbListDataBuffer)));

#define DB_GET_LIST_ASTS_C \
	status = DBlget_astsC(((void*)(&dbDataReturnBuffer)), ((void*)(&dbListDataBuffer)));

#define DB_GET_LIST_ASTS_COLOR \
	status = asts_color(&micr_i, &unit_i, astsChannelName, DESCRA1(color), DESCRA2(flag), DESCRA3(text), NULL);

#define DB_DEFINE_VARS_AND_PARSE_NAME(type) \
	vmsstat_t status; \
	DBLIST(dbDataReturnBuffer,type); \
	int4u prim_i = 0x20202020, micr_i = 0x20202020, unit_i = 0, secn_i = 0x20202020; \
	char astsChannelName[9]; \
	\
	DBCLEAR(dbDataReturnBuffer); \
	status = LCL_JNI_PARSENAME(uri, &prim_i, &micr_i, &unit_i, &secn_i, astsChannelName);

#define DB_LIST_DEFINE_VARS_AND_PARSE_NAME(type) \
	DBLIST(dbListDataBuffer, int4u); \
	DBCLEAR(dbListDataBuffer); \
	DB_DEFINE_VARS_AND_PARSE_NAME(type)


/**
 * Define DB_GET_SCALAR to get scalars from the database
 * @param type the type of the scalar to be returned
 * @param message the error message to display when things go wrong
 */
#define DB_GET_SCALAR(type, message) \
{ \
	DB_DEFINE_VARS_AND_PARSE_NAME(type) \
	\
	if (SUCCESS(status)) { \
		if (strcmp(astsChannelName, "        ") != 0) { \
			DB_GET_ASTS_C \
		} else { \
			DB_GET_C \
		} \
		\
		if (SUCCESS(status)) { \
			item = dbDataReturnBuffer->dat[0]; \
		} \
	} \
	\
	DBFREE(dbDataReturnBuffer); \
	if (!SUCCESS(status)) { \
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, message); \
	} \
}

/**
 * Define DB_GET_ARRAY to get arrays from the database
 * @param type the type of the scalar returned in the array
 * @param message the error message to display when things go wrong
 */
#define DB_GET_ARRAYS(type, message) \
{ \
	itemArray.count = 0; \
	int itemSize = sizeof(type); \
	DB_LIST_DEFINE_VARS_AND_PARSE_NAME(type) \
	\
	if (SUCCESS(status)) { \
		if (strcmp(astsChannelName, "        ") != 0) { \
			DB_GET_LIST \
			if (SUCCESS(status)) { \
				DB_GET_LIST_ASTS_C \
			} \
 		} else { \
			DB_LIST_C \
			if (SUCCESS(status)) { \
				DB_GET_LIST_C \
			} \
		} \
	 \
		if (SUCCESS(status)) { \
			itemArray.count = DBCNT(dbDataReturnBuffer); \
			itemArray.items = calloc(itemArray.count+1, itemSize); \
			memcpy (itemArray.items, &dbDataReturnBuffer->dat[0], itemArray.count * itemSize + 1); \
			((type *)itemArray.items)[itemArray.count] = '\0';        /* Null terminate */ \
		} \
	} \
	\
	DBFREE(dbListDataBuffer); \
	DBFREE(dbDataReturnBuffer); \
	\
	if (!SUCCESS(status)) { \
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, message); \
	} \
}

/**
 * Initialise the aida service.  Called once by the framework when starting up.
 * @param env to be used to throw exceptions using throw()
 */
void aidaServiceInit(JNIEnv* env);

/**
 * Get channel configuration
 * @param channelName
 * @param env to be used to throw exceptions using throw()
 * @return the channel config
 */
Config aidaChannelConfig(JNIEnv* env, const char* channelName);

/**
 * Get a table of data
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the table
 */
Table aidaRequestTable(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a boolean
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean
 */
int aidaRequestBoolean(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a byte
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the byte
 */
char aidaRequestByte(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a short
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the short
 */
short aidaRequestShort(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a integer
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the integer
 */
int aidaRequestInteger(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a long
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the long
 */
long aidaRequestLong(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a float
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the float
 */
float aidaRequestFloat(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a double
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the double
 */
double aidaRequestDouble(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a string
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the string
 */
char* aidaRequestString(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a boolean array
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the boolean array
 */
Array aidaRequestBooleanArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a byte array
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the byte array
 */
Array aidaRequestByteArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a short array
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the short array
 */
Array aidaRequestShortArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a integer array
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the integer array
 */
Array aidaRequestIntegerArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a long array
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the long array
 */
Array aidaRequestLongArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a float array
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the float array
 */
Array aidaRequestFloatArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a double array
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the double array
 */
Array aidaRequestDoubleArray(JNIEnv* env, const char* uri, Arguments arguments);

/**
 * Get a string array
 *
 * @param env to be used to throw exceptions using throw()
 * @param uri the uri
 * @param arguments the arguments
 * @return the string array
 */
StringArray aidaRequestStringArray(JNIEnv* env, const char* uri, Arguments arguments);

#ifdef __cplusplus
}
#endif
#endif
