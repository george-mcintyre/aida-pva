# 1.5 - SLC Klystron Data Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Klystron Data through AIDA-PVA. This
data provider allows the control and data retrieval for specified klystron(s).
For general information on using AIDA-PVA see [Basic Users Guide to Aida](1_00_User_Guide.md), and the EPICS javadoc.

## Summary

Controls and get status from specified klystrons.

## Synopsis

|                          | PV                          | Arguments                   | Description                                                 |
|:-------------------------|-----------------------------|-----------------------------|:------------------------------------------------------------|
| [1.5.1](@ref section151) | `<prim>:<micr>:<unit>:TACT` | `TYPE` `BEAM` [ `DGRP` ]    | Get Klystron Status                                         |
| [1.5.2](@ref section152) | `KLYSTRONGET:TACT`          | `BEAM` `DEVICES` [ `DGRP` ] | Get Multiple Klystron Statuses                              |
| [1.5.3](@ref section153) | `<prim>:<micr>:<unit>:TACT` | `VALUE` `BEAM` [ `DGRP` ]   | Activate or Deactivate Klystron                             |
| [1.5.4](@ref section154) | `<prim>:<micr>:<unit>:PDES` | `VALUE` [ `TRIM` ]          | Set Klystron Desired Phase                                  |
| [1.5.5](@ref section155) | `<prim>:<micr>:<unit>:KPHR` | `VALUE`                     | Set Klystron HPR value                                      |
| [1.5.6](@ref section156) | `<prim>:<micr>:<unit>:ACON` | `VALUE`                     | Set Klystron ACON configuration                             |
|                          | `<prim>:<micr>:<unit>:PCON` | `VALUE`                     | Set Klystron PCON configuration                             |
| [1.5.7](@ref section157) | `KLYSTRONSET:PMDL`          | `VALUE`                     | Set multiple Sub-booster <br/>main drive line values        |

## 1.5.1. Get Klystron Status {#section151}

- `<prim>:<micr>:<unit>:TACT`
    - `TYPE`* : _return-type_
    - `BEAM`* : _beam-code-number_
    - `DGRP` : _display-group-name_

This data provider allows the retrieval of status code(s) or status string(s) for specified klystron(s) on a beam code.

### Arguments

| Argument Names | Argument Values | Description                                                                                                                                     | 
|----------------|-----------------|-------------------------------------------------------------------------------------------------------------------------------------------------|
| `TYPE`*        |                 | _string_: <br/>Determines the return type                                                                                                       |
|                | `SHORT`         | return a short value                                                                                                                            |
|                | `LONG`          | return a long value                                                                                                                             |
|                | `STRING`        | return a string value                                                                                                                           |
|                | `TABLE`         | return a table corresponding to the <br />required accompanying `TABLE_TYPE` argument                                                           |
| `BEAM`*        |                 | _integer_<br/>Beam code number                                                                                                                  |
| `DGRP`         | `DGRP` name     | A display group associated with the specified <br/>Klystron. Must be specified if the Klystron does <br/>not belong to display group `LIN_KLYS` |

### Return value

@note Return value for BPM device shown below. `TORO`, `GAPM`, `KLYS` or `SBST` will be different.

| TYPE     | Description                                                                                                                                                    |
|----------|----------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `SHORT`  | A short value containing the status code for the <br/>klystron on a beam code. See [linklysta.txt](http://www-mcc.slac.stanford.edu/REF_/SLCTXT/LINKLYSTA.TXT) |
| `LONG`   | A long value containing the status code for the <br/>klystron on a beam code. See [linklysta.txt](http://www-mcc.slac.stanford.edu/REF_/SLCTXT/LINKLYSTA.TXT)  |
| `STRING` | A string value containing a status string having <br/>one of two values: "deactivated" or "activated"                                                          |

| TYPE    | Return Column | Column Type     | Description                        |
|---------|---------------|-----------------|------------------------------------|
| `TABLE` | `name`        | `STRING_ARRAY`  | device name `<prim>:<micr>:<unit>` |
|         | `opstat`      | `BOOLEAN_ARRAY` | operation status                   |
|         | `status`      | `SHORT_ARRAY`   | klystron status                    |
|         | `accel`       | `BOOLEAN_ARRAY` | is in accelerate state             |
|         | `standby`     | `BOOLEAN_ARRAY` | is in standby mode                 | 
|         | `bad`         | `BOOLEAN_ARRAY` | is bad?                            |
|         | `sled`        | `BOOLEAN_ARRAY` | true is sled                       |
|         | `sleded`      | `BOOLEAN_ARRAY` | true if sleded                     |
|         | `pampl`       | `BOOLEAN_ARRAY` | true if pampl                      |
|         | `pphas`       | `BOOLEAN_ARRAY` | true if pphas                      |

### Examples

| operation                                            | Response      |
|------------------------------------------------------|---------------|
| `KLYS:LI31:31:TACT BEAM=8 DGRP=DEV_DGRP TYPE=STRING` | `"activated"` | 
| `KLYS:LI31:31:TACT BEAM=8 DGRP=DEV_DGRP TYPE=LONG`   | `18`          | 
| `KLYS:LI31:31:TACT BEAM=8 DGRP=DEV_DGRP TYPE=SHORT`  | `18`          | 

#### Operation

- `KLYS:LI31:31:TACT BEAM=8 DGRP=DEV_DGRP TYPE=TABLE`

#### Response

| Device Name    | Operation Status | Klystron Status | Accelerate | Standby | Is Bad   | Is Sled | Is Sled Tuned | Is Pampl | Is Pphas |
|----------------|-----------------:|-----------------|------------|---------|----------|---------|---------------|----------|----------|
|                |                  |                 |            |         |          |         |               |          |
| name           |           opstat | status          | accel      | standby | bad      | sled    | sleded        | pampl    | pphas    |
| `KLYS:LI31:31` |           `true` | `18`            | `false`    | `true`  | ` false` | `false` | `true`        | `false`  | `false`  |

## 1.5.2. Get Multiple Klystron Statuses {#section152}

- `KLYSTRONGET:TACT`
    - `BEAM`* : _beam-code-number_
    - `DEVICES`* : _list-of-klystron-devices_
    - `DGRP` : _display-group-name_

Get multiple statuses simultaneously.

### Arguments

| Argument Names | Argument Values | Description                                                                                                                           |
|----------------|-----------------|---------------------------------------------------------------------------------------------------------------------------------------|
| `BEAM`*        |          | _integer_<br/>Beam code number                                                                                                                      |
| `DEVICES`*     | Klystron Device | Contains an array of `<prim>:<micr>:<unit>` to get the status of.                                                                     |
| `DGRP`         | Dgrp name       | A display group associated with the specified Klystron. Must be specified if the Klystron does not belong to display group `LIN_KLYS` |

### Return value

| TYPE    | Return Column | Column Type     | Description                        |
|---------|---------------|-----------------|------------------------------------|
| `TABLE` | `name`        | `STRING_ARRAY`  | device name `<prim>:<micr>:<unit>` |
|         | `opstat`      | `BOOLEAN_ARRAY` | operation status                   |
|         | `status`      | `SHORT_ARRAY`   | klystron status                    |
|         | `accel`       | `BOOLEAN_ARRAY` | is in accelerate state             |
|         | `standby`     | `BOOLEAN_ARRAY` | is in standby mode                 | 
|         | `bad`         | `BOOLEAN_ARRAY` | is bad?                            |
|         | `sled`        | `BOOLEAN_ARRAY` | true is sled                       |
|         | `sleded`      | `BOOLEAN_ARRAY` | true if sleded                     |
|         | `pampl`       | `BOOLEAN_ARRAY` | true if pampl                      |
|         | `pphas`       | `BOOLEAN_ARRAY` | true if pphas                      |

### Examples

#### Operations

- `KLYSTRONGET:TACT DEVICES=["KLYS:LI11:11", "KLYS:LI11:21"] BEAM=10`

#### Response

| Device Name  | Operation Status | Klystron Status | Is accelerating | Is in Standby | Is Bad | Is Sled | Is Sleded | Is Pampl | Is PPhas |
|--------------|------------------|-----------------|-----------------|---------------|--------|---------|-----------|----------|----------|
|              |                  |                 |                 |               |        |         |
| `name`       | `opstat`         | `status`        | `accel`         | `standby`     | `bad`  | `sled`  | `sleded`  | `pampl`  | `pphas`  |
| `KLYS:LI11:11` | `true`             | `26`              | `false`           | `true`          | `false`  | `true`    | `true`      | `false`    | `false`    |
| `KLYS:LI11:21` | `true`             | `26`              | `false`           | `true`          | `false`  | `true`    | `true`      | `false`    | `false`    |

## 1.5.3. Activate or Deactivate Klystron {#section153}

- `<prim>:<micr>:<unit>:TACT`
    - `VALUE`* : _float_
    - `BEAM`* : _list-of-klystron-devices_
    - `DGRP` : _display-group-name_

Deactivation or Reactivation of a specified klystron on a beam code.

### Arguments

| Argument Names | Argument Values | Description                                                                                                                           | 
|----------------|-----------------|---------------------------------------------------------------------------------------------------------------------------------------|
| `VALUE`*       |                 | _short_<br/>flag code indicating the desired operation                                                                                |
|                | `0`             | deactivate                                                                                                                            |
|                | `1`             | reactivate.                                                                                                                           |
| `BEAM`*        |                 | _integer_<br/>Beam code number                                                                                                        |
| `DGRP`         | `DGRP` name     | A display group associated with the specified klystron. Must be specified if the klystron does not belong to display group `LIN_KLYS` |

### Return value

| TYPE    | Return Column | Column Type   | Description                                                                                                                             |
|---------|---------------|---------------|-----------------------------------------------------------------------------------------------------------------------------------------|
| `TABLE` | `status`      | `SHORT_ARRAY` | Status code for the specified klystron on a beam code - See [linklysta.txt](http://www-mcc.slac.stanford.edu/REF_/SLCTXT/LINKLYSTA.TXT) |

### Examples

#### Operations

- `KLYS:LI31:31:TACT BEAM=8 DGRP=DEV_DGRP VALUE=0`

| Klystron Status |
|-----------------|
|                 |
| `status`        |
| `26`            |

## 1.5.4. Set Klystron Desired Phase {#section154}

- `<prim>:<micr>:<unit>:PDES`
    - `VALUE`* : _short_
    - `TRIM` : _boolean_

Set `PDES` value for a klystron or sub-booster and optionally trim the phase.

### Arguments

| Argument Names | Argument Values | Description                                                                                             | 
|----------------|-----------------|---------------------------------------------------------------------------------------------------------|
| `VALUE`*       |                 | _float_<br/>The desired phase setting                                                                   |
| `TRIM`         |          | _boolean_<br/>Default: `YES`. <br/>Controls whether the optional <br/>trim operation will be carried out.         |
|                | `YES`           | If not specified or set to `YES`, the trim phase operation will be performed.                           |
|                | `NO`            | If specified and set to `NO`, no trim phase operation will be performed after setting the `PDES` value. |

### Return value

| TYPE    | Return Column | Column Type   | Description                                                                            |
|---------|---------------|---------------|----------------------------------------------------------------------------------------|
| `TABLE` | `phas`        | `FLOAT_ARRAY` | The PHAS secondary value after the set `PDES` and <br />optional trim phase operation. |

### Examples

#### Operations

- `KLYS:LI31:31:PDES VALUE=90.0`

| Klystron Status |
|-----------------|
|                 |
| `phas`             |
| `26`            |

## 1.5.5. Set Klystron HPR value {#section155}

- `<prim>:<micr>:<unit>:KPHR`
    - `VALUE`* : _float_

Set the `KPHR` value for a klystron or sub-booster.

### Arguments

| Argument Names | Argument Values | Description                     | 
|----------------|-----------------|---------------------------------|
| `VALUE`*       |            | _float_<br/>The desired KPHR setting |

### Return value

| TYPE    | Return Column | Column Type   | Description                                             |
|---------|---------------|---------------|---------------------------------------------------------|
| `TABLE` | `phas`        | `FLOAT_ARRAY` | The PHAS secondary value after the set `KPHR` operation |

### Examples

#### Operations

- `KLYS:LI31:31:KPHR VALUE=60.0`

| Klystron Status |
|-----------------|
|                 |
| `phas`          |
| `26`            |

## 1.5.6. Set Klystron ACON and PCON configurations {#section156}

- `<prim>:<micr>:<unit>:ACON`
    - `VALUE`* : _desired-ACON-setting_
- `<prim>:<micr>:<unit>:PCON`
    - `VALUE`* : _desired-PCON-setting_

Set the configuration value, `PCON` or `ACON`, value of a specified klystron or sub-booster.

### Arguments

| Argument Names | Argument Values | Description                                | 
|----------------|-----------------|--------------------------------------------|
| `VALUE`*       |                 | _float_<br/>desired `PCON` or `ACON` value |

### Return value

None

### Examples

| operation                     |
|-------------------------------|
| `KLYS:LI31:31:ACON VALUE=5.0` | 
| `KLYS:LI31:31:PCON VALUE=5.0` |

## 1.5.7. Set Multiple Sub-booster Main Drive Line values {#section157}

- `KLYSTRONSET:PMDL`
    - `VALUE`* : _structure_
        - `NAMES`* : _list-of-sub-booster-names_
        - `VALUES`* : _list-of-values_

The service allows multiple values to be set for the main drive line phase (PMDL) and other secondaries of specified
sub-boosters simultaneously.


### Arguments
| Argument Names | Argument Values                 | Description                                                  | 
|----------------|---------------------------------|--------------------------------------------------------------|
| `VALUE`*       | `{ <names-array>, <values-array> }` | a structure containing congruent <br/>arrays for names and values |
|                | `<names-array>`                 | an array of sub-booster names                                |
|                | `<values-array>`                | an array of sub-booster set values                           |

### Return value

None

### Examples
| operation                                                                                |
|------------------------------------------------------------------------------------------|
| `KLYSTRONSET:PMDL' VALUE='{"names":["SBST:LI00:1", "SBST:LI01:1"], "values":[0.1, 1.2]}` | 

## CLI and Code Examples
### Commandline Examples

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">example type</th><th class="markdownTableHeadNone">example</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">**pvcall**</td>

<td class="markdownTableBodyNone">

```shell
pvcall "KLYS:LI31:31:TACT" BEAM=8 DGRP=DEV_DGRP TYPE=SHORT
pvcall "KLYS:LI31:31:TACT" BEAM=8 DGRP=DEV_DGRP TYPE=LONG
pvcall "KLYS:LI31:31:TACT" BEAM=8 DGRP=DEV_DGRP TYPE=STRING
pvcall "KLYS:LI31:31:TACT" BEAM=8 DGRP=DEV_DGRP TYPE=TABLE
``` 

``` shell
pvcall "KLYSTRONGET:TACT" BEAM=8 DGRP=DEV_DGRP DEVICES='["KLYS:LI31:31", "KLYS:LI31:32"]'
"Device Name" "Operation Status" "Klystron Status" Accellerate Standby   Bad "Sled Tuned" Sleded Pampl pphas
 KLYS:LI31:31               true                18       false    true false        false   true false false
 KLYS:LI31:32              false            -32763       false   false false        false  false false false
```

```shell
pvcall "KLYSTRONGET:SCAN" FILE='TEST.BTN'
"step"       "pdes"        "BPM Name" "x offset (mm)" "y offset (mm)" "num particles (coulomb)"
     1  -0.9996438     BPMS:LI11:501      0.45966175      0.81614506                   1.0E-10
     1  -0.9996438     BPMS:LI11:502      -0.9996438      -0.9657358                   1.0E-10
     2  -0.9996439     BPMS:LI11:501     -0.28856283      0.69943863                   1.0E-10
     2  -0.9996439     BPMS:LI11:502      -0.9565048     -0.91913205                   1.0E-10
     3  -0.9996440     BPMS:LI11:501       0.5896404     -0.47585112                   1.0E-10
     3  -0.9996440     BPMS:LI11:502      0.45966175      0.81614506                   1.0E-10
     4  -0.9996441     BPMS:LI11:501      -0.9996438      -0.9657358                   1.0E-10
     4  -0.9996441     BPMS:LI11:502     -0.28856283      0.69943863                   1.0E-10
     5  -0.9996442     BPMS:LI11:501      -0.9565048     -0.91913205                   1.0E-10
     5  -0.9996442     BPMS:LI11:502       0.5896404     -0.47585112                   1.0E-10
```

```shell
pvcall "KLYS:LI31:31:TACT" BEAM=8 DGRP=DEV_DGRP VALUE=0
pvcall "KLYS:LI31:31:PDES" VALUE=90.0
pvcall "KLYS:LI31:31:KPHR" VALUE=60.0
pvcall "KLYS:LI31:31:PCON" VALUE=5.0
```

```shell
pvcall 'KLYSTRONSET:PMDL' VALUE='{"names":["SBST:LI00:1", "SBST:LI01:1"], "values":[0.1, 1.2]}'
```

</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">**eget**</td>

<td class="markdownTableBodyNone">

```shell
eget -s KLYS:LI31:31:TACT -a BEAM 8 -a DGRP 'DEV_DGRP' -a TYPE 'SHORT'
eget -s KLYS:LI31:31:TACT -a BEAM 8 -a DGRP 'DEV_DGRP' -a TYPE 'LONG'
eget -s KLYS:LI31:31:TACT -a BEAM 8 -a DGRP 'DEV_DGRP' -a TYPE 'STRING'
eget -s KLYS:LI31:31:TACT -a BEAM 8 -a DGRP 'DEV_DGRP' -a TYPE 'TABLE'
```

```shell
eget -s KLYSTRONGET:TACT -a BEAM 8 -a DGRP 'DEV_DGRP' -a DEVICES '["KLYS:LI31:31", "KLYS:LI31:32"]'
eget -s KLYSTRONGET:SCAN -a FILE 'TEST.BTN'
```

```shell
eget -s KLYS:LI31:31:TACT -a BEAM 8 -a DGRP 'DEV_DGRP' -a VALUE 0
eget -s KLYS:LI31:31:PDES -a VALUE 90.0
eget -s KLYS:LI31:31:KPHR -a VALUE 60.0
eget -s KLYS:LI31:31:PCON -a VALUE 5.0
```

```shell
eget -s KLYSTRONSET:PMDL -a VALUE '{"names": ["SBST:LI00:1", "SBST:LI01:1"], "values":[0.1, 1.2] }'
```

</td>
</tr>

</table>

### Matlab Examples

@note For general details about accessing AIDA-PVA from matlab see [User Guide for Matlab Users](1_12_Matlab_Code.md)

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">action</th><th class="markdownTableHeadNone">example</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```matlab
try
    builder = pvaRequest('KLYS:LI31:31:TACT');
    builder.with('BEAM', 8).with('DGRP', 'DEV_DGRP');
    builder.returning(AIDA_SHORT);
    shortResponse = builder.get()
catch e
    handleExceptions(e);
end

shortResponse =
    18
```

```matlab
try
    builder = pvaRequest('KLYS:LI31:31:TACT');
    builder.with('BEAM', 8).with('DGRP', 'DEV_DGRP');
    builder.returning(AIDA_STRING);
    stringResponse = builder.get()
catch e
    handleExceptions(e);
end

stringResponse =
activated
```

```matlab
try
    builder = pvaRequest('KLYSTRONGET:TACT');
    builder.with('BEAM', 8).with('DGRP', 'DEV_DGRP').with('devices', {'KLYS:LI31:31' 'KLYS:LI31:32'});
    tableResponse = ML(builder.get())
catch e
    handleExceptions(e);
end

tableResponse =
            size: 2
          labels: {'Device Name'  'Operation Status'  'Klystron Status'  'Accellerate'  'Standby'  'Bad'  'Sled Tuned'  'Sleded'  'Pampl'  'pphas'}
           units: []
    descriptions: []
      fieldnames: {'name'  'opstat'  'status'  'accel'  'standby'  'bad'  'sled'  'sleded'  'pampl'  'pphas'}
          values: [1x1 struct]

tableResponse.values.status(1)
ans =
   18

tableResponse.values.opstat(1)
ans =
     1

tableResponse.values.name(1)
ans =
    'KLYS:LI31:31'

tableResponse.values
ans =
       name: {'KLYS:LI31:31'  'KLYS:LI31:32'}
     opstat: [1 0]
     status: [18 32757]
      accel: [0 0]
    standby: [1 0]
        bad: [0 0]
       sled: [0 0]
     sleded: [1 0]
      pampl: [0 0]
      pphas: [0 0]
```

```matlab
try
    builder = pvaRequest('KLYSTRONGET:SCAN');
    builder.with('FILE', 'TEST.BTN');
    tableResponse = ML(builder.get())
catch e
    handleExceptions(e);
end

"primary step" "secondary step"    "BPM Name" "pulse id" "x offset (mm)" "y offset (mm)" "num particles (coulomb)" "stat" "good measurement" "phase"
    -0.9996438              0.0 BPMS:LI11:501      71311      0.45966175      0.81614506                   1.0E-10      1                 15    0.38

tableResponse =
            size: 2
          labels: {'primary step' 'secondary step'    'BPM Name' 'pulse id' 'x offset ()' 'y offset (mm)' 'num particles (coulomb)' 'stat' 'good measurement' 'phase'}
           units: []
    descriptions: []
      fieldnames: {'primary_step'  'secondary_step'  'name'  'pulseId'  'x'  'y'  'tmits'  'stat'  'goodmeas'  'phas'}
          values: [320x1 struct]

tableResponse.values.status(1)
ans =
   320

tableResponse.values.primary_step(1)
ans =
     -0.9996438

tableResponse.values.name(1)
ans =
    'BPMS:LI11:501'

tableResponse.values
ans =
         primary_step: {-0.9996438 -0.9996439 ...}
       secondary_step: {0.0 0.0  ...}
                 name: {'BPMS:LI11:501' 'BPMS:LI11:501'  ...}
              pulseId: [71311 71311 ...]
                    x: [0.45966175 -0.9996438 ...]
                    y: [0.81614506 -0.9657358 ...]
                tmits: [1.0E-10 1.0E-10 ...]
                 stat: [1 1 ...]
             goodmeas: [15 15 ...]
                 phas: [0.38 0.21 ...]
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Set</td>
<td class="markdownTableBodyNone">

```matlab
try
    builder = pvaRequest('KLYS:LI31:31:TACT');
    builder.with('BEAM', 8);
    builder.with('DGRP', 'DEV_DGRP');
    statuses = ML(builder.set(0));
    status = statuses(1)
catch e
    handleExceptions(e);
end

status =
     0
```

```matlab
try
    builder = pvaRequest('KLYS:LI31:31:TACT');
    builder.with('BEAM', 8);
    builder.with('DGRP', 'DEV_DGRP');
    statuses = ML(builder.set(90.0));
    phas = statuses(1)
catch e
    handleExceptions(e);
end
```

```matlab
try
    pvaSetM('KLYS:LI31:31:KPHR', 60.0)
catch e
    handleExceptions(e);
end
```

```matlab
try
    pvaSet('KLYS:LI31:31:PCON', 5.0);
catch e
    handleExceptions(e);
end
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">Multi-Set</td>
<td class="markdownTableBodyNone">

```matlab
try
    builder = pvaRequest('KLYSTRONSET:PMDL');
    jstruct = AidaPvaStruct();
    jstruct.put('names', { 'SBST:LI00:1', 'SBST:LI01:1'});
    jstruct.put('values', { 0.1, 1.2 } );
    builder.set(jstruct);
catch e
    handleExceptions(e);
end
```

</td>
</tr>
</table>

### Java Examples

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">example type</th><th class="markdownTableHeadNone">example</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">**AidaPvaClient**</td>

<td class="markdownTableBodyNone">

```java
import edu.stanford.slac.aida.client.PvaTable;
import org.epics.pvaccess.server.rpc.RPCRequestException;

import java.util.*;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.pvaRequest;
import static edu.stanford.slac.aida.client.AidaPvaClientUtils.pvaSet;
import static edu.stanford.slac.aida.client.AidaType.*;

public class Example {
    public Short getStatusShort() throws RPCRequestException {
        return (Short) pvaRequest("KLYS:LI31:31:TACT")
                .with("BEAM", 8)
                .with("DGRP", "DEV_DGRP")
                .returning(AIDA_SHORT)
                .get();
    }

    public Long getStatusLong() throws RPCRequestException {
        return (Long) pvaRequest("KLYS:LI31:31:TACT")
                .with("BEAM", 8)
                .with("DGRP", "DEV_DGRP")
                .returning(AIDA_LONG)
                .get();
    }

    public String getStatusString() throws RPCRequestException {
        return (String) pvaRequest("KLYS:LI31:31:TACT")
                .with("BEAM", 8)
                .with("DGRP", "DEV_DGRP")
                .returning(AIDA_STRING)
                .get();
    }

    public PvaTable getStatusTable() throws RPCRequestException {
        return (PvaTable) pvaRequest("KLYS:LI31:31:TACT")
                .with("BEAM", 8)
                .with("DGRP", "DEV_DGRP")
                .returning(AIDA_TABLE)
                .get();
    }

    public PvaTable getStatuses(Integer beamCode, List<String> devices) throws RPCRequestException {
        return (PvaTable) pvaRequest("KLYSTRONGET:TACT")
                .with("BEAM", beamCode)
                .with("DEVICES", devices)
                .get();
    }

    public PvaTable activateKlystron(Integer beamCode, Boolean activateStatus) throws RPCRequestException {
        return pvaRequest("KLYS:LI31:31:TACT")
                .with("BEAM", beamCode)
                .set(activateStatus);
    }

    public PvaTable setPdes(Float value) throws RPCRequestException {
        return pvaRequest("KLYS:LI31:31:PDES").set(value);
    }

    public PvaTable setKhpr(Float value) throws RPCRequestException {
        return pvaRequest("KLYS:LI31:31:KPHR").set(value);
    }

    public void setPconAcon(Float value) throws RPCRequestException {
        pvaSet("KLYS:LI31:31:PCON", value);
        pvaSet("KLYS:LI31:31:ACON", value);
    }

    public void setPmdls(List<String>subBoosters, List<Float> values) throws RPCRequestException {
        pvaSet("KLYSTRONSET:PMDL", Map.of("names", subBoosters, "values", values));
    }
}
```

</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">**PvaClient**</td>

<td class="markdownTableBodyNone">

```java
import org.epics.nt.NTURI;
import org.epics.pvaClient.PvaClient;
import org.epics.pvaClient.PvaClientChannel;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.pv.PVLong;
import org.epics.pvdata.pv.PVShort;
import org.epics.pvdata.pv.PVString;
import org.epics.pvdata.pv.PVStructure;

import java.util.List;

public class Example {
    public Short getStatusShort(Integer beamCode) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("beam")
                .addQueryString("type")
                .addQueryString("dgrp")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("beam").put(beamCode);
        uri.getQuery().getStringField("type").put("SHORT");
        uri.getQuery().getStringField("dgrp").put("DEV_DGRP");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(uri.getPVStructure());

        PVShort field = response.getSubField(PVShort.class, "value");
        return field.get();
    }

    public Long getStatusLong(Integer beamCode) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("beam")
                .addQueryString("type")
                .addQueryString("dgrp")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("beam").put(beamCode);
        uri.getQuery().getStringField("type").put("LONG");
        uri.getQuery().getStringField("dgrp").put("DEV_DGRP");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(uri.getPVStructure());

        PVLong field = response.getSubField(PVLong.class, "value");
        return field.get();
    }

    public String getStatusString(Integer beamCode) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("beam")
                .addQueryString("type")
                .addQueryString("dgrp")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("beam").put(beamCode);
        uri.getQuery().getStringField("type").put("STRING");
        uri.getQuery().getStringField("dgrp").put("DEV_DGRP");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(uri.getPVStructure());

        PVString field = response.getSubField(PVString.class, "value");
        return field.get();
    }

    public PVStructure getStatusTable(Integer beamCode) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("beam")
                .addQueryString("type")
                .addQueryString("dgrp")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("beam").put(beamCode);
        uri.getQuery().getStringField("type").put("TABLE");
        uri.getQuery().getStringField("dgrp").put("DEV_DGRP");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        return channel.rpc(uri.getPVStructure());
    }

    public PVStructure getStatuses(Integer beamCode, List<String> devices) throws RPCRequestException {
        String pvName = "KLYSTRONGET:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("devices")
                .addQueryInt("beam")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("devices").put("[\"KLYS:LI31:31\", \"KLYS:LI31:32\"]");
        uri.getQuery().getIntField("beam").put(beamCode);

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        return channel.rpc(uri.getPVStructure());
    }

    public PVStructure activateKlystron(Integer beamCode, Boolean activateStatus) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:PDES";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("value")
                .addQueryInt("beam")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("value").put(activateStatus.toString());
        uri.getQuery().getIntField("beam").put(beamCode);

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        return channel.rpc(uri.getPVStructure());
    }

    public PVStructure setPdes(Double value) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:PDES";

        NTURI uri = NTURI.createBuilder().addQueryDouble("value").create();
        uri.getPath().put(pvName);
        uri.getQuery().getDoubleField("value").put(value);

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        return channel.rpc(uri.getPVStructure());
    }

    public PVStructure setKhpr(Double value) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:KPHR";

        NTURI uri = NTURI.createBuilder().addQueryDouble("value").create();
        uri.getPath().put(pvName);
        uri.getQuery().getDoubleField("value").put(value);

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        return channel.rpc(uri.getPVStructure());
    }

    public void setPconAcon(Double value) throws RPCRequestException {

        NTURI uri = NTURI.createBuilder().addQueryDouble("value").create();
        uri.getQuery().getDoubleField("value").put(value);

        String pvName = "KLYS:LI31:31:PCON";
        uri.getPath().put(pvName);
        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        channel.rpc(uri.getPVStructure());

        pvName = "KLYS:LI31:31:ACON";
        uri.getPath().put(pvName);
        channel = client.createChannel(pvName);
        channel.rpc(uri.getPVStructure());
    }

    public void setPmdls() throws RPCRequestException {
        String pvName = "KLYSTRONSET:PMDL";

        NTURI uri = NTURI.createBuilder().addQueryString("value").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("value").put("{\"names\":[\"SBST:LI00:1\", \"SBST:LI01:1\"], \"values\":[0.1, 1.2]}");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        channel.rpc(uri.getPVStructure());
    }
}
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td rowspan=2 class="markdownTableBodyNone">**PvAccess**</td>

<td class="markdownTableBodyNone">

```java
import org.epics.nt.NTURI;
import org.epics.pvaccess.client.rpc.RPCClientImpl;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.pv.*;

import java.util.List;

public class Example {
    public Short getStatusShort(Integer beamCode) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("beam")
                .addQueryString("type")
                .addQueryString("dgrp")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("beam").put(beamCode);
        uri.getQuery().getStringField("type").put("SHORT");
        uri.getQuery().getStringField("dgrp").put("DEV_DGRP");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();


        PVShort field = response.getSubField(PVShort.class, "value");
        return field.get();
    }

    public Long getStatusLong(Integer beamCode) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("beam")
                .addQueryString("type")
                .addQueryString("dgrp")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("beam").put(beamCode);
        uri.getQuery().getStringField("type").put("LONG");
        uri.getQuery().getStringField("dgrp").put("DEV_DGRP");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();


        PVLong field = response.getSubField(PVLong.class, "value");
        return field.get();
    }

    public String getStatusString(Integer beamCode) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("beam")
                .addQueryString("type")
                .addQueryString("dgrp")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("beam").put(beamCode);
        uri.getQuery().getStringField("type").put("STRING");
        uri.getQuery().getStringField("dgrp").put("DEV_DGRP");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();


        PVString field = response.getSubField(PVString.class, "value");
        return field.get();
    }

    public PVStructure getStatusTable(Integer beamCode) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("beam")
                .addQueryString("type")
                .addQueryString("dgrp")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("beam").put(beamCode);
        uri.getQuery().getStringField("type").put("TABLE");
        uri.getQuery().getStringField("dgrp").put("DEV_DGRP");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        return response;
    }

    public PVStructure getStatuses(Integer beamCode, List<String> devices) throws RPCRequestException {
        String pvName = "KLYSTRONGET:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("devices")
                .addQueryInt("beam")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("devices").put("[\"KLYS:LI31:31\", \"KLYS:LI31:32\"]");
        uri.getQuery().getIntField("beam").put(beamCode);

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        return response;
    }

    public PVStructure activateKlystron(Integer beamCode, Boolean activateStatus) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:PDES";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("value")
                .addQueryInt("beam")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("value").put(activateStatus.toString());
        uri.getQuery().getIntField("beam").put(beamCode);

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        return response;
    }

    public PVStructure setPdes(Double value) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:PDES";

        NTURI uri = NTURI.createBuilder().addQueryDouble("value").create();
        uri.getPath().put(pvName);
        uri.getQuery().getDoubleField("value").put(value);

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        return response;
    }

    public PVStructure setKhpr(Double value) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:KPHR";

        NTURI uri = NTURI.createBuilder().addQueryDouble("value").create();
        uri.getPath().put(pvName);
        uri.getQuery().getDoubleField("value").put(value);

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        return response;
    }

    public void setPconAcon(Double value) throws RPCRequestException {

        NTURI uri = NTURI.createBuilder().addQueryDouble("value").create();
        uri.getQuery().getDoubleField("value").put(value);

        String pvName = "KLYS:LI31:31:PCON";
        uri.getPath().put(pvName);
        RPCClientImpl client = new RPCClientImpl(pvName);
        client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        pvName = "KLYS:LI31:31:ACON";
        uri.getPath().put(pvName);
        client = new RPCClientImpl(pvName);
        client.request(uri.getPVStructure(), 3.0);
        client.destroy();
    }

    public void setPmdls() throws RPCRequestException {
        String pvName = "KLYSTRONSET:PMDL";

        NTURI uri = NTURI.createBuilder().addQueryString("value").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("value").put("{\"names\":[\"SBST:LI00:1\", \"SBST:LI01:1\"], \"values\":[0.1, 1.2]}");

        RPCClientImpl client = new RPCClientImpl(pvName);
        client.request(uri.getPVStructure(), 3.0);
        client.destroy();
    }
}
```

</td>
</tr>

</table>
