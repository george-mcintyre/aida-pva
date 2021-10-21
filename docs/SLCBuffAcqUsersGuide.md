# SLC Buffered Data Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Buffered Data through AIDA-PVA. This data service gives access to the so called "Buffered BPM Data" acquisition facilities of the SLC Control System, in the SLAC accelerator complex. Buffered BPM data, is the name given to the pulse-by-pulse data acquisition system, in which a user names the devices whose values they want to be acquired, for how many beam pulses, and the service then returns a table of results - the x and y position values and the Tmit, given in one row per device, per pulse.  For general
information on using AIDA-PVA see [Basic Users Guide to Aida](UserGuide.md), and the EPICS javadoc.

# AIDA-PVA Data Provider for SLC Buffered Acquisition Data

## Summary
The SLC Buffered Acquisition Provider gets SLC control system pulse synchronized "Buffered Data", for the following device types:
 - BPM
   - x (mm) 
   - y (mm)
   - tmit (num particles estimated)
 - TORO tmit
 - KLYS and SBST phase 
 - GAPM data 

and status information for these, as specified in the user's query. 
The acquisition is subject to the so-called measurement definition `BPMD` given, over the given
`NRPOS` parameter sequential pulses.

@note 
A table is returned. Since the kind of data one gets back from these device types is all a little different, 
some elements of the table of data you get back should be ignored.


## Examples
| | | 
|  ----------- |----------- |
| pvcall example | ```pvcall NDRFACET:BUFFACQ BPMD=57 NRPOS=5 DEVS='["KLYS:LI03:31","SBST:LI03:001", "BPMS:LI02:501", "TORO:LI20:2040"]'```|
| Java Tests | SlcBuffAcqTest.java | 
| Matlab example |  |

## Instances and Attributes

| | | |
| ----------- | ----------- | ----------- |
| **get** | Syntax    | `<dgrp-mnemonic-name>:BUFFACQ` |
| | Examples | `NDRFACET:BUFFACQ` |
| |  | `INJ_ELEC:BUFFACQ` |
| |  | `INJ_POSI:BUFFACQ` |
| |  | `TAXXALL:BUFFACQ` |

## Attribute operation summary

| | |
| ----------- | -----------  |
| Attribute | Description |
| `BUFFACQ` | Makes a buffered acquisition reading of each given device, in the dgrp specified in the INSTANCE part of the query, under the BPM Measurement Definition specified by the `BPMD` parameter, at each beam crossing for the number of pulses given by `NRPOS`. |

## Attribute operations
### BUFFACQ : get
_Parameters_

| | | |
| ----------- | -----------| ----------- |
| Parameter Names | Parameter Values |Description | 
| `BPMD`*  |   Integer Bpm Measurement Definition number | This specifies the timing profile of the acquisition. Each `BPMD` corresponds to exactly one DGRP. To find likely BPMDs, go to the BPM Device panel of a SCP and hit the 'HELP' button, then select the button corresponding to the bpm orbit you would have wanted to see. The button name for that acquisition, as displayed in the help, is the BPMD for the selected orbit. Use that number for this parameter. Eg `BPMD=57` is the primary BPMD for the DGRP `NDRFACET`, and so will be the main one used for FACET. |
| `NRPOS`| `1 =< n >= 2800` |  Default is 1.  In theory, this parameter can range up to `2800`, but some things must be borne in mind. Firstly, roughly speaking the minimum time it could take a buffered acquisition to complete, is `NRPOS`/machine-rate-in-Hz. But, the SCP in fact adds time to this theoretical minimum heuristically, to give the micros time to prep the acquisition. Secondly, AIDA-PVA's network timeout ranges between 60 and 90 seconds depending on platform, so if your acquisition takes too long, the client will timeout. So, in practice, limit your NRPOS to numbers that will result in relatively short acquisitions, plan for about 30 seconds max. |
| `BPMS`| array of device names | The names of the devices from which a user would like the Buffered service to acquire data. Device names for the following devices:  `BPM`, `TORO`, `GAPM`, `KLYS` or `SBST`. |
| `DEVS`| array of device names | The names of the devices from which a user would like the Buffered service to acquire data. Device names for the following devices:  `BPM`, `TORO`, `GAPM`, `KLYS` or `SBST`. |

_Return value_

@note
Return value for BPM device shown below. `TORO`, `GAPM`, `KLYS` or `SBST` will be different.

| | | | |
|----------- | ----------- | -----------  |-----------  |
| TYPE  | Return Column | Column Type |Description |
| `TABLE` | `name` | `STRING_ARRAY` | name of each BPM |
|  |  `pulseId`| `INTEGER_ARRAY` |  pulse ID |
|  |  `x`| `FLOAT_ARRAY`|  x offsets (mm) |
|  |  `y`| `FLOAT_ARRAY` |  y offsets (mm) |
|  |  `tmits`| `FLOAT_ARRAY`|  tmits (num particles) |
|  |  `stat`| `INTEGER_ARRAY`|  (32 bit field) |
|  |  `goodmeas`| `BOOLEAN_ARRAY`| true = good, false = bad |




