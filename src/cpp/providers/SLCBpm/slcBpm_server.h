#ifndef _Included_slcBpm_server
#define _Included_slcBpm_server
#ifdef __cplusplus
extern "C" {
#endif
/*
 *
 *  **CMS**=C_INC
 *
 */

#include "slc_types.h"            /* prev of dpslcbpm.h */

#include "bpmparamv.h"            /* prev of dpslcbpm.h */
#include "bpmparam.hc"            /* prev of bpmstruc.hc */
#include "bpmstruc.hc"            /* bpms_data_ts, prev of dpslcbpm.h */
#include "dpslcbpm.h"             /* Suite include file for dpslcbpm */

// Use this ID number temporarily until we can get a permanent ID assigned
#define AIDAPVA_SLCBPMID 192

#define NAVG  1
#define BPMD_ROGUE 0            // 0 is invalid, used as rogue value
#define SORTORDER_Z  1
#define SORTORDER_DISPLAY  2
#define CNFTYPE_NONE  0
#define CNFTYPE_GOLD  1
#define CNFTYPE_LOADED  2
#define CNFTYPE_SCRATCH  3
#define CNFTYPE_NORMAL  4
#define CNFTYPE_TEMPORARY  5

#define CHECK_VMS_STATUS \
if (!$VMS_STATUS_SUCCESS(status)) { \
    endAcquireBpmData(env); \
    aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "reading BPM values"); \
    return EXIT_FAILURE; \
} \

#ifdef __cplusplus
}
#endif
#endif
