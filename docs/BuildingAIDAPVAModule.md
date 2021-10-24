# Building AIDA-PVA Module and adding to AIDASHRLIB
 
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
- [ ] On VMS, Go into development directory for AIDASHR module that you've created in the last step
```shell
MCCDEV> set def [-.AIDA-PVA] 
```
- [ ] Compile this source code into DEVLIB.OLB - don't worry about the warnings!
```shell
MCCDEV> LIBRARY /CREATE AIDASHR_DEVLIB.OLB
MCCDEV> CINC *.C
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDA_PVA_JNI_HELPER.C;11
**MEMBER**=SLCLIBS:AIDASHRLIB overridden to AIDASHR_DEVLIB
Note:  using NOOPT by default for DECC

	bool isFloat;                    ///< Determines whether the value is a single or double precision floating point value
........^
%CC-I-BOOLEXT, The _Bool data type is a new feature in the C99 standard.  Other C compilers may not support this feature.
at line number 82 in file REF_:[C_INC]AIDA_PVA_TYPES.H;1

	jshort jShortArray[array.count];
...............^
%CC-I-VLAEXTENSION, In the declaration of "jShortArray", variable length arrays are a new feature in the C99 standard.  Other C compilers may not support this extensi
on.
at line number 533 in file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDA_PVA_JNI_HELPER.C;11
%LIBRAR-S-INSERTED, module AIDA_PVA_JNI_HELPER inserted in DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDA_PVA_JSON.C;11
**MEMBER**=SLCLIBS:AIDASHRLIB overridden to AIDASHR_DEVLIB
Note:  using NOOPT by default for DECC

const struct _json_value json_value_none;
^
%CC-I-CONSTNOINIT, In this declaration, the const object "json_value_none" is uninitialized.
at line number 43 in file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDA_PVA_JSON.C;11
%LIBRAR-S-INSERTED, module AIDA_PVA_JSON inserted in DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDA_PVA_SERVER_HELPER.C;11
**MEMBER**=SLCLIBS:AIDASHRLIB overridden to AIDASHR_DEVLIB
Note:  using NOOPT by default for DECC

	bool isFloat;                    ///< Determines whether the value is a single or double precision floating point value
........^
%CC-I-BOOLEXT, The _Bool data type is a new feature in the C99 standard.  Other C compilers may not support this feature.
at line number 82 in file REF_:[C_INC]AIDA_PVA_TYPES.H;1

		char arrayValueToParse[strlen(valueToParse + 30)];
.....................^
%CC-I-VLAEXTENSION, In the declaration of "arrayValueToParse", variable length arrays are a new feature in the C99 standard.  Other C compilers may not support this e
xtension.
at line number 186 in file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDA_PVA_SERVER_HELPER.C;11
%LIBRAR-S-INSERTED, module AIDA_PVA_SERVER_HELPER inserted in DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDA_PVA_TYPES_HELPER.C;11
**MEMBER**=SLCLIBS:AIDASHRLIB overridden to AIDASHR_DEVLIB
Note:  using NOOPT by default for DECC

	bool isFloat;                    ///< Determines whether the value is a single or double precision floating point value
........^
%CC-I-BOOLEXT, The _Bool data type is a new feature in the C99 standard.  Other C compilers may not support this feature.
at line number 82 in file REF_:[C_INC]AIDA_PVA_TYPES.H;1

	char _formatString[strlen(formatString) + 1];
.............^
%CC-I-VLAEXTENSION, In the declaration of "_formatString", variable length arrays are a new feature in the C99 standard.  Other C compilers may not support this exten
sion.
at line number 266 in file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDA_PVA_TYPES_HELPER.C;11
%LIBRAR-S-INSERTED, module AIDA_PVA_TYPES_HELPER inserted in DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]NATIVECHANNELPROVIDERJNI.C;44
**MEMBER**=SLCLIBS:AIDASHRLIB overridden to AIDASHR_DEVLIB
Note:  using NOOPT by default for DECC

	bool isFloat;                    ///< Determines whether the value is a single or double precision floating point value
........^
%CC-I-BOOLEXT, The _Bool data type is a new feature in the C99 standard.  Other C compilers may not support this feature.
at line number 82 in file REF_:[C_INC]AIDA_PVA_TYPES.H;1
%LIBRAR-S-INSERTED, module NATIVECHANNELPROVIDERJNI inserted in DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
MCCDEV>
```
