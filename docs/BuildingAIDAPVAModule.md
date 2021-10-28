@tableofcontents
# Building AIDA-PVA Module and adding to STANDALONELIB
 
- [ ] Make sure all include files have correct CMS header
```c
  /*   **CMS**=C_INC   */
```
- [ ] Check out original code from github (first time only)
```shell
cd dev
git clone  git@github.com:slaclab/aida-pva.git
cd aida-pva/src/cpp/include/
```
- [ ] copy contents of this directory to VMS
```shell
> sftp mccdev
Connecting to mccdev...
 
sftp> cd /data_disk_slc/slc/george
sftp> mkdir include
sftp> cd include
sftp> mput *.h 
```
- [ ] On VMS, add all the AIDA-PVA API headers to CMS
```shell
 MCCDEV> CMP AIDA_*.h
  Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.INCLUDE]AIDA_PVA_API.H;11
 
 SLY, Why are You doing this?
 Data:	AIDA-PVA Deployment
  Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.INCLUDE]AIDA_PVA.H;11
 %CMS-I-LIBIS, library is CMS_:[C_INC]
 %CMS-S-LIBSET, library set
 -CMS-I-SUPERSEDE, library list superseded
%CMS-E-NOTFOUND, Element AIDA_PVA.H not found
 Please enter CMS CREATE ELEMENT remark. [AIDA-PVA Deployment]:
%CMS-S-CREATED, element CMS_:[C_INC]AIDA_PVA.H created
 %STRIP_C_COMMENTS-I, producing file REF_C_INC:AIDA_PVA.NO_COMMENTS_H
  Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.INCLUDE]AIDA_PVA_CONVERT.H;11
 %CMS-I-LIBIS, library is CMS_:[C_INC]
 %CMS-S-LIBSET, library set
 -CMS-I-SUPERSEDE, library list superseded
 %CMS-E-NOTFOUND, Element AIDA_PVA_API.H not found
 Please enter CMS CREATE ELEMENT remark. [AIDA-PVA Deployment]:
 %CMS-S-CREATED, element CMS_:[C_INC]AIDA_PVA_API.H created
 %STRIP_C_COMMENTS-I, producing file REF_C_INC:AIDA_PVA_API.NO_COMMENTS_H
  Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.INCLUDE]AIDA_PVA_CONVERT.H;11
 %CMS-I-LIBIS, library is CMS_:[C_INC]
 %CMS-S-LIBSET, library set
 -CMS-I-SUPERSEDE, library list superseded
 %CMS-E-NOTFOUND, Element AIDA_PVA_CONVERT.H not found
 Please enter CMS CREATE ELEMENT remark. [AIDA-PVA Deployment]:
 %CMS-S-CREATED, element CMS_:[C_INC]AIDA_PVA_CONVERT.H created
 %STRIP_C_COMMENTS-I, producing file REF_C_INC:AIDA_PVA_CONVERT.NO_COMMENTS_H
  Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.INCLUDE]AIDA_PVA_EXCEPTIONS.H;11
 %CMS-I-LIBIS, library is CMS_:[C_INC]
 %CMS-S-LIBSET, library set
 -CMS-I-SUPERSEDE, library list superseded
 %CMS-E-NOTFOUND, Element AIDA_PVA_EXCEPTIONS.H not found
 Please enter CMS CREATE ELEMENT remark. [AIDA-PVA Deployment]:
 %CMS-S-CREATED, element CMS_:[C_INC]AIDA_PVA_EXCEPTIONS.H created
 %STRIP_C_COMMENTS-I, producing file REF_C_INC:AIDA_PVA_EXCEPTIONS.NO_COMMENTS_H
  Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.INCLUDE]AIDA_PVA_JSON.H;11
 %CMS-I-LIBIS, library is CMS_:[C_INC]
 %CMS-S-LIBSET, library set
 -CMS-I-SUPERSEDE, library list superseded
 %CMS-E-NOTFOUND, Element AIDA_PVA_JSON.H not found
 Please enter CMS CREATE ELEMENT remark. [AIDA-PVA Deployment]:
 %CMS-S-CREATED, element CMS_:[C_INC]AIDA_PVA_JSON.H created
 %STRIP_C_COMMENTS-I, producing file REF_C_INC:AIDA_PVA_JSON.NO_COMMENTS_H
  Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.INCLUDE]AIDA_PVA_MEMORY.H;11
 %CMS-I-LIBIS, library is CMS_:[C_INC]
 %CMS-S-LIBSET, library set
 -CMS-I-SUPERSEDE, library list superseded
 %CMS-E-NOTFOUND, Element AIDA_PVA_MEMORY.H not found
 Please enter CMS CREATE ELEMENT remark. [AIDA-PVA Deployment]:
 %CMS-S-CREATED, element CMS_:[C_INC]AIDA_PVA_MEMORY.H created
 %STRIP_C_COMMENTS-I, producing file REF_C_INC:AIDA_PVA_MEMORY.NO_COMMENTS_H
  Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.INCLUDE]AIDA_PVA_TYPES.H;11
 %CMS-I-LIBIS, library is CMS_:[C_INC]
 %CMS-S-LIBSET, library set
 -CMS-I-SUPERSEDE, library list superseded
 %CMS-E-NOTFOUND, Element AIDA_PVA_TYPES.H not found
 Please enter CMS CREATE ELEMENT remark. [AIDA-PVA Deployment]:
 %CMS-S-CREATED, element CMS_:[C_INC]AIDA_PVA_TYPES.H created
 %STRIP_C_COMMENTS-I, producing file REF_C_INC:AIDA_PVA_TYPES.NO_COMMENTS_H
  Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.INCLUDE]AIDA_PVA_URI.H;11
 %CMS-I-LIBIS, library is CMS_:[C_INC]
 %CMS-S-LIBSET, library set
 -CMS-I-SUPERSEDE, library list superseded
 %CMS-E-NOTFOUND, Element AIDA_PVA_URI.H not found
 Please enter CMS CREATE ELEMENT remark. [AIDA-PVA Deployment]:
 %CMS-S-CREATED, element CMS_:[C_INC]AIDA_PVA_URI.H created
 %STRIP_C_COMMENTS-I, producing file REF_C_INC:AIDA_PVA_URI.NO_COMMENTS_H
 MCCDEV>
```
- [ ] Back on Unix, go to AIDA-PVA source directory
```shell
cd ../aida-pva
```
- [ ] copy source code in this directory to VMS
```shell
> sftp mccdev
Connecting to mccdev...
 
sftp> cd /data_disk_slc/slc/george
sftp> mkdir aida-pva
sftp> cd aida-pva
sftp> mput *.c 
sftp> mput *.h 
```
- [ ] On VMS, Go into development directory for AIDA-PVA module that you've created in the last step
```shell
MCCDEV> set def [-.AIDA-PVA] 
```
- [ ] Compile this source code into STANDALONE_DEVLIB.OLB
```shell
MCCDEV> LIBRARY /CREATE STANDALONE_DEVLIB.OLB
MCCDEV> CINC *.C
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]AIDA_PVA_JNI_HELPER.C;4
**MEMBER**=SLCLIBS:STANDALONELIB overridden to STANDALONE_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_JNI_HELPER replaced in DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]STANDALONE_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]AIDA_PVA_JSON.C;4
**MEMBER**=SLCLIBS:STANDALONELIB overridden to STANDALONE_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_JSON replaced in DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]STANDALONE_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]AIDA_PVA_SERVER_HELPER.C;4
**MEMBER**=SLCLIBS:STANDALONELIB overridden to STANDALONE_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_SERVER_HELPER replaced in DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]STANDALONE_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]AIDA_PVA_TYPES_HELPER.C;4
**MEMBER**=SLCLIBS:STANDALONELIB overridden to STANDALONE_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_TYPES_HELPER replaced in DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]STANDALONE_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]NATIVECHANNELPROVIDERJNI.C;4
**MEMBER**=SLCLIBS:STANDALONELIB overridden to STANDALONE_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module NATIVECHANNELPROVIDERJNI replaced in DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]STANDALONE_DEVLIB.OLB;1
MCCDEV>
```
- [ ] Create a STANDALONELIB_XFR_ALPHA.OPT
```shell
MCCDEV> LIBRARY/EXTRACT=NATIVECHANNELPROVIDERJNI/output=NATIVECHANNELPROVIDERJNI STANDALONE_DEVLIB
MCCDEV> @java$jni_example:scan_globals_for_option *.OBJ STANDALONELIB_XFR_ALPHA.OPT
DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]NATIVECHANNELPROVIDERJNI.OBJ;1
```
