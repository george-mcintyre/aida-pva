This page provides documentation for users and programmers of AIDA's SLC Trigger Data Provider. This data provider allows the retrieval of the status code or a status string for a specified trigger device on a beam code. It also allows the deactivation or reactivation of a specified trigger device on a beam code.

See Also: Basic Users Guide to Aida, REF_AIDASHR; AIDA SLC Peer Programmers Guide

Users Guide
This section descibes what an AIDA user should know about using the SLC Trigger data provider. For general information on using AIDA see Basic Users Guide to Aida, and the Aida javadoc, in particular the classes DaObject and DaReference, in aida.lib.da which form Aida's programming interface. One can also use Aida through Matlab.

Table 1: Summary of AIDA Data Provider for SLC Trigger Data

SUMMARY
Supports get and set operations. The get operation obtains the status code or a status string for a specified trigger device on a beam code. The set operation deactivates or reactivates a specified trigger device on a beam code.
Status and limitations	
The data provider is complete and there are no known limitations

Plan	No active plans for extensions.

EXAMPLES
Schematic	Java examples, see DpSlcUtilTests.java for more examples:
String query = "TRIG:LI31:109:TACT";
da.setParam("BEAM=1");         /* Required parameter specifying a beam code. */
Short v = (Short) da.get(query, DaValue.Type.SHORT); /* Get the status code for the trigger device on the beam code. */ 

String query = "TRIG:LI31:109:TACT";
da.setParam("BEAM=1");         /* Required parameter specifying a beam code. */
String v = (String) da.get(query, DaValue.Type.STRING); /* String type indicates to get the status string. */ 

String query = "TRIG:LI31:109:TACT";
da.setParam("BEAM=1");         /* Required parameter specifying a beam code. */
Short setValue = new Short((short) 0); /* Flag 0 indicates a deactivation operation. */
DaValue inData = new DaValue(setValue); /* Creation a DaValue to hold operation flag. */
DaValue outData = da.setDaValue(query, inData); /* Perform specified operation to trigger device on beam code. */

Java	$CD_SOFT/ref/package/aida/test/java/DpSlcUtilTests.java
Matlab	$CD_SOFT/ref/package/aida/test/matlab/trigGetDemo.m gets status of a trigger device;
$CD_SOFT/ref/package/aida/test/matlab/trigSetDemo.m deactivates or reactivates a trigger device.

INSTANCES and ATTRIBUTES
Instance Type	Description
Get	Syntax	<prim>:<micr>:<unit>//TACT
Examples	
TRIG:LI31:109:TACT

Set	Syntax	<prim>:<micr>:<unit>//TACT
Examples	
TRIG:LI31:109:TACT

Attributes
Attribute	Description
TACT	Gets a status code or a status string for the specified trigger device on a beam code.
Methods	Name*	Returns
get(d,DaValue.Types.SHORT)	A short value containing the status code for the trigger device on a beam code: 0 => deactivated, 1 => activated.
get(d,DaValue.Types.LONG)	A long value containing the status code for the trigger device on a beam code: 0 => deactivated, 1 => activated.
get(d,DaValue.Types.STRING)	A string value containing a status string having one of two values: "deactivated" or "activated".
Parameters	Name	
Req/
Opt

Syntax	Semantics
BEAM	
req

Integer	Beam code number
TACT	Deactivates or reactivates a specified trigger device on a beam code. The parameter data argument (p) is a DaValue containing a Short flag code indicating the desired operation: 0 => deactivate, 1 => reactivate.
Methods	Name*	Returns
setDaValue(q, p)	Returns a DaValue containing a matrix having one value of type short: the status code for the specified trigger device on a beam code: 0 => deactivated, 1 => activated.
Parameters	Name	
Req/
Opt

Syntax	Semantics
BEAM	req	Integer	Beam code number
* See DaObject and DaReference (and DaReference's parent _DaReference) in aida.lib.da for full API and method signatures.



Users Guide
This section describes what an AIDA user should know about using the SLC Multiknob data provider.For general information on using see Basic Users Guide to Aida, and the Aida javadoc, in particular the classes DaObject and DaReference (and DaReference's parent _DaReference) in aida.lib.da which form Aida's programming interface. One can also use Matlab.

Table 1: Summary of AIDA Data Provider for SLC Multiknob

SUMMARY
Supports a set operation to set the devices referenced in a specified multiknob file by knob rotation using a specified delta value. Only a relative (not absolute) mulitknob file may be specified. The set operation returns the name of each device in the specified multiknob file and the device value after the operation.
Status and limitations	The data provider is complete and there are no known limitations.
Plan	No active plans for extensions.
EXAMPLES
Schematic	Java example. See DpSlcUtilTests.java for more multiknob examples.
String query  = "MKB:VAL";
da.setParam("MKB", "mkb:li02b_xb.mkb");     /* Required MKB parameter specifying a multiknob file. */
Float relativeDelta = new Float(1.0f);      /* Specified relative delta knob rotation. */
DaValue inData = new DaValue(relativeDelta); /* Creation of DaValue to hold relative delta value. */
DaValue outData = da.setDaValue(query, inData); /* Perform set operation for multiknob file knob rotation. */
     
Java	$CD_SOFT/ref/package/aida/test/java/DpSlcUtilTests.java
Matlab	$CD_SOFT/ref/package/aida/test/matlab/multiknobDemo.m
INSTANCES
Types	Description
Multiknob Request	Syntax	MKB:VAL  
Example	MKB:VAL
Instance Attributes
Attribute	Description
VAL	Sets devices referenced in a specified multiknob file, which is a required parameter. The parameter data argument (p) is a DaValue containing a Float value specifying the relative delta knob rotation value.
Methods	Name*	Returns
setDaValue(q, p)	Returns a DaValue containing a heterogenous vector of 2 homogenous congruent vectors:
name of each device referenced in the specified multiknob file
value of each device after the knob rotation set operation
Parameters	Name	
Req/
Opt

Syntax	Semantics
MKB	
req

multiknob filename
The filename path specification (including the prefix "mkb:", if file is in the standard multiknob file directory) of a multiknob file.
* See DaObject and DaReference (and DaReference's parent _DaReference) in aida.lib.da for full API and method signatures.

Users Guide
This section describes what an AIDA user should know about using the SLC BGRP Set Variable data provider.For general information on using see Basic Users Guide to Aida, and the Aida javadoc, in particular the classes DaObject and DaReference (and DaReference's parent _DaReference) in aida.lib.da which form Aida's programming interface. One can also use Matlab.

Table 1: Summary of AIDA Data Provider for SLC Set Variable

SUMMARY
Supports a set operation to set the value of a specified variable for a specified BGRP.
Status and limitations	The data provider is complete and there are no known limitations.
Plan	No active plans for extensions.
EXAMPLES
Schematic	Java example. See DpSlcUtilTests.java for two BGRP set variable examples.
String query  = "BGRP:VAL";
da.setParam("BGRP", "LCLS");            /* Required BGRP parameter specifying a BGRP name. */
da.setParam("VARNAME", "T_CAV");        /* Required VARNAME parameter specifying a variable name for the BGRP. */
String setValue = "Y";                  /* New BGRP variable value ("N" or "Y"). */
DaValue inData = new DaValue(setValue); /* Creation of DaValue to hold new BGRP variable set value. */
da.setDaValue(query, inData);           /* Perform BGRP varaible set value operation. */
     
Java	$CD_SOFT/ref/package/aida/test/java/DpSlcUtilTests.java
Matlab	$CD_SOFT/ref/package/aida/test/matlab/bgrpDemo.m
INSTANCES
Types	Description
BGRP Set Variable Request	Syntax	BGRP:VAL  
Example	BGRP:VAL
Instance Attributes
Attribute	Description
VAL	Sets a BGRP variable to a new value. The parameter data argument (p) is a DaValue containing a String specifying the new variable value ("N" or "Y").
Methods	Name*	Returns
setDaValue(q, p)	Does not return a value.
Parameters	Name	
Req/
Opt

Syntax	Semantics
BGRP	
req

String
The BGRP name.
VARNAME	
req

String
A BGRP variable name for the specified BGRP.
* See DaObject and DaReference (and DaReference's parent _DaReference) in aida.lib.da for full API and method signatures.
