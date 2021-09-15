#ifndef _Included_slc_server
#define _Included_slc_server
#ifdef __cplusplus
extern "C" {
#endif

#include "aida_types.h"

/*
** Routines in dpslc_jni_helper.c to call into SLC shareables
*/
extern vmsstat_t DB_INIT(void);
extern vmsstat_t JNI_DBGETBYTE(const char*, unsigned char*);
extern vmsstat_t JNI_DBGETSHORT(const char*, short int*);
extern vmsstat_t JNI_DBGETINT(const char*, int*);
extern vmsstat_t JNI_DBGETFLOAT(const char*, float*);
extern vmsstat_t JNI_DBGETCHAR(const char*, char*);
extern vmsstat_t JNI_DBGETSTRING(const char*, char**);

extern vmsstat_t JNI_DBGETBYTEA(const char*, unsigned char**);
extern vmsstat_t JNI_DBGETSHORTA(const char*, short int**);
extern vmsstat_t JNI_DBGETINTA(const char*, int**);
extern vmsstat_t JNI_DBGETFLOATA(const char*, float**);

extern int JNI_DBACCESSENABLED(void);

extern vmsstat_t JNI_DBSETFLOAT(const char*, float*, int length);

extern void JNI_ERR_TRANSLATE(const unsigned long int*, struct dsc$descriptor*);

#ifdef __cplusplus
}
#endif
#endif
