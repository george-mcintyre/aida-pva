#include <jni.h>
#include <stdlib.h>
#include <string.h>

#include "aida_types.h"
#include "aida_server_helper.h"

static void tableColumn(Table* table, int columnNumber, Type aidaType, size_t elementSize);

static Type arrayTypeOf(Type type);
static size_t elementSizeOfOf(Type type);

/**
 * Convert Type to string
 * @param env env
 * @param type type
 * @return string
 */
jstring toTypeString(JNIEnv* env, Type type)
{
	switch (type) {
	case AIDA_BOOLEAN_TYPE :
		return (*env)->NewStringUTF(env, "BOOLEAN");
	case AIDA_BYTE_TYPE :
		return (*env)->NewStringUTF(env, "BYTE");
	case AIDA_SHORT_TYPE :
		return (*env)->NewStringUTF(env, "SHORT");
	case AIDA_INTEGER_TYPE :
		return (*env)->NewStringUTF(env, "INTEGER");
	case AIDA_LONG_TYPE :
		return (*env)->NewStringUTF(env, "LONG");
	case AIDA_FLOAT_TYPE :
		return (*env)->NewStringUTF(env, "FLOAT");
	case AIDA_DOUBLE_TYPE :
		return (*env)->NewStringUTF(env, "DOUBLE");
	case AIDA_STRING_TYPE :
		return (*env)->NewStringUTF(env, "STRING");
	case AIDA_BOOLEAN_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "BOOLEAN_ARRAY");
	case AIDA_BYTE_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "BYTE_ARRAY");
	case AIDA_SHORT_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "SHORT_ARRAY");
	case AIDA_INTEGER_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "INTEGER_ARRAY");
	case AIDA_LONG_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "LONG_ARRAY");
	case AIDA_FLOAT_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "FLOAT_ARRAY");
	case AIDA_DOUBLE_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "DOUBLE_ARRAY");
	case AIDA_STRING_ARRAY_TYPE :
		return (*env)->NewStringUTF(env, "STRING_ARRAY");
	case AIDA_TABLE_TYPE :
	default :
		return (*env)->NewStringUTF(env, "TABLE");
	}
}

/**
 * Convert Layout to string
 * @param env env
 * @param layout layout
 * @return string
 */
jstring toLayoutString(JNIEnv* env, Layout layout)
{
	switch (layout) {
	case AIDA_ROW_MAJOR_LAYOUT:
		return (*env)->NewStringUTF(env, "ROW_MAJOR");
	case AIDA_COLUMN_MAJOR_LAYOUT:
	default:
		return (*env)->NewStringUTF(env, "COLUMN_MAJOR");
	}
}

static void tableColumn(Table* table, int columnNumber, Type aidaType, size_t elementSize)
{
	table->types[columnNumber] = aidaType;
	table->ppData[columnNumber] = calloc(table->rowCount, elementSize);
}

void tableStringColumn(Table* table, int columnNumber, int maxStringLength)
{
	tableColumn(table, columnNumber, AIDA_STRING_ARRAY_TYPE, sizeof(char*));

	// allocate data for each string too
	char** stringArray = table->ppData[columnNumber];
	for (int row = 0; row < table->rowCount; row++) {
		stringArray[row] = malloc(maxStringLength);
	}
}

void tableFloatColumn(Table* table, int columnNumber)
{
	tableColumn(table, columnNumber, AIDA_FLOAT_ARRAY_TYPE, sizeof(float));
}

void tableLongColumn(Table* table, int columnNumber)
{
	tableColumn(table, columnNumber, AIDA_LONG_ARRAY_TYPE, sizeof(unsigned long));
}

void tableBooleanColumn(Table* table, int columnNumber)
{
	tableColumn(table, columnNumber, AIDA_BOOLEAN_ARRAY_TYPE, sizeof(unsigned char));
}

void tableByteColumn(Table* table, int columnNumber)
{
	tableColumn(table, columnNumber, AIDA_BYTE_ARRAY_TYPE, sizeof(unsigned char));
}

void tableShortColumn(Table* table, int columnNumber)
{
	tableColumn(table, columnNumber, AIDA_BOOLEAN_ARRAY_TYPE, sizeof(short));
}

void tableIntegerColumn(Table* table, int columnNumber)
{
	tableColumn(table, columnNumber, AIDA_BOOLEAN_ARRAY_TYPE, sizeof(int));
}

void tableDoubleColumn(Table* table, int columnNumber)
{
	tableColumn(table, columnNumber, AIDA_BOOLEAN_ARRAY_TYPE, sizeof(double));
}

Table* initTable(JNIEnv* env, Table* table)
{
	table->_currentColumn = 0;  // Reset current column so that any addColumn() calls are correct

	// Allocate space for the table columns
	table->ppData = calloc(table->columnCount, sizeof(void*));
	if (!table->ppData) {
		table->columnCount = 0;
		char errorString[BUFSIZ];
		sprintf(errorString, "Unable to allocate memory for table: %ld", table->columnCount * sizeof(void*));
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
		return NULL;
	}

	// Allocate space for the column types
	table->types = calloc(table->columnCount, sizeof(Type*));
	if (!table->types) {
		char errorString[BUFSIZ];
		sprintf(errorString, "Unable to allocate memory for table types: %ld", table->columnCount * sizeof(Type*));
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION, errorString);
		return NULL;
	}

	return table;
}

/**
 * This reads data from an that is itself a list of pointers to strings
 * We allocate just enough space to store the strings in our table
 *
 * @param env
 * @param table
 * @param data
 */
void addStringColumn(JNIEnv* env, Table* table, void** data)
{
	addColumn(env, table, AIDA_STRING_ARRAY_TYPE, data);

	// allocate data for each string too
	char** stringArray = table->ppData[table->_currentColumn];
	for (int row = 0; row < table->rowCount; row++) {
		unsigned long len = strlen((char*)*data);
		stringArray[row] = malloc(len + 1);
		strncpy(stringArray[row], *data, len + 1);
	}

	table->_currentColumn++;
}

/**
 * This reads data from an allocated space that is rows * width with each string occupying width characters
 * Though the strings are null terminated if there is space, there is no guarantee so an exact number of
 * bytes is copied.  Each string in the table is allocated maximally.
 *
 * @param env
 * @param table
 * @param data
 * @param width
 */
void addFixedWidthStringColumn(JNIEnv* env, Table* table, void* data, int width)
{
	addColumn(env, table, AIDA_STRING_ARRAY_TYPE, data);

	// allocate data for each string too
	char** stringArray = table->ppData[table->_currentColumn];
	char* dataPointer = (char*)data;
	for (int row = 0; row < table->rowCount; row++, dataPointer += width) {
		stringArray[row] = malloc(width + 1);
		strncpy(stringArray[row], dataPointer, width);
		stringArray[row][width] = 0x0;
	}

	table->_currentColumn++;
}

void addColumn(JNIEnv* env, Table* table, Type type, void* data)
{
	// Table full?
	if (table->_currentColumn == table->columnCount) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"Internal Error: more columns added than table size");
		return;
	}

	// No Data supplied ?
	if (!data) {
		aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
				"Internal Error: Attempt to add column with no data");
		return;
	}

	// Correct type for tables
	type = arrayTypeOf(type);

	// Set column type, and allocate space
	tableColumn(table, table->_currentColumn, type, elementSizeOfOf(type));

	// Rest of processing for strings is done in addStringColumn
	if (type == AIDA_STRING_ARRAY_TYPE) {
		return;
	}

	// Add data to column
	for (int row = 0; row < table->rowCount; row++) {
		// add data and increment pointer based on type
		switch (type) {
		case AIDA_BOOLEAN_ARRAY_TYPE:
		case AIDA_BYTE_ARRAY_TYPE:
			((unsigned char*)(table->ppData[table->_currentColumn]))[row] = ((unsigned char*)data)[row];
			break;
		case AIDA_SHORT_ARRAY_TYPE:
			((short*)(table->ppData[table->_currentColumn]))[row] = ((short*)data)[row];
			break;
		case AIDA_INTEGER_ARRAY_TYPE:
			((int*)(table->ppData[table->_currentColumn]))[row] = ((int*)data)[row];
			break;
		case AIDA_LONG_ARRAY_TYPE:
			((long*)(table->ppData[table->_currentColumn]))[row] = ((long*)data)[row];
			break;
		case AIDA_FLOAT_ARRAY_TYPE:
			((float*)(table->ppData[table->_currentColumn]))[row] = ((float*)data)[row];
			break;
		case AIDA_DOUBLE_ARRAY_TYPE:
			((double*)(table->ppData[table->_currentColumn]))[row] = ((double*)data)[row];
			break;
		default:
			aidaThrowNonOsException(env, UNABLE_TO_GET_DATA_EXCEPTION,
					"Internal Error: Call to addColumn() un-supported type");
			return;
		}
	}

	table->_currentColumn++;
}

void addSingleRowFloatColumn(JNIEnv* env, Table* table, float data)
{
	addColumn(env, table, AIDA_FLOAT_TYPE, &data);
}

void addSingleRowLongColumn(JNIEnv* env, Table* table, long data)
{
	addColumn(env, table, AIDA_LONG_TYPE, &data);
}

void addSingleRowBooleanColumn(JNIEnv* env, Table* table,  unsigned char data)
{
	addColumn(env, table, AIDA_BOOLEAN_TYPE, &data);
}

void addSingleRowByteColumn(JNIEnv* env, Table* table,  unsigned char data)
{
	addColumn(env, table, AIDA_BYTE_TYPE, &data);
}

void addSingleRowShortColumn(JNIEnv* env, Table* table, short data)
{
	addColumn(env, table, AIDA_SHORT_TYPE, &data);
}

void addSingleRowIntegerColumn(JNIEnv* env, Table* table, int data)
{
	addColumn(env, table, AIDA_INTEGER_TYPE, &data);
}

void addSingleRowDoubleColumn(JNIEnv* env, Table* table, double data)
{
	addColumn(env, table, AIDA_DOUBLE_TYPE, &data);
}

void addSingleRowStringColumn(JNIEnv* env, Table* table, char* data)
{
	addStringColumn(env, table, (void**)&data);
}

/**
 * Return the corresponding array type of the given type
 *
 * @param type the given type
 * @return the corresponding array type
 */
static Type arrayTypeOf(Type type)
{
	switch (type) {
	case AIDA_BOOLEAN_TYPE:
		type = AIDA_BOOLEAN_ARRAY_TYPE;
		break;
	case AIDA_BYTE_TYPE:
		type = AIDA_BYTE_ARRAY_TYPE;
		break;
	case AIDA_SHORT_TYPE:
		type = AIDA_SHORT_ARRAY_TYPE;
		break;
	case AIDA_INTEGER_TYPE:
		type = AIDA_INTEGER_ARRAY_TYPE;
		break;
	case AIDA_LONG_TYPE:
		type = AIDA_LONG_ARRAY_TYPE;
		break;
	case AIDA_FLOAT_TYPE:
		type = AIDA_FLOAT_ARRAY_TYPE;
		break;
	case AIDA_DOUBLE_TYPE:
		type = AIDA_DOUBLE_ARRAY_TYPE;
		break;
	case AIDA_STRING_TYPE:
		type = AIDA_STRING_ARRAY_TYPE;
		break;
	default:
		break;
	}
	return type;
}

/**
 * Return the corresponding data element size for the given type
 *
 * @param type the given type
 * @return the corresponding data element size
 */
static size_t elementSizeOfOf(Type type)
{
	size_t size;

	switch (type) {
	case AIDA_BOOLEAN_TYPE:
	case AIDA_BOOLEAN_ARRAY_TYPE:
		size = sizeof(unsigned char);
		break;
	case AIDA_BYTE_TYPE:
	case AIDA_BYTE_ARRAY_TYPE:
		size = sizeof(unsigned char);
		break;
	case AIDA_SHORT_TYPE:
	case AIDA_SHORT_ARRAY_TYPE:
		size = sizeof(short);
		break;
	case AIDA_INTEGER_TYPE:
	case AIDA_INTEGER_ARRAY_TYPE:
		size = sizeof(int);
		break;
	case AIDA_LONG_TYPE:
	case AIDA_LONG_ARRAY_TYPE:
		size = sizeof(long);
		break;
	case AIDA_FLOAT_TYPE:
	case AIDA_FLOAT_ARRAY_TYPE:
		size = sizeof(float);
		break;
	case AIDA_DOUBLE_TYPE:
	case AIDA_DOUBLE_ARRAY_TYPE:
		size = sizeof(double);
		break;
	case AIDA_STRING_TYPE:
	case AIDA_STRING_ARRAY_TYPE:
		size = sizeof(char*);
		break;

	default:
		size = 0;
	}
	return size;
}

/**
 * Make a table for return.  Specify the number of columns then call addColumn() and addStringColumn() to add
 * columns before returning the table
 *
 * @param env
 * @param columns the number of columns
 * @return the newly created table
 */
Table makeTable(JNIEnv* env, int rows, int columns)
{
	Table table;
	memset(&table, 0, sizeof(table));
	table._currentColumn = 0;  // Reset current column so that any addColumn() calls are correct
	table.columnCount = 0;

	if ( rows <= 0 ) {
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, "Attempt to allocate a zero length table");
		return table;
	}

	// Allocate space for the table columns
	table.ppData = calloc(columns, sizeof(void*));
	if (!table.ppData) {
		char errorString[BUFSIZ];
		sprintf(errorString, "Unable to allocate memory for table: %ld", columns * sizeof(void*));
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, errorString);
		return table;
	}

	// Allocate space for the column types
	table.types = calloc(columns, sizeof(Type*));
	if (!table.types) {
		free(table.ppData);
		char errorString[BUFSIZ];
		sprintf(errorString, "Unable to allocate memory for table types: %ld", columns * sizeof(Type*));
		aidaThrowNonOsException(env, AIDA_INTERNAL_EXCEPTION, errorString);
		return table;
	}

	table.rowCount = rows;
	table.columnCount = columns;
	return table;
}
