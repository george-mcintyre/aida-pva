#ifndef _Included_slcBpm_server
#define _Included_slcBpm_server
#ifdef __cplusplus
extern "C" {
#endif

#include "aida_types.h"

// Use this ID number temporarily until we can get a permanent ID assigned
#define AIDAPVA_SLCBPMID 192

#define SORTORDER_Z 1
#define SORTORDER_DISPLAY 2

#define CHECK_VMS_STATUS \
if (!$VMS_STATUS_SUCCESS(status)) { \
    endAcquireBpmData(env); \
	aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "reading BPM values"); \
	return 0; \
} \

#ifdef __cplusplus
}
#endif
#endif
