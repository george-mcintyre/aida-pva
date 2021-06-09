#include <jni.h>
#include <memory.h>
#include <stdlib.h>
#include "../include/aida_types.h"
#include "../include/aida_jni_helper.h"

/**
 * Empty config block for null comparison.  Static variables are automatically initialised to zero
 */
static const Config EMPTY_CONFIG;

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
	JavaObject javaObject = newObject(env, "Ledu/stanford/slac/aida/lib/model/AidaChannelConfig");
	jobject configObject = javaObject.object;
	jclass cls = javaObject.class;
	if (configObject == NULL) {
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
		jmethodID midAddField = getMethodId(env, cls, "addField", "(Ledu/stanford/slac/aida/lib/model/AidaField)V");
		if (NULL == midAddField) {
			return NULL;
		}

		// For each field, add it to config object's fields collection
		for (int i = 0; i < config.fieldCount; i++) {
			Field field = config.fields[i];
			(*env)->CallObjectMethod(env, configObject, midAddField, getAidaField(env, field));
		}

		// free any defined fields
		free(&config.fields);
	}

	return configObject;
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

	// Get a class reference
	javaObject.class = (*env)->FindClass(env, classToCreate);

	// Get the Method ID of the no-args constructor
	jmethodID midInit = getConstructorMethodId(env, javaObject.class);
	if (NULL == midInit) {
		javaObject.class = NULL;
		javaObject.object = NULL;
		return javaObject;
	}
	// Call back constructor to allocate a new instance
	javaObject.object = (*env)->NewObject(env, javaObject.class, midInit);

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
	JavaObject javaObject = newObject(env, "Ledu/stanford/slac/aida/lib/model/AidaField");
	jobject fieldObject = javaObject.object;

	// Set name, label, description and units
	callSetterWithString(env, javaObject, "setName", field.name);
	callSetterWithString(env, javaObject, "setLabel", field.label);
	callSetterWithString(env, javaObject, "setDescription", field.description);
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
	callSetterWithJString(env, javaObject, method, toJniString(env, value));
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
		jmethodID midSetter = (*env)->GetMethodID(env, javaObject.class, method, "(Ljava/lang/String)V");
		if (NULL != midSetter) {
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
	jclass aidaArgumentClass = (*env)->FindClass(env, "Ledu.stanford.slac.aida.lib.model.AidaArgument");

	// retrieve the size and the get methods of list
	jmethodID mSize = (*env)->GetMethodID(env, cList, "size", "()I");
	jmethodID mGet = (*env)->GetMethodID(env, cList, "get", "(I)Ljava/lang/Object;");

	// retrieve the getName and the getValue methods of list
	jmethodID mName = (*env)->GetMethodID(env, aidaArgumentClass, "getName", "()Ljava/lang/String");
	jmethodID mValue = (*env)->GetMethodID(env, aidaArgumentClass, "getValue", "()Ljava/lang/String");

	if (mSize == NULL || mGet == NULL || mName == NULL || mValue == NULL) {
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
 * Free up any memory allocated with arguments
 * @param arguments
 */
void releaseArguments(Arguments arguments)
{
	if (arguments.argumentCount > 0) {
		free(arguments.arguments);
	}
}
