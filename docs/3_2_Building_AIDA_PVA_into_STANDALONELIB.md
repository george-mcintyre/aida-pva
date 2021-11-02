# 3.2 - Building AIDA-PVA into STANDALONELIB
This does not need to be done for each new Channel Provider. It only needs to be done once. But if it has not been done
you will need to do it first.


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

4. On VMS, add all the AIDA-PVA API headers to Production CMS (CMP)

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

5. Back on Unix, go to AIDA-PVA source directory

```shell
cd ../aida-pva
```

6. copy source code in this directory to VMS

```shell
> sftp mccdev
Connecting to mccdev...
 
sftp> mkdir aida-pva
sftp> cd aida-pva
sftp> mput *.c 
sftp> mput *.h 
```

7. On VMS, Go into development directory for AIDA-PVA module that you've created in the last step

```shell
MCCDEV> SET DEF [.AIDA-PVA] 
```

8. Compile this source code into STANDALONE_DEVLIB.OLB

```shell
MCCDEV> LIBRARY /CREATE STANDALONE_DEVLIB.OLB
MCCDEV> CINC *.C
 Processing file DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_JNI_HELPER.C;4
**MEMBER**=SLCLIBS:STANDALONELIB overridden to STANDALONE_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_JNI_HELPER replaced in DATA_DISK_SLC:[.AIDA-PVA]STANDALONE_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_JSON.C;4
**MEMBER**=SLCLIBS:STANDALONELIB overridden to STANDALONE_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_JSON replaced in DATA_DISK_SLC:[.AIDA-PVA]STANDALONE_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_SERVER_HELPER.C;4
**MEMBER**=SLCLIBS:STANDALONELIB overridden to STANDALONE_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_SERVER_HELPER replaced in DATA_DISK_SLC:[.AIDA-PVA]STANDALONE_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[.AIDA-PVA]AIDA_PVA_TYPES_HELPER.C;4
**MEMBER**=SLCLIBS:STANDALONELIB overridden to STANDALONE_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module AIDA_PVA_TYPES_HELPER replaced in DATA_DISK_SLC:[.AIDA-PVA]STANDALONE_DEVLIB.OLB;1
 Processing file DATA_DISK_SLC:[.AIDA-PVA]NATIVECHANNELPROVIDERJNI.C;4
**MEMBER**=SLCLIBS:STANDALONELIB overridden to STANDALONE_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-REPLACED, module NATIVECHANNELPROVIDERJNI replaced in DATA_DISK_SLC:[.AIDA-PVA]STANDALONE_DEVLIB.OLB;1
MCCDEV>
```

9. Create a STANDALONELIB_XFR_ALPHA.OPT

```shell
MCCDEV> LIBRARY/EXTRACT=NATIVECHANNELPROVIDERJNI/output=NATIVECHANNELPROVIDERJNI STANDALONE_DEVLIB
MCCDEV> @java$jni_example:scan_globals_for_option *.OBJ STANDALONELIB_XFR_ALPHA.OPT
DATA_DISK_SLC:[.AIDA-PVA]NATIVECHANNELPROVIDERJNI.OBJ;1
```

10. Once you've tested your STANDALONE_DEVLIB.OLB you can Move it to Production (CMP)

```shell
MCCDEV> SLCCMS
SlcCMS> SET LIBR CMS_STANDALONE
%CMS-I-LIBIS, library is CMS_:[STANDALONE]
%CMS-S-LIBSET, library set
-CMS-I-SUPERSEDE, library list superseded
SlcCMS> RESERVE AIDA_PVA_JNI_HELPER.C,AIDA_PVA_SERVER_HELPER.C, AIDA_PVA_JSON.C, AIDA_PVA_TYPES_HELPER.C,NATIVECHANNELPROVIDERJNI.C
_Remark: Deployment of AIDA-PVA Module
%CMS-I-RESERVED, generation 1 of element CMS_:[STANDALONE]AIDA_PVA_JNI_HELPER.C reserved
%CMS-I-RESERVED, generation 1 of element CMS_:[STANDALONE]AIDA_PVA_JSON.C reserved
%CMS-I-RESERVED, generation 1 of element CMS_:[STANDALONE]AIDA_PVA_SERVER_HELPER.C reserved
%CMS-I-RESERVED, generation 1 of element CMS_:[STANDALONE]AIDA_PVA_TYPES_HELPER.C reserved
%CMS-I-RESERVED, generation 1 of element CMS_:[STANDALONE]NATIVECHANNELPROVIDERJNI.C reserved
%CMS-I-RESERVATIONS, 5 elements reserved
MCCDEV> CMP *.C
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]AIDA_PVA_JNI_HELPER.C;6

SLY, Why are You doing this?
Data:	AIDA-PVA-MODULE Deployment
%CMS-I-LIBIS, library is CMS_:[STANDALONE]
%CMS-S-LIBSET, library set
%CMS-E-NOTFOUND, Element AIDA_PVA_JNI_HELPER.C not found
Please enter CMS CREATE ELEMENT remark. [AIDA-PVA-MODULE Deployment]:
%CMS-S-CREATED, element CMS_:[STANDALONE]AIDA_PVA_JNI_HELPER.C created
Note:  using NOOPT by default for DECC

	bool isFloat;                    ///< Determines whether the value is a single or double precision floating point value
........^
%CC-I-BOOLEXT, The _Bool data type is a new feature in the C99 standard.  Other C compilers may not support this feature.
at line number 82 in file REF_:[C_INC]AIDA_PVA_TYPES.H;1

	jshort jShortArray[array.count];
...............^
%CC-I-VLAEXTENSION, In the declaration of "jShortArray", variable length arrays are a new feature in the C99 standard.  Other C compilers may not suppo
rt this extension.
at line number 532 in file REF_:[STANDALONE]AIDA_PVA_JNI_HELPER.C;1
%LIBRAR-S-INSERTED, module AIDA_PVA_JNI_HELPER inserted in SLC_LIB:[LIBRARIES_ALPHA]STANDALONELIB.OLB;1
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]AIDA_PVA_JSON.C;6
%CMS-I-LIBIS, library is CMS_:[STANDALONE]
%CMS-S-LIBSET, library set
-CMS-I-SUPERSEDE, library list superseded
%CMS-E-NOTFOUND, Element AIDA_PVA_JSON.C not found
Please enter CMS CREATE ELEMENT remark. [AIDA-PVA-MODULE Deployment]:
%CMS-S-CREATED, element CMS_:[STANDALONE]AIDA_PVA_JSON.C created
Note:  using NOOPT by default for DECC

const struct _json_value json_value_none;
^
%CC-I-CONSTNOINIT, In this declaration, the const object "json_value_none" is uninitialized.
at line number 42 in file REF_:[STANDALONE]AIDA_PVA_JSON.C;1
%LIBRAR-S-INSERTED, module AIDA_PVA_JSON inserted in SLC_LIB:[LIBRARIES_ALPHA]STANDALONELIB.OLB;1
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]AIDA_PVA_SERVER_HELPER.C;6
%CMS-I-LIBIS, library is CMS_:[STANDALONE]
%CMS-S-LIBSET, library set
-CMS-I-SUPERSEDE, library list superseded
%CMS-E-NOTFOUND, Element AIDA_PVA_SERVER_HELPER.C not found
Please enter CMS CREATE ELEMENT remark. [AIDA-PVA-MODULE Deployment]:
%CMS-S-CREATED, element CMS_:[STANDALONE]AIDA_PVA_SERVER_HELPER.C created
Note:  using NOOPT by default for DECC

	bool isFloat;                    ///< Determines whether the value is a single or double precision floating point value
........^
%CC-I-BOOLEXT, The _Bool data type is a new feature in the C99 standard.  Other C compilers may not support this feature.
at line number 82 in file REF_:[C_INC]AIDA_PVA_TYPES.H;1

		char arrayValueToParse[strlen(valueToParse) + 30];
.....................^
%CC-I-VLAEXTENSION, In the declaration of "arrayValueToParse", variable length arrays are a new feature in the C99 standard.  Other C compilers may not
 support this extension.
at line number 185 in file REF_:[STANDALONE]AIDA_PVA_SERVER_HELPER.C;1
%LIBRAR-S-INSERTED, module AIDA_PVA_SERVER_HELPER inserted in SLC_LIB:[LIBRARIES_ALPHA]STANDALONELIB.OLB;1
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]AIDA_PVA_TYPES_HELPER.C;6
%CMS-I-LIBIS, library is CMS_:[STANDALONE]
%CMS-S-LIBSET, library set
-CMS-I-SUPERSEDE, library list superseded
%CMS-E-NOTFOUND, Element AIDA_PVA_TYPES_HELPER.C not found
Please enter CMS CREATE ELEMENT remark. [AIDA-PVA-MODULE Deployment]:
%CMS-S-CREATED, element CMS_:[STANDALONE]AIDA_PVA_TYPES_HELPER.C created
Note:  using NOOPT by default for DECC

	bool isFloat;                    ///< Determines whether the value is a single or double precision floating point value
........^
%CC-I-BOOLEXT, The _Bool data type is a new feature in the C99 standard.  Other C compilers may not support this feature.
at line number 82 in file REF_:[C_INC]AIDA_PVA_TYPES.H;1

	char _formatString[strlen(formatString) + 1];
.............^
%CC-I-VLAEXTENSION, In the declaration of "_formatString", variable length arrays are a new feature in the C99 standard.  Other C compilers may not sup
port this extension.
at line number 265 in file REF_:[STANDALONE]AIDA_PVA_TYPES_HELPER.C;1
%LIBRAR-S-INSERTED, module AIDA_PVA_TYPES_HELPER inserted in SLC_LIB:[LIBRARIES_ALPHA]STANDALONELIB.OLB;1
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.AIDA-PVA]NATIVECHANNELPROVIDERJNI.C;6
%CMS-I-LIBIS, library is CMS_:[STANDALONE]
%CMS-S-LIBSET, library set
-CMS-I-SUPERSEDE, library list superseded
%CMS-E-NOTFOUND, Element NATIVECHANNELPROVIDERJNI.C not found
Please enter CMS CREATE ELEMENT remark. [AIDA-PVA-MODULE Deployment]:
%CMS-S-CREATED, element CMS_:[STANDALONE]NATIVECHANNELPROVIDERJNI.C created
%CMP-W-NOPRODREPOS, no production repository CXXREP_ for this source
Note:  using NOOPT by default for DECC

	bool isFloat;                    ///< Determines whether the value is a single or double precision floating point value
........^
%CC-I-BOOLEXT, The _Bool data type is a new feature in the C99 standard.  Other C compilers may not support this feature.
at line number 82 in file REF_:[C_INC]AIDA_PVA_TYPES.H;1
%LIBRAR-S-INSERTED, module NATIVECHANNELPROVIDERJNI inserted in SLC_LIB:[LIBRARIES_ALPHA]STANDALONELIB.OLB;1
```
