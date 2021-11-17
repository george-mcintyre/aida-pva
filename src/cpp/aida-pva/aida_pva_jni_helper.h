/** @file
 *  @brief The Header File for the JNI helper functions.
 *       **CMS**=C_INC
 */
#ifndef aida_pva_jni_helper_h
#define aida_pva_jni_helper_h
#ifdef __cplusplus
extern "C" {
#endif

#include "aida_pva.h"

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
 * Convert given jstring to a C string.
 *
 * @param env environment.
 * @param string jstring.
 * @return C string.
 */
char* toCString(JNIEnv* env, jstring string);

/**
 * Convert C string to jstring.
 *
 * @param env environment.
 * @param string C string.
 * @return jstring.
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
 * @param jArguments java arguments list - List<{@link AidaArgument}>
 * @return {@link AidaArgument} structure
 */
Arguments toArguments(JNIEnv* env, jobject jArguments);

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

#ifdef __cplusplus
}
#endif
#endif

