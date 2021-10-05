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
 * Defines the permissible types of fields
 */
typedef enum
{
	AIDA_NO_TYPE,						// Used to indicate that no type was provided as an argument
	AIDA_VOID_TYPE,						// Used when no return value is to be returned from a channel

	AIDA_BOOLEAN_TYPE,					// Represents a boolean
	AIDA_BYTE_TYPE,						// Represents a byte
	AIDA_SHORT_TYPE,					// Represents a short
	AIDA_INTEGER_TYPE,					// Represents an integer
	AIDA_LONG_TYPE,						// Represents a long
	AIDA_FLOAT_TYPE,					// Represents a float
	AIDA_DOUBLE_TYPE,					// Represents a double
	AIDA_STRING_TYPE,					// Represents a string
	AIDA_BOOLEAN_ARRAY_TYPE,			// Represents a boolean array
	AIDA_BYTE_ARRAY_TYPE,				// Represents a byte array
	AIDA_SHORT_ARRAY_TYPE,				// Represents a short array
	AIDA_INTEGER_ARRAY_TYPE,			// Represents an integer array
	AIDA_LONG_ARRAY_TYPE,				// Represents a long array
	AIDA_FLOAT_ARRAY_TYPE,				// Represents a float array
	AIDA_DOUBLE_ARRAY_TYPE,				// Represents a double array
	AIDA_STRING_ARRAY_TYPE,				// Represents a string array
	AIDA_TABLE_TYPE,					// Represents a table
	AIDA_JSON_TYPE,						// Argument was provided as JSON text

	// Internal use only: DO NOT USE IN SERVICE IMPLEMENTATIONS!!
	AIDA_UNSIGNED_SHORT_TYPE,			// Represents an internal type of unsigned short
	AIDA_UNSIGNED_INTEGER_TYPE,			// Represents an internal type of unsigned integer
	AIDA_UNSIGNED_LONG_TYPE,			// Represents an internal type of unsigned long
	AIDA_UNSIGNED_SHORT_ARRAY_TYPE,		// Represents an internal type of unsigned short array
	AIDA_UNSIGNED_INTEGER_ARRAY_TYPE,	// Represents an internal type of unsigned integer array
	AIDA_UNSIGNED_LONG_ARRAY_TYPE		// Represents an internal type of unsigned long array
} Type;

/**
 * Defines the layout of TABLES.
 */
typedef enum
{
	AIDA_NO_LAYOUT,
	AIDA_COLUMN_MAJOR_LAYOUT,  // Each top level array entry is a column containing row data
	AIDA_ROW_MAJOR_LAYOUT      // Each top level array entry is a row containing column data
} Layout;

/**
 * Definition of a field in a channel configuration
 *  name:           the name of the field
 *  label:          the field label
 *  description:    the description
 *  units;          the units e.g. cm, mm, nm ...
 */
typedef struct
{
	char* name;
	char* label;
	char* description;
	char* units;
} Field;

/**
 * Definition of the Channel Configuration structure
 *  type:           the type to be returned in this field
 *  description:    the description of the field
 *  layout:         for TABLES only, this determines whether you'll be returning data COLUMN_MAJOR or ROW_MAJOR
 *  fieldCount:     the number of fields
 *  fields:         field descriptions.  Here we limit the number of fields that can be described to 10.  If you
 *                  want more then use yaml configuration files on the java side
 */
typedef struct
{
	Type type;
	char* description;
	Layout layout;
	int fieldCount;
	Field* fields;
} Config;

typedef struct
{
	char* name;
	char* value;
} Argument;

typedef union
{
	char* stringValue;
	json_value* jsonValue;
} ValueContents;

typedef struct
{
	Type type;    // AIDA_STRING_TYPE or AIDA_JSON_TYPE
	ValueContents value;
} Value;

typedef struct
{
	int argumentCount;
	Argument* arguments;
} Arguments;

/**
 * Table structure
 *  columnCount:    number of columns in table
 *  rowCount:       number of rows in table
 *  types:          the scalar type of each column, one of BOOLEAN, BYTE, SHORT, INTEGER, LONG, FLOAT, DOUBLE, or STRING
 *  ppData:         the data.  Array of [rows][columns]
 */
typedef struct
{
	int columnCount;
	int rowCount;
	Type* types;
	void** ppData;
	int _currentColumn; // For internal use by addColumn() etc
} Table;

/**
 * An array of data
 */
typedef struct
{
	int count;
	void* items;
} Array;

/**
 * An array of data
 */
typedef struct
{
	int count;
	char** items;
} StringArray;

/**
 * Make a table for return.  Specify the number of columns then call tableAddColumn() and tableAddStringColumn() to add
 * columns before returning the table
 *
 * @param env
 * @param columns the number of columns
 * @return the newly created table
 */
Table tableCreate(JNIEnv* env, int rows, int columns);

/**
 * Add a column to a table
 * @param env
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
 * Convert Layout to string name of Layout e.g. AIDA_ROW_MAJOR_LAYOUT returns "ROW_MAJOR"
 * @param layout layout
 * @return string
 */
jstring toLayoutString(JNIEnv* env, Layout layout);


/**
 * Make a single entry json_value array from a string.
 *
 * @param stringValue the string to add as the single entry in the json_value array
 * @return the Value
 */
Value asArrayValue(char * stringValue);

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
 *     // Do stuff
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
 *     short flag = 10;  // 10 is the default value
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
 *     // Do stuff
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

