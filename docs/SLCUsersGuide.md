# SLC Database Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Database data through AIDA-PVA. 
SLC Database provides data from the database values in the SLC Control System. 
It also allows the setting of float scalar database values.  For general
information on using AIDA-PVA see [Basic Users Guide to Aida](UserGuide.md), and the EPICS javadoc. 

# AIDA-PVA Data Provider for SLC Database Data

## Summary

Acquires data for the named SLC device from the SLC database. Also allows setting of float scalar database values

## Examples
| | | 
|  ----------- |----------- |
| pvcall example | `pvcall "XCOR:LI03:120:LEFF" TYPE=FLOAT`|
|  | `pvcall "XCOR:LI31:41:BCON" VALUE=5.0`|
| Java Tests | SlcTest.java | 
| Matlab example |  |

## Instances and Attributes

| | | |
| ----------- | ----------- | ----------- |
| **get** | Syntax    | `<prim>:<micr>:<unit>:<secn>` |
| | | `ASTS:<micr>:<channel>:<pseudo-secn>` |
| | Examples | `XCOR:LI03:120:LEFF` | 
| | | `ASTS:PR02:VP3012:DATA` |
| **set** | Syntax | `<prim>:<micr>:<unit>:<secn>` |
| | Examples|  `XCOR:LI31:41:BCON` |

## Attribute operation summary

| | |
| ----------- | -----------  |
| Attribute | Description |
| `<secn>` | Gets SLC db device data for a named device - the channel name |
| `<secn>` | Sets the value of a float scalar secondary in the SLC database. The `VALUE` argument is a float with the desired value. |

## Attribute operations
### <secn>  : get

_Parameters_

| | | |
| ----------- | -----------| ----------- | 
| Parameter Names | Parameter Values | Description | 
| | | |
| `TYPE`*  |  | Determines the return type of the request |
| | `BOOLEAN`   | return a boolean value |
| | `BYTE`      | return a byte value |
| | `SHORT`     | return a short value |
| | `INTEGER`   | return a integer value |
| | `LONG`      | return a long value |
| | `FLOAT`     | return a float value |
| | `DOUBLE`    | return a double value |
| | `STRING`    | return a string value |
| | `BOOLEAN_ARRAY`   | return an array of boolean values |
| | `BYTE_ARRAY`      | return an array of byte values |
| | `SHORT_ARRAY`     | return an array of short values |
| | `INTEGER_ARRAY`   | return an array of integer values |
| | `LONG_ARRAY`      | return an array of long values |
| | `FLOAT_ARRAY`     | return an array of float values |
| | `DOUBLE_ARRAY`    | return an array of double values |
| | `STRING_ARRAY`    | return an array of string values |
| | `TABLE`    | return an table corresponding to the required accompanying `TABLE_TYPE` argument |
| `TABLE_TYPE`|     | return table with one column and one or more rows with the type specified |
| | `BOOLEAN_ARRAY`   | table contains a single boolean value |
| | `BYTE_ARRAY`      | table contains a single byte value |
| | `SHORT_ARRAY`     | table contains a single short value |
| | `INTEGER_ARRAY`   | table contains a single integer value |
| | `LONG_ARRAY`      | table contains a single long value |
| | `FLOAT_ARRAY`     | table contains a single float value |
| | `DOUBLE_ARRAY`    | table contains a single double value |
| | `STRING_ARRAY`    | table contains a single string value |

_Return value_

| | | | |
|----------- | ----------- | -----------  |-----------  |
| TYPE  | Return Column | Column Type |Description |
| any scalar type |  |  | the scalar value |
| any scalar array type |  |  | the scalar array |
| `TABLE` | `value` | depends on the accompanying `TABLE_TYPE` argument | the scalar_array with one or more values |


### <secn>  : set
_Parameters_

| | | |
| ----------- | -----------| ----------- | 
| Parameter Names | Parameter Values | Description | 
| `VALUE`*  | <floating point number> | float to set secondary |

_Return value_

None
