# 1.6 - SLC Magnet Data Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Magnet Data through AIDA-PVA. This data
provider allows the retrieval of magnet primary/micro/unit names and secondary values for a specified
display group, primary, and secondary. It also allows the setting of `BDES`/`VDES` values for specified magnet
primary/micro/unit names and optionally performs either a trim or perturb operation. This set operation returns the
state names and new `BACT`/`VACT` values. Finally, this data provider allows for the setting of `BCON` values for
specified magnet primary/micro/unit names. For general information on using AIDA-PVA
see [Basic Users Guide to Aida](1_00_User_Guide.md), and the EPICS javadoc.

## Summary

- Obtains primary/micro/unit strings and secondary values.
- Sets `BDES`/`VDES`/`BCON` values and optionally trim.

## Synopsis

|                          | PV                                      | Arguments                          | Description          |
|:-------------------------|-----------------------------------------|------------------------------------|:---------------------|
| [1.6.1](@ref section161) | `<dgrp-mnemonic>:<primary>:<secondary>` | [ `MICROS` `UNITS` ]               | Get secondary value  |
| [1.6.2](@ref section162) | `MAGNETSET:BDES`                        | `VALUE` `MAGFUNC` [ `LIMITCHECK` ] | Set BDES value       |
|                          | `MAGNETSET:VDES`                        | `VALUE` `MAGFUNC` [ `LIMITCHECK` ] | Set VDES value       |
| [1.6.3](@ref section163) | `MAGNETSET:BCON`                        | `VALUE`                            | Set BCON value       |

## 1.6.1. Get Secondary Value {#section161}

- `<display-group-mnemonic><primary>:<secondary>`
    - `MICROS` : _range-of-micros_
    - `UNITS` : _range-of-units_

Obtains primary/micro/unit strings and secondary values for a specified display group,
primary, and secondary name, with optional micro and unit range filters.

### Arguments

| Argument Names | Argument Values             | Description                                                                                 |
|----------------|-----------------------------|---------------------------------------------------------------------------------------------|
| `MICROS`       | `<start_micro>-<end_micro>` | This specifies a range of micro names of <br />interest within the specified display group  |
| `UNITS`        | `<start_unit>-<end_unit>`   | This specified a range of unit numbers of <br />interest within the specified display group |

### Return value

| TYPE    | Return Column | Column Type    | Description                                                                                                                |
|---------|---------------|----------------|----------------------------------------------------------------------------------------------------------------------------|
| `TABLE` | `name`        | `STRING_ARRAY` | name of each magnet for specified display group <br />and primary (optionally filtered by specified micro and unit ranges) |
|         | `secondary`       | `FLOAT_ARRAY`  | secondary values (BDES or VDES)                                                                                            |

### Examples

#### Operations

- `DEV_DGRP:XCOR:BDES MICROS="LI31-LI31" UNITS="1-500"`

#### Response

| Magnet Name   | secondary   |
|---------------|-------------|
|               |             |
| `name`        | `secondary` |
| XCOR:LI31:41  | 5.0         |
| XCOR:LI31:201 | 0.0         |
| XCOR:LI31:301 | 0.0         |
| XCOR:LI31:401 | 0.03        |

## 1.6.2. Set BDES / VDES Value  {#section162}

- `MAGNETSET:BDES`
    - `VALUE`* : _structure_
        - `NAMES`* : _list-of-magnet-names_
        - `VALUES`* : _list-of-values_
    - `MAGFUNC`* : _trim-operation_
        - `TRIM`
        - `PTRB`
        - `NOFUNC`
    - `LIMITCHECK` : _limit-check-behavior_
        - `ALL`
        - `SOME`
- `MAGNETSET:VDES`
    - `VALUE`* : _structure_
        - `NAMES`* : _list-of-magnet-names_
        - `VALUES`* : _list-of-values_
    - `MAGFUNC`* : _trim-operation_
        - `TRIM`
        - `PTRB`
        - `NOFUNC`
    - `LIMITCHECK` : _limit-check-behavior_
        - `ALL`
        - `SOME`

Sets `BDES`/`VDES` values for specified primary/micro/unit name strings to specified values and optionally performs a
specified operation (trim or perturb). This set operation returns the resulting state names and `BACT`/`VACT` values

### Arguments

| Argument Names | Argument Values                     | Description                                                                                                                                                                                                                                                                   |
|----------------|-------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `VALUE`*       | `{ <names-array>, <values-array> }` | a structure containing congruent <br/>arrays for names and values                                                                                                                                                                                                             |
|                | `<names-array>`                     | an array of magnet names                                                                                                                                                                                                                                                      |
|                | `<values-array>`                    | an array of set values                                                                                                                                                                                                                                                        |
| `MAGFUNC`*     | `<trim-operation>`                  | Specifies trim operation                                                                                                                                                                                                                                                      |
|                | `TRIM`                              | Perform a trim operation                                                                                                                                                                                                                                                      |
|                | `PTRB`                              | Perform perturb operation                                                                                                                                                                                                                                                     |
|                | `NOFUNC`                            | Perform neither a trim or perturb operation                                                                                                                                                                                                                                   |
| `LIMITCHECK`   | `<limit-check-behavior>`            | Used to determine behavior when the set value  for<br /> one or more devices is outside of its low/high  limits. The<br /> default setting of this parameter is `ALL`.                                                                                                        |
|                | `ALL`                               | The  entire request will fail resulting in an <br/>exception being thrown and no `BDES`/`VDES` <br/>values  being set for any of the request devices                                                                                                                          |
|                | `SOME`                              | The set  value action will succeed for those<br /> set values  that are within limits and will not be<br /> performed  for those set values outside their limits<br />  (the state string return values for these devices<br />  will be set to the string "Outside Limits"). |

### Return value

| TYPE    | Return Column | Column Type    | Description                                                                                                                                                                                                                                                                                                                            |
|---------|---------------|----------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `TABLE` | `state`       | `STRING_ARRAY` | string indicating the state of each specified magnet<br /> device. Set to blanks (checking that the first character<br /> is blank  is all that is necessary) if the state is<br /> good. Set to the string "Outside Limits" if a set<br /> value is outside of its  limits and the optional parameter<br /> LIMITCHECK is set to SOME |
|         | `value`       | `FLOAT_ARRAY`  | `BACT`/`VACT` value for each device                                                                                                                                                                                                                                                                                                    |

### Examples

#### Operations

- `MAGNETSET:BDES" MAGFUNC=TRIM VALUE='{"names": [ "XCOR:LI31:41"], "values": [ 4.0 ] }`
- `MAGNETSET:VDES" MAGFUNC=TRIM VALUE='{"names": [ "XCOR:LI31:41"], "values": [ 4.0 ] }`

#### Response

| State   | value   |
|---------|---------|
|         |         |
| `state` | `value` |
|         | 5.0     |

## 1.6.3. Set BCON Value  {#section163}

- `MAGNETSET:BCON`
    - `VALUE`* : _names-and-values_

Sets specified `BCON` secondary values

### Arguments

| Argument Names | Argument Values                     | Description                                                       |
|----------------|-------------------------------------|-------------------------------------------------------------------|
| `VALUE`*       | `{ <names-array>, <values-array> }` | a structure containing congruent <br/>arrays for names and values |
|                | `<names-array>`                     | an array of magnet names                                          |
|                | `<values-array>`                    | an array of set values                                            |

### Return value

None

### Examples

| operation                                                                |
|--------------------------------------------------------------------------|
| `MAGNETSET:BCON VALUE='{"names": [ "XCOR:LI31:41"], "values": [ 5.0 ] }` | 

## CLI and Code Examples

### Commandline Examples

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">example type</th><th class="markdownTableHeadNone">example</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">**pvcall**</td>

<td class="markdownTableBodyNone">

```shell
pvcall "DEV_DGRP:XCOR:BDES" MICROS="LI31-LI31" UNITS="1-100"
```

```shell
pvcall "MAGNETSET:BDES" MAGFUNC=TRIM VALUE='{"names": [ "XCOR:LI31:41"], "values": [ 4.0 ] }'
pvcall "MAGNETSET:BCON" VALUE='{"names": [ "XCOR:LI31:41"], "values": [ 5.0 ] }'
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">**eget**</td>

<td class="markdownTableBodyNone">

```shell
eget -s DEV_DGRP:XCOR:BDES -a MICROS 'LI31-LI31' -a UNITS '1-100'
```

```shell
eget -s MAGNETSET:BDES -a MAGFUNC TRIM -a VALUE '{"names": [ "XCOR:LI31:41"], "values": [ 4.0 ] }'
eget -s MAGNETSET:BCON -a VALUE '{"names": [ "XCOR:LI31:41"], "values": [ 5.0 ] }'
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
    builder = pvaRequest('DEV_DGRP:XCOR:BDES');
    builder.with('MICROS', 'LI31-LI31');
    builder.with('UNITS', '1-100');
    table = ML(builder.get());
    labels = table.labels
    names = table.values.name
    values = table.values.secondary
catch e
    handleExceptions(e);
end

labels =
    'name of magnet'    'secondary values'

names =
    'XCOR:LI31:41'

values =
     4
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Set</td>
<td class="markdownTableBodyNone">

```matlab
try
    builder = pvaRequest('MAGNETSET:BDES');
    builder.with('MAGFUNC', 'NOFUNC');
    jstruct = AidaPvaStruct();
    jstruct.put('names', { 'XCOR:LI31:41'});
    jstruct.put('values', { 4.0 } );
    mstruct= ML(builder.set(jstruct))
    mstruct.values.status(1)
    mstruct.values.bact_vact(1)
catch e
    handleExceptions(e);
end

mstruct =
            size: 1
          labels: {'status'  'bact/vact'}
           units: []
    descriptions: []
      fieldnames: {'status'  'bact_vact'}
          values: [1x1 struct]

ans =
    'OUTOFTOL '

ans =
    0.2983          
```

```matlab
try
    builder = pvaRequest('MAGNETSET:BCON');
    jstruct = AidaPvaStruct();
    jstruct.put('names', { 'XCOR:LI31:41'});
    jstruct.put('values', { 5.0 } );
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

import java.util.List;
import java.util.Map;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.pvaRequest;

public class Example {
    public PvaTable getValues() throws RPCRequestException {
        return (PvaTable) pvaRequest("DEV_DGRP:XCOR:BDES")
                .with("MICROS", "LI31-LI31")
                .with("UNITS", "1-100")
                .get();
    }

    public PvaTable setValues(List<String> names, List<Float> values) throws RPCRequestException {
        return (PvaTable) pvaRequest("MAGNETSET:BDES")
                .with("MAGFUNC", "TRIM")
                .set(Map.of("names", names, "values", values));
    }

    public void setBconValues(List<String> names, List<Float> values) throws RPCRequestException {
        pvaRequest("MAGNETSET:BCON").set(Map.of("names", names, "values", values));
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
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.pv.PVStructure;

import java.util.List;

public class Example {
    public PVStructure getValues() throws RPCRequestException {
        String pvName = "DEV_DGRP:XCOR:BDES";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("micros")
                .addQueryString("units")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("micros").put("LI31-LI31");
        uri.getQuery().getStringField("units").put("1-100");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        return channel.rpc(uri.getPVStructure());
    }

    public PVStructure setValues() throws RPCRequestException {
        String pvName = "MAGNETSET:BDES";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("magfunc")
                .addQueryString("value")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("magfunc").put("TRIM");
        uri.getQuery().getStringField("value").put("{\"names\": [ \"XCOR:LI31:41\"], \"values\": [ 4.0 ] }");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        return channel.rpc(uri.getPVStructure());
    }

    public void setBconValues(List<String> names, List<Float> values) throws RPCRequestException {
        String pvName = "MAGNETSET:BCON";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("value")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("value").put("{\"names\": [ \"XCOR:LI31:41\"], \"values\": [ 5.0 ] }");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        channel.rpc(uri.getPVStructure());
    }
}
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">**PvAccess**</td>

<td class="markdownTableBodyNone">

```java
import org.epics.nt.NTURI;
import org.epics.pvaccess.client.rpc.RPCClientImpl;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.pv.PVStructure;

import java.util.List;

public class Example {
    public PVStructure getValues() throws RPCRequestException {
        String pvName = "DEV_DGRP:XCOR:BDES";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("micros")
                .addQueryString("units")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("micros").put("LI31-LI31");
        uri.getQuery().getStringField("units").put("1-100");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();
        return response;
    }

    public PVStructure setValues() throws RPCRequestException {
        String pvName = "MAGNETSET:BDES";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("magfunc")
                .addQueryString("value")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("magfunc").put("TRIM");
        uri.getQuery().getStringField("value").put("{\"names\": [ \"XCOR:LI31:41\"], \"values\": [ 4.0 ] }");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();
        return response;
    }

    public void setBconValues(List<String> names, List<Float> values) throws RPCRequestException {
        String pvName = "MAGNETSET:BCON";

        NTURI uri = NTURI.createBuilder()
                .addQueryString("value")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("value").put("{\"names\": [ \"XCOR:LI31:41\"], \"values\": [ 5.0 ] }");

        RPCClientImpl client = new RPCClientImpl(pvName);
        client.request(uri.getPVStructure(), 3.0);
        client.destroy();
    }
}
```

</td>
</tr>

</table>
