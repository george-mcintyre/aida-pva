# Reference Provider Users Guide

This section describes what an AIDA-PVA user should know about accessing the Reference Provider through AIDA-PVA. This data provider:
- Testing of your application by providing data in any way you want it.
- Allowing you to test most scenarios with predicable results.
- Provides a way to test the AIDA-PVA framework

see [Basic Users Guide to Aida](UserGuide.md), and the EPICS javadoc.

# AIDA-PVA Reference Provider

## Summary

Supports **get** and **set** operations. 

- The **get** operation allows getting channels of all supported types. 
- The **set** operation allows setting channels returning all supported types.

## Examples

| | | |
|  ----------- |----------- |----------- |
| pvcall examples | `pvcall AIDA:SAMPLE:TEST:attribute01` | Get the status code for the trigger device on the beam code |
|  | `AIDA:SAMPLE:TEST:attribute02` |  Get the status string |
|  | `AIDA:SAMPLE:TEST:attribute03 x=3` |  Perform specified operation to trigger device on beam code |
|  | `pvcall AIDA:SAMPLE:TEST:attribute30 VALUE=100` |  Perform set operation for multiknob file knob rotation |
| Java Tests | ReferenceTest.java | |

## Instances and Attributes

| | | |
| ----------- | ----------- | ----------- |
| **get** | Syntax    | `AIDA:SAMPLE:<anything>:attribute<1-20>` |
| | Examples | `AIDA:SAMPLE:TEST:attribute01` |
| **get** | Syntax    | `AIDA:SAMPLE:<anything>:attribute32` |
| | Examples | `AIDA:SAMPLE:TEST:attribute32` |
| **set** | Syntax    | `AIDA:SAMPLE:<anything>:attribute<30-31>` |
| | Examples | `AIDA:SAMPLE:TEST:attribute30` |
| not supported exception test| Syntax    | `AIDA:SAMPLE:<anything>:attribute00` |
| | Examples | `AIDA:SAMPLE:<anything>:attribute00` |

## Attribute operation summary

| | | |
| ----------- | -----------  | -----------  |
| Attribute | operation |Description |
| `attribute01` | **get** |  Gets a boolean true value |
| `attribute02` | **get** |  Gets a byte 2 value |
| `attribute03` | **get** |  Gets a short 3 value |
| `attribute04` | **get** |  Gets a integer 4 value |
| `attribute05` | **get** |  Gets a long 5L value |
| `attribute06` | **get** |  Gets a float 6.6 value |
| `attribute07` | **get** |  Gets a double 7.7 value |
| `attribute08` | **get** |  Gets a string "eight" value |
| `attribute11` | **get** |  Gets a boolean array with a single true value |
| `attribute12` | **get** |  Gets a byte array with a single 2 value |
| `attribute13` | **get** |  Gets a short array with a single 3 value |
| `attribute14` | **get** |  Gets a integer array with a single 4 value |
| `attribute15` | **get** |  Gets a long array with a single 5L value |
| `attribute16` | **get** |  Gets a float array with a single 6.6 value |
| `attribute17` | **get** |  Gets a double array with a single 7.7 value |
| `attribute18` | **get** |  Gets a string array with a single "eight" value |
| `attribute20` | **get** |  Gets a table with 8 columns, one for each of the supported types |
| `attribute30` | **set** |  Simulate setting a value with no return value |
| `attribute31` | **set** |  Simulate setting a value with a table return value |

## Attribute operations

### attribute01 : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `X`  | boolean value. | If present it will be returned from the call |

_Return value_

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `BOOLEAN` |  `true` unless parameter `X` is present in which case the value of `X` will be returned |

### attribute02 : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `X`  | byte value. | If present `X` bitwise OR with `2` will be returned from the call |

_Return value_

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `BYTE` |  `2` unless parameter `X` is present in which case `X` bitwise OR with `2` will be returned from the call |

### attribute03 : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `X`  | short value. | If present `X` + `3` will be returned from the call |

_Return value_

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `SHORT` |  `3` unless parameter `X` is present in which case `X` + `3` will be returned from the call |

### attribute04 : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `X`  | integer value. | If present `X` + `4` will be returned from the call |

_Return value_

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `INTEGER` |  `4` unless parameter `X` is present in which case `X` + `4` will be returned from the call |

### attribute05 : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `X`  | long value. | If present `X` + `5L` will be returned from the call |

_Return value_

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `LONG` |  `5L` unless parameter `X` is present in which case `X` + `5L` will be returned from the call |

### attribute06 : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `X`  | float value. | If present `X` * `6.6f` will be returned from the call |

_Return value_

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `FLOAT` |  `6.6f` unless parameter `X` is present in which case `X` * `6.6f` will be returned from the call |

### attribute07 : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `X`  | double value. | If present `X` * `7.7` will be returned from the call |

_Return value_

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `DOUBLE` |  `7.7` unless parameter `X` is present in which case `X` * `7.7` will be returned from the call |

### attribute08 : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `X`  | string value. | If present `"eight: " + X` will be returned from the call |

_Return value_

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `STRING` |  `"eight"` unless parameter `X` is present in which case `"eight: " + X` will be returned from the call |

### attribute11 : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `X`  | boolean array value. | If present it will be returned from the call |

_Return value_

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `BOOLEAN_ARRAY` |  `true` unless parameter `X` is present in which case the value of `X` will be returned |

### attribute12 : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `X`  | byte array value. |  If present every element in array `X` bitwise OR with `12` will be returned from the call |

_Return value_

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `BYTE_ARRAY` |  `12` unless parameter `X` is present in which case every element in array `X` bitwise OR with `12` will be returned from the call |

### attribute13 : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `X`  | short array value. | If present every element in array `X` + `13` will be returned from the call |

_Return value_

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `SHORT_ARRAY` |  `13` unless parameter `X` is present in which case every element in array `X` + `13` will be returned from the call |

### attribute14 : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `X` | integer array value. |  If present every element in array `X` + `14` will be returned from the call |

_Return value_

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `INTEGER_ARRAY` |  `14` unless parameter `X` is present in which case every element in array `X` + `14` will be returned from the call |

### attribute15 : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `X` |  long array value |  If present every element in array `X` + `15L` will be returned from the call |

_Return value_

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `LONG_ARRAY` |  `15L` unless parameter `X` is present in which case every element in array `X` + `15L` will be returned from the call |

### attribute16 : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `X` | float array value. |  If present every element in array `X` * `16.6f` will be returned from the call |

_Return value_

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `FLOAT_ARRAY` |  `6.6f` unless parameter `X` is present in which case every element in array `X` * `16.6f` will be returned from the call |

### attribute17 : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `X` |  double array value. |  If present every element in array `X` * `17.7` will be returned from the call |

_Return value_

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `DOUBLE_ARRAY` |  `7.7` unless parameter `X` is present in which case every element in array `X` * `17.7` will be returned from the call |

### attribute18 : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `X` | string array value. |  If present every element in array `X` will be returned as `"eighteen: " + X` |

_Return value_

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `STRING_ARRAY` |  `"eighteen"` unless parameter `X` is present in which case every element in array `X` will be returned as `"eighteen: " + X`|


### attribute20 : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values | Description | 
| `X` | structure with each of the supported scalar types in field names as follows: | If present every element in array `X` be subjected to the same operation as the scalar channel would undergo.  e.g., the integer value would be increased by 4.   |
| | `"boolean"`| a boolean value  |
| | `"byte"`| a byte value  |
| | `"short"`| a short value  |
| | `"integer"`| a integer value  |
| | `"long"`| a long value |  |
| | `"float"`| a float value |  |
| | `"double"`| a double value | |
| | `"string"`| a string value  |

_Return value_

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `STRING_ARRAY` |  If `X` present every element in array `X` be subjected to the same operation as the scalar channel would undergo.  e.g., the integer value would be increased by 4.  |

