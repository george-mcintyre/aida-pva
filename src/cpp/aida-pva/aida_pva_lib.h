/** @file
 *  @brief Conversion of camelCase to uppercase references from AIDA-PVA LIB to providers.
 *       **CMS**=C_INC
 */
#ifndef _Included_aida_provider
#define _Included_aida_provider
#ifdef __cplusplus
extern "C" {
#endif

#define getValue(__env, __arguments)      GETVALUE(__env, __arguments)
#define releaseArguments(__arguments)     RELEASEARGUMENTS(__arguments)
#define releaseArray(__array)             RELEASEARRAY(__array)
#define releaseStringArray(__array)       RELEASESTRINGARRAY(__array)
#define releaseTable(__table)             RELEASETABLE(__table)
#define releaseValue(__value)             RELEASEVALUE(__value)
#define toArguments(__env, __jArgs)       TOARGUMENTS(__env, __jArgs)
#define toBooleanArray(__env, __array)    TOBOOLEANARRAY(__env, __array)
#define toByteArray(__env, __array)       TOBYTEARRAY(__env, __array)
#define toCString(__env, __string)        TOCSTRING(__env, __string)
#define toDoubleArray(__env, __array)     TODOUBLEARRAY(__env, __array)
#define toFloatArray(__env, __array)      TOFLOATARRAY(__env, __array)
#define toIntegerArray(__env, __array)    TOINTEGERARRAY(__env, __array)
#define toJString(__env, __string)        TOJSTRING(__env, __string)
#define toLongArray(__env, __array)       TOLONGARRAY(__env, __array)
#define toShortArray(__env, __array)      TOSHORTARRAY(__env, __array)
#define toStringArray(__env, __array)     TOSTRINGARRAY(__env, __array)
#define toTable(__env, __table)           TOTABLE(__env, __table)

#ifdef __cplusplus
}
#endif
#endif

