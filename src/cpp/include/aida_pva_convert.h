/** @file
 *  @brief The Header File for the Native Channel Provider AIDA-PVA type related functions.
 *       **CMS**=C_INC
 */
#include <jni.h>
#include <stdbool.h>
#include "aida_pva_json.h"

#ifndef aida_pva_convert_h
#define aida_pva_convert_h
#ifdef __cplusplus
extern "C" {
#endif

#include "sysutil_proto.h"
#include "aida_pva.h"

/**
 * @def CONVERT_TO_VMS_FLOAT
 * Convert in-place, floating point numbers from ieee to VMS format. Give a pointer to
 * an array of ieee floating point numbers and this macro will convert it in-place, to VMS format
 * @param _float pointer to a single ieee floating point number or an array of them
 * @param _count the number of ieee point numbers to convert
 */
#define CONVERT_TO_VMS_FLOAT(_float, _count) \
{  \
    int2u _n = _count; \
    CVT_IEEE_TO_VMS_FLT(_float, _float, &_n); \
}

/**
 * @def CONVERT_TO_VMS_DOUBLE
 * Convert in-place, doubles from ieee to VMS format. Give a pointer to
 * an array of ieee doubles and this macro will convert it in-place, to VMS format
 * @param _double pointer to a single ieee double or an array of them
 * @param _count the number of doubles to convert
 */
#define CONVERT_TO_VMS_DOUBLE(_double, _count) \
{  \
    int2u _n = _count; \
    CVT_IEEE_TO_VMS_DBL(_double, _double, &_n); \
}

/**
 * @def CONVERT_FROM_VMS_FLOAT
 * Convert in-place, floating point numbers from VMS to ieee format. Give a pointer to
 * an array of VMS floating point numbers and this macro will convert it in-place, to ieee format
 * @param _float pointer to a single VMS floating point number or an array of them
 * @param _count the number of floating point numbers to convert
 */
#define CONVERT_FROM_VMS_FLOAT(_float, _count) \
{  \
    int2u _n = _count; \
    CVT_VMS_TO_IEEE_FLT(_float, _float, &_n); \
}

/**
 * @def CONVERT_FROM_VMS_DOUBLE
 * Convert in-place, doubles from VMS to ieee format. Give a pointer to
 * an array of VMS doubles and this macro will convert it in-place, to ieee format
 * @param _double pointer to a single VMS double or an array of them
 * @param _count the number of doubles to convert
 */
#define CONVERT_FROM_VMS_DOUBLE(_double, _count) \
{  \
    int2u _n = _count; \
    CVT_VMS_TO_IEEE_DBL(_double, _double, &_n); \
}

#ifdef __cplusplus
}
#endif
#endif

