#ifndef _Included_slcKlys_server
#define _Included_slcKlys_server
#ifdef __cplusplus
extern "C" {
#endif

#include "aida_types.h"

extern vmsstat_t DPSLCKLYS_DB_INIT(void);
extern int DPSLCKLYS_DBGETKLYSFLOAT(char*, float*);
extern int DPSLCKLYS_GETSTATUS(char*, char*, char*, short*);
extern int DPSLCKLYS_ACCESSENABLED(void);
extern int DPSLCKLYS_SETDEACTORREACT(char*, int, char*);
extern int DPSLCKLYS_SETCONFIG(char*, float*, char*);
extern int DPSLCKLYS_SETTRIMPHASE(char*, char*, float*, char*, float*);
extern void JNI_ERR_KLYSTRANSLATE(const unsigned long int*, struct dsc$descriptor*);

#ifdef __cplusplus
}
#endif
#endif
