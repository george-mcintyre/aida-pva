# 1.2 - SLC Database Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Database data through AIDA-PVA. SLC
Database provides data from the database values in the SLC Control System. It also allows the setting of float scalar
database values. For general information on using AIDA-PVA see [Basic Users Guide to Aida](1_00_User_Guide.md), and the
EPICS javadoc.

## Summary

Acquires data for the named SLC device from the SLC database. Also allows setting of float scalar database values

## Instances and Attributes

| operation | info     | instance & attribute                  |
|-----------|----------|---------------------------------------|
| **get**   | Syntax   | `<prim>:<micr>:<unit>:<secn>`         |
|           |          | `ASTS:<micr>:<channel>:<pseudo-secn>` |
|           | Examples | `XCOR:LI03:120:LEFF`                  | 
|           |          | `ASTS:PR02:VP3012:DATA`               |
| **set**   | Syntax   | `<prim>:<micr>:<unit>:<secn>`         |
|           | Examples | `XCOR:LI31:41:BCON`                   |

## Attribute operation summary

| Attribute | Description                                                                                                                                            |
|-----------|--------------------------------------------------------------------------------------------------------------------------------------------------------|
| `<secn>`  | Gets SLC db device data for a named device - the channel name                                                                                          |
| `<secn>`  | Sets the value of a float or integer scalar secondary in the SLC database. <br />The `VALUE` argument is a float or an integer with the desired value. |

## Attribute operations

### <secn>  : get

_Parameters_

| Parameter Names | Parameter Values | Description                                                                            |
|-----------------|------------------|----------------------------------------------------------------------------------------|
| `TYPE`*         |                  | Determines the return type of the request                                              |
|                 | `BOOLEAN`        | return a boolean value                                                                 |
|                 | `BYTE`           | return a byte value                                                                    |
|                 | `SHORT`          | return a short value                                                                   |
|                 | `INTEGER`        | return a integer value                                                                 |
|                 | `LONG`           | return a long value                                                                    |
|                 | `FLOAT`          | return a float value                                                                   |
|                 | `DOUBLE`         | return a double value                                                                  |
|                 | `STRING`         | return a string value                                                                  |
|                 | `BOOLEAN_ARRAY`  | return an array of boolean values                                                      |
|                 | `BYTE_ARRAY`     | return an array of byte values                                                         |
|                 | `SHORT_ARRAY`    | return an array of short values                                                        |
|                 | `INTEGER_ARRAY`  | return an array of integer values                                                      |
|                 | `LONG_ARRAY`     | return an array of long values                                                         |
|                 | `FLOAT_ARRAY`    | return an array of float values                                                        |
|                 | `DOUBLE_ARRAY`   | return an array of double values                                                       |
|                 | `STRING_ARRAY`   | return an array of string values                                                       |
|                 | `TABLE`          | return an table corresponding to the <br />required accompanying `TABLE_TYPE` argument |
| `TABLE_TYPE`    |                  | return table with one column and one <br />or more rows with the type specified        |
|                 | `BOOLEAN_ARRAY`  | table contains a single boolean value                                                  |
|                 | `BYTE_ARRAY`     | table contains a single byte value                                                     |
|                 | `SHORT_ARRAY`    | table contains a single short value                                                    |
|                 | `INTEGER_ARRAY`  | table contains a single integer value                                                  |
|                 | `LONG_ARRAY`     | table contains a single long value                                                     |
|                 | `FLOAT_ARRAY`    | table contains a single float value                                                    |
|                 | `DOUBLE_ARRAY`   | table contains a single double value                                                   |
|                 | `STRING_ARRAY`   | table contains a single string value                                                   |

_Return value_

| TYPE                  | Return Column | Column Type                                             | Description                                    |
|-----------------------|---------------|---------------------------------------------------------|------------------------------------------------|
| any scalar type       |               |                                                         | the scalar value                               |
| any scalar array type |               |                                                         | the scalar array                               |
| `TABLE`               | `value`       | depends on the accompanying <br />`TABLE_TYPE` argument | the scalar_array with <br />one or more values |

@warning For the following Attributes `ACON`, `KPHR`, `PCON` and `PDES` you need to prefix the channel name with `SLC::`
to disambiguate it from the channels provided by other Channel Providers. e.g. `SLC::KLYS:LI31:31:PDES`
as this same channel name is supported by the Klystron Provider for setting values.

### <secn>  : set

_Parameters_

| Parameter Names | Parameter Values                | Description                                                                                                                                                                                                | 
|-----------------|---------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `VALUE`*        |                                 | float or integer to set secondary                                                                                                                                                                          |
|                 | `<floating point number>`       | value to use to set secondary                                                                                                                                                                              |
|                 | `<floating point number array>` | value to use to set secondary                                                                                                                                                                              |
|                 | `<integer>`                     | value to use to set secondary                                                                                                                                                                              |
|                 | `<integer array>`               | value to use to set secondary                                                                                                                                                                              |
| `VALUE_TYPE`    |                                 | Determines type to set in secondary                                                                                                                                                                        |
|                 | `FLOAT_ARRAY`                   | Default: value is cast as a float or float array and used to set secondary                                                                                                                                 |
|                 | `INTEGER_ARRAY`                 | value is cast as an integer or integer array and used to set secondary.  If the value or any of the values in the values array cannot be converted to an integer then the function will throw an exception |

_Return value_

None

## Examples

### Commandline Examples

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">example type</th><th class="markdownTableHeadNone">action</th><th class="markdownTableHeadNone">example</th></tr>
<tr class="markdownTableRowOdd">
<td rowspan=2 class="markdownTableBodyNone">**pvcall**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```shell
pvcall "XCOR:LI03:120:LEFF" TYPE=FLOAT
0.262
pvcall "SLC::KLYS:LI31:31:PDES" TYPE=FLOAT
90
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Set</td>
<td class="markdownTableBodyNone">

```shell
pvcall "XCOR:LI31:41:BCON" VALUE=5.0

pvcall "FBCK:LI11:26:HSTA" VALUE=268468385 VALUE_TYPE=INTEGER_ARRAY
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td rowspan=2 class="markdownTableBodyNone">**eget**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```shell
eget -s XCOR:LI03:120:LEFF -a TYPE 'FLOAT'
0.262
eget -s SLC::KLYS:LI31:31:PDES -a TYPE 'FLOAT'
90
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">Set</td>
<td class="markdownTableBodyNone">

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
    public Float getFloat() throws RPCException {
        return pvaGet("XCOR:LI03:120:LEFF", FLOAT);
    }

    public Float getAnotherFloat() throws RPCException {
        return pvaRequest("XCOR:LI03:120:LEFF")
                .returning(FLOAT)
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
    public void setFloat(Float value) throws RPCException {
        pvaSet("XCOR:LI31:41:BCON", value);
    }

    public void setAnInteger(Integer value) throws RPCException {
        pvaRequest("FBCK:LI11:26:HSTA")
                .with("VALUE_TYPE", "INTEGER_ARRAY")
                .set(value);
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

        PVStructure request = dataCreate.createPVStructure(uriStructure);
        request.getStringField("scheme").put("pva");
        request.getStringField("path").put(pvName);

        PVStructure args = request.getStringField("query");
        args.getStringField("type").put("FLOAT");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(request);

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
import org.epics.pvaClient.*;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.factory.FieldFactory;
import org.epics.pvdata.factory.PVDataFactory;
import org.epics.pvdata.pv.*;

public class PvaClientExample {
    public void setFloat(Float value) throws RPCRequestException {
        String pvName = "XCOR:LI31:41:BCON";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"value"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvFloat)
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
        args.getFloatField("value").put(value);

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

    public Float getFloat() throws RuntimeException {
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

        PVStructure request = dataCreate.createPVStructure(uriStructure);
        request.getStringField("scheme").put("pva");
        request.getStringField("path").put(pvName);

        PVStructure args = request.getStringField("query");
        args.getStringField("type").put("FLOAT");

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

        PVStructure response = easyrpc.request(nturi_pvs);
        if (!easypva.getStatus().isOK()) {
            throw new RuntimeException("Unable to get data");
        }

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
import org.epics.pvaccess.*;
import org.epics.pvaccess.easyPVA.*;
import org.epics.pvdata.*;
import org.epics.pvdata.pv.PVStructure;

import java.lang.String;

public class EzExample {
    public static final String NTURI_ID = "epics:nt/NTURI:1.0";
    private final static FieldCreate fieldCreate = factory.FieldFactory.getFieldCreate();
    private final static PVDataCreate dataCreate = factory.PVDataFactory.getPVDataCreate();

    public void setFloat(Float value) throws RuntimeException {
        String pvName = "XCOR:LI31:41:BCON";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"value"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvFloat)
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
        args.getFloatField("value").put(value);

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

        PVStructure request = dataCreate.createPVStructure(uriStructure);
        request.getStringField("scheme").put("pva");
        request.getStringField("path").put(pvName);

        PVStructure args = request.getStringField("query");
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
    public void setFloat(Float value) throws RPCRequestException {
        String pvName = "XCOR:LI31:41:BCON";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"value"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvFloat)
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
        args.getFloatField("value").put(value);

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

Tests are implemented in: SlcTest.java

```shell
java -cp aida-pva-tests.jar  "edu.stanford.slac.aida.test.SlcTest" -c
#################################################
AIDA-PVA SLC TESTS
#################################################

████ Test 1: Acquire scalar types SLC PMUS
_________________________________________________

get: XCOR:LI03:120:LEFF => FLOAT
    Float BACT: 0.262 ✔
get: XCOR:LI03:120//LEFF => FLOAT
    Backwards compatibility - using legacy channel format: Float BACT: 0.262 ✔
get: XCOR:LI03:120:LEFF => DOUBLE
    Double BACT: 0.2619999945163727 ✔
get: XCOR:LI03:120:VSTA => BOOLEAN
    Boolean VSTA: true ✔
get: XCOR:LI03:120:SETL => SHORT
    Short SETL: 3000 ✔
get: XCOR:LI03:120:ADCP => BYTE
    Byte ADCP: 48 ✔
get: XCOR:LI03:120:ADCP => CHAR
    Char ADCP: '0' ✔
get: XCOR:LI03:120:ADCP => CHAR
    WCHAR not possible in AIDA-PVA: Falling back to CHAR: ADCP: '0' ✔
get: XCOR:LI03:120:PSNM => STRING
    String PSNM: UNKNOWN  ✔
get: XCOR:LI03:120:PSNM => STRING
    WSTRING not possible in AIDA-PVA: Falling back to STRING: PSNM: UNKNOWN  ✔
get: XCOR:LI03:120:PSNM => BYTE_ARRAY
    Byte array PSNM:
        85 ✔
        78 ✔
        75 ✔
        78 ✔
        79 ✔
        87 ✔
        78 ✔
        32 ✔
get: XCOR:LI03:120:PSNM => CHAR_ARRAY
    Char array PSNM:
        'U' ✔
        'N' ✔
        'K' ✔
        'N' ✔
        'O' ✔
        'W' ✔
        'N' ✔
        ' ' ✔

████ Test 2: Acquire FLOAT type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:BACT => FLOAT
    BACT: 0.0 ✔

████ Test 3: Acquire LONG INT type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:VSTA => LONG
    VSTA: 8256 ✔

████ Test 4: Acquire array of FLOAT type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:IMMS => FLOAT_ARRAY
    IMMS:
        -5.0 ✔
        5.0 ✔
        0.0 ✔

████ Test 5: Acquire array of DOUBLE type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:IMMS => DOUBLE_ARRAY
    IMMS:
        -5.0 ✔
        5.0 ✔
        0.0 ✔

████ Test 6: Acquire array of SHORT type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:NSCY => SHORT_ARRAY
    NSCY:
        0 ✔
        0 ✔

████ Test 7: Acquire array of LONG type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:RAMP => LONG_ARRAY
    RAMP:
        0 ✔
        0 ✔

████ Test 8: Acquire array of BOOLEAN type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:RAMP => BOOLEAN_ARRAY
    RAMP:
        true ✔
        false ✔

████ Test 9: Acquire FLOAT type SLC Database PV
_________________________________________________

get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔

████ Test 10: Looping acquire FLOAT type SLC Database PV pausing 10 seconds between each.
_________________________________________________

get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔

████ Test 11: ASTS channel name test
_________________________________________________

get: ASTS:LI00:ISOPLAN2:DATA => FLOAT
    DATA: 0.0 ✔
get: ASTS:PR02:VP3012:DATA => FLOAT
    DATA: 0.0 ✔
get: ASTS:PR02:VP3012:STAT => SHORT
    STAT: 40 ✔
get: ASTS:PR02:T2092BLW:LIM1 => FLOAT
    LIM1: 32.0 ✔
get: ASTS:PR02:T2092BLW:LIM2 => FLOAT
    LIM2: 150.0 ✔
get: ASTS:PR02:T2092BLW:LIMS => FLOAT_ARRAY
    LIMS:
        32.0 ✔
        150.0 ✔
get: ASTS:PR02:T2092QUA:SCAL => FLOAT_ARRAY
    SCAL:
        32.0 ✔
        1.8 ✔
get: ASTS:PR02:T2092QUA:RAW => FLOAT
    RAW: 1.0E-9 ✔
get: ASTS:PR02:T2092QUA:STAT => STRING
    STAT: CamErr   RED        ✔

████ Test 12: ASTS channel name with one of the pseudo-secondaries STAT, CTRL, or VSTA
_________________________________________________

get: ASTS:PR02:T2092QUA:STAT => STRING_ARRAY
    STAT:
        CamErr   ✔
        RED      ✔
          ✔

████ Test 13: Set value test
_________________________________________________

set: XCOR:LI31:41:BCON (VALUE=[5.0]) ✔
```
