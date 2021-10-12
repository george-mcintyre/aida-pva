#include <jni.h>
#include <memory.h>
#include <stdlib.h>
#include <ctype.h>
#include "aida_types.h"
#include "json.h"
#include "aida_server_helper.h"
#include "aida_jni_helper.h"

static int getArgumentClasses(JNIEnv* env, jclass* listClass, jclass* floatArgumentClass, jclass* doubleArgumentClass,
		jclass* aidaArgumentsClass, jclass* aidaArgumentClass);

static void
getArgumentClassMethods(JNIEnv* env,
		jclass listClass, jclass floatArgumentClass, jclass doubleArgumentClass,
		jclass aidaArgumentsClass, jclass aidaArgumentClass,
		jmethodID* listSizeMethod, jmethodID* listGetMethod,
		jmethodID* argumentsGetArgumentsMethod,
		jmethodID* argumentGetNameMethod, jmethodID* argumentGetValueMethod,
		jmethodID* argumentsGetFloatArgumentsMethod, jmethodID* argumentsGetDoubleArgumentsMethod,
		jmethodID* getFloatNameMethod, jmethodID* getFloatValueMethod,
		jmethodID* getDoubleNameMethod, jmethodID* getDoubleValueMethod);

static int checkMethods(JNIEnv* env, jmethodID listSizeMethod, jmethodID listGetMethod, jmethodID argumentGetNameMethod,
		jmethodID argumentGetValueMethod, jmethodID argumentsGetArgumentsMethod,
		jmethodID argumentsGetFloatArgumentsMethod, jmethodID argumentsGetDoubleArgumentsMethod,
		jmethodID getFloatNameMethod, jmethodID getFloatValueMethod, jmethodID getDoubleNameMethod,
		jmethodID getDoubleValueMethod);

static int allocateSpaceForArguments(JNIEnv* env, Arguments* cArgs, int totalFloatingPoints);

/**
 * Create a new java object
 * @param env environment
 * @param class class of object to create
 * @return the new jni object
 */
jobject newObjectFromClass(JNIEnv* env, jclass class)
{
	jobject object;

	// Get the Method ID of the no-args constructor
	jmethodID midInit = getConstructorMethodId(env, class);
	if (!midInit) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to create object");
		return NULL;
	}
	// Call back constructor to allocate a new instance
	object = (*env)->NewObject(env, class, midInit);

	return object;
}

/**
 * Look up class in the given `env`,
 * and create a new java object.
 *
 * @param env environment.
 * @param clazz string name of java class to create.
 * @return the new java object.
 */
JavaObject newObject(JNIEnv* env, char* clazz)
{
	JavaObject javaObject;
	javaObject.class = NULL;
	javaObject.object = NULL;

	// Get a class reference
	javaObject.class = (*env)->FindClass(env, clazz);
	if (!javaObject.class) {
		char errorString[BUFSIZ];
		sprintf(errorString, "Failed to create object of class: %s", clazz);
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
		return javaObject;
	}
	javaObject.object = newObjectFromClass(env, javaObject.class);

	return javaObject;
}

/**
 * Convert given `jstring` to a C string.
 *
 * @param env environment.
 * @param string `jstring`.
 * @return C string.
 */
char* toCString(JNIEnv* env, jstring string)
{
	return (char*)(*env)->GetStringUTFChars(env, string, NULL);
}

/**
 * Convert C string to `jstring`.
 *
 * @param env environment.
 * @param string C string.
 * @return `jstring`.
 */
jstring toJString(JNIEnv* env, const char* string)
{
	if (!string) {
		return NULL;
	}
	return (*env)->NewStringUTF(env, string);
}

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
jmethodID getMethodId(JNIEnv* env, jclass clazz, char* methodName, char* methodSignature)
{
	return (*env)->GetMethodID(env, clazz, methodName, methodSignature);
}

/**
 * Get the method ID of the constructor of the given class.
 *
 * @param env environment.
 * @param clazz given class.
 * @return the constructor method id.
 */
jmethodID getConstructorMethodId(JNIEnv* env, jclass clazz)
{
	return getMethodId(env, clazz, "<init>", "()V");
}

/**
 * Get an `Arguments` structure,
 * from the given java List<AidaArgument>
 *
 * @param env environment.
 * @param jArgs java arguments list - List<AidaArgument>
 * @return `Arguments` structure
 */
Arguments toArguments(JNIEnv* env, jobject jArguments)
{
	Arguments cArgs;
	cArgs.argumentCount = 0;

	// Get all classes needed for processing arguments
	jclass listClass, floatArgumentClass, doubleArgumentClass, aidaArgumentsClass, aidaArgumentClass;
	if (getArgumentClasses(env, &listClass, &floatArgumentClass, &doubleArgumentClass, &aidaArgumentsClass,
			&aidaArgumentClass)) {
		return cArgs;
	}

	// Get class methods
	jmethodID listSizeMethod, listGetMethod,
			argumentGetNameMethod, argumentGetValueMethod,
			argumentsGetArgumentsMethod, argumentsGetFloatArgumentsMethod, argumentsGetDoubleArgumentsMethod,
			getFloatNameMethod, getFloatValueMethod,
			getDoubleNameMethod, getDoubleValueMethod;

	getArgumentClassMethods(env,
			listClass, floatArgumentClass, doubleArgumentClass,
			aidaArgumentsClass, aidaArgumentClass,
			&listSizeMethod, &listGetMethod,
			&argumentsGetArgumentsMethod,
			&argumentGetNameMethod, &argumentGetValueMethod,
			&argumentsGetFloatArgumentsMethod, &argumentsGetDoubleArgumentsMethod,
			&getFloatNameMethod, &getFloatValueMethod,
			&getDoubleNameMethod, &getDoubleValueMethod);

	// Check that the methods were retrieved correctly
	if (checkMethods(env,
			listSizeMethod, listGetMethod,
			argumentGetNameMethod, argumentGetValueMethod,
			argumentsGetArgumentsMethod, argumentsGetFloatArgumentsMethod, argumentsGetDoubleArgumentsMethod,
			getFloatNameMethod, getFloatValueMethod, getDoubleNameMethod, getDoubleValueMethod)) {
		return cArgs;
	}

	// Get the arguments list
	jobject argumentsList = (*env)->CallObjectMethod(env, jArguments, argumentsGetArgumentsMethod);

	// get the size of the list of arguments
	cArgs.argumentCount = (*env)->CallIntMethod(env, argumentsList, listSizeMethod);

	// Get list of floats and doubles
	jobject jFloatsList = (*env)->CallObjectMethod(env, jArguments, argumentsGetFloatArgumentsMethod);
	jobject jDoublesList = (*env)->CallObjectMethod(env, jArguments, argumentsGetDoubleArgumentsMethod);

	// Get the size of the list of floats and doubles
	int floatCount = jFloatsList ? (*env)->CallIntMethod(env, jFloatsList, listSizeMethod) : 0;
	int doubleCount = jDoublesList ? (*env)->CallIntMethod(env, jDoublesList, listSizeMethod) : 0;
	int totalFloatingPoints = floatCount + doubleCount;

	// Create space for arguments, allocates space for arguments and the
	// array of floats / doubles and space for then float/double path names
	if (allocateSpaceForArguments(env, &cArgs, totalFloatingPoints)) {
		return cArgs;
	}

	// walk through and fill array
	for (int i = 0; i < cArgs.argumentCount; i++) {
		jobject argument = (*env)->CallObjectMethod(env, argumentsList, listGetMethod, i);
		cArgs.arguments[i].name = toCString(env, (*env)->CallObjectMethod(env, argument, argumentGetNameMethod));
		cArgs.arguments[i].value = toCString(env, (*env)->CallObjectMethod(env, argument, argumentGetValueMethod));
	}

	// If any floats or doubles add them to the allocated space
	cArgs.floatingPointValuesCountCount = 0;
	if (totalFloatingPoints > 0) {
		// Loop through the FloatArgument list
		for (int i = 0; i < floatCount; i++) {
			jobject floatArgument = (*env)->CallObjectMethod(env, jFloatsList, listGetMethod, i);
			jobject name = (*env)->CallObjectMethod(env, floatArgument, getFloatNameMethod);
			jfloat value = (*env)->CallFloatMethod(env, floatArgument, getFloatValueMethod);

			// Add key and value to Arguments.
			cArgs.floatingPointValues[cArgs.floatingPointValuesCountCount].path = toCString(env, name);
			cArgs.floatingPointValues[cArgs.floatingPointValuesCountCount++].value.floatValue = value;
		}

		for (int i = 0; i < doubleCount; i++) {
			jobject doubleArgument = (*env)->CallObjectMethod(env, jDoublesList, listGetMethod, i);
			jobject name = (*env)->CallObjectMethod(env, doubleArgument, getDoubleNameMethod);
			jdouble value = (*env)->CallDoubleMethod(env, doubleArgument, getDoubleValueMethod);

			// Add key and value to Arguments.
			cArgs.floatingPointValues[cArgs.floatingPointValuesCountCount].path = toCString(env, name);
			cArgs.floatingPointValues[cArgs.floatingPointValuesCountCount++].value.doubleValue = value;
		}
	}

	// Return arguments
	return cArgs;
}

/**
 * Get java classes that will be used to process the arguments structure
 * @param env
 * @param listClass
 * @param floatArgumentClass
 * @param doubleArgumentClass
 * @param aidaArgumentsClass
 * @param aidaArgumentClass
 * @return
 */
static int getArgumentClasses(JNIEnv* env, jclass* listClass, jclass* floatArgumentClass, jclass* doubleArgumentClass,
		jclass* aidaArgumentsClass, jclass* aidaArgumentClass)
{
	// retrieve the java.util.List interface class
	*listClass = (*env)->FindClass(env, "java/util/List");
	if (!*listClass) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, "Failed to get List class");
		return EXIT_FAILURE;
	}

	// retrieve the FloatArgument class
	*floatArgumentClass = (*env)->FindClass(env, "edu/stanford/slac/aida/lib/model/FloatArgument");
	if (!*floatArgumentClass) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, "Failed to get FloatArgument class");
		return EXIT_FAILURE;
	}

	// retrieve the DoubleArgument class
	*doubleArgumentClass = (*env)->FindClass(env, "edu/stanford/slac/aida/lib/model/DoubleArgument");
	if (!*doubleArgumentClass) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, "Failed to get DoubleArgument class");
		return EXIT_FAILURE;
	}

	// retrieve the AidaArguments class
	*aidaArgumentsClass = (*env)->FindClass(env, "edu/stanford/slac/aida/lib/model/AidaArguments");
	if (!*aidaArgumentsClass) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, "Failed to get AidaArguments class");
		return EXIT_FAILURE;
	}

	// retrieve the AidaArgument class
	*aidaArgumentClass = (*env)->FindClass(env, "edu/stanford/slac/aida/lib/model/AidaArgument");
	if (!*aidaArgumentClass) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, "Failed to get AidaArgument class");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

/**
 * Get the method IDs of the java methods needed to process the arguments structure
 * @param env
 * @param listClass
 * @param floatArgumentClass
 * @param doubleArgumentClass
 * @param aidaArgumentsClass
 * @param aidaArgumentClass
 * @param listSizeMethod
 * @param listGetMethod
 * @param argumentsGetArgumentsMethod
 * @param argumentGetNameMethod
 * @param argumentGetValueMethod
 * @param argumentsGetFloatArgumentsMethod
 * @param argumentsGetDoubleArgumentsMethod
 * @param getFloatNameMethod
 * @param getFloatValueMethod
 * @param getDoubleNameMethod
 * @param getDoubleValueMethod
 */
static void
getArgumentClassMethods(JNIEnv* env,
		jclass listClass, jclass floatArgumentClass, jclass doubleArgumentClass,
		jclass aidaArgumentsClass, jclass aidaArgumentClass,
		jmethodID* listSizeMethod, jmethodID* listGetMethod,
		jmethodID* argumentsGetArgumentsMethod,
		jmethodID* argumentGetNameMethod, jmethodID* argumentGetValueMethod,
		jmethodID* argumentsGetFloatArgumentsMethod, jmethodID* argumentsGetDoubleArgumentsMethod,
		jmethodID* getFloatNameMethod, jmethodID* getFloatValueMethod,
		jmethodID* getDoubleNameMethod, jmethodID* getDoubleValueMethod)
{
	// retrieve the size and the get methods of list
	(*listSizeMethod) = (*env)->GetMethodID(env, listClass, "size", "()I");
	(*listGetMethod) = (*env)->GetMethodID(env, listClass, "get", "(I)Ljava/lang/Object;");

	// retrieve the getName and the getValue methods
	(*argumentGetNameMethod) = (*env)->GetMethodID(env, aidaArgumentClass, "getName", "()Ljava/lang/String;");
	(*argumentGetValueMethod) = (*env)->GetMethodID(env, aidaArgumentClass, "getValue", "()Ljava/lang/String;");

	// retrieve the getArguments, getFloats and the getDoubles methods from AidaArguments
	(*argumentsGetArgumentsMethod) = (*env)
			->GetMethodID(env, aidaArgumentsClass, "getArguments", "()Ljava/util/List;");
	(*argumentsGetFloatArgumentsMethod) = (*env)
			->GetMethodID(env, aidaArgumentsClass, "getFloatArguments", "()Ljava/util/List;");
	(*argumentsGetDoubleArgumentsMethod) = (*env)
			->GetMethodID(env, aidaArgumentsClass, "getDoubleArguments", "()Ljava/util/List;");

	// get float and double getters from their boxed classes
	(*getFloatNameMethod) = (*env)->GetMethodID(env, floatArgumentClass, "getName", "()Ljava/lang/String;");
	(*getFloatValueMethod) = (*env)->GetMethodID(env, floatArgumentClass, "getValue", "()F");
	(*getDoubleNameMethod) = (*env)->GetMethodID(env, doubleArgumentClass, "getName", "()Ljava/lang/String;");
	(*getDoubleValueMethod) = (*env)->GetMethodID(env, doubleArgumentClass, "getValue", "()D");
}

/**
 * Check that all the java methods needed to process the arguments structure were obtained successfully
 * @param env
 * @param listSizeMethod
 * @param listGetMethod
 * @param argumentGetNameMethod
 * @param argumentGetValueMethod
 * @param argumentsGetArgumentsMethod
 * @param argumentsGetFloatArgumentsMethod
 * @param argumentsGetDoubleArgumentsMethod
 * @param getFloatNameMethod
 * @param getFloatValueMethod
 * @param getDoubleNameMethod
 * @param getDoubleValueMethod
 * @return
 */
static int checkMethods(JNIEnv* env,
		jmethodID listSizeMethod, jmethodID listGetMethod,
		jmethodID argumentGetNameMethod, jmethodID argumentGetValueMethod,
		jmethodID argumentsGetArgumentsMethod, jmethodID argumentsGetFloatArgumentsMethod,
		jmethodID argumentsGetDoubleArgumentsMethod,
		jmethodID getFloatNameMethod, jmethodID getFloatValueMethod,
		jmethodID getDoubleNameMethod, jmethodID getDoubleValueMethod)
{
	if (!listSizeMethod) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION,
				"Failed to get size(String) method on List object");
		return EXIT_FAILURE;
	}
	if (!listGetMethod) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, "Failed to get get(int) method on List object");
		return EXIT_FAILURE;
	}
	if (!argumentGetNameMethod) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION,
				"Failed to get getName() method on AidaArgument object");
		return EXIT_FAILURE;
	}
	if (!argumentGetValueMethod) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION,
				"Failed to get getValue() method on AidaArgument object");
		return EXIT_FAILURE;
	}

	if (!argumentsGetArgumentsMethod) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION,
				"Failed to get getArguments() method on AidaArguments object");
		return EXIT_FAILURE;
	}

	if (!argumentsGetFloatArgumentsMethod) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION,
				"Failed to get getFloatArguments() method on AidaArguments object");
		return EXIT_FAILURE;
	}

	if (!argumentsGetDoubleArgumentsMethod) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION,
				"Failed to get getDoubleArguments() method on AidaArguments object");
		return EXIT_FAILURE;
	}

	if (!getFloatNameMethod) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION,
				"Failed to get getName() method on FloatValue object");
		return EXIT_FAILURE;
	}

	if (!getFloatValueMethod) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION,
				"Failed to get getValue() method on FloatValue object");
		return EXIT_FAILURE;
	}

	if (!getDoubleNameMethod) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION,
				"Failed to get getName() method on DoubleValue object");
		return EXIT_FAILURE;
	}

	if (!getDoubleValueMethod) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION,
				"Failed to get getValue() method on DoubleValue object");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/**
 * Allocate space for arguments including space for floating point numbers
 *
 * @param env
 * @param cArgs
 * @param totalFloatingPoints
 * @return
 */
static int allocateSpaceForArguments(JNIEnv* env, Arguments* cArgs, int totalFloatingPoints)
{
	// Create array of arguments
	cArgs->arguments = calloc(cArgs->argumentCount, sizeof(Argument));
	if (!cArgs->arguments) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION,
				"Failed to allocate memory for arguments");
		return EXIT_FAILURE;
	}

	// Create space for floating point numbers
	if (totalFloatingPoints > 0) {
		cArgs->floatingPointValues = calloc(totalFloatingPoints, sizeof(FloatingPointValue));
		if (!cArgs->floatingPointValues) {
			free(cArgs->arguments);
			aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION,
					"Failed to allocate memory for floating point values");
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

/**
 * Get value from the `value` argument,
 * in the provided `arguments` structure,
 * when the value is a scalar.
 *
 * @param env environment.
 * @param arguments provided arguments structure
 * @return the extracted value
 */
Value getValue(JNIEnv* env, Arguments arguments)
{
	return getNamedValue(env, arguments, "Value");
}

/**
 * Get value from the `value` argument,
 * in the provided `arguments` structure,
 * when the value is a scalar array.
 *
 * Even if the argument is not
 * an array it will create a one element array to put it in.
 *
 * @param env environment.
 * @param arguments provided arguments structure
 * @return the extracted value
 */
Value getArrayValue(JNIEnv* env, Arguments arguments)
{
	return getNamedArrayValue(env, arguments, "Value");
}

/**
 * Release all allocated memory in the given value
 *
 * @param env environment.
 * @param value the given value'
 */
void releaseValue(Value value)
{
	// Only free json values because the string values will be freed with the arguments directly
	if (value.type == AIDA_JSON_TYPE) {
		json_value_free(value.value.jsonValue);
		value.type = AIDA_NO_TYPE;
	}
}

/**
 * Create a new instance of a java `boolean[]`,
 * from the `Array` of boolean primitives.
 *
 * @param env environment.
 * @param array `Array` of boolean primitives provided.
 * @return new java `boolean[]`.
 */
jbooleanArray toBooleanArray(JNIEnv* env, Array array)
{
	jbooleanArray returnValue;

	returnValue = (*env)->NewBooleanArray(env, array.count);
	if (!returnValue) {
		char errorString[BUFSIZ];
		sprintf(errorString, "Failed to create a new Boolean Array with %d elements", array.count);
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
		return NULL;
	}

	// Copy values
	(*env)->SetBooleanArrayRegion(env, returnValue, 0, array.count, array.items);

	// Free up array
	releaseArray(array);
	return returnValue;
}

/**
 * Create a new instance of a java `byte[]`,
 * from the `Array` of byte primitives.
 *
 * @param env environment.
 * @param array `Array` of byte primitives provided.
 * @return new java `byte[]`.
 */
jbyteArray toByteArray(JNIEnv* env, Array array)
{
	jbyteArray returnValue;

	returnValue = (*env)->NewByteArray(env, array.count);
	if (!returnValue) {
		char errorString[BUFSIZ];
		sprintf(errorString, "Failed to create a new Byte Array with %d elements", array.count);
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
		return NULL;
	}

	// Copy values
	(*env)->SetByteArrayRegion(env, returnValue, 0, array.count, array.items);

	// Free up array
	releaseArray(array);

	return returnValue;
}

/**
 * Create a new instance of a java `short[]`,
 * from the `Array` of short primitives.
 *
 * @param env environment.
 * @param array `Array` of short primitives provided.
 * @return new java `short[]`.
 */
jshortArray toShortArray(JNIEnv* env, Array array)
{
	jshortArray returnValue;

	returnValue = (*env)->NewShortArray(env, array.count);
	if (!returnValue) {
		char errorString[BUFSIZ];
		sprintf(errorString, "Failed to create a new Short Array with %d elements", array.count);
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
		return NULL;
	}

	// Copy values
	(*env)->SetShortArrayRegion(env, returnValue, 0, array.count, array.items);

	// Free up array
	releaseArray(array);

	return returnValue;
}

/**
 * Create a new instance of a java `int[]`,
 * from the `Array` of integer primitives.
 *
 * @param env environment.
 * @param array `Array` of integer primitives provided.
 * @return new java `int[]`.
 */
jintArray toIntegerArray(JNIEnv* env, Array array)
{
	jintArray returnValue;

	returnValue = (*env)->NewIntArray(env, array.count);
	if (!returnValue) {
		char errorString[BUFSIZ];
		sprintf(errorString, "Failed to create a new Integer Array with %d elements", array.count);
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
		return NULL;
	}

	// Copy values
	(*env)->SetIntArrayRegion(env, returnValue, 0, array.count, array.items);

	// Free up array
	releaseArray(array);

	return returnValue;
}

/**
 * Create a new instance of a java `long[]`,
 * from the `Array` of long primitives.
 *
 * @param env environment.
 * @param array `Array` of long primitives provided.
 * @return new java `long[]`.
 */
jlongArray toLongArray(JNIEnv* env, Array array)
{
	jlongArray returnValue;

	returnValue = (*env)->NewLongArray(env, array.count);
	if (!returnValue) {
		char errorString[BUFSIZ];
		sprintf(errorString, "Failed to create a new Long Array with %d elements", array.count);
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
		return NULL;
	}

	// Copy values
	(*env)->SetLongArrayRegion(env, returnValue, 0, array.count, array.items);

	// Free up array
	releaseArray(array);

	return returnValue;
}

/**
 * Create a new instance of a java `float[]`,
 * from the `Array` of float primitives.
 *
 * @param env environment.
 * @param array `Array` of float primitives provided.
 * @return new java `float[]`.
 */
jfloatArray toFloatArray(JNIEnv* env, Array array)
{
	jfloatArray returnValue;

	returnValue = (*env)->NewFloatArray(env, array.count);
	if (!returnValue) {
		char errorString[BUFSIZ];
		sprintf(errorString, "Failed to create a new Float Array with %d elements", array.count);
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
		return NULL;
	}

	// Copy values
	(*env)->SetFloatArrayRegion(env, returnValue, 0, array.count, array.items);

	// Free up array
	releaseArray(array);

	return returnValue;
}

/**
 * Create a new instance of a java `double[]`,
 * from the `Array` of double primitives.
 *
 * @param env environment.
 * @param array `Array` of double primitives provided.
 * @return new java `double[]`.
 */
jdoubleArray toDoubleArray(JNIEnv* env, Array array)
{
	jdoubleArray returnValue;

	// create result array
	returnValue = (*env)->NewDoubleArray(env, array.count);
	if (!returnValue) {
		char errorString[BUFSIZ];
		sprintf(errorString, "Failed to create a new Double Array with %d elements", array.count);
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
		return NULL;
	}

	// Copy values
	(*env)->SetDoubleArrayRegion(env, returnValue, 0, array.count, array.items);

	// Free up array
	releaseArray(array);

	return returnValue;
}

/**
 * Create a new instance of a java `String[]`,
 * from the `StringArray` of C string primitives.
 *
 * @param env environment.
 * @param array `StringArray` of C string primitives provided
 * @return new java `String[]`
 */
jobjectArray toStringArray(JNIEnv* env, StringArray array)
{
	jobjectArray returnValue;

	// Get a class reference for java.lang.String
	jclass classString = (*env)->FindClass(env, "java/lang/String");
	if (!classString) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to get jclass of java String");
		return NULL;
	}

	returnValue = (*env)->NewObjectArray(env, array.count, classString, NULL);
	if (!returnValue) {
		releaseStringArray(array);
		char errorString[BUFSIZ];
		sprintf(errorString, "Failed to create a new String Array with %d elements", array.count);
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
		return NULL;
	}

	// Copy values
	for (int i = 0; i < array.count; i++) {
		(*env)->SetObjectArrayElement(env, returnValue, i, toJString(env, array.items[i]));
	}

	// Free up array
	releaseStringArray(array);

	return returnValue;
}

/**
 * Create a new instance of a java `List` of `List`s,
 * from the given `Table `structure.
 *
 * Tables are returned as lists of Lists so:-
 * we create a java `ArrayList`
 * then for each column in the table
 *   we create a sub list
 *   then we loop over each row to call `add()` to add entries to that list
 *   then we `add()` the sublist to the main list
 *
 * @param env environment.
 * @param table the `Table` provided
 * @return new java List of Lists
 */
jobject toTable(JNIEnv* env, Table table)
{
	jobject tableToReturn;

	JavaObject listObject = newObject(env, "edu/stanford/slac/aida/lib/model/AidaTable");
	CHECK_EXCEPTION_AND_RETURN_(NULL)

	tableToReturn = listObject.object;
	jclass cList = listObject.class;

	// retrieve the add method of the list
	jmethodID mAdd = (*env)->GetMethodID(env, cList, "add", "(ILjava/lang/Object;)Z");
	if (!mAdd) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"Failed to find the add(int, Object) method on AidaTable object");
		return NULL;
	}

	if (!tableToReturn) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to create a new AidaTable object");
		return NULL;
	}

	// Loop over each column
	for (int column = 0; column < table.columnCount; column++) {
		// loop over each row
		for (int row = 0; row < table.rowCount; row++) {
			switch (table.types[column]) {
			case AIDA_BOOLEAN_ARRAY_TYPE : {
				jboolean data = ((jboolean*)(table.ppData[column]))[row];
				jobject dataObject = toBoolean(env, data);
				CHECK_EXCEPTION_AND_RETURN_(NULL)

				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_BYTE_ARRAY_TYPE: {
				jbyte data = ((jbyte*)(table.ppData[column]))[row];
				jobject dataObject = toByte(env, data);
				CHECK_EXCEPTION_AND_RETURN_(NULL)

				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_SHORT_ARRAY_TYPE: {
				jshort data = ((jshort*)(table.ppData[column]))[row];
				jobject dataObject = toShort(env, data);
				CHECK_EXCEPTION_AND_RETURN_(NULL)

				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_INTEGER_ARRAY_TYPE: {
				jint data = ((jint*)(table.ppData[column]))[row];
				jobject dataObject = toInteger(env, data);
				CHECK_EXCEPTION_AND_RETURN_(NULL)

				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_LONG_ARRAY_TYPE: {
				jlong data = ((jlong*)(table.ppData[column]))[row];
				jobject dataObject = toLong(env, data);
				CHECK_EXCEPTION_AND_RETURN_(NULL)

				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_FLOAT_ARRAY_TYPE: {
				jfloat data = ((jfloat*)(table.ppData[column]))[row];
				jobject dataObject = toFloat(env, data);
				CHECK_EXCEPTION_AND_RETURN_(NULL)

				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_DOUBLE_ARRAY_TYPE: {
				jdouble data = ((jdouble*)(table.ppData[column]))[row];
				jobject dataObject = toDouble(env, data);
				CHECK_EXCEPTION_AND_RETURN_(NULL)

				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_STRING_ARRAY_TYPE: {
				char* string = ((char**)(table.ppData[column]))[row];
				jstring stringValue = toJString(env, string);
				CHECK_EXCEPTION_FREE_STRING_AND_RETURN_(NULL)

				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, stringValue);

				// Free up string buffer
				free(string);
				(*env)->DeleteLocalRef(env, stringValue);
				break;
			}
			default:
				aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
						"Unsupported type found in table.  Perhaps you declared a table with n columns but didnt add n columns");
				return NULL;
			}
		}
	}

	releaseTable(table);

	return tableToReturn;
}

/**
 * Free up any memory allocated for the given arguments
 *
 * @param arguments the given arguments
 */
void releaseArguments(Arguments arguments)
{
	if (arguments.floatingPointValues && arguments.floatingPointValuesCountCount) {
		free(arguments.floatingPointValues);
		arguments.floatingPointValuesCountCount = 0;
		arguments.floatingPointValues = NULL;
	}
	if (arguments.argumentCount && arguments.arguments) {
		free(arguments.arguments);
		arguments.argumentCount = 0;
		arguments.arguments = NULL;
	}
}

/**
 * Free up any memory allocated the given scalar array
 *
 * @param array the given scalar array
 */
void releaseArray(Array array)
{
	if (array.count && array.items) {
		free(array.items);
		array.count = 0;
		array.items = NULL;
	}
}

/**
 * Free up any memory allocated for string arrays
 * @param array
 */
void releaseStringArray(StringArray array)
{
	if (array.count && array.items) {
		free(array.items);
		array.count = 0;
		array.items = NULL;
	}
}

/**
 * Free up any memory allocated for the given table
 *
 * @param table the given tables
 */
void releaseTable(Table table)
{
	if (table.ppData && table.columnCount) {
		for (int column = 0; column < table.columnCount; column++) {
			if (table.ppData[column]) {
				free(table.ppData[column]);
				table.ppData[column] = NULL;
			}
		}
		table.columnCount = 0;

		free(table.ppData);
		table.ppData = NULL;

		if (table.types) {
			free(table.types);
			table.types = NULL;
		}
	}
}

/**
 * Get the class and the valueOf() method for the given signature
 *
 * @param env
 * @param boxedClassSignature
 * @param valueOfMethodSignature
 * @return
 */
ClassAndMethod getClassAndValueOfMethod(JNIEnv* env, char* boxedClassSignature, char* valueOfMethodSignature)
{
	ClassAndMethod classAndMethod;

	// Get a class reference
	classAndMethod.class = (*env)->FindClass(env, boxedClassSignature);
	if (!classAndMethod.class) {
		char errorString[BUFSIZ];
		sprintf(errorString, "Failed to get class of: %s", boxedClassSignature);
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
		return classAndMethod;
	}

	// retrieve the valueOf method
	classAndMethod.methodId = (*env)->GetStaticMethodID(env, classAndMethod.class, "valueOf", valueOfMethodSignature);
	if (!classAndMethod.methodId) {
		char errorString[BUFSIZ];
		sprintf(errorString, "Failed to valueOf method with signature: %s", valueOfMethodSignature);
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
		return classAndMethod;
	}

	return classAndMethod;
}

/**
 * Create a new instance of a java `Boolean`,
 * from the primitive boolean provided.
 *
 * @param env environment.
 * @param primitive primitive boolean provided
 * @return new java Boolean
 */
jobject toBoolean(JNIEnv* env, jboolean primitive)
{
	ClassAndMethod classAndMethod = getClassAndValueOfMethod(env, "java/lang/Boolean", "(Z)Ljava/lang/Boolean;");
	CHECK_EXCEPTION_AND_RETURN_(NULL)

	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, primitive);
	if (!dataObject) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to convert boolean to Boolean");
	}

	return dataObject;
}

/**
 * Create a new instance of a java `Byte`,
 * from the primitive byte provided.
 *
 * @param env environment.
 * @param primitive primitive byte provided
 * @return new java Byte
 */
jobject toByte(JNIEnv* env, jbyte primitive)
{
	ClassAndMethod classAndMethod = getClassAndValueOfMethod(env, "java/lang/Byte", "(B)Ljava/lang/Byte;");
	CHECK_EXCEPTION_AND_RETURN_(NULL)

	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, primitive);
	if (!dataObject) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to convert byte to Byte");
	}

	return dataObject;
}

/**
 * Create a new instance of a java `Short`,
 * from the primitive short provided.
 *
 * @param env environment.
 * @param primitive primitive short provided
 * @return new java Short
 */
jobject toShort(JNIEnv* env, jshort primitive)
{
	ClassAndMethod classAndMethod = getClassAndValueOfMethod(env, "java/lang/Short", "(S)Ljava/lang/Short;");
	CHECK_EXCEPTION_AND_RETURN_(NULL)

	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, primitive);
	if (!dataObject) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to convert short to Short");
	}

	return dataObject;
}

/**
 * Create a new instance of a java `Integer`,
 * from the primitive int provided.
 *
 * @param env environment.
 * @param primitive primitive int provided
 * @return new java `Integer`
 */
jobject toInteger(JNIEnv* env, jint primitive)
{
	ClassAndMethod classAndMethod = getClassAndValueOfMethod(env, "java/lang/Integer", "(I)Ljava/lang/Integer;");
	CHECK_EXCEPTION_AND_RETURN_(NULL)

	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, primitive);
	if (!dataObject) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to convert int to Integer");
	}

	return dataObject;
}

/**
 * Create a new instance of a java `Long`,
 * from the primitive long provided.
 *
 * @param env environment.
 * @param primitive primitive long provided
 * @return new java `Long`
 */
jobject toLong(JNIEnv* env, jlong primitive)
{
	ClassAndMethod classAndMethod = getClassAndValueOfMethod(env, "java/lang/Long", "(J)Ljava/lang/Long;");
	CHECK_EXCEPTION_AND_RETURN_(NULL)

	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, primitive);
	if (!dataObject) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to convert long to Long");
	}

	return dataObject;
}

/**
 * Create a new instance of a java `Float`,
 * from the primitive float provided.
 *
 * @param env environment.
 * @param primitive primitive float provided
 * @return new java `Float`
 */
jobject toFloat(JNIEnv* env, jfloat primitive)
{
	ClassAndMethod classAndMethod = getClassAndValueOfMethod(env, "java/lang/Float", "(F)Ljava/lang/Float;");
	CHECK_EXCEPTION_AND_RETURN_(NULL)

	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, primitive);
	if (!dataObject) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to convert float to Float");
	}

	return dataObject;
}

/**
 * Create a new instance of a java `Double`,
 * from the primitive double provided.
 *
 * @param env environment.
 * @param primitive primitive double provided
 * @return new java `Double`
 */
jobject toDouble(JNIEnv* env, jdouble primitive)
{
	ClassAndMethod classAndMethod = getClassAndValueOfMethod(env, "java/lang/Double", "(D)Ljava/lang/Double;");
	CHECK_EXCEPTION_AND_RETURN_(NULL)

	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, primitive);
	if (!dataObject) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to convert double to Double");
	}

	return dataObject;
}


