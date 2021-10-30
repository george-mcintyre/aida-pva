# 3 - Platform Technical Reference

Guide to the implementation of AIDA-PVA in Stanford National Laboratory.

## EPICS Backport 1.5
In order to make AIDA-PVA work on VMS EPICS had to be backported to Java 1.5. This created
the following libraries that were used to build the AIDA-PVA SERVICE Jar and run the EPICS framework on VMS.
- EPICS-PVACCESS-bp15.JAR - Backported version of PVACCESS.JAR.
- EPICS-PVDATA-bp15.JAR - Backported version of PVDATA.JAR.
- EPICS-FORWARDER.JAR - New component of EPICS required for the framework on VMS.

@see [Porting EPICS to Java 1.5 on VMS](5_0_Porting_EPICS_to_Java_1_5_on_VMS.md) for more information

## AIDA-PVA SERVICE
The AIDA-PVA Service is a [shaded jar](https://stackoverflow.com/questions/49810578/what-is-a-shaded-jar-and-what-is-the-difference-similarities-between-uber-jar-a) 
that implements the AIDA-PVA framework.   It is therefore a self-contained executable JAR that when run becomes the Channel Provider Process.   

The AIDA-PVA service uses a plug-and-play architecture for the implementation of the channel provider.  It is configured as follows:
- It checks the value of a property (`java.library.path`) to determine which
path to load the Channel Provider library from.  In our SLAC environment this is always `/SLCLIBS`.
- It checks the value of a property (`aida.pva.lib.name`) or environment variable (`AIDA_PVA_LIB_NAME`) to determine which
Channel Provider library to load.  e.g. `AIDASLCDB` to load `AIDASLCDB.EXE` library.
- It checks the value of a property (`aida.pva.channels.filename`) or environment variable (`AIDA_PVA_CHANNELS_FILENAME`) to determine which
 channel configuration file to load. This can be a full path name.  e.g. `/SLCTXT/AIDASLCDB_CHANNELS.YML`
- It reads configuration from the selected channel configuration file.

This determines which channels the Channel Provider will support, and where it will find implementations for the endpoints required to service the supported channels. 
In this way the same JAR can become any Channel Provider simply by modifying these configuration parameters.

## AIDA-PVA Module

## AIDA-PVA Channel Provider

Native Providers are coded to implement the following AIDA-PVA API:

## AIDA Modules

AIDA Modules are legacy modules that were created for AIDA. They are used in AIDA-PVA as the low level modules that
access the SLAC Network devices. They are implemented as libraries in AIDASHR and are linked with the AIDA-PVA Channel
Provider.

General Documentation for these AIDA Modules can be [found here](https://www.slac.stanford.edu/grp/cd/soft/aida/)

_Specific Documentation_:

* [SLC Control Database Module](https://www.slac.stanford.edu/grp/cd/soft/aida/slcDatabaseDpGuide.html)
    * Java Code
    * Jni Code
    * Implementation Code
* [SLC Bpm Module](https://www.slac.stanford.edu/grp/cd/soft/aida/slcBpmDpGuide.html)
    * Java Code
    * Jni Code
    * [Implementation Code](http://www-mcc.slac.stanford.edu/ref_0/AIDASHR/DPSLCBPM_JNI_HELPER.C)
* [SLC Buffered Data Module](https://www.slac.stanford.edu/grp/cd/soft/aida/slcBuffDpGuide.html)
    * Java Code
    * Jni Code
    * Implementation Code
* [SLC Klystron Module](https://www.slac.stanford.edu/grp/cd/soft/aida/slcKlysDpGuide.html)
    * Java Code
    * [Jni Code](http://www-mcc.slac.stanford.edu/ref_0/AIDASHR/DPSLCKLYS_JNI.C)
    * Implementation Code
* [SLC Magnet Module](https://www.slac.stanford.edu/grp/cd/soft/aida/slcMagnetDpGuide.html)
    * Java Code
    * Jni Code
    * Implementation Code
* [LCLS Model Module](https://www.slac.stanford.edu/grp/cd/soft/aida/lclsModelDpGuide.html)
    * Java Code
    * Jni Code
    * Implementation Code
* [SLC Master Oscillator](https://www.slac.stanford.edu/grp/cd/soft/aida/slcMasterOscDpGuide.html)
    * Java Code
    * Jni Code
    * Implementation Code
* Util Module: [SLC Trigger](https://www.slac.stanford.edu/grp/cd/soft/aida/slcTrigDpGuide.html)
  , [SLC Multiknob](https://www.slac.stanford.edu/grp/cd/soft/aida/slcMultiknobDpGuide.html)
  , [SLC BGRP](https://www.slac.stanford.edu/grp/cd/soft/aida/slcBgrpDpGuide.html)
    * Java Code
    * Jni Code
    * Implementation Code

## AIDA-PVA Tests

### Reference Tests

### SLC Control Database Tests

### SLC Bpm Tests

### SLC Buffered Data Tests

### SLC Klystron Tests
