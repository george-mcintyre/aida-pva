#ifndef AIDASLCKLYS_SERVER_H
#define AIDASLCKLYS_SERVER_H
#ifdef __cplusplus
extern "C" {
#endif

/*
 *
 *  **CMS**=C_INC
 */
extern vmsstat_t DPSLCKLYS_DB_INIT(void);
extern int DPSLCKLYS_DBGETKLYSFLOAT(char*, float*);
extern int DPSLCKLYS_GETSTATUS(char*, char*, char*, short*);
extern int DPSLCKLYS_ACCESSENABLED(void);
extern int DPSLCKLYS_SETDEACTORREACT(char*, int, char*);
extern int DPSLCKLYS_SETCONFIG(char*, float*, char*);
extern int DPSLCKLYS_SETTRIMPHASE(char*, char*, float*, char*, float*);
extern void JNI_ERR_KLYSTRANSLATE(const unsigned long int*, struct dsc$descriptor*);
extern void DPSLCKLYS_SETNAMESVALIDATE(int, char *, char *, int *, int4u, char *);
extern int DPSLCKLYS_SETVALUES(int, char *, char *, int *, int4u, float *);

#define MAX_VALIDITY_STRING_LEN 40

#define LINKLYSTA_ACCEL       0x0001
#define LINKLYSTA_BAD         0x0004
#define LINKLYSTA_STANDBY     0x0002
#define LINKLYSTA_SLED_TUNED  0x0008
#define LINKLYSTA_PAMPL       0x0020
#define LINKLYSTA_SLEDED      0x0010
#define LINKLYSTA_PPHAS       0x0040
#ifdef __cplusplus
}
#endif
#endif
