# 2.5.1 - AIDA-PVA Module Functions

The AIDA-PVA Module is a module contained in the `STANDALONELIB` shared library that provides all the boilerplate
functionality needed to respond to **get** and **set** requests, marshal and unmarshal objects, and simple types across
the JNI boundary and between your Channel Provider, VMS, and any AIDA Module in `AIDASHR` that you need.

## Functions provided in AIDA-PVA Module

The AIDA-PVA Module contains the following functions that you should use to process arguments, extract parts of URIs,
create tables, allocate memory and raise exceptions:

- Argument Processing
    - ascanf() - See avscanf()
    - avscanf() - Reads data from the given arguments and stores them according to parameter format into the locations given by the additional arguments, as if scanf() was used, but reading from arguments instead of the standard input (stdin)
- URI and PMU Handling
    - groupNameFromUri() - _Get the Display group name from a URI._
    - pmuFromDeviceName() - _Get primary, micro and unit from a device name._
    - pmuStringFromUri() - _Get the pmu part of a URI._
    - secnFromUri() - _Get secondary from pseudo secondary (containing a colon) number from URI._
    - secondaryFromUri() - _Get secondary from URI._
    - uriLegacyName() - _Convert the given URI to the legacy AIDA name for low level functions that still require it
      that way._
    - uriToSlcName() - _Convert all URIs to slac names before making queries._
- Table Management
    - tableCreate() - _Make a Table for return to client._
    - tableAddStringColumn() - _Add a String column to the given Table._
    - tableAddFixedWidthStringColumn() - _Add fixed-width string data to a column in the given Table._
    - tableAddSingleRowBooleanColumn() - _Add a boolean column to a Table with only one row._
    - tableAddSingleRowByteColumn() - _Add a byte column to a Table with only one row._
    - tableAddSingleRowShortColumn() - _Add a short column to a Table with only one row._
    - tableAddSingleRowIntegerColumn() - _Add a integer column to a Table with only one row._
    - tableAddSingleRowLongColumn() - _Add a long column to a Table with only one row._
    - tableAddSingleRowFloatColumn() - _Add a float column to a Table with only one row._
    - tableAddSingleRowDoubleColumn() - _Add a double column to a Table with only one row._
    - tableAddSingleRowStringColumn() - _Add a string column to a Table with only one row._
- String Handling
    - endsWith() - _Check if a string ends with another string._
    - startsWith() - _Check if a string starts with another string._
- Memory Management
    - allocateMemory() - _Allocate memory and copy the source to it if specified._
    - releaseArguments() - _Free up any memory allocated for the given Arguments._
    - releaseArray() - _Free up any memory allocated the given scalar Array._
    - releaseStringArray() - _Free up any memory allocated for a StringArray._
    - releaseTable() - _Free up any memory allocated for the given Table._
    - releaseValue() - _Release all allocated memory in the given Value._
- Exception Handling:
    - aidaThrow() - _To log any exceptions and throw back to java._
    - aidaThrowNonOsException() - _To log any non-OS exceptions and throw back to java._

