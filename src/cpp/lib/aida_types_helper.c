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
	case AIDA_BOOLEAN_TYPE :
		return (*env)->NewStringUTF(env, "BOOLEAN");
	case AIDA_BYTE_TYPE :
		return (*env)->NewStringUTF(env, "BYTE");
	case AIDA_SHORT_TYPE :
		return (*env)->NewStringUTF(env, "SHORT");
	case AIDA_INTEGER_TYPE :
		return (*env)->NewStringUTF(env, "INTEGER");
	case AIDA_LONG_TYPE :
		return (*env)->NewStringUTF(env, "LONG");
	case AIDA_FLOAT_TYPE :
		return (*env)->NewStringUTF(env, "FLOAT");
	case AIDA_DOUBLE_TYPE :
		return (*env)->NewStringUTF(env, "DOUBLE");
	case AIDA_STRING_TYPE :
		return (*env)->NewStringUTF(env, "STRING");
	case AIDA_BOOLEAN_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "BOOLEAN_ARRAY");
	case AIDA_BYTE_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "BYTE_ARRAY");
	case AIDA_SHORT_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "SHORT_ARRAY");
	case AIDA_INTEGER_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "INTEGER_ARRAY");
	case AIDA_LONG_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "LONG_ARRAY");
	case AIDA_FLOAT_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "FLOAT_ARRAY");
	case AIDA_DOUBLE_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "DOUBLE_ARRAY");
	case AIDA_STRING_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "STRING_ARRAY");
	case AIDA_TABLE_TYPE :
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
	case AIDA_ROW_MAJOR_LAYOUT:
		return (*env)->NewStringUTF(env, "ROW_MAJOR");
	case AIDA_COLUMN_MAJOR_LAYOUT:
	default:
		return (*env)->NewStringUTF(env, "COLUMN_MAJOR");
	}
}
