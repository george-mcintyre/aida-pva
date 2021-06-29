/**
 * JNI Service interface: DO NOT EDIT THIS FILE
      **MEMBER**=SLCLIBS:AIDASHRLIB
      **ATTRIBUTES**=JNI
 */
#include <jni.h>
#include <stdio.h>
#include "aida_types.h"
#include "reference_server.h"
#include "aida_jni_helper.h"

/*
 * Get channel configuration for the specified channel
 *
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaChannelConfig
 * Signature: (Ljava/lang/String;)Ledu/stanford/slac/aida/lib/model/AidaChannelConfig;
 */
JNIEXPORT jobject
JNICALL Java_slac_aida_NativeChannelProvider_aidaChannelConfig
		(JNIEnv* env, jobject jthis, jstring channelName)
{
	jobject returnValue = NULL;

	const char* channel = toCString(env, channelName);

	// Call the implementation
	Config config = aidaChannelConfig(channel);

	// marshal results and free fields
	returnValue = aidaChannelConfigToJObject(env, config);

	return returnValue;
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
	jboolean returnValue;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);

	returnValue = aidaRequestBoolean(pv, arguments);

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
	jbyte returnValue;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);

	returnValue = aidaRequestByte(pv, arguments);

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
	jshort returnValue;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);

	returnValue = aidaRequestShort(pv, arguments);

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
	jint returnValue;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);

	returnValue = aidaRequestInteger(pv, arguments);

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
	jlong returnValue;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);

	returnValue = aidaRequestLong(pv, arguments);

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
	jfloat returnValue;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);

	returnValue = aidaRequestFloat(pv, arguments);

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
	jdouble returnValue;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);

	returnValue = aidaRequestDouble(pv, arguments);

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

	returnValue = toJString(env, aidaRequestString(pv, arguments));

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

	Array array = aidaRequestBooleanArray(pv, arguments);

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

	Array array = aidaRequestByteArray(pv, arguments);

	// create result array
	returnValue =toByteArray(env, array);

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

	Array array = aidaRequestShortArray(pv, arguments);

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

	Array array = aidaRequestIntegerArray(pv, arguments);

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

	Array array = aidaRequestLongArray(pv, arguments);

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

	Array array = aidaRequestFloatArray(pv, arguments);

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

	Array array = aidaRequestDoubleArray(pv, arguments);

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

	StringArray array = aidaRequestStringArray(pv, arguments);

	// create result array
	returnValue = toStringArray(env, array);

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}

/*
 * Class:     slac_aida_NativeChannelProvider
 * Method:    aidaRequestTable
 * Signature: (Ljava/lang/String;Ljava/util/List;)Ljava/util/List;
 */
JNIEXPORT jobject
JNICALL Java_slac_aida_NativeChannelProvider_aidaRequestTable
		(JNIEnv* env, jobject jthis, jstring uri, jobject args)
{
	jobject returnValue;

	const char* pv = toCString(env, uri);
	Arguments arguments = toArguments(env, args);

	Table table = aidaRequestTable(pv, arguments);

	// create result table
	returnValue = toTable(env, table);

	// Free up arguments list
	releaseArguments(arguments);

	return returnValue;
}


