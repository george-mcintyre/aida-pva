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

| operation       | info        | instance & attribute        |
|---------|----------|---------------------------------|
| **get** | Syntax   | `<dgrp-mnemonic>:<prim>:<secn>` |
|         | Examples | `DEV_DGRP:XCOR:BDES`            |
| **set** | Syntax   | `MAGNETSET:<secn>`              |
|         | Examples | `MAGNETSET:BDES`                |
|         |          | `MAGNETSET:BCON`                |

## Attribute operation summary

| Attribute       | operation | Description                                                                                                                                                                              |
|-----------------|-----------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `<prim>:<secn>` | **get**   | Returns a table containing the name of each  magnet<br /> for specified display group and primary, and secondary<br /> values                                                            |
| `BDES`          | **set**   | Sets specified `BDES` secondary values and returns<br /> a table  containing a string indicating the state<br /> of each specified magnet device, and `BACT` value<br /> for each device |
| `VDES`          | **set**   | Sets specified `VDES` secondary values and returns<br /> a table  containing a string indicating the state<br /> of each specified magnet device, and `VACT` value<br /> for each device |
| `BCON`          | **set**   | Sets specified `BCON` secondary values                                                                                                                                                   |

## Attribute operations

### <prim>:<secn> : get

_Parameters_

| Parameter Names | Parameter Values            | Description                                                                                 | 
|-----------------|-----------------------------|---------------------------------------------------------------------------------------------|
| `MICROS`        | `<start_micro>-<end_micro>` | This specifies a range of micro names of <br />interest within the specified display group  |
| `UNITS`         | `<start_unit>-<end_unit>`   | This specified a range of unit numbers of <br />interest within the specified display group |

_Return value_

| TYPE    | Return Column | Column Type    | Description                                                                                                                |
|---------|---------------|----------------|----------------------------------------------------------------------------------------------------------------------------|
| `TABLE` | `name`        | `STRING_ARRAY` | name of each magnet for specified display group <br />and primary (optionally filtered by specified micro and unit ranges) |
|         | `secondary`       | `FLOAT_ARRAY`  | secondary values (BDES or VDES)                                                                                            |

### BDES : set, VDES : set

_Parameters_

| Parameter Names | Parameter Values |Description | 
| ----------- | -----------| ----------- |
| `VALUE`*  | structure: {names[], values[]}  | an array of magnet names and an array of corresponding<br /> set values  |
| `MAGFUNC`*  |   `TRIM`, `PTRB`, or `NOFUNC`  | Specifies whether a trim or perturb operation will<br /> be performed. If `NOFUNC`, neither a trim nor a perturb<br /> operation will be performed  |
| `LIMITCHECK`  |  `ALL` or `SOME`  | Used to determine behavior when the set value  for<br /> one or more devices is outside of its low/high  limits.<br /> If this parameter is set to `ALL`, the  entire request<br /> will fail resulting in an  exception being thrown<br /> and no `BDES`/`VDES` values  being set for any of<br /> the request devices. If this parameter is set to<br /> `SOME`, the set  value action will succeed for those<br /> set values  that are within limits and will not be<br /> performed  for those set values outside their limits<br />  (the state string return values for these devices<br />  will be set to the string "Outside Limits"). The<br /> default setting of this parameter is `ALL`.  |

_Return value_

| TYPE  | Return Column | Column Type |Description |
|----------- | ----------- | -----------  |-----------  |
| `TABLE` | `state` | `STRING_ARRAY` | string indicating the state of each specified magnet<br /> device. Set to blanks (checking that the first character<br /> is blank  is all that is necessary) if the state is<br /> good. Set to the string "Outside Limits" if a set<br /> value is outside of its  limits and the optional parameter<br /> LIMITCHECK is set to SOME |
|  |  `value`| `FLOAT_ARRAY`|  `BACT`/`VACT` value for each device |

### BCON : set

_Parameters_

| Parameter Names | Parameter Values               | Description                                                       | 
|-----------------|--------------------------------|-------------------------------------------------------------------|
| `VALUE`*        | structure: {names[], values[]} | an array of magnet names and an array of corresponding set values |

_Return value_

None

## Examples

@note For general details about accessing AIDA-PVA from matlab see [Matlab Coding](1_12_Matlab_Code.md)

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">example type</th><th class="markdownTableHeadNone">action</th><th class="markdownTableHeadNone">example</th></tr>
<tr class="markdownTableRowOdd">
<td rowspan=2 class="markdownTableBodyNone">commandline **pvcall**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```shell
pvcall "DEV_DGRP:XCOR:BDES" MICROS="LI31-LI31" UNITS="1-100"
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Set</td>
<td class="markdownTableBodyNone">

```shell
pvcall "MAGNETSET:BDES" MAGFUNC=TRIM VALUE='{"names": [ "XCOR:LI31:41"], "values": [ 4.0 ] }'
pvcall "MAGNETSET:BCON" VALUE='{"names": [ "XCOR:LI31:41"], "values": [ 5.0 ] }'
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td rowspan=2 class="markdownTableBodyNone">commandline **eget**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```shell
eget -s DEV_DGRP:XCOR:BDES -a MICROS 'LI31-LI31' -a UNITS '1-100'
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Set</td>
<td class="markdownTableBodyNone">

```shell
eget -s MAGNETSET:BDES -a MAGFUNC TRIM -a VALUE '{"names": [ "XCOR:LI31:41"], "values": [ 4.0 ] }'
eget -s MAGNETSET:BCON -a VALUE '{"names": [ "XCOR:LI31:41"], "values": [ 5.0 ] }'
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td rowspan=2 class="markdownTableBodyNone">java **AidaPvaClient**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```java
import org.epics.pvaccess.server.rpc.RPCRequestException;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.*;
import static edu.stanford.slac.aida.client.AidaType.*;

public class AidaPvaClientExample {
    public AidaTable getTable() throws RPCException {
        return pvaRequest("DEV_DGRP:XCOR:BDES")
                .with("MICROS", "LI31-LI31")
                .with("UNITS", "1-100")
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
    public AidaTable getTable() throws RPCException {
        return pvaRequest("MAGNETSET:BDES")
                .with("MAGFUNC", "TRIM")
                .set(Map.of(
                        "names", List.of("XCOR:LI31:41"),
                        "values", List.of(4.0f)
                ));
    }
}
```

```java
import org.epics.pvaccess.server.rpc.RPCRequestException;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.*;
import static edu.stanford.slac.aida.client.AidaType.*;

public class AidaPvaClientExample {
    public void getTable() throws RPCException {
        pvaRequest("MAGNETSET:BCON")
                .set(Map.of(
                        "names", List.of("XCOR:LI31:41"),
                        "values", List.of(5.0f)
                ));
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
    public PVStructure getTable() throws RPCRequestException {
        String pvName = "DEV_DGRP:XCOR:BDES";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"micros", "units"},
                new Field[]{
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
        args.getStringField("micros").put("LI31-LI31");
        args.getStringField("units").put("1-100");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);

        return channel.rpc(request);
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
    public PVStructure setTable() throws RPCRequestException {
        String pvName = "MAGNETSET:BDES";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"magfunc", "value"},
                new Field[]{
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
        args.getStringField("magfunc").put("TRIM");
        args.getStringField("value").put("{\"names\": [ \"XCOR:LI31:41\"], \"values\": [ 5.0 ] }");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        return channel.rpc(request);
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

    public PVStructure getTable() throws RPCRequestException {
        String pvName = "DEV_DGRP:XCOR:BDES";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"micros", "units"},
                new Field[]{
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
        args.getStringField("micros").put("LI31-LI31");
        args.getStringField("units").put("1-100");

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

        return response;
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

    public PVStructure setTable() throws RPCRequestException {
        String pvName = "MAGNETSET:BDES";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"magfunc", "value"},
                new Field[]{
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
        args.getStringField("magfunc").put("TRIM");
        args.getStringField("value").put("{\"names\": [ \"XCOR:LI31:41\"], \"values\": [ 5.0 ] }");

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

        return response;
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
    public Float getFloat() throws RPCRequestException {
        String pvName = "XCOR:LI03:120:LEFF";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"type"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvString)
                });

        Structure uriStructure = fieldCreate.createStructure(
                new String[]{"path", "scheme", "query"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvString),
                        arguments
                });

        PVStructure nturi = dataCreate.createPVStructure(uriStructure);
        nturi.getStringField("scheme").put("pva");
        nturi.getStringField("path").put(pvName);

        PVStructure args = nturi.getStringField("query");
        args.getStringField("type").put("FLOAT");

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
    public PVStructure setTable() throws RPCRequestException {
        String pvName = "MAGNETSET:BDES";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"magfunc", "value"},
                new Field[]{
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
        args.getStringField("magfunc").put("TRIM");
        args.getStringField("value").put("{\"names\": [ \"XCOR:LI31:41\"], \"values\": [ 5.0 ] }");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(request, 3.0);
        client.destroy();
        return response;
    }
}
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td rowspan=2 class="markdownTableBodyNone">matlab **AidaPvaClient**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```matlab
try
    table = pvaRequest('DEV_DGRP:XCOR:BDES').with('MICROS', 'LI31-LI31').with('UNITS', '1-100').get();
    labels = table.getLabels();
    values = table.getValues();
    names = values.get('name'); 
    values = values.get('value'); 
catch e
    handleExceptions(e);
end
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Set</td>
<td class="markdownTableBodyNone">

```matlab
try
    table = pvaRequest('MAGNETSET:BDES').with('MAGFUNC', 'TRIM').set('{"names": [ "XCOR:LI31:41"], "values": [ 4.0 ] }');
    labels = table.getLabels();
    values = table.getValues();
    states = values.get('state'); 
    values = values.get('value'); 
catch e
    handleExceptions(e);
end
```

```matlab
try
    pvaSet('MAGNETSET:BCON').set('{"names": [ "XCOR:LI31:41"], "values": [ 5.0 ] }');
catch e
    handleExceptions(e);
end
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td rowspan=2 class="markdownTableBodyNone">matlab **PvaClient**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```matlab
tableStruct = nttable2struct(pvarpc(nturi('DEV_DGRP:XCOR:BDES', 'micros', 'LI31-LI31', 'units', '1-100')));
labels = tableStruct.labels;
values = table.values;
names = values.name; 
values = values.value; 
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Set</td>
<td class="markdownTableBodyNone">

```matlab
tableStruct = nttable2struct(pvarpc(nturi('MAGNETSET:BDES', 'magfunc', 'trim', 'value', '{"names": [ "XCOR:LI31:41"], "values": [ 4.0 ] }')));
labels = tableStruct.labels;
values = table.values;
states = values.state; 
values = values.value; 
```

```matlab
pvarpc(nturi('MAGNETSET:BCON', 'value', '{"names": [ "XCOR:LI31:41"], "values": [ 5.0 ] }'));
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td rowspan=2 class="markdownTableBodyNone">matlab **EasyPVA**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```matlab
tableStruct = nttable2struct(ezrpc(nturi('DEV_DGRP:XCOR:BDES', 'micros', 'LI31-LI31', 'units', '1-100')));
labels = tableStruct.labels;
values = table.values;
names = values.name; 
values = values.value; 
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Set</td>
<td class="markdownTableBodyNone">

```matlab
tableStruct = nttable2struct(ezrpc(nturi('MAGNETSET:BDES', 'magfunc', 'trim', 'value', '{"names": [ "XCOR:LI31:41"], "values": [ 4.0 ] }')));
labels = tableStruct.labels;
values = table.values;
states = values.state; 
values = values.value; 
```

```matlab
ezrpc(nturi('MAGNETSET:BCON', 'value', '{"names": [ "XCOR:LI31:41"], "values": [ 5.0 ] }'));
```

</td>
</tr>

</table>

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

