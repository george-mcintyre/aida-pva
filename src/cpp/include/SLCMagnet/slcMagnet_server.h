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

#define MAX_PRIM_NAME_LEN 4
#define MAX_MICR_NAME_LEN 4
#define MAX_SECN_NAME_LEN 4
#define MAX_PMU_STRING_LEN 15
#define MAX_STATE_NAME_LEN 9
#define MAX_VALIDITY_STRING_LEN 40

extern vmsstat_t DPSLCMAGNET_DB_INIT(void);
extern vmsstat_t DPSLCMAGNET_GET(char*, char*, char*, int*);
extern int DPSLCMAGNET_GETNUMPVS(void);
extern void DPSLCMAGNET_GETNAMES(char*);
extern void DPSLCMAGNET_GETSECNVALUES(float*);
extern void DPSLCMAGNET_GETCLEANUP(void);
extern void DPSLCMAGNET_SETNAMESVALIDATE(int, char*, char*, int*, int4u, char*);
extern void DPSLCMAGNET_RET_MAGLIMITS(int, float*);
extern int DPSLCMAGNET_ACCESSENABLED(void);
extern vmsstat_t DPSLCMAGNET_SETCONFIG(int, char*, char*, int*, int4u, float*);
extern vmsstat_t DPSLCMAGNET_SET(int, char*, char*, int*, int4u, float*, char*);
extern int DPSLCMAGNET_RET_SETNUMPVS(void);
extern void DPSLCMAGNET_RET_STATENAMES(char*);
extern void DPSLCMAGNET_RET_BACTVALUES(float*);
extern void DPSLCMAGNET_SETCLEANUP(void);
extern void JNI_ERR_MAGNETTRANSLATE(const unsigned long int*, struct dsc$descriptor*);

#ifdef __cplusplus
}
#endif
#endif
