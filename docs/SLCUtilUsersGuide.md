@tableofcontents
# SLC Utilities Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Utilities through AIDA-PVA. This data provider:
- allows the retrieval of the status code or a status string for a specified trigger device on a beam code. It also allows the deactivation or reactivation of a specified trigger device on a beam code.
- allows the setting of devices referenced in a specified multiknob file by knob rotation using a specified relative delta value. Only a relative (not absolute) multiknob file may be specified,
- allows for the setting of a BGRP variable value

see [Basic Users Guide to Aida](UserGuide.md), and the EPICS javadoc.

# AIDA-PVA Data Provider for SLC Utilities

## Summary

Supports **get** and **set** operations. 

- The **get** operation obtains the status code or a status string for a specified trigger device on a beam code. 
- A **set** operation deactivates or reactivates a specified trigger device on a beam code
- A  **set** set the devices referenced in a specified multiknob file by knob rotation using a specified delta value. 
- A **set** operation set the value of a specified variable for a specified BGRP. 

## Examples

| | | |
|  ----------- |----------- |----------- |
| pvcall examples | `pvcall TRIG:LI31:109:TACT BEAM=1 TYPE=SHORT` | Get the status code for the trigger device on the beam code |
|  | `pvcall TRIG:LI31:109:TACT BEAM=1 TYPE=STRING` |  Get the status string |
|  | `pvcall TRIG:LI31:109:TACT BEAM=1 VALUE=1.0f` |  Perform specified operation to trigger device on beam code |
|  | `pvcall MKB//VAL MKB="mkb:li02b_xb.mkb" VALUE=0` |  Perform set operation for multiknob file knob rotation |
|  | `pvcall BGRP//VAL BGRP="LCLS" VARNAME=T_CAV VALUE=Y` |  Perform BGRP variable set value operation |
| Java Tests | SlcUtilTest.java | |
| Matlab example |  | |

## Instances and Attributes

| | | |
| ----------- | ----------- | ----------- |
| **get** | Syntax    | `<prim>:<micr>:<unit>:TACT` |
| | Examples | `TRIG:LI31:109:TACT` |
| **set** | Syntax    | `<prim>:<micr>:<unit>:TACT` |
| | Examples | `TRIG:LI31:109:TACT` |
| **set** | Syntax    | `MKB:VAL ` |
| | Examples | `MKB:VAL` |
| **set** | Syntax    | `BGRP:VAL ` |
| | Examples | `BGRP:VAL` |

## Attribute operation summary

| | | |
| ----------- | -----------  | -----------  |
| Attribute | operation |Description |
| `TACT` | **get** |  Gets a status code or a status string for the specified trigger device on a beam code |
| `TACT` | **set** |  Deactivates or reactivates a specified trigger device on a beam code |
| `VAL` | **set** |  Sets devices referenced in a specified multiknob file, which is a required parameter |
| `VAL` | **set** |  Sets a BGRP variable to a new value |

## Attribute operations

### TACT : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `TYPE`*  |   `SHORT`  | A short value containing the status code for the trigger device on a beam code: `0` => deactivated, `1` => activated |
| | `LONG`  | A long value containing the status code for the trigger device on a beam code: `0` => deactivated, `1` => activated |
| | `STRING`  | string value containing a status string having one of two values: "deactivated" or "activated" |
| `BEAM`*  |   Integer  | Beam code number |

_Return value_

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `SHORT` |  A short value containing the status code for the trigger device on a beam code: `0` => deactivated, `1` => activated  |
| `LONG` |  A long value containing the status code for the trigger device on a beam code: `0` => deactivated, `1` => activated   |
| `STRING` | A string value containing a status string having one of two values: "deactivated" or "activated"  |


### TACT : set

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `VALUE`*  | Short  | flag code indicating the desired operation: `0` => deactivate, `1` => reactivate. |
| `BEAM`*  |   Integer  | Beam code number |

_Return value_
| | | | |
|----------- | ----------- | -----------  |-----------  |
| TYPE  | Return Column | Column Type |Description |
| `TABLE` | `value` | `SHORT_ARRAY` | status code for the specified trigger device on a beam code: 0 => deactivated, 1 => activated |

### MKB:VAL : set

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `VALUE`*  | Float  | relative delta knob rotation value. |
| `MKB`  |   String  | multiknob filename.  The filename path specification (including the prefix "mkb:", if file is in the standard multiknob file directory) of a multiknob file |

_Return value_
| | | | |
|----------- | ----------- | -----------  |-----------  |
| TYPE  | Return Column | Column Type |Description |
| `TABLE` | `name` | `STRING_ARRAY` | name of each device referenced in the specified multiknob file |
|  | `value` | `DOUBLE_ARRAY` | value of each device after the knob rotation set operation |

### BGRP:VAL : set

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `VALUE`*  | Boolean  | specifying the new variable value "`N`" or "`Y`". |
| `BGRP`*  |   String  | The BGRP name |
| `VARNAME`*  | String  | A BGRP variable name for the specified BGRP..  |

_Return value_

None

## Test Output

```shell
java -cp aida-pva-tests.jar  "edu.stanford.slac.aida.test.SlcUtilTest" -c
#################################################
AIDA-PVA SLC Utility TESTS
#################################################

████ Test 1: set value for MKB.  sleeping for 5 seconds between runs
_________________________________________________

set: MKB:VAL (MKB=mkb:gregstestli31.mkb, VALUE=1.0)
  Bad file name; UnableToSetDataException; unable to set value ✘
set: MKB:VAL (MKB=mkb:gregstestli31.mkb, VALUE=1.0)
  Bad file name; UnableToSetDataException; unable to set value ✘
set: MKB:VAL (MKB=mkb:gregstestli31.mkb, VALUE=1.0)
  Bad file name; UnableToSetDataException; unable to set value ✘
set: MKB:VAL (MKB=mkb:gregstestli31.mkb, VALUE=-1.0)
  Bad file name; UnableToSetDataException; unable to set value ✘
set: MKB:VAL (MKB=mkb:gregstestli31.mkb, VALUE=-1.0)
  Bad file name; UnableToSetDataException; unable to set value ✘
set: MKB:VAL (MKB=mkb:gregstestli31.mkb, VALUE=-1.0)
  Bad file name; UnableToSetDataException; unable to set value ✘

████ Test 2: set value for MKB.  sleeping for 5 seconds between runs
_________________________________________________

set: MKB:VAL (MKB=mkb:gregstestli31.mkb, VALUE=1.0)
  Bad file name; UnableToSetDataException; unable to set value ✘
set: MKB:VAL (MKB=mkb:gregstestli31.mkb, VALUE=1.0)
  Bad file name; UnableToSetDataException; unable to set value ✘
set: MKB:VAL (MKB=mkb:gregstestli31.mkb, VALUE=1.0)
  Bad file name; UnableToSetDataException; unable to set value ✘
set: MKB:VAL (MKB=mkb:gregstestli31.mkb, VALUE=-1.0)
  Bad file name; UnableToSetDataException; unable to set value ✘
set: MKB:VAL (MKB=mkb:gregstestli31.mkb, VALUE=-1.0)
  Bad file name; UnableToSetDataException; unable to set value ✘
set: MKB:VAL (MKB=mkb:gregstestli31.mkb, VALUE=-1.0)
  Bad file name; UnableToSetDataException; unable to set value ✘

████ Test 3: Test of specified absolute multiknob file, which is not permitted.  sleeping for 5 seconds between runs. The requested set operation should fail since the specified multiknob file is absolute, which is not permitted
_________________________________________________

set: MKB:VAL (MKB=mkb:li31test.mkb, VALUE=1.0)
  Bad file name; UnableToSetDataException; unable to set value ✔
set: MKB:VAL (MKB=mkb:li31test.mkb, VALUE=1.0)
  Bad file name; UnableToSetDataException; unable to set value ✔
set: MKB:VAL (MKB=mkb:li31test.mkb, VALUE=1.0)
  Bad file name; UnableToSetDataException; unable to set value ✔
set: MKB:VAL (MKB=mkb:li31test.mkb, VALUE=-1.0)
  Bad file name; UnableToSetDataException; unable to set value ✔
set: MKB:VAL (MKB=mkb:li31test.mkb, VALUE=-1.0)
  Bad file name; UnableToSetDataException; unable to set value ✔
set: MKB:VAL (MKB=mkb:li31test.mkb, VALUE=-1.0)
  Bad file name; UnableToSetDataException; unable to set value ✔

████ Test 4: PROD environment test Extract device string and secondary value vectors.  This test should only be run in the PROD environment
_________________________________________________

Skipped

████ Test 5: Acquire SHORT type
_________________________________________________

get: TRIG:LI31:109:TACT (BEAM=1, TYPE=SHORT) => SHORT
    TACT: 1 ✔

████ Test 6: Acquire STRING type
_________________________________________________

get: TRIG:LI31:109:TACT (BEAM=1, TYPE=STRING) => STRING
    TACT: activated ✔

████ Test 7: Deactivate the specified TRIG device
_________________________________________________

set: TRIG:LI31:109:TACT (BEAM=1, VALUE=0)
    Deactivated:  1 rows retrieved: ✔
 Status Code
      status
           0

████ Test 8: Reactivate the specified TRIG device
_________________________________________________

set: TRIG:LI31:109:TACT (BEAM=1, VALUE=1)
    Reactivated:  1 rows retrieved: ✔
 Status Code
      status
           1

████ Test 9: Set the specified variable of a specified BGRP to the new value "Y"
_________________________________________________

set: BGRP:VAL (VARNAME=T_CAV, VALUE=N, BGRP=LCLS)
    Variable: T_CAV:  ✔

████ Test 10: Set the specified variable of a specified BGRP to the new value "N"
_________________________________________________

set: BGRP:VAL (VARNAME=T_CAV, VALUE=Y, BGRP=LCLS)
    Variable: T_CAV:  ✔
```

