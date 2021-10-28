@tableofcontents
# SLC Klystron Data Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Klystron Data through AIDA-PVA. This data provider allows the retrieval of the status code or a status string for a specified klystron on a beam code. It also allows the deactivation or reactivation of a specified klystron on a beam code. The `PDES` value for a klystron or subbooster may be set and the phase may be optionally trimmed. The `KPHR` value for a klystron or subboster may be set. Finally, a configuration value (`PCON` or `ACON`) of a specified klystron or subbooster may be set.  For general
information on using AIDA-PVA see [Basic Users Guide to Aida](UserGuide.md), and the EPICS javadoc.

# AIDA-PVA Data Provider for SLC Klystron Data

## Summary
Supports **get** and **set** operations. 

The **get** operation obtains the status code or a status string for a specified klystron on a beam code. 

There are four **set** operations that can be performed: 
1. deactivate or reactivate a specified klystron on a beam code, 
2. set the `PDES` value and optionally trim the phase of a specified klystron or subbooster, 
3. set the `KPHR` value of a specified klystron or subbooster, and 
4. set a configuration value (`PCON` or `ACON`) of a specified klystron or subbooster

## Examples
| | | |
|  ----------- |----------- |----------- |
| pvcall examples | `pvcall KLYS:LI31:31:TACT BEAM=8 DGRP=DEV_DGRP TYPE=SHORT`| Get the status code for the klystron on the beam code |
|  | `pvcall KLYS:LI31:31:TACT BEAM=8 DGRP=DEV_DGRP TYPE=STRING` | Get the status string |
|  | `pvcall KLYS:LI31:31:TACT BEAM=8 DGRP=DEV_DGRP VALUE=0` | Deactivate klystron on beam code |
|  | `pvcall KLYS:LI31:31:PDES VALUE=90.0f` | Perform set PDES and trim phase operation |
|  | `pvcall KLYS:LI31:31:KPHR VALUE=60.0f` | Perform set KPHR value operation |
|  | `pvcall KLYS:LI31:31:PCON VALUE=5.0f` | Perform set PCON value operation (no value is returned) |
| Java Tests | SlcKlysTest.java | |
| Matlab example |  | |

## Instances and Attributes

| | | |
| ----------- | ----------- | ----------- |
| **get** | Syntax    | `<prim>:<micr>:<unit>:TACT` |
| | Examples | `KLYS:LI31:31:TACT` |
| **set** | Syntax    | `<prim>:<micr>:<unit>:TACT` |
| | | `<prim>:<micr>:<unit>:PDES` |
| | | `<prim>:<micr>:<unit>:KPHR` |
| | | `<prim>:<micr>:<unit>:PCON` |
| | | `<prim>:<micr>:<unit>:ACON` |
| | Examples | `KLYS:LI31:31:TACT` |
| | | `KLYS:LI31:31:PDES` |
| | | `KLYS:LI31:31:KPHR` |
| | | `KLYS:LI31:31:PCON` |
| | | `KLYS:LI31:31:ACON` |

## Attribute operation summary

| | | |
| ----------- | -----------  | -----------  |
| Attribute | operation |Description |
| `TACT` | **get** |  Gets a status code or a status string for the specified klystron on a beam code |
| `TACT` | **set** |  Deactivates or reactivates a specified klystron on a beam code |
| `PDES` | **set** |  Sets the PDES value and optionally trims the phase for a specified klystron or subbooster |
| `KPHR` | **set** |  Sets the KPHR value of a specified klystron or subbooster |
| `PCON` | **set** |  Sets the PCON value of a specified klystron or subbooster |
| `ACON` | **set** |  Sets the ACON value of a specified klystron or subbooster |

## Attribute operations
### TACT : get
_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `TYPE`*  |   return type  | Must be one of `SHORT`, `LONG`, or `STRING`  |
| `BEAM`*  |   Integer  | Beam code number |
| `DGRP`| Dgrp name |  A display group associated with the specified klystron. Must be specified if the klystron does not belong to display group `LIN_KLYS` |

_Return value_

@note
Return value for BPM device shown below. `TORO`, `GAPM`, `KLYS` or `SBST` will be different.

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `SHORT` |  A short value containing the status code for the klystron on a beam code.  See [linklysta.txt](http://www-mcc.slac.stanford.edu/REF_/SLCTXT/LINKLYSTA.TXT)   |
| `LONG` |  A long value containing the status code for the klystron on a beam code.  See [linklysta.txt](http://www-mcc.slac.stanford.edu/REF_/SLCTXT/LINKLYSTA.TXT)   |
| `STRING` |  A string value containing a status string having one of two values: "deactivated" or "activated"   |


### TACT : set
_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `VALUE`*  |  Short   | Short flag code indicating the desired operation: `0` => deactivate, `1` => reactivate.  |
| `BEAM`*  |   Integer  | Beam code number |
| `DGRP`| Dgrp name |  A display group associated with the specified klystron. Must be specified if the klystron does not belong to display group `LIN_KLYS` |

_Return value_

| | | | |
|----------- | ----------- | -----------  |-----------  |
| TYPE  | Return Column | Column Type |Description |
| `TABLE` | `status` | `SHORT_ARRAY` | Status code for the specified klystron on a beam code - See [linklysta.txt](http://www-mcc.slac.stanford.edu/REF_/SLCTXT/LINKLYSTA.TXT)  |

### PDES : set
_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `VALUE`*  |  Short   | Short flag code indicating the desired operation: `0` => deactivate, `1` => reactivate.  |
| `TRIM`  |   Boolean  | `YES` or `NO`. If specified and set to `NO`, no trim phase operation will be performed after setting the `PDES` value. If not specified or set to `YES`, the trim phase operation will be performed. |

_Return value_

| | | | |
|----------- | ----------- | -----------  |-----------  |
| TYPE  | Return Column | Column Type |Description |
| `TABLE` | `phas` | `FLOAT_ARRAY` | The PHAS secondary value after the set `PDES` and optional trim phase operation. |

### KPHR : set
_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `VALUE`*  |  Float   | desired `KPHR` value.  |

_Return value_

| | | | |
|----------- | ----------- | -----------  |-----------  |
| TYPE  | Return Column | Column Type |Description |
| `TABLE` | `phas` | `FLOAT_ARRAY` | The PHAS secondary value after the set `KPHR` operation |

### PCON : set, ACON : set 
_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `VALUE`*  |  Float   | desired `PCON` or `ACON` value.  |

_Return value_

None

## Test Output
```shell
java -cp aida-pva-tests.jar  "edu.stanford.slac.aida.test.SlcKlysTest" -c
#################################################
AIDA-PVA SLC Klystron TESTS
#################################################

████ Test 1: Acquire SHORT type
_________________________________________________

get: KLYS:LI31:31:TACT (BEAM=8, TYPE=SHORT, DGRP=DEV_DGRP) => SHORT
    Short: 18 ✔

████ Test 2: Acquire STRING type
_________________________________________________

get: KLYS:LI31:31:TACT (BEAM=8, TYPE=STRING, DGRP=DEV_DGRP) => STRING
    String: activated ✔

████ Test 3: Acquire TABLE type
_________________________________________________

get: KLYS:LI31:31:TACT (BEAM=8, TYPE=TABLE, DGRP=DEV_DGRP) => TABLE
    Table:  1 rows retrieved: ✔
 Accellerate Standby   Bad Sled Tuned Sleded Pampl pphas
       accel standby   bad       sled sleded pampl pphas
       false    true false      false   true false false

████ Test 4: Deactivate the specified klystron
_________________________________________________

set: KLYS:LI31:31:TACT (BEAM=8, VALUE=0, DGRP=DEV_DGRP)
 UnableToSetDataException; Cannot deactivate klystron when not in accelerate state ✘

████ Test 5: Reactivate the specified klystron
_________________________________________________

set: KLYS:LI31:31:TACT (BEAM=8, VALUE=1, DGRP=DEV_DGRP)
  SBST in; UnableToSetDataException; Could not set activation state ✘

████ Test 6: Set a sub-booster or klystron PDES value and trim it
_________________________________________________

set: KLYS:LI31:31:PDES (VALUE=90.0)
  Klystron; UnableToSetDataException; Error setting value ✘

████ Test 7: Set a subbooster or klystron PDES value without the trim operation
_________________________________________________

set: KLYS:LI31:31:PDES (TRIM=NO, VALUE=90.0)
    PDES:  1 rows retrieved: ✔
 PHAS secondary value
                 PHAS
                  0.0

████ Test 8: Set a subbooster or klystron KPHR value
_________________________________________________

set: KLYS:LI31:31:KPHR (VALUE=60.0)
  Klystron; UnableToSetDataException; Error setting value ✘

████ Test 9: Set configuration secondary PCON value
_________________________________________________

set: KLYS:LI31:31:PCON (VALUE=5.0) ✔

████ Test 10: Set configuration secondary ACON value
_________________________________________________

set: KLYS:LI31:31:ACON (VALUE=5.0) ✔

████ Test 11: Get configuration secondary PDES value
_________________________________________________

get: SLC::KLYS:LI31:31:PDES => FLOAT
    PDES value: 0.0 ✔

████ Test 12: Get configuration secondary KPHR value
_________________________________________________

get: SLC::KLYS:LI31:31:KPHR => FLOAT
    KPHR value: 59.724426 ✔

████ Test 13: Error Test: Invalid BEAM
_________________________________________________

set: KLYS:LI31:31:TACT (BEAM=XYZ, VALUE=1, DGRP=DEV_DGRP)
 UnableToGetDataException; failed to get klystron status ✔

████ Test 14: Error Test: Invalid BEAM
_________________________________________________

get: KLYS:LI31:31:TACT (BEAM=1, TYPE=SHORT, DGRP=LIN_KLYS) => SHORT
 UnableToGetDataException; failed to get klystron status ✔
```


