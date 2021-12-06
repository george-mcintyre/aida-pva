# 1.4 - SLC Buffered Data Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Buffered Data through AIDA-PVA. This
data service gives access to the so called "Buffered BPM Data" acquisition facilities of the SLC Control System, in the
SLAC accelerator complex. Buffered BPM data, is the name given to the pulse-by-pulse data acquisition system, in which a
user names the devices whose values they want to be acquired, for how many beam pulses, and the service then returns a
table of results - the x and y position values and the Tmit, given in one row per device, per pulse. For general
information on using AIDA-PVA see [Basic Users Guide to Aida](1_00_User_Guide.md), and the EPICS javadoc.

## Summary

The SLC Buffered Acquisition Channel Provider gets SLC control system pulse synchronized "Buffered Data", for the
following device types:

- BPM
    - x (mm)
    - y (mm)
    - tmit (num particles estimated)
- TORO tmit
- KLYS and SBST phase
- GAPM data

and status information for these, as specified in the user's query. The acquisition is subject to the so-called
measurement definition `BPMD` given, over the given
`NRPOS` parameter sequential pulses.

@note A table is returned. Since the kind of data one gets back from these device types is all a little different, some
elements of the table of data you get back should be ignored.

## Instances and Attributes

|         |          |                                |
|---------|----------|--------------------------------|
| **get** | Syntax   | `<dgrp-mnemonic-name>:BUFFACQ` |
|         | Examples | `NDRFACET:BUFFACQ`             |
|         |          | `INJ_ELEC:BUFFACQ`             |
|         |          | `INJ_POSI:BUFFACQ`             |
|         |          | `TAXXALL:BUFFACQ`              |

## Attribute operation summary

|           |                                                                                                                                                                                                                                                                            |
|-----------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Attribute | Description                                                                                                                                                                                                                                                                |
| `BUFFACQ` | Makes a buffered acquisition reading of each given device, in the dgrp specified <br /> in the INSTANCE part of the query, under the BPM Measurement Definition specified <br /> by the `BPMD` parameter, at each beam crossing for the number of pulses given by `NRPOS`. |

## Attribute operations

### BUFFACQ : get

_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `BPMD`*  | Integer Bpm Measurement Definition number | This specifies the timing profile of the acquisition.<br /> Each `BPMD` corresponds to exactly one DGRP. To find<br /> likely BPMDs, go to the BPM Device panel of a SCP<br /> and hit the 'HELP' button, then select the button<br /> corresponding to the bpm orbit you would have wanted<br /> to see. The button name for that acquisition, as displayed<br /> in the help, is the BPMD for the selected orbit. Use<br /> that number for this parameter. Eg `BPMD=57` is the<br /> primary BPMD for the DGRP `NDRFACET`, and so will<br /> be the main one used for FACET. |
| `NRPOS` | `1 =< n >= 2800` | Default is 1. In theory, this parameter can range up<br />  to `2800`, but some things must be borne in mind.<br />  Firstly, roughly speaking the minimum time it could<br />  take a buffered acquisition to complete, is `NRPOS`/machine-rate-in-Hz.<br />  But, the SCP in fact adds time to this theoretical<br />  minimum heuristically, to give the micros time to<br />  prep the acquisition. Secondly, AIDA-PVA's network<br />  timeout ranges between60 and 90 seconds depending<br />  on platform, so if your acquisition takes too long,<br />  the client will timeout. So, in practice, limit your<br />  NRPOS to numbers that will result in relatively short<br />  acquisitions, <br />plan for about 30 seconds max.<br />  |
| `BPMS` | array of device names | The names of the devices from which a user would like<br /> the Buffered service to acquire data. Device names<br /> for the following devices:  `BPM`, `TORO`, `GAPM`,<br /> `KLYS` or `SBST`. |
| `DEVS` | array of device names | The names of the devices from which a user would like<br /> the Buffered service to acquire data. Device names<br /> for the following devices:  `BPM`, `TORO`, `GAPM`,<br /> `KLYS` or `SBST`. |

_Return value_

@note Return value for BPM device shown below. `TORO`, `GAPM`, `KLYS` or `SBST` will be different.

|         |               |                 |                          |
|---------|---------------|-----------------|--------------------------|
| TYPE    | Return Column | Column Type     | Description              |
| `TABLE` | `name`        | `STRING_ARRAY`  | name of each BPM         |
|         | `pulseId`     | `INTEGER_ARRAY` | pulse ID                 |
|         | `x`           | `FLOAT_ARRAY`   | x offsets (mm)           |
|         | `y`           | `FLOAT_ARRAY`   | y offsets (mm)           |
|         | `tmits`       | `FLOAT_ARRAY`   | tmits (num particles)    |
|         | `stat`        | `INTEGER_ARRAY` | (32 bit field)           |
|         | `goodmeas`    | `BOOLEAN_ARRAY` | true = good, false = bad |

## Examples

|                |                                                                                                                                                 | 
|----------------|-------------------------------------------------------------------------------------------------------------------------------------------------|
| pvcall example | ```pvcall NDRFACET:BUFFACQ BPMD=57 NRPOS=5 DEVS='["KLYS:LI03:31","SBST:LI03:001", "BPMS:LI02:501", "TORO:LI20:2040"]'```                        |
| eget examples   |                                          |

### Java examples

#### aida-pva-client

```java
import org.epics.pvaccess.server.rpc.RPCRequestException;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.*;
import static edu.stanford.slac.aida.client.AidaType.*;

public class AidaPvaClientExample {
    public Map<String, List<Object>> getTable() throws RPCException {
        AidaTable table = request("NDRFACET:BUFFACQ")
                .with("BPMD", 57)
                .with("NRPOS", 5)
                .with("DEVS", List.of("KLYS:LI03:31", "SBST:LI03:001", "BPMS:LI02:501", "TORO:LI20:2040"))
                .get();
        return table.getValues();
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

    public Map<String, List<Object>> getTable() throws RuntimeException {
        String pvName = "NDRFACET:BUFFACQ";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"bpmd", "nrpos", "devs"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvInt),
                        fieldCreate.createScalar(ScalarType.pvInt),
                        fieldCreate.createScalar(ScalarArrayType.pvStringArray)
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
        args.getIntField("bpmd").put(57);
        args.getIntField("nrpos").put(5);
        args.getIntField("devs").put(0, 4, new String[]{"KLYS:LI03:31", "SBST:LI03:001", "BPMS:LI02:501", "TORO:LI20:2040"}, 0);

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

        PVStructure result = response.getSubField(PVFloat.class, "value");

        return Arrays.stream(result.getSubField(PVStructure.class, "value").getPVFields())
                .map(column -> {
                    List<Object> columnValues = new ArrayList<>();
                    arrayIterator(column, columnValues::add);
                    return Pair.of(column.getFieldName(), columnValues);
                })
                .collect(Collectors.toMap(Pair::getLeft, Pair::getRight));
    }

    static <T extends PVField> void arrayIterator(T array, Consumer<Object> consumer) {
        arrayLoop(array, (s, i) -> consumer.accept(s));
    }

    static <T extends PVField> void arrayLoop(T array, BiConsumer<Object, Integer> consumer) {
        if (array instanceof PVBooleanArray) {
            booleanArrayLoop((PVBooleanArray) array, consumer::accept);
        } else if (array instanceof PVIntArray) {
            integerArrayLoop((PVIntArray) array, consumer::accept);
        } else if (array instanceof PVFloatArray) {
            floatArrayLoop((PVFloatArray) array, consumer::accept);
        } else if (array instanceof PVStringArray) {
            stringArrayLoop((PVStringArray) array, consumer::accept);
        }
    }

    static void booleanArrayLoop(PVBooleanArray array, BiConsumer<Boolean, Integer> consumer) {
        int index = 0, offset = 0, len = array.getLength();
        BooleanArrayData data = new BooleanArrayData();
        while (offset < len) {
            int num = array.get(offset, (len - offset), data);
            for (int i = 0; i < num; i++) {
                consumer.accept(data.data[offset + i], index++);
            }
            offset += num;
        }
    }

    static void integerArrayLoop(PVIntArray array, BiConsumer<Integer, Integer> consumer) {
        int index = 0;
        IteratorInteger it = array.get().iterator();
        while (it.hasNext()) {
            consumer.accept(it.nextInt(), index++);
        }
    }

    static void floatArrayLoop(PVFloatArray array, BiConsumer<Float, Integer> consumer) {
        int index = 0;
        IteratorFloat it = array.get().iterator();
        while (it.hasNext()) {
            consumer.accept(it.nextFloat(), index++);
        }
    }

    static void stringArrayLoop(PVStringArray array, BiConsumer<String, Integer> consumer) {
        int index = 0, offset = 0, len = array.getLength();
        StringArrayData data = new StringArrayData();
        while (offset < len) {
            int num = array.get(offset, (len - offset), data);
            for (int i = 0; i < num; i++) {
                consumer.accept(data.data[offset + i], index++);
            }
            offset += num;
        }
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
    public Map<String, List<Object>> getTable() throws RuntimeException {
        String pvName = "NDRFACET:BUFFACQ";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"bpmd", "nrpos", "devs"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvInt),
                        fieldCreate.createScalar(ScalarType.pvInt),
                        fieldCreate.createScalar(ScalarArrayType.pvStringArray)
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
        args.getIntField("bpmd").put(57);
        args.getIntField("nrpos").put(5);
        args.getIntField("devs").put(0, 4, new String[]{"KLYS:LI03:31", "SBST:LI03:001", "BPMS:LI02:501", "TORO:LI20:2040"}, 0);

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure result = channel.rpc(nturi);

        return Arrays.stream(result.getSubField(PVStructure.class, "value").getPVFields())
                .map(column -> {
                    List<Object> columnValues = new ArrayList<>();
                    arrayIterator(column, columnValues::add);
                    return Pair.of(column.getFieldName(), columnValues);
                })
                .collect(Collectors.toMap(Pair::getLeft, Pair::getRight));
    }

    static <T extends PVField> void arrayIterator(T array, Consumer<Object> consumer) {
        arrayLoop(array, (s, i) -> consumer.accept(s));
    }

    static <T extends PVField> void arrayLoop(T array, BiConsumer<Object, Integer> consumer) {
        if (array instanceof PVBooleanArray) {
            booleanArrayLoop((PVBooleanArray) array, consumer::accept);
        } else if (array instanceof PVIntArray) {
            integerArrayLoop((PVIntArray) array, consumer::accept);
        } else if (array instanceof PVFloatArray) {
            floatArrayLoop((PVFloatArray) array, consumer::accept);
        } else if (array instanceof PVStringArray) {
            stringArrayLoop((PVStringArray) array, consumer::accept);
        }
    }

    static void booleanArrayLoop(PVBooleanArray array, BiConsumer<Boolean, Integer> consumer) {
        int index = 0, offset = 0, len = array.getLength();
        BooleanArrayData data = new BooleanArrayData();
        while (offset < len) {
            int num = array.get(offset, (len - offset), data);
            for (int i = 0; i < num; i++) {
                consumer.accept(data.data[offset + i], index++);
            }
            offset += num;
        }
    }

    static void integerArrayLoop(PVIntArray array, BiConsumer<Integer, Integer> consumer) {
        int index = 0;
        IteratorInteger it = array.get().iterator();
        while (it.hasNext()) {
            consumer.accept(it.nextInt(), index++);
        }
    }

    static void floatArrayLoop(PVFloatArray array, BiConsumer<Float, Integer> consumer) {
        int index = 0;
        IteratorFloat it = array.get().iterator();
        while (it.hasNext()) {
            consumer.accept(it.nextFloat(), index++);
        }
    }

    static void stringArrayLoop(PVStringArray array, BiConsumer<String, Integer> consumer) {
        int index = 0, offset = 0, len = array.getLength();
        StringArrayData data = new StringArrayData();
        while (offset < len) {
            int num = array.get(offset, (len - offset), data);
            for (int i = 0; i < num; i++) {
                consumer.accept(data.data[offset + i], index++);
            }
            offset += num;
        }
    }
}
```

#### Plain old Java

```java
import org.epics.pvaccess.ClientFactory;
import org.epics.pvaccess.client.rpc.RPCClientImpl;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.factory.FieldFactory;
import org.epics.pvdata.factory.PVDataFactory;
import org.epics.pvdata.pv.*;

public class JavaExample {
    public Map<String, List<Object>> getTable() throws RuntimeException {
        String pvName = "NDRFACET:BUFFACQ";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"bpmd", "nrpos", "devs"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvInt),
                        fieldCreate.createScalar(ScalarType.pvInt),
                        fieldCreate.createScalar(ScalarArrayType.pvStringArray)
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
        args.getIntField("bpmd").put(57);
        args.getIntField("nrpos").put(5);
        args.getIntField("devs").put(0, 4, new String[]{"KLYS:LI03:31", "SBST:LI03:001", "BPMS:LI02:501", "TORO:LI20:2040"}, 0);

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure result = client.request(request, 3.0);
        client.destroy();

        return Arrays.stream(result.getSubField(PVStructure.class, "value").getPVFields())
                .map(column -> {
                    List<Object> columnValues = new ArrayList<>();
                    arrayIterator(column, columnValues::add);
                    return Pair.of(column.getFieldName(), columnValues);
                })
                .collect(Collectors.toMap(Pair::getLeft, Pair::getRight));
    }

    static <T extends PVField> void arrayIterator(T array, Consumer<Object> consumer) {
        arrayLoop(array, (s, i) -> consumer.accept(s));
    }

    static <T extends PVField> void arrayLoop(T array, BiConsumer<Object, Integer> consumer) {
        if (array instanceof PVBooleanArray) {
            booleanArrayLoop((PVBooleanArray) array, consumer::accept);
        } else if (array instanceof PVIntArray) {
            integerArrayLoop((PVIntArray) array, consumer::accept);
        } else if (array instanceof PVFloatArray) {
            floatArrayLoop((PVFloatArray) array, consumer::accept);
        } else if (array instanceof PVStringArray) {
            stringArrayLoop((PVStringArray) array, consumer::accept);
        }
    }

    static void booleanArrayLoop(PVBooleanArray array, BiConsumer<Boolean, Integer> consumer) {
        int index = 0, offset = 0, len = array.getLength();
        BooleanArrayData data = new BooleanArrayData();
        while (offset < len) {
            int num = array.get(offset, (len - offset), data);
            for (int i = 0; i < num; i++) {
                consumer.accept(data.data[offset + i], index++);
            }
            offset += num;
        }
    }

    static void integerArrayLoop(PVIntArray array, BiConsumer<Integer, Integer> consumer) {
        int index = 0;
        IteratorInteger it = array.get().iterator();
        while (it.hasNext()) {
            consumer.accept(it.nextInt(), index++);
        }
    }

    static void floatArrayLoop(PVFloatArray array, BiConsumer<Float, Integer> consumer) {
        int index = 0;
        IteratorFloat it = array.get().iterator();
        while (it.hasNext()) {
            consumer.accept(it.nextFloat(), index++);
        }
    }

    static void stringArrayLoop(PVStringArray array, BiConsumer<String, Integer> consumer) {
        int index = 0, offset = 0, len = array.getLength();
        StringArrayData data = new StringArrayData();
        while (offset < len) {
            int num = array.get(offset, (len - offset), data);
            for (int i = 0; i < num; i++) {
                consumer.accept(data.data[offset + i], index++);
            }
            offset += num;
        }
    }
}
```

### Matlab examples

Matlab includes the following utility functions:

- `void` **aidainit**() - to initialise access to the AIDA framework
- `NTURI` **nturi**(`pvName`, `varargin`) - to create an `NTURI` Structure (
  see [Normative Types](2_2_Normative_Types.md)) for use with EPICS/AIDA-PVA data providers
- `matlab_structure` **nttable2struct** - to convert from NTTables to matlab structures
- `PVStructure` **ezrpc**(`nturi`) - takes an `NTURI` and executes it using EasyPva
- `PVStructure` **pvarpc**(`nturi`) - takes an `NTURI` and executes it using PvaClient
- `matlab_dynamic_type` **PvaChannel**(`pvName`) - takes a `pvName` and executes a **get()** or **set()** request with
  builder pattern
    - **with**(`name`, `value`) - specifies a parameter for the request
    - **returning**(`aidaType`) - specified the aida type to return from the request
    - **setReturningTable**(`value`) - For channels that return a table after setting a `value` use this API.
    - **get**() - executes the get request
    - **set**(`value`) - executes the set request with the given value

These have all been updated/added to be able to interact with the new AIDA-PVA framework.

#### aida-pva-client

```matlab
    aidainit;
    devs = ['KLYS:LI03:31' 'SBST:LI03:001' 'BPMS:LI02:501' 'TORO:LI20:2040'];
    try
      table = request('NDRFACET:BUFFACQ')
                  .with('BPMD', 57)
                  .with('NRPOS', 5)
                  .with('DEVS', devs)
                  .get();
    catch ME
       % do something when errors occur or just show ME.identifier
    end
```

#### using aidaget

```matlab
    aidainit;
    devs = ['KLYS:LI03:31' 'SBST:LI03:001' 'BPMS:LI02:501' 'TORO:LI20:2040'];
    tableResponse = aidaget('NDRFACET:BUFFACQ','TABLE',{'BPMD=57' 'NRPOS=5' ['DEVS=' devs] });
    table = nttable2struct(tableResponse);
```

#### EasyPVA

```matlab
    aidainit;
    devs = ['KLYS:LI03:31' 'SBST:LI03:001' 'BPMS:LI02:501' 'TORO:LI20:2040'];
    tableResponse = ezrpc(nturi('NDRFACET:BUFFACQ', 'BPMD', 57, 'NRPOS', 5, 'DEVS', devs)); 
    table = nttable2struct(tableResponse);
```

#### PvaClient

```matlab
    aidainit;
    devs = ['KLYS:LI03:31' 'SBST:LI03:001' 'BPMS:LI02:501' 'TORO:LI20:2040'];
    tableResponse = pvarpc(nturi('NDRFACET:BUFFACQ', 'BPMD', 57, 'NRPOS', 5, 'DEVS', devs)); 
    table = nttable2struct(tableResponse);
```

## Test Output

Tests are implemented in: SlcBuffAcqTest.java

```shell
java -cp aida-pva-tests.jar  "edu.stanford.slac.aida.test.SlcBuffAcqTest" -c
#################################################
AIDA-PVA SLC Buffered Acquisition TESTS
#################################################

████ Test 1: Get values of given BPM for 1 pulse
_________________________________________________

get: NDRFACET:BUFFACQ (BPMD=57, BPMS=[BPMS:LI11:501])
    BPM Values:  1 rows retrieved: ✔
      BPM Name pulse id x offset (mm) y offset (mm) num particles (coulomb) STAT good measurement
          name       id             x             y                   tmits STAT         goodmeas
 BPMS:LI11:501    39714   -0.98880225     0.9987344                 1.0E-10    1               15

████ Test 2: Get values of 4 BPMs
_________________________________________________

get: NDRFACET:BUFFACQ (NRPOS=180, BPMD=57, BPMS=[BPMS:LI11:501, BPMS:LI11:601, BPMS:LI11:701, BPMS:LI11:801])
    BPM Values:  720 rows retrieved: ✔
      BPM Name pulse id x offset (mm) y offset (mm) num particles (coulomb) STAT good measurement
          name       id             x             y                   tmits STAT         goodmeas
 BPMS:LI11:501    71311    0.45966175    0.81614506                 1.0E-10    1               15
 BPMS:LI11:501    71312    -0.9996438    -0.9657358                 1.0E-10    1               15
 BPMS:LI11:501    71313   -0.28856283    0.69943863                 1.0E-10    1               15
 BPMS:LI11:501    71314    -0.9565048   -0.91913205                 1.0E-10    1               15
 BPMS:LI11:501    71315     0.5896404   -0.47585112                 1.0E-10    1               15
 BPMS:LI11:501    71316    -0.9961865   -0.58156544                 1.0E-10    1               15
 BPMS:LI11:501    71317  -0.019657379    -0.9489798                 1.0E-10    1               15
 BPMS:LI11:501    71318    0.20760125    -0.3898321                 1.0E-10    1               15
 BPMS:LI11:501    71319   -0.99998397    -0.4982474                 1.0E-10    1               15
 BPMS:LI11:501    71320    -0.9509019  -0.028907135                 1.0E-10    1               15
 BPMS:LI11:501    71321    -0.9561267    0.16867593                 1.0E-10    1               15
 BPMS:LI11:501    71322    -0.9519366    0.99881446                 1.0E-10    1               15
 BPMS:LI11:501    71323     0.6986331     -0.817034                 1.0E-10    1               15
 BPMS:LI11:501    71324     -0.416191  -0.076078236                 1.0E-10    1               15
 BPMS:LI11:501    71325    -0.2768235    0.99203396                 1.0E-10    1               15
 BPMS:LI11:501    71326    0.13568361    0.31046924                 1.0E-10    1               15
 BPMS:LI11:501    71327   -0.99950105    -0.7631048                 1.0E-10    1               15
 BPMS:LI11:501    71328   -0.97782874   -0.32393646                 1.0E-10    1               15
 BPMS:LI11:501    71329    -0.9816144   -0.78763425                 1.0E-10    1               15
 BPMS:LI11:501    71330    -0.7512656     0.5648408                 1.0E-10    1               15
 BPMS:LI11:501    71331    -0.3999047     0.5881298                 1.0E-10    1               15
 BPMS:LI11:501    71332     -0.944516   -0.16060852                 1.0E-10    1               15
 BPMS:LI11:501    71333   -0.31001797    -0.7929373                 1.0E-10    1               15
 BPMS:LI11:501    71334    0.33051273    -0.4097404                 1.0E-10    1               15
 BPMS:LI11:501    71335    0.81092834    -0.9597818                 1.0E-10    1               15
 BPMS:LI11:501    71336    0.88678473    0.06904734                 1.0E-10    1               15
 BPMS:LI11:501    71337      0.394236      0.934358                 1.0E-10    1               15
 BPMS:LI11:501    71338   -0.10614119    -0.9791559                 1.0E-10    1               15
 BPMS:LI11:501    71339   -0.84746563   -0.88753253                 1.0E-10    1               15
 BPMS:LI11:501    71340    -0.3995812   -0.27460724                 1.0E-10    1               15
 BPMS:LI11:501    71341    -0.8932582   -0.33694443                 1.0E-10    1               15
 BPMS:LI11:501    71342   -0.70726365   0.007962271                 1.0E-10    1               15
 BPMS:LI11:501    71343     -0.912166     -0.750424                 1.0E-10    1               15
 BPMS:LI11:501    71344       0.91774   -0.44082963                 1.0E-10    1               15
 BPMS:LI11:501    71345    0.97010416    0.39791656                 1.0E-10    1               15
 BPMS:LI11:501    71346    0.65207165   -0.83904237                 1.0E-10    1               15
 BPMS:LI11:501    71347    0.24208377      0.897283                 1.0E-10    1               15
 BPMS:LI11:501    71348     0.9792773    0.19851404                 1.0E-10    1               15
 BPMS:LI11:501    71349    0.51079124    -0.9753791                 1.0E-10    1               15
 BPMS:LI11:501    71350     0.9985141     0.7495926                 1.0E-10    1               15
 BPMS:LI11:501    71351     0.8671039    -0.6893454                 1.0E-10    1               15
 BPMS:LI11:501    71352    0.04772717    0.39067003                 1.0E-10    1               15
 BPMS:LI11:501    71353    -0.9986429   -0.35313043                 1.0E-10    1               15
 BPMS:LI11:501    71354    0.11313338     0.5115758                 1.0E-10    1               15
 BPMS:LI11:501    71355    0.19663319   -0.15201674                 1.0E-10    1               15
 BPMS:LI11:501    71356     0.7304926   -0.92357635                 1.0E-10    1               15
 BPMS:LI11:501    71357   -0.85164225    0.77008027                 1.0E-10    1               15
 BPMS:LI11:501    71358    0.07215431    -0.3466189                 1.0E-10    1               15
 BPMS:LI11:501    71359   -0.37135527     0.9954546                 1.0E-10    1               15
 BPMS:LI11:501    71360      0.427083   -0.99184394                 1.0E-10    1               15
 BPMS:LI11:501    71361   -0.88769317    -0.8517041                 1.0E-10    1               15
 BPMS:LI11:501    71362   -0.14050159    0.19369832                 1.0E-10    1               15
 BPMS:LI11:501    71363     0.6440539   -0.60858464                 1.0E-10    1               15
 BPMS:LI11:501    71364     -0.423585    0.17715824                 1.0E-10    1               15
 BPMS:LI11:501    71365   -0.14801487     0.9921004                 1.0E-10    1               15
 BPMS:LI11:501    71366    -0.9980812   -0.10050726                 1.0E-10    1               15
 BPMS:LI11:501    71367     0.4759536     0.8690094                 1.0E-10    1               15
 BPMS:LI11:501    71368    0.22479656   -0.42007473                 1.0E-10    1               15
 BPMS:LI11:501    71369    -0.2726276   -0.47542286                 1.0E-10    1               15
 BPMS:LI11:501    71370    -0.9843776    -0.9724591                 1.0E-10    1               15
 BPMS:LI11:501    71371     0.8260674    0.97675973                 1.0E-10    1               15
 BPMS:LI11:501    71372    -0.8724894   -0.13175222                 1.0E-10    1               15
 BPMS:LI11:501    71373     0.9586525   0.044867836                 1.0E-10    1               15
 BPMS:LI11:501    71374    0.27142605     -0.698981                 1.0E-10    1               15
 BPMS:LI11:501    71375    0.82612574     0.3290843                 1.0E-10    1               15
 BPMS:LI11:501    71376    -0.8036695      0.724112                 1.0E-10    1               15
 BPMS:LI11:501    71377   -0.23967166     -0.960036                 1.0E-10    1               15
 BPMS:LI11:501    71378    0.43958148   -0.62089926                 1.0E-10    1               15
 BPMS:LI11:501    71379    0.59204334    0.30272016                 1.0E-10    1               15
 BPMS:LI11:501    71380     0.7277365   -0.30172017                 1.0E-10    1               15
 BPMS:LI11:501    71381   -0.68280077   0.123198465                 1.0E-10    1               15
 BPMS:LI11:501    71382     0.8936497    0.07123592                 1.0E-10    1               15
 BPMS:LI11:501    71383   -0.53492385    0.98391604                 1.0E-10    1               15
 BPMS:LI11:501    71384      0.975298    0.63912165                 1.0E-10    1               15
 BPMS:LI11:501    71385    -0.6287968    0.28650317                 1.0E-10    1               15
 BPMS:LI11:501    71386    0.33430973   0.030775528                 1.0E-10    1               15
 BPMS:LI11:501    71387    0.63676924    -0.5559926                 1.0E-10    1               15
 BPMS:LI11:501    71388    -0.9653396     0.8751663                 1.0E-10    1               15
 BPMS:LI11:501    71389   0.033790134  -0.073080026                 1.0E-10    1               15
 BPMS:LI11:501    71390    0.75387365   -0.74192035                 1.0E-10    1               15
 BPMS:LI11:501    71391    0.84703827    -0.9139328                 1.0E-10    1               15
 BPMS:LI11:501    71392   -0.54507655    0.60608476                 1.0E-10    1               15
 BPMS:LI11:501    71393   -0.49261868   -0.62699187                 1.0E-10    1               15
 BPMS:LI11:501    71394     0.3265573      0.784005                 1.0E-10    1               15
 BPMS:LI11:501    71395    0.38260993    -0.9944412                 1.0E-10    1               15
 BPMS:LI11:501    71396   -0.70519364    0.81792265                 1.0E-10    1               15
 BPMS:LI11:501    71397    0.96830606   -0.16703272                 1.0E-10    1               15
 BPMS:LI11:501    71398     0.1818699     0.9559538                 1.0E-10    1               15
 BPMS:LI11:501    71399     0.8436884   -0.43848473                 1.0E-10    1               15
 BPMS:LI11:501    71400    -0.7583512    0.75110674                 1.0E-10    1               15
 BPMS:LI11:501    71401     0.8418935    0.36348823                 1.0E-10    1               15
 BPMS:LI11:501    71402    0.93876666     0.1748013                 1.0E-10    1               15
 BPMS:LI11:501    71403    -0.7207577    0.96613854                 1.0E-10    1               15
 BPMS:LI11:501    71404    -0.7381788    -0.6948941                 1.0E-10    1               15
 BPMS:LI11:501    71405     0.9098735     0.5959392                 1.0E-10    1               15
 BPMS:LI11:501    71406   -0.15142688    0.10010313                 1.0E-10    1               15
 BPMS:LI11:501    71407    -0.1415196     0.9765598                 1.0E-10    1               15
 BPMS:LI11:501    71408  -0.050638814    0.73721933                 1.0E-10    1               15
 BPMS:LI11:501    71409   -0.97793293   -0.98375857                 1.0E-10    1               15
 BPMS:LI11:501    71410   -0.53157634   -0.91122633                 1.0E-10    1               15
 BPMS:LI11:501    71411    -0.8989742     0.9960181                 1.0E-10    1               15
 BPMS:LI11:501    71412   0.052087273    -0.6074661                 1.0E-10    1               15
 BPMS:LI11:501    71413     0.9997046    0.39412636                 1.0E-10    1               15
 BPMS:LI11:501    71414    -0.9720048    0.99334663                 1.0E-10    1               15
 BPMS:LI11:501    71415    0.39648655   -0.99614745                 1.0E-10    1               15
 BPMS:LI11:501    71416    0.09939547   -0.61114097                 1.0E-10    1               15
 BPMS:LI11:501    71417     0.7942105   -0.50935674                 1.0E-10    1               15
 BPMS:LI11:501    71418    -0.3339006   -0.91446966                 1.0E-10    1               15
 BPMS:LI11:501    71419     0.9998781    0.30164674                 1.0E-10    1               15
 BPMS:LI11:501    71420     0.5981621    0.13918823                 1.0E-10    1               15
 BPMS:LI11:501    71421     0.9867392    0.14137723                 1.0E-10    1               15
 BPMS:LI11:501    71422    -0.7356974    -0.9938359                 1.0E-10    1               15
 BPMS:LI11:501    71423    0.22534445    -0.9217651                 1.0E-10    1               15
 BPMS:LI11:501    71424    0.84428495  0.0022321064                 1.0E-10    1               15
 BPMS:LI11:501    71425    -0.5101407    -0.9917169                 1.0E-10    1               15
 BPMS:LI11:501    71426    -0.8810613   -0.90036243                 1.0E-10    1               15
 BPMS:LI11:501    71427   -0.64354056   -0.98539436                 1.0E-10    1               15
 BPMS:LI11:501    71428    0.84113663    0.39981365                 1.0E-10    1               15
 BPMS:LI11:501    71429     0.4444436    0.31982425                 1.0E-10    1               15
 BPMS:LI11:501    71430    -0.5566323   -0.99733686                 1.0E-10    1               15
 BPMS:LI11:501    71431    0.85911906    0.89702815                 1.0E-10    1               15
 BPMS:LI11:501    71432     0.9874898    -0.8394479                 1.0E-10    1               15
 BPMS:LI11:501    71433     0.6918125   -0.17769906                 1.0E-10    1               15
 BPMS:LI11:501    71434   -0.97485286    0.86241204                 1.0E-10    1               15
 BPMS:LI11:501    71435   -0.22053108    0.82497877                 1.0E-10    1               15
 BPMS:LI11:501    71436     0.9960778    -0.8272127                 1.0E-10    1               15
 BPMS:LI11:501    71437     0.3987547    0.25882813                 1.0E-10    1               15
 BPMS:LI11:501    71438    0.41305885   -0.11357355                 1.0E-10    1               15
 BPMS:LI11:501    71439     0.9114743   -0.24363701                 1.0E-10    1               15
 BPMS:LI11:501    71440   -0.20371307   -0.87146246                 1.0E-10    1               15
 BPMS:LI11:501    71441   -0.98881894    0.72429204                 1.0E-10    1               15
 BPMS:LI11:501    71442   -0.52359074     0.9757992                 1.0E-10    1               15
 BPMS:LI11:501    71443     0.6328884   -0.97273666                 1.0E-10    1               15
 BPMS:LI11:501    71444   -0.14318043    -0.9061928                 1.0E-10    1               15
 BPMS:LI11:501    71445   -0.87597066    0.64724684                 1.0E-10    1               15
 BPMS:LI11:501    71446  0.0089267995     0.9670762                 1.0E-10    1               15
 BPMS:LI11:501    71447     0.5489665     0.6302186                 1.0E-10    1               15
 BPMS:LI11:501    71448     0.6396804   -0.96682674                 1.0E-10    1               15
 BPMS:LI11:501    71449    -0.9735195   -0.64020824                 1.0E-10    1               15
 BPMS:LI11:501    71450     0.9871718     0.6844541                 1.0E-10    1               15
 BPMS:LI11:501    71451   -0.84059614   -0.48853055                 1.0E-10    1               15
 BPMS:LI11:501    71452   -0.98744386      0.956624                 1.0E-10    1               15
 BPMS:LI11:501    71453    -0.1004668    0.95857704                 1.0E-10    1               15
 BPMS:LI11:501    71454   -0.84079003     0.7634589                 1.0E-10    1               15
 BPMS:LI11:501    71455      0.518603    0.21073267                 1.0E-10    1               15
 BPMS:LI11:501    71456   -0.93585163    -0.9429042                 1.0E-10    1               15
 BPMS:LI11:501    71457   -0.83704984    -0.9990208                 1.0E-10    1               15
 BPMS:LI11:501    71458   0.016150163   -0.48110363                 1.0E-10    1               15
 BPMS:LI11:501    71459    -0.5330927   -0.06754128                 1.0E-10    1               15
 BPMS:LI11:501    71460   -0.25574782     0.8720495                 1.0E-10    1               15
 BPMS:LI11:501    71461    -0.7702961   -0.94659364                 1.0E-10    1               15
 BPMS:LI11:501    71462     -0.794114     0.4627768                 1.0E-10    1               15
 BPMS:LI11:501    71463     0.8486972    0.41484162                 1.0E-10    1               15
 BPMS:LI11:501    71464     0.7101495   -0.35839385                 1.0E-10    1               15
 BPMS:LI11:501    71465      0.961015    -0.7876115                 1.0E-10    1               15
 BPMS:LI11:501    71466    0.10201535    0.27392614                 1.0E-10    1               15
 BPMS:LI11:501    71467   -0.99706274    -0.9533732                 1.0E-10    1               15
 BPMS:LI11:501    71468      0.680962    -0.9983168                 1.0E-10    1               15
 BPMS:LI11:501    71469    -0.9386571   -0.44404662                 1.0E-10    1               15
 BPMS:LI11:501    71470     0.7955671    0.81082195                 1.0E-10    1               15
 BPMS:LI11:501    71471     0.9901383     0.9886071                 1.0E-10    1               15
 BPMS:LI11:501    71472    -0.5573699      0.766041                 1.0E-10    1               15
 BPMS:LI11:501    71473     0.7369902     0.7162255                 1.0E-10    1               15
 BPMS:LI11:501    71474     0.7465771    0.86946505                 1.0E-10    1               15
 BPMS:LI11:501    71475    -0.8411641    -0.5878626                 1.0E-10    1               15
 BPMS:LI11:501    71476     0.7480264   -0.36268374                 1.0E-10    1               15
 BPMS:LI11:501    71477   -0.15844312     0.2460512                 1.0E-10    1               15
 BPMS:LI11:501    71478    -0.8636311   -0.42578933                 1.0E-10    1               15
 BPMS:LI11:501    71479     0.8482402     0.7312344                 1.0E-10    1               15
 BPMS:LI11:501    71480   -0.95361364   -0.14575562                 1.0E-10    1               15
 BPMS:LI11:501    71481     0.9830331    0.60959744                 1.0E-10    1               15
 BPMS:LI11:501    71482    0.51569325   -0.15023045                 1.0E-10    1               15
 BPMS:LI11:501    71483   -0.36940446   -0.23545368                 1.0E-10    1               15
 BPMS:LI11:501    71484    0.96660835    0.72336155                 1.0E-10    1               15
 BPMS:LI11:501    71485   -0.42492622   -0.93705595                 1.0E-10    1               15
 BPMS:LI11:501    71486   0.006242506     0.9855929                 1.0E-10    1               15
 BPMS:LI11:501    71487    0.81454295     0.5568136                 1.0E-10    1               15
 BPMS:LI11:501    71488     0.9737794   -0.99178386                 1.0E-10    1               15
 BPMS:LI11:501    71489   -0.68368006    0.98194814                 1.0E-10    1               15
 BPMS:LI11:501    71490    0.99717355    0.89559335                 1.0E-10    1               15
 BPMS:LI11:601    71311    -0.8219435     0.9281382                 1.0E-10    1               15
 BPMS:LI11:601    71312    0.99994314   -0.19392668                 1.0E-10    1               15
 BPMS:LI11:601    71313    0.62368256  -0.039565127                 1.0E-10    1               15
 BPMS:LI11:601    71314   -0.83815616    0.94231474                 1.0E-10    1               15
 BPMS:LI11:601    71315    0.12465538    -0.5499371                 1.0E-10    1               15
 BPMS:LI11:601    71316    -0.8771476     0.7711539                 1.0E-10    1               15
 BPMS:LI11:601    71317     0.8504254    -0.7005151                 1.0E-10    1               15
 BPMS:LI11:601    71318    -0.8735146    0.22922716                 1.0E-10    1               15
 BPMS:LI11:601    71319    0.38819876   -0.22996408                 1.0E-10    1               15
 BPMS:LI11:601    71320     0.6971772    0.38518313                 1.0E-10    1               15
 BPMS:LI11:601    71321    0.97390115    0.49474615                 1.0E-10    1               15
 BPMS:LI11:601    71322   -0.16693966     0.9999495                 1.0E-10    1               15
 BPMS:LI11:601    71323   0.060076233    0.67385536                 1.0E-10    1               15
 BPMS:LI11:601    71324    0.39473626      0.225012                 1.0E-10    1               15
 BPMS:LI11:601    71325     0.9102174   -0.66293097                 1.0E-10    1               15
 BPMS:LI11:601    71326   -0.30141008    0.25232184                 1.0E-10    1               15
 BPMS:LI11:601    71327    0.97451377   -0.40665013                 1.0E-10    1               15
 BPMS:LI11:601    71328    0.87502813    -0.8331743                 1.0E-10    1               15
 BPMS:LI11:601    71329    0.92534304     0.5361781                 1.0E-10    1               15
 BPMS:LI11:601    71330   -0.55702597    0.05949258                 1.0E-10    1               15
 BPMS:LI11:601    71331    0.59395444    -0.5285554                 1.0E-10    1               15
 BPMS:LI11:601    71332   -0.41752765   -0.72258806                 1.0E-10    1               15
 BPMS:LI11:601    71333     0.3418039    0.98503196                 1.0E-10    1               15
 BPMS:LI11:601    71334   -0.46270335    0.58406407                 1.0E-10    1               15
 BPMS:LI11:601    71335     0.9604537    0.84184146                 1.0E-10    1               15
 BPMS:LI11:601    71336    -0.5589288   -0.41047192                 1.0E-10    1               15
 BPMS:LI11:601    71337    -0.3419715      0.994495                 1.0E-10    1               15
 BPMS:LI11:601    71338    0.28516388    0.19332036                 1.0E-10    1               15
 BPMS:LI11:601    71339    0.88220346    -0.3426906                 1.0E-10    1               15
 BPMS:LI11:601    71340     0.4353436    0.86704993                 1.0E-10    1               15
 BPMS:LI11:601    71341    0.37793446    0.92560667                 1.0E-10    1               15
 BPMS:LI11:601    71342    0.86343604    0.64532816                 1.0E-10    1               15
 BPMS:LI11:601    71343     0.3256814   -0.84396356                 1.0E-10    1               15
 BPMS:LI11:601    71344     -0.891814    0.47862545                 1.0E-10    1               15
 BPMS:LI11:601    71345   -0.99981326    -0.9605129                 1.0E-10    1               15
 BPMS:LI11:601    71346   -0.12371068     0.5826127                 1.0E-10    1               15
 BPMS:LI11:601    71347    0.98163885    -0.6654697                 1.0E-10    1               15
 BPMS:LI11:601    71348     0.7478975   -0.55680937                 1.0E-10    1               15
 BPMS:LI11:601    71349   -0.96500206    0.48202518                 1.0E-10    1               15
 BPMS:LI11:601    71350    0.55442756    -0.4820744                 1.0E-10    1               15
 BPMS:LI11:601    71351    -0.8293157    0.75680566                 1.0E-10    1               15
 BPMS:LI11:601    71352   -0.70161307    -0.6810635                 1.0E-10    1               15
 BPMS:LI11:601    71353    -0.9991677    -0.9592668                 1.0E-10    1               15
 BPMS:LI11:601    71354    0.12772197     0.9950443                 1.0E-10    1               15
 BPMS:LI11:601    71355   -0.31355044    -0.4138511                 1.0E-10    1               15
 BPMS:LI11:601    71356    0.98265797    -0.5932387                 1.0E-10    1               15
 BPMS:LI11:601    71357     0.8967251     0.8762128                 1.0E-10    1               15
 BPMS:LI11:601    71358     0.3460206     0.9085633                 1.0E-10    1               15
 BPMS:LI11:601    71359    -0.8900372   -0.78090715                 1.0E-10    1               15
 BPMS:LI11:601    71360    -0.7576049     0.9959889                 1.0E-10    1               15
 BPMS:LI11:601    71361   -0.97760296     0.9529342                 1.0E-10    1               15
 BPMS:LI11:601    71362    -0.4609868    -0.9994636                 1.0E-10    1               15
 BPMS:LI11:601    71363     0.2968512    0.19284235                 1.0E-10    1               15
 BPMS:LI11:601    71364    0.38155898  -0.019203788                 1.0E-10    1               15
 BPMS:LI11:601    71365    0.46042892    -0.9386584                 1.0E-10    1               15
 BPMS:LI11:601    71366   -0.81012964    0.65075576                 1.0E-10    1               15
 BPMS:LI11:601    71367   -0.18398438    -0.9651494                 1.0E-10    1               15
 BPMS:LI11:601    71368    0.15813921   -0.63850987                 1.0E-10    1               15
 BPMS:LI11:601    71369   -0.29797912     0.9999977                 1.0E-10    1               15
 BPMS:LI11:601    71370      0.387556    -0.9784014                 1.0E-10    1               15
 BPMS:LI11:601    71371    0.27750102    -0.9686138                 1.0E-10    1               15
 BPMS:LI11:601    71372      0.767146     0.9990378                 1.0E-10    1               15
 BPMS:LI11:601    71373    0.52259725  -0.076075904                 1.0E-10    1               15
 BPMS:LI11:601    71374   -0.25034857     0.7841307                 1.0E-10    1               15
 BPMS:LI11:601    71375    0.97558206     0.7979054                 1.0E-10    1               15
 BPMS:LI11:601    71376    0.44497716     0.9810236                 1.0E-10    1               15
 BPMS:LI11:601    71377    -0.9963738      0.968057                 1.0E-10    1               15
 BPMS:LI11:601    71378    0.99253905    0.43318236                 1.0E-10    1               15
 BPMS:LI11:601    71379     -0.240938    -0.7862343                 1.0E-10    1               15
 BPMS:LI11:601    71380     0.9988522    -0.0649279                 1.0E-10    1               15
 BPMS:LI11:601    71381    -0.9275139    0.97523373                 1.0E-10    1               15
 BPMS:LI11:601    71382   0.057630792    -0.9528599                 1.0E-10    1               15
 BPMS:LI11:601    71383    -0.6663012     -0.946775                 1.0E-10    1               15
 BPMS:LI11:601    71384    -0.3697446     0.9932793                 1.0E-10    1               15
 BPMS:LI11:601    71385      0.977756     0.4992586                 1.0E-10    1               15
 BPMS:LI11:601    71386     0.4665908   -0.19824485                 1.0E-10    1               15
 BPMS:LI11:601    71387    -0.9447621    0.93172807                 1.0E-10    1               15
 BPMS:LI11:601    71388     0.6575576     -0.779576                 1.0E-10    1               15
 BPMS:LI11:601    71389    0.15396583    -0.9999642                 1.0E-10    1               15
 BPMS:LI11:601    71390    -0.8940038   -0.30362833                 1.0E-10    1               15
 BPMS:LI11:601    71391    0.77017266    -0.9963527                 1.0E-10    1               15
 BPMS:LI11:601    71392      0.450517   -0.96513754                 1.0E-10    1               15
 BPMS:LI11:601    71393    -0.7636015     0.4127448                 1.0E-10    1               15
 BPMS:LI11:601    71394      0.963743   -0.23216143                 1.0E-10    1               15
 BPMS:LI11:601    71395   -0.18571445     0.8905072                 1.0E-10    1               15
 BPMS:LI11:601    71396    0.91070634   0.031596296                 1.0E-10    1               15
 BPMS:LI11:601    71397   -0.09692367    0.99548465                 1.0E-10    1               15
 BPMS:LI11:601    71398    -0.9243528     0.2193999                 1.0E-10    1               15
 BPMS:LI11:601    71399     0.8147163    -0.8971157                 1.0E-10    1               15
 BPMS:LI11:601    71400    0.48645744   -0.84772605                 1.0E-10    1               15
 BPMS:LI11:601    71401   -0.97650754     0.0913066                 1.0E-10    1               15
 BPMS:LI11:601    71402     0.9660282     0.8414745                 1.0E-10    1               15
 BPMS:LI11:601    71403     0.9948445      0.962355                 1.0E-10    1               15
 BPMS:LI11:601    71404   -0.43265128   -0.40304285                 1.0E-10    1               15
 BPMS:LI11:601    71405     0.9522055    0.98211616                 1.0E-10    1               15
 BPMS:LI11:601    71406    0.77180064   -0.36823732                 1.0E-10    1               15
 BPMS:LI11:601    71407   -0.38921762    -0.8721417                 1.0E-10    1               15
 BPMS:LI11:601    71408    -0.9999975    -0.4644595                 1.0E-10    1               15
 BPMS:LI11:601    71409    -0.4296512     0.9408689                 1.0E-10    1               15
 BPMS:LI11:601    71410    0.96648854   -0.34049585                 1.0E-10    1               15
 BPMS:LI11:601    71411   0.014522631   -0.17319746                 1.0E-10    1               15
 BPMS:LI11:601    71412     0.7292903    0.09892284                 1.0E-10    1               15
 BPMS:LI11:601    71413    0.54237616   -0.94806653                 1.0E-10    1               15
 BPMS:LI11:601    71414    -0.9724827    -0.9194372                 1.0E-10    1               15
 BPMS:LI11:601    71415     0.9642841     -0.816948                 1.0E-10    1               15
 BPMS:LI11:601    71416   -0.93219495    -0.7056387                 1.0E-10    1               15
 BPMS:LI11:601    71417    -0.8262042    -0.9993223                 1.0E-10    1               15
 BPMS:LI11:601    71418    0.24526745    0.42675102                 1.0E-10    1               15
 BPMS:LI11:601    71419    0.42022496      0.837645                 1.0E-10    1               15
 BPMS:LI11:601    71420    -0.5528839   -0.45138562                 1.0E-10    1               15
 BPMS:LI11:601    71421   0.038593736    -0.6251892                 1.0E-10    1               15
 BPMS:LI11:601    71422    0.92597055       0.54306                 1.0E-10    1               15
 BPMS:LI11:601    71423    0.34571967     0.0901222                 1.0E-10    1               15
 BPMS:LI11:601    71424   -0.95628667   -0.91590446                 1.0E-10    1               15
 BPMS:LI11:601    71425   -0.79992336   -0.93922096                 1.0E-10    1               15
 BPMS:LI11:601    71426  -0.062200602   -0.24530445                 1.0E-10    1               15
 BPMS:LI11:601    71427   -0.47883797     0.8213696                 1.0E-10    1               15
 BPMS:LI11:601    71428    0.09137389   -0.99233407                 1.0E-10    1               15
 BPMS:LI11:601    71429    0.61946875    -0.6492953                 1.0E-10    1               15
 BPMS:LI11:601    71430     0.3729088    -0.5071314                 1.0E-10    1               15
 BPMS:LI11:601    71431    0.42061615     0.7170832                 1.0E-10    1               15
 BPMS:LI11:601    71432    0.75426775    0.57493454                 1.0E-10    1               15
 BPMS:LI11:601    71433    0.22989133    -0.4700645                 1.0E-10    1               15
 BPMS:LI11:601    71434     0.2042702   -0.34118858                 1.0E-10    1               15
 BPMS:LI11:601    71435    0.89419895   -0.19507428                 1.0E-10    1               15
 BPMS:LI11:601    71436    -0.9937747    0.73380643                 1.0E-10    1               15
 BPMS:LI11:601    71437    0.25278762    0.65255076                 1.0E-10    1               15
 BPMS:LI11:601    71438     0.5906282    0.87122566                 1.0E-10    1               15
 BPMS:LI11:601    71439  -0.036804616   -0.08405336                 1.0E-10    1               15
 BPMS:LI11:601    71440    -0.8395487   -0.71994305                 1.0E-10    1               15
 BPMS:LI11:601    71441   -0.80723464   -0.59737957                 1.0E-10    1               15
 BPMS:LI11:601    71442   -0.27021322    -0.8345473                 1.0E-10    1               15
 BPMS:LI11:601    71443    0.51903385   -0.98246604                 1.0E-10    1               15
 BPMS:LI11:601    71444    -0.9275985   -0.99601376                 1.0E-10    1               15
 BPMS:LI11:601    71445    0.44202718    0.93100953                 1.0E-10    1               15
 BPMS:LI11:601    71446    -0.3356131     0.9890123                 1.0E-10    1               15
 BPMS:LI11:601    71447   -0.96157426    -0.5920849                 1.0E-10    1               15
 BPMS:LI11:601    71448   0.005028298    0.26531765                 1.0E-10    1               15
 BPMS:LI11:601    71449     0.3681962   -0.36661893                 1.0E-10    1               15
 BPMS:LI11:601    71450   -0.54472405     0.9818061                 1.0E-10    1               15
 BPMS:LI11:601    71451  -0.021177497   -0.81902295                 1.0E-10    1               15
 BPMS:LI11:601    71452   -0.15681936     0.9113828                 1.0E-10    1               15
 BPMS:LI11:601    71453   -0.22213106   -0.24566194                 1.0E-10    1               15
 BPMS:LI11:601    71454    0.91075045    0.83526987                 1.0E-10    1               15
 BPMS:LI11:601    71455   -0.99613297   -0.78029454                 1.0E-10    1               15
 BPMS:LI11:601    71456    0.45512667    -0.2649125                 1.0E-10    1               15
 BPMS:LI11:601    71457    0.37303352   -0.32581925                 1.0E-10    1               15
 BPMS:LI11:601    71458    -0.4863761   -0.39771274                 1.0E-10    1               15
 BPMS:LI11:601    71459   -0.17870475     0.3461055                 1.0E-10    1               15
 BPMS:LI11:601    71460    0.36956957     0.7803506                 1.0E-10    1               15
 BPMS:LI11:601    71461      0.957522    -0.5981401                 1.0E-10    1               15
 BPMS:LI11:601    71462     0.8203869     0.6201008                 1.0E-10    1               15
 BPMS:LI11:601    71463   -0.08929736    0.68250203                 1.0E-10    1               15
 BPMS:LI11:601    71464     0.7217724    0.93035245                 1.0E-10    1               15
 BPMS:LI11:601    71465    -0.3038789    0.35819095                 1.0E-10    1               15
 BPMS:LI11:601    71466     0.5589633    0.19286093                 1.0E-10    1               15
 BPMS:LI11:601    71467   -0.49199453    0.88934076                 1.0E-10    1               15
 BPMS:LI11:601    71468    0.64933753     0.9365795                 1.0E-10    1               15
 BPMS:LI11:601    71469    0.70797855   -0.87700677                 1.0E-10    1               15
 BPMS:LI11:601    71470    -0.9601566   -0.21550333                 1.0E-10    1               15
 BPMS:LI11:601    71471   -0.42298767    0.22337641                 1.0E-10    1               15
 BPMS:LI11:601    71472     0.8363755  -0.036139514                 1.0E-10    1               15
 BPMS:LI11:601    71473    -0.7744886    -0.2496342                 1.0E-10    1               15
 BPMS:LI11:601    71474    0.16207951    0.22282203                 1.0E-10    1               15
 BPMS:LI11:601    71475    -0.6585601    0.28995952                 1.0E-10    1               15
 BPMS:LI11:601    71476   -0.23523204   -0.08365819                 1.0E-10    1               15
 BPMS:LI11:601    71477    0.09337933    -0.9811438                 1.0E-10    1               15
 BPMS:LI11:601    71478   -0.49230847    -0.4505128                 1.0E-10    1               15
 BPMS:LI11:601    71479     0.8157707    0.12948133                 1.0E-10    1               15
 BPMS:LI11:601    71480   -0.24521169   -0.79661345                 1.0E-10    1               15
 BPMS:LI11:601    71481     0.6505355    -0.9824934                 1.0E-10    1               15
 BPMS:LI11:601    71482     0.8300129    -0.7551726                 1.0E-10    1               15
 BPMS:LI11:601    71483    0.62699836   -0.76495063                 1.0E-10    1               15
 BPMS:LI11:601    71484    -0.6153157   -0.99993724                 1.0E-10    1               15
 BPMS:LI11:601    71485     0.7721119   -0.99934924                 1.0E-10    1               15
 BPMS:LI11:601    71486    0.89630586    0.56544733                 1.0E-10    1               15
 BPMS:LI11:601    71487   -0.96827024    0.82427144                 1.0E-10    1               15
 BPMS:LI11:601    71488    -0.8844884    0.73460215                 1.0E-10    1               15
 BPMS:LI11:601    71489   -0.43412685     0.8560743                 1.0E-10    1               15
 BPMS:LI11:601    71490    -0.9464635   -0.76299363                 1.0E-10    1               15
 BPMS:LI11:701    71311   -0.16707267     0.2857163                 1.0E-10    1               15
 BPMS:LI11:701    71312     0.5922336   -0.37277633                 1.0E-10    1               15
 BPMS:LI11:701    71313       0.94211    0.99977446                 1.0E-10    1               15
 BPMS:LI11:701    71314   -0.99824995   -0.98047656                 1.0E-10    1               15
 BPMS:LI11:701    71315   -0.22513205     0.3235358                 1.0E-10    1               15
 BPMS:LI11:701    71316    0.97838587   -0.98869234                 1.0E-10    1               15
 BPMS:LI11:701    71317    -0.9838701    -0.9869774                 1.0E-10    1               15
 BPMS:LI11:701    71318   -0.69129175  -0.051180474                 1.0E-10    1               15
 BPMS:LI11:701    71319   -0.94244295    -0.8121991                 1.0E-10    1               15
 BPMS:LI11:701    71320     0.2620227   -0.99497575                 1.0E-10    1               15
 BPMS:LI11:701    71321    0.99763334    -0.8581179                 1.0E-10    1               15
 BPMS:LI11:701    71322   -0.96518743     0.8059581                 1.0E-10    1               15
 BPMS:LI11:701    71323    0.13036406     0.3713265                 1.0E-10    1               15
 BPMS:LI11:701    71324    0.17594528   -0.09436919                 1.0E-10    1               15
 BPMS:LI11:701    71325     0.5418064    0.44043896                 1.0E-10    1               15
 BPMS:LI11:701    71326    0.22478646    0.94306743                 1.0E-10    1               15
 BPMS:LI11:701    71327    0.97899467     0.0747191                 1.0E-10    1               15
 BPMS:LI11:701    71328    0.26329628    -0.7129646                 1.0E-10    1               15
 BPMS:LI11:701    71329   -0.33966538   -0.50097287                 1.0E-10    1               15
 BPMS:LI11:701    71330    -0.5830432     0.9801058                 1.0E-10    1               15
 BPMS:LI11:701    71331    -0.8271131     0.7204811                 1.0E-10    1               15
 BPMS:LI11:701    71332    0.23194428      0.727105                 1.0E-10    1               15
 BPMS:LI11:701    71333   -0.02069914    0.93042356                 1.0E-10    1               15
 BPMS:LI11:701    71334   -0.97808135     0.1453952                 1.0E-10    1               15
 BPMS:LI11:701    71335    -0.9578331    -0.2611278                 1.0E-10    1               15
 BPMS:LI11:701    71336     0.5519686     0.4574459                 1.0E-10    1               15
 BPMS:LI11:701    71337    -0.9599065    -0.8594734                 1.0E-10    1               15
 BPMS:LI11:701    71338   0.057467323      0.646338                 1.0E-10    1               15
 BPMS:LI11:701    71339    0.13986315   0.056020267                 1.0E-10    1               15
 BPMS:LI11:701    71340   -0.98957396     0.5655917                 1.0E-10    1               15
 BPMS:LI11:701    71341   -0.96537125     -0.836016                 1.0E-10    1               15
 BPMS:LI11:701    71342    0.20229585    -0.6402634                 1.0E-10    1               15
 BPMS:LI11:701    71343    0.12294944     0.9966622                 1.0E-10    1               15
 BPMS:LI11:701    71344    -0.9422524   -0.72944206                 1.0E-10    1               15
 BPMS:LI11:701    71345   -0.37643722    -0.8602062                 1.0E-10    1               15
 BPMS:LI11:701    71346    0.73458004   0.052228775                 1.0E-10    1               15
 BPMS:LI11:701    71347    0.94135153    -0.7021029                 1.0E-10    1               15
 BPMS:LI11:701    71348    0.11904683     0.2673653                 1.0E-10    1               15
 BPMS:LI11:701    71349     0.0878207     0.3070107                 1.0E-10    1               15
 BPMS:LI11:701    71350     0.9028638   -0.42168748                 1.0E-10    1               15
 BPMS:LI11:701    71351    0.28661087   -0.49076438                 1.0E-10    1               15
 BPMS:LI11:701    71352    -0.5991983     0.3729611                 1.0E-10    1               15
 BPMS:LI11:701    71353     0.1651427    0.06201481                 1.0E-10    1               15
 BPMS:LI11:701    71354    0.62094665   -0.78386164                 1.0E-10    1               15
 BPMS:LI11:701    71355     -0.799097     -0.975341                 1.0E-10    1               15
 BPMS:LI11:701    71356   -0.85525846    -0.4670013                 1.0E-10    1               15
 BPMS:LI11:701    71357    -0.4340582   -0.13011213                 1.0E-10    1               15
 BPMS:LI11:701    71358  -7.247398E-4    -0.9574475                 1.0E-10    1               15
 BPMS:LI11:701    71359   -0.14859082    0.12330404                 1.0E-10    1               15
 BPMS:LI11:701    71360     0.2179347    0.32104412                 1.0E-10    1               15
 BPMS:LI11:701    71361     0.5185018    -0.4449447                 1.0E-10    1               15
 BPMS:LI11:701    71362     0.9727578     0.9421424                 1.0E-10    1               15
 BPMS:LI11:701    71363    0.57660323   -0.94476295                 1.0E-10    1               15
 BPMS:LI11:701    71364    -0.6319889    -0.8826787                 1.0E-10    1               15
 BPMS:LI11:701    71365   -0.58454823   -0.99237275                 1.0E-10    1               15
 BPMS:LI11:701    71366   -0.92139256    0.22906768                 1.0E-10    1               15
 BPMS:LI11:701    71367   -0.07513226    0.06631963                 1.0E-10    1               15
 BPMS:LI11:701    71368     0.9616566    0.87507606                 1.0E-10    1               15
 BPMS:LI11:701    71369    0.50277716   -0.97874105                 1.0E-10    1               15
 BPMS:LI11:701    71370     0.9476361     0.8191101                 1.0E-10    1               15
 BPMS:LI11:701    71371    -0.9871529 -0.0070563033                 1.0E-10    1               15
 BPMS:LI11:701    71372   -0.33246294    -0.8260679                 1.0E-10    1               15
 BPMS:LI11:701    71373   -0.17754315   -0.31769744                 1.0E-10    1               15
 BPMS:LI11:701    71374    0.99007696   -0.99448097                 1.0E-10    1               15
 BPMS:LI11:701    71375    0.10611625    0.23348983                 1.0E-10    1               15
 BPMS:LI11:701    71376    0.24123418   -0.23517086                 1.0E-10    1               15
 BPMS:LI11:701    71377     -0.612764    0.96371114                 1.0E-10    1               15
 BPMS:LI11:701    71378    -0.5847206    -0.9993666                 1.0E-10    1               15
 BPMS:LI11:701    71379   -0.88354343     0.9061117                 1.0E-10    1               15
 BPMS:LI11:701    71380     -0.998581    -0.9808996                 1.0E-10    1               15
 BPMS:LI11:701    71381     0.8954374    -0.9914447                 1.0E-10    1               15
 BPMS:LI11:701    71382   0.027889116    0.96778804                 1.0E-10    1               15
 BPMS:LI11:701    71383      -0.62504    0.11342724                 1.0E-10    1               15
 BPMS:LI11:701    71384    0.27340984    0.96977013                 1.0E-10    1               15
 BPMS:LI11:701    71385    0.18054357    -0.8055469                 1.0E-10    1               15
 BPMS:LI11:701    71386    -0.3713821    0.99498254                 1.0E-10    1               15
 BPMS:LI11:701    71387     0.7324847     0.0983511                 1.0E-10    1               15
 BPMS:LI11:701    71388     0.9625896    0.23490651                 1.0E-10    1               15
 BPMS:LI11:701    71389     0.8422795   -0.99597836                 1.0E-10    1               15
 BPMS:LI11:701    71390   -0.89028883    0.94369787                 1.0E-10    1               15
 BPMS:LI11:701    71391     0.6338151     0.5726365                 1.0E-10    1               15
 BPMS:LI11:701    71392   -0.84290963   -0.99738127                 1.0E-10    1               15
 BPMS:LI11:701    71393   -0.06550807    0.99941486                 1.0E-10    1               15
 BPMS:LI11:701    71394   -0.06257147    0.40770066                 1.0E-10    1               15
 BPMS:LI11:701    71395     0.6971393   -0.33558494                 1.0E-10    1               15
 BPMS:LI11:701    71396   -0.47150645   -0.17381294                 1.0E-10    1               15
 BPMS:LI11:701    71397       0.05999    -0.7920638                 1.0E-10    1               15
 BPMS:LI11:701    71398     0.9929923     0.0967537                 1.0E-10    1               15
 BPMS:LI11:701    71399  -0.017204763    -0.9923933                 1.0E-10    1               15
 BPMS:LI11:701    71400   -0.31027037   -0.82897884                 1.0E-10    1               15
 BPMS:LI11:701    71401    -0.5747528   0.105278395                 1.0E-10    1               15
 BPMS:LI11:701    71402     0.8359465    -0.7532255                 1.0E-10    1               15
 BPMS:LI11:701    71403   -0.48334774     0.9472381                 1.0E-10    1               15
 BPMS:LI11:701    71404      0.937598   -0.09558577                 1.0E-10    1               15
 BPMS:LI11:701    71405     0.5443451    -0.8295011                 1.0E-10    1               15
 BPMS:LI11:701    71406   -0.86666024     0.9825373                 1.0E-10    1               15
 BPMS:LI11:701    71407     0.1282642   -0.99970245                 1.0E-10    1               15
 BPMS:LI11:701    71408     0.4236877    0.76232344                 1.0E-10    1               15
 BPMS:LI11:701    71409 -0.0120343845     0.9999991                 1.0E-10    1               15
 BPMS:LI11:701    71410    -0.6762383    0.73085505                 1.0E-10    1               15
 BPMS:LI11:701    71411    -0.9319905   -0.23064642                 1.0E-10    1               15
 BPMS:LI11:701    71412      0.969152   -0.86345965                 1.0E-10    1               15
 BPMS:LI11:701    71413  -0.062636934     0.8251216                 1.0E-10    1               15
 BPMS:LI11:701    71414   -0.81390405   -0.41767308                 1.0E-10    1               15
 BPMS:LI11:701    71415   -0.63782084     0.9165142                 1.0E-10    1               15
 BPMS:LI11:701    71416    -0.8499817    -0.9508953                 1.0E-10    1               15
 BPMS:LI11:701    71417     0.9832837    -0.9999136                 1.0E-10    1               15
 BPMS:LI11:701    71418   -0.03350008    0.81809855                 1.0E-10    1               15
 BPMS:LI11:701    71419     0.1836207    0.33322185                 1.0E-10    1               15
 BPMS:LI11:701    71420    0.99882823   -0.12267365                 1.0E-10    1               15
 BPMS:LI11:701    71421    0.27395347     0.9999871                 1.0E-10    1               15
 BPMS:LI11:701    71422   -0.31975213     0.3361481                 1.0E-10    1               15
 BPMS:LI11:701    71423    -0.9844917    -0.9503489                 1.0E-10    1               15
 BPMS:LI11:701    71424     0.7150475    0.33607885                 1.0E-10    1               15
 BPMS:LI11:701    71425    0.41068524    -0.8234129                 1.0E-10    1               15
 BPMS:LI11:701    71426   -0.63276297   -0.49515438                 1.0E-10    1               15
 BPMS:LI11:701    71427    -0.8590265    0.81321824                 1.0E-10    1               15
 BPMS:LI11:701    71428    0.29954085    -0.5926448                 1.0E-10    1               15
 BPMS:LI11:701    71429    -0.3527204   -0.84642947                 1.0E-10    1               15
 BPMS:LI11:701    71430    -0.9963928   -0.12546031                 1.0E-10    1               15
 BPMS:LI11:701    71431    0.99473894   -0.98945737                 1.0E-10    1               15
 BPMS:LI11:701    71432      0.918032    0.51426274                 1.0E-10    1               15
 BPMS:LI11:701    71433    0.93999213   -0.94501567                 1.0E-10    1               15
 BPMS:LI11:701    71434    -0.6751422    -0.7191324                 1.0E-10    1               15
 BPMS:LI11:701    71435    -0.8925289      -0.96179                 1.0E-10    1               15
 BPMS:LI11:701    71436     0.4533363   -0.67172146                 1.0E-10    1               15
 BPMS:LI11:701    71437   -0.11054584    0.99855363                 1.0E-10    1               15
 BPMS:LI11:701    71438     0.5012762    -0.7731678                 1.0E-10    1               15
 BPMS:LI11:701    71439    0.99972653   -0.43252224                 1.0E-10    1               15
 BPMS:LI11:701    71440    -0.7747788     0.9731597                 1.0E-10    1               15
 BPMS:LI11:701    71441   -0.46999457    -0.1781048                 1.0E-10    1               15
 BPMS:LI11:701    71442     0.8923079     0.5176484                 1.0E-10    1               15
 BPMS:LI11:701    71443    -0.9838124     0.6184728                 1.0E-10    1               15
 BPMS:LI11:701    71444    -0.3324713    0.12087107                 1.0E-10    1               15
 BPMS:LI11:701    71445    0.81005967     0.8930901                 1.0E-10    1               15
 BPMS:LI11:701    71446    0.78390825   -0.67498654                 1.0E-10    1               15
 BPMS:LI11:701    71447   -0.97786045     -0.981063                 1.0E-10    1               15
 BPMS:LI11:701    71448    -0.7834097     0.5425384                 1.0E-10    1               15
 BPMS:LI11:701    71449    0.18272787     0.4589096                 1.0E-10    1               15
 BPMS:LI11:701    71450     0.8465464     0.6886511                 1.0E-10    1               15
 BPMS:LI11:701    71451     0.7568344      0.998513                 1.0E-10    1               15
 BPMS:LI11:701    71452    0.43048012      0.519975                 1.0E-10    1               15
 BPMS:LI11:701    71453     0.3775714   -0.49787533                 1.0E-10    1               15
 BPMS:LI11:701    71454    0.39383444    0.72175103                 1.0E-10    1               15
 BPMS:LI11:701    71455    0.91773975     0.7360965                 1.0E-10    1               15
 BPMS:LI11:701    71456     0.2550394    0.15667753                 1.0E-10    1               15
 BPMS:LI11:701    71457    0.11280947     0.6965351                 1.0E-10    1               15
 BPMS:LI11:701    71458   -0.10259826    0.85342354                 1.0E-10    1               15
 BPMS:LI11:701    71459    0.31853682     -0.521613                 1.0E-10    1               15
 BPMS:LI11:701    71460    0.77880305   -0.84764427                 1.0E-10    1               15
 BPMS:LI11:701    71461     0.7294757    -0.8325046                 1.0E-10    1               15
 BPMS:LI11:701    71462    -0.4172901    0.12063182                 1.0E-10    1               15
 BPMS:LI11:701    71463     0.9285348   -0.74036014                 1.0E-10    1               15
 BPMS:LI11:701    71464    0.43702307   -0.78969884                 1.0E-10    1               15
 BPMS:LI11:701    71465     0.4106081    -0.6963482                 1.0E-10    1               15
 BPMS:LI11:701    71466    -0.0450015   -0.50888586                 1.0E-10    1               15
 BPMS:LI11:701    71467     0.7117357   -0.24586853                 1.0E-10    1               15
 BPMS:LI11:701    71468   -0.96964604    -0.4980464                 1.0E-10    1               15
 BPMS:LI11:701    71469   -0.71165246 -0.0056696893                 1.0E-10    1               15
 BPMS:LI11:701    71470     0.3888084    0.33188906                 1.0E-10    1               15
 BPMS:LI11:701    71471    -0.3415891   -0.88529676                 1.0E-10    1               15
 BPMS:LI11:701    71472    -0.1812791     0.6336256                 1.0E-10    1               15
 BPMS:LI11:701    71473    -0.9248761    0.75009257                 1.0E-10    1               15
 BPMS:LI11:701    71474     0.8850984    0.83642554                 1.0E-10    1               15
 BPMS:LI11:701    71475   -0.24575344   0.087582774                 1.0E-10    1               15
 BPMS:LI11:701    71476    -0.9681796     0.8853262                 1.0E-10    1               15
 BPMS:LI11:701    71477     0.5960413    -0.6854967                 1.0E-10    1               15
 BPMS:LI11:701    71478      0.999978    -0.9984465                 1.0E-10    1               15
 BPMS:LI11:701    71479   -0.90467286     0.6094263                 1.0E-10    1               15
 BPMS:LI11:701    71480   -0.77350456   0.096877225                 1.0E-10    1               15
 BPMS:LI11:701    71481    0.64573556    0.29902807                 1.0E-10    1               15
 BPMS:LI11:701    71482    0.51446974     -0.669707                 1.0E-10    1               15
 BPMS:LI11:701    71483     0.8442104   -0.66292536                 1.0E-10    1               15
 BPMS:LI11:701    71484   -0.83959454    0.79751205                 1.0E-10    1               15
 BPMS:LI11:701    71485    0.17689215   -0.82969975                 1.0E-10    1               15
 BPMS:LI11:701    71486    -0.9228376    0.71242994                 1.0E-10    1               15
 BPMS:LI11:701    71487    0.44585967    0.79219574                 1.0E-10    1               15
 BPMS:LI11:701    71488    0.99819344     0.7351773                 1.0E-10    1               15
 BPMS:LI11:701    71489     0.4190041     -0.999074                 1.0E-10    1               15
 BPMS:LI11:701    71490    0.23116902   -0.47676393                 1.0E-10    1               15
 BPMS:LI11:801    71311    0.89435065    0.42228273                 1.0E-10    1               15
 BPMS:LI11:801    71312    0.26858768   -0.49921754                 1.0E-10    1               15
 BPMS:LI11:801    71313    0.16995367    -0.6319323                 1.0E-10    1               15
 BPMS:LI11:801    71314   -0.86223084   -0.24007294                 1.0E-10    1               15
 BPMS:LI11:801    71315     0.8112742   -0.33968678                 1.0E-10    1               15
 BPMS:LI11:801    71316   -0.51374173    -0.8000353                 1.0E-10    1               15
 BPMS:LI11:801    71317     0.9416728    -0.9797486                 1.0E-10    1               15
 BPMS:LI11:801    71318    0.08258322    0.97269726                 1.0E-10    1               15
 BPMS:LI11:801    71319   -0.65195835  -0.027599042                 1.0E-10    1               15
 BPMS:LI11:801    71320     0.9997904   -0.39284995                 1.0E-10    1               15
 BPMS:LI11:801    71321   -0.16394024    -0.1276952                 1.0E-10    1               15
 BPMS:LI11:801    71322   -0.48837358   -0.15405476                 1.0E-10    1               15
 BPMS:LI11:801    71323    0.98256516   -0.06321013                 1.0E-10    1               15
 BPMS:LI11:801    71324    -0.6581897   -0.79333305                 1.0E-10    1               15
 BPMS:LI11:801    71325     0.7313524     0.4729435                 1.0E-10    1               15
 BPMS:LI11:801    71326   -0.07567292     0.5966135                 1.0E-10    1               15
 BPMS:LI11:801    71327     0.8292842    -0.9203214                 1.0E-10    1               15
 BPMS:LI11:801    71328    -0.7912196   -0.39477772                 1.0E-10    1               15
 BPMS:LI11:801    71329   -0.59834546    -0.8403268                 1.0E-10    1               15
 BPMS:LI11:801    71330   -0.97843134    0.49473315                 1.0E-10    1               15
 BPMS:LI11:801    71331    -0.8826865    -0.6786495                 1.0E-10    1               15
 BPMS:LI11:801    71332   0.013729391     0.9995007                 1.0E-10    1               15
 BPMS:LI11:801    71333   -0.72936463     0.8591405                 1.0E-10    1               15
 BPMS:LI11:801    71334     0.8460996    -0.9072279                 1.0E-10    1               15
 BPMS:LI11:801    71335    0.84780425   -0.22025795                 1.0E-10    1               15
 BPMS:LI11:801    71336    -0.9833736    0.58158827                 1.0E-10    1               15
 BPMS:LI11:801    71337    0.45466614     0.5632273                 1.0E-10    1               15
 BPMS:LI11:801    71338    -0.7975792     0.3959225                 1.0E-10    1               15
 BPMS:LI11:801    71339   -0.95982236    0.72619116                 1.0E-10    1               15
 BPMS:LI11:801    71340    0.77490205  -0.010913516                 1.0E-10    1               15
 BPMS:LI11:801    71341    -0.9618494   -0.16358538                 1.0E-10    1               15
 BPMS:LI11:801    71342   -0.99945766    0.17009634                 1.0E-10    1               15
 BPMS:LI11:801    71343    -0.9827655     -0.538309                 1.0E-10    1               15
 BPMS:LI11:801    71344     0.5836144     -0.973655                 1.0E-10    1               15
 BPMS:LI11:801    71345     0.5827458    0.15916738                 1.0E-10    1               15
 BPMS:LI11:801    71346    0.89338857   -0.95811766                 1.0E-10    1               15
 BPMS:LI11:801    71347   -0.71327746    -0.6840108                 1.0E-10    1               15
 BPMS:LI11:801    71348     0.6634452    0.51829803                 1.0E-10    1               15
 BPMS:LI11:801    71349    -0.9992053    0.37212232                 1.0E-10    1               15
 BPMS:LI11:801    71350    0.94293845    0.93999046                 1.0E-10    1               15
 BPMS:LI11:801    71351   0.048516557    -0.7512361                 1.0E-10    1               15
 BPMS:LI11:801    71352   -0.09201102    -0.9082222                 1.0E-10    1               15
 BPMS:LI11:801    71353     0.9287366   0.049854636                 1.0E-10    1               15
 BPMS:LI11:801    71354     0.9952483   -0.89061016                 1.0E-10    1               15
 BPMS:LI11:801    71355     0.9819817    -0.3987979                 1.0E-10    1               15
 BPMS:LI11:801    71356   -0.84245104    0.73029196                 1.0E-10    1               15
 BPMS:LI11:801    71357     0.3326127    0.99238545                 1.0E-10    1               15
 BPMS:LI11:801    71358    -0.5067468    -0.5285413                 1.0E-10    1               15
 BPMS:LI11:801    71359   -0.50997305    -0.9996577                 1.0E-10    1               15
 BPMS:LI11:801    71360     0.9809931    0.49830022                 1.0E-10    1               15
 BPMS:LI11:801    71361   -0.76547205     0.9132482                 1.0E-10    1               15
 BPMS:LI11:801    71362     0.9596802    0.98324394                 1.0E-10    1               15
 BPMS:LI11:801    71363     0.3049223     0.9310298                 1.0E-10    1               15
 BPMS:LI11:801    71364   -0.79781497   -0.46650967                 1.0E-10    1               15
 BPMS:LI11:801    71365     -0.557484   -0.90137845                 1.0E-10    1               15
 BPMS:LI11:801    71366     0.9994231    0.98865145                 1.0E-10    1               15
 BPMS:LI11:801    71367     -0.346725    -0.9998914                 1.0E-10    1               15
 BPMS:LI11:801    71368     0.5865337    0.99361914                 1.0E-10    1               15
 BPMS:LI11:801    71369    0.74243563    0.95726854                 1.0E-10    1               15
 BPMS:LI11:801    71370    0.26574495    0.83564436                 1.0E-10    1               15
 BPMS:LI11:801    71371    0.66718984     0.6458388                 1.0E-10    1               15
 BPMS:LI11:801    71372   -0.98929924   -0.34036565                 1.0E-10    1               15
 BPMS:LI11:801    71373   -0.33993322     0.9204205                 1.0E-10    1               15
 BPMS:LI11:801    71374   -0.43864712    0.27368316                 1.0E-10    1               15
 BPMS:LI11:801    71375  -0.082056314     0.9199434                 1.0E-10    1               15
 BPMS:LI11:801    71376    0.77168196     0.7650966                 1.0E-10    1               15
 BPMS:LI11:801    71377     0.9976097    -0.8848359                 1.0E-10    1               15
 BPMS:LI11:801    71378  -0.058578264  -0.017835869                 1.0E-10    1               15
 BPMS:LI11:801    71379     -0.787832     0.9999145                 1.0E-10    1               15
 BPMS:LI11:801    71380     0.1597066    0.20444652                 1.0E-10    1               15
 BPMS:LI11:801    71381   -0.83095723   -0.79484105                 1.0E-10    1               15
 BPMS:LI11:801    71382     0.8848067   -0.50235146                 1.0E-10    1               15
 BPMS:LI11:801    71383    0.45676258    -0.9313255                 1.0E-10    1               15
 BPMS:LI11:801    71384     0.9882627   -0.75926906                 1.0E-10    1               15
 BPMS:LI11:801    71385   -0.26749048   -0.01415549                 1.0E-10    1               15
 BPMS:LI11:801    71386     0.9999937    0.08580772                 1.0E-10    1               15
 BPMS:LI11:801    71387     0.9245617      0.931424                 1.0E-10    1               15
 BPMS:LI11:801    71388    0.95004416    0.99807155                 1.0E-10    1               15
 BPMS:LI11:801    71389    -0.9556885      0.281221                 1.0E-10    1               15
 BPMS:LI11:801    71390    -0.9976731    -0.9936181                 1.0E-10    1               15
 BPMS:LI11:801    71391     0.6079601    0.35955784                 1.0E-10    1               15
 BPMS:LI11:801    71392    0.05167483      -0.69548                 1.0E-10    1               15
 BPMS:LI11:801    71393    0.41428426   -0.87439543                 1.0E-10    1               15
 BPMS:LI11:801    71394    -0.9235464  -0.115970895                 1.0E-10    1               15
 BPMS:LI11:801    71395     0.6447669     0.6649088                 1.0E-10    1               15
 BPMS:LI11:801    71396   -0.58189213     0.9556188                 1.0E-10    1               15
 BPMS:LI11:801    71397    0.80456746   -0.97047335                 1.0E-10    1               15
 BPMS:LI11:801    71398   -0.10189835   -0.29823402                 1.0E-10    1               15
 BPMS:LI11:801    71399    0.99999774   -0.70556444                 1.0E-10    1               15
 BPMS:LI11:801    71400     0.6172779     0.9999659                 1.0E-10    1               15
 BPMS:LI11:801    71401   -0.32284892    -0.9348692                 1.0E-10    1               15
 BPMS:LI11:801    71402   -0.15680344 -0.0128904255                 1.0E-10    1               15
 BPMS:LI11:801    71403   -0.75709563    -0.8046828                 1.0E-10    1               15
 BPMS:LI11:801    71404    -0.5191204     0.1819421                 1.0E-10    1               15
 BPMS:LI11:801    71405    -0.9412438    -0.9198878                 1.0E-10    1               15
 BPMS:LI11:801    71406     0.9774116     -0.583923                 1.0E-10    1               15
 BPMS:LI11:801    71407   -0.79585284    0.88172805                 1.0E-10    1               15
 BPMS:LI11:801    71408  -0.121187635     0.5003244                 1.0E-10    1               15
 BPMS:LI11:801    71409   -0.04069852   -0.75186056                 1.0E-10    1               15
 BPMS:LI11:801    71410   -0.60402197   -0.19327351                 1.0E-10    1               15
 BPMS:LI11:801    71411   -0.78261304    -0.8268955                 1.0E-10    1               15
 BPMS:LI11:801    71412  -0.009014834    -0.9590776                 1.0E-10    1               15
 BPMS:LI11:801    71413    0.93796253      0.813238                 1.0E-10    1               15
 BPMS:LI11:801    71414    0.26975223     0.5034259                 1.0E-10    1               15
 BPMS:LI11:801    71415    0.99612415   -0.33007511                 1.0E-10    1               15
 BPMS:LI11:801    71416    0.49333856   -0.99883026                 1.0E-10    1               15
 BPMS:LI11:801    71417   -0.98793393   -0.19577287                 1.0E-10    1               15
 BPMS:LI11:801    71418    -0.5340337    0.83074516                 1.0E-10    1               15
 BPMS:LI11:801    71419    -0.9221741   -0.99627036                 1.0E-10    1               15
 BPMS:LI11:801    71420     0.5138918    -0.7285111                 1.0E-10    1               15
 BPMS:LI11:801    71421   -0.25980946    -0.7943612                 1.0E-10    1               15
 BPMS:LI11:801    71422    0.42427543     0.9728706                 1.0E-10    1               15
 BPMS:LI11:801    71423    -0.8772104     0.8812384                 1.0E-10    1               15
 BPMS:LI11:801    71424    0.89471793    0.51343954                 1.0E-10    1               15
 BPMS:LI11:801    71425   -0.13222852   -0.47842434                 1.0E-10    1               15
 BPMS:LI11:801    71426     0.5934242    -0.8273297                 1.0E-10    1               15
 BPMS:LI11:801    71427   -0.99987656   -0.47478265                 1.0E-10    1               15
 BPMS:LI11:801    71428   -0.98725027    0.80059946                 1.0E-10    1               15
 BPMS:LI11:801    71429   -0.93446195    -0.8200869                 1.0E-10    1               15
 BPMS:LI11:801    71430   -0.85800785     0.7724226                 1.0E-10    1               15
 BPMS:LI11:801    71431      0.480128     0.9865015                 1.0E-10    1               15
 BPMS:LI11:801    71432    0.67495555     -0.271307                 1.0E-10    1               15
 BPMS:LI11:801    71433    -0.6420991    0.35679758                 1.0E-10    1               15
 BPMS:LI11:801    71434     0.7555356    0.99461836                 1.0E-10    1               15
 BPMS:LI11:801    71435     0.9996687    0.44496346                 1.0E-10    1               15
 BPMS:LI11:801    71436    0.92083657   -0.33536246                 1.0E-10    1               15
 BPMS:LI11:801    71437    -0.9977074    -0.9967292                 1.0E-10    1               15
 BPMS:LI11:801    71438   -0.96591276   -0.50975174                 1.0E-10    1               15
 BPMS:LI11:801    71439     0.7755726   -0.58402926                 1.0E-10    1               15
 BPMS:LI11:801    71440     0.6999035    0.96974415                 1.0E-10    1               15
 BPMS:LI11:801    71441  -0.069457136    0.91758716                 1.0E-10    1               15
 BPMS:LI11:801    71442    0.96928173    -0.7715745                 1.0E-10    1               15
 BPMS:LI11:801    71443    -0.9935889   -0.37441072                 1.0E-10    1               15
 BPMS:LI11:801    71444     0.9240675    -0.9936523                 1.0E-10    1               15
 BPMS:LI11:801    71445   -0.07101745   -0.58338326                 1.0E-10    1               15
 BPMS:LI11:801    71446   -0.88886356   0.041708585                 1.0E-10    1               15
 BPMS:LI11:801    71447   -0.31157422   -0.95528716                 1.0E-10    1               15
 BPMS:LI11:801    71448    -0.0804906    -0.9025785                 1.0E-10    1               15
 BPMS:LI11:801    71449   -0.35343382    -0.6960443                 1.0E-10    1               15
 BPMS:LI11:801    71450    -0.9985474    -0.6425677                 1.0E-10    1               15
 BPMS:LI11:801    71451    -0.9999987    -0.7532709                 1.0E-10    1               15
 BPMS:LI11:801    71452   -0.67522043    -0.5499586                 1.0E-10    1               15
 BPMS:LI11:801    71453    0.79688114   -0.68232673                 1.0E-10    1               15
 BPMS:LI11:801    71454    -0.9606662   -0.92405194                 1.0E-10    1               15
 BPMS:LI11:801    71455    0.81591785    -0.9974173                 1.0E-10    1               15
 BPMS:LI11:801    71456    0.55294305    -0.9253695                 1.0E-10    1               15
 BPMS:LI11:801    71457   -0.98148495    -0.0874502                 1.0E-10    1               15
 BPMS:LI11:801    71458   -0.16345327    0.31773445                 1.0E-10    1               15
 BPMS:LI11:801    71459   -0.50558794    0.18323605                 1.0E-10    1               15
 BPMS:LI11:801    71460    -0.0712564     0.9540606                 1.0E-10    1               15
 BPMS:LI11:801    71461    -0.6819829    -0.3917621                 1.0E-10    1               15
 BPMS:LI11:801    71462    -0.9408379   -0.92900884                 1.0E-10    1               15
 BPMS:LI11:801    71463   -0.25048372   -0.72002137                 1.0E-10    1               15
 BPMS:LI11:801    71464    0.19868931    -0.5987871                 1.0E-10    1               15
 BPMS:LI11:801    71465     0.9763241   -0.26771992                 1.0E-10    1               15
 BPMS:LI11:801    71466    -0.9432121  -0.080954045                 1.0E-10    1               15
 BPMS:LI11:801    71467   0.057041306     0.8933088                 1.0E-10    1               15
 BPMS:LI11:801    71468   -0.88161457    0.34056434                 1.0E-10    1               15
 BPMS:LI11:801    71469    0.31664434    -0.6366775                 1.0E-10    1               15
 BPMS:LI11:801    71470    -0.9035179    -0.9831505                 1.0E-10    1               15
 BPMS:LI11:801    71471   -0.24777895    -0.5946402                 1.0E-10    1               15
 BPMS:LI11:801    71472     0.7156164    0.96782774                 1.0E-10    1               15
 BPMS:LI11:801    71473    0.34632918    -0.9583771                 1.0E-10    1               15
 BPMS:LI11:801    71474     0.7945797    0.81145275                 1.0E-10    1               15
 BPMS:LI11:801    71475    0.54216427     0.9771044                 1.0E-10    1               15
 BPMS:LI11:801    71476    -0.8035533    0.60033405                 1.0E-10    1               15
 BPMS:LI11:801    71477      0.962712     0.7346848                 1.0E-10    1               15
 BPMS:LI11:801    71478     0.3023123  -0.119504735                 1.0E-10    1               15
 BPMS:LI11:801    71479    -0.5578731     0.7458844                 1.0E-10    1               15
 BPMS:LI11:801    71480    0.34101534    -0.9830838                 1.0E-10    1               15
 BPMS:LI11:801    71481  -0.009555249   -0.99415016                 1.0E-10    1               15
 BPMS:LI11:801    71482     0.9988799   -0.03938037                 1.0E-10    1               15
 BPMS:LI11:801    71483     0.8120967   -0.59533066                 1.0E-10    1               15
 BPMS:LI11:801    71484   -0.46456987     -0.989617                 1.0E-10    1               15
 BPMS:LI11:801    71485    0.69944674     0.9992312                 1.0E-10    1               15
 BPMS:LI11:801    71486     0.9727733    -0.8812482                 1.0E-10    1               15
 BPMS:LI11:801    71487   -0.66673774    -0.9923903                 1.0E-10    1               15
 BPMS:LI11:801    71488    0.89928526    -0.5296994                 1.0E-10    1               15
 BPMS:LI11:801    71489     0.6919425   -0.98034155                 1.0E-10    1               15
 BPMS:LI11:801    71490   -0.09254324    -0.9999214                 1.0E-10    1               15

████ Test 3: Bad argument Name - superfish=7
_________________________________________________

get: NDRFACET:BUFFACQ (superfish=7, BPMD=57)
 NDRFACET:BUFFACQ:  SUPERFISH is not a valid argument for get requests to this channel. Valid arguments are: [DEVS, BPMD, BPMS, NRPOS] ✔

████ Test 4: Bad argument value BPM1=BPMS:GREG:DOESNTEXIST
_________________________________________________

get: NDRFACET:BUFFACQ (BPMD=57, BPMS=[BPMS:GREG:DOESNTEXIST])
 UnableToGetDataException; Device name is not valid ✔
```


