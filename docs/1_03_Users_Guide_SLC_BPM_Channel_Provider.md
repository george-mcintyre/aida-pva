# 1.3 - SLC Bpm Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Bpm data through AIDA-PVA. For general
information on using AIDA-PVA see [Basic Users Guide to Aida](1_00_User_Guide.md), and the EPICS javadoc.

## Summary

Acquires SLC BPM orbit data of each bpm in a given display group, under a given measurement definition.

## Synopsis

|                          | PV                              | Arguments                                                 | Description                          |
|:-------------------------|---------------------------------|-----------------------------------------------------------|----------------------------|
| [1.3.1](@ref section131) | `<display-group-mnemonic>:BPMS` | `BPMD` [ `CNFTYPE` ] [ `CNFNUM` ] [ `N` ] [ `SORTORDER` ] | Acquire BPM data |

## 1.3.1. Acquire SLC BPM orbit data  {#section131}

- `<display-group-mnemonic>:BPMS`
    - `BPMD`* : _measurement-definition-number_
    - `CNFTYPE` : _config-type_
    - `CNFNUM` : _config-number_
    - `N` : _number-of-readings_
    - `SORTORDER` : _sort-order_

Acquires SLC BPM orbit data, including the `name`, `x` (mm), `y` (mm), `tmit` (num particles), `z` position (m), `hsta`
and `stat` of each bpm in a given display group, under a given measurement definition.

Makes a BPM reading of every BPM in the `display-group-mnemonic` specified in the INSTANCE part of the query, under the BPM _measurement definition_ specified by the `BPMD` parameter.

@note Doing a BPM reading has the potential of taking a long time.  You may need to
[increase the timeout for a request](@ref increaseTimeoutBpm) to make it run to completion.  

### Arguments

| Argument Names | Argument Values                   | Description                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          | 
|----------------|-----------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `BPMD`*        | `<measurement-definition-number>` | _integer_<br/>This specifies the timing profile of the acquisition.<br />Each BPMD corresponds to exactly one DGRP. <br/><br/>To find likely BPMDs, go to the BPM Device panel<br/> of a SCP and hit the 'HELP' button.  <br/>Then select the button corresponding to <br/>the bpm orbit you would have wanted<br /> to see. <br/>The button name for that acquisition, as displayed<br /> in the help, is the BPMD for the selected orbit. Use<br /> that number for this parameter. <br/><br/>Eg, for 'HER Bunch train' `BPMD=38`. |
| `CNFTYPE`      | `<config-type>`                   | _string_                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             |
|                | `NONE`                            | Absolute orbit. This is the default                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  |
|                | `GOLD`                            | Diff to the golden config                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            |
|                | `NORMAL`                          | Diff to the normal config whose number must be given<br /> in the accompanying `CNFNUM` parameter                                                                                                                                                                                                                                                                                                                                                                                                                                    |
|                | `SCRATCH`                         | Diff to the scratch config whose number must be given<br /> in the accompanying `CNFNUM` parameter                                                                                                                                                                                                                                                                                                                                                                                                                                   |
|                | `LOADED`                          | Diff to the temp config whose number must be given<br /> in the accompanying `CNFNUM` parameter                                                                                                                                                                                                                                                                                                                                                                                                                                      |
|                | `TEMPORARY`                       | Diff to the last loaded config from any process, including<br /> another SCP                                                                                                                                                                                                                                                                                                                                                                                                                                                         |
| `CNFNUM`       | `<config-number>`                 | _positive integer_<br/>The number of the bpm device configuration, to whose<br /> values you want the difference to the present readings.<br /> See a SCP bpm config panel for a list of the present<br /> bpm configs                                                                                                                                                                                                                                                                                                               |
| `N`            | `<number-of-readings>`            | _positive integer_<br/>`1 =< N >= 10000`: Default is `1`. <br/>The "NAvg" argument. If the BPMD defines<br /> a ring, then `N` is interpreted as the number of turns;<br /> if BPMD defines a transport, then `N` is interpreted<br /> as readings to average (in the processor)                                                                                                                                                                                                                                                     |
| `SORTORDER`    | `<sort-order>`                    | _positive integer_<br/>`1` or `2`. Default is `2`. <br/>This refers to the order of the returned<br /> BPMs. In PEPII, the z position of BPMS is such that<br /> the greatest and least z (one complete turn) breaks<br /> on a micro boundary, before the micro in charge of<br /> the injection region                                                                                                                                                                                                                             |
|                | `1`                               | gives the BPMs in the order used in SCP steering displays                                                                                                                                                                                                                                                                                                                                                                                                                                                                            |
|                | `2`                               | gives the BPMs in the order used in SCP BPM device panel<br /> displays, and is oriented towards displaying injection<br /> to injection                                                                                                                                                                                                                                                                                                                                                                                             |

### Return value

| TYPE    | Return Column | Column Type     | Description                                                                      |
|---------|---------------|-----------------|----------------------------------------------------------------------------------|
| `TABLE` | `name`        | `STRING_ARRAY`  | name of each BPM                                                                 |
|         | `x`           | `FLOAT_ARRAY`   | x offsets (mm)                                                                   |
|         | `y`           | `FLOAT_ARRAY`   | y offsets (mm)                                                                   |
|         | `tmits`       | `FLOAT_ARRAY`   | tmits (num particles)                                                            |
|         | `z`           | `FLOAT_ARRAY`   | z positions (meters)                                                             |
|         | `hsta`        | `INTEGER_ARRAY` | (32 bit field); to interpret see VMS help: <br/>`MCCDEV> help @slchelp prim bpms` |
|         | `stat`        | `INTEGER_ARRAY` | (32 bit field)                                                                   |


### Examples
#### Operations
 - `P2BPMHER:BPMS BPMD=55`
 - `LCLS_SL2:BPMS BPMD=38`
 - `P2BPMHER:BPMS BPMD=38 SORTORDER=1 N=1024`

#### Response
| BPM Name         | y offset     | x offset      | num particles | z position | hsta stat   | stat   |
|------------------|--------------|---------------|-------------|-----------|-------------|--------|
|                  | _mm_           | _mm_            | _coulomb_     | _meters_    |             |        |
| `name`           | `x`          | `y`           | `tmits`     | `z`       | `hsta stat` | `stat` |
| `BPMS:PR10:8022` | `0.066477716` | `-0.066477716` | `0.0`       | `3333.5989` | `17`        | `0`    |
| `BPMS:PR10:8032` | `1.0967255`  | `-1.0967255`  | `0.0`       | `3356.7275` | `17`        | `0`    |
| `BPMS:PR10:8042` | `0.058775872` | `-0.058775872` | `0.0`       | `3364.437` | `17`        | `0`    |
| `BPMS:PR10:9012` | `-0.9340391` | `0.9340391`   | `0.0`       | `3373.0083` | `17`        | `0`    |
| `BPMS:PR10:9022` | `0.051719327` | `-0.051719327` | `0.0`       | `3380.6082` | `17`        | `0`    |
| `BPMS:PR10:9032` | `1.00613`    | `-1.00613`    | `0.0`       | `3388.2083` | `17`        | `0`    |
| `BPMS:PR10:9042` | `0.09205267` | `-0.09205267` | `0.0`       | `3395.808` | `17`        | `0`    |
| `BPMS:PR10:9052` | `-0.9331402` | `0.9331402`   | `0.0`       | `3403.4082` | `17`        | `0`    |
| `BPMS:PR10:9062` | `0.08750657` | `-0.08750657` | `0.0`       | `3411.0083` | `17`        | `0`    |
| `BPMS:PR10:9072` | `1.0201329`  | `-1.0201329`  | `0.0`       | `3418.6082` | `17`        | `0`    |
| `BPMS:PR10:9082` | `0.031224214` | `-0.031224214` | `0.0`       | `3426.2083` | `17`        | `0`    |
| `BPMS:PR10:9092` | `-0.9497527` | `0.9497527`   | `0.0`       | `3433.808` | `17`        | `0`    |
| `BPMS:PR10:9102` | `0.004137807` | `-0.004137807` | `0.0`       | `3441.4082` | `17`        | `0`    |
| `BPMS:PR10:9112` | `1.0184262`  | `-1.0184262`  | `0.0`       | `3449.0083` | `17`        | `0`    |
| `BPMS:PR10:9122` | `0.09096262` | `-0.09096262` | `0.0`       | `3456.6082` | `17`        | `0`    |
| ...              | ...          | ...           | ...         | ...       | ...         | ...    |
| `BPMS:PR10:6142` | `1.0156763`  | `-1.0156763`  | `0.0`       | `3226.8562` | `17`        | `0`    |
| `BPMS:PR10:6152` | `0.028162582` | `-0.028162582` | `0.0`       | `3234.4563` | `17`        | `0`    |
| `BPMS:PR10:6162` | `-0.9782702` | `0.9782702`   | `0.0`       | `3242.0562` | `17`        | `0`    |
| `BPMS:PR10:7012` | `0.015536556` | `-0.015536556` | `0.0`       | `3249.6553` | `17`        | `0`    |
| `BPMS:PR10:7022` | `1.0258453`  | `-1.0258453`  | `0.0`       | `3256.5032` | `17`        | `0`    |
| `BPMS:PR10:7032` | `0.04465735` | `-0.04465735` | `0.0`       | `3265.096` | `17`        | `0`    |
| `BPMS:PR10:7042` | `-0.9776291` | `0.9776291`   | `0.0`       | `3287.3416` | `17`        | `0`    |
| `BPMS:PR10:8012` | `1.5865061E`-4 | `-1.5865061E`-4 | `0.0`       | `3313.1792` | `17`        | `0`    |

## Increasing timeout for AIDA-PVA requests {#increaseTimeoutBpm}

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
pvcall -w 30 "P2BPMHER:BPMS" BPMD=38 CNFTYPE=GOLD N=1024
```

</td>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">**eget**</td>

<td class="markdownTableBodyNone">

```shell
eget -w 30 -s P2BPMHER:BPMS -a BPMD 38 -a CNFTYPE GOLD -a N 1024 
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
    builder = pvaRequest('P2BPMHER:BPMS');
    builder.with('BPMD', 38);
    builder.with('cnftype', 'gold');
    builder.with('n', 1024);
    builder.timeout(30);
    table = ML(builder.get());
    labels = table.labels;
    names =  table.values.name
catch e
    handleExceptions(e);
end
names =
  Columns 1 through 7
    'BPMS:PR10:8022'    'BPMS:PR10:8032'    'BPMS:PR10:8042'    'BPMS:PR10:9012'    'BPMS:PR10:9022'    'BPMS:PR10:9032'    'BPMS:PR10:9042'

...

Columns 288 through 293
    'BPMS:PR10:6162'    'BPMS:PR10:7012'    'BPMS:PR10:7022'    'BPMS:PR10:7032'    'BPMS:PR10:7042'    'BPMS:PR10:8012'
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

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.pvaRequest;

public class Example {
    public PvaTable getTable() throws RPCRequestException {
        return (PvaTable) pvaRequest("P2BPMHER:BPMS")
                .with("BPMD", 38)
                .with("CNFTYPE", "GOLD")
                .with("N", 1024)
                .get();
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
import org.epics.pvdata.pv.PVStructure;

public class Example {
    public PVStructure getTable() {
        String pvName = "P2BPMHER:BPMS";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("bpmd")
                .addQueryString("cnftype")
                .addQueryInt("n")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("bpmd").put(38);
        uri.getQuery().getStringField("cnftype").put("GOLD");
        uri.getQuery().getIntField("n").put(1024);

        PvaClient client = PvaClient.get("pva");
        PvaClientChannel channel = client.createChannel(pvName);
        return channel.rpc(uri.getPVStructure());
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

public class Example {
    public PVStructure getTable() throws RPCRequestException {
        String pvName = "P2BPMHER:BPMS";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("bpmd")
                .addQueryString("cnftype")
                .addQueryInt("n")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("bpmd").put(38);
        uri.getQuery().getStringField("cnftype").put("GOLD");
        uri.getQuery().getIntField("n").put(1024);

        RPCClientImpl client = new RPCClientImpl(pvName);
        PVStructure response = client.request(uri.getPVStructure(), 3.0);
        client.destroy();
        return response;
    }
}
```

</td>
</tr>
</table>
