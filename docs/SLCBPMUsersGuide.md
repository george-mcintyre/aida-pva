# SLC Bpm Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Bpm data through AIDA-PVA. For general
information on using AIDA-PVA see [Basic Users Guide to Aida](UserGuide.md), and the EPICS javadoc. 

# AIDA-PVA Data Provider for SLC BPM Data

## Summary
Aquires SLC BPM orbit data, including the `name`, `x` (mm), `y` (mm), `tmit` (num particles), `z` position (m), `HSTA` and `STAT` of each bpm in a given DGRP, under a given measurement definition.

## Examples
| | | 
|  ----------- |----------- |
| pvcall example | `pvcall "P2BPMHER:BPMS" BPMD=38 CNFTYPE=GOLD N=1024`|
| Java Tests | SlcBpmTest.java | 
| Matlab example |  |

## Instances and Attributes

| | | |
| ----------- | ----------- | ----------- |
| **get** | Syntax    | `<dgrp-mnemonic>:BPMS` |
| | Examples | `P2BPMHER:BPMS` |

## Attribute operation summary

| | |
| ----------- | -----------  |
| Attribute | Description |
| `BPMS` | Makes a BPM reading of every BPM in the `dgrp` specified in the INSTANCE part of the query, under the BPM Measurement Definition specified by the `BPMD` parameter. |

## Attribute operations
### BPMS : get
_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `BPMD`*  |   Integer Bpm Measurement Definition number | This specifies the timing profile of the acquisition. Each BPMD corresponds to exactly one DGRP. To find likely BPMDs, go to the BPM Device panel of a SCP and hit the 'HELP' button, then select the button corresponding to the bpm orbit you would have wanted to see. The button name for that acquisition, as displayed in the help, is the BPMD for the selected orbit. Use that number for this parameter. Eg, for 'HER Bunch train' BPMD=38. |
| `CNFTYPE`| `NONE` |  Absolute orbit. This is the default |
| | `GOLD` |  Diff to the golden config |
| | `NORMAL` |  Diff to the normal config whose number must be given in the accompanying `CNFNUM` parameter |
| | `SCRATCH` |  Diff to the scratch config whose number must be given in the accompanying `CNFNUM` parameter |
| | `LOADED` |  Diff to the temp config whose number must be given in the accompanying `CNFNUM` parameter |
| | `TEMPORARY` |  diff to the last loaded config from any process, including another SCP |
| `CNFNUM`| +ve integer |  The number of the bpm device configuration, to whose values you want the difference to the present readings. See a SCP bpm config panel for a list of the present bpm configs |
| `N`| 	`1 =< n >= 10000` | Default is 1. The "NAvg" argument. If the BPMD defines a ring, then `N` is interpreted as the number of turns; if BPMD defines a transport, then `N` is interpreted as readings to average (in the processor |
| `SORTORDER`| `1` or `2` | Default is 2.  This refers to the order of the returned BPMs. In PEPII, the z position of BPMS is such that the greatest and least z (one complete turn) breaks on a micro boundary, before the micro in charge of the injection region. `SORTORDER`=`1` gives the BPMs in this order, as is used in SCP steering displays. `SORTORDER`=`2` is that used in SCP BPM device panel displays, and is oriented towards displaying injection to injection |

_Return value_

| | | | |
|----------- | ----------- | -----------  |-----------  |
| TYPE  | Return Column | Column Type |Description |
| `TABLE` | `name` | `STRING_ARRAY` | name of each BPM |
|  |  `x`| `FLOAT_ARRAY`|  x offsets (mm) |
|  |  `y`| `FLOAT_ARRAY` |  y offsets (mm) |
|  |  `tmits`| `FLOAT_ARRAY`|  tmits (num particles) |
|  |  `z`| `FLOAT_ARRAY` |  z positions (meters) |
|  |  `hsta`| `INTEGER_ARRAY`|  (32 bit field); to interpret see VMS help: `MCCDEV> help @slchelp prim bpms` |
|  |  `stat`| `INTEGER_ARRAY`|  (32 bit field) |


