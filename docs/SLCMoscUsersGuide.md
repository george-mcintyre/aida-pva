This page provides documentation for users and programmers of AIDA's SLC Master Oscillator Data Provider. This data provider allows for the retrieval of the Master Oscillator setting in units of absolute frequency. It also allows the setting of the Master Oscillator by specifying the relative value in units of frequency (the default) or energy. This set operation returns the new measurement value of the Master Oscillator in units of absolute frequency.

See Also: Basic Users Guide to Aida, REF_AIDASHR; AIDA SLC Peer Programmers Guide

Users Guide
This section descibes what an AIDA user should know about using the SLC Master Oscillator data provider. For general information on using AIDA see Basic Users Guide to Aida, and the Aida javadoc, in particular the classes DaObject and DaReference, in aida.lib.da which form Aida's programming interface. One can also use Aida through Matlab.

Table 1: Summary of AIDA Data Provider for SLC Master Oscillator

SUMMARY
Supports get and set operations. The get operation obtains the Master Oscillator setting in units of absolute frequency. The set operation sets the Master Oscillator value to a specified setting in units of relative frequency or energy. If the specified units is relative energy, the ring (HER or LER) must also be specified. The set operation returns the new measured Master Oscillator setting value in units of absolute frequency.
Status and limitations	
The data provider is complete and there are no known limitations

Plan	No active plans for extensions.

EXAMPLES
Schematic	Java examples, see DpSlcMoscTests.java for more examples:
String query  = "MASTEROSC//VAL";
DaValue v = da.getDaValue(query);       /* Get measured Master Oscillator setting value. */

String query = "MASTEROSC//VAL";        /* Indication of set operation requested. */ 
Float setValue = new Float(0.328f);     /* Specified relative set value. */
DaValue inData = new DaValue(setValue); /* Creation of DaValue to hold setValue. */
DaValue outData = da.setDaValue(query, inData); /* Perform set operation. */
Java	$CD_SOFT/ref/package/aida/test/java/DpSlcMoscTests.java
Matlab	$CD_SOFT/ref/package/aida/test/matlab/masterOscGetDemo.m obtains the Master Oscillator setting; $CD_SOFT/ref/package/aida/test/matlab/masterOscSetDemo.m sets the Master Oscillator.

INSTANCES and ATTRIBUTES
Instance Type	Description
Get	Syntax	MASTEROSC//VAL
Example	
MASTEROSC//VAL

Set	Syntax	MASTEROSC//VAL
Example	
MASTEROSC//VAL

Attributes
Attribute	Description
VAL	Gets the Master Oscillator setting in units of absolute frequency (KHz).
Methods	Name*	Returns
getDaValue(q)	Returns a DaValue containing a matrix having one value of type double: the measured setting of the Master Oscillator in KHz.
get(d,DaValue.Types.DOUBLE)	a double value, see getDaValue
Parameters	None
VAL	Sets the Master Oscillator value to a specified setting in units of relative frequency or energy. The parameter data argument (p) is a DaValue containing a Float with the value of this relative setting. The default units of the relative setting is frequency (KHz). If the specified units of the relative setting is relative energy, the ring (HER or LER) must also be specified.
Methods	Name*	Returns
setDaValue(q, p)	Returns a DaValue containing a matrix having one value of type double: the measured setting of the Master Oscillator in KHz.
Parameters	Name	
Req/
Opt

Syntax	Semantics
UNITS	opt	FREQUENCY or ENERGY	Indicates whether the specified relative set value is in units of frequency or energy. The default is frequency.
RING	opt	HER or LER	If the specified relative set value is in units of energy, this parameter is required and specifies the ring (HER or LER).
* See DaObject and DaReference (and DaReference's parent _DaReference) in aida.lib.da for full API and method signatures.

