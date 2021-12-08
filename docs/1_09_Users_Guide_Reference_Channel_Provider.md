# 1.9 - Reference Channel Provider Users Guide

This section describes what an AIDA-PVA user should know about accessing the Reference Channel Provider through AIDA-PVA. This
data provider:

- Testing of your application by providing data in any way you want it.
- Allowing you to test most scenarios with predicable results.
- Provides a way to test the AIDA-PVA framework

see [Basic Users Guide to Aida](1_00_User_Guide.md), and the EPICS javadoc.

## Summary

Supports **get** and **set** operations.

- The **get** operation allows getting channels of all supported types.
- The **set** operation allows setting channels returning all supported types.

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
| `BYTE_ARRAY` |  `12` unless parameter `X` is present in which case every element in <br />array `X` bitwise OR with `12` will be returned from the call |

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
| `SHORT_ARRAY` |  `13` unless parameter `X` is present in which case every element in <br />array `X` + `13` will be returned from the call |

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
| `INTEGER_ARRAY` |  `14` unless parameter `X` is present in which case every element in <br />array `X` + `14` will be returned from the call |

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
| `LONG_ARRAY` |  `15L` unless parameter `X` is present in which case every element in <br />array `X` + `15L` will be returned from the call |

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
| `FLOAT_ARRAY` |  `6.6f` unless parameter `X` is present in which case every element <br />in array `X` * `16.6f` will be returned from the call |

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
| `DOUBLE_ARRAY` |  `7.7` unless parameter `X` is present in which case every element <br />in array `X` * `17.7` will be returned from the call |

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
| `STRING_ARRAY` |  `"eighteen"` unless parameter `X` is present in which case every <br />element in array `X` will be returned as `"eighteen: " + X`|

### attribute20 : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values | Description | 
| `X` | structure with each of the supported scalar types in field names as follows: | If present every element in array `X` be subjected to the same operation as the scalar channel would undergo. e.g., the integer value would be increased by 4.   |
| | `"boolean"`| a boolean value  |
| | `"byte"` |  a byte value  |
| | `"short"` | a short value  |
| | `"integer"` | an integer value  |
| | `"long"` | a long value  |
| | `"float"` | a float value  |
| | `"double"` | a double value  |
| | `"string"` | a string value | 

_Return value_

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `STRING_ARRAY` |  If `X` present every element in array<br /> `X` be subjected to the same operation<br /> as the scalar channel would undergo. <br />e.g., the integer value would be <br />increased by 4.  |

## Test Output

```shell
java -cp aida-pva-tests.jar  "edu.stanford.slac.aida.test.ReferenceTest" -c
#################################################
AIDA-PVA REFERENCE TESTS
#################################################

████ Test 1: Get Boolean
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute01 ()
    Boolean: true ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=1)
    Boolean: x[1] : true ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=0)
    Boolean: x[0] : false ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=true)
    Boolean: x["true"] : true ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=false)
    Boolean: x["false"] : false ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=TRUE)
    Boolean: x["TRUE"] : true ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=FALSE)
    Boolean: x["FALSE"] : false ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=T)
    Boolean: x["T"] : true ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=F)
    Boolean: x["F"] : false ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=t)
    Boolean: x["t"] : true ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=f)
    Boolean: x["f"] : false ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=Y)
    Boolean: x["Y"] : true ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=N)
    Boolean: x["N"] : false ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=y)
    Boolean: y["y"] : true ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=n)
    Boolean: x["n"] : false ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=YES)
    Boolean: x["YES"] : true ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=NO)
    Boolean: x["NO"] : false ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=yes)
    Boolean: x["yes"] : true ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=no)
    Boolean: x["no"] : false ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=true)
    Boolean: x[Boolean.TRUE]: true ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=false)
    Boolean: x[Boolean.FALSE]: false ✔

████ Test 2: Get Byte
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute02 ()
    Byte: 2 ✔
get: AIDA:SAMPLE:TEST:attribute02 (TYPE=BYTE) => CHAR
    Unprintable Char: 0x2: '' ✔
get: AIDA:SAMPLE:TEST:attribute02 (x=4)
    Byte: 0x2 | x[0x4]: 54 ✔
get: AIDA:SAMPLE:TEST:attribute02 (x=8)
    Byte: 0x2 | x[0x8]: 58 ✔
get: AIDA:SAMPLE:TEST:attribute02 (x=72, TYPE=BYTE) => CHAR
    Char: 0x2 | x[0x48]: '7' ✔

████ Test 3: Get Short
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute03 ()
    Short: 3 ✔
get: AIDA:SAMPLE:TEST:attribute03 (x=3)
    Short: 3 + x[3]: 6 ✔
get: AIDA:SAMPLE:TEST:attribute03 (x=-3)
    Short: 3 + x[-3]: 0 ✔
get: AIDA:SAMPLE:TEST:attribute03 (x=32767)
    Max: Short: 3 + x[32767]: -32766 ✔
get: AIDA:SAMPLE:TEST:attribute03 (x=-32768)
    Min: Short: 3 + x[-32768]: -32765 ✔

████ Test 4: Get Integer
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute04 ()
    Integer: 4 ✔
get: AIDA:SAMPLE:TEST:attribute04 (x=4)
    Integer: 4 + x[4]: 8 ✔
get: AIDA:SAMPLE:TEST:attribute04 (x=-4)
    Integer: 4 + x[-4]: 0 ✔
get: AIDA:SAMPLE:TEST:attribute04 (x=2147483647)
    Max: Integer: 4 + x[2147483647]: -2147483645 ✔
get: AIDA:SAMPLE:TEST:attribute04 (x=-2147483648)
    Min: Integer: 4 + x[-2147483648]: -2147483644 ✔

████ Test 5: Get Long
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute05 ()
    Long: 5 ✔
get: AIDA:SAMPLE:TEST:attribute05 (x=5)
    Long: 5 + x[5]: 10 ✔
get: AIDA:SAMPLE:TEST:attribute05 (x=-5)
    Long: 5 + x[-5]: 0 ✔
get: AIDA:SAMPLE:TEST:attribute05 (x=2147483647)
    Max (32bit): Long: 5 + x[2147483647]: -2147483644 ✔
get: AIDA:SAMPLE:TEST:attribute05 (x=-2147483648)
    Min (32bit): Long: 5 + x[-2147483648]: -2147483643 ✔
get: AIDA:SAMPLE:TEST:attribute05 (x=9223372036854775807)
 AidaInternalException; can't convert argument "9223372036854775807" to long ✔
get: AIDA:SAMPLE:TEST:attribute05 (x=-9223372036854775808)
 AidaInternalException; can't convert argument "-9223372036854775808" to long ✔

████ Test 6: Get Float
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute06 ()
    Float: 6.6 ✔
get: AIDA:SAMPLE:TEST:attribute06 (x=6.6)
    Float: 6.6f * x[6.6f]: 0.0 ✔
get: AIDA:SAMPLE:TEST:attribute06 (x=-6.6)
    Float: 6.6f * x[-6.6f]: 0.0 ✔
get: AIDA:SAMPLE:TEST:attribute06 (x=3.141592653589793)
    Float: 6.6f * x[pi]: 20.734512 ✔
get: AIDA:SAMPLE:TEST:attribute06 (x=0.4759988937855432)
    Float: 6.6f * x[pi/6.6f]: 3.1415927 ✔
get: AIDA:SAMPLE:TEST:attribute06 (x=2.718281828459045)
    Float: 6.6f * x[e]: 17.940659 ✔
get: AIDA:SAMPLE:TEST:attribute06 (x=0.41186088905108487)
    Float: 6.6f * x[e/6.6f]: 2.7182817 ✔
get: AIDA:SAMPLE:TEST:attribute06 (x=3.4028235E38)
    Max Float: 6.6f * x[3.4028235E38]: 0.0 ✔
get: AIDA:SAMPLE:TEST:attribute06 (x=1.4E-45)
    Min Float: 6.6f * x[1.4E-45]: 0.0 ✔

████ Test 7: Get Double
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute07 ()
    Double: 7.7 ✔
get: AIDA:SAMPLE:TEST:attribute07 (x=7.7)
    Double: 7.7 * x[7.7]: 59.290000000000006 ✔
get: AIDA:SAMPLE:TEST:attribute07 (x=-7.7)
    Double: 7.7 * x[-7.7]: -59.290000000000006 ✔
get: AIDA:SAMPLE:TEST:attribute07 (x=3.141592653589793)
    Double: 7.7 * x[pi]: 24.19026343264141 ✔
get: AIDA:SAMPLE:TEST:attribute07 (x=0.40799904592075237)
    Double: 7.7 * x[pi/7.7]: 3.141592653589793 ✔
get: AIDA:SAMPLE:TEST:attribute07 (x=2.718281828459045)
    Double: 7.7 * x[e]: 20.930770079134646 ✔
get: AIDA:SAMPLE:TEST:attribute07 (x=0.35302361408559024)
    Double: 7.7 * x[e/7.7]: 2.718281828459045 ✔
get: AIDA:SAMPLE:TEST:attribute07 (x=1.7976931348623157E308)
    Max Double: 7.7 * x[1.7976931348623157E308]: Infinity ✔
get: AIDA:SAMPLE:TEST:attribute07 (x=4.9E-324)
    Min Double: 7.7 * x[4.9E-324]: 4.0E-323 ✔

████ Test 8: Get String
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute08 ()
    String: eight ✔
get: AIDA:SAMPLE:TEST:attribute08 (x=Hello World)
    Concatenate String: "eight: " + x["Hello World"]: eight: Hello World ✔

████ Test 9: Get Boolean Array
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute11 ()
    Boolean Array:
        true ✔
get: AIDA:SAMPLE:TEST:attribute11 (x=[TRUE, FALSE, T, F, Y, N, YES, NO])
    Boolean Array: x[["TRUE", "FALSE", "T", "F", "Y", "N", "YES", "NO"]]:
        true ✔
        false ✔
        true ✔
        false ✔
        true ✔
        false ✔
        true ✔
        false ✔
get: AIDA:SAMPLE:TEST:attribute11 (x=[1, 0])
    Boolean Array: x[[1, 0]]:
        true ✔
        false ✔
get: AIDA:SAMPLE:TEST:attribute11 (x=[true, false])
    Boolean Array: x[[Boolean.TRUE, Boolean.FALSE]]:
        true ✔
        false ✔
get: AIDA:SAMPLE:TEST:attribute11 (x=[1, 0])
    As Json: x[[1, 0]]:
        true ✔
        false ✔

████ Test 10: Get Byte Array
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute12 ()
    Byte Array:
        12 ✔
get: AIDA:SAMPLE:TEST:attribute12 (x=[4, 8, 72, 65])
    Byte Array: 12 | x[[0x4, 0x8, 0x48, 65]]:
        12 ✔
        12 ✔
        76 ✔
        77 ✔
get: AIDA:SAMPLE:TEST:attribute12 (x=[64, 65, 72, 73], TYPE=BYTE_ARRAY) => CHAR_ARRAY
    Char Array: 12 | x[[0x40, 0x41, 0x48, 0x49]]:
        'L' ✔
        'M' ✔
        'L' ✔
        'M' ✔
get: AIDA:SAMPLE:TEST:attribute12 (x=[4, 8, 72, 65])
    As Json: 12 | x[[0x4, 0x8, 0x48, 65]]:
        12 ✔
        12 ✔
        76 ✔
        77 ✔

████ Test 11: Get Short Array
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute13 ()
    Short Array:
        13 ✔
get: AIDA:SAMPLE:TEST:attribute13 (x=[2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37])
    Short Array: 13 + x[[2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37]]:
        15 ✔
        16 ✔
        18 ✔
        20 ✔
        24 ✔
        26 ✔
        30 ✔
        32 ✔
        36 ✔
        42 ✔
        44 ✔
        50 ✔
get: AIDA:SAMPLE:TEST:attribute13 (x=[32693, 32707, 32713, 32717, 32719, 32749])
    Short Array: 13 + x[[32693, 32707, 32713, 32717, 32719, 32749]]:
        32706 ✔
        32720 ✔
        32726 ✔
        32730 ✔
        32732 ✔
        32762 ✔
get: AIDA:SAMPLE:TEST:attribute13 (x=[32693, 32707, 32713, 32717, 32719, 32749])
    As Json: 13 + x[[32693, 32707, 32713, 32717, 32719, 32749]]:
        32706 ✔
        32720 ✔
        32726 ✔
        32730 ✔
        32732 ✔
        32762 ✔

████ Test 12: Get Integer Array
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute14 ()
    Integer Array:
        14 ✔
get: AIDA:SAMPLE:TEST:attribute14 (x=[10000019, 10000079, 10000103, 10000121, 10000139, 10000141])
    Integer Array: 14 + x[[10000019, 10000079,10000103,10000121,10000139,10000141]]:
        10000033 ✔
        10000093 ✔
        10000117 ✔
        10000135 ✔
        10000153 ✔
        10000155 ✔
get: AIDA:SAMPLE:TEST:attribute14 (x=[10000019, 10000079,10000103,10000121,10000139,10000141])
    As Json: 14 + x[[10000019, 10000079,10000103,10000121,10000139,10000141]]:
        10000033 ✔
        10000093 ✔
        10000117 ✔
        10000135 ✔
        10000153 ✔
        10000155 ✔

████ Test 13: Get Long Array
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute15 ()
    Long Array:
        15 ✔
get: AIDA:SAMPLE:TEST:attribute15 (x=[1000000007, 1000000009, 1000000021, 1000000033, 1000000087, 1000000093])
    Long Array: 15 + x[[1000000007L, 1000000009L, 1000000021L, 1000000033L, 1000000087L, 1000000093L]]:
        1000000022 ✔
        1000000024 ✔
        1000000036 ✔
        1000000048 ✔
        1000000102 ✔
        1000000108 ✔
get: AIDA:SAMPLE:TEST:attribute15 (x=[1000000007, 1000000009, 1000000021, 1000000033, 1000000087, 1000000093])
    As Json: 15 + x[[1000000007, 1000000009, 1000000021, 1000000033, 1000000087, 1000000093]]:
        1000000022 ✔
        1000000024 ✔
        1000000036 ✔
        1000000048 ✔
        1000000102 ✔
        1000000108 ✔

████ Test 14: Get Float Array
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute16 ()
    Float Array:
        16.6 ✔
get: AIDA:SAMPLE:TEST:attribute16 (x=[0.18925257, 0.16375192, 1.0E-10, 1.0E10, 1.234567E-23, 1.234567E23, 1.2345679E23])
    Formats: Float Array: 16.6 * x[[PI/16.6, E/16.6, 1e-10f, 1e+10f, 1.234567e-23f, 1.234567e+23f, 1.23456789e+23f]]:
        3.1415927 ✔
        2.7182817 ✔
        1.66E-9 ✔
        1.66000001E11 ✔
        2.0493812E-22 ✔
        2.0493812E24 ✔
        2.0493827E24 ✔
get: AIDA:SAMPLE:TEST:attribute16 (x=[1.2345678, 1.2345679, -1.2345678, -1.2345679, 1.0000001, 1.0000002, -1.0000001, -1.0000002])
    Precision: Float Array: 16.6 * x[[1.2345678f, 1.2345679f, -1.2345678f, -1.2345679f, 1.0000001f, 1.0000002f, -1.0000001f, -1.0000002f]]:
        20.493826 ✔
        20.493828 ✔
        -20.493826 ✔
        -20.493828 ✔
        16.600002 ✔
        16.600004 ✔
        -16.600002 ✔
        -16.600004 ✔
get: AIDA:SAMPLE:TEST:attribute16 (x=[1.2345679, 1.2345679, -1.2345679, -1.2345679, 1.0000001, 1.0000001, -1.0000001, -1.0000001])
    Significance Limit: Float Array: 16.6 * x[[1.23456789f, 1.234567896f, -1.23456789f, -1.234567896f, 1.0000001f, 1.00000016f, -1.0000001f, -1.00000016f]]:
        20.493828 ✔
        20.493828 ✔
        -20.493828 ✔
        -20.493828 ✔
        16.600002 ✔
        16.600002 ✔
        -16.600002 ✔
        -16.600002 ✔
get: AIDA:SAMPLE:TEST:attribute16 (x=[1.2345679E-23, 1.2345679E-23, 1.2345679E23, 1.2345679E23])
    Significance Limit e-notation: Float Array: 16.6 * x[[1.23456789e-23f, 1.234567896e-23f, 1.23456789e+23f, 1.234567896e+23f]]:
        2.0493828E-22 ✔
        2.0493828E-22 ✔
        2.0493827E24 ✔
        2.0493827E24 ✔
get: AIDA:SAMPLE:TEST:attribute16 (x=[1.4E-45, 3.4028235E38, 3.141592653589793, 2.718281828459045])
    Max/Min/pi/e: Float Array: 16.6 * x[[Float.MIN_VALUE, Float.MAX_VALUE, PI, E]]:
        2.4E-44 ✔
        Infinity ✔
        52.15044 ✔
        45.123478 ✔
get: AIDA:SAMPLE:TEST:attribute16 (x=[1.4E-45, 3.4028235E38, 3.141592653589793, 2.718281828459045])
    Max/Min/pi/e: As Json Array: 16.6 * x[[1.4E-45, 3.4028235E38, 3.141592653589793, 2.718281828459045]]]:
        2.4E-44 ✔
        Infinity ✔
        52.15044 ✔
        45.123478 ✔

████ Test 15: Get Double Array
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute17 ()
    Double Array:
        17.7 ✔
get: AIDA:SAMPLE:TEST:attribute17 (x=[0.1774911103723047, 0.15357524454570876, 1.0E-100, 1.0E100, 1.0000000234567E-230, 1.0000000234567E230, 1.000000023456789E230])
    Formats: Double Array: 17.7 * x[[PI/17.7, E/17.7, 1e-100, 1e+100, 1.0000000234567e-230, 1.0000000234567e+230, 1.000000023456789e+230]]:
        3.141592653589793 ✔
        2.7182818284590446 ✔
        1.7699999999999999E-99 ✔
        1.77E101 ✔
        1.770000041518359E-229 ✔
        1.7700000415183587E231 ✔
        1.7700000415185162E231 ✔
get: AIDA:SAMPLE:TEST:attribute17 (x=[1.000000002345678, 1.000000002345679, -1.000000002345678, -1.000000002345679, 1.000000000000001, 1.000000000000002, -1.000000000000001, -1.000000000000002])
    Precision: Double Array: 17.7 * x[[1.000000002345678, 1.000000002345679, -1.000000002345678, -1.000000002345679, 1.000000000000001, 1.000000000000002, -1.000000000000001, -1.000000000000002]]:
        17.7000000415185 ✔
        17.700000041518518 ✔
        -17.7000000415185 ✔
        -17.700000041518518 ✔
        17.70000000000002 ✔
        17.700000000000035 ✔
        -17.70000000000002 ✔
        -17.700000000000035 ✔
get: AIDA:SAMPLE:TEST:attribute17 (x=[1.0000000023456788, 1.0000000023456788, -1.0000000023456788, -1.0000000023456788, 1.0000000000000018, 1.0000000000000018, -1.0000000000000018, -1.0000000000000018])
    Significance Limit: Double Array: 17.7 * x[[1.0000000023456787, 1.0000000023456788, -1.0000000023456787, -1.0000000023456788, 1.0000000000000017, 1.0000000000000018, -1.0000000000000017, -1.0000000000000018]]:
        17.700000041518514 ✔
        17.700000041518514 ✔
        -17.700000041518514 ✔
        -17.700000041518514 ✔
        17.70000000000003 ✔
        17.70000000000003 ✔
        -17.70000000000003 ✔
        -17.70000000000003 ✔
get: AIDA:SAMPLE:TEST:attribute17 (x=[1.0000000023456787E-23, 1.0000000023456787E-23, 1.0000000023456787E23, 1.0000000023456789E23])
    Significance Limit e-notation: Double Array: 17.7 * x[[1.0000000023456787e-23, 1.0000000023456786e-23, 1.0000000023456787e+23, 1.0000000023456788e+23]]:
        1.770000004151851E-22 ✔
        1.770000004151851E-22 ✔
        1.7700000041518512E24 ✔
        1.7700000041518515E24 ✔
get: AIDA:SAMPLE:TEST:attribute17 (x=[4.9E-324, 1.7976931348623157E308, 3.141592653589793, 2.718281828459045])
    Max/Min/pi/e: Double Array: 17.7 * x[[Double.MIN_VALUE, Double.MAX_VALUE, PI, E]]:
        8.9E-323 ✔
        Infinity ✔
        55.60618996853933 ✔
        48.11358836372509 ✔
get: AIDA:SAMPLE:TEST:attribute17 (x=[4.9E-324, 1.7976931348623157E308, 3.141592653589793, 2.718281828459045])
    Max/Min/pi/e: As Json Array: 17.7 * x[[4.9E-324, 1.7976931348623157E308, 3.141592653589793, 2.718281828459045]]:
        0.0 ✔
        Infinity ✔
        55.60618996853933 ✔
        48.11358836372509 ✔

████ Test 16: Get String Array
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute18 ()
    String Array:
        eighteen ✔
get: AIDA:SAMPLE:TEST:attribute18 (x=[Hello, AIDA-PVA, World, Have, a, nice, day])
    Multiple Strings:
        Hello ✔
        AIDA-PVA ✔
        World ✔
        Have ✔
        a ✔
        nice ✔
        day ✔
get: AIDA:SAMPLE:TEST:attribute18 (x=["Hello", "AIDA-PVA", "World", "Have", "a", "json", "day"])
    As json:
        Hello ✔
        AIDA-PVA ✔
        World ✔
        Have ✔
        a ✔
        json ✔
        day ✔

████ Test 17: Get Table
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute20 ()
    Table:  1 rows retrieved: ✔
 Is active? Mode Code Status Code Buffers allocated Memory used (MB) Sensor Reading (KHz) Precision Sensor (Mpa) Message
   isActive      mode      status             nbufs           memory               sensor               hisensor message
       true         2           3                 4                5                  6.6                    7.7   eight
get: AIDA:SAMPLE:TEST:attribute20 (x={"boolean": true,"byte": 102,"short": 103,"integer": 104,"long": 105,"float": 106.5,"double": 107.7,"string": "one hundred and eight"})
    Table x[same operations as scalar]:  1 rows retrieved: ✔
 Is active? Mode Code Status Code Buffers allocated Memory used (MB) Sensor Reading (KHz) Precision Sensor (Mpa)               Message
   isActive      mode      status             nbufs           memory               sensor               hisensor               message
       true       102         106               108              110            702.89996      829.2900000000001 one hundred and eight

████ Test 18: Void Setter
_________________________________________________

set: AIDA:SAMPLE:TEST:attribute30 (VALUE=true) ✔
set: AIDA:SAMPLE:TEST:attribute30 (x=false, VALUE=true)
    Set Value: TRUE, x=FALSE:  ✔

████ Test 19: Setter returning Table
_________________________________________________

set: AIDA:SAMPLE:TEST:attribute31 (VALUE=true)
    Set Value: True, Return Table:  1 rows retrieved: ✔
 Result of setting value
                  status
                    true
set: AIDA:SAMPLE:TEST:attribute31 (VALUE=false)
    Set Value: False, Return Table:  1 rows retrieved: ✔
 Result of setting value
                  status
                   false
set: AIDA:SAMPLE:TEST:attribute31 (VALUE=1)
    Set Value: 1, Return Table:  1 rows retrieved: ✔
 Result of setting value
                  status
                    true
set: AIDA:SAMPLE:TEST:attribute31 (VALUE=0)
    Set Value: 0, Return Table:  1 rows retrieved: ✔
 Result of setting value
                  status
                   false
set: AIDA:SAMPLE:TEST:attribute31 (x=false, VALUE=true)
    Set Value: True, x=FALSE, Return Table:  1 rows retrieved: ✔
 Result of setting value
                  status
                    true

████ Test 20: Arbitrary Getters on the same channel
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute32 => BOOLEAN
    Boolean: true ✔
get: AIDA:SAMPLE:TEST:attribute32 => BYTE
    Byte: 2 ✔
get: AIDA:SAMPLE:TEST:attribute32 => CHAR
    Char: '' ✔
get: AIDA:SAMPLE:TEST:attribute32 => SHORT
    Short: 3 ✔
get: AIDA:SAMPLE:TEST:attribute32 => INTEGER
    Integer: 4 ✔
get: AIDA:SAMPLE:TEST:attribute32 => LONG
    Long: 5 ✔
get: AIDA:SAMPLE:TEST:attribute32 => FLOAT
    Float: 6.6 ✔
get: AIDA:SAMPLE:TEST:attribute32 => DOUBLE
    Double: 7.7 ✔
get: AIDA:SAMPLE:TEST:attribute32 => STRING
    String: eight ✔
get: AIDA:SAMPLE:TEST:attribute32 => BOOLEAN_ARRAY
    Boolean Array:
        true ✔
get: AIDA:SAMPLE:TEST:attribute32 => BYTE_ARRAY
    Byte Array:
        12 ✔
get: AIDA:SAMPLE:TEST:attribute32 => CHAR_ARRAY
    Char Array:
        '
         ' ✔
get: AIDA:SAMPLE:TEST:attribute32 => SHORT_ARRAY
    Short Array:
        13 ✔
get: AIDA:SAMPLE:TEST:attribute32 => INTEGER_ARRAY
    Integer Array:
        14 ✔
get: AIDA:SAMPLE:TEST:attribute32 => LONG_ARRAY
    Long Array:
        15 ✔
get: AIDA:SAMPLE:TEST:attribute32 => FLOAT_ARRAY
    Float Array:
        16.6 ✔
get: AIDA:SAMPLE:TEST:attribute32 => DOUBLE_ARRAY
    Double Array:
        17.7 ✔
get: AIDA:SAMPLE:TEST:attribute32 => STRING_ARRAY
    String Array:
        eighteen ✔
get: AIDA:SAMPLE:TEST:attribute32 => TABLE
    Table:  1 rows retrieved: ✔
 Is active? Mode Code Status Code Buffers allocated Memory used (MB) Sensor Reading (KHz) Precision Sensor (Mpa) Message
   isActive      mode      status             nbufs           memory               sensor               hisensor message
       true         2           3                 4                5                  6.6                    7.7   eight

████ Test 21: Channel Not Supported
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute00 ()
 UnsupportedChannelException; AIDA:SAMPLE:TEST:attribute00 ✔

████ Test 22: Invalid Parameters
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute01 (y=1)
 AIDA:SAMPLE:TEST:attribute01:  Y is not a valid argument for get requests to this channel. Valid arguments are: [X] ✔
get: AIDA:SAMPLE:TEST:attribute02 (y=2)
 AIDA:SAMPLE:TEST:attribute02:  Y is not a valid argument for get requests to this channel. Valid arguments are: [X] ✔
get: AIDA:SAMPLE:TEST:attribute03 (y=3)
 AIDA:SAMPLE:TEST:attribute03:  Y is not a valid argument for get requests to this channel. Valid arguments are: [X] ✔
get: AIDA:SAMPLE:TEST:attribute04 (y=4)
 AIDA:SAMPLE:TEST:attribute04:  Y is not a valid argument for get requests to this channel. Valid arguments are: [X] ✔
get: AIDA:SAMPLE:TEST:attribute05 (y=5)
 AIDA:SAMPLE:TEST:attribute05:  Y is not a valid argument for get requests to this channel. Valid arguments are: [X] ✔
get: AIDA:SAMPLE:TEST:attribute06 (y=6.6)
 AIDA:SAMPLE:TEST:attribute06:  Y is not a valid argument for get requests to this channel. Valid arguments are: [X] ✔
get: AIDA:SAMPLE:TEST:attribute07 (y=7.7)
 AIDA:SAMPLE:TEST:attribute07:  Y is not a valid argument for get requests to this channel. Valid arguments are: [X] ✔
get: AIDA:SAMPLE:TEST:attribute08 (y=Hello World)
 AIDA:SAMPLE:TEST:attribute08:  Y is not a valid argument for get requests to this channel. Valid arguments are: [X] ✔
get: AIDA:SAMPLE:TEST:attribute11 (y=[TRUE, FALSE, T, F, Y, N, YES, NO])
 AIDA:SAMPLE:TEST:attribute11:  Y is not a valid argument for get requests to this channel. Valid arguments are: [X] ✔
get: AIDA:SAMPLE:TEST:attribute12 (y=[4, 8, 72, 65])
 AIDA:SAMPLE:TEST:attribute12:  Y is not a valid argument for get requests to this channel. Valid arguments are: [X] ✔
get: AIDA:SAMPLE:TEST:attribute13 (y=[2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37])
 AIDA:SAMPLE:TEST:attribute13:  Y is not a valid argument for get requests to this channel. Valid arguments are: [X] ✔
get: AIDA:SAMPLE:TEST:attribute14 (y=[10000019, 10000079, 10000103, 10000121, 10000139, 10000141])
 AIDA:SAMPLE:TEST:attribute14:  Y is not a valid argument for get requests to this channel. Valid arguments are: [X] ✔
get: AIDA:SAMPLE:TEST:attribute15 (y=[1000000007, 1000000009, 1000000021, 1000000033, 1000000087, 1000000093])
 AIDA:SAMPLE:TEST:attribute15:  Y is not a valid argument for get requests to this channel. Valid arguments are: [X] ✔
get: AIDA:SAMPLE:TEST:attribute16 (y=[0.18925257, 0.16375192, 1.0E-10, 1.0E10, 1.234567E-23, 1.234567E23, 1.2345679E23])
 AIDA:SAMPLE:TEST:attribute16:  Y is not a valid argument for get requests to this channel. Valid arguments are: [X] ✔
get: AIDA:SAMPLE:TEST:attribute17 (y=[0.1774911103723047, 0.15357524454570876, 1.0E-100, 1.0E100, 1.0000000234567E-230, 1.0000000234567E230, 1.000000023456789E230])
 AIDA:SAMPLE:TEST:attribute17:  Y is not a valid argument for get requests to this channel. Valid arguments are: [X] ✔
get: AIDA:SAMPLE:TEST:attribute18 (y=[Hello, AIDA-PVA, World, Have, a, nice, day])
 AIDA:SAMPLE:TEST:attribute18:  Y is not a valid argument for get requests to this channel. Valid arguments are: [X] ✔
get: AIDA:SAMPLE:TEST:attribute20 (y={"boolean": true,"byte": 102,"short": 103,"integer": 104,"long": 105,"float": 106.5,"double": 107.7,"string": "one hundred and eight"})
 AIDA:SAMPLE:TEST:attribute20:  Y is not a valid argument for get requests to this channel. Valid arguments are: [X] ✔
set: AIDA:SAMPLE:TEST:attribute30 (y=false, VALUE=true)
 AIDA:SAMPLE:TEST:attribute30:  Y is not a valid argument for set requests to this channel. Valid arguments are: [X] ✔
set: AIDA:SAMPLE:TEST:attribute31 (y=false, VALUE=true)
 AIDA:SAMPLE:TEST:attribute31:  Y is not a valid argument for set requests to this channel. Valid arguments are: [X] ✔

████ Test 23: Invalid Booleans
_________________________________________________

get: AIDA:SAMPLE:TEST:attribute01 (x=1100)
    Valid Boolean: x[1100] : true ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=0.0)
    Valid Boolean: x[0.0] : false ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=0.1)
    Valid Boolean: x[0.1] : true ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=truly)
 AidaInternalException; can't convert argument to boolean: truly ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=UNTRUE)
 AidaInternalException; can't convert argument to boolean: UNTRUE ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=O)
 AidaInternalException; can't convert argument to boolean: O ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=yeah)
 AidaInternalException; can't convert argument to boolean: yeah ✔
get: AIDA:SAMPLE:TEST:attribute01 (x=naw)
 AidaInternalException; can't convert argument to boolean: naw ✔
```
