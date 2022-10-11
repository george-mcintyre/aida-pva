# 1.5 - SLC Klystron Data Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Klystron Data through AIDA-PVA. This
data provider allows the retrieval of status code(s) or status string(s) for specified klystron(s) on a beam code. It
also allows the deactivation or reactivation of a specified klystron on a beam code. The `PDES` value for a klystron or
sub-booster may be set and the phase may be optionally trimmed. The `KPHR` value for a klystron or sub-boster may be set.
The service allows multiple values to be set for the main drive line phase (PMDL) and other secondaries
of specified sub-boosters simultaneously. 
A configuration value (`PCON` or `ACON`) of a specified klystron or sub-booster may also be set. 

A new feature of this provider is its ability to perform fast phase scan/dither for Klystron Golding.  A correlated plot
that has been set up and saved as a button file can be run and the results returned by this new feature. 

For general
information on using AIDA-PVA see [Basic Users Guide to Aida](1_00_User_Guide.md), and the EPICS javadoc.

## Summary

Supports **get**, **set**  and **multi-set** operations.

There are three **get** operations that can be performed:

1. obtain the status code or string for specified klystron on a beam code, 
2. obtain multiple status codes and strings for the specified klystrons on a beam code, 
3. get the results returned from running a specified phase scan - a correlated plot.

There are five **set** operations that can be performed:

1. deactivate or reactivate a specified klystron on a beam code,
2. set the `PDES` value and optionally trim the phase of a specified klystron or sub-booster,
3. set the `KPHR` value of a specified klystron or sub-booster, and
4. set a configuration value (`PCON` or `ACON`) of a specified klystron or sub-booster,
5. sets `PMDL` values for multiple specified sub-boosters simultaneously.

## Instances and Attributes

| operation          | info     | instance & attribute        |
|--------------------|----------|-----------------------------|
| **get**            | Syntax   | `<prim>:<micr>:<unit>:TACT` |
|                    | Examples | `KLYS:LI31:31:TACT`         |
| multi- **get**     | Syntax   | `KLYSTRONGET:TACT`          |
|                    | Examples | `KLYSTRONGET:TACT`          |
| phase scan **get** | Syntax   | `KLYSTRONGET:SCAN`          |
|                    | Examples | `KLYSTRONGET:SCAN`          |
| **set**            | Syntax   | `<prim>:<micr>:<unit>:TACT` |
|                    |          | `<prim>:<micr>:<unit>:PDES` |
|                    |          | `<prim>:<micr>:<unit>:KPHR` |
|                    |          | `<prim>:<micr>:<unit>:PCON` |
|                    |          | `<prim>:<micr>:<unit>:ACON` |
|                    | Examples | `KLYS:LI31:31:TACT`         |
|                    |          | `KLYS:LI31:31:PDES`         |
|                    |          | `KLYS:LI31:31:KPHR`         |
|                    |          | `KLYS:LI31:31:PCON`         |
|                    |          | `KLYS:LI31:31:ACON`         |
| **multi-set**      | Syntax   | `KLYSTRONSET:<secn>`        |
|                    | Examples | `KLYSTRONSET:PMDL`          |

## Attribute operation summary

| Attribute | operation     |     | Description                                                                                  |
|-----------|---------------|:----|----------------------------------------------------------------------------------------------|
| `TACT`    | **get**       |     | Gets status code(s), or status string(s) for the specified klystron(s) on a beam code        |
| `SCAN`    | **get**       |     | Executes the specified phase scan and returns phase and BPM data                             |
| `TACT`    | **set**       |     | Deactivates or reactivates a specified klystron on a beam code                               |
| `PDES`    | **set**       |     | Sets the `PDES` value and optionally trims the phase for a specified klystron or sub-booster |
| `KPHR`    | **set**       |     | Sets the `KPHR` value of a specified klystron or sub-booster                                 |
| `PCON`    | **set**       |     | Sets the `PCON` value of a specified klystron or sub-booster                                 |
| `ACON`    | **set**       |     | Sets the `ACON` value of a specified klystron or sub-booster                                 |
| `PMDL`    | **multi-set** |     | Sets the `PMDL` values of the specified sub-boosters                                         |

## Attribute operations

### TACT : get

_Parameters_

| Parameter Names | Parameter Values | Description                                                                                                                           | 
|-----------------|------------------|---------------------------------------------------------------------------------------------------------------------------------------|
| `TYPE`*         | return type      | Must be one of `SHORT`, `LONG`, `STRING`, or `TABLE`                                                                                  |
| `BEAM`*         | Integer          | Beam code number                                                                                                                      |
| `DGRP`          | Dgrp name        | A display group associated with the specified Klystron. Must be specified if the Klystron does not belong to display group `LIN_KLYS` |

_Return value_

@note Return value for BPM device shown below. `TORO`, `GAPM`, `KLYS` or `SBST` will be different.

| TYPE     | Description                                                                                                                                               |
|----------|-----------------------------------------------------------------------------------------------------------------------------------------------------------|
| `SHORT`  | A short value containing the status code for the klystron on a beam code. See [linklysta.txt](http://www-mcc.slac.stanford.edu/REF_/SLCTXT/LINKLYSTA.TXT) |
| `LONG`   | A long value containing the status code for the klystron on a beam code. See [linklysta.txt](http://www-mcc.slac.stanford.edu/REF_/SLCTXT/LINKLYSTA.TXT)  |
| `STRING` | A string value containing a status string having one of two values: "deactivated" or "activated"                                                          |

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

### TACT : multi-get

_Parameters_

| Parameter Names | Parameter Values | Description                                                                                                                           | 
|-----------------|------------------|---------------------------------------------------------------------------------------------------------------------------------------|
| `BEAM`*         | Integer          | Beam code number                                                                                                                      |
| `DGRP`          | Dgrp name        | A display group associated with the specified Klystron. Must be specified if the Klystron does not belong to display group `LIN_KLYS` |
| `DEVICES`*      | Klystron Device  | Contains an array of `<prim>:<micr>:<unit>` to get the status of.                                                                     |

_Return value_

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
### SCAN : phase scan get
Prior to calling this AIDA-PVA endpoint the operator needs to set up the parameters of a Correlation Plot 
in the Data Acquisition panel for "Correlation Plots" of the SCP application program and save them
to a button file.  The correlation plot application can be used for a large
variety of plots but the only step and sample variables that this provider supports are:

- **Step Variables**: `KLYS:PDES` only. Iterating the desired phase of a sub-booster. 
- **Sample Variables**: Type: `BPM` only; Sample: `X`, `Y`, or `TMIT`. e.g. `BPMS:LI20:3315:TMIT`

The single parameter `FILE` is then used when this endpoint is accessed to retrieve these parameters and 
execute the correlation plot and return the results.

_Parameters_

| Parameter Names | Parameter Values | Description                                                                                                                         | 
|-----------------|------------------|-------------------------------------------------------------------------------------------------------------------------------------|
| `FILE`*         | Button File Name | Button file name. <br />including the "BTN" suffix.  The file must be in the standard <br />button file directory `SLC_CRR_BUTTON:` |

_Return value_

| TYPE    | Return Column | Column Type     | Description                                                               |
|---------|---------------|-----------------|---------------------------------------------------------------------------|
| `TABLE` | `step`        | `INTEGER_ARRAY` | The step variable number, 1,2,3,...<br/>repeats for each name             |
|         | `pdes`        | `FLOAT_ARRAY`   | The step variable: `PDES` desired phase setting<br/>repeats for each name |
|         | `name`        | `STRING_ARRAY`  | name of each BPM sample variable                                          |
|         | `x`           | `FLOAT_ARRAY`   | x offsets (mm)                                                            |
|         | `y`           | `FLOAT_ARRAY`   | y offsets (mm)                                                            |
|         | `tmits`       | `FLOAT_ARRAY`   | tmits (num particles)                                                     |

_Example Return values_

| step | phase  | name           | x             | y             | tmits         |
|------|--------|----------------|---------------|---------------|---------------|
| `1`  | `0.26` | BPMS:LI20:2445 | `0.103910382` | `0.103910382` | `0.103910382` |
| `1`  | `0.26` | BPMS:LI20:3013 | `0.103910382` | `0.103910382` | `0.103910382` |
| `1`  | `0.26` | BPMS:LI20:3036 | `0.103910382` | `0.103910382` | `0.103910382` |
| `1`  | `0.26` | BPMS:LI20:3120 | `0.103910382` | `0.103910382` | `0.103910382` |
| `1`  | `0.26` | BPMS:LI20:3101 | `0.103910382` | `0.103910382` | `0.103910382` |
| `1`  | `0.26` | BPMS:LI20:3156 | `0.103910382` | `0.103910382` | `0.103910382` |
| `2`  | `0.27` | BPMS:LI20:2445 | `0.203910382` | `0.303910382` | `0.403910382` |
| `2`  | `0.27` | BPMS:LI20:3013 | `0.203910382` | `0.303910382` | `0.403910382` |
| `2`  | `0.27` | BPMS:LI20:3036 | `0.203910382` | `0.303910382` | `0.403910382` |
| `2`  | `0.27` | BPMS:LI20:3120 | `0.203910382` | `0.303910382` | `0.403910382` |
| `2`  | `0.27` | BPMS:LI20:3101 | `0.203910382` | `0.303910382` | `0.403910382` |
| `2`  | `0.27` | BPMS:LI20:3156 | `0.203910382` | `0.303910382` | `0.403910382` |
| `3`  | `0.28` | BPMS:LI20:2445 | `0.303910382` | `0.503910382` | `0.703910382` |
| `3`  | `0.28` | BPMS:LI20:3013 | `0.303910382` | `0.503910382` | `0.703910382` |
| `3`  | `0.28` | BPMS:LI20:3036 | `0.303910382` | `0.503910382` | `0.703910382` |
| `3`  | `0.28` | BPMS:LI20:3120 | `0.303910382` | `0.503910382` | `0.703910382` |
| `3`  | `0.28` | BPMS:LI20:3101 | `0.303910382` | `0.503910382` | `0.703910382` |
| `3`  | `0.28` | BPMS:LI20:3156 | `0.303910382` | `0.503910382` | `0.703910382` |


### TACT : set

_Parameters_

| Parameter Names | Parameter Values | Description                                                                                                                           | 
|-----------------|------------------|---------------------------------------------------------------------------------------------------------------------------------------|
| `VALUE`*        | Short            | Short flag code indicating the desired operation: `0` => deactivate, `1` => reactivate.                                               |
| `BEAM`*         | Integer          | Beam code number                                                                                                                      |
| `DGRP`          | Dgrp name        | A display group associated with the specified klystron. Must be specified if the klystron does not belong to display group `LIN_KLYS` |

_Return value_

| TYPE    | Return Column | Column Type   | Description                                                                                                                             |
|---------|---------------|---------------|-----------------------------------------------------------------------------------------------------------------------------------------|
| `TABLE` | `status`      | `SHORT_ARRAY` | Status code for the specified klystron on a beam code - See [linklysta.txt](http://www-mcc.slac.stanford.edu/REF_/SLCTXT/LINKLYSTA.TXT) |

### PDES : set

_Parameters_

| Parameter Names | Parameter Values | Description                                                                                                                                                                                          | 
|-----------------|------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `VALUE`*        | Short            | Short flag code indicating the desired operation: `0` => deactivate, `1` => reactivate.                                                                                                              |
| `TRIM`          | Boolean          | `YES` or `NO`. If specified and set to `NO`, no trim phase operation will be performed after setting the `PDES` value. If not specified or set to `YES`, the trim phase operation will be performed. |

_Return value_

| TYPE    | Return Column | Column Type   | Description                                                                            |
|---------|---------------|---------------|----------------------------------------------------------------------------------------|
| `TABLE` | `phas`        | `FLOAT_ARRAY` | The PHAS secondary value after the set `PDES` and <br />optional trim phase operation. |

### KPHR : set

_Parameters_

| Parameter Names | Parameter Values | Description           | 
|-----------------|------------------|-----------------------|
| `VALUE`*        | Float            | desired `KPHR` value. |

_Return value_

| TYPE    | Return Column | Column Type   | Description                                             |
|---------|---------------|---------------|---------------------------------------------------------|
| `TABLE` | `phas`        | `FLOAT_ARRAY` | The PHAS secondary value after the set `KPHR` operation |

### PCON : set, ACON : set

_Parameters_

| Parameter Names | Parameter Values | Description                     | 
|-----------------|------------------|---------------------------------|
| `VALUE`*        | Float            | desired `PCON` or `ACON` value. |

_Return value_

None

### PMDL : multi-set

_Parameters_

| Parameter Names | Parameter Values               | Description                                                            | 
|-----------------|--------------------------------|------------------------------------------------------------------------|
| `VALUE`*        | structure: {names[], values[]} | an array of sub-booster names and an array of corresponding set values |

_Return value_

None

## Examples

### Commandline Examples

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">example type</th><th class="markdownTableHeadNone">action</th><th class="markdownTableHeadNone">example</th></tr>
<tr class="markdownTableRowOdd">
<td rowspan=3 class="markdownTableBodyNone">commandline **pvcall**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```shell
pvcall "KLYS:LI31:31:TACT" BEAM=8 DGRP=DEV_DGRP TYPE=SHORT
pvcall "KLYS:LI31:31:TACT" BEAM=8 DGRP=DEV_DGRP TYPE=LONG
pvcall "KLYS:LI31:31:TACT" BEAM=8 DGRP=DEV_DGRP TYPE=STRING
pvcall "KLYS:LI31:31:TACT" BEAM=8 DGRP=DEV_DGRP TYPE=TABLE
pvcall "KLYSTRONGET:TACT" BEAM=8 DGRP=DEV_DGRP DEVICES='["KLYS:LI31:31", "KLYS:LI31:32"]'
"Device Name" "Operation Status" "Klystron Status" Accellerate Standby   Bad "Sled Tuned" Sleded Pampl pphas
 KLYS:LI31:31               true                18       false    true false        false   true false false
 KLYS:LI31:32              false            -32763       false   false false        false  false false false
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

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Set</td>
<td class="markdownTableBodyNone">

```shell
pvcall "KLYS:LI31:31:TACT" BEAM=8 DGRP=DEV_DGRP VALUE=0
pvcall "KLYS:LI31:31:PDES" VALUE=90.0
pvcall "KLYS:LI31:31:KPHR" VALUE=60.0
pvcall "KLYS:LI31:31:PCON" VALUE=5.0
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">Multi-Set</td>
<td class="markdownTableBodyNone">

```shell
pvcall 'KLYSTRONSET:PMDL' VALUE='{"names":["SBST:LI00:1", "SBST:LI01:1"], "values":[0.1, 1.2]}'
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td rowspan=3 class="markdownTableBodyNone">commandline **eget**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```shell
eget -s KLYS:LI31:31:TACT -a BEAM 8 -a DGRP 'DEV_DGRP' -a TYPE 'SHORT'
eget -s KLYS:LI31:31:TACT -a BEAM 8 -a DGRP 'DEV_DGRP' -a TYPE 'LONG'
eget -s KLYS:LI31:31:TACT -a BEAM 8 -a DGRP 'DEV_DGRP' -a TYPE 'STRING'
eget -s KLYS:LI31:31:TACT -a BEAM 8 -a DGRP 'DEV_DGRP' -a TYPE 'TABLE'
eget -s KLYSTRONGET:TACT -a BEAM 8 -a DGRP 'DEV_DGRP' -a DEVICES '["KLYS:LI31:31", "KLYS:LI31:32"]'
eget -s KLYSTRONGET:SCAN -a FILE 'TEST.BTN'
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">Set</td>
<td class="markdownTableBodyNone">

```shell
eget -s KLYS:LI31:31:TACT -a BEAM 8 -a DGRP 'DEV_DGRP' -a VALUE 0
eget -s KLYS:LI31:31:PDES -a VALUE 90.0
eget -s KLYS:LI31:31:KPHR -a VALUE 60.0
eget -s KLYS:LI31:31:PCON -a VALUE 5.0
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Multi-Set</td>
<td class="markdownTableBodyNone">

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
<tr class="markdownTableHead"><th class="markdownTableHeadNone">example type</th><th class="markdownTableHeadNone">action</th><th class="markdownTableHeadNone">example</th></tr>
<tr class="markdownTableRowOdd">
<td rowspan=2 class="markdownTableBodyNone">java **AidaPvaClient**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```java
import org.epics.pvaccess.server.rpc.RPCRequestException;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.*;
import static edu.stanford.slac.aida.client.AidaType.*;

public class AidaPvaClientExample {
    public Short getShort() throws RPCException {
        return pvaRequest("KLYS:LI31:31:TACT")
                .with("BEAM", 8)
                .with("DGRP", "DEV_DGRP")
                .returning(SHORT)
                .get();
    }
}
```

```java
import org.epics.pvaccess.server.rpc.RPCRequestException;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.*;
import static edu.stanford.slac.aida.client.AidaType.*;

public class AidaPvaClientExample {
    public String getString() throws RPCException {
        return pvaRequest("KLYS:LI31:31:TACT")
                .with("BEAM", 8)
                .with("DGRP", "DEV_DGRP")
                .returning(STRING)
                .get();
    }
}
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Set</td>
<td class="markdownTableBodyNone">

```java
import org.epics.pvaccess.server.rpc.RPCRequestException;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.*;
import static edu.stanford.slac.aida.client.AidaType.*;

public class AidaPvaClientExample {
    public AidaTable setShort(Short value) throws RPCException {
        return pvaRequest("KLYS:LI31:31:TACT")
                .with("BEAM", 8)
                .with("DGRP", "DEV_DGRP")
                .set(value);
    }
}
```

```java
import org.epics.pvaccess.server.rpc.RPCRequestException;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.*;
import static edu.stanford.slac.aida.client.AidaType.*;

public class AidaPvaClientExample {
    public AidaTable setFloat(FLoat value) throws RPCException {
        return pvaRequest("KLYS:LI31:31:PDES")
                .set(value);
    }
}
```

```java
import org.epics.pvaccess.server.rpc.RPCRequestException;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.*;
import static edu.stanford.slac.aida.client.AidaType.*;

public class AidaPvaClientExample {
    public void setFloat(Float value) throws RPCException {
        pvaSet("KLYS:LI31:31:KPHR", value);
    }
}
```

```java
import org.epics.pvaccess.server.rpc.RPCRequestException;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.*;
import static edu.stanford.slac.aida.client.AidaType.*;

public class AidaPvaClientExample {
    public void setFloat(Float value) throws RPCException {
        pvaSet("KLYS:LI31:31:PCON", value);
    }
}
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td rowspan=2 class="markdownTableBodyNone">java **PvaClient**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```java
import org.epics.pvaClient.*;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.factory.FieldFactory;
import org.epics.pvdata.factory.PVDataFactory;
import org.epics.pvdata.pv.*;

public class PvaClientExample {
    public Short getShort() throws RPCRequestException {
        String pvName = "KLYS:LI31:31:TACT";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"beam", "dgrp", "type"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvInt),
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvString)
                });

        Structure uriStructure = fieldCreate.createStructure(
                new String[]{"path", "scheme", "query"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvString),
                        arguments
                });

        PVStructure request = dataCreate.createPVStructure(uriStructure);
        request.getStringField("scheme").put("pva");
        request.getStringField("path").put(pvName);

        PVStructure args = request.getStringField("query");
        args.getIntField("beam").put(8);
        args.getStringField("dgrp").put("DEV_DGRP");
        args.getStringField("type").put("SHORT");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(request);

        PVShort field = response.getSubField(PVShort.class, "value");
        return field.get();
    }
}
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Set</td>
<td class="markdownTableBodyNone">

```java
import org.epics.pvaClient.*;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.factory.FieldFactory;
import org.epics.pvdata.factory.PVDataFactory;
import org.epics.pvdata.pv.*;

public class PvaClientExample {
    public void setFloat(Float value) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:TACT";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"beam", "dgrp", "value"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvInt),
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvShort)
                });

        Structure uriStructure = fieldCreate.createStructure(
                new String[]{"path", "scheme", "query"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvString),
                        arguments
                });

        PVStructure request = dataCreate.createPVStructure(uriStructure);
        request.getStringField("scheme").put("pva");
        request.getStringField("path").put(pvName);

        PVStructure args = request.getStringField("query");
        args.getIntField("beam").put(8);
        args.getStringField("dgrp").put("DEV_DGRP");
        args.getShortField("value").put(value);

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        channel.rpc(request);
    }
}
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td rowspan=2 class="markdownTableBodyNone">java **EasyPVA**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```java
import org.epics.pvaccess.*;
import org.epics.pvaccess.easyPVA.*;
import org.epics.pvdata.*;
import org.epics.pvdata.pv.PVStructure;

import java.lang.String;

public class EzExample {
    public static final String NTURI_ID = "epics:nt/NTURI:1.0";
    private final static FieldCreate fieldCreate = factory.FieldFactory.getFieldCreate();
    private final static PVDataCreate dataCreate = factory.PVDataFactory.getPVDataCreate();

    public Short getShort() throws RuntimeException {
        String pvName = "KLYS:LI31:31:TACT";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"beam", "dgrp", "type"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvInt),
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvString)
                });

        Structure uriStructure = fieldCreate.createStructure(
                new String[]{"path", "scheme", "query"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvString),
                        arguments
                });

        PVStructure request = dataCreate.createPVStructure(uriStructure);
        request.getStringField("scheme").put("pva");
        request.getStringField("path").put(pvName);

        PVStructure args = request.getStringField("query");
        args.getIntField("beam").put(8);
        args.getStringField("dgrp").put("DEV_DGRP");
        args.getStringField("type").put("SHORT");

        EasyChannel channel = easypva.createChannel(pvName);
        if (!channel.connect(5.0)) {
            throw new RuntimeException("Unable to connect");
        }

        EasyRPC easyrpc = channel.createRPC();
        if (!easypva.getStatus().isOK()) {
            throw new RuntimeException("Unable to create RPC channel");
        }

        if (!easyrpc.connect()) {
            throw new RuntimeException("Unable to connect to RPC channel");
        }

        PVStructure response = easyrpc.request(request);
        if (!easypva.getStatus().isOK()) {
            throw new RuntimeException("Unable to get data");
        }

        PVShort field = response.getSubField(PVShort.class, "value");
        return field.get();
    }
}
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Set</td>
<td class="markdownTableBodyNone">

```java
import org.epics.pvaccess.*;
import org.epics.pvaccess.easyPVA.*;
import org.epics.pvdata.*;
import org.epics.pvdata.pv.PVStructure;

import java.lang.String;

public class EzExample {
    public static final String NTURI_ID = "epics:nt/NTURI:1.0";
    private final static FieldCreate fieldCreate = factory.FieldFactory.getFieldCreate();
    private final static PVDataCreate dataCreate = factory.PVDataFactory.getPVDataCreate();

    public void setFloat(Float value) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:TACT";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"beam", "dgrp", "value"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvInt),
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvShort)
                });

        Structure uriStructure = fieldCreate.createStructure(
                new String[]{"path", "scheme", "query"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvString),
                        arguments
                });

        PVStructure request = dataCreate.createPVStructure(uriStructure);
        request.getStringField("scheme").put("pva");
        request.getStringField("path").put(pvName);

        PVStructure args = request.getStringField("query");
        args.getIntField("beam").put(8);
        args.getStringField("dgrp").put("DEV_DGRP");
        args.getShortField("value").put(value);

        EasyChannel channel = easypva.createChannel(pvName);
        if (!channel.connect(5.0)) {
            throw new RuntimeException("Unable to connect");
        }

        EasyRPC easyrpc = channel.createRPC();
        if (!easypva.getStatus().isOK()) {
            throw new RuntimeException("Unable to create RPC channel");
        }

        if (!easyrpc.connect()) {
            throw new RuntimeException("Unable to connect to RPC channel");
        }

        easyrpc.request(request);
        if (!easypva.getStatus().isOK()) {
            throw new RuntimeException("Unable to get data");
        }
    }
}
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td rowspan=2 class="markdownTableBodyNone">java **PvAccess**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```java
import org.epics.pvaccess.ClientFactory;
import org.epics.pvaccess.client.rpc.RPCClientImpl;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.factory.FieldFactory;
import org.epics.pvdata.factory.PVDataFactory;
import org.epics.pvdata.pv.*;

public class JavaExample {
    public Short getShort() throws RuntimeException {
        String pvName = "KLYS:LI31:31:TACT";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"beam", "dgrp", "type"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvInt),
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvString)
                });

        Structure uriStructure = fieldCreate.createStructure(
                new String[]{"path", "scheme", "query"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvString),
                        arguments
                });

        PVStructure request = dataCreate.createPVStructure(uriStructure);
        request.getStringField("scheme").put("pva");
        request.getStringField("path").put(pvName);

        PVStructure args = request.getStringField("query");
        args.getIntField("beam").put(8);
        args.getStringField("dgrp").put("DEV_DGRP");
        args.getStringField("type").put("SHORT");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(request, 3.0);
        client.destroy();

        PVFloat field = response.getSubField(PVFloat.class, "value");
        return field.get();
    }
}
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Set</td>
<td class="markdownTableBodyNone">

```java
import org.epics.pvaccess.ClientFactory;
import org.epics.pvaccess.client.rpc.RPCClientImpl;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.factory.FieldFactory;
import org.epics.pvdata.factory.PVDataFactory;
import org.epics.pvdata.pv.*;

public class JavaExample {
    public void setFloat(Float value) throws RPCRequestException {
        String pvName = "KLYS:LI31:31:TACT";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"beam", "dgrp", "value"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvInt),
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvShort)
                });

        Structure uriStructure = fieldCreate.createStructure(
                new String[]{"path", "scheme", "query"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvString),
                        arguments
                });

        PVStructure request = dataCreate.createPVStructure(uriStructure);
        request.getStringField("scheme").put("pva");
        request.getStringField("path").put(pvName);

        PVStructure args = request.getStringField("query");
        args.getIntField("beam").put(8);
        args.getStringField("dgrp").put("DEV_DGRP");
        args.getShortField("value").put(value);

        RPCClientImpl client = new RPCClientImpl(pvName);
        client.request(request, 3.0);
        client.destroy();
    }
}
```

</td>
</tr>

</table>

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

████ Test 7: Set a sub-booster or klystron PDES value without the trim operation
_________________________________________________

set: KLYS:LI31:31:PDES (TRIM=NO, VALUE=90.0)
    PDES:  1 rows retrieved: ✔
 PHAS secondary value
                 PHAS
                  0.0

████ Test 8: Set a sub-booster or klystron KPHR value
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


