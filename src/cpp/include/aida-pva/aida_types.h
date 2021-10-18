#include <jni.h>
#include <stdbool.h>
#include "json.h"

#ifndef _Included_aida_types
#define _Included_aida_types
#ifdef __cplusplus
extern "C" {
#endif

/* Override prototypes of externals to uppercase names since
compile.com adds cc/name=UPPERCASE on compiles by default.
However, if the ATTRIBUTE=JNI is in effect (as it is for
this module), then it is /names=AS_IS.
-------------------------------------------------------------*/
void CVT_VMS_TO_IEEE_FLT(void* sorc_a, float dest_a[], unsigned short* nlong_p);
void CVT_VMS_TO_IEEE_DBL(void* sorc_a, double dest_a[], unsigned short* nlong_p);
void CVT_IEEE_TO_VMS_FLT(void* sorc_a, float dest_a[], unsigned short* nlong_p);
void CVT_IEEE_TO_VMS_DBL(void* sorc_a, double dest_a[], unsigned short* nlong_p);

#define CONVERT_TO_VMS_FLOAT(_float, _count) \
{  \
    int2u _n = _count; \
    CVT_IEEE_TO_VMS_FLT(_float, _float, &_n); \
}

#define CONVERT_TO_VMS_DOUBLE(_double, _count) \
{  \
    int2u _n = _count; \
    CVT_IEEE_TO_VMS_DBL(_double, _double, &_n); \
}

#define CONVERT_FROM_VMS_DOUBLE(_double, _count) \
{  \
    int2u _n = _count; \
    CVT_VMS_TO_IEEE_DBL(_double, _double, &_n); \
}

#define CONVERT_FROM_VMS_FLOAT(_float, _count) \
{  \
    int2u _n = _count; \
    CVT_VMS_TO_IEEE_FLT(_float, _float, &_n); \
}

/**
 * The definition of Aida Types.
 * Defines the permissible types of fields.  This enumerated type defines all the possible AIDA-PVA types and classes.
 */
typedef enum
{
	AIDA_NO_TYPE,                      ///< Used to indicate that no type was provided as an argument
	AIDA_VOID_TYPE,                    ///< Used when no return value is to be returned from a channel

	AIDA_BOOLEAN_TYPE,                 ///< Represents a boolean
	AIDA_BYTE_TYPE,                    ///< Represents a byte
	AIDA_SHORT_TYPE,                   ///< Represents a short
	AIDA_INTEGER_TYPE,                 ///< Represents an integer
	AIDA_LONG_TYPE,                    ///< Represents a long
	AIDA_FLOAT_TYPE,                   ///< Represents a float
	AIDA_DOUBLE_TYPE,                  ///< Represents a double
	AIDA_STRING_TYPE,                  ///< Represents a string
	AIDA_BOOLEAN_ARRAY_TYPE,           ///< Represents a boolean array
	AIDA_BYTE_ARRAY_TYPE,              ///< Represents a byte array
	AIDA_SHORT_ARRAY_TYPE,             ///< Represents a short array
	AIDA_INTEGER_ARRAY_TYPE,           ///< Represents an integer array
	AIDA_LONG_ARRAY_TYPE,              ///< Represents a long array
	AIDA_FLOAT_ARRAY_TYPE,             ///< Represents a float array
	AIDA_DOUBLE_ARRAY_TYPE,            ///< Represents a double array
	AIDA_STRING_ARRAY_TYPE,            ///< Represents a string array
	AIDA_TABLE_TYPE,                   ///< Represents a table
	AIDA_JSON_TYPE,                    ///< Argument was provided as JSON text

	///< Internal use only: DO NOT USE IN SERVICE IMPLEMENTATIONS!!
	AIDA_UNSIGNED_SHORT_TYPE,           ///< Represents an internal type of unsigned short
	AIDA_UNSIGNED_INTEGER_TYPE,         ///< Represents an internal type of unsigned integer
	AIDA_UNSIGNED_LONG_TYPE,            ///< Represents an internal type of unsigned long
	AIDA_UNSIGNED_SHORT_ARRAY_TYPE,     ///< Represents an internal type of unsigned short array
	AIDA_UNSIGNED_INTEGER_ARRAY_TYPE,   ///< Represents an internal type of unsigned integer array
	AIDA_UNSIGNED_LONG_ARRAY_TYPE       ///< Represents an internal type of unsigned long array
} Type;

/**
 * This union stores the value part of a FloatingPointValue.
 * The `floatValue` member stores the float.
 * The `doubleValue` member stores the double.
 */
typedef union
{
	float floatValue;          ///< The single precision floating point value
	double doubleValue;        ///< The double precision floating point value
} FloatOrDoubleValue;

/**
 * Represents a floating point number.
 * This can be either a single or double precision value.
 * The `isFloat` flag
 * is set to `true` if this is a single precision value and the value can be accessed
 * via the `value.floatValue`.
 * If the `isFloat` flag
 * is set to `false` then this is a double precision value and the value can be accessed
 * via the `value.doubleValue`.
 *
 */
typedef struct
{
	char* path;                      ///< The string path from the root of the json structure in which this value is found
	bool isFloat;                    ///< Determines whether the value is a single or double precision floating point value
	FloatOrDoubleValue value;        ///< The floating point value
} FloatingPointValue;

/**
 * A single request argument.
 * This is passed to an API endpoint in an Arguments structure.
 * It contains a `name` `value` pair representing a single argument that was included in the
 * PVAccess request.  The `value` is a string.
 */
typedef struct
{
	char* name;                 ///< The name of the argument
	char* value;                ///< The string value of the argument
} Argument;

/**
 * This union stores either the string or the json_value of a Value.
 * An Value can be a string or can be a json structure.  If json is detected it
 * is automatically parsed and placed in the ValueContents union under the `jsonValue`
 * key.
 */
typedef union
{
	char* stringValue;            ///< The string value of this Value if the type is AIDA_STRING_TYPE
	json_value* jsonValue;        ///< The parsed json_value of this Value if the type is AIDA_JSON_TYPE
} ValueContents;

/**
 * This special type represents a Value.
 * A Value is a special type that is used to pass complex data to the Native Channel Provider.
 * The value member, of type ValueContents, can hold either a simple string or a json_value.
 */
typedef struct
{
	Type type;                    ///< AIDA_STRING_TYPE or AIDA_JSON_TYPE
	ValueContents value;          ///< The value's contents, either a string or parsed json
} Value;

/**
 * An Arguments structure stores all of the arguments passed from the request to the Native Channel Provider.
 * It contains a count of the total number of arguments - `argumentCount` - and a pointer
 * to that many Arguments.
 * Arguments from the client are stored as strings, and converted to whatever type is required whenever
 * they are used.  But floating points are different because they require more precision and significance
 * than string representations can give.  For this reason floating points are additionally passed in the
 * FloatingPointValue array where `floatingPointValuesCountCount` is the count of how many of them there are.
 */
typedef struct
{
	int argumentCount;								///< The number of arguments sent with this request
	int floatingPointValuesCountCount;			    ///< The number of floating point numbers in the arguments of this request
	Argument* arguments;						    ///< The array of Arguments
	FloatingPointValue* floatingPointValues;	    ///< The array of FloatingPointValue
} Arguments;

/**
 * Table structure.
 * This structure holds everything that a Native Channel Provider needs for returning a Table
 * to the client.  A table is a set of homogeneously sized vectors representing columns of data.
 * The configuration of the table (labels, names, descriptions, etc) is handled outside of the
 * Native Channel Provider, so all the implementation needs to do is provide the raw data for the
 * columns.
 *
 * Implementors don't manipulate these structures directly as AIDA-PVA Module provides
 * table manipulation functions:
 *  - tableCreate.
 *  - tableAddColumn.
 *  - tableAddSingleRowFloatColumn.
 *  - tableAddSingleRowLongColumn.
 *  - tableAddSingleRowBooleanColumn.
 *  - tableAddSingleRowByteColumn.
 *  - tableAddSingleRowShortColumn.
 *  - tableAddSingleRowIntegerColumn.
 *  - tableAddSingleRowDoubleColumn.
 *  - tableAddSingleRowStringColumn.
 *  - tableAddFixedWidthStringColumn.
 *  - tableAddStringColumn.
 */
typedef struct
{
	int columnCount;        ///< number of columns in table
	int rowCount;           ///<  number of rows in table
	Type* types;            ///< the scalar type of each column, one of BOOLEAN, BYTE, SHORT, INTEGER, LONG, FLOAT, DOUBLE, or STRING
	void** ppData;          ///< the data.  an array of [rows][columns]
	int _currentColumn;     ///< For internal use by addColumn() etc
} Table;

/**
 * An array of data.
 * This is used to return an array of data to the client.
 *
 * When you are implementing an API that returns an array you need to fill an Array structure appropriately.
 * Fill the `count` member with the number of elements in the array.  Allocate memory for those items
 * and set the `items` member to that pointer.  The framework will automatically free up  the
 * `items` pointer memory.
 */
typedef struct
{
	int count;				///< The number of items in this array
	void* items;			///< The items in this array
} Array;

/**
 * An array of string data.
 * This is used to return an array of string data to the client.
 *
 * When you are implementing an API that returns an array of strings you need to fill an StringArray
 * structure appropriately.
 * Fill the `count` member with the number of elements in the array.  Allocate memory for those items
 * and set the `items` member to that pointer.  The `items` will be a `char **`
 * meaning that it will be an array of character pointers, and each of the strings MUST be allocated
 * as well.  This is because the framework will automatically free up strings for you as well as the
 * `items` pointer itself.
 */
typedef struct
{
	int count;				///< The number of items in this array
	char** items;			///< The items in this array - pointers to the strings you allocate
} StringArray;

/**
 * Make a table for return to the client.
 * Specify the number of columns then call tableAddColumn and tableAddStringColumn to add
 * columns before returning the table.
 *
 * @param env this is the environment pointer so that exceptions can be raised if necessary
 * @param columns the number of columns
 * @return the newly created table
 */
Table tableCreate(JNIEnv* env, int rows, int columns);

/**
 * Add a column to a table.
 * This adds a new column to a table that is under construction.
 * @param env this is the environment pointer so that exceptions can be raised if necessary
 * @param table
 * @param type
 * @param data
 */
void tableAddColumn(JNIEnv* env, Table* table, Type type, void* data, bool ieeeFormat);
void tableAddSingleRowFloatColumn(JNIEnv* env, Table* table, float data, bool ieeeFloat);
void tableAddSingleRowLongColumn(JNIEnv* env, Table* table, long data);
void tableAddSingleRowBooleanColumn(JNIEnv* env, Table* table, unsigned char data);
void tableAddSingleRowByteColumn(JNIEnv* env, Table* table, unsigned char data);
void tableAddSingleRowShortColumn(JNIEnv* env, Table* table, short data);
void tableAddSingleRowIntegerColumn(JNIEnv* env, Table* table, int data);
void tableAddSingleRowDoubleColumn(JNIEnv* env, Table* table, double data, bool ieeeDouble);
void tableAddSingleRowStringColumn(JNIEnv* env, Table* table, char* data);
void tableAddFixedWidthStringColumn(JNIEnv* env, Table* table, void* data, int width);
void tableAddStringColumn(JNIEnv* env, Table* table, char** data);

float valueGetFloat(Value value);
short valueGetShort(Value value);

/**
 * Convert Type to string name of Type e.g. AIDA_BOOLEAN_TYPE returns "BOOLEAN"
 *
 * @param type type
 * @return string
 */
jstring toTypeString(JNIEnv* env, Type type);

/**
 * Make a single entry json_value array from a string.
 *
 * @param stringValue the string to add as the single entry in the json_value array
 * @return the Value
 */
Value asArrayValue(char* stringValue);

/**
 * ascanf, avscanf
 *
 * Synopsis
 *
 *     int ascanf(Arguments *arguments, const char *format, ...);
 *     int avscanf(Arguments *arguments, Value *value, const char *format, ...);
 *
 * Details
 * Reads data from the given arguments and stores them according to parameter format into the locations given by the additional arguments, as if scanf was used, but reading from arguments instead of the standard input (stdin).
 *
 * The additional arguments should point to already allocated objects of the type specified by their corresponding format specifier.  For strings and arrays only the pointer needs to be pre-allocated.
 *
 * The only space allocated by this function is for strings, or arrays.  You need to free those when you're done
 *
 * Note, only the provided pointer needs to be freed as only one allocation is made e.g.
 *
 *     Arguments arguments;
 *     float theFloat;
 *     int *intArray;
 *     ascanf(arguments "$of, %da", "f", &theFloat, "fooArray", &intArray);
 *     ///< Do stuff
 *
 * Differences from scanf
 * There are a number of other differences from scanf which are best described by example:
 *
 * 1. Scan into simple variable
 *
 *     int n;
 *     ascanf("%d", "NPOS", &n);
 *
 * You must always provide the name of the variable and the pointer to the place to put the value in pairs
 *
 * 2. Optional arguments
 * Optional arguments are specified with the o character before the format character.
 *
 *     short flag = 10;  ///< 10 is the default value
 *     ascanf("%ohd", "flag", &flag);
 *
 * By default all arguments are considered required unless this character is specified. For optional arguments the pointer provided must point to the default value. In the case of arrays and strings this will be copied into allocated storage that will need to be freed as normal. i.e. strings themselves don't need to be freed.
 *
 * Variable Names
 * 1. You can specify simple variable names
 *     int simpleInt;
 *     ascanf(&arguments "%d, "simple", &simpleInt);
 *
 * 1. You can specify simple names or you can use dot and square brace notation to refer to arguments that refer to json structures. e.g. given a variable named json and presented as
 *
 *     json=' { "foo": {"bar": 0} }}'
 *
 * You can specify the name as json.foo.bar to retrieve the 0 value *
 *
 * 2. Also given a variable named jsonArray and presented as
 *     jsonArray=' [ {"foo": 10}, {"bar": 20} ]'
 *
 * You can specify the name as jsonArray[1].bar to retrieve the 20 value
 *
 * 3. Finally if you use the name value in the the avscanf() function will use the supplied value to get the data for that parameter
 *     Arguments arguments;
 *     Value value;
 *     int *intArray;
 *     avscanf(&arguments &value, "%da, "fooArray", &intArray);
 *     ///< Do stuff
 *     free(intArray);
 *
 * Format Specifiers
 * Supported formats specifiers
 * - d : int * - extract an integer into the corresponding variable (see l & h below).
 * - u : unsigned int * - extract an unsigned integer into the corresponding variable (see l & h below)
 * - f  : float * - extract a floating point number (see l below)
 * - s : char * - extract a string of characters into allocated space and point the corresponding variable to it
 * - c : char * - extract a single character into the corresponding variable
 *
 * Required flag
 * - o - optional precede the format with 'o' to indicate that the argument is optional
 *
 * Prefixes
 * - h - short * - preceding d will retrieve a short e.g. %hd
 * - l - long *, double * - preceding d will retrieve a long eg. %ld, preceding f will retrieve a double eg. %lf
 *
 * Suffixes
 * - a - extract an array of the preceding type into a block of allocated space and point the corresponding variable to it. the variable will have an extra level of indirection than the non-array version e.g. "%d" "int *" becomes "%da" "int **"
 *
 * @param env
 * @param arguments      arguments that the function processes as its source to retrieve the data.
 * @param value          value that the function processes as its source to retrieve the data
 * @param formatString   C  string that contains a format string as described above
 * @param ...            Depending on the format string, the function may expect a sequence of additional arguments,
 * 						 containing pairs of names and pointers to allocated storage (except as indicated above),
 * 						 where the interpretation of the extracted data is stored with the appropriate type.
 *                       There should be at least as many pairs of these arguments as the number of values stored
 *                       by the format specifiers.
 *                       Additional arguments are ignored by the function
 * @return true if all required arguments were read and no errors occurred
 * @throw MissingRequiredArgument if one of the required arguments are missing
*/
int ascanf(JNIEnv* env, Arguments* arguments, const char* formatString, ...);
int avscanf(JNIEnv* env, Arguments* arguments, Value* value, const char* formatString, ...);

#ifdef __cplusplus
}
#endif
#endif

