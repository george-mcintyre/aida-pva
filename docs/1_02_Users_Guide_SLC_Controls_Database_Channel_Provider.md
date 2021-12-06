# 1.2 - SLC Database Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Database data through AIDA-PVA. SLC
Database provides data from the database values in the SLC Control System. It also allows the setting of float scalar
database values. For general information on using AIDA-PVA see [Basic Users Guide to Aida](1_00_User_Guide.md), and the
EPICS javadoc.

## Summary

Acquires data for the named SLC device from the SLC database. Also allows setting of float scalar database values

## Instances and Attributes

|         |          |                                       |
|---------|----------|---------------------------------------|
| **get** | Syntax   | `<prim>:<micr>:<unit>:<secn>`         |
|         |          | `ASTS:<micr>:<channel>:<pseudo-secn>` |
|         | Examples | `XCOR:LI03:120:LEFF`                  | 
|         |          | `ASTS:PR02:VP3012:DATA`               |
| **set** | Syntax   | `<prim>:<micr>:<unit>:<secn>`         |
|         | Examples | `XCOR:LI31:41:BCON`                   |

## Attribute operation summary

|           |                                                                                                                               |
|-----------|-------------------------------------------------------------------------------------------------------------------------------|
| Attribute | Description                                                                                                                   |
| `<secn>`  | Gets SLC db device data for a named device - the channel name                                                                 |
| `<secn>`  | Sets the value of a float scalar secondary in the SLC database. <br />The `VALUE` argument is a float with the desired value. |

## Attribute operations

### <secn>  : get

_Parameters_

|                 |                  |                                                                                        |
|-----------------|------------------|----------------------------------------------------------------------------------------|
| Parameter Names | Parameter Values | Description                                                                            |
|                 |                  |                                                                                        |
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

|                       |               |                                                         |                                                |
|-----------------------|---------------|---------------------------------------------------------|------------------------------------------------|
| TYPE                  | Return Column | Column Type                                             | Description                                    |
| any scalar type       |               |                                                         | the scalar value                               |
| any scalar array type |               |                                                         | the scalar array                               |
| `TABLE`               | `value`       | depends on the accompanying <br />`TABLE_TYPE` argument | the scalar_array with <br />one or more values |

### <secn>  : set

_Parameters_

|                 |                           |                        |
|-----------------|---------------------------|------------------------|
| Parameter Names | Parameter Values          | Description            | 
| `VALUE`*        | `<floating point number>` | float to set secondary |

_Return value_

None

## Examples

### Command line examples

|                 |                                          |
|-----------------|------------------------------------------|
| pvcall examples | `pvcall "XCOR:LI03:120:LEFF" TYPE=FLOAT` |
|                 | `pvcall "XCOR:LI31:41:BCON" VALUE=5.0`   |
| eget examples   |                                          |

### Java examples

#### aida-pva-client

```java
import org.epics.pvaccess.server.rpc.RPCRequestException;
import static edu.stanford.slac.aida.client.AidaPvaClientUtils.*;
import static edu.stanford.slac.aida.client.AidaType.*;

public class AidaPvaClientExample {
    public Float getFloat() throws RPCException {
        return request("XCOR:LI03:120:LEFF")
                .returning(FLOAT)
                .get();
    }
}
```

#### EasyPVA

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

        PVStructure nturi = dataCreate.createPVStructure(uriStructure);
        nturi.getStringField("scheme").put("pva");
        nturi.getStringField("path").put(pvName);

        PVStructure args = nturi.getStringField("query");
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

#### pvaClient
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

        PVStructure nturi = dataCreate.createPVStructure(uriStructure);
        nturi.getStringField("scheme").put("pva");
        nturi.getStringField("path").put(pvName);

        PVStructure args = nturi.getStringField("query");
        args.getStringField("type").put("FLOAT");

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(nturi);

        PVFloat field = response.getSubField(PVFloat.class, "value");
        return field.get();
    }
}
```

#### Plain PvAccess

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

### Matlab examples
Matlab includes the following utility functions:
- `void` **aidainit**() - to initialise access to the AIDA framework
- `void` **aidaget**(`aidaName`, `aidaType`, `aidaParams`) - gets SCALAR or SCALAR_ARRAY from EPICS/AIDA-PVA data providers
- `NTURI` **nturi**(`pvName`, `varargin`) - to create an `NTURI` Structure (see [Normative Types](2_2_Normative_Types.md)) for use with EPICS/AIDA-PVA data providers
- `matlab_structure` **nttable2struct** - to convert from NTTables to matlab structures
- `PVStructure` **ezrpc**(`nturi`) - takes an `NTURI` and executes it using EasyPva
- `PVStructure` **pvarpc**(`nturi`) - takes an `NTURI` and executes it using PvaClient
- `matlab_dynamic_type` **request**(`pvName`) - takes a `pvName` and executes a **get()** or **set()** request with builder pattern 
  - **with**(`name`, `value`) - specifies a parameter for the request  
  - **returning**(`aidaType`) - specified the aida type to return from the request
  - **get**() - executes the get request
  - **set**(`value`) - executes the set request with the given value

These have all been updated/added to be able to interact with the new AIDA-PVA framework.

#### aida-pva-client
```matlab
    aidainit
    try
        floatResponse = request('XCOR:LI03:120:LEFF').returning(FLOAT).get();
    catch ME
       % do something when errors occur or just show ME.identifier
    end
```
#### using aidaget
```matlab
    aidainit
    floatResponse = aidaget('XCOR:LI03:120:LEFF','FLOAT')
```
#### EasyPVA
```matlab
    aidainit
    response = ezrpc(nturi('XCOR:LI03:120:LEFF', 'type', 'FLOAT'))    
    floatResponse = response.getSubField(PVFloat.class, "value")
```
#### PvaClient
```matlab
    aidainit
    response = pvarpc(nturi('XCOR:LI03:120:LEFF', 'type', 'FLOAT'))    
    floatResponse = response.getSubField(PVFloat.class, "value")
```
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
