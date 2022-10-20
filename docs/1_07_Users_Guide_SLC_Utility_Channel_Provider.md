# 1.7 - SLC Utilities Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Utilities through AIDA-PVA.
For general information on using AIDA-PVA see [Basic Users Guide to Aida](1_00_User_Guide.md), and the
EPICS javadoc.

## Summary

- Retrieve trigger device status
- Deactivation and reactivation of a trigger device.
- Control devices via knobs using multiknob files
- Set BGRP variable value
- Set and retrieve SCP transverse feedback
- Execute correlation plots

## Synopsis

|                          | PV                               | Arguments                | Description                                       |
|:-------------------------|----------------------------------|--------------------------|:--------------------------------------------------|
| [1.7.1](@ref section171) | `<prim>:<micr>:<unit>:TACT`      | `TYPE` `BEAM`            | Retrieve trigger device status                    |
| [1.7.2](@ref section172) | `<prim>:<micr>:<unit>:TACT`      | `VALUE` `BEAM`           | Deactivation and reactivation of a trigger device |
| [1.7.3](@ref section173) | `MKB:VAL`                        | `VALUE` `MKB`            | Control devices via knobs using multiknob files   |
| [1.7.4](@ref section174) | `BGRP:VAL`                       | `VALUE` `BGRP` `VARNAME` | Set BGRP variable value                           |
| [1.7.5](@ref section175) | `<prim>:<micr>:<unit>:LOOP_STAT` | `TYPE`                   | Get SCP transverse feedback                       |
| [1.7.6](@ref section176) | `<prim>:<micr>:<unit>:LOOP_STAT` | `VALUE`                  | Set SCP transverse feedback                       |
| [1.7.7](@ref section177) | `CORRPLOT:SCAN`                  | `FILE` `STEPVARS`        | Execute correlation plots                         |

## 1.7.1. Retrieve trigger device status {#section171}

- `<prim>:<micr>:<unit>:TACT`
    - `TYPE`* : _type_
        - `SHORT`
        - `LONG`
        - `STRING`
    - `BEAM`* : _beam code_

Allows the retrieval of the status code or a status string for a specified trigger device on a beam code.

### Arguments

| Argument Names | Argument Values | Description                                            |
|----------------|-----------------|--------------------------------------------------------|
| `TYPE`*        |                 | _string_<br/>Determines the return type of the request |
|                | `SHORT`         | return a short value                                   |
|                | `LONG`          | return a long value                                    |
|                | `STRING`        | return a string value                                  |
| `BEAM`*        | Integer         | Beam code number                                       |

### Return value

| TYPE     | Description                                                                                                                |
|----------|----------------------------------------------------------------------------------------------------------------------------|
| `SHORT`  | A short value containing the status code for the trigger device on a beam code: <br />`0` => deactivated, `1` => activated |
| `LONG`   | A long value containing the status code for the trigger device on a beam code: <br />`0` => deactivated, `1` => activated  |
| `STRING` | A string value containing a status string having one of two values: "deactivated" or "activated"                           |

### Examples

| operation                               | Response      |
|-----------------------------------------|---------------|
| `TRIG:LI31:109:TACT BEAM=1 TYPE=SHORT`  | `1`           | 
| `TRIG:LI31:109:TACT BEAM=1 TYPE=LONG`   | `0`           | 
| `TRIG:LI31:109:TACT BEAM=1 TYPE=STRING` | `"activated"` | 

## 1.7.2. Deactivation and reactivation of a trigger device {#section172}

- `<prim>:<micr>:<unit>:TACT`
    - `VALUE`* : _operation_
    - `BEAM`* : _beam code_

Allows the deactivation or reactivation of a specified trigger device on a beam code.

### Arguments

| Argument Names | Argument Values | Description                                              |
|----------------|-----------------|----------------------------------------------------------|
| `VALUE`*       |                 | _integer_<br/>Flag code indicating the desired operation |
|                | `0`             | deactivate                                               |
|                | `1`             | reactivate                                               |
| `BEAM`*        |                 | _integer_<br/>Beam code number                           |

### Return value

| TYPE    | Return Column | Column Type   | Description                                                 |
|---------|---------------|---------------|-------------------------------------------------------------|
| `TABLE` | `value`       | `SHORT_ARRAY` | status code for the specified trigger device on a beam code |
|         |               | `0`           | trigger device has been deactivated                         |
|         |               | `1`           | trigger device has been activated                           |

### Examples

#### Operations

- `TRIG:LI31:109:TACT BEAM=1 VALUE=1`
- `TRIG:LI31:109:TACT BEAM=1 VALUE=0`

#### Response

| value   |
|---------|
|         |
| `value` |
| `1`     |

## 1.7.3. Control devices via knobs using multiknob files {#section173}

- `MKB:VAL`
    - `VALUE`* : _knob rotation value_
    - `MKB`* : _multiknob filename_

Allows the setting of devices referenced in a specified multiknob file by knob rotation using a specified relative
delta value. Only a relative (not absolute) multiknob file may be specified.

### Arguments

| Argument Names | Argument Values | Description                                                                                                                                                                    |
|----------------|-----------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `VALUE`*       |            | _float_<br/>relative delta knob rotation value.                                                                                                                                |
| `MKB`          |           | _string_<br/>multiknob filename. The filename path specification <br />(including the prefix "mkb:", if file is in the standard <br />multiknob file directory) of a multiknob file |

### Return value

| TYPE    | Return Column | Column Type    | Description                                                    |
|---------|---------------|----------------|----------------------------------------------------------------|
| `TABLE` | `name`        | `STRING_ARRAY` | name of each device referenced in the specified multiknob file | 
|         | `value`       | `DOUBLE_ARRAY` | value of each device after the knob rotation set operation     |

### Examples

#### Operations

- `MKB:VAL MKB='mkb:li02b_xb.mkb' VALUE=1.0`

#### Response

| name                | value   |
|:--------------------|---------|
|                     |         |
| `name`              | `value` |
| `KLYS:LI31:31:PDES` | `0.26`  |
| `KLYS:LI11:31:PDES` | `0.11`  |

## 1.7.4. Set BGRP variable value {#section174}

- `BGRP:VAL`
    - `VALUE`* : _value_
    - `BGRP`* : _button group name_
    - `VARNAME`* : _button group variable name_

Allows for the setting of a `BGRP` variable value. Sets the value of a specified variable for a specified `BGRP`

### Arguments

| Argument Names | Argument Values | Description                                                |
|----------------|-----------------|------------------------------------------------------------|
| `VALUE`*       |                 | _boolean_<br/>specifying the new variable value            |
|                | `true`          |                                                            |
|                | `false`         |                                                            |
| `BGRP`*        |                 | _string_<br/>The BGRP name                                 |
| `VARNAME`*     |                 | _string_<br/>A BGRP variable name for the specified BGRP.. |

### Return value

None

### Examples

| operation                                      |
|------------------------------------------------|
| `BGRP:VAL BGRP=LCLS VARNAME=T_CAV VALUE='Yes'` | 

## 1.7.5. Get SCP transverse feedback {#section175}

- `<prim>:<micr>:<unit>:LOOP_STAT`
    - `TYPE`* : _type_

Allows for getting SCP transverse feedback status. Gets a code or a string that describes the status of the specified
SCP transverse feedback

### Arguments

| Argument Names | Argument Values | Description                                              |
|----------------|-----------------|----------------------------------------------------------|
| `TYPE`*        | Return type     | _string_: <br/>Determines the return type of the request |
|                | `BOOLEAN`       | return a boolean value.                                  |
|                | `SHORT`         | return a short value.                                    |
|                | `INTEGER`       | return an integer value.                                 |
|                | `LONG`          | return a long value.                                     |
|                | `STRING`        | return a string value.                                   |

### Return value

| TYPE      | Return Value | Description                                                                                                 |
|-----------|--------------|-------------------------------------------------------------------------------------------------------------|
| `BOOLEAN` |              | A boolean value containing the SCP transverse feedback                                                      |
|           | `true`       | in feedback state                                                                                           |
|           | `false`      | not in feedback state                                                                                       |
| `SHORT`   |              | A short value containing the SCP transverse feedback <br />status code for the loop device                  |
|           | `0`          | off                                                                                                         |
|           | `1`          | compute                                                                                                     |
|           | `2`          | feedback                                                                                                    |
| `INTEGER` |              | An integer value containing the SCP transverse feedback <br />status code for the loop device               |
|           | `0`          | off                                                                                                         |
|           | `1`          | compute                                                                                                     |
|           | `2`          | feedback                                                                                                    |
| `LONG`    |              | A long value containing the SCP transverse feedback <br />status code for the loop: <br /> the HSTA bitmask |
| `STRING`  |              | A string value containing a SCP transverse feedback <br />status string having one of six values            |
|           | `"off"`      | loop is in off state                                                                                        |
|           | `"sample"`   | loop is in sample state                                                                                     |
|           | `"compute"`  | loop is in compute state                                                                                    |
|           | `"actuate"`  | loop is in actuate state                                                                                    |
|           | `"dither"`   | loop is in dither state                                                                                     |
|           | `"feedback"` | loop is in feedback state                                                                                   |

### Examples

| operation                             | Response     |
|---------------------------------------|--------------|
| `FBCK:LI18:28:LOOP_STAT TYPE=BOOLEAN` | `true`       | 
| `FBCK:LI18:28:LOOP_STAT TYPE=SHORT`   | `1`          |
| `FBCK:LI18:28:LOOP_STAT TYPE=LONG`    | `2`          |
| `FBCK:LI18:28:LOOP_STAT TYPE=STRING`  | `"feedback"` |

## 1.7.6. Set SCP transverse feedback {#section176}

- `<prim>:<micr>:<unit>:LOOP_STAT`
    - `VALUE`* : _value_

Allows for setting and getting SCP transverse feedback status. Sets the specified SCP transverse feedback to: `off`
, `sample`, `compute`/`on`, `actuate`, `dither`, or `feedback`

### Arguments

| Argument Names | Argument Values | Description                                            |
|----------------|-----------------|--------------------------------------------------------|
| `VALUE`*       |                 | _string_<br/>Determines the return type of the request |
|                | `"OFF"`         | set transverse feedback status to `off`.               |
|                | `"SAMPLE"`      | set transverse feedback status to `sample`.            |
|                | `"COMPUTE"`     | set transverse feedback status to `compute`.           |
|                | `"ON"`          | set transverse feedback status to `on`.                |
|                | `"ACTUATE"`     | set transverse feedback status to `actuate`.           |
|                | `"DITHER"`      | set transverse feedback status to `dither`.            |
|                | `"FEEDBACK"`    | set transverse feedback status to `feedback`.          |

### Return value

None

### Examples

| operation                               |
|-----------------------------------------|
| `FBCK:LI18:28:LOOP_STAT VALUE=OFF`      | 
| `FBCK:LI18:28:LOOP_STAT VALUE=COMPUTE`  |
| `FBCK:LI18:28:LOOP_STAT VALUE=ON`       |
| `FBCK:LI18:28:LOOP_STAT VALUE=FEEDBACK` |

## 1.7.7. Execute correlation plots {#section177}

- `CORRPLOT:SCAN`
    - `FILE`* : _plot config button file_
    - `PRIMSTV`* : _primary step variable structure_
        - `name`*: _device_
        - `low`*: _low value_
        - `high`*: _high value_
        - `steps`*: _number of steps_
    - `SECNSTV` : _secondary step variable structure_
        - `name`*: _device_
        - `low`*: _low value_
        - `high`*: _high value_
        - `steps`*: _number of steps_

Executes a correlation plot by loading the parameters from the given file and applying the specified
primary step variable (and optionally secondary step variable). The function returns
a matrix that cross references the step variable values with the sample values at each step.

@note this returns a special kind of AIDA-PVA table. Normally the number of columns are fixed, but in this case
we return a variable number of columns, albeit with predictable names. The labels for these columns are
variable.

Prior to calling this AIDA-PVA endpoint the operator needs to set up the parameters of a Correlation Plot
in the Data Acquisition panel for "Correlation Plots" of the SCP application program and save them
to a button file.

@note some correlation plots may take an extended amount of time to run so you may need
to [increase the timeout for a request](@ref increaseTimeoutUtil) when running this command.

### Arguments

| Argument Names | Argument Values            | Description                                                                                                                                   |
|----------------|----------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------|
| `FILE`*        |                            | _string_<br/>Button file name including the "BTN" suffix.<br/>  The file must be in the standard button file directory<br/> `SLC_CRR_BUTTON:` |
| `PRIMSTV`*     | `{name, low, high, steps}` | _structure_<br/>Specifies the primary step variable parameters                                                                                |
|                | `name`*                    | _string_<br/>The name of the SLC data source to use                                                                                           |
|                | `low`*                     | _float_<br/>The start value that the plot function will <br/>step the value of the source from                                                |
|                | `high`*                    | _float_<br/>The end value that the plot function will <br/>step the value of the source to                                                    |
|                | `steps`*                   | _integer_<br/>The number of steps that the plot function will<br/>take to scan from low to high                                               |
| `SECNSTV`      | `{name, low, high, steps}` | _structure_<br/>Specifies the secondary step variable parameters                                                                              |
|                | `name`*                    | _string_<br/>The name of the SLC data source to use                                                                                           |
|                | `low`*                     | _float_<br/>The start value that the plot function will <br/>step the value of the source from                                                |
|                | `high`*                    | _float_<br/>The end value that the plot function will <br/>step the value of the source to                                                    |
|                | `steps`*                   | _integer_<br/>The number of steps that the plot function will<br/>take to scan from low to high                                               |

### Return value

| TYPE    | Return Column       | Column Type   | Description                                                                                                                                                                                                                                                                |
|---------|---------------------|---------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `TABLE` | `primary`           | `FLOAT_ARRAY` | value of the primary step variable for this step                                                                                                                                                                                                                           |
|         | `secondary`         | `FLOAT_ARRAY` | value of the secondary step variable for this step.<br/>If no secondary step value is defined <br/>then this column will be omitted from the output table                                                                                                                  |
|         | `samp<samp-number>` | `FLOAT_ARRAY` | Column names are created by concatenating "samp" <br/>with the _sample number_.  <br/>e.g. first non-`ZERO` sample variable will<br/> be called: `samp1` .<br/>The label of the field will be set to <br/>the sample SLC data source <br/>name. e.g. `BPMS:LI20:2445:TMIT` |
|         | ...                 | ...           | Repeat for each sample variable                                                                                                                                                                                                                                            |

### Examples

#### Operations

- `CORRPLOT:SCAN FILE=FACET_BPMS.BTN PRIMSTV={"name": "KLYS:LI31:31:PDES", "low": 0.5, "high": 1.0, "steps": 6}`

#### Response

| KLYS:LI31:31:PDES | BPMS:LI20:2445:X | BPMS:LI20:2445:Y | BPMS:LI20:2445:TMITS |
|------------------:|-----------------:|-----------------:|---------------------:|
|                   |                  |                  |                      |
|         `primary` |          `samp1` |          `samp2` |              `samp3` |
|             `0.5` |    `0.103910382` |   `0.9549588893` |       `0.0000000012` |
|             `0.6` |    `0.103910383` |   `0.9549588892` |       `0.0000000013` |
|             `0.7` |    `0.103910384` |   `0.9549588891` |       `0.0000000014` |
|             `0.8` |    `0.103910385` |   `0.9549588880` |       `0.0000000015` |
|             `0.9` |    `0.103910386` |   `0.9549588889` |       `0.0000000016` |
|             `1.0` |    `0.103910387` |   `0.9549588888` |       `0.0000000017` |

## Increasing timeout for AIDA-PVA requests {#increaseTimeoutUtil}

For requests that can take a long time you may need to increase the request timeout. Depending
on how you are accessing AIDA-PVA the method you use to do this will vary.

| method          | Steps                                                                                                                                                                                                                                                                                                        |
|-----------------|:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| matlab          | 1. Add the `timeout()` to the builder<br/><br/>e.g. `.timeout(30)`                                                                                                                                                                                                                                           | 
| aida-pva-client | 1. Add the `timeout()` to the builder<br/><br/>e.g. `.timeout(30)`                                                                                                                                                                                                                                           | 
| pvaClient       | 1. Add the `timeout` field to query<br/><br/>e.g. `.addQueryInt("timeout")`<br/><br/>2. Set `timeout` field to  desired value<br/><br/>e.g. `uri.getQuery().getIntField("timeout").put(30)` <br/><br/>3. Set pvaClient timeout<br/><br/>e.g. `client.channel(pvName, "pva", 30.0)`                           | 
| PvAccess        | 1. Add the `timeout` field to query<br/><br/><br/>e.g. `.addQueryInt("timeout")`<br/><br/>2. Set `timeout` field to  desired value<br/><br/>e.g. `uri.getQuery().getIntField("timeout").put(30)` <br/><br/>3. Set pvAccess timeout when executing<br/><br/>e.g. `client.request(uri.getPVStructure(), 30.0)` | 
| `pvcall`        | 1. Add the `timeout` field to query<br/><br/>e.g. ` ... TIMEOUT=30`<br/><br/>2. Set `pvcall` timeout when executing<br/><br/>e.g. `pvcall -w 30 ...`                                                                                                                                                         | 
| `eget`          | 1. Add the `timeout` field to query<br/><br/>e.g. ` ... TIMEOUT=30`<br/><br/>2. Set `eget` timeout when executing<br/><br/>e.g. `eget -w 30 ...`                                                                                                                                                             | 

## CLI and Code Examples

### Commandline Examples

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">example type</th><th class="markdownTableHeadNone">example</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">**pvcall**</td>

<td class="markdownTableBodyNone">

```shell
pvcall "TRIG:LI31:109:TACT" BEAM=1 TYPE=SHORT
pvcall "TRIG:LI31:109:TACT" BEAM=1 TYPE=LONG
pvcall "TRIG:LI31:109:TACT" BEAM=1 TYPE=STRING

pvcall "TRIG:LI31:109:TACT" BEAM=1 VALUE=0

pvcall "MKB:VAL" MKB='mkb:li02b_xb.mkb' VALUE=1.0 

pvcall "BGRP:VAL" BGRP=LCLS VARNAME=T_CAV VALUE='Yes'

pvcall "FBCK:LI18:28:LOOP_STAT" TYPE=BOOLEAN
pvcall "FBCK:LI18:28:LOOP_STAT" TYPE=SHORT
pvcall "FBCK:LI18:28:LOOP_STAT" TYPE=INTEGER
pvcall "FBCK:LI18:28:LOOP_STAT" TYPE=LONG
pvcall "FBCK:LI18:28:LOOP_STAT" TYPE=STRING

pvcall "FBCK:LI18:28:LOOP_STAT" VALUE='feedback'

pvcall "CORRPLOT:SCAN" FILE='FACET_BPMS.BTN' PRIMSTV='{"name": "KLYS:LI31:31:PDES", "low": 0.5, "high": 1.0, "steps": 6}'
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">**eget**</td>

<td class="markdownTableBodyNone">

```shell
eget -s TRIG:LI31:109:TACT -a BEAM 1 -a TYPE SHORT 
eget -s TRIG:LI31:109:TACT -a BEAM 1 -a TYPE LONG 
eget -s TRIG:LI31:109:TACT -a BEAM 1 -a TYPE STRING 

eget -s TRIG:LI31:109:TACT -a BEAM 1 VALUE 0

eget -s FBCK:LI18:28:LOOP_STAT 1 VALUE compute

eget -s MKB:VAL -a MKB 'mkb:li02b_xb.mkb' -a VALUE 1.0 

eget -s BGRP:VAL -a BGRP LCLS -a VARNAME T_CAV -a VALUE=Y

eget -s FBCK:LI18:28:LOOP_STAT -a TYPE BOOLEAN 
eget -s FBCK:LI18:28:LOOP_STAT -a TYPE SHORT 
eget -s FBCK:LI18:28:LOOP_STAT -a TYPE INTEGER 
eget -s FBCK:LI18:28:LOOP_STAT -a TYPE LONG 
eget -s FBCK:LI18:28:LOOP_STAT -a TYPE STRING 

eget -s FBCK:LI18:28:LOOP_STAT -a VALUE 'FEEDBACK' 

eget -s CORRPLOT:SCAN -a FILE 'FACET_BPMS.BTN' -a PRIMSTV '{"name": "KLYS:LI31:31:PDES", "low": 0.5, "high": 1.0, "steps": 6}' 
```

</td>
</tr>

</table>

### Matlab Examples

@note For general details about accessing AIDA-PVA from matlab see [User Guide for Matlab Users](1_12_Matlab_Code.md)

<table class="markdownTable">
<tr class="markdownTableHead">
<th class="markdownTableHeadNone">example</th></tr>
<tr class="markdownTableRowOdd">

<td class="markdownTableBodyNone">

```matlab
try
    builder = pvaRequest('TRIG:LI31:109:TACT');
    builder.with('BEAM', 1);
    builder.returning(AIDA_SHORT)
    shortResponse = builder.get()
catch e
    handleExceptions(e);
end
shortResponse =
     0
```

```matlab
try
    builder = pvaRequest('TRIG:LI31:109:TACT');
    builder.with('BEAM', 1);
    builder.returning(AIDA_LONG)
    longResponse = builder.get()
catch e
    handleExceptions(e);
end
longResponse =
     0
```

```matlab
try
    builder = pvaRequest('TRIG:LI31:109:TACT');
    builder.with('BEAM', 1);
    builder.returning(AIDA_STRING)
    stringResponse = builder.get()
catch e
    handleExceptions(e);
end
stringResponse =

deactivated
```

```matlab
try
    builder = pvaRequest('TRIG:LI31:109:TACT');
    builder.with('BEAM', 1);
    tableResponse = ML(builder.set(0))
catch e
    handleExceptions(e);
end

tableResponse =
            size: 1
          labels: {'value'}
           units: []
    descriptions: []
      fieldnames: {'value'}
          values: [1x1 struct]
```

```matlab
try
    builder = pvaRequest('MKB:VAL');
    builder.with('MKB', 'mkb:li02b_xb.mkb');
    tableResponse = ML(builder.set(1.0))
catch e
    handleExceptions(e);
end

tableResponse =
            size: 1
          labels: {'value'}
           units: []
    descriptions: []
      fieldnames: {'value'}
          values: [1x1 struct]
```

```matlab
try
    builder = pvaRequest('BGRP:VAL');
    builder.with('BGRP', 'LCLS');
    builder.with('VARNAME', 'T_CAV');
    builder.set('Y')
catch e
    handleExceptions(e);
end
```

```matlab
try
    builder = pvaRequest('FBCK:LI18:28:LOOP_STAT');
    builder.returning(AIDA_BOOLEAN)
    booleanResponse = builder.get()
catch e
    handleExceptions(e);
end
booleanResponse =
     true
```

```matlab
try
    builder = pvaRequest('FBCK:LI18:28:LOOP_STAT');
    builder.returning(AIDA_SHORT)
    shortResponse = builder.get()
catch e
    handleExceptions(e);
end
shortResponse =
     0
```

```matlab
try
    builder = pvaRequest('FBCK:LI18:28:LOOP_STAT');
    builder.returning(AIDA_LONG)
    longResponse = builder.get()
catch e
    handleExceptions(e);
end
longResponse =
     1
```

```matlab
try
    builder = pvaRequest('FBCK:LI18:28:LOOP_STAT');
    builder.returning(AIDA_STRING)
    stringResponse = builder.get()
catch e
    handleExceptions(e);
end
stringResponse =

feedback
```

```matlab
try
    pvaSet('FBCK:LI18:28:LOOP_STAT', 'compute');
catch e
    handleExceptions(e);
end
```

```matlab
try
    builder = pvaRequest('CORRPLOT:SCAN"');
    builder.with('FILE', 'FACET_BPMS.BTN');
    jstruct = AidaPvaStruct();
    jstruct.put('name', { 'KLYS:LI31:31:PDES'});
    jstruct.put('low', { 0.5 } );
    jstruct.put('high', { 1.0 } );
    jstruct.put('steps', { 6 } );
    builder.with('PRIMSTV', jstruct);
    builder.timeout(30);
    tableResponse = ML(builder.get())
catch e
    handleExceptions(e);
end

tableResponse =
            size: 6
          labels: {'KLYS:LI31:31:PDES'  'BPMS:LI20:2445:X'  'BPMS:LI20:2445:Y'  'BPMS:LI20:2445:TMITS'}
           units: []
    descriptions: []
      fieldnames: {'primary'  'samp1'  'samp2'  'samp3'  }
          values: [1x1 struct]

tableResponse.values
ans =
       primary: [0.5 0.6 0.7 0.8 0.9 1.0]
         samp1: [0.103910382 0.103910383 0.103910384 0.103910385 0.103910386 0.103910387 ]
         samp2: [0.9549588893 0.9549588892 0.9549588891 0.9549588880 0.9549588889 0.9549588888]
         samp3: [0.0000000012 0.0000000013 0.0000000014 0.0000000015 0.0000000016 0.0000000017]
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

import java.util.HashMap;
import java.util.Map;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.*;
import static edu.stanford.slac.aida.client.AidaType.*;

public class Example {
    public Short getTrigStatusShort() throws RPCRequestException {
        return (Short) pvaRequest("TRIG:LI31:109:TACT")
                .with("BEAM", 1)
                .returning(AIDA_SHORT)
                .get();
    }

    public Long getTrigStatusLong() throws RPCRequestException {
        return (Long) pvaRequest("TRIG:LI31:109:TACT")
                .with("BEAM", 1)
                .returning(AIDA_LONG)
                .get();
    }

    public String getTrigStatusString() throws RPCRequestException {
        return (String) pvaRequest("TRIG:LI31:109:TACT")
                .with("BEAM", 1)
                .returning(AIDA_STRING)
                .get();
    }

    public PvaTable setTrigStatusStatus(Integer status) throws RPCRequestException {
        return pvaRequest("TRIG:LI31:109:TACT")
                .with("BEAM", 1)
                .set(status);
    }

    public PvaTable turnKnob(Float turnAmount) throws RPCRequestException {
        return pvaRequest("MKB:VAL")
                .with("MKB", "MKB:LI02B_XB.MKB")
                .set(turnAmount);
    }

    public void setBgrp(Boolean value) throws RPCRequestException {
        pvaRequest("BGRP:VAL")
                .with("BGRP", "LCLS")
                .with("VARNAME", "T_CAV")
                .set(value);
    }

    public Boolean getFeedbackStatusBoolean() throws RPCRequestException {
        return (Boolean) pvaGet("FBCK:LI18:28:LOOP_STAT", AIDA_BOOLEAN);
    }

    public Short getFeedbackStatusShort() throws RPCRequestException {
        return (Short) pvaGet("FBCK:LI18:28:LOOP_STAT", AIDA_SHORT);
    }

    public Long getFeedbackStatusLong() throws RPCRequestException {
        return (Long) pvaGet("FBCK:LI18:28:LOOP_STAT", AIDA_LONG);
    }

    public String getFeedbackStatusString() throws RPCRequestException {
        return (String) pvaGet("FBCK:LI18:28:LOOP_STAT", AIDA_STRING);
    }

    public void setFeedbackStatus(String status) throws RPCRequestException {
        pvaSet("FBCK:LI18:28:LOOP_STAT", status);
    }

    public PvaTable getCorrelatedPlot() throws RPCRequestException {
        Map<String, Object> primStepVar = new HashMap<String, Object>();
        primStepVar.put("name", "KLYS:LI31:31:PDES");
        primStepVar.put("low", 0.5);
        primStepVar.put("high", 1.0);
        primStepVar.put("steps", 6);

        return (PvaTable) pvaRequest("CORRPLOT:SCAN")
                .with("FILE", "FACET_BPMS.BTN")
                .with("PRIMSTV", primStepVar)
                .get();
    }
}
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">**PvaClient**</td>

<td class="markdownTableBodyNone">

```java
import org.epics.nt.NTURI;
import org.epics.pvaClient.PvaClient;
import org.epics.pvaClient.PvaClientChannel;
import org.epics.pvaccess.client.rpc.RPCClientImpl;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.pv.*;

public class Example {
    public Short getTrigStatusShort() throws RPCRequestException {
        String pvName = "TRIG:LI31:109:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("type")
                .addQueryInt("beam")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("SHORT");
        uri.getQuery().getIntField("beam").put(1);

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(uri.getPVStructure());

        PVShort field = response.getSubField(PVShort.class, "value");
        return field.get();
    }

    public Long getTrigStatusLong() throws RPCRequestException {
        String pvName = "TRIG:LI31:109:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("type")
                .addQueryInt("beam")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("LONG");
        uri.getQuery().getIntField("beam").put(1);

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(uri.getPVStructure());

        PVLong field = response.getSubField(PVLong.class, "value");
        return field.get();
    }

    public String getTrigStatusString() throws RPCRequestException {
        String pvName = "TRIG:LI31:109:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("type")
                .addQueryInt("beam")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("STRING");
        uri.getQuery().getIntField("beam").put(1);

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(uri.getPVStructure());

        PVString field = response.getSubField(PVString.class, "value");
        return field.get();
    }

    public PVStructure setTrigStatusStatus(Integer status) throws RPCRequestException {
        String pvName = "TRIG:LI31:109:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("value")
                .addQueryInt("beam")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("value").put(status);
        uri.getQuery().getIntField("beam").put(1);

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        return channel.rpc(uri.getPVStructure());
    }

    public PVStructure turnKnob(Float turnAmount) throws RPCRequestException {
        String pvName = "MKB:VAL";

        NTURI uri = NTURI.createBuilder()
                .addQueryDouble("value")
                .addQueryString("mkb")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getDoubleField("value").put(turnAmount);
        uri.getQuery().getStringField("mkb").put("MKB:LI02B_XB.MKB");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        return channel.rpc(uri.getPVStructure());
    }

    public void setBgrp(Boolean value) throws RPCRequestException {
        String pvName = "BGRP:VAL";

        NTURI uri = NTURI.createBuilder()
                .addQueryDouble("value")
                .addQueryString("bgrp")
                .addQueryString("varname")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("value").put(value.toString());
        uri.getQuery().getStringField("bgrp").put("LCLS");
        uri.getQuery().getStringField("varname").put("T_CAV");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        channel.rpc(uri.getPVStructure());
    }

    public Boolean getFeedbackStatusBoolean() throws RPCRequestException {
        String pvName = "FBCK:LI18:28:LOOP_STAT";

        NTURI uri = NTURI.createBuilder().addQueryString("type").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("BOOLEAN");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(uri.getPVStructure());

        PVBoolean field = response.getSubField(PVBoolean.class, "value");
        return field.get();
    }

    public Short getFeedbackStatusShort() throws RPCRequestException {
        String pvName = "FBCK:LI18:28:LOOP_STAT";

        NTURI uri = NTURI.createBuilder().addQueryString("type").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("SHORT");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(uri.getPVStructure());

        PVShort field = response.getSubField(PVShort.class, "value");
        return field.get();
    }

    public Long getFeedbackStatusLong() throws RPCRequestException {
        String pvName = "FBCK:LI18:28:LOOP_STAT";

        NTURI uri = NTURI.createBuilder().addQueryString("type").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("LONG");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(uri.getPVStructure());

        PVLong field = response.getSubField(PVLong.class, "value");
        return field.get();
    }

    public String getFeedbackStatusString() throws RPCRequestException {
        String pvName = "FBCK:LI18:28:LOOP_STAT";

        NTURI uri = NTURI.createBuilder().addQueryString("type").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("STRING");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(uri.getPVStructure());

        PVString field = response.getSubField(PVString.class, "value");
        return field.get();
    }

    public void setFeedbackStatus(String status) throws RPCRequestException {
        String pvName = "FBCK:LI18:28:LOOP_STAT";

        NTURI uri = NTURI.createBuilder().addQueryString("value").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("value").put(status);

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        channel.rpc(uri.getPVStructure());
    }

    public PVStructure getCorrelatedPlot() throws RPCRequestException {
        String pvName = "CORRPLOT:SCAN";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("file")
                .addQueryString("primstv")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("file").put("FACET_BPMS.BTN");
        uri.getQuery().getStringField("primstv").put("{\"name\": \"KLYS:LI31:31:PDES\", \"low\": 0.5, \"high\": 1.0, \"steps\": 6}");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 30.0);
        client.destroy();

        return response;
    }
}
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">**PVAccess**</td>

<td class="markdownTableBodyNone">

```java
import org.epics.nt.NTURI;
import org.epics.pvaccess.client.rpc.RPCClientImpl;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.pv.*;

public class Example {
    public Short getTrigStatusShort() throws RPCRequestException {
        String pvName = "TRIG:LI31:109:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("type")
                .addQueryInt("beam")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("SHORT");
        uri.getQuery().getIntField("beam").put(1);

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        PVShort field = response.getSubField(PVShort.class, "value");
        return field.get();
    }

    public Long getTrigStatusLong() throws RPCRequestException {
        String pvName = "TRIG:LI31:109:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("type")
                .addQueryInt("beam")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("LONG");
        uri.getQuery().getIntField("beam").put(1);

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        PVLong field = response.getSubField(PVLong.class, "value");
        return field.get();
    }

    public String getTrigStatusString() throws RPCRequestException {
        String pvName = "TRIG:LI31:109:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("type")
                .addQueryInt("beam")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("STRING");
        uri.getQuery().getIntField("beam").put(1);

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        PVString field = response.getSubField(PVString.class, "value");
        return field.get();
    }

    public PVStructure setTrigStatusStatus(Integer status) throws RPCRequestException {
        String pvName = "TRIG:LI31:109:TACT";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("value")
                .addQueryInt("beam")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("value").put(status);
        uri.getQuery().getIntField("beam").put(1);

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 30.0);
        client.destroy();

        return response;
    }

    public PVStructure turnKnob(Float turnAmount) throws RPCRequestException {
        String pvName = "MKB:VAL";

        NTURI uri = NTURI.createBuilder()
                .addQueryDouble("value")
                .addQueryString("mkb")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getDoubleField("value").put(turnAmount);
        uri.getQuery().getStringField("mkb").put("MKB:LI02B_XB.MKB");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 30.0);
        client.destroy();

        return response;
    }

    public void setBgrp(Boolean value) throws RPCRequestException {
        String pvName = "BGRP:VAL";

        NTURI uri = NTURI.createBuilder()
                .addQueryDouble("value")
                .addQueryString("bgrp")
                .addQueryString("varname")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("value").put(value.toString());
        uri.getQuery().getStringField("bgrp").put("LCLS");
        uri.getQuery().getStringField("varname").put("T_CAV");

        RPCClientImpl client = new RPCClientImpl(pvName);
        client.request(uri.getPVStructure(), 3.0);
        client.destroy();
    }

    public Boolean getFeedbackStatusBoolean() throws RPCRequestException {
        String pvName = "FBCK:LI18:28:LOOP_STAT";

        NTURI uri = NTURI.createBuilder().addQueryString("type").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("BOOLEAN");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        PVBoolean field = response.getSubField(PVBoolean.class, "value");
        return field.get();
    }

    public Short getFeedbackStatusShort() throws RPCRequestException {
        String pvName = "FBCK:LI18:28:LOOP_STAT";

        NTURI uri = NTURI.createBuilder().addQueryString("type").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("SHORT");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        PVShort field = response.getSubField(PVShort.class, "value");
        return field.get();
    }

    public Long getFeedbackStatusLong() throws RPCRequestException {
        String pvName = "FBCK:LI18:28:LOOP_STAT";

        NTURI uri = NTURI.createBuilder().addQueryString("type").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("LONG");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        PVLong field = response.getSubField(PVLong.class, "value");
        return field.get();
    }

    public String getFeedbackStatusString() throws RPCRequestException {
        String pvName = "FBCK:LI18:28:LOOP_STAT";

        NTURI uri = NTURI.createBuilder().addQueryString("type").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("STRING");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        PVString field = response.getSubField(PVString.class, "value");
        return field.get();
    }

    public void setFeedbackStatus(String status) throws RPCRequestException {
        String pvName = "FBCK:LI18:28:LOOP_STAT";

        NTURI uri = NTURI.createBuilder().addQueryString("value").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("value").put(status);

        RPCClientImpl client = new RPCClientImpl(pvName);
        client.request(uri.getPVStructure(), 3.0);
        client.destroy();
    }

    public PVStructure getCorrelatedPlot() throws RPCRequestException {
        String pvName = "CORRPLOT:SCAN";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("file")
                .addQueryString("primstv")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("file").put("FACET_BPMS.BTN");
        uri.getQuery().getStringField("primstv").put("{\"name\": \"KLYS:LI31:31:PDES\", \"low\": 0.5, \"high\": 1.0, \"steps\": 6}");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 30.0);
        client.destroy();

        return response;
    }
}
```

</td>
</tr>

</table>

