#include <jni.h>

#ifndef _Included_aida_types
#define _Included_aida_types
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Defines the permissible types of fields
 */
typedef enum
{
	NONE,
	BOOLEAN,
	BYTE,
	SHORT,
	INTEGER,
	LONG,
	FLOAT,
	DOUBLE,
	STRING,
	BOOLEAN_ARRAY,
	BYTE_ARRAY,
	SHORT_ARRAY,
	INTEGER_ARRAY,
	LONG_ARRAY,
	FLOAT_ARRAY,
	DOUBLE_ARRAY,
	STRING_ARRAY,
	TABLE
} Type;

/**
 * Convert Type to string
 * @param type type
 * @return string
 */
jstring toTypeString(JNIEnv* env, Type type);

/**
 * Defines the layout of TABLES.
 */
typedef enum
{
	NONE,
	COLUMN_MAJOR,
	ROW_MAJOR
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
	const char* name;
	const char* value;
} Argument;

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
 *  ppData:         the data.  Either and array of [rows][columns] or an array of [columns][rows].
 */
typedef struct
{
	int columnCount;
	int rowCount;
	Type* types;
	void** ppData;
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
 * Convert Layout to string
 * @param layout layout
 * @return string
 */
jstring toLayoutString(JNIEnv* env, Layout layout);

#ifdef __cplusplus
}
#endif
#endif

