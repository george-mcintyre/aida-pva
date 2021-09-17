#ifndef _Included_slcUtil_server
#define _Included_slcUtil_server
#ifdef __cplusplus
extern "C" {
#endif

#include "aida_types.h"

#define MAX_DEVICE_STRING_LEN 20

#define MEMORY_ALLOCATE_FAIL     2
#define NO_BGRP_NAMES            4
#define BGRP_NAME_NOT_FOUND      6
#define NO_BGRP_VARIABLES        8
#define BGRP_VARIABLE_NOT_FOUND 10

#define MEMORY_ALLOCATE_FAIL_MSG      "Error while allocating memory"
#define NO_BGRP_NAMES_MSG             "No BGRP names found"
#define BGRP_NAME_NOT_FOUND_MSG       "Specified BGRP name not valid"
#define NO_BGRP_VARIABLES_MSG         "No variables found for BGRP name"
#define BGRP_VARIABLE_NOT_FOUND_MSG   "Specified variable not valid for BGRP"

extern vmsstat_t DPSLCUTIL_DB_INIT(void);
extern int DPSLCUTIL_MKB_ACCESSENABLED(void);
extern vmsstat_t DPSLCUTIL_DO_MKB(char*, float*, int*);
extern int DPSLCUTIL_MKB_GETABSFLAG(void);
extern int DPSLCUTIL_MKB_GETNUMDEVICES(void);
extern void DPSLCUTIL_MKB_GETNAMES(char*);
extern void DPSLCUTIL_MKB_GETSECNVALUES(float*);
extern void DPSLCUTIL_MKB_GETCLEANUP(void);
extern int DPSLCUTIL_TRIG_GETSTATUS(char*, int, short*);
extern int DPSLCUTIL_TRIG_ACCESSENABLED(void);
extern int DPSLCUTIL_TRIG_SETDEACTORREACT(char*, int, int);
extern int DPSLCUTIL_BGRP_ACCESSENABLED(void);
extern int DPSLCUTIL_BGRP_SETVAR(char*, char*, char*);

#ifdef __cplusplus
}
#endif
#endif
