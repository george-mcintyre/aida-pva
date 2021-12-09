# 1.3 - SLC Bpm Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Bpm data through AIDA-PVA. For general
information on using AIDA-PVA see [Basic Users Guide to Aida](1_00_User_Guide.md), and the EPICS javadoc.

## Summary

Aquires SLC BPM orbit data, including the `name`, `x` (mm), `y` (mm), `tmit` (num particles), `z` position (m), `HSTA`
and `STAT` of each bpm in a given DGRP, under a given measurement definition.

## Instances and Attributes

| operation  | info        | instance & attribute        |
|---------|----------|------------------------|
| **get** | Syntax   | `<dgrp-mnemonic>:BPMS` |
|         | Examples | `P2BPMHER:BPMS`        |

## Attribute operation summary

| Attribute | Description                                                                                                                                                         |
|-----------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `BPMS`    | Makes a BPM reading of every BPM in the `dgrp` specified in the INSTANCE part of the query, under the BPM Measurement Definition specified by the `BPMD` parameter. |

## Attribute operations

### BPMS : get

_Parameters_

| Parameter Names | Parameter Values                          | Description                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         | 
|-----------------|-------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `BPMD`*         | Integer Bpm Measurement Definition number | This specifies the timing profile of the acquisition.<br /> Each BPMD corresponds to exactly one DGRP. To find<br /> likely BPMDs, go to the BPM Device panel of a SCP<br /> and hit the 'HELP' button, then select the button<br /> corresponding to the bpm orbit you would have wanted<br /> to see. The button name for that acquisition, as displayed<br /> in the help, is the BPMD for the selected orbit. Use<br /> that number for this parameter. Eg, for 'HER Bunch<br /> train' BPMD=38. |
| `CNFTYPE`       | `NONE`                                    | Absolute orbit. This is the default                                                                                                                                                                                                                                                                                                                                                                                                                                                                 |
|                 | `GOLD`                                    | Diff to the golden config                                                                                                                                                                                                                                                                                                                                                                                                                                                                           |
|                 | `NORMAL`                                  | Diff to the normal config whose number must be given<br /> in the accompanying `CNFNUM` parameter                                                                                                                                                                                                                                                                                                                                                                                                   |
|                 | `SCRATCH`                                 | Diff to the scratch config whose number must be given<br /> in the accompanying `CNFNUM` parameter                                                                                                                                                                                                                                                                                                                                                                                                  |
|                 | `LOADED`                                  | Diff to the temp config whose number must be given<br /> in the accompanying `CNFNUM` parameter                                                                                                                                                                                                                                                                                                                                                                                                     |
|                 | `TEMPORARY`                               | Diff to the last loaded config from any process, including<br /> another SCP                                                                                                                                                                                                                                                                                                                                                                                                                        |
| `CNFNUM`        | +ve integer                               | The number of the bpm device configuration, to whose<br /> values you want the difference to the present readings.<br /> See a SCP bpm config panel for a list of the present<br /> bpm configs                                                                                                                                                                                                                                                                                                     |
| `N`             | `1 =< n >= 10000`                         | Default is 1. The "NAvg" argument. If the BPMD defines<br /> a ring, then `N` is interpreted as the number of turns;<br /> if BPMD defines a transport, then `N` is interpreted<br /> as readings to average (in the processor)                                                                                                                                                                                                                                                                     |
| `SORTORDER`     | `1` or `2`                                | Default is 2. This refers to the order of the returned<br /> BPMs. In PEPII, the z position of BPMS is such that<br /> the greatest and least z (one complete turn) breaks<br /> on a micro boundary, before the micro in charge of<br /> the injection region. `SORTORDER`=`1` gives the BPMs<br /> in this order, as is used in SCP steering displays.<br /> `SORTORDER`=`2` is that used in SCP BPM device panel<br /> displays, and is oriented towards displaying injection<br /> to injection |

_Return value_

| TYPE    | Return Column | Column Type     | Description                                                                        |
|---------|---------------|-----------------|------------------------------------------------------------------------------------|
| `TABLE` | `name`        | `STRING_ARRAY`  | name of each BPM                                                                   |
|         | `x`           | `FLOAT_ARRAY`   | x offsets (mm)                                                                     |
|         | `y`           | `FLOAT_ARRAY`   | y offsets (mm)                                                                     |
|         | `tmits`       | `FLOAT_ARRAY`   | tmits (num particles)                                                              |
|         | `z`           | `FLOAT_ARRAY`   | z positions (meters)                                                               |
|         | `hsta`        | `INTEGER_ARRAY` | (32 bit field); to interpret see VMS help: `MCCDEV><br /> help @slchelp prim bpms` |
|         | `stat`        | `INTEGER_ARRAY` | (32 bit field)                                                                     |

## Examples

@note For general details about accessing AIDA-PVA from matlab see [Matlab Coding](1_12_Matlab_Code.md) 

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">example type</th><th class="markdownTableHeadNone">action</th><th class="markdownTableHeadNone">example</th></tr>
<td class="markdownTableBodyNone">commandline **pvcall**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```shell
pvcall "P2BPMHER:BPMS" BPMD=38 CNFTYPE=GOLD N=1024
```
</td>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">commandline **eget**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```shell
eget -s P2BPMHER:BPMS -a BPMD 38 -a CNFTYPE GOLD -a N 1024 
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">java **AidaPvaClient**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```java
import org.epics.pvaccess.server.rpc.RPCRequestException;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.*;
import static edu.stanford.slac.aida.client.AidaType.*;

public class AidaPvaClientExample {
    public PvaTable getTable() throws RPCException {
        return pvaRequest("P2BPMHER:BPMS")
                .with("BPMD", 38)
                .with("CNFTYPE", "GOLD")
                .with("N", 1024)
                .get();
    }
}
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">java **PvaClient**</td>
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
        String pvName = "P2BPMHER:BPMS";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"bpmd", "cnftype", "n"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvShort),
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvInt)
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
        args.getShortField("bpmd").put(38);
        args.getStringField("cnftype").put("GOLD");
        args.getIntField("n").put(1024);

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        PVStructure response = channel.rpc(request);

        return response;
    }
}
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">java **EasyPVA**</td>
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
        String pvName = "P2BPMHER:BPMS";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"bpmd", "cnftype", "n"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvShort),
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvInt)
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
        args.getShortField("bpmd").put(38);
        args.getStringField("cnftye").put("GOLD");
        args.getIntField("n").put(1024);

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
<td class="markdownTableBodyNone">java **PvAccess**</td>
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
    public PVStructure getTable() throws RPCRequestException {
        String pvName = "P2BPMHER:BPMS";

        Structure arguments = fieldCreate.createStructure(
                new String[]{"bpmd", "cnftype", "n"},
                new Field[]{
                        fieldCreate.createScalar(ScalarType.pvShort),
                        fieldCreate.createScalar(ScalarType.pvString),
                        fieldCreate.createScalar(ScalarType.pvInt)
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
        args.getShortField("bpmd").put(38);
        args.getStringField("cnftype").put("GOLD");
        args.getIntField("n").put(1024);
        
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
<td class="markdownTableBodyNone">matlab **AidaPvaClient**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">


```matlab
aidainit
try
    table = pvaRequest('P2BPMHER:BPMS').with("BPMD", 38).with("cnftype", "gold").with("n", 1024).get();
    labels = table.getLabels();
    values = table.getValues();
    names = values.get('name');
catch ME
    % do something when errors occur or just show ME.identifier
end
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">matlab **PvaClient**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```matlab
aidainit
table = pvarpc(nturi('P2BPMHER:BPMS', 'BPMD', '38', 'CNFTYPE', 'GOLD', 'N', '1024'));
tableStruct = nttable2struct(table);
labels = tableStruct.labels;
names = tableStruct.value.name;
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">matlab **EasyPVA**</td>
<td class="markdownTableBodyNone">Get</td>

<td class="markdownTableBodyNone">

```matlab
aidainit
table = ezrpc(nturi('P2BPMHER:BPMS', 'BPMD', '38', 'CNFTYPE', 'GOLD', 'N', '1024'));
tableStruct = nttable2struct(table);
labels = tableStruct.labels;
names = tableStruct.value.name;
```

</td>
</tr>

</table>

## Test Output

Tests are implemented in: SlcBpmTest.java

```shell
java -cp aida-pva-tests.jar  "edu.stanford.slac.aida.test.SlcBpmTest" -c 1 2 3 8 10
#################################################
AIDA-PVA SLC BPM TESTS
#################################################

████ Test 1: All values of all BPMs of LCLS dgrp under meas def 55, default options
_________________________________________________

get: LCLS_SL2:BPMS (BPMD=55)
    BPM Values:  123 rows retrieved: ✔
       BPM Name x offset (mm) y offset (mm) num particles (coulomb) z position (meters) HSTA STATS
           name             x             y                   tmits                   z HSTA STATS
  BPMS:IA20:221   0.061334226  -0.061334226                     0.0           2015.5988    4     0
  BPMS:IA20:235     1.0534176    -1.0534176                     0.0           2015.9229    4     0
  BPMS:IA20:371   0.029374985  -0.029374985                     0.0           2019.8888    4     0
  BPMS:IA20:425    -0.9041202     0.9041202                     0.0           2023.1998    4     0
  BPMS:IA20:511    0.07792781   -0.07792781                     0.0           2026.2728    4     0
  BPMS:IB20:525      1.096979     -1.096979                     0.0           2026.6798    4     0
  BPMS:IB20:581    0.05345309   -0.05345309                     0.0           2029.1068    4     0
  BPMS:IB20:631    -0.9330085     0.9330085                     0.0           2031.0618    4     0
  BPMS:IB20:651    0.07530291   -0.07530291                     0.0           2031.6168    4     0
  BPMS:IB20:731      1.049553     -1.049553                     0.0           2032.8788    4     0
  BPMS:IB20:771     0.0728055    -0.0728055                     0.0           2034.2788    4     0
  BPMS:IB20:781   -0.92077863    0.92077863                     0.0           2034.6838    4     0
  BPMS:LI21:201   0.008002042  -0.008002042                     0.0            2045.434   33     0
  BPMS:LI21:301     1.0021266    -1.0021266                     0.0            2057.861    1     0
  BPMS:LI21:401   0.092580825  -0.092580825                     0.0            2068.953    1     0
  BPMS:LI21:501    -0.9195405     0.9195405                     0.0            2081.297    1     0
  BPMS:LI21:601    0.03972704   -0.03972704                     0.0           2093.6416    1     0
  BPMS:LI21:701     1.0435222    -1.0435222                     0.0            2105.986    1     0
  BPMS:LI21:801   0.067538366  -0.067538366                     0.0           2118.3303    1     0
  BPMS:LI21:901    -0.9854073     0.9854073                     0.0            2130.994    1     0
  BPMS:LA21:131   0.039337922  -0.039337922                     0.0           2037.9948    1     0
  BPMS:LA21:161     1.0362937    -1.0362937                     0.0           2041.0358    1     0
  BPMS:LA21:201    0.02759437   -0.02759437                     0.0           2045.4338    1     0
  BPMS:LA21:233    -0.9627801     0.9627801                     0.0            2049.178    1     0
  BPMS:LA21:278   0.043459445  -0.043459445                     0.0           2053.6492    1     0
  BPMS:LA21:301     1.0733495    -1.0733495                     0.0            2057.861    1     0
  BPMS:LA21:315    0.09538827   -0.09538827                     0.0            2059.017    1     0
  BPMS:LB21:401    -0.9377013     0.9377013                     0.0            2068.953    4     0
  BPMS:LB21:501    0.03010131   -0.03010131                     0.0            2081.297    4     0
  BPMS:LB21:601     1.0607538    -1.0607538                     0.0           2093.6416    4     0
  BPMS:LB21:701    0.09396982   -0.09396982                     0.0            2105.986    4     0
  BPMS:LB21:801    -0.9085273     0.9085273                     0.0           2118.3303    4     0
  BPMS:LB21:901   0.014128995  -0.014128995                     0.0            2130.994    4     0
  BPMS:LI22:201     1.0233023    -1.0233023                     0.0            2145.864    1     0
  BPMS:LI22:301    0.08739759   -0.08739759                     0.0           2158.2085    1     0
  BPMS:LI22:401   -0.90789545    0.90789545                     0.0            2170.553    1     0
  BPMS:LI22:501   0.020233572  -0.020233572                     0.0           2182.8972    1     0
  BPMS:LI22:601     1.0407976    -1.0407976                     0.0           2195.2417    1     0
  BPMS:LI22:701   0.059146486  -0.059146486                     0.0           2207.5862    1     0
  BPMS:LI22:801    -0.9044051     0.9044051                     0.0           2219.9304    1     0
  BPMS:LI22:901   0.090297565  -0.090297565                     0.0            2232.594    1     0
  BPMS:LI23:201     1.0318882    -1.0318882                     0.0           2247.4639    1     0
  BPMS:LI23:301   0.074871965  -0.074871965                     0.0           2259.8083    1     0
  BPMS:LI23:401    -0.9016314     0.9016314                     0.0           2272.1528    1     0
  BPMS:LI23:501   0.033956554  -0.033956554                     0.0            2284.497    1     0
  BPMS:LI23:601     1.0673785    -1.0673785                     0.0           2296.8416    1     0
  BPMS:LI23:701   0.078889094  -0.078889094                     0.0            2309.186    1     0
  BPMS:LI23:801    -0.9262368     0.9262368                     0.0           2321.5303    1     0
  BPMS:LI23:901   0.010837052  -0.010837052                     0.0           2334.1938    1     0
  BPMS:LA24:801     1.0463425    -1.0463425                     0.0           2424.7297    4     0
  BPMS:LI24:201    0.08842223   -0.08842223                     0.0            2349.064    1     0
  BPMS:LI24:301    -0.9497348     0.9497348                     0.0           2361.4084    1     0
  BPMS:LI24:401    0.08256736   -0.08256736                     0.0            2373.753    1     0
  BPMS:LI24:501     1.0159211    -1.0159211                     0.0           2386.0972    1     0
  BPMS:LI24:601    0.08755674   -0.08755674                     0.0           2398.4417    1     0
  BPMS:LI24:701   -0.97387236    0.97387236                     0.0            2411.129    1     0
  BPMS:LI24:801    0.08919624   -0.08919624                     0.0             2424.73    1     0
  BPMS:LI24:901     1.0828389    -1.0828389                     0.0            2437.849    1     0
  BPMS:LA25:201    0.08850329   -0.08850329                     0.0            2450.664    4     0
  BPMS:LA25:601     -0.980591      0.980591                     0.0           2500.0415    4     0
  BPMS:LI25:201   0.043513138  -0.043513138                     0.0           2450.6638    1     0
  BPMS:LI25:301     1.0823563    -1.0823563                     0.0           2463.0083    1     0
  BPMS:LI25:401   0.010963948  -0.010963948                     0.0           2475.3528    1     0
  BPMS:LI25:501    -0.9422358     0.9422358                     0.0            2487.697    1     0
  BPMS:LI25:601  0.0055737877 -0.0055737877                     0.0           2500.0415    1     0
  BPMS:LI25:701     1.0982342    -1.0982342                     0.0            2512.386    1     0
  BPMS:LI25:801    0.04995634   -0.04995634                     0.0           2524.7302    1     0
  BPMS:LI25:901   -0.97405994    0.97405994                     0.0           2537.3938    1     0
  BPMS:LI26:201   0.038941547  -0.038941547                     0.0            2552.264    1     0
  BPMS:LI26:301     1.0089648    -1.0089648                     0.0           2564.6084    1     0
  BPMS:LI26:401    0.02162777   -0.02162777                     0.0            2576.953    1     0
  BPMS:LI26:501    -0.9706176     0.9706176                     0.0            2589.297    1     0
  BPMS:LI26:601   0.040757608  -0.040757608                     0.0           2601.6416    1     0
  BPMS:LI26:701     1.0963511    -1.0963511                     0.0            2613.986    1     0
  BPMS:LI26:801   0.027849197  -0.027849197                     0.0           2626.3303    1     0
  BPMS:LI26:901    -0.9251319     0.9251319                     0.0            2638.994    1     0
  BPMS:LA27:301    0.06362878   -0.06362878                     0.0           2666.2085    4     0
  BPMS:LA27:401      1.006579     -1.006579                     0.0            2678.553    4     0
  BPMS:LB27:701   0.048734758  -0.048734758                     0.0           2715.5862    4     0
  BPMS:LB27:801    -0.9253696     0.9253696                     0.0           2727.9304    4     0
  BPMS:LI27:201   0.052815482  -0.052815482                     0.0            2653.864    1     0
  BPMS:LI27:301     1.0369872    -1.0369872                     0.0           2666.2085    1     0
  BPMS:LI27:401    0.02500435   -0.02500435                     0.0            2678.553    1     0
  BPMS:LI27:501     -0.964442      0.964442                     0.0           2690.8972    1     0
  BPMS:LI27:601   0.052796785  -0.052796785                     0.0           2703.2417    1     0
  BPMS:LI27:701     1.0123806    -1.0123806                     0.0           2715.5862    1     0
  BPMS:LI27:801   0.061799716  -0.061799716                     0.0           2727.9304    1     0
  BPMS:LI27:901    -0.9578212     0.9578212                     0.0            2740.594    1     0
  BPMS:LA28:301   0.095102765  -0.095102765                     0.0           2767.8083    4     0
  BPMS:LA28:401     1.0501119    -1.0501119                     0.0           2780.1528    4     0
  BPMS:LB28:701   0.061707187  -0.061707187                     0.0            2817.186    4     0
  BPMS:LB28:801    -0.9611877     0.9611877                     0.0           2829.5303    4     0
  BPMS:LI28:201   0.032346245  -0.032346245                     0.0           2755.4639    1     0
  BPMS:LI28:301     1.0724695    -1.0724695                     0.0           2767.8083    1     0
  BPMS:LI28:401    0.09670118   -0.09670118                     0.0           2780.1528    1     0
  BPMS:LI28:501     -0.961873      0.961873                     0.0            2792.497    1     0
  BPMS:LI28:601    0.07057626   -0.07057626                     0.0           2804.8416    1     0
  BPMS:LI28:701     1.0464453    -1.0464453                     0.0            2817.186    1     0
  BPMS:LI28:801    0.06419708   -0.06419708                     0.0           2829.5303    1     0
  BPMS:LI28:901    -0.9902646     0.9902646                     0.0           2842.1938    1     0
  BPMS:LI29:201   0.055277396  -0.055277396                     0.0            2857.064    1     0
  BPMS:LI29:301     1.0856019    -1.0856019                     0.0           2869.4084    1     0
  BPMS:LI29:401    0.03925314   -0.03925314                     0.0            2881.753    1     0
  BPMS:LI29:501   -0.90373677    0.90373677                     0.0           2894.0972    1     0
  BPMS:LI29:601   0.081815094  -0.081815094                     0.0           2906.4417    1     0
  BPMS:LI29:701     1.0668688    -1.0668688                     0.0           2918.7861    1     0
  BPMS:LI29:801    0.07127192   -0.07127192                     0.0           2931.1304    1     0
  BPMS:LI29:901    -0.9543173     0.9543173                     0.0            2943.794    1     0
  BPMS:LI30:201    0.07330461   -0.07330461                     0.0           2958.6638    4     0
  BPMS:LI30:301     1.0197625    -1.0197625                     0.0           2971.0083    1     0
  BPMS:LI30:401      0.020459     -0.020459                     0.0           2983.3528    1     0
  BPMS:LI30:501    -0.9736305     0.9736305                     0.0            2995.697    1     0
  BPMS:LI30:601    0.05660111   -0.05660111                     0.0           3007.8398    1     0
  BPMS:LI30:701     1.0452086    -1.0452086                     0.0              3020.2    4     0
  BPMS:LI30:801    0.06207876   -0.06207876                     0.0           3032.5298    1     0
 BPMS:LI30:4001     -0.990342      0.990342                     0.0           3046.3264   33     0
   BPMS:CA11:29   0.057435345  -0.057435345                     0.0           3062.6152   33     0
   BPMS:CA11:39     1.0236131    -1.0236131                     0.0            3067.157   33     0
   BPMS:CA11:52    0.05199328   -0.05199328                     0.0            3073.737 2593     0
   BPMS:CA11:56   -0.94719124    0.94719124                     0.0           3076.4597  545     0
   BPMS:BA00:29   0.073565856  -0.073565856                     0.0           3062.6152    4     0
   BPMS:BA00:39     1.0134245    -1.0134245                     0.0            3067.157    4     0
   BPMS:BA00:52    0.09178291   -0.09178291                     0.0            3073.737    4     0

████ Test 2: All values of all BPMs of P2BPMHER dgrp under meas def 38, default options
_________________________________________________

get: P2BPMHER:BPMS (BPMD=38)
    BPM Values:  293 rows retrieved: ✔
       BPM Name x offset (mm) y offset (mm) num particles (coulomb) z position (meters)   HSTA STATS
           name             x             y                   tmits                   z   HSTA STATS
 BPMS:PR10:8022   0.066477716  -0.066477716                     0.0           3333.5989     17     0
 BPMS:PR10:8032     1.0967255    -1.0967255                     0.0           3356.7275     17     0
 BPMS:PR10:8042   0.058775872  -0.058775872                     0.0            3364.437     17     0
 BPMS:PR10:9012    -0.9340391     0.9340391                     0.0           3373.0083     17     0
 BPMS:PR10:9022   0.051719327  -0.051719327                     0.0           3380.6082     17     0
 BPMS:PR10:9032       1.00613      -1.00613                     0.0           3388.2083     17     0
 BPMS:PR10:9042    0.09205267   -0.09205267                     0.0            3395.808     17     0
 BPMS:PR10:9052    -0.9331402     0.9331402                     0.0           3403.4082     17     0
 BPMS:PR10:9062    0.08750657   -0.08750657                     0.0           3411.0083     17     0
 BPMS:PR10:9072     1.0201329    -1.0201329                     0.0           3418.6082     17     0
 BPMS:PR10:9082   0.031224214  -0.031224214                     0.0           3426.2083     17     0
 BPMS:PR10:9092    -0.9497527     0.9497527                     0.0            3433.808     17     0
 BPMS:PR10:9102   0.004137807  -0.004137807                     0.0           3441.4082     17     0
 BPMS:PR10:9112     1.0184262    -1.0184262                     0.0           3449.0083     17     0
 BPMS:PR10:9122    0.09096262   -0.09096262                     0.0           3456.6082     17     0
 BPMS:PR10:9132    -0.9415369     0.9415369                     0.0           3464.2083     17     0
 BPMS:PR10:9142    0.06736275   -0.06736275                     0.0            3471.808     17     0
 BPMS:PR10:9152      1.083822     -1.083822                     0.0           3479.4082     17     0
 BPMS:PR10:9162    0.00559754   -0.00559754                     0.0           3487.0083     17     0
 BPMS:PR12:6012     -0.914439      0.914439                     0.0           3494.6082      1     0
 BPMS:PR12:6022  0.0099558635 -0.0099558635                     0.0           3502.2083      1     0
 BPMS:PR12:6032     1.0901673    -1.0901673                     0.0            3509.808      1     0
 BPMS:PR12:6042    0.08543185   -0.08543185                     0.0           3517.4082      1     0
 BPMS:PR12:6052   -0.90465623    0.90465623                     0.0           3525.0083      1     0
 BPMS:PR12:6062   0.028607039  -0.028607039                     0.0           3532.6082      1     0
 BPMS:PR12:6072      1.097813     -1.097813                     0.0           3540.2083      1     0
 BPMS:PR12:6082    0.09549012   -0.09549012                     0.0            3547.808      1     0
 BPMS:PR12:6092    -0.9446404     0.9446404                     0.0           3555.4082      1     0
 BPMS:PR12:6102   0.029040897  -0.029040897                     0.0           3563.0083      1     0
 BPMS:PR12:6112     1.0563161    -1.0563161                     0.0           3570.6082      1     0
 BPMS:PR12:6122   0.014306065  -0.014306065                     0.0           3578.2083      1     0
 BPMS:PR12:6132    -0.9044442     0.9044442                     0.0            3585.808      1     0
 BPMS:PR12:6142   0.052999165  -0.052999165                     0.0           3593.4082      1     0
 BPMS:PR12:6152     1.0730395    -1.0730395                     0.0           3601.0083      1     0
 BPMS:PR12:6162    0.06156179   -0.06156179                     0.0           3608.6082      1     0
 BPMS:PR12:7012    -0.9952337     0.9952337                     0.0           3616.2083      1     0
 BPMS:PR12:7022    0.07912166   -0.07912166                     0.0           3619.4702      1     0
 BPMS:PR12:7032     1.0535613    -1.0535613                     0.0           3624.9702      1     0
 BPMS:PR12:7042   0.071676455  -0.071676455                     0.0           3630.9163      1     0
 BPMS:PR12:7052   -0.93331337    0.93331337                     0.0           3636.8623      1     0
 BPMS:PR12:7062    0.07364122   -0.07364122                     0.0            3642.808      1     0
 BPMS:PR12:7072      1.002837     -1.002837                     0.0           3648.7542      1     0
 BPMS:PR12:7082   0.016989613  -0.016989613                     0.0           3655.7273      1     0
 BPMS:PR12:7092     -0.922152      0.922152                     0.0           3662.8003      1     0
 BPMS:PR12:7102   0.021204831  -0.021204831                     0.0           3669.8733      1     0
 BPMS:PR12:8012      1.007878     -1.007878                     0.0           3676.9463      1     0
 BPMS:PR12:8022   0.036372174  -0.036372174                     0.0           3684.0203      1     0
 BPMS:PR12:8032    -0.9113528     0.9113528                     0.0           3691.0933      1     0
 BPMS:PR12:8042     0.0916362    -0.0916362                     0.0           3698.1663      1     0
 BPMS:PR12:8052     1.0418848    -1.0418848                     0.0           3705.1392      1     0
 BPMS:PR12:8062   0.074274495  -0.074274495                     0.0           3711.0852      1     0
 BPMS:PR12:8072    -0.9983177     0.9983177                     0.0           3717.0312      1     0
 BPMS:PR12:8082   0.031983107  -0.031983107                     0.0           3722.9773      1     0
 BPMS:PR12:8092     1.0596108    -1.0596108                     0.0            3728.923      1     0
 BPMS:PR12:8102    0.09709769   -0.09709769                     0.0            3734.423      1     0
 BPMS:PR12:9012     -0.939309      0.939309                     0.0           3739.5613      1     0
 BPMS:PR12:9022    0.05734954   -0.05734954                     0.0           3747.1611      1     0
 BPMS:PR12:9032     1.0924816    -1.0924816                     0.0           3754.7612      1     0
 BPMS:PR12:9042   0.016127529  -0.016127529                     0.0            3762.361      1     0
 BPMS:PR12:9052    -0.9134981     0.9134981                     0.0           3769.9612      1     0
 BPMS:PR12:9062    0.04871823   -0.04871823                     0.0           3777.5613      1     0
 BPMS:PR12:9072     1.0303168    -1.0303168                     0.0           3785.1611      1     0
 BPMS:PR12:9082    0.08213992   -0.08213992                     0.0           3792.7612      1     0
 BPMS:PR12:9092   -0.99816054    0.99816054                     0.0            3800.361      1     0
 BPMS:PR12:9102  0.0032714023 -0.0032714023                     0.0           3807.9612      1     0
 BPMS:PR12:9112     1.0435743    -1.0435743                     0.0           3815.5613      1     0
 BPMS:PR12:9122    0.00669328   -0.00669328                     0.0           3823.1611      1     0
 BPMS:PR12:9132   -0.91747427    0.91747427                     0.0           3830.7612      1     0
 BPMS:PR12:9142   0.097045496  -0.097045496                     0.0            3838.361      1     0
 BPMS:PR12:9152     1.0782317    -1.0782317                     0.0           3845.9612      1     0
 BPMS:PR12:9162    0.04930521   -0.04930521                     0.0           3853.5613      1     0
 BPMS:PR02:6012      -0.92917       0.92917                     0.0           3861.1611     17     0
 BPMS:PR02:6022    0.08097321   -0.08097321                     0.0            3868.761      1     0
 BPMS:PR02:6032     1.0661463    -1.0661463                     0.0            3876.361     17     0
 BPMS:PR02:6042   0.048776206  -0.048776206                     0.0           3883.9612     17     0
 BPMS:PR02:6052     -0.997668      0.997668                     0.0            3891.561      1     0
 BPMS:PR02:6062    0.07392332   -0.07392332                     0.0           3899.1611     17     0
 BPMS:PR02:6072     1.0849892    -1.0849892                     0.0           3906.7612     17     0
 BPMS:PR02:6082    0.09108262   -0.09108262                     0.0            3914.361     17     0
 BPMS:PR02:6092    -0.9342759     0.9342759                     0.0           3921.9612     17     0
 BPMS:PR02:6102   0.026767815  -0.026767815                     0.0            3929.561     17     0
 BPMS:PR02:6112     1.0651873    -1.0651873                     0.0           3937.1611      1     0
 BPMS:PR02:6122    0.06751512   -0.06751512                     0.0           3944.7612     17     0
 BPMS:PR02:6132   -0.94107395    0.94107395                     0.0            3952.361     17     0
 BPMS:PR02:6142   0.024686651  -0.024686651                     0.0           3959.9612     17     0
 BPMS:PR02:6152     1.0644324    -1.0644324                     0.0            3967.561     17     0
 BPMS:PR02:6162   0.019731145  -0.019731145                     0.0           3975.1611     17     0
 BPMS:PR02:7012    -0.9177781     0.9177781                     0.0           3982.7612     17     0
 BPMS:PR02:7022   0.056797285  -0.056797285                     0.0            3996.791      1     0
 BPMS:PR02:7030     1.0356677    -1.0356677                     0.0           3998.9531      1     0
 BPMS:PR02:7035    0.06884324   -0.06884324                     0.0            4014.104     17     0
 BPMS:PR02:7045   -0.99428815    0.99428815                     0.0            4036.395      1     0
 BPMS:PR02:7052    0.06586239   -0.06586239                     0.0            4038.685      1     0
 BPMS:PR02:7083     1.0507815    -1.0507815                     0.0           4043.2212 131073     0
 BPMS:PR02:7087  0.0076760505 -0.0076760505                     0.0            4044.821 131073     0
 BPMS:PR02:8012   -0.93065923    0.93065923                     0.0           4049.3572      1     0
 BPMS:PR02:8020    0.09422845   -0.09422845                     0.0            4051.686      1     0
 BPMS:PR02:8026      1.014157     -1.014157                     0.0           4064.4172      1     0
 BPMS:PR02:8032   0.051996533  -0.051996533                     0.0           4088.0532      1     0
 BPMS:PR02:8042    -0.9085085     0.9085085                     0.0            4090.216      1     0
 BPMS:PR02:9012   0.092256114  -0.092256114                     0.0            4106.119     17     0
 BPMS:PR02:9022     1.0010788    -1.0010788                     0.0            4113.719     17     0
 BPMS:PR02:9032   0.062456872  -0.062456872                     0.0           4121.3193     17     0
 BPMS:PR02:9042   -0.92654246    0.92654246                     0.0            4128.919     17     0
 BPMS:PR02:9052   0.067087084  -0.067087084                     0.0            4136.519     17     0
 BPMS:PR02:9062     1.0109996    -1.0109996                     0.0            4144.119     17     0
 BPMS:PR02:9072    0.07593012   -0.07593012                     0.0            4151.719      1     0
 BPMS:PR02:9082    -0.9587508     0.9587508                     0.0           4159.3193     17     0
 BPMS:PR02:9092    0.09584542   -0.09584542                     0.0            4166.919     17     0
 BPMS:PR02:9102     1.0667686    -1.0667686                     0.0            4174.519     17     0
 BPMS:PR02:9112  0.0071192095 -0.0071192095                     0.0            4182.119     17     0
 BPMS:PR02:9122    -0.9771373     0.9771373                     0.0            4189.719     17     0
 BPMS:PR02:9132   0.031807292  -0.031807292                     0.0           4197.3193      1     0
 BPMS:PR02:9142     1.0743796    -1.0743796                     0.0            4204.919     17     0
 BPMS:PR02:9152   0.081939995  -0.081939995                     0.0            4212.519     17     0
 BPMS:PR02:9162     -0.943246      0.943246                     0.0            4220.119      1     0
 BPMS:PR04:6012    0.03865805   -0.03865805                     0.0           4227.7153      1     0
 BPMS:PR04:6022     1.0014057    -1.0014057                     0.0            4235.315      1     0
 BPMS:PR04:6032   0.039132494  -0.039132494                     0.0            4242.915      1     0
 BPMS:PR04:6042     -0.904274      0.904274                     0.0            4250.515      1     0
 BPMS:PR04:6052    0.03691418   -0.03691418                     0.0            4258.115      1     0
 BPMS:PR04:6062     1.0076997    -1.0076997                     0.0           4265.7153      1     0
 BPMS:PR04:6072  0.0015997429 -0.0015997429                     0.0            4273.315      1     0
 BPMS:PR04:6082   -0.99694216    0.99694216                     0.0            4280.915      1     0
 BPMS:PR04:6092   0.058316723  -0.058316723                     0.0            4288.515      1     0
 BPMS:PR04:6102     1.0089892    -1.0089892                     0.0            4296.115      1     0
 BPMS:PR04:6112     0.0725658    -0.0725658                     0.0           4303.7153      1     0
 BPMS:PR04:6122    -0.9471629     0.9471629                     0.0            4311.315      1     0
 BPMS:PR04:6132   0.022976419  -0.022976419                     0.0            4318.915      1     0
 BPMS:PR04:6142     1.0242652    -1.0242652                     0.0            4326.515      1     0
 BPMS:PR04:6152   0.044501077  -0.044501077                     0.0            4334.115      1     0
 BPMS:PR04:6162   -0.98446494    0.98446494                     0.0           4341.7153      1     0
 BPMS:PR04:7012   0.025168836  -0.025168836                     0.0            4349.315      1     0
 BPMS:PR04:7022     1.0066501    -1.0066501                     0.0            4352.877      1     0
 BPMS:PR04:7032    0.08917037   -0.08917037                     0.0            4358.877      1     0
 BPMS:PR04:7042   -0.90743095    0.90743095                     0.0            4364.823      1     0
 BPMS:PR04:7052   0.017469248  -0.017469248                     0.0            4370.769      1     0
 BPMS:PR04:7062      1.064782     -1.064782                     0.0           4376.7153      1     0
 BPMS:PR04:7072    0.03400136   -0.03400136                     0.0            4383.661      1     0
 BPMS:PR04:7082   -0.98636156    0.98636156                     0.0            4391.434      1     0
 BPMS:PR04:7092    0.03136491   -0.03136491                     0.0             4397.64      1     0
 BPMS:PR04:7102     1.0407915    -1.0407915                     0.0            4403.847      1     0
 BPMS:PR04:8012   0.036689498  -0.036689498                     0.0            4410.053      1     0
 BPMS:PR04:8022   -0.93649346    0.93649346                     0.0           4416.2603      1     0
 BPMS:PR04:8032   0.014980054  -0.014980054                     0.0           4422.4663      1     0
 BPMS:PR04:8042     1.0182898    -1.0182898                     0.0           4428.6733      1     0
 BPMS:PR04:8052    0.02045426   -0.02045426                     0.0           4436.4463      1     0
 BPMS:PR04:8062    -0.9460169     0.9460169                     0.0            4443.392      1     0
 BPMS:PR04:8072   0.019499244  -0.019499244                     0.0            4449.338      1     0
 BPMS:PR04:8082     1.0592364    -1.0592364                     0.0            4455.284      1     0
 BPMS:PR04:8092   0.049908087  -0.049908087                     0.0             4461.23      1     0
 BPMS:PR04:8102     -0.943231      0.943231                     0.0             4467.23      1     0
 BPMS:PR04:9012   0.066734575  -0.066734575                     0.0            4472.668      1     0
 BPMS:PR04:9022     1.0511469    -1.0511469                     0.0            4480.268      1     0
 BPMS:PR04:9032   0.060031183  -0.060031183                     0.0            4487.868      1     0
 BPMS:PR04:9042    -0.9745825     0.9745825                     0.0           4495.4683      1     0
 BPMS:PR04:9052    0.05992917   -0.05992917                     0.0           4503.0684      1     0
 BPMS:PR04:9062     1.0322255    -1.0322255                     0.0            4510.668      1     0
 BPMS:PR04:9072    0.07846418   -0.07846418                     0.0            4518.268      1     0
 BPMS:PR04:9082    -0.9167176     0.9167176                     0.0            4525.868      1     0
 BPMS:PR04:9092   0.056278322  -0.056278322                     0.0           4533.4683      1     0
 BPMS:PR04:9102     1.0225831    -1.0225831                     0.0           4541.0684      1     0
 BPMS:PR04:9112    0.09903238   -0.09903238                     0.0            4548.668      1     0
 BPMS:PR04:9122    -0.9181654     0.9181654                     0.0            4556.268      1     0
 BPMS:PR04:9132   0.029015675  -0.029015675                     0.0            4563.868      1     0
 BPMS:PR04:9142       1.08781      -1.08781                     0.0           4571.4683      1     0
 BPMS:PR04:9152    0.07462387   -0.07462387                     0.0           4579.0684      1     0
 BPMS:PR04:9162     -0.953117      0.953117                     0.0            4586.668      1     0
 BPMS:PR06:6012   0.052369166  -0.052369166                     0.0            4594.268     17     0
 BPMS:PR06:6022     1.0082219    -1.0082219                     0.0            4601.868     17     0
 BPMS:PR06:6032    0.06074693   -0.06074693                     0.0           4609.4683     17     0
 BPMS:PR06:6042   -0.91585726    0.91585726                     0.0           4617.0684     17     0
 BPMS:PR06:6052   0.048785202  -0.048785202                     0.0            4624.668     17     0
 BPMS:PR06:6062     1.0970224    -1.0970224                     0.0            4632.268     17     0
 BPMS:PR06:6072   0.047880147  -0.047880147                     0.0            4639.868     17     0
 BPMS:PR06:6082   -0.93581545    0.93581545                     0.0           4647.4683     17     0
 BPMS:PR06:6092    0.01507878   -0.01507878                     0.0           4655.0684     17     0
 BPMS:PR06:6102     1.0679098    -1.0679098                     0.0            4662.668     17     0
 BPMS:PR06:6112   0.018403765  -0.018403765                     0.0            4670.268     17     0
 BPMS:PR06:6122   -0.96499205    0.96499205                     0.0            4677.868     17     0
 BPMS:PR06:6132    0.02690749   -0.02690749                     0.0           4685.4683     17     0
 BPMS:PR06:6142     1.0678766    -1.0678766                     0.0           4693.0684     17     0
 BPMS:PR06:6152    0.09201839   -0.09201839                     0.0            4700.668     17     0
 BPMS:PR06:6162    -0.9059174     0.9059174                     0.0            4708.268     17     0
 BPMS:PR06:7012   0.018779477  -0.018779477                     0.0            4715.868     17     0
 BPMS:PR06:7022     1.0516038    -1.0516038                     0.0             4719.43     17     0
 BPMS:PR06:7032   0.019746829  -0.019746829                     0.0             4725.43     17     0
 BPMS:PR06:7042    -0.9208402     0.9208402                     0.0            4731.376     17     0
 BPMS:PR06:7052   0.083579816  -0.083579816                     0.0           4737.3223     17     0
 BPMS:PR06:7062     1.0977546    -1.0977546                     0.0            4743.268     17     0
 BPMS:PR06:7072   0.062694244  -0.062694244                     0.0           4750.2144     17     0
 BPMS:PR06:7082    -0.9596801     0.9596801                     0.0           4757.9873     17     0
 BPMS:PR06:7092   0.020082984  -0.020082984                     0.0           4764.1934     17     0
 BPMS:PR06:7102     1.0612595    -1.0612595                     0.0           4770.4004     17     0
 BPMS:PR06:8012    0.02241184   -0.02241184                     0.0            4776.606     17     0
 BPMS:PR06:8022   -0.95042896    0.95042896                     0.0            4782.813     17     0
 BPMS:PR06:8032    0.04880956   -0.04880956                     0.0            4789.019     17     0
 BPMS:PR06:8042      1.096558     -1.096558                     0.0            4795.226     17     0
 BPMS:PR06:8052   0.096716665  -0.096716665                     0.0            4802.999     17     0
 BPMS:PR06:8062    -0.9983383     0.9983383                     0.0           4809.9453     17     0
 BPMS:PR06:8072   0.004514593  -0.004514593                     0.0            4815.891     17     0
 BPMS:PR06:8082     1.0569752    -1.0569752                     0.0            4821.837     17     0
 BPMS:PR06:8092    0.08607242   -0.08607242                     0.0            4827.783     17     0
 BPMS:PR06:8102    -0.9462066     0.9462066                     0.0            4833.783     17     0
 BPMS:PR06:9012   0.053727135  -0.053727135                     0.0            4839.221     17     0
 BPMS:PR06:9022     1.0334537    -1.0334537                     0.0           4846.8213     17     0
 BPMS:PR06:9032   0.018251209  -0.018251209                     0.0           4854.4214     17     0
 BPMS:PR06:9042    -0.9306899     0.9306899                     0.0            4862.021     17     0
 BPMS:PR06:9052  0.0010875503 -0.0010875503                     0.0            4869.621     17     0
 BPMS:PR06:9062     1.0361454    -1.0361454                     0.0            4877.221     17     0
 BPMS:PR06:9072   0.004596631  -0.004596631                     0.0           4884.8213     17     0
 BPMS:PR06:9082   -0.97149014    0.97149014                     0.0           4892.4214     17     0
 BPMS:PR06:9092  0.0037408683 -0.0037408683                     0.0            4900.021     17     0
 BPMS:PR06:9102     1.0960948    -1.0960948                     0.0            4907.621     17     0
 BPMS:PR06:9112   0.022876328  -0.022876328                     0.0            4915.221     17     0
 BPMS:PR06:9122    -0.9769542     0.9769542                     0.0           4922.8213     17     0
 BPMS:PR06:9132     0.0474121    -0.0474121                     0.0           4930.4214     17     0
 BPMS:PR06:9142     1.0420924    -1.0420924                     0.0            4938.021     17     0
 BPMS:PR06:9152  0.0024947524 -0.0024947524                     0.0            4945.621     17     0
 BPMS:PR06:9162     -0.968472      0.968472                     0.0            4953.221     17     0
 BPMS:PR08:6012   0.039555136  -0.039555136                     0.0           4960.8213     17     0
 BPMS:PR08:6022     1.0646477    -1.0646477                     0.0           4968.4214     17     0
 BPMS:PR08:6032    0.07214235   -0.07214235                     0.0            4976.021     17     0
 BPMS:PR08:6042    -0.9398152     0.9398152                     0.0            4983.621     17     0
 BPMS:PR08:6052   0.025610056  -0.025610056                     0.0            4991.221     17     0
 BPMS:PR08:6062     1.0940022    -1.0940022                     0.0           4998.8213     17     0
 BPMS:PR08:6072   0.010055614  -0.010055614                     0.0           5006.4214     17     0
 BPMS:PR08:6082    -0.9250231     0.9250231                     0.0            5014.021     17     0
 BPMS:PR08:6092     0.0902578    -0.0902578                     0.0            5021.621     17     0
 BPMS:PR08:6102     1.0062097    -1.0062097                     0.0            5029.221     17     0
 BPMS:PR08:6112    0.07694371   -0.07694371                     0.0           5036.8213     17     0
 BPMS:PR08:6122    -0.9046597     0.9046597                     0.0           5044.4214     17     0
 BPMS:PR08:6132    0.06287724   -0.06287724                     0.0            5052.021     17     0
 BPMS:PR08:6142     1.0624429    -1.0624429                     0.0            5059.621     17     0
 BPMS:PR08:6152   0.049444094  -0.049444094                     0.0            5067.221     17     0
 BPMS:PR08:6162    -0.9828171     0.9828171                     0.0           5074.8213     17     0
 BPMS:PR08:7012    0.09558353   -0.09558353                     0.0           5082.4214     17     0
 BPMS:PR08:7022     1.0671115    -1.0671115                     0.0            5085.682     17     0
 BPMS:PR08:7032    0.08680873   -0.08680873                     0.0            5091.182     17     0
 BPMS:PR08:7042   -0.90273976    0.90273976                     0.0           5097.1284     17     0
 BPMS:PR08:7052  0.0029385663 -0.0029385663                     0.0            5103.074     17     0
 BPMS:PR08:7062     1.0908109    -1.0908109                     0.0             5109.02     17     0
 BPMS:PR08:7072   0.026091093  -0.026091093                     0.0           5114.9663     17     0
 BPMS:PR08:7082    -0.9927208     0.9927208                     0.0             5121.94     17     0
 BPMS:PR08:7092    0.08658214   -0.08658214                     0.0            5129.013     17     0
 BPMS:PR08:7102     1.0483624    -1.0483624                     0.0            5136.086     17     0
 BPMS:PR08:8012   0.030651294  -0.030651294                     0.0            5143.159     17     0
 BPMS:PR08:8022    -0.9653954     0.9653954                     0.0           5150.2324     17     0
 BPMS:PR08:8032     0.0901258    -0.0901258                     0.0            5157.305     17     0
 BPMS:PR08:8042     1.0325303    -1.0325303                     0.0           5164.3784     17     0
 BPMS:PR08:8052     0.0664642    -0.0664642                     0.0            5171.351     17     0
 BPMS:PR08:8062    -0.9696981     0.9696981                     0.0           5177.2974     17     0
 BPMS:PR08:8072    0.09684373   -0.09684373                     0.0            5183.243     17     0
 BPMS:PR08:8082     1.0379802    -1.0379802                     0.0            5189.189     17     0
 BPMS:PR08:8092   0.090823635  -0.090823635                     0.0           5195.1353     17     0
 BPMS:PR08:8102    -0.9769146     0.9769146                     0.0           5200.6353     17     0
 BPMS:PR08:9012   0.031642884  -0.031642884                     0.0           5205.7744     17     0
 BPMS:PR08:9022     1.0002424    -1.0002424                     0.0            5213.374     17     0
 BPMS:PR08:9032    0.09840452   -0.09840452                     0.0            5220.974     17     0
 BPMS:PR08:9042    -0.9774571     0.9774571                     0.0            5228.574     17     0
 BPMS:PR08:9052  0.0061071403 -0.0061071403                     0.0           5236.1743     17     0
 BPMS:PR08:9062     1.0747007    -1.0747007                     0.0           5243.7744     17     0
 BPMS:PR08:9072   0.018230733  -0.018230733                     0.0            5251.374     17     0
 BPMS:PR08:9082   -0.93036276    0.93036276                     0.0            5258.974     17     0
 BPMS:PR08:9092   0.036793433  -0.036793433                     0.0            5266.574     17     0
 BPMS:PR08:9102     1.0670167    -1.0670167                     0.0           5274.1743     17     0
 BPMS:PR08:9112    0.08717295   -0.08717295                     0.0           5281.7744     17     0
 BPMS:PR08:9122    -0.9669596     0.9669596                     0.0            5289.374     17     0
 BPMS:PR08:9132   0.033772666  -0.033772666                     0.0            5296.974     17     0
 BPMS:PR08:9142     1.0733129    -1.0733129                     0.0            5304.574     17     0
 BPMS:PR08:9152   0.030658767  -0.030658767                     0.0           5312.1743     17     0
 BPMS:PR08:9162    -0.9626147     0.9626147                     0.0           5319.7744     17     0
 BPMS:PR10:6012   0.063763045  -0.063763045                     0.0           3128.0562     17     0
 BPMS:PR10:6022     1.0560704    -1.0560704                     0.0           3135.6562     20     0
 BPMS:PR10:6032   0.045027934  -0.045027934                     0.0            3143.256     17     0
 BPMS:PR10:6042   -0.94897014    0.94897014                     0.0           3150.8562     17     0
 BPMS:PR10:6052   0.004066749  -0.004066749                     0.0           3158.4563     17     0
 BPMS:PR10:6062     1.0749892    -1.0749892                     0.0           3166.0562     17     0
 BPMS:PR10:6072    0.08600322   -0.08600322                     0.0           3173.6562     17     0
 BPMS:PR10:6082   -0.90511215    0.90511215                     0.0            3181.256     17     0
 BPMS:PR10:6092  0.0071482034 -0.0071482034                     0.0           3188.8562     17     0
 BPMS:PR10:6102     1.0517668    -1.0517668                     0.0           3196.4563     17     0
 BPMS:PR10:6112   0.025563812  -0.025563812                     0.0           3204.0562     17     0
 BPMS:PR10:6122   -0.99530214    0.99530214                     0.0           3211.6562     17     0
 BPMS:PR10:6132    0.08937037   -0.08937037                     0.0            3219.256     17     0
 BPMS:PR10:6142     1.0156763    -1.0156763                     0.0           3226.8562     17     0
 BPMS:PR10:6152   0.028162582  -0.028162582                     0.0           3234.4563     17     0
 BPMS:PR10:6162    -0.9782702     0.9782702                     0.0           3242.0562     17     0
 BPMS:PR10:7012   0.015536556  -0.015536556                     0.0           3249.6553     17     0
 BPMS:PR10:7022     1.0258453    -1.0258453                     0.0           3256.5032     17     0
 BPMS:PR10:7032    0.04465735   -0.04465735                     0.0            3265.096     17     0
 BPMS:PR10:7042    -0.9776291     0.9776291                     0.0           3287.3416     17     0
 BPMS:PR10:8012  1.5865061E-4 -1.5865061E-4                     0.0           3313.1792     17     0

████ Test 3: All values of all BPMs of P2BPMHER dgrp, under meas def 38, SORTORDER=1 (by dgrp z)
_________________________________________________

get: P2BPMHER:BPMS (SORTORDER=1, BPMD=38, N=1024)
    BPM Values:  293 rows retrieved: ✔
       BPM Name x offset (mm) y offset (mm) num particles (coulomb) z position (meters)   HSTA STATS
           name             x             y                   tmits                   z   HSTA STATS
 BPMS:PR10:6012    0.06399027   -0.06399027                     0.0           3128.0562     17     0
 BPMS:PR10:6022     1.0627301    -1.0627301                     0.0           3135.6562     20     0
 BPMS:PR10:6032    0.08438437   -0.08438437                     0.0            3143.256     17     0
 BPMS:PR10:6042   -0.91278464    0.91278464                     0.0           3150.8562     17     0
 BPMS:PR10:6052   0.040330563  -0.040330563                     0.0           3158.4563     17     0
 BPMS:PR10:6062     1.0543094    -1.0543094                     0.0           3166.0562     17     0
 BPMS:PR10:6072     0.0493266    -0.0493266                     0.0           3173.6562     17     0
 BPMS:PR10:6082     -0.907122      0.907122                     0.0            3181.256     17     0
 BPMS:PR10:6092    0.04673903   -0.04673903                     0.0           3188.8562     17     0
 BPMS:PR10:6102     1.0181769    -1.0181769                     0.0           3196.4563     17     0
 BPMS:PR10:6112     0.0265346    -0.0265346                     0.0           3204.0562     17     0
 BPMS:PR10:6122    -0.9154063     0.9154063                     0.0           3211.6562     17     0
 BPMS:PR10:6132    0.09946488   -0.09946488                     0.0            3219.256     17     0
 BPMS:PR10:6142     1.0853612    -1.0853612                     0.0           3226.8562     17     0
 BPMS:PR10:6152    0.03885104   -0.03885104                     0.0           3234.4563     17     0
 BPMS:PR10:6162   -0.98758954    0.98758954                     0.0           3242.0562     17     0
 BPMS:PR10:7012 -2.7024842E-4  2.7024842E-4                     0.0           3249.6553     17     0
 BPMS:PR10:7022     1.0267206    -1.0267206                     0.0           3256.5032     17     0
 BPMS:PR10:7032   0.031649794  -0.031649794                     0.0            3265.096     17     0
 BPMS:PR10:7042   -0.93913937    0.93913937                     0.0           3287.3416     17     0
 BPMS:PR10:8012   0.058519244  -0.058519244                     0.0           3313.1792     17     0
 BPMS:PR10:8022  0.0061933813 -0.0061933813                     0.0           3333.5989     17     0
 BPMS:PR10:8032      1.085894     -1.085894                     0.0           3356.7275     17     0
 BPMS:PR10:8042    0.08820017   -0.08820017                     0.0            3364.437     17     0
 BPMS:PR10:9012    -0.9556796     0.9556796                     0.0           3373.0083     17     0
 BPMS:PR10:9022    0.05659228   -0.05659228                     0.0           3380.6082     17     0
 BPMS:PR10:9032     1.0346427    -1.0346427                     0.0           3388.2083     17     0
 BPMS:PR10:9042   0.008395407  -0.008395407                     0.0            3395.808     17     0
 BPMS:PR10:9052     -0.933667      0.933667                     0.0           3403.4082     17     0
 BPMS:PR10:9062    0.09004221   -0.09004221                     0.0           3411.0083     17     0
 BPMS:PR10:9072     1.0939894    -1.0939894                     0.0           3418.6082     17     0
 BPMS:PR10:9082  0.0054640598 -0.0054640598                     0.0           3426.2083     17     0
 BPMS:PR10:9092   -0.91368395    0.91368395                     0.0            3433.808     17     0
 BPMS:PR10:9102    0.07592655   -0.07592655                     0.0           3441.4082     17     0
 BPMS:PR10:9112     1.0723196    -1.0723196                     0.0           3449.0083     17     0
 BPMS:PR10:9122   0.057465892  -0.057465892                     0.0           3456.6082     17     0
 BPMS:PR10:9132   -0.97837484    0.97837484                     0.0           3464.2083     17     0
 BPMS:PR10:9142    0.04574629   -0.04574629                     0.0            3471.808     17     0
 BPMS:PR10:9152     1.0772098    -1.0772098                     0.0           3479.4082     17     0
 BPMS:PR10:9162   0.041962106  -0.041962106                     0.0           3487.0083     17     0
 BPMS:PR12:6012   -0.92786294    0.92786294                     0.0           3494.6082      1     0
 BPMS:PR12:6022   0.033933043  -0.033933043                     0.0           3502.2083      1     0
 BPMS:PR12:6032     1.0871469    -1.0871469                     0.0            3509.808      1     0
 BPMS:PR12:6042   0.034093738  -0.034093738                     0.0           3517.4082      1     0
 BPMS:PR12:6052   -0.95638245    0.95638245                     0.0           3525.0083      1     0
 BPMS:PR12:6062    0.04470424   -0.04470424                     0.0           3532.6082      1     0
 BPMS:PR12:6072     1.0576776    -1.0576776                     0.0           3540.2083      1     0
 BPMS:PR12:6082    0.09548874   -0.09548874                     0.0            3547.808      1     0
 BPMS:PR12:6092   -0.90245515    0.90245515                     0.0           3555.4082      1     0
 BPMS:PR12:6102    0.03136559   -0.03136559                     0.0           3563.0083      1     0
 BPMS:PR12:6112     1.0088787    -1.0088787                     0.0           3570.6082      1     0
 BPMS:PR12:6122    0.08920571   -0.08920571                     0.0           3578.2083      1     0
 BPMS:PR12:6132    -0.9624039     0.9624039                     0.0            3585.808      1     0
 BPMS:PR12:6142    0.09473024   -0.09473024                     0.0           3593.4082      1     0
 BPMS:PR12:6152     1.0773634    -1.0773634                     0.0           3601.0083      1     0
 BPMS:PR12:6162   0.081961624  -0.081961624                     0.0           3608.6082      1     0
 BPMS:PR12:7012   -0.94862974    0.94862974                     0.0           3616.2083      1     0
 BPMS:PR12:7022     0.0119584    -0.0119584                     0.0           3619.4702      1     0
 BPMS:PR12:7032     1.0903039    -1.0903039                     0.0           3624.9702      1     0
 BPMS:PR12:7042   0.017753683  -0.017753683                     0.0           3630.9163      1     0
 BPMS:PR12:7052     -0.997941      0.997941                     0.0           3636.8623      1     0
 BPMS:PR12:7062    0.08424028   -0.08424028                     0.0            3642.808      1     0
 BPMS:PR12:7072      1.023154     -1.023154                     0.0           3648.7542      1     0
 BPMS:PR12:7082    0.08843075   -0.08843075                     0.0           3655.7273      1     0
 BPMS:PR12:7092    -0.9397642     0.9397642                     0.0           3662.8003      1     0
 BPMS:PR12:7102   0.095415294  -0.095415294                     0.0           3669.8733      1     0
 BPMS:PR12:8012     1.0458224    -1.0458224                     0.0           3676.9463      1     0
 BPMS:PR12:8022    0.08192203   -0.08192203                     0.0           3684.0203      1     0
 BPMS:PR12:8032   -0.95875883    0.95875883                     0.0           3691.0933      1     0
 BPMS:PR12:8042   0.022968525  -0.022968525                     0.0           3698.1663      1     0
 BPMS:PR12:8052     1.0237992    -1.0237992                     0.0           3705.1392      1     0
 BPMS:PR12:8062   0.013444574  -0.013444574                     0.0           3711.0852      1     0
 BPMS:PR12:8072    -0.9430082     0.9430082                     0.0           3717.0312      1     0
 BPMS:PR12:8082    0.01087706   -0.01087706                     0.0           3722.9773      1     0
 BPMS:PR12:8092     1.0474428    -1.0474428                     0.0            3728.923      1     0
 BPMS:PR12:8102   6.809593E-4  -6.809593E-4                     0.0            3734.423      1     0
 BPMS:PR12:9012    -0.9443179     0.9443179                     0.0           3739.5613      1     0
 BPMS:PR12:9022   0.005046148  -0.005046148                     0.0           3747.1611      1     0
 BPMS:PR12:9032     1.0960635    -1.0960635                     0.0           3754.7612      1     0
 BPMS:PR12:9042   0.053303953  -0.053303953                     0.0            3762.361      1     0
 BPMS:PR12:9052   -0.96347684    0.96347684                     0.0           3769.9612      1     0
 BPMS:PR12:9062   0.004862687  -0.004862687                     0.0           3777.5613      1     0
 BPMS:PR12:9072     1.0423928    -1.0423928                     0.0           3785.1611      1     0
 BPMS:PR12:9082    0.07420156   -0.07420156                     0.0           3792.7612      1     0
 BPMS:PR12:9092     -0.900285      0.900285                     0.0            3800.361      1     0
 BPMS:PR12:9102   0.019671405  -0.019671405                     0.0           3807.9612      1     0
 BPMS:PR12:9112     1.0560358    -1.0560358                     0.0           3815.5613      1     0
 BPMS:PR12:9122    0.05117284   -0.05117284                     0.0           3823.1611      1     0
 BPMS:PR12:9132    -0.9682375     0.9682375                     0.0           3830.7612      1     0
 BPMS:PR12:9142    0.04624955   -0.04624955                     0.0            3838.361      1     0
 BPMS:PR12:9152     1.0687885    -1.0687885                     0.0           3845.9612      1     0
 BPMS:PR12:9162   0.033914346  -0.033914346                     0.0           3853.5613      1     0
 BPMS:PR02:6012   -0.96936685    0.96936685                     0.0           3861.1611     17     0
 BPMS:PR02:6022    0.09184707   -0.09184707                     0.0            3868.761      1     0
 BPMS:PR02:6032     1.0221965    -1.0221965                     0.0            3876.361     17     0
 BPMS:PR02:6042    0.09096713   -0.09096713                     0.0           3883.9612     17     0
 BPMS:PR02:6052    -0.9125837     0.9125837                     0.0            3891.561      1     0
 BPMS:PR02:6062      0.067918     -0.067918                     0.0           3899.1611     17     0
 BPMS:PR02:6072       1.07273      -1.07273                     0.0           3906.7612     17     0
 BPMS:PR02:6082   0.028760947  -0.028760947                     0.0            3914.361     17     0
 BPMS:PR02:6092   -0.90894896    0.90894896                     0.0           3921.9612     17     0
 BPMS:PR02:6102    0.09642303   -0.09642303                     0.0            3929.561     17     0
 BPMS:PR02:6112     1.0420357    -1.0420357                     0.0           3937.1611      1     0
 BPMS:PR02:6122   0.048151635  -0.048151635                     0.0           3944.7612     17     0
 BPMS:PR02:6132   -0.99252474    0.99252474                     0.0            3952.361     17     0
 BPMS:PR02:6142   0.089367025  -0.089367025                     0.0           3959.9612     17     0
 BPMS:PR02:6152     1.0486522    -1.0486522                     0.0            3967.561     17     0
 BPMS:PR02:6162    0.06327147   -0.06327147                     0.0           3975.1611     17     0
 BPMS:PR02:7012   -0.90540105    0.90540105                     0.0           3982.7612     17     0
 BPMS:PR02:7022    0.04459895   -0.04459895                     0.0            3996.791      1     0
 BPMS:PR02:7030     1.0163844    -1.0163844                     0.0           3998.9531      1     0
 BPMS:PR02:7035   0.031241518  -0.031241518                     0.0            4014.104     17     0
 BPMS:PR02:7045     -0.950342      0.950342                     0.0            4036.395      1     0
 BPMS:PR02:7052     0.0586552    -0.0586552                     0.0            4038.685      1     0
 BPMS:PR02:7083     1.0052414    -1.0052414                     0.0           4043.2212 131073     0
 BPMS:PR02:7087   0.049497712  -0.049497712                     0.0            4044.821 131073     0
 BPMS:PR02:8012    -0.9214664     0.9214664                     0.0           4049.3572      1     0
 BPMS:PR02:8020    0.06114985   -0.06114985                     0.0            4051.686      1     0
 BPMS:PR02:8026     1.0004582    -1.0004582                     0.0           4064.4172      1     0
 BPMS:PR02:8032   0.010426093  -0.010426093                     0.0           4088.0532      1     0
 BPMS:PR02:8042     -0.992383      0.992383                     0.0            4090.216      1     0
 BPMS:PR02:9012   0.069114126  -0.069114126                     0.0            4106.119     17     0
 BPMS:PR02:9022     1.0441176    -1.0441176                     0.0            4113.719     17     0
 BPMS:PR02:9032    0.03838546   -0.03838546                     0.0           4121.3193     17     0
 BPMS:PR02:9042    -0.9388106     0.9388106                     0.0            4128.919     17     0
 BPMS:PR02:9052    0.06617605   -0.06617605                     0.0            4136.519     17     0
 BPMS:PR02:9062      1.029119     -1.029119                     0.0            4144.119     17     0
 BPMS:PR02:9072    0.04874631   -0.04874631                     0.0            4151.719      1     0
 BPMS:PR02:9082    -0.9656671     0.9656671                     0.0           4159.3193     17     0
 BPMS:PR02:9092  0.0017057332 -0.0017057332                     0.0            4166.919     17     0
 BPMS:PR02:9102     1.0772631    -1.0772631                     0.0            4174.519     17     0
 BPMS:PR02:9112   0.025529865  -0.025529865                     0.0            4182.119     17     0
 BPMS:PR02:9122   -0.90162176    0.90162176                     0.0            4189.719     17     0
 BPMS:PR02:9132   0.019150216  -0.019150216                     0.0           4197.3193      1     0
 BPMS:PR02:9142     1.0734267    -1.0734267                     0.0            4204.919     17     0
 BPMS:PR02:9152   0.006004683  -0.006004683                     0.0            4212.519     17     0
 BPMS:PR02:9162    -0.9912227     0.9912227                     0.0            4220.119      1     0
 BPMS:PR04:6012   0.021924995  -0.021924995                     0.0           4227.7153      1     0
 BPMS:PR04:6022     1.0690107    -1.0690107                     0.0            4235.315      1     0
 BPMS:PR04:6032   0.003532773  -0.003532773                     0.0            4242.915      1     0
 BPMS:PR04:6042   -0.93320537    0.93320537                     0.0            4250.515      1     0
 BPMS:PR04:6052    0.08523594   -0.08523594                     0.0            4258.115      1     0
 BPMS:PR04:6062     1.0344983    -1.0344983                     0.0           4265.7153      1     0
 BPMS:PR04:6072   0.016614478  -0.016614478                     0.0            4273.315      1     0
 BPMS:PR04:6082    -0.9558276     0.9558276                     0.0            4280.915      1     0
 BPMS:PR04:6092   0.039575204  -0.039575204                     0.0            4288.515      1     0
 BPMS:PR04:6102     1.0658256    -1.0658256                     0.0            4296.115      1     0
 BPMS:PR04:6112   0.022873178  -0.022873178                     0.0           4303.7153      1     0
 BPMS:PR04:6122     -0.998983      0.998983                     0.0            4311.315      1     0
 BPMS:PR04:6132    0.06611404   -0.06611404                     0.0            4318.915      1     0
 BPMS:PR04:6142      1.033002     -1.033002                     0.0            4326.515      1     0
 BPMS:PR04:6152   0.008806433  -0.008806433                     0.0            4334.115      1     0
 BPMS:PR04:6162    -0.9644693     0.9644693                     0.0           4341.7153      1     0
 BPMS:PR04:7012    0.07694447   -0.07694447                     0.0            4349.315      1     0
 BPMS:PR04:7022     1.0468841    -1.0468841                     0.0            4352.877      1     0
 BPMS:PR04:7032    0.09689786   -0.09689786                     0.0            4358.877      1     0
 BPMS:PR04:7042   -0.95656633    0.95656633                     0.0            4364.823      1     0
 BPMS:PR04:7052     0.0758227    -0.0758227                     0.0            4370.769      1     0
 BPMS:PR04:7062     1.0453258    -1.0453258                     0.0           4376.7153      1     0
 BPMS:PR04:7072    0.07794961   -0.07794961                     0.0            4383.661      1     0
 BPMS:PR04:7082    -0.9221478     0.9221478                     0.0            4391.434      1     0
 BPMS:PR04:7092   0.022403115  -0.022403115                     0.0             4397.64      1     0
 BPMS:PR04:7102     1.0031505    -1.0031505                     0.0            4403.847      1     0
 BPMS:PR04:8012    0.07641878   -0.07641878                     0.0            4410.053      1     0
 BPMS:PR04:8022    -0.9581123     0.9581123                     0.0           4416.2603      1     0
 BPMS:PR04:8032   0.076386124  -0.076386124                     0.0           4422.4663      1     0
 BPMS:PR04:8042     1.0820838    -1.0820838                     0.0           4428.6733      1     0
 BPMS:PR04:8052   0.050858684  -0.050858684                     0.0           4436.4463      1     0
 BPMS:PR04:8062    -0.9013439     0.9013439                     0.0            4443.392      1     0
 BPMS:PR04:8072   0.050898235  -0.050898235                     0.0            4449.338      1     0
 BPMS:PR04:8082     1.0540411    -1.0540411                     0.0            4455.284      1     0
 BPMS:PR04:8092    0.06564972   -0.06564972                     0.0             4461.23      1     0
 BPMS:PR04:8102    -0.9635102     0.9635102                     0.0             4467.23      1     0
 BPMS:PR04:9012    0.08833783   -0.08833783                     0.0            4472.668      1     0
 BPMS:PR04:9022     1.0819033    -1.0819033                     0.0            4480.268      1     0
 BPMS:PR04:9032     0.0808665    -0.0808665                     0.0            4487.868      1     0
 BPMS:PR04:9042   -0.97172076    0.97172076                     0.0           4495.4683      1     0
 BPMS:PR04:9052    0.04752193   -0.04752193                     0.0           4503.0684      1     0
 BPMS:PR04:9062     1.0033681    -1.0033681                     0.0            4510.668      1     0
 BPMS:PR04:9072   0.029505813  -0.029505813                     0.0            4518.268      1     0
 BPMS:PR04:9082    -0.9859872     0.9859872                     0.0            4525.868      1     0
 BPMS:PR04:9092   0.036157962  -0.036157962                     0.0           4533.4683      1     0
 BPMS:PR04:9102     1.0379301    -1.0379301                     0.0           4541.0684      1     0
 BPMS:PR04:9112    0.04975839   -0.04975839                     0.0            4548.668      1     0
 BPMS:PR04:9122    -0.9865101     0.9865101                     0.0            4556.268      1     0
 BPMS:PR04:9132    0.08459661   -0.08459661                     0.0            4563.868      1     0
 BPMS:PR04:9142     1.0462635    -1.0462635                     0.0           4571.4683      1     0
 BPMS:PR04:9152   0.057143744  -0.057143744                     0.0           4579.0684      1     0
 BPMS:PR04:9162   -0.93918264    0.93918264                     0.0            4586.668      1     0
 BPMS:PR06:6012    0.09136636   -0.09136636                     0.0            4594.268     17     0
 BPMS:PR06:6022       1.03469      -1.03469                     0.0            4601.868     17     0
 BPMS:PR06:6032   0.038895085  -0.038895085                     0.0           4609.4683     17     0
 BPMS:PR06:6042   -0.98582184    0.98582184                     0.0           4617.0684     17     0
 BPMS:PR06:6052    0.03761223   -0.03761223                     0.0            4624.668     17     0
 BPMS:PR06:6062     1.0148998    -1.0148998                     0.0            4632.268     17     0
 BPMS:PR06:6072   0.056296676  -0.056296676                     0.0            4639.868     17     0
 BPMS:PR06:6082   -0.98558235    0.98558235                     0.0           4647.4683     17     0
 BPMS:PR06:6092     0.0967502    -0.0967502                     0.0           4655.0684     17     0
 BPMS:PR06:6102     1.0067308    -1.0067308                     0.0            4662.668     17     0
 BPMS:PR06:6112   0.013309883  -0.013309883                     0.0            4670.268     17     0
 BPMS:PR06:6122   -0.95192164    0.95192164                     0.0            4677.868     17     0
 BPMS:PR06:6132   0.060533147  -0.060533147                     0.0           4685.4683     17     0
 BPMS:PR06:6142     1.0785244    -1.0785244                     0.0           4693.0684     17     0
 BPMS:PR06:6152    0.08480955   -0.08480955                     0.0            4700.668     17     0
 BPMS:PR06:6162    -0.9506885     0.9506885                     0.0            4708.268     17     0
 BPMS:PR06:7012    0.06018359   -0.06018359                     0.0            4715.868     17     0
 BPMS:PR06:7022     1.0652303    -1.0652303                     0.0             4719.43     17     0
 BPMS:PR06:7032    0.07783748   -0.07783748                     0.0             4725.43     17     0
 BPMS:PR06:7042   -0.99184334    0.99184334                     0.0            4731.376     17     0
 BPMS:PR06:7052    0.06834899   -0.06834899                     0.0           4737.3223     17     0
 BPMS:PR06:7062     1.0068868    -1.0068868                     0.0            4743.268     17     0
 BPMS:PR06:7072   0.022421494  -0.022421494                     0.0           4750.2144     17     0
 BPMS:PR06:7082    -0.9950313     0.9950313                     0.0           4757.9873     17     0
 BPMS:PR06:7092    0.04456226   -0.04456226                     0.0           4764.1934     17     0
 BPMS:PR06:7102     1.0717129    -1.0717129                     0.0           4770.4004     17     0
 BPMS:PR06:8012   0.018715924  -0.018715924                     0.0            4776.606     17     0
 BPMS:PR06:8022   -0.97036874    0.97036874                     0.0            4782.813     17     0
 BPMS:PR06:8032    0.01771632   -0.01771632                     0.0            4789.019     17     0
 BPMS:PR06:8042      1.075382     -1.075382                     0.0            4795.226     17     0
 BPMS:PR06:8052   0.090711266  -0.090711266                     0.0            4802.999     17     0
 BPMS:PR06:8062   -0.99043435    0.99043435                     0.0           4809.9453     17     0
 BPMS:PR06:8072   0.009806673  -0.009806673                     0.0            4815.891     17     0
 BPMS:PR06:8082     1.0291181    -1.0291181                     0.0            4821.837     17     0
 BPMS:PR06:8092   0.024011776  -0.024011776                     0.0            4827.783     17     0
 BPMS:PR06:8102    -0.9520875     0.9520875                     0.0            4833.783     17     0
 BPMS:PR06:9012   0.043747336  -0.043747336                     0.0            4839.221     17     0
 BPMS:PR06:9022     1.0798095    -1.0798095                     0.0           4846.8213     17     0
 BPMS:PR06:9032   0.062603414  -0.062603414                     0.0           4854.4214     17     0
 BPMS:PR06:9042   -0.95899826    0.95899826                     0.0            4862.021     17     0
 BPMS:PR06:9052    0.08626439   -0.08626439                     0.0            4869.621     17     0
 BPMS:PR06:9062     1.0754038    -1.0754038                     0.0            4877.221     17     0
 BPMS:PR06:9072   0.089358665  -0.089358665                     0.0           4884.8213     17     0
 BPMS:PR06:9082    -0.9526876     0.9526876                     0.0           4892.4214     17     0
 BPMS:PR06:9092    0.05364694   -0.05364694                     0.0            4900.021     17     0
 BPMS:PR06:9102     1.0736481    -1.0736481                     0.0            4907.621     17     0
 BPMS:PR06:9112   0.096907735  -0.096907735                     0.0            4915.221     17     0
 BPMS:PR06:9122   -0.98564404    0.98564404                     0.0           4922.8213     17     0
 BPMS:PR06:9132   0.038591765  -0.038591765                     0.0           4930.4214     17     0
 BPMS:PR06:9142     1.0742145    -1.0742145                     0.0            4938.021     17     0
 BPMS:PR06:9152    0.02280181   -0.02280181                     0.0            4945.621     17     0
 BPMS:PR06:9162    -0.9925232     0.9925232                     0.0            4953.221     17     0
 BPMS:PR08:6012    0.08080948   -0.08080948                     0.0           4960.8213     17     0
 BPMS:PR08:6022     1.0446819    -1.0446819                     0.0           4968.4214     17     0
 BPMS:PR08:6032  0.0127399955 -0.0127399955                     0.0            4976.021     17     0
 BPMS:PR08:6042    -0.9740816     0.9740816                     0.0            4983.621     17     0
 BPMS:PR08:6052   0.016097622  -0.016097622                     0.0            4991.221     17     0
 BPMS:PR08:6062     1.0309039    -1.0309039                     0.0           4998.8213     17     0
 BPMS:PR08:6072    0.05584945   -0.05584945                     0.0           5006.4214     17     0
 BPMS:PR08:6082   -0.96562874    0.96562874                     0.0            5014.021     17     0
 BPMS:PR08:6092  0.0059834877 -0.0059834877                     0.0            5021.621     17     0
 BPMS:PR08:6102     1.0459981    -1.0459981                     0.0            5029.221     17     0
 BPMS:PR08:6112    0.04424199   -0.04424199                     0.0           5036.8213     17     0
 BPMS:PR08:6122    -0.9836419     0.9836419                     0.0           5044.4214     17     0
 BPMS:PR08:6132    0.07480843   -0.07480843                     0.0            5052.021     17     0
 BPMS:PR08:6142     1.0676806    -1.0676806                     0.0            5059.621     17     0
 BPMS:PR08:6152    0.06458097   -0.06458097                     0.0            5067.221     17     0
 BPMS:PR08:6162    -0.9808657     0.9808657                     0.0           5074.8213     17     0
 BPMS:PR08:7012   0.047177043  -0.047177043                     0.0           5082.4214     17     0
 BPMS:PR08:7022     1.0266006    -1.0266006                     0.0            5085.682     17     0
 BPMS:PR08:7032   0.060451753  -0.060451753                     0.0            5091.182     17     0
 BPMS:PR08:7042   -0.96596944    0.96596944                     0.0           5097.1284     17     0
 BPMS:PR08:7052  0.0016859757 -0.0016859757                     0.0            5103.074     17     0
 BPMS:PR08:7062     1.0492159    -1.0492159                     0.0             5109.02     17     0
 BPMS:PR08:7072   0.081663944  -0.081663944                     0.0           5114.9663     17     0
 BPMS:PR08:7082    -0.9440673     0.9440673                     0.0             5121.94     17     0
 BPMS:PR08:7092    0.02254038   -0.02254038                     0.0            5129.013     17     0
 BPMS:PR08:7102     1.0779666    -1.0779666                     0.0            5136.086     17     0
 BPMS:PR08:8012   0.070614964  -0.070614964                     0.0            5143.159     17     0
 BPMS:PR08:8022   -0.93825746    0.93825746                     0.0           5150.2324     17     0
 BPMS:PR08:8032    0.05185211   -0.05185211                     0.0            5157.305     17     0
 BPMS:PR08:8042     1.0928011    -1.0928011                     0.0           5164.3784     17     0
 BPMS:PR08:8052    0.06955095   -0.06955095                     0.0            5171.351     17     0
 BPMS:PR08:8062    -0.9667174     0.9667174                     0.0           5177.2974     17     0
 BPMS:PR08:8072    0.03714876   -0.03714876                     0.0            5183.243     17     0
 BPMS:PR08:8082     1.0816647    -1.0816647                     0.0            5189.189     17     0
 BPMS:PR08:8092    0.05953792   -0.05953792                     0.0           5195.1353     17     0
 BPMS:PR08:8102     -0.946122      0.946122                     0.0           5200.6353     17     0
 BPMS:PR08:9012   0.012229009  -0.012229009                     0.0           5205.7744     17     0
 BPMS:PR08:9022     1.0147505    -1.0147505                     0.0            5213.374     17     0
 BPMS:PR08:9032    0.08859144   -0.08859144                     0.0            5220.974     17     0
 BPMS:PR08:9042   -0.98114526    0.98114526                     0.0            5228.574     17     0
 BPMS:PR08:9052     0.0604037    -0.0604037                     0.0           5236.1743     17     0
 BPMS:PR08:9062     1.0321859    -1.0321859                     0.0           5243.7744     17     0
 BPMS:PR08:9072   0.035560314  -0.035560314                     0.0            5251.374     17     0
 BPMS:PR08:9082     -0.964135      0.964135                     0.0            5258.974     17     0
 BPMS:PR08:9092    0.09951628   -0.09951628                     0.0            5266.574     17     0
 BPMS:PR08:9102     1.0994831    -1.0994831                     0.0           5274.1743     17     0
 BPMS:PR08:9112   0.055352103  -0.055352103                     0.0           5281.7744     17     0
 BPMS:PR08:9122    -0.9526432     0.9526432                     0.0            5289.374     17     0
 BPMS:PR08:9132    0.02572821   -0.02572821                     0.0            5296.974     17     0
 BPMS:PR08:9142      1.015135     -1.015135                     0.0            5304.574     17     0
 BPMS:PR08:9152    0.08174548   -0.08174548                     0.0           5312.1743     17     0
 BPMS:PR08:9162    -0.9719117     0.9719117                     0.0           5319.7744     17     0

████ Test 8: Bad argument Name - superfish=7
_________________________________________________

get: P2BPMHER:BPMS (superfish=7, BPMD=38)
 P2BPMHER:BPMS:  SUPERFISH is not a valid argument for get requests to this channel. Valid arguments are: [BPMD, CNFNUM, CNFTYPE, N, SORTORDER] ✔


████ Test 10: BPMD not valid for DGRP
_________________________________________________

get: P2BPMHER:BPMS (SORTORDER=1, BPMD=39)
   Message number 00000000; UnableToGetDataException; while making BPM acquisition ✔
```

