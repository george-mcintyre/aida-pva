# SLC Database Users Guide

This section describes what an AIDA-PVA user should know about accessing the SLC Database data through AIDA. For general
information on using AIDA-PVA see [Basic Users Guide to Aida](UserGuide.md), and the EPICS javadoc, in particular the
classes ??? and ??? in the epics libraries which are now Aida's programming interface. One can also use Matlab.

# AIDA-PVA Data Provider for SLC Database Data

## Summary

|      |              |
| ----------- | ----------- |
| SUMMARY      | Acquires data for the named SLC device from the SLC database. Also allows setting of float scalar database values       |
| Status and limitations   | Simple database retrieval of values is complete (retrieval using primary, micro, unit, and secondary names). Retrieval for one special case has been implemented: the retrieval of values for analog devices specified by micro, component, and attribute name. Retrieval of values for digital devices specified by primary, micro, unit, and component names has not yet been implemented. Storage of values has only been implemented for scalar float secondaries.        |
| Plan |Implement the digital devices special case described above. Also allow the setting of other data types in addition to float secondary database values. |

# EXAMPLES 

| | | 
| ----------- | ----------- |
| Schematic | Java examples, see AidaPvaSlcTests.java for more examples: 
String query = "XCOR:LI03:120:LEFF"; DaReference r = new DaReference(query, da); r.setType(DaValue.Type.FLOAT);  r.compile(da); DaValue v = da.getDaValue(query);  String query = "XCOR:LI31:41:BCON"; /* Indication of set value operation. */ Float floatValue = new Float(5.0f); /* Specified desired value. */ DaValue inData = new DaValue(floatValue); /* Creation of DaValue to hold floatValue. */ da.setDaValue(query, inData); /* Perform set BCON value operation (no value is returned). */ | 
Java
$CD_SOFT/ref/package/aida/test/java/DpSlcTests.java Matlab $CD_SOFT/ ref/package/aida/test/matlab/slcDbSetDemo.m

INSTANCES Types Description Get Syntax    <prim>:<micr>:<unit>:<secn>
ASTS:<micr>:<channel>:<pseudo-secn>
Examples XCOR:LI03:120:LEFF ASTS:PR02:VP3012:DATA Set Syntax    <prim>:<micr>:<unit>:<secn>
Examples XCOR:LI31:41:BCON

Instance Attributes Attribute Description
<secn>    Gets SLC db device data for a named device (d). Methods Name*    Returns getDaValue(d)    DaValue contains a
matrix having one or more values. At present, the type of the value must be explicitly set before retrieving data with
DaValue (see the Java example above). Extract the array with methods of java.util.Vector such as get() (which DaValue
extends). get(d,DaValue.Types.FLOAT)    a float value get(d,DaValue.Types.DOUBLE)    a double value get(
d,DaValue.Types.BYTE)    a byte value get(d,DaValue.Types.SHORT)    a short value get(d,DaValue.Types.INT)    an integer
value get(d,DaValue.Types.LONG)    a long value get(d,DaValue.Types.BOOLEAN)    a boolean value get(
d,DaValue.Types.CHAR)    a character value get(d,DaValue.Types.STRING)    a string value geta(d,DaValue.Types.FLOATA)
an array of float values geta(d,DaValue.Types.DOUBLEA)    an array of double values geta(d,DaValue.Types.BYTEA)    an
array of byte values geta(d,DaValue.Types.SHORTA)    an array of short values geta(d,DaValue.Types.INTA)    an array of
integer values geta(d,DaValue.Types.LONGA)    an array of long values geta(d,DaValue.Types.BOOLEANA)    an array of
boolean values geta(d,DaValue.Types.CHARA)    an array of character values Parameters None
<secn>    Sets the value of a float scalar secondary in the SLC database. The parameter data argument (p) is a DaValue
containing a Float with the desired value. Methods Name*    Returns setDaValue(q, p)    No data is returned. Parameters
None

* See DaObject and DaReference (and DaReference's parent _DaReference) in aida.lib.da for full API and method
  signatures.

