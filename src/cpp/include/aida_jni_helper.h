#ifndef _Included_aida_jni_helper
#define _Included_aida_jni_helper
#ifdef __cplusplus
extern "C" {
#endif

#include <jni.h>
#include "aida_types.h"

/**
 * Java object: containing the jobject and the class
 */
typdef struct
{
	jobject object;
	jclass class;
} JavaObject;

JavaObject newObject(JNIEnv* env, char* classToCreate);

/**
 * Get an aida channel config jni object from Config
 * @param env env
 * @param config config
 * @return aida channel config jni object
 */
jobject aidaChannelConfigToJObject(JNIEnv* env, Config config);

/**
 * Get an aida field jni object from a Field
 * @param env env
 * @param field  field
 * @return aida jni field object
 */
jobject getAidaField(JNIEnv* env, Field field);

/**
 * Look up class in environment and create a new object
 * @param env environment
 * @param class class to create
 * @return the new jni object
 */
jobject newObject(JNIEnv* env, char* classToCreate);

/**
 * Convert jstring to c string
 * @param env env
 * @param string jstring
 * @return c-string
 */
const char* toCString(JNIEnv* env, jstring string);

jmethodID getMethodId(JNIEnv* env, jclass cls, char* methodName, char* methodSignature);

jmethodID getConstructorMethodId(JNIEnv* env, jclass cls);

void callSetterWithString(JNIEnv* env, JavaObject javaObject, char* method, char* value);

void callSetterWithJString(JNIEnv* env, JavaObject javaObject, char* method, jstring value);

#ifdef __cplusplus
}
#endif
#endif

