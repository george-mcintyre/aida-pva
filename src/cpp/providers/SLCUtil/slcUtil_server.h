#ifndef slcUtil_server_h
#define slcUtil_server_h
#ifdef __cplusplus
extern "C" {
#endif
/*
 *
 *  **CMS**=C_INC
 *
 */
#define MAX_DEVICE_STRING_LEN 20

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
