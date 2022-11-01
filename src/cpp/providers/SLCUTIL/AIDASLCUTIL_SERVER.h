#ifndef AIDASLCUTIL_SERVER_H
#define AIDASLCUTIL_SERVER_H
#ifdef __cplusplus
extern "C" {
#endif
/*
 *
 *  **CMS**=C_INC
 *
 */
#include "fbck_common.hc"
#include "fbckstat.hc"
#include "descr.h"                /* descriptor macros */

#define MAX_DEVICE_STRING_LEN 20

extern int DPSLCUTIL_MKB_ACCESSENABLED(void);
extern vmsstat_t DPSLCUTIL_DO_MKB(char*, float*, int*);
extern int DPSLCUTIL_MKB_GETABSFLAG(void);
extern void DPSLCUTIL_MKB_GETNAMES(char*);
extern void DPSLCUTIL_MKB_GETSECNVALUES(float*);
extern void DPSLCUTIL_MKB_GETCLEANUP(void);
extern int DPSLCUTIL_TRIG_GETSTATUS(char*, int, short*);
extern int DPSLCUTIL_TRIG_ACCESSENABLED(void);
extern int DPSLCUTIL_TRIG_SETDEACTORREACT(char*, int, int);
extern int DPSLCUTIL_BGRP_ACCESSENABLED(void);
extern int DPSLCUTIL_BGRP_SETVAR(char*, char*, char*);
extern int FA_CHANGE_HSTA(fstring_ts *, int, BOOLEAN , fbckhsta_ts2 *);
extern int CRR_RESTORE_BUTTON_FILE(struct dsc$descriptor*);
extern int CRR_DATA_ACQ(struct dsc$descriptor*);
extern int CRR_RESET_ALL();
extern int CRR_GET_NVAR(long*);
extern int CRR_GET_VAR_NAME(long*, long*, long*, long*, long*, long*, long*, long*);
extern int CRR_GET_VAR_DEF(long*, long*, long*);
extern struct dsc$descriptor AP_DEV_TO_STR(long*);
extern int CRR_SAMPLES();
extern int CRR_GET_VAR(long *, float*, float*, long*, long*);
extern int CRR_DATA_ACQ_INIT();
extern int CRR_SET_SAMP_PARMS(long *, long*, float *, long *, long*, long*, float*, float*);
extern int CRR_DATA_ACQ_DONE();
extern int CRR_SET_VAR_NAME_FREEFORM(long*,long*,struct dsc$descriptor*,long*);
extern int CRR_SET_STEP(long*, float*, float*, int*, float*);
extern unsigned long int BPMD_SELECT_UTIL(short int  *,long int  *,long int  *, long int  *, short int  *, short int  *);

#define SLC_BUTTON_DIR        "SLC_CRR_BUTTON:"

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
