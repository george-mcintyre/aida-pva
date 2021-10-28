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
- [ ] Create an AIDASHR_XFR_ALPHA.OPT
```shell
 MCCDEV> LIBRARY/EXTRACT=NATIVECHANNELPROVIDERJNI/output=NATIVECHANNELPROVIDERJNI AIDASHR_DEVLIB
 MCCDEV> @java$jni_example:scan_globals_for_option *.OBJ AIDASHR_XFR_ALPHA.OPT
 DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]NATIVECHANNELPROVIDERJNI.OBJ;1
 MCCDEV>
```
- [ ] Link your AIDASHR, using the OPT file just created. (Ed. Please Note!! See email)
```shell
MCCDEV> BUILDTEST AIDASHR /DEFAULT

Processing AIDASHR
 Building Shareable image TST_AIDASHR.EXE
%BUILDTEST-I, using AIDASHR_DEVLIB/INCL=(all-modules)
%LINK-W-NUDFSYMS, 38 undefined symbols:
%LINK-I-UDFSYM, 	aidaRequestBoolean
%LINK-I-UDFSYM, 	aidaRequestBooleanArray
%LINK-I-UDFSYM, 	aidaRequestByte
%LINK-I-UDFSYM, 	aidaRequestByteArray
%LINK-I-UDFSYM, 	aidaRequestDouble
%LINK-I-UDFSYM, 	aidaRequestDoubleArray
%LINK-I-UDFSYM, 	aidaRequestFloat
%LINK-I-UDFSYM, 	aidaRequestFloatArray
%LINK-I-UDFSYM, 	aidaRequestInteger
%LINK-I-UDFSYM, 	aidaRequestIntegerArray
%LINK-I-UDFSYM, 	aidaRequestLong
%LINK-I-UDFSYM, 	aidaRequestLongArray
%LINK-I-UDFSYM, 	aidaRequestShort
%LINK-I-UDFSYM, 	aidaRequestShortArray
%LINK-I-UDFSYM, 	aidaRequestString
%LINK-I-UDFSYM, 	aidaRequestStringArray
%LINK-I-UDFSYM, 	aidaRequestTable
%LINK-I-UDFSYM, 	aidaServiceInit
%LINK-I-UDFSYM, 	aidaSetValue
%LINK-I-UDFSYM, 	aidaSetValueWithResponse
%LINK-I-UDFSYM, 	getValue
%LINK-I-UDFSYM, 	releaseArguments
%LINK-I-UDFSYM, 	releaseArray
%LINK-I-UDFSYM, 	releaseStringArray
%LINK-I-UDFSYM, 	releaseTable
%LINK-I-UDFSYM, 	releaseValue
%LINK-I-UDFSYM, 	toArguments
%LINK-I-UDFSYM, 	toBooleanArray
%LINK-I-UDFSYM, 	toByteArray
%LINK-I-UDFSYM, 	toCString
%LINK-I-UDFSYM, 	toDoubleArray
%LINK-I-UDFSYM, 	toFloatArray
%LINK-I-UDFSYM, 	toIntegerArray
%LINK-I-UDFSYM, 	toJString
%LINK-I-UDFSYM, 	toLongArray
%LINK-I-UDFSYM, 	toShortArray
%LINK-I-UDFSYM, 	toStringArray
%LINK-I-UDFSYM, 	toTable
%LINK-W-USEUNDEF, undefined symbol aidaServiceInit referenced
	in psect $LINK$ offset %X00000020
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol releaseArguments referenced
	in psect $LINK$ offset %X00000050
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol releaseValue referenced
	in psect $LINK$ offset %X00000060
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol toArguments referenced
	in psect $LINK$ offset %X00000070
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol toCString referenced
	in psect $LINK$ offset %X00000090
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol releaseTable referenced
	in psect $LINK$ offset %X000000A0
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol toTable referenced
	in psect $LINK$ offset %X000000B0
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol getValue referenced
	in psect $LINK$ offset %X000000C0
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol aidaSetValueWithResponse referenced
	in psect $LINK$ offset %X000000D0
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol aidaSetValue referenced
	in psect $LINK$ offset %X00000100
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol aidaRequestTable referenced
	in psect $LINK$ offset %X00000130
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol aidaRequestStringArray referenced
	in psect $LINK$ offset %X00000160
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol releaseStringArray referenced
	in psect $LINK$ offset %X00000170
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol toStringArray referenced
	in psect $LINK$ offset %X00000180
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol aidaRequestDoubleArray referenced
	in psect $LINK$ offset %X000001B0
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol releaseArray referenced
	in psect $LINK$ offset %X000001C0
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol toDoubleArray referenced
	in psect $LINK$ offset %X000001D0
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol toFloatArray referenced
	in psect $LINK$ offset %X00000200
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol aidaRequestFloatArray referenced
	in psect $LINK$ offset %X00000210
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol toLongArray referenced
	in psect $LINK$ offset %X00000240
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol aidaRequestLongArray referenced
	in psect $LINK$ offset %X00000250
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol toIntegerArray referenced
	in psect $LINK$ offset %X00000280
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol aidaRequestIntegerArray referenced
	in psect $LINK$ offset %X00000290
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol toShortArray referenced
	in psect $LINK$ offset %X000002C0
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol aidaRequestShortArray referenced
	in psect $LINK$ offset %X000002D0
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol toByteArray referenced
	in psect $LINK$ offset %X00000300
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol aidaRequestByteArray referenced
	in psect $LINK$ offset %X00000310
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol toBooleanArray referenced
	in psect $LINK$ offset %X00000340
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol aidaRequestBooleanArray referenced
	in psect $LINK$ offset %X00000350
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol aidaRequestString referenced
	in psect $LINK$ offset %X00000390
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol toJString referenced
	in psect $LINK$ offset %X000003A0
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol aidaRequestDouble referenced
	in psect $LINK$ offset %X000003D0
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol aidaRequestFloat referenced
	in psect $LINK$ offset %X00000400
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol aidaRequestLong referenced
	in psect $LINK$ offset %X00000430
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol aidaRequestInteger referenced
	in psect $LINK$ offset %X00000460
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol aidaRequestShort referenced
	in psect $LINK$ offset %X00000490
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol aidaRequestByte referenced
	in psect $LINK$ offset %X000004C0
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
%LINK-W-USEUNDEF, undefined symbol aidaRequestBoolean referenced
	in psect $LINK$ offset %X000004F0
	in module NATIVECHANNELPROVIDERJNI file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA.AIDAPVA]AIDASHR_DEVLIB.OLB;1
 
```
