/** @file
 *  @brief The Header File for the JNI helper functions.
 */
#ifndef _Included_aida_jni_helper
#define _Included_aida_jni_helper
#ifdef __cplusplus
extern "C" {
#endif

#include <jni.h>
#include "aida_types.h"

/**
 * Check to see if an exception has been raised,
 * and return the given return value.
 *
 * Uses local variable `env` for checking exception status.
 *
 * @param _r the specified return value.
 * @return This MACRO will return the specified return value from your function if there has been an exception.
 */
#define ON_EXCEPTION_RETURN_(r) \
    if ((*env)->ExceptionCheck(env)) { \
        return r; \
    }

/**
 * Check to see if an exception has been raised,
 * then free tracked memory,
 * and return the given return value.
 *
 * Uses local variable `env` for checking exception status.
 *
 * @param _r	the specified return value.
 * @return This MACRO will return the specified return value from your function if there has been an exception.
 */
#define ON_EXCEPTION_FREE_MEMORY_AND_RETURN_(r) \
    if ((*env)->ExceptionCheck(env)) { \
        FREE_MEMORY \
        return r; \
    }

/**
 * Check to see if an exception has been raised,
 * and return void,
 *
 * Uses local variable `env` for checking exception status.
 *
 * @return This MACRO will return from your function if there has been an exception.
 */
#define ON_EXCEPTION_RETURN_VOID \
    if ((*env)->ExceptionCheck(env)) { \
        return; \
    }

/**
 * Check to see if an exception has been raised,
 * free local variable {@link Arguments}
 * and return the given return value.
 *
 * Uses local variable `env` for checking exception status.
 *
 * @param _r the specified return value.
 * @return This MACRO will return the specified return value from your function if there has been an exception.
 */
#define ON_EXCEPTION_FREE_ARGUMENTS_AND_RETURN_(r) \
    if ((*env)->ExceptionCheck(env)) { \
        releaseArguments(arguments); \
        return r; \
    }

/**
 * Check to see if an exception has been raised,
 * free local variable {@link Arguments}
 * and return void,
 *
 * Uses local variable `env` for checking exception status.
 *
 * @return This MACRO will return from your function if there has been an exception.
 */
#define ON_EXCEPTION_FREE_ARGUMENTS_AND_RETURN_VOID \
    if ((*env)->ExceptionCheck(env)) { \
        releaseArguments(arguments); \
        return; \
    }

/**
 * Check to see if an exception has been raised,
 * free a local variable `string`,
 * free local variable {@link Arguments}
 * and return the given return value.
 *
 * Uses local variable `env` for checking exception status.
 *
 * @param _r the specified return value.
 * @return This MACRO will return the specified return value from your function if there has been an exception.
 */
#define ON_EXCEPTION_FREE_STRING_AND_ARGS_AND_RETURN_(r) \
    if ((*env)->ExceptionCheck(env)) { \
        if ( string ) free(string); \
        releaseArguments(arguments); \
        return r; \
    }

/**
 * Check to see if an exception has been raised,
 * free a local variable `string`,
 * and return the given return value.
 *
 * Uses local variable `env` for checking exception status.
 *
 * @param _r the specified return value.
 * @return This MACRO will return the specified return value from your function if there has been an exception.
 */
#define ON_EXCEPTION_FREE_STRING_AND_RETURN_(r) \
    if ((*env)->ExceptionCheck(env)) { \
        if ( string ) free(string); \
        return r; \
    }

/**
 * Check to see if an exception has been raised,
 * free local variable {@link Array},
 * free local variable {@link Arguments}
 * and return the given return value.
 *
 * Uses local variable `env` for checking exception status.
 *
 * @param _r the specified return value.
 * @return This MACRO will return the specified return value from your function if there has been an exception.
 */
#define ON_EXCEPTION_FREE_ARRAY_AND_ARGS_AND_RETURN_(r) \
    if ((*env)->ExceptionCheck(env)) { \
        releaseArray(array); \
        releaseArguments(arguments); \
        return r; \
    }

/**
 * Check to see if an exception has been raised,
 * free local variable {@link Array},
 * free local variable {@link Arguments}
 * and return void.
 *
 * Uses local variable `env` for checking exception status.
 *
 * @return This MACRO will return from your function if there has been an exception.
 */
#define ON_EXCEPTION_FREE_STRING_ARRAY_AND_ARGS_AND_RETURN_NULL \
    if ((*env)->ExceptionCheck(env)) { \
        releaseStringArray(array); \
        releaseArguments(arguments); \
        return NULL; \
    }

/**
 * Check to see if an exception has been raised,
 * free local variable {@link Table},
 * free local variable {@link Arguments},
 * and return the given return value.
 *
 * Uses local variable `env` for checking exception status.
 *
 * @param _r the specified return value.
 * @return This MACRO will return the specified return value from your function if there has been an exception.
 */
#define ON_EXCEPTION_FREE_TABLE_AND_ARGS_AND_RETURN_(r) \
    if ((*env)->ExceptionCheck(env)) { \
        releaseTable(table); \
        releaseArguments(arguments); \
        return r; \
    }

/**
 * Check to see if an exception has been raised,
 * free local variable `value`,
 * free local variable {@link Arguments}
 * and return void.
 *
 * Uses local variable `env` for checking exception status.
 *
 * @return This MACRO will return from your function if there has been an exception.
 */
#define ON_EXCEPTION_FREE_VALUE_AND_ARGS_AND_RETURN_VOID \
    if ((*env)->ExceptionCheck(env)) { \
        releaseValue(value); \
        releaseArguments(arguments); \
        return; \
    }

/**
 * Check to see if an exception has been raised,
 * free local variable `value`,
 * free local variable {@link Arguments},
 * and return the given return value.
 *
 * Uses local variable `env` for checking exception status.
 *
 * @param _r the specified return value.
 * @return This MACRO will return the specified return value from your function if there has been an exception.
 */
#define ON_EXCEPTION_FREE_VALUE_AND_ARGS_AND_RETURN_(r) \
    if ((*env)->ExceptionCheck(env)) { \
        releaseValue(value); \
        releaseArguments(arguments); \
        return r; \
    }

/**
 * A structure to hold a java object and java class.
 */
typedef struct
{
	jobject object;
	jclass class;
} JavaObject;

/**
 * A structure to hold a java class and a java method ID.
 */
typedef struct
{
	jclass class;
	jmethodID methodId;
} ClassAndMethod;


// For Argument Processing

/**
 * Set of java classes that are needed to process the java {@link edu.stanford.slac.aida.lib.model.AidaArgument} class in C
 */
typedef struct
{
	jclass listClass;
	jclass floatArgumentClass;
	jclass doubleArgumentClass;
	jclass aidaArgumentsClass;
	jclass aidaArgumentClass;
} ArgumentClasses;

/**
 * Set of java methods that are needed to process the java {@link edu.stanford.slac.aida.lib.model.AidaArgument} class in C
 */
typedef struct
{
	ArgumentClasses* argumentClasses;
	jmethodID listSizeMethod;
	jmethodID listGetMethod;
	jmethodID argumentsGetArgumentsMethod;
	jmethodID argumentGetNameMethod;
	jmethodID argumentGetValueMethod;
	jmethodID argumentsGetFloatArgumentsMethod;
	jmethodID argumentsGetDoubleArgumentsMethod;
	jmethodID getFloatNameMethod;
	jmethodID getFloatValueMethod;
	jmethodID getDoubleNameMethod;
	jmethodID getDoubleValueMethod;
} ArgumentMethods;

/**
 * Look up class in the given `env`,
 * and create a new java object.
 *
 * @param env environment.
 * @param clazz string name of java class to create.
 * @return the new java object.
 */
JavaObject newObject(JNIEnv* env, char* clazz);

/**
 * Convert given {@link jstring} to a C string.
 *
 * @param env environment.
 * @param string {@link jstring}.
 * @return C string.
 */
char* toCString(JNIEnv* env, jstring string);

/**
 * Convert C string to {@link jstring}.
 *
 * @param env environment.
 * @param string C string.
 * @return {@link jstring}.
 */
jstring toJString(JNIEnv* env, const char* string);

/**
 * Get the method ID on the given class,
 * with the given method name,
 * and signature.
 *
 * @param env environment.
 * @param clazz given class.
 * @param methodName given method name.
 * @param methodSignature given signature.
 * @return the method that matches the name and signature specified.
 */
jmethodID getMethodId(JNIEnv* env, jclass clazz, char* methodName, char* methodSignature);

/**
 * Get the method ID of the constructor of the given class.
 *
 * @param env environment.
 * @param clazz given class.
 * @return the constructor method id.
 */
jmethodID getConstructorMethodId(JNIEnv* env, jclass clazz);

/**
 * Get an {@link Arguments} structure,
 * from the given java List<AidaArgument>
 *
 * @param env environment.
 * @param jArgs java arguments list - List<AidaArgument>
 * @return {@link Arguments} structure
 */
Arguments toArguments(JNIEnv* env, jobject jArgs);

/**
 * Free up any memory allocated for the given arguments
 *
 * @param arguments the given arguments
 */
void releaseArguments(Arguments arguments);

/**
 * Free up any memory allocated the given scalar array
 *
 * @param array the given scalar array
 */
void releaseArray(Array array);

/**
 * Free up any memory allocated for string arrays
 * @param array
 */
void releaseStringArray(StringArray array);

/**
 * Free up any memory allocated for the given table
 *
 * @param table the given tables
 */
void releaseTable(Table table);

/**
 * Create a new instance of a java `boolean[]`,
 * from the {@link Array} of boolean primitives.
 *
 * @param env environment.
 * @param array {@link Array} of boolean primitives provided.
 * @return new java `boolean[]`.
 */
jbooleanArray toBooleanArray(JNIEnv* env, Array array);

/**
 * Create a new instance of a java `byte[]`,
 * from the {@link Array} of byte primitives.
 *
 * @param env environment.
 * @param array {@link Array} of byte primitives provided.
 * @return new java `byte[]`.
 */
jbyteArray toByteArray(JNIEnv* env, Array array);

/**
 * Create a new instance of a java short[],
 * from the {@link Array} of short primitives.
 *
 * @param env environment.
 * @param array {@link Array} of short primitives provided.
 * @return new java short[].
 */
jshortArray toShortArray(JNIEnv* env, Array array);

/**
 * Create a new instance of a java int[],
 * from the {@link Array} of integer primitives.
 *
 * @param env environment.
 * @param array {@link Array} of integer primitives provided.
 * @return new java int[].
 */
jintArray toIntegerArray(JNIEnv* env, Array array);

/**
 * Create a new instance of a java long[],
 * from the {@link Array} of long primitives.
 *
 * @param env environment.
 * @param array {@link Array} of long primitives provided.
 * @return new java long[].
 */
jlongArray toLongArray(JNIEnv* env, Array array);

/**
 * Create a new instance of a java float[],
 * from the {@link Array} of float primitives.
 *
 * @param env environment.
 * @param array {@link Array} of float primitives provided.
 * @return new java float[].
 */
jfloatArray toFloatArray(JNIEnv* env, Array array);

/**
 * Create a new instance of a java double[],
 * from the {@link Array} of double primitives.
 *
 * @param env environment.
 * @param array {@link Array} of double primitives provided.
 * @return new java double[].
 */
jdoubleArray toDoubleArray(JNIEnv* env, Array array);

/**
 * Create a new instance of a java String[],
 * from the {@link StringArray} of C string primitives.
 *
 * @param env environment.
 * @param array {@link StringArray} of C string primitives provided
 * @return new java String[]
 */
jobjectArray toStringArray(JNIEnv* env, StringArray array);

/**
 * Create a new instance of a java `List` of `List`s,
 * from the given `Table `structure.
 *
 * @param env environment.
 * @param table the {@link Table} provided
 * @return new java List of Lists
 */
jobject toTable(JNIEnv* env, Table table);

/**
 * Create a new instance of a java Boolean,
 * from the primitive boolean provided.
 *
 * @param env environment.
 * @param primitive primitive boolean provided
 * @return new java Boolean
 */
jobject toBoolean(JNIEnv* env, jboolean primitive);

/**
 * Create a new instance of a java Byte,
 * from the primitive byte provided.
 *
 * @param env environment.
 * @param primitive primitive byte provided
 * @return new java Byte
 */
jobject toByte(JNIEnv* env, jbyte primitive);

/**
 * Create a new instance of a java Short,
 * from the primitive short provided.
 *
 * @param env environment.
 * @param primitive primitive short provided
 * @return new java Short
 */
jobject toShort(JNIEnv* env, jshort primitive);

/**
 * Create a new instance of a java Integer,
 * from the primitive int provided.
 *
 * @param env environment.
 * @param primitive primitive int provided
 * @return new java Integer
 */
jobject toInteger(JNIEnv* env, jint primitive);

/**
 * Create a new instance of a java Long,
 * from the primitive long provided.
 *
 * @param env environment.
 * @param primitive primitive long provided
 * @return new java Long
 */
jobject toLong(JNIEnv* env, jlong primitive);

/**
 * Create a new instance of a java Float,
 * from the primitive float provided.
 *
 * @param env environment.
 * @param primitive primitive float provided
 * @return new java Float
 */
jobject toFloat(JNIEnv* env, jfloat primitive);

/**
 * Create a new instance of a java Double,
 * from the primitive double provided.
 *
 * @param env environment.
 * @param primitive primitive double provided
 * @return new java Double
 */
jobject toDouble(JNIEnv* env, jdouble primitive);

/**
 * Get value from the `value` argument,
 * in the provided {@link Arguments} structure,
 * when the value is a scalar.
 *
 * @param env environment.
 * @param arguments provided arguments structure
 * @return the extracted value
 */
Value getValue(JNIEnv* env, Arguments arguments);

/**
 * Get value from the `value` argument,
 * in the provided {@link Arguments} structure,
 * when the value is a scalar array.
 *
 * Even if the argument is not
 * an array it will create a one element array to put it in.
 *
 * @param env environment.
 * @param arguments provided arguments structure
 * @return the extracted value
 */
Value getArrayValue(JNIEnv* env, Arguments arguments);

/**
 * Release all allocated memory in the given value
 *
 * @param env environment.
 * @param value the given value'
 */
void releaseValue(Value value);

#ifdef __cplusplus
}
#endif
#endif

