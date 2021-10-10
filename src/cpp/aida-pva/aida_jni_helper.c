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
		jmethodID argumentGetValueMethod, jmethodID argumentsGetArgumentsMethod, jmethodID argumentsGetFloatArgumentsMethod, jmethodID argumentsGetDoubleArgumentsMethod,
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
 * Look up class in environment and create a new java object
 * @param env environment
 * @param class class to create
 * @return the new jni object
 */
JavaObject newObject(JNIEnv* env, char* classToCreate)
{
	JavaObject javaObject;
	javaObject.class = NULL;
	javaObject.object = NULL;

	// Get a class reference
	javaObject.class = (*env)->FindClass(env, classToCreate);
	if (!javaObject.class) {
		char errorString[BUFSIZ];
		sprintf(errorString, "Failed to create object of class: %s", classToCreate);
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
		return javaObject;
	}
	javaObject.object = newObjectFromClass(env, javaObject.class);

	return javaObject;
}

/**
 * Convert j-string to c-string
 *
 * @param env env
 * @param string j-string
 * @return c-string
 */
char* toCString(JNIEnv* env, jstring string)
{
	return (char*)(*env)->GetStringUTFChars(env, string, NULL);
}

/**
 * Convert c-string to j-string
 *
 * @param env env
 * @param string c-string
 * @return j-string
 */
jstring toJString(JNIEnv* env, const char* string)
{
	if (!string) {
		return NULL;
	}
	return (*env)->NewStringUTF(env, string);
}

/**
 * Get the method ID on the given class with the given method name and signature
 *
 * @param env env
 * @param cls given class
 * @param methodName given method name
 * @param methodSignature given signature
 * @return the method that matches the name and signature specified
 */
jmethodID getMethodId(JNIEnv* env, jclass cls, char* methodName, char* methodSignature)
{
	return (*env)->GetMethodID(env, cls, methodName, methodSignature);
}

/**
 * Get the method ID of the constructor of the given class
 *
 * @param env env
 * @param cls given class
 * @return the constructor method id
 */
jmethodID getConstructorMethodId(JNIEnv* env, jclass cls)
{
	return getMethodId(env, cls, "<init>", "()V");
}

/**
 * Get c arguments structure from a java arguments list - List<AidaArgument>
 *
 *
 * @param env env
 * @param jArgs java arguments list - List<AidaArgument> (name, value}
 * @return c arguments structure
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
			getFloatNameMethod, getFloatValueMethod,  getDoubleNameMethod, getDoubleValueMethod)) {
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
	int floatingPointValueIndex = 0;
	if (totalFloatingPoints > 0) {
		// Loop through the FloatArgument list
		for (int i = 0; i < floatCount; i++) {
			jobject floatArgument = (*env)->CallObjectMethod(env, jFloatsList, listGetMethod, i);
			jobject name = (*env)->CallObjectMethod(env, floatArgument, getFloatNameMethod);
			jfloat value = (*env)->CallFloatMethod(env, floatArgument, getFloatValueMethod);

			// Add key and value to Arguments.
			cArgs.arguments->floatingPointValues[floatingPointValueIndex].path = toCString(env, name);
			cArgs.arguments->floatingPointValues[floatingPointValueIndex++].value.floatValue = value;
		}

		for (int i = 0; i < doubleCount; i++) {
			jobject doubleArgument = (*env)->CallObjectMethod(env, jDoublesList, listGetMethod, i);
			jobject name = (*env)->CallObjectMethod(env, doubleArgument, getDoubleNameMethod);
			jdouble value = (*env)->CallDoubleMethod(env, doubleArgument, getDoubleValueMethod);

			// Add key and value to Arguments.
			cArgs.arguments->floatingPointValues[floatingPointValueIndex].path = toCString(env, name);
			cArgs.arguments->floatingPointValues[floatingPointValueIndex++].value.doubleValue = value;
		}
	}

	// Return arguments
	return cArgs;
}

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

static int checkMethods(JNIEnv* env,
		jmethodID listSizeMethod, jmethodID listGetMethod,
		jmethodID argumentGetNameMethod, jmethodID argumentGetValueMethod,
		jmethodID argumentsGetArgumentsMethod, jmethodID argumentsGetFloatArgumentsMethod, jmethodID argumentsGetDoubleArgumentsMethod,
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
		cArgs->arguments->floatingPointValues = calloc(totalFloatingPoints, sizeof(FloatingPointValue));
		if (!cArgs->arguments->floatingPointValues) {
			free(cArgs->arguments);
			aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION,
					"Failed to allocate memory for floating point values");
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

/**
 * Get value from the {@code Value} argument in the provided arguments structure.
 *
 * @param env env
 * @param arguments provided arguments structure
 * @return the extracted Value
 */
Value getValue(JNIEnv* env, Arguments arguments)
{
	return getNamedValue(env, arguments, "Value");
}

/**
 * Get an array value from the Value argument.  Even if the argument is not
 * an array it will create a one element array to put it in
 * @param env env
 * @param arguments provided arguments structure
 * @return the extracted array Value that will contain parsed json
 */
Value getArrayValue(JNIEnv* env, Arguments arguments)
{
	return getNamedArrayValue(env, arguments, "Value");
}

/**
 * Free the json value in a Value structure.
 * @param value
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
 * Create a java boolean array from a c-array
 *
 * @param env env
 * @param array c-array
 * @return java boolean array
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
 * Create a java byte array from a c-array
 *
 * @param env env
 * @param array c-array
 * @return java byte array
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
 * Create a java short array from a c-array
 *
 * @param env env
 * @param array c-array
 * @return java short array
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
 * Create a java integer array from a c-array
 *
 * @param env env
 * @param array c-array
 * @return java integer array
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
 * Create a java long array from a c-array
 *
 * @param env env
 * @param array c-array
 * @return java long array
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
 * Create a java float array from a c-array
 *
 * @param env env
 * @param array c-array
 * @return java float array
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
 * Create a java double array from a c-array
 *
 * @param env env
 * @param array c-array
 * @return java double array
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
 * Convert a string array to a java object array
 *
 * @param env env
 * @param array string array
 * @return jobject
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
 * Convert a table to a jobject for returning to java.
 * Tables are returned as lists of Lists so:-
 * we create a java ArrayList
 * then for each column in the table
 *   we create a sub list
 *   then we loop over each row to call add() to add entries to that list
 *   then we add() the sublist to the main list
 *
 * @param env env
 * @param table the table
 * @return jobject to return to java
 */
jobject toTable(JNIEnv* env, Table table)
{
	jobject tableToReturn;

	JavaObject listObject = newObject(env, "edu/stanford/slac/aida/lib/model/AidaTable");
	CHECK_EXCEPTION(NULL)

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
				CHECK_EXCEPTION(NULL)

				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_BYTE_ARRAY_TYPE: {
				jbyte data = ((jbyte*)(table.ppData[column]))[row];
				jobject dataObject = toByte(env, data);
				CHECK_EXCEPTION(NULL)

				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_SHORT_ARRAY_TYPE: {
				jshort data = ((jshort*)(table.ppData[column]))[row];
				jobject dataObject = toShort(env, data);
				CHECK_EXCEPTION(NULL)

				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_INTEGER_ARRAY_TYPE: {
				jint data = ((jint*)(table.ppData[column]))[row];
				jobject dataObject = toInteger(env, data);
				CHECK_EXCEPTION(NULL)

				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_LONG_ARRAY_TYPE: {
				jlong data = ((jlong*)(table.ppData[column]))[row];
				jobject dataObject = toLong(env, data);
				CHECK_EXCEPTION(NULL)

				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_FLOAT_ARRAY_TYPE: {
				jfloat data = ((jfloat*)(table.ppData[column]))[row];
				jobject dataObject = toFloat(env, data);
				CHECK_EXCEPTION(NULL)

				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_DOUBLE_ARRAY_TYPE: {
				jdouble data = ((jdouble*)(table.ppData[column]))[row];
				jobject dataObject = toDouble(env, data);
				CHECK_EXCEPTION(NULL)

				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_STRING_ARRAY_TYPE: {
				char* string = ((char**)(table.ppData[column]))[row];
				jstring stringValue = toJString(env, string);
				CHECK_EXCEPTION_FOR_STRING_COLUMN(NULL)

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
 * Free up any memory allocated for arguments
 * @param arguments
 */
void releaseArguments(Arguments arguments)
{
	if (arguments.argumentCount > 0 && arguments.arguments) {
		if (arguments.arguments->floatingPointValues) {
			free(arguments.arguments->floatingPointValues);
		}
		free(arguments.arguments);
	}
}

/**
 * Free up any memory allocated scalar arrays
 * @param array
 */
void releaseArray(Array array)
{
	if (array.count > 0 && array.items) {
		free(array.items);
	}
}

/**
 * Free up any memory allocated for string arrays
 * @param array
 */
void releaseStringArray(StringArray array)
{
	if (array.count > 0 && array.items) {
		free(array.items);
	}
}

/**
 * Free up any memory allocated for tables
 * @param table
 */
void releaseTable(Table table)
{
	if (table.ppData && table.types && table.columnCount > 0) {
		for (int column = 0; column < table.columnCount; column++) {
			if (table.ppData[column]) {
				free(table.ppData[column]);
			}
		}
		if (table.ppData) {
			free(table.ppData);
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
 * Create a new instance of a Java Boolean from the scalar
 * @param env env
 * @param data scalar
 * @return new Java Boolean
 */
jobject toBoolean(JNIEnv* env, jboolean data)
{
	ClassAndMethod classAndMethod = getClassAndValueOfMethod(env, "java/lang/Boolean", "(Z)Ljava/lang/Boolean;");
	CHECK_EXCEPTION(NULL)

	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, data);
	if (!dataObject) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to convert boolean to Boolean");
	}

	return dataObject;
}

/**
 * Create a new instance of a Java Byte from the scalar
 * @param env env
 * @param data scalar
 * @return new Java Byte
 */
jobject toByte(JNIEnv* env, jbyte data)
{
	ClassAndMethod classAndMethod = getClassAndValueOfMethod(env, "java/lang/Byte", "(B)Ljava/lang/Byte;");
	CHECK_EXCEPTION(NULL)

	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, data);
	if (!dataObject) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to convert byte to Byte");
	}

	return dataObject;
}

/**
 * Create a new instance of a Java Short from the scalar
 * @param env env
 * @param data scalar
 * @return new Java Short
 */
jobject toShort(JNIEnv* env, jshort data)
{
	ClassAndMethod classAndMethod = getClassAndValueOfMethod(env, "java/lang/Short", "(S)Ljava/lang/Short;");
	CHECK_EXCEPTION(NULL)

	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, data);
	if (!dataObject) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to convert short to Short");
	}

	return dataObject;
}

/**
 * Create a new instance of a Java Int from the scalar
 * @param env env
 * @param data scalar
 * @return new Java Integer
 */
jobject toInteger(JNIEnv* env, jint data)
{
	ClassAndMethod classAndMethod = getClassAndValueOfMethod(env, "java/lang/Integer", "(I)Ljava/lang/Integer;");
	CHECK_EXCEPTION(NULL)

	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, data);
	if (!dataObject) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to convert int to Integer");
	}

	return dataObject;
}

/**
 * Create a new instance of a Java Long from the scalar
 * @param env env
 * @param data scalar
 * @return new Java Long
 */
jobject toLong(JNIEnv* env, jlong data)
{
	ClassAndMethod classAndMethod = getClassAndValueOfMethod(env, "java/lang/Long", "(J)Ljava/lang/Long;");
	CHECK_EXCEPTION(NULL)

	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, data);
	if (!dataObject) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to convert long to Long");
	}

	return dataObject;
}

/**
 * Create a new instance of a Java Float from the scalar
 * @param env env
 * @param data scalar
 * @return new Java Float
 */
jobject toFloat(JNIEnv* env, jfloat data)
{
	ClassAndMethod classAndMethod = getClassAndValueOfMethod(env, "java/lang/Float", "(F)Ljava/lang/Float;");
	CHECK_EXCEPTION(NULL)

	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, data);
	if (!dataObject) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to convert float to Float");
	}

	return dataObject;
}

/**
 * Create a new instance of a Java Double from the scalar
 * @param env env
 * @param data scalar
 * @return new Java Double
 */
jobject toDouble(JNIEnv* env, jdouble data)
{
	ClassAndMethod classAndMethod = getClassAndValueOfMethod(env, "java/lang/Double", "(D)Ljava/lang/Double;");
	CHECK_EXCEPTION(NULL)

	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, data);
	if (!dataObject) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to convert double to Double");
	}

	return dataObject;
}


