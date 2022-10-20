# 1.4 - SLC Buffered Data Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Buffered Data through AIDA-PVA. This
data service gives access to the so called "Buffered BPM Data" acquisition facilities of the SLC Control System, in the
SLAC accelerator complex.
For general information on xusing AIDA-PVA see [Basic Users Guide to Aida](1_00_User_Guide.md), and the EPICS javadoc.

## Summary

Acquires SLC control system pulse synchronized "Buffered Data" and status information for the given set of devices and
pulses.

## Synopsis

|                          | PV                                 | Arguments                 | Description |
|:-------------------------|------------------------------------|---------------------------|:------------|
| [1.4.1](@ref section141) | `<display-group-mnemonic>:BUFFACQ` | `BPMD` `BPMS` [ `NRPOS` ] | Buffered  Acquisition  |
|                          | `<display-group-mnemonic>:BUFFACQ` | `BPMD` `DEVS` [ `NRPOS` ] |             |

## 1.4.1. Buffered Acquisition {#section141}

- `<display-group-mnemonic>:BUFFACQ`
    - `BPMD`* : _measurement-definition-number_
    - `BPMS`* : _list-of-device-names_
    - `NRPOS` : _number-of-pulses_
- `<display-group-mnemonic>:BUFFACQ`
    - `BPMD`* : _measurement-definition-number_
    - `DEVS`* : _list-of-device-names_
    - `NRPOS` : _number-of-pulses_

Buffered BPM data, is the name given to the pulse-by-pulse data acquisition system.

Makes a buffered acquisition reading of each given device, in the measurement definition group specified in the INSTANCE
part of the query, under the BPM Measurement Definition specified by the `BPMD` argument, at each beam crossing for the
number of pulses given by `NRPOS`.

The service returns a table of results - the `x` and `y` position values and the `tmit`, given in one row per device,
per pulse.

The following device types are supported:

- BPM
    - x (mm)
    - y (mm)
    - tmit (num particles estimated)
- TORO
    - tmit
- KLYS and SBST
    - phase
- GAPM data

@note A table is returned. Since the kind of data one gets back from these device types may differ, some elements of the
table of data you get back should be ignored.

### Arguments

| Argument Names | Argument Values                   | Description                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           | 
|----------------|-----------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `BPMD`*        | `<measurement-definition-number>` | _integer_<br/>This specifies the timing profile of the acquisition.<br /> Each `BPMD` corresponds to exactly one DGRP. To find<br /> likely BPMDs, go to the BPM Device panel of a SCP<br /> and hit the 'HELP' button, then select the button<br /> corresponding to the bpm orbit you would have wanted<br /> to see. The button name for that acquisition, as displayed<br /> in the help, is the BPMD for the selected orbit. Use<br /> that number for this parameter. Eg `BPMD=57` is the<br /> primary BPMD for the DGRP `NDRFACET`, and so will<br /> be the main one used for FACET.                                                                                                                                                                                                                                         |
| `BPMS`*        | `<list-of-device-names>`          | The names of the devices from which a user would like<br /> the Buffered service to acquire data. Device names<br /> for the following devices:  `BPM`, `TORO`, `GAPM`,<br /> `KLYS` or `SBST`.<br/>Either `BPMS` or `DEVS` must be specified but not both                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            |
| `DEVS`*        | `<list-of-device-names>`          | The names of the devices from which a user would like<br /> the Buffered service to acquire data. Device names<br /> for the following devices:  `BPM`, `TORO`, `GAPM`,<br /> `KLYS` or `SBST`.<br/>Either `BPMS` or `DEVS` must be specified but not both                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            |
| `NRPOS`        | `<number-of-pulses>`              | _positive integer_<br/>`1 =< N >= 10000`: Default is `1`. <br/>In theory, this parameter can range up<br />  to `2800`, but some things must be borne in mind.<br /><br/>Firstly, roughly speaking the minimum time it could<br />  take a buffered acquisition to complete, is `NRPOS`/machine-rate-in-Hz.<br />  But, the SCP in fact adds time to this theoretical<br />  minimum heuristically, to give the micros time to<br />  prep the acquisition. <br/><br/>Secondly, AIDA-PVA's network's default timeout<br />  is small so you will need to increase it <br/>if you expect the acquisition will take more <br/>than a few seconds.  <br/><br/>So, in practice, limit your  NRPOS to numbers<br/> that will result in relatively short<br />  acquisitions, or [increase the timeout](@ref increaseTimeoutBuffAcq).<br /> |

### Return value

| TYPE    | Return Column | Column Type     | Description              |
|---------|---------------|-----------------|--------------------------|
| `TABLE` | `name`        | `STRING_ARRAY`  | name of each BPM         |
|         | `pulseId`     | `INTEGER_ARRAY` | pulse ID                 |
|         | `x`           | `FLOAT_ARRAY`   | x offsets (mm)           |
|         | `y`           | `FLOAT_ARRAY`   | y offsets (mm)           |
|         | `tmits`       | `FLOAT_ARRAY`   | tmits (num particles)    |
|         | `stat`        | `INTEGER_ARRAY` | (32 bit field)           |
|         | `goodmeas`    | `BOOLEAN_ARRAY` | true = good, false = bad |

@note Return value for BPM device shown below. `TORO`, `GAPM`, `KLYS` or `SBST` will be different.

### Examples

#### Operations

- `NDRFACET:BUFFACQ BPMD=57 BPMS=["BPMS:LI11:501"]`
- `NDRFACET:BUFFACQ BPMD=57 NRPOS=180 BPMS=["BPMS:LI11:501", "BPMS:LI11:601", "BPMS:LI11:701", "BPMS:LI11:801"]`

#### Response

| BPM Name         |  pulse ID | y offset       | x offset       | num particles | stat   | goodmeas   |
|------------------|----------:|----------------|----------------|---------------|--------|------------|
|                  |           | _mm_           | _mm_           | _coulomb_     |        |            |
| `name`           | `pulseId` | `x`            | `y`            | `tmits`       | `stat` | `goodmeas` |
| `BPMS:PR10:8022` |     71312 | `0.066477716`  | `-0.066477716` | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:8032` |     71313 | `1.0967255`    | `-1.0967255`   | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:8042` |     71314 | `0.058775872`  | `-0.058775872` | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:9012` |     71315 | `-0.9340391`   | `0.9340391`    | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:9022` |     71316 | `0.051719327`  | `-0.051719327` | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:9032` |     71317 | `1.00613`      | `-1.00613`     | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:9042` |     71318 | `0.09205267`   | `-0.09205267`  | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:9052` |     71319 | `-0.9331402`   | `0.9331402`    | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:9062` |     71320 | `0.08750657`   | `-0.08750657`  | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:9072` |     71321 | `1.0201329`    | `-1.0201329`   | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:9082` |     71322 | `0.031224214`  | `-0.031224214` | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:9092` |     71323 | `-0.9497527`   | `0.9497527`    | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:9102` |     71324 | `0.004137807`  | `-0.004137807` | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:9112` |     71325 | `1.0184262`    | `-1.0184262`   | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:9122` |     71326 | `0.09096262`   | `-0.09096262`  | `1.0E-10`     | `1`    | `15`       |
| ...              |       ... | ...            | ...            | ...           | ...    | ...        |
| `BPMS:PR10:6142` |     71328 | `1.0156763`    | `-1.0156763`   | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:6152` |     71329 | `0.028162582`  | `-0.028162582` | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:6162` |     71330 | `-0.9782702`   | `0.9782702`    | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:7012` |     71331 | `0.015536556`  | `-0.015536556` | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:7022` |     71332 | `1.0258453`    | `-1.0258453`   | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:7032` |     71333 | `0.04465735`   | `-0.04465735`  | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:7042` |     71334 | `-0.9776291`   | `0.9776291`    | `1.0E-10`     | `1`    | `15`       |
| `BPMS:PR10:8012` |     71335 | `1.5865061E`-4 | `-1.5865061`   | `1.0E-10`     | `1`    | `15`       |

## Increasing timeout for AIDA-PVA requests {#increaseTimeoutBuffAcq}

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
pvcall -w 30 "NDRFACET:BUFFACQ" TIMEOUT=30 BPMD=57 NRPOS=10 BPMS='["BPMS:LI02:501", "BPMS:DR12:334"]'
```

</td>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">**eget**</td>

<td class="markdownTableBodyNone">

```shell
eget -w 30 -s NDRFACET:BUFFACQ -a TIMEOUT 30 -a BPMD 57 -a NRPOS 10 -a BPMS '["BPMS:LI02:501", "BPMS:DR12:334"]' 
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
    builder = pvaRequest('NDRFACET:BUFFACQ');
    builder.with('BPMD', 57);
    builder.with('NRPOS', 10);
    builder.with('BPMS', { 'BPMS:LI11:501' 'BPMS:DR12:334'});
    builder.timeout(30);
    mstruct = ML(builder.get())
    mstruct.values.x(1:10)
catch e
    handleExceptions(e);
end
mstruct =
            size: 20
          labels: {'BPM Name'  'pulse id'  'x offset (mm)'  'y offset (mm)'  'num particles (coulomb)'  'stat'  'good measurement'}
           units: []
    descriptions: []
      fieldnames: {'name'  'pulseId'  'x'  'y'  'tmits'  'stat'  'goodmeas'}
          values: [1x1 struct]

ans =
   -0.0466    0.4037   -0.5933    0.9624   -0.8016    0.9845   -0.0871    0.9946    0.3572    0.9041
```

</td>
</tr>
</table>

### Java Examples

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">example type</th><th class="markdownTableHeadNone">action</th><th class="markdownTableHeadNone">example</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">**AidaPvaClient**</td>

<td class="markdownTableBodyNone">

```java
import java.util.List;

import edu.stanford.slac.aida.client.PvaTable;
import org.epics.pvaccess.server.rpc.RPCRequestException;

import static edu.stanford.slac.aida.client.AidaPvaClientUtils.pvaRequest;

public class Example {
    public PvaTable getTable() throws RPCRequestException {
        return (PvaTable) pvaRequest("NDRFACET:BUFFACQ")
                .with("BPMD", 57)
                .with("NRPOS", 10)
                .with("BPMS", List.of("BPMS:LI02:501", "BPMS:DR12:334"))
                .timeout(30)
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
import org.epics.pvdata.pv.PVStructure;

public class Example {
    public PVStructure getTable() {
        String pvName = "NDRFACET:BUFFACQ";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("bpmd")
                .addQueryString("bpms")
                .addQueryInt("nrpos")
                .addQueryInt("timeout")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("bpmd").put(57);
        uri.getQuery().getStringField("bpms").put("[\"BPMS:LI02:501\", \"BPMS:DR12:334\"]");
        uri.getQuery().getIntField("nrpos").put(10);
        uri.getQuery().getIntField("timeout").put(30);

        PvaClient client = PvaClient.get();
        PvaClientChannel channel = client.channel(pvName, "pva", 30.0);
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
        String pvName = "NDRFACET:BUFFACQ";

        NTURI uri = NTURI.createBuilder()
                .addQueryInt("bpmd")
                .addQueryString("bpms")
                .addQueryInt("nrpos")
                .addQueryInt("timeout")
                .create();
        uri.getPath().put(pvName);
        uri.getQuery().getIntField("bpmd").put(57);
        uri.getQuery().getStringField("bpms").put("[\"BPMS:LI02:501\", \"BPMS:DR12:334\"]");
        uri.getQuery().getIntField("nrpos").put(10);
        uri.getQuery().getIntField("timeout").put(30);

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
