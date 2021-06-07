/**
 * Sample implementation for service
      **MEMBER**=SLCLIBS:AIDASHRLIB
      **ATTRIBUTES**=JNI
 */
#include "slac_aida_NativeChannelProvider.h"
#include <jni.h>
#include <stdio.h>

/*
 * Get channel configuration for the specified channel
 *
 * Class:     edu_stanford_slac_aida_lib_AidaNativeChannelProvider
 * Method:    aidaChannelConfig
 * Signature: (Ljava/lang/String;)Ledu/stanford/slac/aida/lib/model/AidaChannelConfig;
 */
JNIEXPORT jobject

JNICALL Java_slac_aida_AidaNativeChannelProvider_aidaChannelConfig
        (JNIEnv *env, jobject jthis, jstring channelName) {
    printf("Called aidaChannelConfig()");
    return NULL;
}

/*
 * Class:     edu_stanford_slac_aida_lib_AidaNativeChannelProvider
 * Method:    aidaRequestTable
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ljava/util/List;
 */
JNIEXPORT jobject

JNICALL Java_slac_aida_AidaNativeChannelProvider_aidaRequestTable
        (JNIEnv *env, jobject jthis, jstring uri, jobject args) {
    printf("Called aidaRequestTable()");
    return NULL;
}

/*
 * Class:     edu_stanford_slac_aida_lib_AidaNativeChannelProvider
 * Method:    aidaRequestBoolean
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ljava/lang/Boolean;
 */
JNIEXPORT jboolean

JNICALL Java_slac_aida_AidaNativeChannelProvider_aidaRequestBoolean
        (JNIEnv *env, jobject jthis, jstring uri, jobject args) {
    printf("Called aidaRequestBoolean()");
    return 0;
}

/*
 * Class:     edu_stanford_slac_aida_lib_AidaNativeChannelProvider
 * Method:    aidaRequestByte
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ljava/lang/Byte;
 */
JNIEXPORT jbyte

JNICALL Java_slac_aida_AidaNativeChannelProvider_aidaRequestByte
        (JNIEnv *env, jobject jthis, jstring uri, jobject args) {
    printf("Called aidaRequestByte()");
    return 0;
}


/*
 * Class:     edu_stanford_slac_aida_lib_AidaNativeChannelProvider
 * Method:    aidaRequestShort
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ljava/lang/Short;
 */
JNIEXPORT jshort

JNICALL Java_slac_aida_AidaNativeChannelProvider_aidaRequestShort
        (JNIEnv *env, jobject jthis, jstring uri, jobject args) {
    printf("Called aidaRequestShort()");
    return 0;
}


/*
 * Class:     edu_stanford_slac_aida_lib_AidaNativeChannelProvider
 * Method:    aidaRequestInteger
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ljava/lang/Integer;
 */
JNIEXPORT jint

JNICALL Java_slac_aida_AidaNativeChannelProvider_aidaRequestInteger
        (JNIEnv *env, jobject jthis, jstring uri, jobject args) {
    printf("Called aidaRequestInteger()");
    return 0;
}

/*
 * Class:     edu_stanford_slac_aida_lib_AidaNativeChannelProvider
 * Method:    aidaRequestLong
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ljava/lang/Long;
 */
JNIEXPORT jlong

JNICALL Java_slac_aida_AidaNativeChannelProvider_aidaRequestLong
        (JNIEnv *env, jobject jthis, jstring uri, jobject args) {
    printf("Called aidaRequestLong()");
    return 0;
}


/*
 * Class:     edu_stanford_slac_aida_lib_AidaNativeChannelProvider
 * Method:    aidaRequestFloat
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ljava/lang/Float;
 */
JNIEXPORT jfloat

JNICALL Java_slac_aida_AidaNativeChannelProvider_aidaRequestFloat
        (JNIEnv *env, jobject jthis, jstring uri, jobject args) {
    printf("Called aidaRequestFloat()");
    return 0.0;
}


/*
 * Class:     edu_stanford_slac_aida_lib_AidaNativeChannelProvider
 * Method:    aidaRequestDouble
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ljava/lang/Double;
 */
JNIEXPORT jdouble

JNICALL Java_slac_aida_AidaNativeChannelProvider_aidaRequestDouble
        (JNIEnv *env, jobject jthis, jstring uri, jobject args) {
    printf("Called aidaRequestDouble()");
    return 0.0;
}


/*
 * Class:     edu_stanford_slac_aida_lib_AidaNativeChannelProvider
 * Method:    aidaRequestString
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ljava/lang/String;
 */
JNIEXPORT jstring

JNICALL Java_slac_aida_AidaNativeChannelProvider_aidaRequestString
        (JNIEnv *env, jobject jthis, jstring uri, jobject args) {
    printf("Called aidaRequestString()");
    return NULL;
}


/*
 * Class:     edu_stanford_slac_aida_lib_AidaNativeChannelProvider
 * Method:    aidaRequestBooleanArray
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ljava/util/List;
 */
JNIEXPORT jbooleanArray

JNICALL Java_slac_aida_AidaNativeChannelProvider_aidaRequestBooleanArray
        (JNIEnv *env, jobject jthis, jstring uri, jobject args) {
    printf("Called aidaRequestBooleanArray()");
    return NULL;
}


/*
 * Class:     edu_stanford_slac_aida_lib_AidaNativeChannelProvider
 * Method:    aidaRequestByteArray
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ljava/util/List;
 */
JNIEXPORT jbyteArray

JNICALL Java_slac_aida_AidaNativeChannelProvider_aidaRequestByteArray
        (JNIEnv *env, jobject jthis, jstring uri, jobject args) {
    printf("Called aidaRequestByteArray()");
    return NULL;
}


/*
 * Class:     edu_stanford_slac_aida_lib_AidaNativeChannelProvider
 * Method:    aidaRequestShortArray
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ljava/util/List;
 */
JNIEXPORT jshortArray

JNICALL Java_slac_aida_AidaNativeChannelProvider_aidaRequestShortArray
        (JNIEnv *env, jobject jthis, jstring uri, jobject args) {
    printf("Called aidaRequestShortArray()");
    return NULL;
}

/*
 * Class:     edu_stanford_slac_aida_lib_AidaNativeChannelProvider
 * Method:    aidaRequestIntegerArray
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ljava/util/List;
 */
JNIEXPORT jintArray

JNICALL Java_slac_aida_AidaNativeChannelProvider_aidaRequestIntegerArray
        (JNIEnv *env, jobject jthis, jstring uri, jobject args) {
    printf("Called aidaRequestIntegerArray()");
    return NULL;
}


/*
 * Class:     edu_stanford_slac_aida_lib_AidaNativeChannelProvider
 * Method:    aidaRequestLongArray
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ljava/util/List;
 */
JNIEXPORT jlongArray

JNICALL Java_slac_aida_AidaNativeChannelProvider_aidaRequestLongArray
        (JNIEnv *env, jobject jthis, jstring uri, jobject args) {
    printf("Called aidaRequestLongArray()");
    return NULL;
}


/*
 * Class:     edu_stanford_slac_aida_lib_AidaNativeChannelProvider
 * Method:    aidaRequestFloatArray
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ljava/util/List;
 */
JNIEXPORT jfloatArray

JNICALL Java_slac_aida_AidaNativeChannelProvider_aidaRequestFloatArray
        (JNIEnv *env, jobject jthis, jstring uri, jobject args) {
    printf("Called aidaRequestFloatArray()");
    return NULL;
}


/*
 * Class:     edu_stanford_slac_aida_lib_AidaNativeChannelProvider
 * Method:    aidaRequestDoubleArray
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ljava/util/List;
 */
JNIEXPORT jdoubleArray

JNICALL Java_slac_aida_AidaNativeChannelProvider_aidaRequestDoubleArray
        (JNIEnv *env, jobject jthis, jstring uri, jobject args) {
    printf("Called aidaRequestDoubleArray()");
    return NULL;
}


/*
 * Class:     edu_stanford_slac_aida_lib_AidaNativeChannelProvider
 * Method:    aidaRequestStringArray
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ljava/util/List;
 */
JNIEXPORT jobjectArray

JNICALL Java_slac_aida_AidaNativeChannelProvider_aidaRequestStringArray
        (JNIEnv *env, jobject jthis, jstring uri, jobject args) {
    printf("Called aidaRequestStringArray()");
    return NULL;
}


