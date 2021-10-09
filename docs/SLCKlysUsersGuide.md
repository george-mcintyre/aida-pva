This page provides documentation for users and programmers of AIDA's SLC Klystron Data Provider. This data provider allows the retrieval of the status code or a status string for a specified klystron on a beam code. It also allows the deactivation or reactivation of a specified klystron on a beam code. The PDES value for a klystron or subbooster may be set and the phase may be optionally trimmed. The KPHR value for a klystron or subboster may be set. Finally, a configuration value (PCON or ACON) of a specified klystron or subbooster may be set.

See Also: Basic Users Guide to Aida, REF_AIDASHR; AIDA SLC Peer Programmers Guide

Users Guide
This section descibes what an AIDA user should know about using the SLC Klystron data provider. For general information on using AIDA see Basic Users Guide to Aida, and the Aida javadoc, in particular the classes DaObject and DaReference, in aida.lib.da which form Aida's programming interface. One can also use Aida through Matlab.

Table 1: Summary of AIDA Data Provider for SLC Klystron Data

SUMMARY
Supports get and set operations. The get operation obtains the status code or a status string for a specified klystron on a beam code. There are four set operations that can be performed: (1) deactivate or reactivate a specified klystron on a beam code, (2) set the PDES value and optionally trim the phase of a specified klystron or subbooster, (3) set the KPHR value of a specified klystron or subbooster, and (4) set a configuration value (PCON or ACON) of a specified klystron or subbooster.
Status and limitations	
The data provider is complete and there are no known limitations

Plan	No active plans for extensions.

EXAMPLES
Schematic	Java examples, see DpSlcKlysTests.java for more examples:
String query = "KLYS:LI31:31:TACT";
da.setParam("BEAM=8");         /* Required parameter specifying a beam code. */
da.setParam("DGRP=DEV_DGRP");  /* Display group must be specified if klystron is not in display group LIN_KLYS. */
Short v = (Short) da.get(query, DaValue.Type.SHORT); /* Get the status code for the klystron on the beam code. */ 

String query = "KLYS:LI31:31:TACT";
da.setParam("BEAM=8");         /* Required parameter specifying a beam code. */
da.setParam("DGRP=DEV_DGRP");  /* Display group must be specified if klystron is not in display group LIN_KLYS. */
String v = (String) da.get(query, DaValue.Type.STRING); /* String type indicates to get the status string. */ 

String query = "KLYS:LI31:31:TACT";
da.setParam("BEAM=8");         /* Required parameter specifying a beam code. */
da.setParam("DGRP=DEV_DGRP");  /* Display group must be specified if klystron is not in display group LIN_KLYS. */
Short setValue = new Short((short) 0); /* Flag 0 indicates a deactivation operation. */
DaValue inData = new DaValue(setValue); /* Creation a DaValue to hold operation flag. */
DaValue outData = da.setDaValue(query, inData); /* Perform specified operation to klystron on beam code. */

String query = "KLYS:LI31:31:PDES"; /* Indication of set PDES and optional trim phase operation. */
Float pdesValue = new Float(90.0f); /* Specified desired PDES value. */
DaValue inData = new DaValue(pdesValue); /* Creation of DaValue to hold pdesValue. */
DaValue outData = da.setDaValue(query, inData); /* Perform set PDES and trim phase operation. */

String query = "KLYS:LI31:31:KPHR"; /* Indication of set KPHR value operation. */
Float kphrValue = new Float(60.0f); /* Specified desired KPHR value. */
DaValue inData = new DaValue(kphrValue); /* Creation of DaValue to hold kphrValue. */
DaValue outData = da.setDaValue(query, inData); /* Perform set KPHR value operation. */

String query = "KLYS:LI31:31:PCON"; /* Indication of set PCON value operation. */
Float newValue = new Float(5.0f); /* Specified desired PCON value. */
DaValue inData = new DaValue(newValue); /* Creation of DaValue to hold newValue. */
da.setDaValue(query, inData); /* Perform set PCON value operation (no value is returned). */
Java	$CD_SOFT/ref/package/aida/test/java/DpSlcKlysTests.java
Matlab	$CD_SOFT/ref/package/aida/test/matlab/klysGetDemo.m gets status of a klystron;
$CD_SOFT/ref/package/aida/test/matlab/klysSetDemo.m deactivates or reactivates a klystron.
$CD_SOFT/ref/package/aida/test/matlab/klysSetTrimDemo.m sets the PDES value and trims the phase of a klystron or subbooster.
$CD_SOFT/ref/package/aida/test/matlab/klysSetKphrDemo.m sets the KPHR value of a klystron or subbooster.

INSTANCES and ATTRIBUTES
Instance Type	Description
Get	Syntax	<prim>:<micr>:<unit>:TACT
Examples	
KLYS:LI31:31:TACT

Set	Syntax	<prim>:<micr>:<unit>:TACT
<prim>:<micr>:<unit>:PDES
<prim>:<micr>:<unit>:KPHR
<prim>:<micr>:<unit>:PCON
<prim>:<micr>:<unit>:ACON
Examples	
KLYS:LI31:31:TACT
KLYS:LI31:31:PDES
KLYS:LI31:31:KPHR
KLYS:LI31:31:PCON
KLYS:LI31:31:ACON

Attributes
Attribute	Description
TACT	Gets a status code or a status string for the specified klystron on a beam code.
Methods	Name*	Returns
get(d,DaValue.Types.SHORT)	A short value containing the status code for the klystron on a beam code - see linklysta.txt
get(d,DaValue.Types.LONG)	A long value containing the status code for the klystron on a beam code - see linklysta.txt
get(d,DaValue.Types.STRING)	A string value containing a status string having one of two values: "deactivated" or "activated".
Parameters	Name	
Req/
Opt

Syntax	Semantics
BEAM	
req

Integer	Beam code number
DGRP	opt	Dgrp name	A display group associated with the specified klystron. Must be specified if the klystron does not belong to display group LIN_KLYS.
TACT	Deactivates or reactivates a specified klystron on a beam code. The parameter data argument (p) is a DaValue containing a Short flag code indicating the desired operation: 0 => deactivate, 1 => reactivate.
Methods	Name*	Returns
setDaValue(q, p)	Returns a DaValue containing a matrix having one value of type short: the status code for the specified klystron on a beam code - see linklysta.txt
Parameters	Name	
Req/
Opt

Syntax	Semantics
BEAM	req	Integer	Beam code number
DGRP	opt	Dgrp name	A display group associated with the specified klystron. Must be specified if the klystron does not belong to display group LIN_KLYS.
PDES	Sets the PDES value and optionally trims the phase for a specified klystron or subbooster. The parameter data argument (p) is a DaValue containing a Float with the desired PDES value.
Methods	Name*	Returns
setDaValue(q, p)	Returns a DaValue containing a matrix having one value of type float: the PHAS secondary value after the set PDES and optional trim phase operation.
Parameters	Name	
Req/
Opt

Syntax	Semantics
TRIM	opt	YES or NO	If specified and set to NO, no trim phase operation will be performed after setting the PDES value. If not specified or set to YES, the trim phase operation will be performed.
KPHR	Sets the KPHR value of a specified klystron or subbooster. The parameter data argument (p) is a DaValue containing a Float with the desired KPHR value.
Methods	Name*	Returns
setDaValue(q, p)	Returns a DaValue containing a matrix having one value of type float: the PHAS secondary value after the set KPHR operation.
Parameters	None
PCON or ACON	Sets the PCON or ACON value of a specified klystron or subbooster. The parameter data argument (p) is a DaValue containing a Float with the desired PCON or ACON value.
Methods	Name*	Returns
setDaValue(q, p)	No data is returned.
Parameters	None
* See DaObject and DaReference (and DaReference's parent _DaReference) in aida.lib.da for full API and method signatures.

