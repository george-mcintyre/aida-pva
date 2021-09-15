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

#ifdef __cplusplus
}
#endif
#endif
