This page provides documentation for users and programmers of AIDA's SLC Beam Position Monitor (BPM) Data Provider. This data provider gives the orbit offset read by BPMs in the SLAC accelerator complex.

See Also: Basic Users Guide to Aida, REF_AIDASHR; AIDA SLC Peer Programmers Guide

Users Guide
This section descibes what an AIDA user should know about accessing SLC Bpm Data through AIDA. For general information on using AIDA see Basic Users Guide to Aida, and the Aida javadoc, in particular the classes DaObject and DaReference, in aida.lib.da which form Aida's programming interface. One can also use Aida through Matlab.

SUMMARY
Aquires SLC BPM orbit data, including the name, x (mm), y (mm), tmit (num particles), z position (m), HSTA and STAT of each bpm in a given DGRP, under a given measurement definition.
Status and limitations	
Presently acquires data for every BPM in a given DGRP, not by given bpms' names.

Names	AIDA entities for making BPM orbit acquisitions, are composed of DGPR name, with attribute == "BPMS" (see INSTANCES and ATTRIBUTES below). See aidalist % BPMS for an up-to-date list. The following names exist at the time of writing:
Plan	No active plans for extension, but could add (i) ability to acquire only specifically named devices; (ii) add acquisition of other timing related devices; (iii) implement get_<array-type> Aida methods for super-fast acquisition of only x or y orbit data.

EXAMPLES
Schematic	Java example, see DpSlcBpmTests.java for more examples:
String query  = "P2BPMHER//BPMS";
da.setParam("BPMD=38");             /* Public Measurement Definition parameter */
da.setParam("CNFTYPE=GOLD");       /* Return diff to the GOLD orbit */
da.setParam("N=1024");              /* "Navg", for ring, the number of turns (Optional param) */
DaValue v = da.getDaValue(query)   ;   /* Acquire BPM data */
Java	$CD_SOFT/ref/package/aida/test/java/DpSlcBpmTests.java
Matlab	$CD_SOFT/ref/package/aida/test/matlab/orbitDemo.m acquires a BPM orbit; $CD_SOFT/ref/package/aida/test/matlab/plotOrbitDemo.m plots the x, y, and tmit from an orbit acquired by orbitDemo.m. See screenshot of how these were used together to acquire and plot an orbit.

???
INSTANCES and ATTRIBUTES
Instance Type	Description
BPM Orbit	Syntax	<dgrp-mnemonic>//BPMS
Examples	
P2BPMHER//BPMS

Attributes
Attribute	Description
BPMS	Makes a BPM reading of every BPM in the dgrp specified in the "instance" part of the query, under the BPM Measurement Definition specified by the BPMD (required) parameter.
Methods	Name*	Returns
getDaValue(q)	Returns a DaValue containing a heterogenous vector of 7 homogenous congruent vectors;
name of each BPM
x offsets (mm)
y offsets (mm)
tmits (num particles)
z positions (meters)
HSTAs (32 bit field); to interpret see VMS help: MCCDEV> help @slchelp prim bpms
STATs (32 bit field)
Parameters	Name	
Req/
Opt

Syntax	Semantics
BPMD	
req

Integer Bpm Measurement Definition number.	This specifies the timing profile of the acquisition. Each BPMD corresponds to exactly one DGRP. To find likely BPMDs, go to the BPM Device panel of a SCP and hit the 'HELP' button, then select the button corresponding to the bpm orbit you would have wanted to see. The button name for that acquisition, as displayed in the help, is the BPMD for the selected orbit. Use that number for this parameter. Eg, for 'HER Bunch train' BPMD=38.
CNFTYPE	opt	
'NONE' | 'GOLD' | 'NORMAL' | 'SCRATCH' | 'LOADED' | 'TEMPORARY'

NONE : The absolute orbit. This is the default.

GOLD : Diff to the golden config

NORMAL : Diff to the normal config whose number must be given in the accompanying CNFNUM parameter.

SCRATCH : Diff to the scratch config whose number must be given in the accompanying CNFNUM parameter.

TEMPORARY : Diff to the temp config whose number must be given in the accompanying CNFNUM parameter.

LOADED : diff to the last loaded config from any process, inlcuding another SCP!

 

CNFNUM	opt	+ve integer.	The number of the bpm device configuration, to whose values you want the difference to the present readings. See a SCP bpm config panel for a list of the present bpm configs.
N	opt	1=<n>=;10000. Default is 1	The "NAvg" argument. If the BPMD defines a ring, then N is interpreted as the number of turns; if BPMD defines a transport, then N is interpreted as readings to average (in the processor.
SORTORDER	opt	1|2. Default is 2	This refers to the order of the returned BPMs. In PEPII, the z position of BPMS is such that the greatest and least z (one complete turn) breaks on a micro boundary, before the micro in charge of the injection region. SORTORDER=1 gives the BPMs in this order, as is used in SCP steering displays. SORTORDER=2 is that used in SCP BPM device panel displays, and is oriented towards displaying injection to injection.
* See DaObject and DaReference (and DaReference's parent _DaReference) in aida.lib.da for full API and method signatures.

