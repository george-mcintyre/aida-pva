/** @file
 *  @brief Conversion of camelCase to uppercase references from providers to AIDA-PVA Module.
 *       **CMS**=C_INC
 */
#ifndef _Included_aida_lib
#define _Included_aida_lib
#ifdef __cplusplus
extern "C" {
#endif

#define aidaRequestBoolean(__env, __uri, __arguments)            AIDAREQUESTBOOLEAN(__env, __uri, __arguments)
#define aidaRequestBooleanArray(__env, __uri, __arguments)      AIDAREQUESTBOOLEANARRAY(__env, __uri, __arguments)
#define aidaRequestByte(__env, __uri, __arguments)              AIDAREQUESTBYTE(__env, __uri, __arguments)
#define aidaRequestByteArray(__env, __uri, __arguments)         AIDAREQUESTBYTEARRAY(__env, __uri, __arguments)
#define aidaRequestDouble(__env, __uri, __arguments)            AIDAREQUESTDOUBLE(__env, __uri, __arguments)
#define aidaRequestDoubleArray(__env, __uri, __arguments)       AIDAREQUESTDOUBLEARRAY(__env, __uri, __arguments)
#define aidaRequestFloat(__env, __uri, __arguments)             AIDAREQUESTFLOAT(__env, __uri, __arguments)
#define aidaRequestFloatArray(__env, __uri, __arguments)        AIDAREQUESTFLOATARRAY(__env, __uri, __arguments)
#define aidaRequestInteger(__env, __uri, __arguments)           AIDAREQUESTINTEGER(__env, __uri, __arguments)
#define aidaRequestIntegerArray(__env, __uri, __arguments)      AIDAREQUESTINTEGERARRAY(__env, __uri, __arguments)
#define aidaRequestLong(__env, __uri, __arguments)              AIDAREQUESTLONG(__env, __uri, __arguments)
#define aidaRequestLongArray(__env, __uri, __arguments)         AIDAREQUESTLONGARRAY(__env, __uri, __arguments)
#define aidaRequestShort(__env, __uri, __arguments)             AIDAREQUESTSHORT(__env, __uri, __arguments)
#define aidaRequestShortArray(__env, __uri, __arguments)        AIDAREQUESTSHORTARRAY(__env, __uri, __arguments)
#define aidaRequestString(__env, __uri, __arguments)            AIDAREQUESTSTRING(__env, __uri, __arguments)
#define aidaRequestStringArray(__env, __uri, __arguments)       AIDAREQUESTSTRINGARRAY(__env, __uri, __arguments)
#define aidaRequestTable(__env, __uri, __arguments)             AIDAREQUESTTABLE(__env, __uri, __arguments)
#define aidaServiceInit(__env)                                  AIDASERVICEINIT(__env)
#define aidaSetValue(__env, __uri, __arguments)                 AIDASETVALUE(__env, __uri, __arguments)
#define aidaSetValueWithResponse(__env, __uri, __arguments)     AIDASETVALUEWITHRESPONSE(__env, __uri, __arguments)

#ifdef __cplusplus
}
#endif
#endif
