# SLC Magnet Data Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Magnet Data through AIDA-PVA. This data
provider allows the retrieval of magnet primary/micro/unit names and `BDES`/`VDES` secondary values for a specified
display group, primary, and secondary. It also allows the setting of `BDES`/`VDES` values for specified magnet
primary/micro/unit names and optionally performs either a trim or perturb operation. This set operation returns the
state names and new `BACT`/`VACT` values. Finally, this data provider allows for the setting of `BCON` values for
specified magnet primary/micro/unit names. For general information on using AIDA-PVA
see [Basic Users Guide to Aida](UserGuide.md), and the EPICS javadoc.

# AIDA-PVA Data Provider for SLC Magnet Data

## Summary

Supports **get** and **set** operations.

- The **get** operation obtains primary/micro/unit strings and `BDES`/`VDES` values for a specified display group,
  primary, and secondary name (with optional micro and unit range filters).
- One **set** operation sets `BDES`/`VDES` values for specified primary/micro/unit name strings to specified values and
  optionally performs a specified operation (trim or perturb). This set operation returns the resulting state names
  and `BACT`/`VACT` values.
- The other **set** operation sets `BCON` values for specified primary/micro/unit name strings to specified values

## Examples

| | | |
|  ----------- |----------- |----------- |
| pvcall examples | `pvcall DEV_DGRP:XCOR:BDES MICROS="LI31-LI31" UNITS="1-100"` | Get primary/micro/unit strings and BDES/VDES settings |
|  | ```pvcall MAGNETSET:BDES VALUE='{"names": ["XCOR:LI31:41"], "values": [4.0f]}' MAGFUNC=TRIM``` |  Perform set operation for setting specified values and optionally performing a `TRIM`/`PTRB` operation |
|  | ```pvcall MAGNETSET:BCON VALUE='{"names": ["XCOR:LI31:41"], "values": [5.0f]}'``` |  Perform set operation for setting specified `BCON` values (no value is returned) |
| Java Tests | SlcMagnetTest.java | |
| Matlab example |  | |

## Instances and Attributes

| | | |
| ----------- | ----------- | ----------- |
| **get** | Syntax    | `<dgrp-mnemonic>:<prim>:<secn>` |
| | Examples | `DEV_DGRP:XCOR:BDES` |
| **set** | Syntax    | `MAGNETSET:<secn>` |
| | Examples | `MAGNETSET:BDES` |
| | | `MAGNETSET:BCON` |

## Attribute operation summary

| | | |
| ----------- | -----------  | -----------  |
| Attribute | operation |Description |
|`<prim>:<secn>` | **get** |  Returns a table containing the name of each magnet for specified display group and primary, and secondary values |
| `BDES` | **set** |  Sets specified `BDES` secondary values and returns a table containing a string indicating the state of each specified magnet device, and `BACT` value for each device |
| `VDES` | **set** |  Sets specified `VDES` secondary values and returns a table containing a string indicating the state of each specified magnet device, and `VACT` value for each device |
| `BCON` | **set** |  Sets specified `BCON` secondary values |

## Attribute operations

### <prim>:<secn> : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `MICROS`  |   <start_micro>-<end_micro>  | This specifies a range of micro names of interest within the specified display group  |
| `UNITS`  |   <start_unit>-<end_unit>  | This specified a range of unit numbers of interest within the specified display group  |

_Return value_

| | | | |
|----------- | ----------- | -----------  |-----------  |
| TYPE  | Return Column | Column Type |Description |
| `TABLE` | `name` | `STRING_ARRAY` | name of each magnet for specified display group and primary (optionally filtered by specified micro and unit ranges) |
|  |  `value`| `FLOAT_ARRAY`|  secondary values (BDES or VDES) |



### BDES : set, VDES : set

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `VALUE`*  | structure: {names[], values[]}  | an array of magnet names and an array of corresponding set values  |
| `MAGFUNC`*  |   `TRIM`, `PTRB`, or `NOFUNC`  | Specifies whether a trim or perturb operation will be performed. If `NOFUNC`, neither a trim nor a perturb operation will be performed  |
| `LIMITCHECK`  |  `ALL` or `SOME`  | Used to determine behavior when the set value for one or more devices is outside of its low/high limits. If this parameter is set to `ALL`, the entire request will fail resulting in an exception being thrown and no `BDES`/`VDES` values being set for any of the request devices. If this parameter is set to `SOME`, the set value action will succeed for those set values that are within limits and will not be performed for those set values outside their limits (the state string return values for these devices will be set to the string "Outside Limits"). The default setting of this parameter is `ALL`.  |

_Return value_

| | | | |
|----------- | ----------- | -----------  |-----------  |
| TYPE  | Return Column | Column Type |Description |
| `TABLE` | `state` | `STRING_ARRAY` | string indicating the state of each specified magnet device. Set to blanks (checking that the first character is blank is all that is necessary) if the state is good. Set to the string "Outside Limits" if a set value is outside of its limits and the optional parameter LIMITCHECK is set to SOME |
|  |  `value`| `FLOAT_ARRAY`|  `BACT`/`VACT` value for each device |



### BCON : set

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `VALUE`*  | structure: {names[], values[]}  | an array of magnet names and an array of corresponding set values  |

_Return value_

None



