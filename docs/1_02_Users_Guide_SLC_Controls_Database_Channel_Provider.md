# 1.2 - SLC Database Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Database data through AIDA-PVA. SLC
Database provides data from the database values in the SLC Control System. It also allows the setting of float scalar
database values. For general information on using AIDA-PVA see [Basic Users Guide to Aida](1_00_User_Guide.md), and the
EPICS javadoc.

## Summary

Acquires data for the named SLC device from the SLC database. Also allows setting of float and integer, scalar and array, database values

## Synopsis

|                          | PV                                                 | Arguments               | Description        |
|:-------------------------|----------------------------------------------------|-------------------------|:-------------------|
| [1.2.1](@ref section121) | [ `SLC::` ] `<primary>:<micro>:<unit>:<secondary>` | `TYPE` [ `TABLE_TYPE` ] | Get Database value |
| [1.2.2](@ref section122) | [ `SLC::` ] `<primary>:<micro>:<unit>:<secondary>` | `VALUE`                 | Set Database value |

## 1.2.1. Get SLC DB Device Data  {#section121}

- `<primary>:<micro>:<unit>:<secondary>` 
  - `TYPE`* : _type_ 
  - `TABLE_TYPE` : _table type_
- `SLC::<primary>:<micro>:<unit>:<secondary>`  
  - `TYPE`* : _type_ 
  - `TABLE_TYPE` : _table type_

Gets SLC db device data for a device identified by the given **primary**, **micro**, **unit** and **secondary**   

### Arguments

| Argument Names | Argument Values | Description                                                                           |
|----------------|-----------------|---------------------------------------------------------------------------------------|
| `TYPE`*        | Return type     | _string_: <br/>Determines the return type of the request                                             |
|                | `BOOLEAN`       | return a boolean value                                                                |
|                | `BYTE`          | return a byte value                                                                   |
|                | `SHORT`         | return a short value                                                                  |
|                | `INTEGER`       | return a integer value                                                                |
|                | `LONG`          | return a long value                                                                   |
|                | `FLOAT`         | return a float value                                                                  |
|                | `DOUBLE`        | return a double value                                                                 |
|                | `STRING`        | return a string value                                                                 |
|                | `BOOLEAN_ARRAY` | return an array of boolean values                                                     |
|                | `BYTE_ARRAY`    | return an array of byte values                                                        |
|                | `SHORT_ARRAY`   | return an array of short values                                                       |
|                | `INTEGER_ARRAY` | return an array of integer values                                                     |
|                | `LONG_ARRAY`    | return an array of long values                                                        |
|                | `FLOAT_ARRAY`   | return an array of float values                                                       |
|                | `DOUBLE_ARRAY`  | return an array of double values                                                      |
|                | `STRING_ARRAY`  | return an array of string values                                                      |
|                | `TABLE`         | return a table corresponding to the <br />required accompanying `TABLE_TYPE` argument |
| `TABLE_TYPE`   | Table type      | return table with one column and one <br />or more rows with the type specified       |
|                | `BOOLEAN_ARRAY` | table contains a single boolean value                                                 |
|                | `BYTE_ARRAY`    | table contains a single byte value                                                    |
|                | `SHORT_ARRAY`   | table contains a single short value                                                   |
|                | `INTEGER_ARRAY` | table contains a single integer value                                                 |
|                | `LONG_ARRAY`    | table contains a single long value                                                    |
|                | `FLOAT_ARRAY`   | table contains a single float value                                                   |
|                | `DOUBLE_ARRAY`  | table contains a single double value                                                  |
|                | `STRING_ARRAY`  | table contains a single string value                                                  |

### Return value

| TYPE                  | Return Column | Column Type                                             | Description                                    |
|-----------------------|---------------|---------------------------------------------------------|------------------------------------------------|
| any scalar type       |               |                                                         | the scalar value                               |
| any scalar array type |               |                                                         | the scalar array                               |
| `TABLE`               | `value`       | depends on the accompanying <br />`TABLE_TYPE` argument | the scalar_array with <br />one or more values |

### Examples

| operation                                   | Response                        |
|---------------------------------------------|---------------------------------|
| `XCOR:LI03:120:LEFF`                        | `0.262`                         | 
| `SLC::KLYS:LI31:31:PDES TYPE=INTEGER`       | `90`                            |
| `XCOR:LI03:120:LEFF TYPE=DOUBLE`            | `0.2619999945163727`            |
| `XCOR:LI03:120:VSTA TYPE=BOOLEAN`           | `true`                          |
| `XCOR:LI03:120:SETL TYPE=SHORT`             | `3000`                          |
| `XCOR:LI03:120:ADCP TYPE=BYTE`              | `48`                            |
| `XCOR:LI03:120:ADCP TYPE=CHAR`              | `0`                             |
| `XCOR:LI03:120:PSNM TYPE=BYTE_ARRAY`        | `[85,78,75,78,79,87,78]`        |
| `XCOR:LI03:120:PSNM TYPE=CHAR_ARRAY`        | `["U","N","K","N","O","W","N"]` |
| `XCOR:LI03:120:VSTA TYPE=LONG`              | `8256`                          |
| `XCOR:LI03:120:IMMS TYPE=FLOAT_ARRAY`       | `[-5.0,5.0,0.0]`                |
| `XCOR:LI03:120:IMMS TYPE=DOUBLE_ARRAY`      | `[-5.0,5.0,0.0]`                |
| `XCOR:LI03:120:NSCY TYPE=SHORT_ARRAY`       | `[0,0]`                         |
| `XCOR:LI03:120:RAMP TYPE=LONG_ARRAY`        | `[0,0]`                         |
| `XCOR:LI03:120:RAMP TYPE=BOOLEAN_ARRAY`     | `[true,false]`                  |
| `ASTS:PR02:T2092QUA:STAT TYPE=STRING_ARRAY` | `["CamErr","fCamErr",""]`       |

## 1.2.2. Set SLC DB Device Data  {#section122}

- `<primary>:<micro>:<unit>:<secondary>` 
  - `VALUE`* : _scalar_
  - `VALUE_TYPE` : _value type_
- `<primary>:<micro>:<unit>:<secondary>` 
  - `VALUE`* :  _scalar array_
  - `VALUE_TYPE` : _value type_
- `SLC::<primary>:<micro>:<unit>:<secondary>` 
  - `VALUE`* : _scalar_
  - `VALUE_TYPE` : _value type_
- `SLC::<primary>:<micro>:<unit>:<secondary>` 
  - `VALUE`* :  _scalar array_
  - `VALUE_TYPE` : _value type_

Sets the value of a float or integer scalar in the SLC database or sets the values of float or integer arrays in the SLC database.  The `VALUE` argument is a float, integer, float array, or integer array, with the desired value. 

### Arguments

| Argument Names | Argument Values                 | Description                                                                                                                                                                                                                            | 
|----------------|---------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `VALUE`*       |                                 | float(s) or integer(s) to set in secondary                                                                                                                                                                                             |
|                | `<floating point number>`       | value to use to set secondary                                                                                                                                                                                                          |
|                | `<floating point number array>` | values to use to set secondary                                                                                                                                                                                                         |
|                | `<integer>`                     | value to use to set secondary                                                                                                                                                                                                          |
|                | `<integer array>`               | values to use to set secondary                                                                                                                                                                                                         |
| `VALUE_TYPE`   |                                 | Determines type to set in secondary                                                                                                                                                                                                    |
|                | `FLOAT_ARRAY`                   | Default: value is cast as a float or float array  <br />and used to set secondary                                                                                                                                                      |
|                | `INTEGER_ARRAY`                 | value is cast as an integer or integer array and  <br />used to set secondary.  If the value or any of  <br />the values in the values array cannot be  <br />converted to an integer then the function will  <br />throw an exception |

### Return value

None

### Examples

| operation                                                      | Response |
|----------------------------------------------------------------|----------|
| `XCOR:LI31:41:BCON VALUE=5.0`                                  |          | 
| `XCOR:LI31:41:BCON VALUE=[5.0]`                                |          | 
| `FBCK:LI11:26:HSTA VALUE=268468385 VALUE_TYPE=INTEGER_ARRAY`   |          | 
| `FBCK:LI11:26:HSTA VALUE=[268468385] VALUE_TYPE=INTEGER_ARRAY` |          | 

## CLI and Code Examples
### Commandline Examples

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">example type</th><th class="markdownTableHeadNone">example</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">**pvcall**</td>

<td class="markdownTableBodyNone">

```shell
pvcall "XCOR:LI03:120:LEFF" TYPE=FLOAT
0.262
pvcall "SLC::KLYS:LI31:31:PDES" TYPE=FLOAT
90
```


```shell
pvcall "XCOR:LI31:41:BCON" VALUE=5.0

pvcall "FBCK:LI11:26:HSTA" VALUE=268468385 VALUE_TYPE=INTEGER_ARRAY
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">**eget**</td>
<td class="markdownTableBodyNone">

```shell
eget -s XCOR:LI03:120:LEFF -a TYPE 'FLOAT'
0.262
eget -s SLC::KLYS:LI31:31:PDES -a TYPE 'FLOAT'
90
```

```shell
eget -s XCOR:LI31:41:BCON -a VALUE 5.0

eget -s FBCK:LI11:26:HSTA -a VALUE 268468385 -s VALUE_TYPE INTEGER_ARRAY
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
    floatResponse = pvaGet('SLC::KLYS:LI31:31:PDES', AIDA_FLOAT)
catch e
    handleExceptions(e);
end
floatResponse =
    90
```

```matlab
try
    floatArrayResponse = pvaGetM('SLC::KLYS:LI31:31:PDES', AIDA_FLOAT_ARRAY)
catch e
    handleExceptions(e);
end
floatArrayResponse =
    90
```

```matlab
try
    requestBuilder = pvaRequest('XCOR:LI03:120:LEFF');
    requestBuilder = requestBuilder.returning(AIDA_FLOAT);
    floatResponse = requestBuilder.get()
catch e
    handleExceptions(e);
end
floatResponse =
    0.2620
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Set</td>
<td class="markdownTableBodyNone">

```matlab
try
    pvaSet('XCOR:LI31:41:BCON', 5.0)    
catch e
    handleExceptions(e);
end
```

```matlab
try
    requestBuilder = pvaRequest('FBCK:LI11:26:HSTA');
    requestBuilder.with("VALUE_TYPE", "INTEGER_ARRAY");
    requestBuilder.set(268468385)
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
import org.epics.pvaccess.server.rpc.RPCRequestException;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.*;
import static edu.stanford.slac.aida.client.AidaType.AIDA_DOUBLE;

public class Example {
    public Double getDouble() throws RPCRequestException {
        return (Double) pvaGet("XCOR:LI03:120:LEFF", AIDA_DOUBLE);
    }

    public void setDouble(Double value) throws RPCRequestException {
        pvaSet("XCOR:LI31:41:BCON", value);
    }

    public void setInteger(Integer value) throws RPCRequestException {
        pvaRequest("FBCK:LI11:26:HSTA")
                .with("VALUE_TYPE", "INTEGER_ARRAY")
                .set(value);
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
import org.epics.pvdata.pv.PVDouble;
import org.epics.pvdata.pv.PVStructure;

public class Example {
    public Double getDouble() {
        String pvName = "XCOR:LI03:120:LEFF";

        NTURI uri = NTURI.createBuilder().addQueryString("type").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("DOUBLE");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(uri.getPVStructure());

        PVDouble field = response.getSubField(PVDouble.class, "value");
        return field.get();
    }

    public void setDouble(Double value) {
        String pvName = "XCOR:LI31:41:BCON";
        NTURI uri = NTURI.createBuilder().addQueryDouble("value").create();
        uri.getPath().put(pvName);
        uri.getQuery().getDoubleField("value").put(value);
        
        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        channel.rpc(uri.getPVStructure());
    }
    
    public void setInteger(Integer value) throws RPCRequestException {
        String pvName = "XCOR:LI31:41:BCON";
  
        NTURI uri = NTURI.createBuilder()
                .addQueryDouble("value")
                .addQueryString("value_type")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getDoubleField("value").put(value);
        uri.getQuery().getStringField("value").put("INTEGER_ARRAY");
  
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
import org.epics.pvdata.pv.PVDouble;
import org.epics.pvdata.pv.PVStructure;

public class Example {
    public Double getDouble() throws RPCRequestException {
        String pvName = "XCOR:LI03:120:LEFF";

        NTURI uri = NTURI.createBuilder().addQueryString("type").create();
        uri.getPath().put(pvName);
        uri.getQuery().getStringField("type").put("DOUBLE");

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();

        PVDouble field = response.getSubField(PVDouble.class, "value");
        return field.get();
    }

    public void setDouble(Double value) throws RPCRequestException {
        String pvName = "XCOR:LI31:41:BCON";

        NTURI uri = NTURI.createBuilder().addQueryDouble("value").create();
        uri.getPath().put(pvName);
        uri.getQuery().getDoubleField("value").put(value);

        RPCClientImpl client = new RPCClientImpl(pvName);
        client.request(uri.getPVStructure(), 3.0);
        client.destroy();
    }
    
    public void setInteger(Integer value) throws RPCRequestException {
        String pvName = "XCOR:LI31:41:BCON";
  
        NTURI uri = NTURI.createBuilder()
                .addQueryDouble("value")
                .addQueryString("value_type")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getDoubleField("value").put(value);
        uri.getQuery().getStringField("value").put("INTEGER_ARRAY");
  
        RPCClientImpl client = new RPCClientImpl(pvName);
        client.request(uri.getPVStructure(), 3.0);
        client.destroy();
    }
}
```

</td>
</tr>
</table>
