#ifndef _Included_slc_server
#define _Included_slc_server
#ifdef __cplusplus
extern "C" {
#endif

#include "aida_types.h"

/* Override prototypes of externals to uppercase names since
compile.com adds cc/name=UPPERCASE on compiles by default.
However, if the ATTRIBUTE=JNI is in effect (as it is for
this module), then it is /names=AS_IS.
-------------------------------------------------------------*/
void CVT_VMS_TO_IEEE_FLT(void* sorc_a, float dest_a[], unsigned short* nlong_p);
void CVT_IEEE_TO_VMS_FLT(void* sorc_a, float dest_a[], unsigned short* nlong_p);

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
