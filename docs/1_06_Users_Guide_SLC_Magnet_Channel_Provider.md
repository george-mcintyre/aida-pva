# 1.6 - SLC Magnet Data Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Magnet Data through AIDA-PVA. This data
provider allows the retrieval of magnet primary/micro/unit names and `BDES`/`VDES` secondary values for a specified
display group, primary, and secondary. It also allows the setting of `BDES`/`VDES` values for specified magnet
primary/micro/unit names and optionally performs either a trim or perturb operation. This set operation returns the
state names and new `BACT`/`VACT` values. Finally, this data provider allows for the setting of `BCON` values for
specified magnet primary/micro/unit names. For general information on using AIDA-PVA
see [Basic Users Guide to Aida](1_00_User_Guide.md), and the EPICS javadoc.

## Summary

Supports **get** and **set** operations.

- The **get** operation obtains primary/micro/unit strings and `BDES`/`VDES` values for a specified display group,
  primary, and secondary name (with optional micro and unit range filters).
- One **set** operation sets `BDES`/`VDES` values for specified primary/micro/unit name strings to specified values and
  optionally performs a specified operation (trim or perturb). This set operation returns the resulting state names
  and `BACT`/`VACT` values.
- The other **set** operation sets `BCON` values for specified primary/micro/unit name strings to specified values

## Instances and Attributes

|         |          |                                 |
|---------|----------|---------------------------------|
| **get** | Syntax   | `<dgrp-mnemonic>:<prim>:<secn>` |
|         | Examples | `DEV_DGRP:XCOR:BDES`            |
| **set** | Syntax   | `MAGNETSET:<secn>`              |
|         | Examples | `MAGNETSET:BDES`                |
|         |          | `MAGNETSET:BCON`                |

## Attribute operation summary

|                 |           |                                                                                                                                                                                          |
|-----------------|-----------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Attribute       | operation | Description                                                                                                                                                                              |
| `<prim>:<secn>` | **get**   | Returns a table containing the name of each  magnet<br /> for specified display group and primary, and secondary<br /> values                                                            |
| `BDES`          | **set**   | Sets specified `BDES` secondary values and returns<br /> a table  containing a string indicating the state<br /> of each specified magnet device, and `BACT` value<br /> for each device |
| `VDES`          | **set**   | Sets specified `VDES` secondary values and returns<br /> a table  containing a string indicating the state<br /> of each specified magnet device, and `VACT` value<br /> for each device |
| `BCON`          | **set**   | Sets specified `BCON` secondary values                                                                                                                                                   |

## Attribute operations

### <prim>:<secn> : get

_Parameters_

|                 |                             |                                                                                             |
|-----------------|-----------------------------|---------------------------------------------------------------------------------------------|
| Parameter Names | Parameter Values            | Description                                                                                 | 
| `MICROS`        | `<start_micro>-<end_micro>` | This specifies a range of micro names of <br />interest within the specified display group  |
| `UNITS`         | `<start_unit>-<end_unit>`   | This specified a range of unit numbers of <br />interest within the specified display group |

_Return value_

|         |               |                |                                                                                                                            |
|---------|---------------|----------------|----------------------------------------------------------------------------------------------------------------------------|
| TYPE    | Return Column | Column Type    | Description                                                                                                                |
| `TABLE` | `name`        | `STRING_ARRAY` | name of each magnet for specified display group <br />and primary (optionally filtered by specified micro and unit ranges) |
|         | `value`       | `FLOAT_ARRAY`  | secondary values (BDES or VDES)                                                                                            |

### BDES : set, VDES : set

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `VALUE`*  | structure: {names[], values[]}  | an array of magnet names and an array of corresponding<br /> set values  |
| `MAGFUNC`*  |   `TRIM`, `PTRB`, or `NOFUNC`  | Specifies whether a trim or perturb operation will<br /> be performed. If `NOFUNC`, neither a trim nor a perturb<br /> operation will be performed  |
| `LIMITCHECK`  |  `ALL` or `SOME`  | Used to determine behavior when the set value  for<br /> one or more devices is outside of its low/high  limits.<br /> If this parameter is set to `ALL`, the  entire request<br /> will fail resulting in an  exception being thrown<br /> and no `BDES`/`VDES` values  being set for any of<br /> the request devices.  If this parameter is set to<br /> `SOME`, the set  value action will succeed for those<br /> set values  that are within limits and will not be<br /> performed  for those set values outside their limits<br />  (the state string return values for these devices<br />  will be set to the string "Outside Limits").  The<br /> default setting of this parameter is `ALL`.  |

_Return value_

| | | | |
|----------- | ----------- | -----------  |-----------  |
| TYPE  | Return Column | Column Type |Description |
| `TABLE` | `state` | `STRING_ARRAY` | string indicating the state of each specified magnet<br /> device.  Set to blanks (checking that the first character<br /> is blank  is all that is necessary) if the state is<br /> good. Set to the string "Outside Limits" if a set<br /> value is outside of its  limits and the optional parameter<br /> LIMITCHECK is set to SOME |
|  |  `value`| `FLOAT_ARRAY`|  `BACT`/`VACT` value for each device |

### BCON : set

_Parameters_

|                 |                                |                                                                   |
|-----------------|--------------------------------|-------------------------------------------------------------------|
| Parameter Names | Parameter Values               | Description                                                       | 
| `VALUE`*        | structure: {names[], values[]} | an array of magnet names and an array of corresponding set values |

_Return value_

None

## Examples

|                 |                                                                                                |                                                                                                                          |
|-----------------|------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------|
| pvcall examples | `pvcall DEV_DGRP:XCOR:BDES MICROS="LI31-LI31" UNITS="1-100"`                                   | Get primary/micro/unit strings<br /> and BDES/VDES settings                                                              |
|                 | ```pvcall MAGNETSET:BDES VALUE='{"names": ["XCOR:LI31:41"], "values": [4.0f]}' MAGFUNC=TRIM``` | Perform set operation for setting<br /> specified values and optionally<br /> performing a `TRIM`/`PTRB` operation<br /> |
|                 | ```pvcall MAGNETSET:BCON VALUE='{"names": ["XCOR:LI31:41"], "values": [5.0f]}'```              | Perform set operation for setting<br /> specified `BCON` values (no value<br /> is returned)                             |
| Java Tests      | SlcMagnetTest.java                                                                             |                                                                                                                          |
| Matlab example  |                                                                                                |                                                                                                                          |

## Test Output

```shell
java -cp aida-pva-tests.jar  "edu.stanford.slac.aida.test.SlcMagnetTest" -c
#################################################
AIDA-PVA SLC Magnet TESTS
#################################################

████ Test 1: Test of magnet get without parameters
_________________________________________________

get: DEV_DGRP:XCOR:BDES ()
    BDES:  4 rows retrieved: ✔
 name of magnet secondary values
           name        secondary
   XCOR:LI31:41              5.0
  XCOR:LI31:201              0.0
  XCOR:LI31:301              0.0
  XCOR:LI31:401             0.03

████ Test 2: Test of magnet "get" with parameters
_________________________________________________

get: DEV_DGRP:XCOR:BDES (MICROS=LI31-LI31, UNITS=1-100)
    BDES:  1 rows retrieved: ✔
 name of magnet secondary values
           name        secondary
   XCOR:LI31:41              5.0

████ Test 3: Test of magnet get without parameters
_________________________________________________

get: DEV_DGRP:XCOR:BDES ()
    BDES:  4 rows retrieved: ✔
 name of magnet secondary values
           name        secondary
   XCOR:LI31:41              5.0
  XCOR:LI31:201              0.0
  XCOR:LI31:301              0.0
  XCOR:LI31:401             0.03

████ Test 4: Test of magnet set
_________________________________________________

set: MAGNETSET:BDES (MAGFUNC=TRIM, VALUE={names=[XCOR:LI31:41], values=[4.0]})
  Magnet; UnableToSetDataException; while setting magnet values ✘

████ Test 5: Test of magnet set using JSON
_________________________________________________

set: MAGNETSET:BDES (MAGFUNC=TRIM, VALUE={"names": ["XCOR:LI31:41"],"values": [4.0]})
  Magnet; UnableToSetDataException; while setting magnet values ✘

████ Test 6: Test of magnet set (duplicate)
_________________________________________________

set: MAGNETSET:BDES (MAGFUNC=TRIM, VALUE={names=[XCOR:LI31:41], values=[4.0]})
  Magnet; UnableToSetDataException; while setting magnet values ✘

████ Test 7: Error handling invalid name: magnet set
_________________________________________________

set: MAGNETSET:BDES (MAGFUNC=TRIM, VALUE={names=[XCOZ:LI31:41], values=[4.0]})
 UnableToSetDataException; Some of the names were not valid: XCOZ ✔

████ Test 8: Error handling values out of limits: magnet set
_________________________________________________

set: MAGNETSET:BDES (MAGFUNC=TRIM, VALUE={names=[XCOR:LI31:41], values=[10.0]})
 UnableToSetDataException; Some or all of the set values are outside of the limits ✔

████ Test 9: Test of magnet set without trim or perturb
_________________________________________________

set: MAGNETSET:BDES (MAGFUNC=NOFUNC, VALUE={names=[XCOR:LI31:41], values=[5.0]})
    Magnet Set:  1 rows retrieved: ✔
    status bact/vact
    status bact_vact
 OUT-RANGE 0.2965088

████ Test 10: Test of magnet set, setting the configuration secondary BCON
_________________________________________________

set: MAGNETSET:BCON (VALUE={names=[XCOR:LI31:41], values=[5.0]})
    Magnet Set:  ✔
```

