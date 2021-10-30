# 1.1 - SLC Database Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Database data through AIDA-PVA. SLC
Database provides data from the database values in the SLC Control System. It also allows the setting of float scalar
database values. For general information on using AIDA-PVA see [Basic Users Guide to Aida](1_00_User_Guide.md), and the
EPICS javadoc.

## Summary

Acquires data for the named SLC device from the SLC database. Also allows setting of float scalar database values

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
| `<secn>` | Sets the value of a float scalar secondary in the SLC database. <br />The `VALUE` argument is a float with the desired value. |

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
| | `TABLE`    | return an table corresponding to the <br />required accompanying `TABLE_TYPE` argument |
| `TABLE_TYPE`|     | return table with one column and one <br />or more rows with the type specified |
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
| `TABLE` | `value` | depends on the accompanying <br />`TABLE_TYPE` argument | the scalar_array with <br />one or more values |

### <secn>  : set

_Parameters_

| | | |
| ----------- | -----------| ----------- | 
| Parameter Names | Parameter Values | Description | 
| `VALUE`*  | <floating point number> | float to set secondary |

_Return value_

None

## Examples

| | | 
|  ----------- |----------- |
| pvcall example | `pvcall "XCOR:LI03:120:LEFF" TYPE=FLOAT`|
|  | `pvcall "XCOR:LI31:41:BCON" VALUE=5.0`|
| Java Tests | SlcTest.java | 
| Matlab example |  |

## Test Output

```shell
java -cp aida-pva-tests.jar  "edu.stanford.slac.aida.test.SlcTest" -c
#################################################
AIDA-PVA SLC TESTS
#################################################

████ Test 1: Acquire scalar types SLC PMUS
_________________________________________________

get: XCOR:LI03:120:LEFF => FLOAT
    Float BACT: 0.262 ✔
get: XCOR:LI03:120//LEFF => FLOAT
    Backwards compatibility - using legacy channel format: Float BACT: 0.262 ✔
get: XCOR:LI03:120:LEFF => DOUBLE
    Double BACT: 0.2619999945163727 ✔
get: XCOR:LI03:120:VSTA => BOOLEAN
    Boolean VSTA: true ✔
get: XCOR:LI03:120:SETL => SHORT
    Short SETL: 3000 ✔
get: XCOR:LI03:120:ADCP => BYTE
    Byte ADCP: 48 ✔
get: XCOR:LI03:120:ADCP => CHAR
    Char ADCP: '0' ✔
get: XCOR:LI03:120:ADCP => CHAR
    WCHAR not possible in AIDA-PVA: Falling back to CHAR: ADCP: '0' ✔
get: XCOR:LI03:120:PSNM => STRING
    String PSNM: UNKNOWN  ✔
get: XCOR:LI03:120:PSNM => STRING
    WSTRING not possible in AIDA-PVA: Falling back to STRING: PSNM: UNKNOWN  ✔
get: XCOR:LI03:120:PSNM => BYTE_ARRAY
    Byte array PSNM:
        85 ✔
        78 ✔
        75 ✔
        78 ✔
        79 ✔
        87 ✔
        78 ✔
        32 ✔
get: XCOR:LI03:120:PSNM => CHAR_ARRAY
    Char array PSNM:
        'U' ✔
        'N' ✔
        'K' ✔
        'N' ✔
        'O' ✔
        'W' ✔
        'N' ✔
        ' ' ✔

████ Test 2: Acquire FLOAT type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:BACT => FLOAT
    BACT: 0.0 ✔

████ Test 3: Acquire LONG INT type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:VSTA => LONG
    VSTA: 8256 ✔

████ Test 4: Acquire array of FLOAT type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:IMMS => FLOAT_ARRAY
    IMMS:
        -5.0 ✔
        5.0 ✔
        0.0 ✔

████ Test 5: Acquire array of DOUBLE type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:IMMS => DOUBLE_ARRAY
    IMMS:
        -5.0 ✔
        5.0 ✔
        0.0 ✔

████ Test 6: Acquire array of SHORT type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:NSCY => SHORT_ARRAY
    NSCY:
        0 ✔
        0 ✔

████ Test 7: Acquire array of LONG type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:RAMP => LONG_ARRAY
    RAMP:
        0 ✔
        0 ✔

████ Test 8: Acquire array of BOOLEAN type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:RAMP => BOOLEAN_ARRAY
    RAMP:
        true ✔
        false ✔

████ Test 9: Acquire FLOAT type SLC Database PV
_________________________________________________

get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔

████ Test 10: Looping acquire FLOAT type SLC Database PV pausing 10 seconds between each.
_________________________________________________

get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔

████ Test 11: ASTS channel name test
_________________________________________________

get: ASTS:LI00:ISOPLAN2:DATA => FLOAT
    DATA: 0.0 ✔
get: ASTS:PR02:VP3012:DATA => FLOAT
    DATA: 0.0 ✔
get: ASTS:PR02:VP3012:STAT => SHORT
    STAT: 40 ✔
get: ASTS:PR02:T2092BLW:LIM1 => FLOAT
    LIM1: 32.0 ✔
get: ASTS:PR02:T2092BLW:LIM2 => FLOAT
    LIM2: 150.0 ✔
get: ASTS:PR02:T2092BLW:LIMS => FLOAT_ARRAY
    LIMS:
        32.0 ✔
        150.0 ✔
get: ASTS:PR02:T2092QUA:SCAL => FLOAT_ARRAY
    SCAL:
        32.0 ✔
        1.8 ✔
get: ASTS:PR02:T2092QUA:RAW => FLOAT
    RAW: 1.0E-9 ✔
get: ASTS:PR02:T2092QUA:STAT => STRING
    STAT: CamErr   RED        ✔

████ Test 12: ASTS channel name with one of the pseudo-secondaries STAT, CTRL, or VSTA
_________________________________________________

get: ASTS:PR02:T2092QUA:STAT => STRING_ARRAY
    STAT:
        CamErr   ✔
        RED      ✔
          ✔

████ Test 13: Set value test
_________________________________________________

set: XCOR:LI31:41:BCON (VALUE=[5.0]) ✔
```
