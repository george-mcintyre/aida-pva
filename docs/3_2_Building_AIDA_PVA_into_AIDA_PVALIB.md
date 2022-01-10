# 3.2 - Building AIDA-PVA into AIDA_PVALIB
## First Time
This step only needs to be done once to do the initial compilation and test and push code to CMS. 

### Verify Code has correct CARDS and transfer to VMS
1. Make sure all include files have correct CMS header
```c
  /*   **CMS**=C_INC   */
```
2. Check out original code from github (first time only)
```shell
cd dev
git clone  git@github.com:slaclab/aida-pva.git
cd aida-pva/src/cpp/include/
```
3. copy contents of this directory to VMS
```shell
> sftp mccdev
Connecting to mccdev...
 
sftp> mkdir include
sftp> cd include
sftp> mput *.h 
```
4. go to AIDA-PVA source directory
```shell
cd ../aida-pva
```
5. copy source code in this directory to VMS
```shell
> sftp mccdev
Connecting to mccdev...
 
sftp> mkdir aida-pva
sftp> cd aida-pva
sftp> mput *.c 
sftp> mput *.h 
```

### Compile and test the code after pushing all header files to CMS
1. On VMS, add all the AIDA-PVA API headers to Production CMS (CMP)
```shell
 MCCDEV> SET DEF [.INCLUDE]
 MCCDEV> CMP AIDA_*.h
  Processing file DATA_DISK_SLC:[.INCLUDE]AIDA_PVA_API.H;11
 
 SLY, Why are You doing this?
 Data:	AIDA-PVA Deployment
  Processing file DATA_DISK_SLC:[.INCLUDE]AIDA_PVA.H;11
 %CMS-I-LIBIS, library is CMS_:[C_INC]
 %CMS-S-LIBSET, library set
 -CMS-I-SUPERSEDE, library list superseded
%CMS-E-NOTFOUND, Element AIDA_PVA.H not found
 Please enter CMS CREATE ELEMENT remark. [AIDA-PVA Deployment]:
%CMS-S-CREATED, element CMS_:[C_INC]AIDA_PVA.H created
 %STRIP_C_COMMENTS-I, producing file REF_C_INC:AIDA_PVA.NO_COMMENTS_H
  Processing file DATA_DISK_SLC:[.INCLUDE]AIDA_PVA_CONVERT.H;11
 %CMS-I-LIBIS, library is CMS_:[C_INC]
 %CMS-S-LIBSET, library set
 -CMS-I-SUPERSEDE, library list superseded
 %CMS-E-NOTFOUND, Element AIDA_PVA_API.H not found
 Please enter CMS CREATE ELEMENT remark. [AIDA-PVA Deployment]:
 %CMS-S-CREATED, element CMS_:[C_INC]AIDA_PVA_API.H created
 %STRIP_C_COMMENTS-I, producing file REF_C_INC:AIDA_PVA_API.NO_COMMENTS_H
  Processing file DATA_DISK_SLC:[.INCLUDE]AIDA_PVA_CONVERT.H;11
 %CMS-I-LIBIS, library is CMS_:[C_INC]
 %CMS-S-LIBSET, library set
 -CMS-I-SUPERSEDE, library list superseded
 %CMS-E-NOTFOUND, Element AIDA_PVA_CONVERT.H not found
 Please enter CMS CREATE ELEMENT remark. [AIDA-PVA Deployment]:
 %CMS-S-CREATED, element CMS_:[C_INC]AIDA_PVA_CONVERT.H created
 %STRIP_C_COMMENTS-I, producing file REF_C_INC:AIDA_PVA_CONVERT.NO_COMMENTS_H
  Processing file DATA_DISK_SLC:[.INCLUDE]AIDA_PVA_EXCEPTIONS.H;11
 %CMS-I-LIBIS, library is CMS_:[C_INC]
 %CMS-S-LIBSET, library set
 -CMS-I-SUPERSEDE, library list superseded
 %CMS-E-NOTFOUND, Element AIDA_PVA_EXCEPTIONS.H not found
 Please enter CMS CREATE ELEMENT remark. [AIDA-PVA Deployment]:
 %CMS-S-CREATED, element CMS_:[C_INC]AIDA_PVA_EXCEPTIONS.H created
 %STRIP_C_COMMENTS-I, producing file REF_C_INC:AIDA_PVA_EXCEPTIONS.NO_COMMENTS_H
  Processing file DATA_DISK_SLC:[.INCLUDE]AIDA_PVA_JSON.H;11
 %CMS-I-LIBIS, library is CMS_:[C_INC]
 %CMS-S-LIBSET, library set
 -CMS-I-SUPERSEDE, library list superseded
 %CMS-E-NOTFOUND, Element AIDA_PVA_JSON.H not found
 Please enter CMS CREATE ELEMENT remark. [AIDA-PVA Deployment]:
 %CMS-S-CREATED, element CMS_:[C_INC]AIDA_PVA_JSON.H created
 %STRIP_C_COMMENTS-I, producing file REF_C_INC:AIDA_PVA_JSON.NO_COMMENTS_H
  Processing file DATA_DISK_SLC:[.INCLUDE]AIDA_PVA_MEMORY.H;11
 %CMS-I-LIBIS, library is CMS_:[C_INC]
 %CMS-S-LIBSET, library set
 -CMS-I-SUPERSEDE, library list superseded
 %CMS-E-NOTFOUND, Element AIDA_PVA_MEMORY.H not found
 Please enter CMS CREATE ELEMENT remark. [AIDA-PVA Deployment]:
 %CMS-S-CREATED, element CMS_:[C_INC]AIDA_PVA_MEMORY.H created
 %STRIP_C_COMMENTS-I, producing file REF_C_INC:AIDA_PVA_MEMORY.NO_COMMENTS_H
  Processing file DATA_DISK_SLC:[.INCLUDE]AIDA_PVA_TYPES.H;11
 %CMS-I-LIBIS, library is CMS_:[C_INC]
 %CMS-S-LIBSET, library set
 -CMS-I-SUPERSEDE, library list superseded
 %CMS-E-NOTFOUND, Element AIDA_PVA_TYPES.H not found
 Please enter CMS CREATE ELEMENT remark. [AIDA-PVA Deployment]:
 %CMS-S-CREATED, element CMS_:[C_INC]AIDA_PVA_TYPES.H created
 %STRIP_C_COMMENTS-I, producing file REF_C_INC:AIDA_PVA_TYPES.NO_COMMENTS_H
  Processing file DATA_DISK_SLC:[.INCLUDE]AIDA_PVA_URI.H;11
 %CMS-I-LIBIS, library is CMS_:[C_INC]
 %CMS-S-LIBSET, library set
 -CMS-I-SUPERSEDE, library list superseded
 %CMS-E-NOTFOUND, Element AIDA_PVA_URI.H not found
 Please enter CMS CREATE ELEMENT remark. [AIDA-PVA Deployment]:
 %CMS-S-CREATED, element CMS_:[C_INC]AIDA_PVA_URI.H created
 %STRIP_C_COMMENTS-I, producing file REF_C_INC:AIDA_PVA_URI.NO_COMMENTS_H
 MCCDEV>
```
2. Go into development directory for AIDA-PVA module that you've created in the last step
```shell
MCCDEV> SET DEF [-.AIDA-PVA] 
```
3. Compile this source code into AIDA_PVA_DEVLIB.OLB
```shell
MCCDEV> LIBRARY /CREATE AIDA_PVA_DEVLIB.OLB
MCCDEV> CINC *.C
 Processing file DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_JNI_HELPER.C;4
**MEMBER**=SLCLIBS:AIDA_PVALIB overridden to AIDA_PVA_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_JNI_HELPER replaced in DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_JSON.C;4
**MEMBER**=SLCLIBS:AIDA_PVALIB overridden to AIDA_PVA_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_JSON replaced in DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_SERVER_HELPER.C;4
**MEMBER**=SLCLIBS:AIDA_PVALIB overridden to AIDA_PVA_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_SERVER_HELPER replaced in DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_TYPES_HELPER.C;4
**MEMBER**=SLCLIBS:AIDA_PVALIB overridden to AIDA_PVA_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_TYPES_HELPER replaced in DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[.AIDA-PVA]NATIVECHANNELPROVIDERJNI.C;4
**MEMBER**=SLCLIBS:AIDA_PVALIB overridden to AIDA_PVA_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module NATIVECHANNELPROVIDERJNI replaced in DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_DEVLIB.OLB;1
MCCDEV>
```
4. Create a AIDA_PVALIB_XFR_ALPHA.OPT
This file will be used when [Building a Channel Provider](3_3_Building_AIDA_PVA_Channel_Provider.md) so keep it safe! 
```shell
MCCDEV> LIBRARY/EXTRACT=NATIVECHANNELPROVIDERJNI/output=NATIVECHANNELPROVIDERJNI AIDA_PVA_DEVLIB
MCCDEV> @java$jni_example:scan_globals_for_option *.OBJ AIDA_PVALIB_XFR_ALPHA.OPT
DATA_DISK_SLC:[.AIDA-PVA]NATIVECHANNELPROVIDERJNI.OBJ;1
```
Then edit this file to add the correct CMS member card at the top:
```text
!==============================================================================
!       **CMS**=SLCSHR_CONTROL
```
## Making Changes to AIDA-PVA Module
To make changes you need to check out the source from CMS. Make changes and then check them back in again.
1. Check out the code from CMS
```shell
MCCDEV> SLCCMS
SlcCMS> SET LIBR CMS_AIDA_PVA
%CMS-I-LIBIS, library is CMS_:[AIDA_PVA]
%CMS-S-LIBSET, library set
-CMS-I-SUPERSEDE, library list superseded
SlcCMS> RESERVE AIDA_PVA_JNI_HELPER.C,AIDA_PVA_SERVER_HELPER.C, AIDA_PVA_JSON.C, AIDA_PVA_TYPES_HELPER.C,NATIVECHANNELPROVIDERJNI.C
_Remark: Deployment of AIDA-PVA Module
%CMS-I-RESERVED, generation 1 of element CMS_:[AIDA_PVA]AIDA_PVA_JNI_HELPER.C reserved
%CMS-I-RESERVED, generation 1 of element CMS_:[AIDA_PVA]AIDA_PVA_JSON.C reserved
%CMS-I-RESERVED, generation 1 of element CMS_:[AIDA_PVA]AIDA_PVA_SERVER_HELPER.C reserved
%CMS-I-RESERVED, generation 1 of element CMS_:[AIDA_PVA]AIDA_PVA_TYPES_HELPER.C reserved
%CMS-I-RESERVED, generation 1 of element CMS_:[AIDA_PVA]NATIVECHANNELPROVIDERJNI.C reserved
%CMS-I-RESERVATIONS, 5 elements reserved
```
2. Make changes
3. Check the code back in
```shell
MCCDEV> LIBRARY /CREATE AIDA_PVA_DEVLIB.OLB
MCCDEV> CINC *.C
 Processing file DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_JNI_HELPER.C;4
**MEMBER**=SLCLIBS:AIDA_PVALIB overridden to AIDA_PVA_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_JNI_HELPER replaced in DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_JSON.C;4
**MEMBER**=SLCLIBS:AIDA_PVALIB overridden to AIDA_PVA_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_JSON replaced in DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_SERVER_HELPER.C;4
**MEMBER**=SLCLIBS:AIDA_PVALIB overridden to AIDA_PVA_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_SERVER_HELPER replaced in DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_TYPES_HELPER.C;4
**MEMBER**=SLCLIBS:AIDA_PVALIB overridden to AIDA_PVA_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_TYPES_HELPER replaced in DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[.AIDA-PVA]NATIVECHANNELPROVIDERJNI.C;4
**MEMBER**=SLCLIBS:AIDA_PVALIB overridden to AIDA_PVA_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module NATIVECHANNELPROVIDERJNI replaced in DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_DEVLIB.OLB;1
MCCDEV>
```
## Move AIDA-PVA Module to Production (CMP)
1. Check out the code
```shell
MCCDEV> SLCCMS

SlcCMS> SET LIBR CMS_AIDA_PVA
%CMS-I-LIBIS, library is CMS_:[AIDA_PVA]
%CMS-S-LIBSET, library set
-CMS-I-SUPERSEDE, library list superseded

SlcCMS> RESERVE AIDA_PVA_JNI_HELPER.C,AIDA_PVA_SERVER_HELPER.C, AIDA_PVA_JSON.C, AIDA_PVA_TYPES_HELPER.C,NATIVECHANNELPROVIDERJNI.C
_Remark: Deployment of AIDA-PVA Module
%CMS-I-RESERVED, generation 1 of element CMS_:[AIDA_PVA]AIDA_PVA_JNI_HELPER.C reserved
%CMS-I-RESERVED, generation 1 of element CMS_:[AIDA_PVA]AIDA_PVA_JSON.C reserved
%CMS-I-RESERVED, generation 1 of element CMS_:[AIDA_PVA]AIDA_PVA_SERVER_HELPER.C reserved
%CMS-I-RESERVED, generation 1 of element CMS_:[AIDA_PVA]AIDA_PVA_TYPES_HELPER.C reserved
%CMS-I-RESERVED, generation 1 of element CMS_:[AIDA_PVA]NATIVECHANNELPROVIDERJNI.C reserved
%CMS-I-RESERVATIONS, 5 elements reserved
SlcCMS> exit

MCCDEV> 
```
2. Once you've tested your AIDA_PVA_DEVLIB.OLB you can Move it to Production (CMP)
```shell
MCCDEV> CMP *.C
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]AIDA_PVA_JNI_HELPER.C;3

SLY, Why are You doing this?
Data:	AIDA-PVA-MODULE Deployment
%CMS-I-LIBIS, library is CMS_:[AIDA_PVA]
%CMS-S-LIBSET, library set
-CMS-I-SUPERSEDE, library list superseded
Please enter CMS REPLACE/IF_CHANGED remark. [AIDA-PVA-MODULE Deployment]:
%CMS-S-GENCREATED, generation 2 of element CMS_:[AIDA_PVA]AIDA_PVA_JNI_HELPER.C created
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_JNI_HELPER replaced in SLC_LIB:[LIBRARIES_ALPHA]AIDA_PVALIB.OLB;1
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]AIDA_PVA_JSON.C;3
%CMS-I-LIBIS, library is CMS_:[AIDA_PVA]
%CMS-S-LIBSET, library set
-CMS-I-SUPERSEDE, library list superseded
Please enter CMS REPLACE/IF_CHANGED remark. [AIDA-PVA-MODULE Deployment]:
%CMS-S-GENCREATED, generation 2 of element CMS_:[AIDA_PVA]AIDA_PVA_JSON.C created
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_JSON replaced in SLC_LIB:[LIBRARIES_ALPHA]AIDA_PVALIB.OLB;1
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]AIDA_PVA_SERVER_HELPER.C;3
%CMS-I-LIBIS, library is CMS_:[AIDA_PVA]
%CMS-S-LIBSET, library set
-CMS-I-SUPERSEDE, library list superseded
Please enter CMS REPLACE/IF_CHANGED remark. [AIDA-PVA-MODULE Deployment]:
%CMS-S-GENCREATED, generation 2 of element CMS_:[AIDA_PVA]AIDA_PVA_SERVER_HELPER.C created
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_SERVER_HELPER replaced in SLC_LIB:[LIBRARIES_ALPHA]AIDA_PVALIB.OLB;1
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]AIDA_PVA_TYPES_HELPER.C;3
%CMS-I-LIBIS, library is CMS_:[AIDA_PVA]
%CMS-S-LIBSET, library set
-CMS-I-SUPERSEDE, library list superseded
Please enter CMS REPLACE/IF_CHANGED remark. [AIDA-PVA-MODULE Deployment]:
%CMS-S-GENCREATED, generation 2 of element CMS_:[AIDA_PVA]AIDA_PVA_TYPES_HELPER.C created
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_TYPES_HELPER replaced in SLC_LIB:[LIBRARIES_ALPHA]AIDA_PVALIB.OLB;1
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]NATIVECHANNELPROVIDERJNI.C;4
%CMS-I-LIBIS, library is CMS_:[AIDA_PVA]
%CMS-S-LIBSET, library set
-CMS-I-SUPERSEDE, library list superseded
Please enter CMS REPLACE/IF_CHANGED remark. [AIDA-PVA-MODULE Deployment]:
%CMS-E-NOREPLACE, error replacing CMS_:[AIDA_PVA]NATIVECHANNELPROVIDERJNI.C
-CMS-E-ELEMULTRES, command applies to multiple reservations of element CMS_:[AIDA_PVA]NATIVECHANNELPROVIDERJNI.C
MCCDEV>
```
