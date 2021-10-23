#ifndef _Included_slcMosc_server
#define _Included_slcMosc_server
#ifdef __cplusplus
extern "C" {
#endif

extern vmsstat_t DPSLCMOSC_DB_INIT(void);
extern vmsstat_t DPSLCMOSC_MEASMASTEROSC(double*);
extern int DPSLCMOSC_ACCESSENABLED(void);
extern vmsstat_t DPSLCMOSC_SETMASTEROSC(float*, char*, char*, double*);
extern void JNI_ERR_MOSCTRANSLATE(const unsigned long int*, struct dsc$descriptor*);

#ifdef __cplusplus
}
#endif
#endif
