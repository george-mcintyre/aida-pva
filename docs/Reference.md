@tableofcontents
# Technical Reference

Guied to the implementation of `AIDA-PVA` in Stanford National Laboratory.

## AIDA-PVA

## AIDA-PVA Module

## AIDA-PVA Native Channel Provider

Native Providers are coded to implement the following AIDA-PVA API:

## AIDA Module

`AIDA Modules` are legacy modules that were created for `AIDA`. They are used in `AIDA-PVA` as the low level modules
that access the SLAC Network devices. They are implemented as libraries in `AIDASHR` and are linked with
the `AIDA-PVA Native Channel Provider`.

General Documentation for these `AIDA` Modules can be found [here](https://www.slac.stanford.edu/grp/cd/soft/aida/)

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
