#include <jni.h>
#include <memory.h>
#include <stdlib.h>
#include <ctype.h>
#include "aida_types.h"
#include "json.h"
#include "aida_server_helper.h"
#include "aida_jni_helper.h"

/**
 * Empty config block for null comparison.  Static variables are automatically initialised to zero
 */
static const Config EMPTY_CONFIG = { 0 };

/**
 * Get an aida channel config jni object from Config.
 * This mutates the config object, freeing up any fields so it can't be used anymore afterwards
 *
 * @param env env
 * @param config config
 * @return aida channel config jni object
 */
jobject aidaChannelConfigToJObject(JNIEnv* env, Config config)
{
	if (!memcmp(&config, &EMPTY_CONFIG, sizeof config)) {
		return NULL;
	}

	// Get a java object reference
	JavaObject javaObject = newObject(env, "edu/stanford/slac/aida/lib/model/AidaChannelConfig");
	CHECK_EXCEPTION(NULL)

	jobject configObject = javaObject.object;
	jclass cls = javaObject.class;
	if (!configObject || !cls) {
		aidaThrowNonOsException(env, SERVER_INITIALISATION_EXCEPTION, "Failed to create AidaChannelConfig object");
		return NULL;
	}

	////////////
	// Set fields appropriately
	////////////

	// Set type, layout, & description
	callSetterWithJString(env, javaObject, "setType", toTypeString(env, config.type));
	CHECK_EXCEPTION(NULL)

	callSetterWithJString(env, javaObject, "setLayout", toLayoutString(env, config.layout));
	CHECK_EXCEPTION(NULL)

	callSetterWithString(env, javaObject, "setDescription", config.description);
	CHECK_EXCEPTION(NULL)


	// Set fields ( and free up storage )
	if (config.fields && config.fieldCount) {
		// Find the method to add fields to the config object's fields collection
		jmethodID midAddField = getMethodId(env, cls, "addField", "(Ledu/stanford/slac/aida/lib/model/AidaField;)V");
		if (!midAddField) {
			aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
					"Failed to find the addField(AidaField) method on AidaChannelConfig object");
			return NULL;
		} else {
			// For each field, add it to config object's fields collection
			for (int i = 0; i < config.fieldCount; i++) {
				Field field = config.fields[i];
				(*env)->CallObjectMethod(env, configObject, midAddField, getAidaField(env, field));
				CHECK_EXCEPTION(NULL)
			}
		}

		// free any defined fields
		free(&(config.fields));
	}

	return configObject;
}

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
 * Get an aida field jni object from a Field
 *
 * @param env env
 * @param field  field
 * @return aida jni field object
 */
jobject getAidaField(JNIEnv* env, Field field)
{
	JavaObject javaObject = newObject(env, "edu/stanford/slac/aida/lib/model/AidaField");
	CHECK_EXCEPTION(NULL)

	// Get new AidaField object
	jobject fieldObject = javaObject.object;

	// Set name, label, description and units
	callSetterWithString(env, javaObject, "setName", field.name);
	CHECK_EXCEPTION(NULL)

	callSetterWithString(env, javaObject, "setLabel", field.label);
	CHECK_EXCEPTION(NULL)

	callSetterWithString(env, javaObject, "setDescription", field.description);
	CHECK_EXCEPTION(NULL)

	callSetterWithString(env, javaObject, "setUnits", field.units);

	return fieldObject;
}

/**
 * Create a new jni string from a c string
 *
 * @param env env
 * @param cString c string
 * @return jni string
 */
jobject toJniString(JNIEnv* env, char* cString)
{
	jobject object;

	if (!cString) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Attempt to convert null string to java String");
		return NULL;
	}
	object = (*env)->NewStringUTF(env, cString);
	return object;
}

/**
 * Call setter on a given object with a string argument
 *
 * @param env environment
 * @param javaObject given object
 * @param method string setter method to call
 * @param value string value to set
 */
void callSetterWithString(JNIEnv* env, JavaObject javaObject, char* method, char* value)
{
	jobject jObject = toJniString(env, value);
	CHECK_EXCEPTION_VOID

	callSetterWithJString(env, javaObject, method, jObject);
}

/**
 * Call a setter on a given object with a jString argument
 *
 * @param env env
 * @param javaObject object
 * @param method method
 * @param value jstring value
 */
void callSetterWithJString(JNIEnv* env, JavaObject javaObject, char* method, jstring value)
{
	if (value) {
		jmethodID midSetter = (*env)->GetMethodID(env, javaObject.class, method, "(Ljava/lang/String;)V");
		if (!midSetter) {
			char errorString[BUFSIZ];
			sprintf(errorString, "Failed to get method %s(String)", method);
			aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
			return;
		}
		(*env)->CallObjectMethod(env, javaObject.object, midSetter, value);
	}
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
	if ( !string ) {
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
Arguments toArguments(JNIEnv* env, jobject jArgs)
{
	Arguments cArgs;
	cArgs.argumentCount = 0;

	// retrieve the java.util.List interface class
	jclass cList = (*env)->FindClass(env, "java/util/List");
	if (!cList) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to get List class");
		return cArgs;
	}

	// retrieve the AidaArgument class
	jclass aidaArgumentClass = (*env)->FindClass(env, "edu/stanford/slac/aida/lib/model/AidaArgument");
	if (!aidaArgumentClass) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to get AidaArgument class");
		return cArgs;
	}

	// retrieve the size and the get methods of list
	jmethodID mSize = (*env)->GetMethodID(env, cList, "size", "()I");
	jmethodID mGet = (*env)->GetMethodID(env, cList, "get", "(I)Ljava/lang/Object;");

	// retrieve the getName and the getValue methods of list
	jmethodID mName = (*env)->GetMethodID(env, aidaArgumentClass, "getName", "()Ljava/lang/String;");
	jmethodID mValue = (*env)->GetMethodID(env, aidaArgumentClass, "getValue", "()Ljava/lang/String;");

	if (!mSize) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"Failed to get size(String) method on List object");
		return cArgs;
	}
	if (!mGet) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, "Failed to get get(int) method on List object");
		return cArgs;
	}
	if (!mName) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"Failed to get getName() method on AidaArgument object");
		return cArgs;
	}
	if (!mValue) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"Failed to get getValue(int) method on AidaArgument object");
		return cArgs;
	}

	// get the size of the list
	cArgs.argumentCount = (*env)->CallIntMethod(env, jArgs, mSize);

	// Create array of arguments
	cArgs.arguments = calloc(cArgs.argumentCount, sizeof(Argument));
	if (!cArgs.arguments) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"Failed to allocate memory for arguments");
		return cArgs;
	}

	// walk through and fill array
	for (int i = 0; i < cArgs.argumentCount; i++) {
		jobject argument = (*env)->CallObjectMethod(env, jArgs, mGet, i);
		cArgs.arguments[i].name = toCString(env, (*env)->CallObjectMethod(env, argument, mName));
		cArgs.arguments[i].value = toCString(env, (*env)->CallObjectMethod(env, argument, mValue));
	}

	// Return arguments
	return cArgs;
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
Value getArrayValue(JNIEnv* env, Arguments arguments) {
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


