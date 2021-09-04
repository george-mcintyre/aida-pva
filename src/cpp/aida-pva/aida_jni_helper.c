#include <jni.h>
#include <memory.h>
#include <stdlib.h>
#include "aida_types.h"
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
	// Null check
	if (memcmp(&config, &EMPTY_CONFIG, sizeof config) == 0) {
		return NULL;
	}

	// Get a java object reference
	JavaObject javaObject = newObject(env, "edu/stanford/slac/aida/lib/model/AidaChannelConfig");
	jobject configObject = javaObject.object;
	jclass cls = javaObject.class;
	if (configObject == NULL || cls == NULL) {
		fprintf(stderr, "Failed to create AidaChannelConfig object\n");
		return NULL;
	}

	////////////
	// Set fields appropriately
	////////////

	// Set type, layout, & description
	callSetterWithJString(env, javaObject, "setType", toTypeString(env, config.type));
	callSetterWithJString(env, javaObject, "setLayout", toLayoutString(env, config.layout));
	callSetterWithString(env, javaObject, "setDescription", config.description);

	// Set fields ( and free up storage )
	if (config.fields != NULL && config.fieldCount != 0) {
		// Find the method to add fields to the config object's fields collection
		jmethodID midAddField = getMethodId(env, cls, "addField", "(Ledu/stanford/slac/aida/lib/model/AidaField;)V");
		if (NULL == midAddField) {
			fprintf(stderr, "Failed to find the addField(AidaField) method on AidaChannelConfig object\n");
		} else {
			// For each field, add it to config object's fields collection
			for (int i = 0; i < config.fieldCount; i++) {
				Field field = config.fields[i];
				(*env)->CallObjectMethod(env, configObject, midAddField, getAidaField(env, field));
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
	if (NULL == midInit) {
		fprintf(stderr, "Failed to create object\n");
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
	if (NULL == javaObject.class) {
		fprintf(stderr, "Failed to create object of class: %s\n", classToCreate);
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
	// Get new AidaField object
	JavaObject javaObject = newObject(env, "edu/stanford/slac/aida/lib/model/AidaField");
	jobject fieldObject = javaObject.object;

	if (fieldObject != NULL) {
		// Set name, label, description and units
		callSetterWithString(env, javaObject, "setName", field.name);
		callSetterWithString(env, javaObject, "setLabel", field.label);
		callSetterWithString(env, javaObject, "setDescription", field.description);
		callSetterWithString(env, javaObject, "setUnits", field.units);
	}

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
	if (cString == NULL) {
		return NULL;
	}
	return (*env)->NewStringUTF(env, cString);
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
	if (jObject != NULL) {
		callSetterWithJString(env, javaObject, method, jObject);
	}
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
	if (value != NULL) {
		jmethodID midSetter = (*env)->GetMethodID(env, javaObject.class, method, "(Ljava/lang/String;)V");
		if (NULL == midSetter) {
			fprintf(stderr, "Failed to get method %s(String)\n", method);
		} else {
			(*env)->CallObjectMethod(env, javaObject.object, midSetter, value);
		}
	}
}

/**
 * Convert j-string to c-string
 *
 * @param env env
 * @param string j-string
 * @return c-string
 */
const char* toCString(JNIEnv* env, jstring string)
{
	return (*env)->GetStringUTFChars(env, string, NULL);
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
 * Get the method ID of the constructor of the given class for ArrayLists
 *
 * @param env env
 * @param cls given class
 * @return the constructor method id
 */
jmethodID getArrayListConstructorMethodId(JNIEnv* env, jclass cls)
{
	return getMethodId(env, cls, "<init>", "(I)V");
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
	if (cList == NULL) {
		fprintf(stderr, "Failed to get List class\n");
		return cArgs;
	}

	// retrieve the AidaArgument class
	jclass aidaArgumentClass = (*env)->FindClass(env, "edu/stanford/slac/aida/lib/model/AidaArgument");
	if (aidaArgumentClass == NULL) {
		fprintf(stderr, "Failed to get AidaArgument class\n");
		return cArgs;
	}

	// retrieve the size and the get methods of list
	jmethodID mSize = (*env)->GetMethodID(env, cList, "size", "()I");
	jmethodID mGet = (*env)->GetMethodID(env, cList, "get", "(I)Ljava/lang/Object;");

	// retrieve the getName and the getValue methods of list
	jmethodID mName = (*env)->GetMethodID(env, aidaArgumentClass, "getName", "()Ljava/lang/String;");
	jmethodID mValue = (*env)->GetMethodID(env, aidaArgumentClass, "getValue", "()Ljava/lang/String;");

	if (mSize == NULL || mGet == NULL || mName == NULL || mValue == NULL) {
		if (mSize == NULL) {
			fprintf(stderr, "Failed to get size(String) method on List object\n");
		}
		if (mGet == NULL) {
			fprintf(stderr, "Failed to get get(int) method on List object\n");
		}
		if (mName == NULL) {
			fprintf(stderr, "Failed to get getName() method on AidaArgument object\n");
		}
		if (mValue == NULL) {
			fprintf(stderr, "Failed to get getValue(int) method on AidaArgument object\n");
		}
		return cArgs;
	}

	// get the size of the list
	cArgs.argumentCount = (*env)->CallIntMethod(env, jArgs, mSize);

	// Create array of arguments
	cArgs.arguments = calloc(cArgs.argumentCount, sizeof(Argument));

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
	if (NULL == returnValue) {
		fprintf(stderr, "Failed to create a new Boolean Array with %d elements\n", array.count);
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
	if (NULL == returnValue) {
		fprintf(stderr, "Failed to create a new Byte Array with %d elements\n", array.count);
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
	if (NULL == returnValue) {
		fprintf(stderr, "Failed to create a new Short Array with %d elements\n", array.count);
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
	if (NULL == returnValue) {
		fprintf(stderr, "Failed to create a new Integer Array with %d elements\n", array.count);
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
	if (NULL == returnValue) {
		fprintf(stderr, "Failed to create a new Long Array with %d elements\n", array.count);
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
	if (NULL == returnValue) {
		fprintf(stderr, "Failed to create a new Float Array with %d elements\n", array.count);
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
	if (NULL == returnValue) {
		fprintf(stderr, "Failed to create a new Double Array with %d elements\n", array.count);
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
	if (classString == NULL) {
		fprintf(stderr, "Failed to get jclass of java String\n");
		return NULL;
	}

	returnValue = (*env)->NewObjectArray(env, array.count, classString, NULL);
	if (NULL == returnValue) {
		fprintf(stderr, "Failed to create a new String Array with %d elements\n", array.count);
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
	tableToReturn = listObject.object;
	jclass cList = listObject.class;

	// retrieve the add method of the list
	jmethodID mAdd = (*env)->GetMethodID(env, cList, "add", "(ILjava/lang/Object;)Z");
	if (mAdd == NULL) {
		fprintf(stderr, "Failed to find the add(int, Object) method on AidaTable object\n");
		return NULL;
	}

	if (tableToReturn == NULL) {
		fprintf(stderr, "Failed to create a new AidaTable object\n");
		return tableToReturn;
	}


	// Loop over each column
	for (int column = 0; column < table.columnCount; column++) {
		// loop over each row
		for (int row = 0; row < table.rowCount; row++) {
			// call appropriate add())
			switch (table.types[column]) {
			case AIDA_BOOLEAN_ARRAY_TYPE : {
				jboolean data = ((jboolean*)(table.ppData[column]))[row];
				jobject dataObject = toBoolean(env, data);
				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_BYTE_ARRAY_TYPE: {
				jbyte data = ((jbyte*)(table.ppData[column]))[row];
				jobject dataObject = toByte(env, data);
				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_SHORT_ARRAY_TYPE: {
				jshort data = ((jshort*)(table.ppData[column]))[row];
				jobject dataObject = toShort(env, data);
				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_INTEGER_ARRAY_TYPE: {
				jint data = ((jint*)(table.ppData[column]))[row];
				jobject dataObject = toInteger(env, data);
				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_LONG_ARRAY_TYPE: {
				jlong data = ((jlong*)(table.ppData[column]))[row];
				jobject dataObject = toLong(env, data);
				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_FLOAT_ARRAY_TYPE: {
				jfloat data = ((jfloat*)(table.ppData[column]))[row];
				jobject dataObject = toFloat(env, data);
				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_DOUBLE_ARRAY_TYPE: {
				jdouble data = ((jdouble*)(table.ppData[column]))[row];
				jobject dataObject = toDouble(env, data);
				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, dataObject);
				(*env)->DeleteLocalRef(env, dataObject);
				break;
			}
			case AIDA_STRING_ARRAY_TYPE: {
				char* string = ((char**)(table.ppData[column]))[row];
				jstring stringValue = toJString(env, string);
				(*env)->CallBooleanMethod(env, tableToReturn, mAdd, column, stringValue);

				// Free up string buffer
				free(string);
				(*env)->DeleteLocalRef(env, stringValue);
				break;
			}
			default:
				fprintf(stderr, "Warning: Unsupported type found in table\n");
				break;
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
	if (arguments.argumentCount > 0) {
		free(arguments.arguments);
	}
}

/**
 * Free up any memory allocated scalar arrays
 * @param array
 */
void releaseArray(Array array)
{
	if (array.count > 0) {
		free(array.items);
	}
}

/**
 * Free up any memory allocated for string arrays
 * @param array
 */
void releaseStringArray(StringArray array)
{
	if (array.count > 0) {
		free(array.items);
	}
}

/**
 * Free up any memory allocated for tables
 * @param table
 */
void releaseTable(Table table)
{
	if (table.columnCount > 0) {
		for (int column = 0; column < table.columnCount; column++) {
			free(table.ppData[column]);
		}
		free(table.ppData);
	}
}

ClassAndMethod getClassAndValueOfMethod(JNIEnv* env, char* boxedClassSignature, char* valueOfMethodSignature)
{
	ClassAndMethod classAndMethod;

	// Get a class reference
	classAndMethod.class = (*env)->FindClass(env, boxedClassSignature);
	if (NULL == classAndMethod.class) {
		fprintf(stderr, "Failed to get class of: %s\n", boxedClassSignature);
		return classAndMethod;
	}

	// retrieve the valueOf method
	classAndMethod.methodId = (*env)->GetStaticMethodID(env, classAndMethod.class, "valueOf", valueOfMethodSignature);
	if (classAndMethod.methodId == NULL) {
		fprintf(stderr, "Failed to valueOf method with signature: %s\n", valueOfMethodSignature);
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
	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, data);
	if (dataObject == NULL) {
		fprintf(stderr, "Failed to convert boolean to Boolean\n");
		return NULL;
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
	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, data);
	if (dataObject == NULL) {
		fprintf(stderr, "Failed to convert byte to Byte\n");
		return NULL;
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
	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, data);
	if (dataObject == NULL) {
		fprintf(stderr, "Failed to convert short to Short\n");
		return NULL;
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
	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, data);
	if (dataObject == NULL) {
		fprintf(stderr, "Failed to convert int to Integer\n");
		return NULL;
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
	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, data);
	if (dataObject == NULL) {
		fprintf(stderr, "Failed to convert long to Long\n");
		return NULL;
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
	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, data);
	if (dataObject == NULL) {
		fprintf(stderr, "Failed to convert float to Float\n");
		return NULL;
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
	jobject dataObject = (*env)->CallStaticObjectMethod(env, classAndMethod.class, classAndMethod.methodId, data);
	if (dataObject == NULL) {
		fprintf(stderr, "Failed to convert double to Double\n");
		return NULL;
	}

	return dataObject;
}


