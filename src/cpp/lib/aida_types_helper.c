#include <jni.h>
#include "aida_types.h"

/**
 * Convert Type to string
 * @param env env
 * @param type type
 * @return string
 */
jstring toTypeString(JNIEnv* env, Type type)
{
	switch (type) {
	case BOOLEAN :
		return (*env)->NewStringUTF(env, "BOOLEAN");
	case BYTE :
		return (*env)->NewStringUTF(env, "BYTE");
	case SHORT :
		return (*env)->NewStringUTF(env, "SHORT");
	case INTEGER :
		return (*env)->NewStringUTF(env, "INTEGER");
	case LONG :
		return (*env)->NewStringUTF(env, "LONG");
	case FLOAT :
		return (*env)->NewStringUTF(env, "FLOAT");
	case DOUBLE :
		return (*env)->NewStringUTF(env, "DOUBLE");
	case STRING :
		return (*env)->NewStringUTF(env, "STRING");
	case BOOLEAN_ARRAY :
		return (*env)->NewStringUTF(env, "BOOLEAN_ARRAY");
	case BYTE_ARRAY :
		return (*env)->NewStringUTF(env, "BYTE_ARRAY");
	case SHORT_ARRAY :
		return (*env)->NewStringUTF(env, "SHORT_ARRAY");
	case INTEGER_ARRAY :
		return (*env)->NewStringUTF(env, "INTEGER_ARRAY");
	case LONG_ARRAY :
		return (*env)->NewStringUTF(env, "LONG_ARRAY");
	case FLOAT_ARRAY :
		return (*env)->NewStringUTF(env, "FLOAT_ARRAY");
	case DOUBLE_ARRAY :
		return (*env)->NewStringUTF(env, "DOUBLE_ARRAY");
	case STRING_ARRAY :
		return (*env)->NewStringUTF(env, "STRING_ARRAY");
	case TABLE :
	default :
		return (*env)->NewStringUTF(env, "TABLE");
	}
}

/**
 * Convert Layout to string
 * @param env env
 * @param layout layout
 * @return string
 */
jstring toLayoutString(JNIEnv* env, Layout layout)
{
	switch (layout) {
	case ROW_MAJOR:
		return (*env)->NewStringUTF(env, "ROW_MAJOR");
	case COLUMN_MAJOR:
	default:
		return (*env)->NewStringUTF(env, "COLUMN_MAJOR");
	}
}
