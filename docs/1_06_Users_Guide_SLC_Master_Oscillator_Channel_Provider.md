# 1.6 - SLC Master Oscillator Data Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Magnet Data through AIDA-PVA. This data
provider allows for the retrieval of the Master Oscillator setting in units of absolute frequency. It also allows the
setting of the Master Oscillator by specifying the relative value in units of frequency (the default) or energy. This
set operation returns the new measurement value of the Master Oscillator in units of absolute frequency. For general
information on using AIDA-PVA see [Basic Users Guide to Aida](1_00_User_Guide.md), and the EPICS javadoc.

## Summary

Supports **get** and **set** operations.

- The **get** operation obtains the Master Oscillator setting in units of absolute frequency.
- The **set** operation sets the Master Oscillator value to a specified setting in units of relative frequency or
  energy. If the specified units is relative energy, the ring (`HER` or `LER`) must also be specified. The set operation
  returns the new measured Master Oscillator setting value in units of absolute frequency

## Instances and Attributes

| | | |
| ----------- | ----------- | ----------- |
| **get** | Syntax    | `MASTEROSC:VAL` |
| | Examples | `MASTEROSC:VAL` |
| **set** | Syntax    | `MASTEROSC:VAL` |
| | Examples | `MASTEROSC:VAL` |

## Attribute operation summary

| | | |
| ----------- | -----------  | -----------  |
| Attribute | operation |Description |
| `VAL` | **get** |  Gets the Master Oscillator setting in units of absolute frequency (KHz). |
| `VAL` | **set** |  Sets the Master Oscillator value to a specified setting in units of relative frequency or energy |

## Attribute operations

### VAL : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `TYPE`*  |   `TABLE`  | return a table |
| | `DOUBLE`  | returns a double value |

_Return value_

| | | | |
|----------- | ----------- | -----------  |-----------  |
| TYPE  | Return Column | Column Type |Description |
| `TABLE` | `value` | `DOUBLE_ARRAY` | measured setting of the Master Oscillator in KHz |

| | |
|-----------  |-----------  |
| TYPE  |  Description |
| `DOUBLE` |  measured setting of the Master Oscillator in KHz  |

### VAL : set

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `VALUE`*  | Float  | setting in units of relative frequency or energy |
| `UNITS`  |   `FREQUENCY` or `ENERGY`  | Indicates whether the specified relative set value is in units of frequency or energy. The default is frequency |
| `RING`  | `HER` or `LER`  | If the specified relative set value is in units of energy, this parameter is required and specifies the ring (HER or LER).  |

_Return value_
| | | | | |----------- | ----------- | ----------- |----------- | | TYPE | Return Column | Column Type |Description |
| `TABLE` | `value` | `DOUBLE_ARRAY` | measured setting of the Master Oscillator in KHz |

## Examples

| | | |
|  ----------- |----------- |----------- |
| pvcall examples | `pvcall MASTEROSC:VAL` | Get measured Master Oscillator setting value |
|  | `pvcall MASTEROSC:VAL VALUE=0.328f` |  Perform set operation |
| Java Tests | SlcMoscTest.java | |
| Matlab example |  | |

## Test Output

```shell
java -cp aida-pva-tests.jar  "edu.stanford.slac.aida.test.SlcMoscTest" -c
#################################################
AIDA-PVA SLC Master Oscillator TESTS
#################################################

████ Test 1: Test of Master Oscillator get method
_________________________________________________

get: MASTEROSC:VAL => TABLE
    VAL:  1 rows retrieved: ✔
 Oscillator Value (KHz)
                  value
             476000.328

████ Test 2: Test of Master Oscillator get method for double
_________________________________________________

get: MASTEROSC:VAL => DOUBLE
    VAL: 476000.328 ✔

████ Test 3: Test of Master Oscillator get method for table
_________________________________________________

get: MASTEROSC:VAL => TABLE
    VAL:  1 rows retrieved: ✔
 Oscillator Value (KHz)
                  value
             476000.328

████ Test 4: Test of Master Oscillator set
_________________________________________________

set: MASTEROSC:VAL (VALUE=0.328) ✔

████ Test 5: Test of Master Oscillator set method Set value is relative energy.  Ring is HER
_________________________________________________

set: MASTEROSC:VAL (RING=HER, VALUE=1.0, UNITS=ENERGY)
    VAL:  1 rows retrieved: ✔
 Oscillator Value (KHz)
                  value
               476000.0

████ Test 6: Test of Master Oscillator set method Set value is relative energy.  Ring is LER
_________________________________________________

set: MASTEROSC:VAL (RING=LER, VALUE=1.0, UNITS=ENERGY)
    VAL:  1 rows retrieved: ✔
 Oscillator Value (KHz)
                  value
               476000.0

████ Test 7: Test of Master Oscillator set
_________________________________________________

set: MASTEROSC:VAL (VALUE=0.328) ✔
```

