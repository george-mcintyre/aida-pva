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
void CVT_VMS_TO_IEEE_DBL(void* sorc_a, double dest_a[], unsigned short* nlong_p);
void CVT_IEEE_TO_VMS_FLT(void* sorc_a, float dest_a[], unsigned short* nlong_p);

extern vmsstat_t DPSLCMOSC_DB_INIT(void);
extern vmsstat_t DPSLCMOSC_MEASMASTEROSC(double*);
extern int DPSLCMOSC_ACCESSENABLED(void);
extern vmsstat_t DPSLCMOSC_SETMASTEROSC(float*, char*, char*, double*);
extern void JNI_ERR_MOSCTRANSLATE(const unsigned long int*, struct dsc$descriptor*);

#ifdef __cplusplus
}
#endif
#endif
