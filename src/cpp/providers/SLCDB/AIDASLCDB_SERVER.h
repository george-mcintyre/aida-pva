#ifndef AIDASLCDB_SERVER_H
#define AIDASLCDB_SERVER_H
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Routines in dpslc_jni_helper.c to call into SLC shareables
 *
 *  **CMS**=C_INC
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
extern vmsstat_t JNI_DBSETINT(const char*, int*, int length);

#define TEXT_SUBSTRING 8
#define COLOR_SUBSTRING 8
#define FLAG_SUBSTRING 1
#define TOTAL_PSEUDO_SECONDARY_LEN (TEXT_SUBSTRING+COLOR_SUBSTRING+FLAG_SUBSTRING + 2)

#ifdef __cplusplus
}
#endif
#endif
