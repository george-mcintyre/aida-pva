# User Guide to AIDA
This is the guide to using the AIDA-PVA system at Stanford National Laboratory.  AIDA is the Accellerator Integrated
Data Access framework developed by SLAC in the early 2000s.  AIDA-PVA is the latest version developed in the early 20s.

If you are already familiar AIDA you can jump to [Differences Between AIDA and AIDA-PVA](UserGuide4AidaUsers.md) without reading 
all of the following sections.  If you are already familiar with EPICS and just want to know how you can use it to 
access AIDA data providers then check out [EPICS and AIDA-PVA](UserGuide4EpicsUsers.md).   
If you're new to both AIDA and EPICS then grab a coffee, and strap in, this may take some time :)

# Overview 
AIDA-PVA is the successor to AIDA a system for providing programatic access to SLC control system devices, data and services.  
Since its introduction in 2002 it has has been used in other laboratories in the US and around the world.  It 
provides a naming convention to address all devices and their attributes, a way of specifying arguments for remote procedure calls 
and obtaining results in any format, including rich formats including metadata.  Finally, it provides security, logging and 
reliability to allow it to be used in a variety of applications from logging, and monitoring, to modeling and control.

AIDA-PVA supercedes AIDA but both AIDA and AIDA-PVA co-exist and share many of the low level AIDA provider layers so that
the results obtained by usine either version remain consistent.  

Legacy AIDA uses CORBA to transport requests to the Data Providers while AIDA-PVA uses EPICS-7's PV-Access mechanism.  
EPICS has become a staple for laboratories around the world and leveraging its features allows scientists
and engineers who are already familiar with programming on EPICS to access AIDA data providers with very little effort. 

[ DIAGRAM ]   - nb. do just the aida-pva part, keep whole thing for differences doc

Programming Language / Command Line tool / 
             
LEGACY CLIENT                CLIENT                   
(AIDA - Client)               EPICS

  CORBA                   PV-ACCESS    --> other EPICS services

AIDA PROVIDER               AIDA PROVIDER
(AIDA - Server)               (AIDA-PVA)
CORBA STUB                   AIDA-PVA.JAR
JNI                         
                            DP-AIDA-PVA-IMPL
DATA-PROVIDER CODE         DATA-PROVIDER CODE

(DaValue)                  (Normative Types and tables - PV Structure)


# Components
As you can see clients using AIDA-PVA will look like any any other EPICS client.  AIDA-PVA data providers are implemented
inside the EPICS framework and so will appear to EPICs clients as just another EPICS service.  
In order to access an AIDA provder you'll give a PV (Process Variable) Name, in EPICS parlance.  The EPICS framework
will find the service that serves requests for that PV and directs services to it.

The AIDA Provider is made up of three parts - there is the AIDA-PVA jar which is a glue that registers the service in the 
EPICS network, the Data provder code that accesses and controls the devices or databases, and the code that links the two together. 


# New naming conventions

In AIDA names consist of DEVICE:ATTRIBUTE where DEVICE can have other sub parts composed typically delimited by colons. 
Though the ATTRIBUTE part is typically a single name in rare cases it also has parts delimited by colons.

in AIDA-PVA names have the same components but are all separated by colons so PRIM:MICR:UNIT//ATTR becomes PRIM:MICR:UNIT:ATTR 
in AIDA-PVA.  In this way AIDA-PVA allows accessing services with the same naming conventions as EPICS.

Even though AIDA-PVA has an updated naming scheme to align with EPICs it is backwards compatible with AIDA and allows legacy style
names.

# Call semantics
## Request
Requests (previously known as get in AIDA) request data from the specified PV.  The requests can take optional arguments which are
simple name,value pairs.  The value can be either a simple string, a scalar value, or some json that allows specifying arrays,
and objects.  The interpretation of these arguments is provider dependent.  Each provider will publish the arguments they 
accept for each set of PVs they support and will describe the names, formats, requiredness, defaults, and acceptable values 
of the parameters they support.

EPICS allows synchronous and Asynchronous call semantics so either can be used with AIDA-PVA.

Exceptions are propagated back to the caller and are logged using the configured logger in the data provider - cmlog.

## Set
AIDA-PVA allows you to set values associated with PVs.  These special requests are known as Setters.  To select 
a Setter request simply add an argument called `VALUE`.  All Setters have a value which is the value that the
PV will be set to.  Other arguments can be specified as described by the provider.

## TYPE
When you need to specify the type of the response you can set the TYPE argument to one of the available types.  
e.g. `TYPE=FLOAT`
  * Scalar Types
    * `BOOLEAN`            to return a boolean
    * `BYTE`               to return a byte
    * `SHORT`              to return a short
    * `INTEGER`            to return an integer
    * `LONG`               to return a long
    * `FLOAT`              to return a float
    * `DOUBLE`             to return a double
    * `STRING`             to return a string
  * Scalar Array Types
    * `BOOLEAN_ARRAY`      to return a boolean array
    * `BYTE_ARRAY`         to return a byte array
    * `SHORT_ARRAY`        to return a short array
    * `INTEGER_ARRAY`      to return an integer array
    * `LONG_ARRAY`         to return a long array
    * `FLOAT_ARRAY`        to return a float array
    * `DOUBLE_ARRAY`       to return a double array
    * `STRING_ARRAY`       to return a string array
  * Tables
    * `TABLE`              to return an NT/TABLE

## TABLE_TYPE
If supported you can specify the type for rows in a table by providing the `TABLE_TYPE` argument.  The 
value can be any Scalar or Scalar Array type. eg. `TABLE_TYPE=FLOAT` for the SLC data provider will select a float for 
single row in the single column table returned from a request.

# Service Address 
In EPICs `//` is used to delimit the optional host or service name from the PV (Process Variable).  e.g. ...

# Using AIDA-PVA
## Important EPICS Environment variables
## From the command line
## From Java
## From Matlab
## From ...


