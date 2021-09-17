#ifndef _Included_slcBuffAcq_server
#define _Included_slcBuffAcq_server
#ifdef __cplusplus
extern "C" {
#endif

#include "aida_types.h"

#define MAXPTS (MAX_DGRP_BPMS*MAX_BUFF_MEAS)

extern vmsstat_t DPSLCBUFF_INIT(void);

#ifdef __cplusplus
}
#endif
#endif
