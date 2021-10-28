#ifndef slcMosc_server_h
#define slcMosc_server_h
#ifdef __cplusplus
extern "C" {
#endif
/*
 *
 *  **CMS**=C_INC
 *
 */
extern vmsstat_t DPSLCMOSC_DB_INIT(void);
extern vmsstat_t DPSLCMOSC_MEASMASTEROSC(double*);
extern int DPSLCMOSC_ACCESSENABLED(void);
extern vmsstat_t DPSLCMOSC_SETMASTEROSC(float*, char*, char*, double*);
extern void JNI_ERR_MOSCTRANSLATE(const unsigned long int*, struct dsc$descriptor*);

#ifdef __cplusplus
}
#endif
#endif
