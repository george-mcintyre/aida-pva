/**
 * JNI Service interface: DO NOT EDIT THIS FILE
      **MEMBER**=SLCLIBS:AIDASHRLIB
      **ATTRIBUTES**=JNI
 */
#include <jni.h>
#include <stdlib.h>
#include "aida_types.h"
#include "aida_server_helper.h"
#include "aida_jni_helper.h"

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaServiceInit
 * Signature: ()V
 */
JNIEXPORT void
JNICALL Java_slac_aida_NativeChannelProvider_aidaServiceInit(JNIEnv* env, jclass jclass)
{
	aidaServiceInit(env);
}

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaRequestBoolean
 * Signature: (Ljava/lang/String;Ljava/util/List;)Z
 */
jboolean
JNICALL Java_slac_aida_NativeChannelProvider_aidaRequestBoolean
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	jboolean returnValue = 0;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);
	CHECK_EXCEPTION_FOR_ARGUMENTS(returnValue)

	returnValue = aidaRequestBoolean(env, pv, arguments);

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaRequestByte
 * Signature: (Ljava/lang/String;Ljava/util/List;)B
 */
JNIEXPORT jbyte
JNICALL Java_slac_aida_NativeChannelProvider_aidaRequestByte
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	jbyte returnValue = 0x0;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);
	CHECK_EXCEPTION_FOR_ARGUMENTS(returnValue)

	returnValue = aidaRequestByte(env, pv, arguments);

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaRequestShort
 * Signature: (Ljava/lang/String;Ljava/util/List;)S
 */
JNIEXPORT jshort
JNICALL Java_slac_aida_NativeChannelProvider_aidaRequestShort
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	jshort returnValue = 0;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);
	CHECK_EXCEPTION_FOR_ARGUMENTS(returnValue)

	returnValue = aidaRequestShort(env, pv, arguments);

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaRequestInteger
 * Signature: (Ljava/lang/String;Ljava/util/List;)I
 */
JNIEXPORT jint
JNICALL Java_slac_aida_NativeChannelProvider_aidaRequestInteger
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	jint returnValue = 0;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);
	CHECK_EXCEPTION_FOR_ARGUMENTS(returnValue)

	returnValue = aidaRequestInteger(env, pv, arguments);

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaRequestLong
 * Signature: (Ljava/lang/String;Ljava/util/List;)J
 */
JNIEXPORT jlong
JNICALL Java_slac_aida_NativeChannelProvider_aidaRequestLong
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	jlong returnValue = 0;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);
	CHECK_EXCEPTION_FOR_ARGUMENTS(returnValue)

	returnValue = aidaRequestLong(env, pv, arguments);

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaRequestFloat
 * Signature: (Ljava/lang/String;Ljava/util/List;)F
 */
JNIEXPORT jfloat
JNICALL Java_slac_aida_NativeChannelProvider_aidaRequestFloat
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	jfloat returnValue = 0;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);
	CHECK_EXCEPTION_FOR_ARGUMENTS(returnValue)

	returnValue = aidaRequestFloat(env, pv, arguments);

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaRequestDouble
 * Signature: (Ljava/lang/String;Ljava/util/List;)D
 */
JNIEXPORT jdouble
JNICALL Java_slac_aida_NativeChannelProvider_aidaRequestDouble
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	jdouble returnValue = 0;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);
	CHECK_EXCEPTION_FOR_ARGUMENTS(returnValue)

	returnValue = aidaRequestDouble(env, pv, arguments);

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaRequestString
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ljava/lang/String;
 */
JNIEXPORT jstring
JNICALL Java_slac_aida_NativeChannelProvider_aidaRequestString
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	jstring returnValue;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);
	CHECK_EXCEPTION_FOR_ARGUMENTS(NULL)

	char* string = aidaRequestString(env, pv, arguments);
	CHECK_EXCEPTION_FOR_STRING(NULL)

	returnValue = toJString(env, string);

	// Release String
	if ( string ) {
		free(string);
	}

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaRequestBooleanArray
 * Signature: (Ljava/lang/String;Ljava/util/List;)[Z
 */
JNIEXPORT jbooleanArray
JNICALL Java_slac_aida_NativeChannelProvider_aidaRequestBooleanArray
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	jbooleanArray returnValue;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);
	CHECK_EXCEPTION_FOR_ARGUMENTS(NULL)

	Array array = aidaRequestBooleanArray(env, pv, arguments);
	CHECK_EXCEPTION_FOR_ARRAY(NULL)

	// create result array
	returnValue = toBooleanArray(env, array);

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaRequestByteArray
 * Signature: (Ljava/lang/String;Ljava/util/List;)[B
 */
JNIEXPORT jbyteArray
JNICALL Java_slac_aida_NativeChannelProvider_aidaRequestByteArray
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	jbyteArray returnValue;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);
	CHECK_EXCEPTION_FOR_ARGUMENTS(NULL)

	Array array = aidaRequestByteArray(env, pv, arguments);
	CHECK_EXCEPTION_FOR_ARRAY(NULL)

	// create result array
	returnValue = toByteArray(env, array);

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaRequestShortArray
 * Signature: (Ljava/lang/String;Ljava/util/List;)[S
 */
JNIEXPORT jshortArray
JNICALL Java_slac_aida_NativeChannelProvider_aidaRequestShortArray
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	jshortArray returnValue;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);
	CHECK_EXCEPTION_FOR_ARGUMENTS(NULL)

	Array array = aidaRequestShortArray(env, pv, arguments);
	CHECK_EXCEPTION_FOR_ARRAY(NULL)

	// create result array
	returnValue = toShortArray(env, array);

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaRequestIntegerArray
 * Signature: (Ljava/lang/String;Ljava/util/List;)[I
 */
JNIEXPORT jintArray
JNICALL Java_slac_aida_NativeChannelProvider_aidaRequestIntegerArray
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	jintArray returnValue;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);
	CHECK_EXCEPTION_FOR_ARGUMENTS(NULL)

	Array array = aidaRequestIntegerArray(env, pv, arguments);
	CHECK_EXCEPTION_FOR_ARRAY(NULL)

	// create result array
	returnValue = toIntegerArray(env, array);

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaRequestLongArray
 * Signature: (Ljava/lang/String;Ljava/util/List;)[J
 */
JNIEXPORT jlongArray
JNICALL Java_slac_aida_NativeChannelProvider_aidaRequestLongArray
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	jlongArray returnValue;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);
	CHECK_EXCEPTION_FOR_ARGUMENTS(NULL)

	Array array = aidaRequestLongArray(env, pv, arguments);
	CHECK_EXCEPTION_FOR_ARRAY(NULL)

	// create result array
	returnValue = toLongArray(env, array);

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaRequestFloatArray
 * Signature: (Ljava/lang/String;Ljava/util/List;)[F
 */
JNIEXPORT jfloatArray
JNICALL Java_slac_aida_NativeChannelProvider_aidaRequestFloatArray
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	jfloatArray returnValue;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);
	CHECK_EXCEPTION_FOR_ARGUMENTS(NULL)

	Array array = aidaRequestFloatArray(env, pv, arguments);
	CHECK_EXCEPTION_FOR_ARRAY(NULL)

	// create result array
	returnValue = toFloatArray(env, array);

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaRequestDoubleArray
 * Signature: (Ljava/lang/String;Ljava/util/List;)[D
 */
JNIEXPORT jdoubleArray
JNICALL Java_slac_aida_NativeChannelProvider_aidaRequestDoubleArray
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	jdoubleArray returnValue;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);
	CHECK_EXCEPTION_FOR_ARGUMENTS(NULL)

	Array array = aidaRequestDoubleArray(env, pv, arguments);
	CHECK_EXCEPTION_FOR_ARRAY(NULL)

	// create result array
	returnValue = toDoubleArray(env, array);

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaRequestStringArray
 * Signature: (Ljava/lang/String;Ljava/util/List;)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray
JNICALL Java_slac_aida_NativeChannelProvider_aidaRequestStringArray
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	jobjectArray returnValue;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);
	CHECK_EXCEPTION_FOR_ARGUMENTS(NULL)

	StringArray array = aidaRequestStringArray(env, pv, arguments);
	CHECK_EXCEPTION_FOR_STRING_ARRAY

	// create result array
	returnValue = toStringArray(env, array);

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaRequestTable
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ledu/stanford/slac/aida/lib/model/AidaTable;
 */
JNIEXPORT jobject
JNICALL Java_slac_aida_NativeChannelProvider_aidaRequestTable
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	jobject returnValue;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);
	CHECK_EXCEPTION_FOR_ARGUMENTS(NULL)

	Table table = aidaRequestTable(env, pv, arguments);
	CHECK_EXCEPTION_FOR_TABLE(NULL)

	// create result table
	returnValue = toTable(env, table);
	CHECK_EXCEPTION_FOR_TABLE(NULL)

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}


/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaSetValue
 * Signature: (Ljava/lang/String;Ljava/util/List;)V
 */
JNIEXPORT void
JNICALL Java_slac_aida_NativeChannelProvider_aidaSetValue
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);
	CHECK_EXCEPTION_AND_ARGUMENTS_VOID

	Value value = getValue(env, arguments);
	CHECK_EXCEPTION_FOR_VALUE_VOID

	aidaSetValue(env, pv, arguments, value);

	// Free up value
	releaseValue(value);

	// Free up arguments list
	releaseArguments(arguments);
}

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaSetValueWithResponse
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ledu/stanford/slac/aida/lib/model/AidaTable;
 */
JNIEXPORT jobject
JNICALL Java_slac_aida_NativeChannelProvider_aidaSetValueWithResponse
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	jobject returnValue;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);
	CHECK_EXCEPTION_FOR_ARGUMENTS(NULL)

	Value value = getValue(env, arguments);
	CHECK_EXCEPTION_FOR_VALUE(NULL)

	Table table = aidaSetValueWithResponse(env, pv, arguments, value);
	CHECK_EXCEPTION_FOR_TABLE(NULL)

	// create result table
	returnValue = toTable(env, table);

	// Free up value
	releaseValue(value);

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}

