/** @file
 *  @brief The Header File for the type enumerations, unions, and typedefs.
 *       **CMS**=C_INC
 */
#ifndef aida_pva_types_h
#define aida_pva_types_h
#ifdef __cplusplus
extern "C" {
#endif

#include <jni.h>
#include <stdbool.h>
#include "aida_pva_json.h"
#include "aida_pva.h"

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
 * It contains a count of the total number of arguments - #argumentCount - and a pointer
 * to that many {@link Argument}s - #arguments.
 * {@link Argument}s from the client are stored as strings, and converted to whatever type is required whenever
 * they are used.  But floating points are different because they require more precision and significance
 * than string representations can give.  For this reason floating points are additionally passed in the
 * FloatingPointValue array - #floatingPointValues - where #floatingPointValuesCount is the count of how many of them there are.
 */
typedef struct
{
	int argumentCount;                                ///< The number of arguments sent with this request
	int floatingPointValuesCount;                ///< The number of floating point numbers in the arguments of this request
	Argument* arguments;                            ///< The array of Arguments
	FloatingPointValue* floatingPointValues;        ///< The array of FloatingPointValue
} Arguments;

/**
 * Table structure.
 * This structure holds everything that a Native Channel Provider needs for returning a Table
 * to the client.  A table is a set of homogeneously sized vectors representing columns of data.
 * The configuration of the table (labels, names, descriptions, etc) is handled outside of the
 * Native Channel Provider, so all the implementation needs to do is provide the raw data for the
 * columns.
 * @see
 * Implementors don't manipulate these structures directly as [AIDA-PVA Module](../../../docs/4_0_AIDA-PVA_Module.md) provides
 * table manipulation functions:
 *  - tableCreate()
 *  - tableAddColumn()
 *  - tableAddSingleRowFloatColumn()
 *  - tableAddSingleRowLongColumn()
 *  - tableAddSingleRowBooleanColumn()
 *  - tableAddSingleRowByteColumn()
 *  - tableAddSingleRowShortColumn()
 *  - tableAddSingleRowIntegerColumn()
 *  - tableAddSingleRowDoubleColumn()
 *  - tableAddSingleRowStringColumn()
 *  - tableAddFixedWidthStringColumn()
 *  - tableAddStringColumn()
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
	int count;              ///< The number of items in this array
	void* items;            ///< The items in this array
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
	int count;               ///< The number of items in this array
	char** items;            ///< The items in this array - pointers to the strings you allocate
} StringArray;

#ifdef __cplusplus
}
#endif
#endif

