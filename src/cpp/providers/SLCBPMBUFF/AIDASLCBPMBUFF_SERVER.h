#ifndef AIDASLCBPMBUFF_SERVER_H
#define AIDASLCBPMBUFF_SERVER_H
#ifdef __cplusplus
extern "C" {
#endif
/*
         **CMS**=C_INC
 */
#include "slc_types.h"            /* prev of dpslcbpm.h */

#include "bpmparamv.h"            /* prev of dpslcbuffacq.h */
#include "bpmparam.hc"            /* prev of bpmstruc.hc */
#include "bpmstruc.hc"            /* bpms_data_ts, prev of dpslcbuffacq.h */
#include "bpm_buff_common.h"      /* MAX_BUFF_MEAS */
#include "dpslcbuff_jni.h"        /* javah generated JNI protos */
#include "dpslcbpm.h"             /* BPM service include file, NAME_SIZE */
#include "dpslcbuff.h"            /* Suite include file for dpslcbuff */

#define NRPOS_DEFAULT  1  		// Get 1 pulse by default
#define BPMD_ROGUE 0     		// 0 is invalid, used as rogue value
#define NRPOS_MIN  1
#define NRPOS_MAX 2800
#define BPMD_MIN  1
#define BPMD_MAX 9999
#define NUMOFFSET 3

#ifdef __cplusplus
}
#endif
#endif
