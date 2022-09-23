# 1.8 - SLC Utilities Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Utilities through AIDA-PVA. This data
provider:

- allows the retrieval of the status code or a status string for a specified trigger device on a beam code. It also
  allows the deactivation or reactivation of a specified trigger device on a beam code.
- allows the setting of devices referenced in a specified multiknob file by knob rotation using a specified relative
  delta value. Only a relative (not absolute) multiknob file may be specified,
- allows for the setting of a BGRP variable value
- allows for setting of SCP transverse feedback 

see [Basic Users Guide to Aida](1_00_User_Guide.md), and the EPICS javadoc.

## Summary

Supports **get** and **set** operations.

- The **get** operation obtains the status code or a status string for a specified trigger device on a beam code.
- The **get** operation obtains the status of the specified SCP transverse feedback.
- A **set** operation deactivates or reactivates a specified trigger device on a beam code
- A  **set** operation sets the devices referenced in a specified multiknob file by knob rotation using a specified delta value.
- A **set** operation sets the value of a specified variable for a specified BGRP.
- A **set** operation sets the SCP transverse feedback to OFF, COMPUTE, or FEEDBACK.

## Instances and Attributes

| operation | info     | instance & attribute             |
|-----------|----------|----------------------------------|
| **get**   | Syntax   | `<prim>:<micr>:<unit>:TACT`      |
|           | Examples | `TRIG:LI31:109:TACT`             |
| **set**   | Syntax   | `<prim>:<micr>:<unit>:TACT`      |
|           | Examples | `TRIG:LI31:109:TACT`             |
| **set**   | Syntax   | `MKB:VAL `                       |
|           | Examples | `MKB:VAL`                        |
| **set**   | Syntax   | `BGRP:VAL `                      |
|           | Examples | `BGRP:VAL`                       |
| **get**   | Syntax   | `<prim>:<micr>:<unit>:LOOP_STAT` |
|           | Examples | `FBCK:LI18:28:LOOP_STAT`         |
| **set**   | Syntax   | `<prim>:<micr>:<unit>:LOOP_STAT` |
|           | Examples | `FBCK:LI18:28:LOOP_STAT`         |

## Attribute operation summary

| Attribute   | operation | Description                                                                                |
|-------------|-----------|--------------------------------------------------------------------------------------------|
| `TACT`      | **get**   | Gets a status code or a status string for the specified trigger device on a beam code      |
| `TACT`      | **set**   | Deactivates or reactivates a specified trigger device on a beam code                       |
| `VAL`       | **set**   | Sets devices referenced in a specified multiknob file, which is a required parameter       |
| `VAL`       | **set**   | Sets a BGRP variable to a new value                                                        |
| `LOOP_STAT` | **get**   | Gets a code or a string that describes the status of the specified SCP transverse feedback |
| `LOOP_STAT` | **set**   | Sets the specified SCP transverse feedback to off, compute, or feedback                    |

## Attribute operations

### TACT : get

_Parameters_

| Parameter Names | Parameter Values | Description                               | 
|-----------------|------------------|-------------------------------------------|
| `TYPE`*         |                  | Determines the return type of the request |
|                 | `SHORT`          | return a short value.                     |
|                 | `LONG`           | return a short value.                     |
|                 | `STRING`         | return a string value.                    |
| `BEAM`*         | Integer          | Beam code number                          |

_Return value_

| TYPE     | Description                                                                                                                |
|----------|----------------------------------------------------------------------------------------------------------------------------|
| `SHORT`  | A short value containing the status code for the trigger device on a beam code: <br />`0` => deactivated, `1` => activated |
| `LONG`   | A long value containing the status code for the trigger device on a beam code: <br />`0` => deactivated, `1` => activated  |
| `STRING` | A string value containing a status string having one of two values: "deactivated" or "activated"                           |

### TACT : set

_Parameters_

| Parameter Names | Parameter Values | Description                                                                             | 
|-----------------|------------------|-----------------------------------------------------------------------------------------|
| `VALUE`*        | Short            | flag code indicating the desired operation: <br />`0` => deactivate, `1` => reactivate. |
| `BEAM`*         | Integer          | Beam code number                                                                        |

_Return value_

| TYPE    | Return Column | Column Type   | Description                                                                                         |
|---------|---------------|---------------|-----------------------------------------------------------------------------------------------------|
| `TABLE` | `value`       | `SHORT_ARRAY` | status code for the specified trigger device on a beam code:<br /> 0 => deactivated, 1 => activated |

### MKB:VAL : set

_Parameters_

| Parameter Names | Parameter Values | Description                                                                                                                                                            | 
|-----------------|------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `VALUE`*        | Float            | relative delta knob rotation value.                                                                                                                                    |
| `MKB`           | String           | multiknob filename. The filename path specification <br />(including the prefix "mkb:", if file is in the standard <br />multiknob file directory) of a multiknob file |

_Return value_

| TYPE    | Return Column | Column Type    | Description                                                    |
|---------|---------------|----------------|----------------------------------------------------------------|
| `TABLE` | `name`        | `STRING_ARRAY` | name of each device referenced in the specified multiknob file | 
|         | `value`       | `DOUBLE_ARRAY` | value of each device after the knob rotation set operation     |

### BGRP:VAL : set

_Parameters_

| Parameter Names | Parameter Values | Description                                       | 
|-----------------|------------------|---------------------------------------------------|
| `VALUE`*        | Boolean          | specifying the new variable value "`N`" or "`Y`". |
| `BGRP`*         | String           | The BGRP name                                     |
| `VARNAME`*      | String           | A BGRP variable name for the specified BGRP..     |

_Return value_

None

### LOOP_STAT : get

_Parameters_

| Parameter Names | Parameter Values | Description                               | 
|-----------------|------------------|-------------------------------------------|
| `TYPE`*         |                  | Determines the return type of the request |
|                 | `BOOLEAN`        | return a boolean value.                   |
|                 | `SHORT`          | return a short value.                     |
|                 | `LONG`           | return a short value.                     |
|                 | `STRING`         | return a string value.                    |

_Return value_

| TYPE      | Description                                                                                                                                                 |
|-----------|-------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `BOOLEAN` | A boolean value containing the SCP transverse feedback status code for the loop device: <br />`true` => in feedback state, `false` => not in feedback state |
| `SHORT`   | A short value containing the SCP transverse feedback status code for the loop device: <br />`0` => off, `1` => compute, `2` => feedback                     |
| `LONG`    | A long value containing the SCP transverse feedback status code for the loop: <br />`0` => off, `1` => compute, `2` => feedback                             |
| `STRING`  | A string value containing a SCP transverse feedback status string having one of three values: "off", "compute" or "feedback"                                |

### LOOP_STAT : set

_Parameters_

| Parameter Names | Parameter Values | Description                                                                                                       | 
|-----------------|------------------|-------------------------------------------------------------------------------------------------------------------|
| `VALUE`*        | String           | flag string indicating the desired state: <br />`"off"` => off, `"compute"` => compute, `"feedback"` => feedback. |

_Return value_

None

## Examples

### Commandline Examples

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">example type</th><th class="markdownTableHeadNone">action</th><th class="markdownTableHeadNone">example</th></tr>
<tr class="markdownTableRowOdd">
<td rowspan=2 class="markdownTableBodyNone">commandline **pvcall**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```shell
pvcall "TRIG:LI31:109:TACT" BEAM=1 TYPE=SHORT
pvcall "TRIG:LI31:109:TACT" BEAM=1 TYPE=LONG
pvcall "TRIG:LI31:109:TACT" BEAM=1 TYPE=STRING
pvcall "FBCK:LI18:28:LOOP_STAT" TYPE=BOOLEAN
pvcall "FBCK:LI18:28:LOOP_STAT" TYPE=SHORT
pvcall "FBCK:LI18:28:LOOP_STAT" TYPE=LONG
pvcall "FBCK:LI18:28:LOOP_STAT" TYPE=STRING
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Set</td>
<td class="markdownTableBodyNone">

```shell
pvcall "TRIG:LI31:109:TACT" BEAM=1 VALUE=0
pvcall "MKB:VAL" MKB='mkb:li02b_xb.mkb' VALUE=1.0 
pvcall "BGRP:VAL" BGRP=LCLS VARNAME=T_CAV VALUE='Yes'
pvcall "FBCK:LI18:28:LOOP_STAT" VALUE='feedback'
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td rowspan=2 class="markdownTableBodyNone">commandline **eget**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```shell
eget -s TRIG:LI31:109:TACT -a BEAM 1 -a TYPE SHORT 
eget -s TRIG:LI31:109:TACT -a BEAM 1 -a TYPE LONG 
eget -s TRIG:LI31:109:TACT -a BEAM 1 -a TYPE STRING 
eget -s FBCK:LI18:28:LOOP_STAT -a BEAM 1 -a TYPE BOOLEAN 
eget -s FBCK:LI18:28:LOOP_STAT -a BEAM 1 -a TYPE SHORT 
eget -s FBCK:LI18:28:LOOP_STAT -a BEAM 1 -a TYPE LONG 
eget -s FBCK:LI18:28:LOOP_STAT -a BEAM 1 -a TYPE STRING 
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Set</td>
<td class="markdownTableBodyNone">

```shell
eget -s TRIG:LI31:109:TACT -a BEAM 1 VALUE 0
eget -s MKB:VAL -a MKB 'mkb:li02b_xb.mkb' -a VALUE 1.0 
eget -s BGRP:VAL -a BGRP LCLS -a VARNAME T_CAV -a VALUE=Y
eget -s FBCK:LI18:28:LOOP_STAT 1 VALUE compute
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

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Set</td>
<td class="markdownTableBodyNone">

```matlab
try
    builder = pvaRequest('TRIG:LI31:109:TACT');
    builder.with('BEAM', 1);
    trigResponse = builder.set(0)
catch e
    handleExceptions(e);
end
```

```matlab
try
    builder = pvaRequest('MKB:VAL');
    builder.with('MKB', 'mkb:li02b_xb.mkb');
    mkbResponse = builder.set(1.0)
catch e
    handleExceptions(e);
end
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
    pvaSet('FBCK:LI18:28:LOOP_STAT', 'compute');
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
    public void getValues() throws RPCException {
        Short shortValue = pvaRequest("TRIG:LI31:109:TACT").with("BEAM", 1).returning(SHORT).get();
        Long longValue = pvaRequest("TRIG:LI31:109:TACT").with("BEAM", 1).returning(LONG).get();
        String stringValue = pvaRequest("TRIG:LI31:109:TACT").with("BEAM", 1).returning(STRING).get();
        Boolean fbckBooleanValue = pvaRequest("FBCK:LI18:28:LOOP_STAT").returning(BOOLEAN).get();
        Short fbckShortValue = pvaRequest("FBCK:LI18:28:LOOP_STAT").returning(SHORT).get();
        Long fbckLongValue = pvaRequest("FBCK:LI18:28:LOOP_STAT").returning(LONG).get();
        String fbckStringValue = pvaRequest("FBCK:LI18:28:LOOP_STAT").returning(STRING).get();
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
    private static String FBCK_OFF = "OFF";
    private static String FBCK_COMPUTE = "COMPUTE";
    private static String FBCK_FEEDBACK = "FEEDBACK";
    
    public void setValues() throws RPCException {
        AidaTable tableValue = pvaRequest("TRIG:LI31:109:TACT").with("BEAM", 1).set(0);
        Map<String, List<Object>> values = tableValue.getValues();
        Short status = values.get("status").get(0);

        tableValue = pvaRequest("MKB:VAL").with("MKB", 1).set('mkb:li02b_xb.mkb');
        values = tableValue.getValues();
        List<String> names = values.get("name");
        List<Float> values = values.get("value");

        pvaRequest("BGRP:VAL").with("BGRP", "LCLS").with("VARNAME", "T_CAV").set(TRUE);
        pvaSet("FBCK:LI18:28:LOOP_STAT", FBCK_FEEDBACK);
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
    public void getValues() throws RPCRequestException {
        String pvName = "TRIG:LI31:109:TACT";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"beam", "type"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvInt),
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
        args.getIntField("beam").put(1);
        args.getStringField("type").put("SHORT");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);

        args.getStringField("type").put("SHORT");
        PVStructure response = channel.rpc(request);
        PVShort shortField = response.getSubField(PVInt.class, "value");
        Short shortValue = shortField.get();

        args.getStringField("type").put("LONG");
        response = channel.rpc(request);
        PVLong longField = response.getSubField(PVLong.class, "value");
        Long longValue = longField.get();

        args.getStringField("type").put("STRING");
        response = channel.rpc(request);
        PVString stringField = response.getSubField(PVString.class, "value");
        String stringValue = stringField.get();
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
    public void setValues() throws RPCRequestException {
        String trigPvName = "TRIG:LI31:109:TACT";
        String mkbPvName = "MKB:VAL";
        String bgrpPvName = "BGRP:VAL";

        Structure trigArguments = fieldCreate.createStructure(
                new String[]{"beam", "value"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvInt),
                        fieldCreate.createScalar(ScalarType.pvShort)
                });

        Structure mkbArguments = fieldCreate.createStructure(
                new String[]{"mkb", "value"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvInt),
                        fieldCreate.createScalar(ScalarType.pvString)
                });

        Structure bgrpArguments = fieldCreate.createStructure(
                new String[]{"bgrp", "varname", "value",},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvBoolean)
                });

        PvaClient client;
        PvaClientChannel channel;
        PVStructure args, request, response, values;
        Structure uriStructure;

        // TRIG 
        uriStructure = fieldCreate.createStructure(
                new String[]{"path", "scheme", "query"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvString),
                        trigArguments
                });

        request = dataCreate.createPVStructure(uriStructure);
        request.getStringField("scheme").put("pva");
        request.getStringField("path").put(trigPvName);

        args = request.getStringField("query");
        args.getIntField("beam").put(1);
        args.getShortField("value").put(0);

        client = PvaClient.get("pva");
        channel = client.createChannel(trigPvName);
        response = channel.rpc(request);
        Short shortStatus = response
                .getSubField(PVStructure.class, "value")    // The values
                .getSubField(PVShortArray.class, "status")  // The status vector
                .get()                                      // As a PvArray
                .getShort(0);                               // First entry

        // MKB
        uriStructure = fieldCreate.createStructure(
                new String[]{"path", "scheme", "query"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvString),
                        mkbArguments
                });

        request = dataCreate.createPVStructure(uriStructure);
        request.getStringField("scheme").put("pva");
        request.getStringField("path").put(mkbPvName);

        args = request.getStringField("query");
        args.getIntField("mkb").put(1);
        args.getStringField("value").put("mkb:li02b_xb.mkb");

        client = PvaClient.get("pva");
        channel = client.createChannel(mkbPvName);
        response = channel.rpc(request);
        values = response.getSubField(PVStructure.class, "value");    // The values
        PVStringArray stringNames = values.getSubField(PVStringArray.class, "name");
        PVFloatArray floatValues = values.getSubField(PVStringArray.class, "value");

        // BGRP
        uriStructure = fieldCreate.createStructure(
                new String[]{"path", "scheme", "query"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvString),
                        bgrpArguments
                });

        request = dataCreate.createPVStructure(uriStructure);
        request.getStringField("scheme").put("pva");
        request.getStringField("path").put(bgrpPvName);

        args = request.getStringField("query");
        args.getStringField("bgrp").put("LCLS");
        args.getStringField("varname").put("T_CAV");
        args.getBooleanField("value").put(true);

        client = PvaClient.get("pva");
        channel = client.createChannel(bgrpPvName);
        channel.rpc(request);

    }
}
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td rowspan=2 class="markdownTableBodyNone">java **EasyPVA**</td>
<td class="markdownTableBodyNone">Get/Set</td>

<td class="markdownTableBodyNone">

- Same as PvaClient example but change this code block: 

```java
        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(request);
```

- For this code block:

```java
        EasyChannel channel = easypva.createChannel(pvName);
        if (!channel.connect(5.0)){
            throw new RuntimeException("Unable to connect");
        }

        EasyRPC easyrpc=channel.createRPC();
        if(!easypva.getStatus().isOK()){
            throw new RuntimeException("Unable to create RPC channel");
        }

        if(!easyrpc.connect()){
            throw new RuntimeException("Unable to connect to RPC channel");
        }

        PVStructure response=easyrpc.request(request);
        if(!easypva.getStatus().isOK()){
            throw new RuntimeException("Unable to get data");
        }
```

</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Get/Set</td>
<td class="markdownTableBodyNone">

- Same as PvaClient example but change this code block: 

```java
        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(request);
```

- For this code block:

```java
        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(request, 3.0);
        client.destroy();
```

</td>
</tr>

</table>

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

