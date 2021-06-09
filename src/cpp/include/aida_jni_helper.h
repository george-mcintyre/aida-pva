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
typedef struct
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
JavaObject newObject(JNIEnv* env, char* classToCreate);

/**
 * Convert jstring to c string
 * @param env env
 * @param string jstring
 * @return c-string
 */
const char* toCString(JNIEnv* env, jstring string);

/**
 * Convert c-string to j-string
 *
 * @param env env
 * @param string c-string
 * @return j-string
 */
 jstring toJString(JNIEnv* env, const char* string);

/**
 * Get the method ID on the given class with the given method name and signature
 *
 * @param env env
 * @param cls given class
 * @param methodName given method name
 * @param methodSignature given signature
 * @return the method that matches the name and signature specified
 */
 jmethodID getMethodId(JNIEnv* env, jclass cls, char* methodName, char* methodSignature);

/**
 * Get the method ID of the constructor of the given class
 *
 * @param env env
 * @param cls given class
 * @return the constructor method id
 */
jmethodID getConstructorMethodId(JNIEnv* env, jclass cls);

/**
 * Call setter on a given object with a string argument
 *
 * @param env environment
 * @param javaObject given object
 * @param method string setter method to call
 * @param value string value to set
 */
void callSetterWithString(JNIEnv* env, JavaObject javaObject, char* method, char* value);

/**
 * Call a setter on a given object with a jString argument
 *
 * @param env env
 * @param javaObject object
 * @param method method
 * @param value jstring value
 */
void callSetterWithJString(JNIEnv* env, JavaObject javaObject, char* method, jstring value);

/**
 * Get c arguments structure from a java arguments list - List<AidaArgument>
 *
 *
 * @param env env
 * @param jArgs java arguments list - List<AidaArgument> (name, value}
 * @return c arguments structure
 */
Arguments toArguments(JNIEnv* env, jobject jArgs);

/**
 * Free up any memory allocated with arguments
 * @param arguments
 */
void releaseArguments(Arguments arguments);

/**
 * Free up any memory allocated with arguments
 * @param array
 */
void releaseArray(Array array);

/**
 * Free up any memory allocated with arguments
 * @param array
 */
void releaseStringArray(StringArray array);

#ifdef __cplusplus
}
#endif
#endif

