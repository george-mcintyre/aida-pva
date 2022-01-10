# 3.3 - Building an AIDA-PVA Channel Provider

## First Time
This step only needs to be done once to do the initial compilation and test and push code to CMS. 
All examples below show AIDASLCDB.

### Deploy your Channel Configuration File
1. Check out original code from github (first time only - afterwards maintain code in CMS)
```shell
cd dev
git clone  git@github.com:slaclab/aida-pva.git
cd aida-pva/src/cpp/providers/SLCDB
```
2. Make sure your Channel Configuration file has the correct CMS header
```c
  #     **CMS**=SLCTXT
```
3. Copy your file to MCCDEV
```shell
> sftp mccdev
Connecting to mccdev...
 
sftp> mput AIDASLCDB_CHANNELS.YML
```
4. Add to CMS in SLCTXT
```shell
MCCDEV> cmp AIDASLCBPM_CHANNELS.YML
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.PROVIDERS.SLCBPM]AIDASLCBPM_CHANNELS.YML;11

SLY, Why are You doing this?
Data:	Adding Channel Configuration Files to CMS
%CMS-I-LIBIS, library is CMS_:[SLCTXT]
%CMS-S-LIBSET, library set
%CMS-E-NOTFOUND, Element AIDASLCBPM_CHANNELS.YML not found
Please enter CMS CREATE ELEMENT remark. [Adding Channel Configuration Files to CMS]:
%CMS-S-CREATED, element CMS_:[SLCTXT]AIDASLCBPM_CHANNELS.YML created
MCCDEV>
```
### Deploy other related files
1. Check out original code from github (first time only - afterwards maintain code in CMS)
```shell
cd dev
git clone  git@github.com:slaclab/aida-pva.git
cd aida-pva/src/cpp/providers
```
2. Make sure STARTAIDASLC.COM and SLCDB/AIDA_SLCDB.SUBMIT files have the correct CMS header
```text
$!       **CMS**=COM_GEN
$!
```
3. Make sure SLCDB/AIDASLCDB_GENERAL.OPT file has the correct CMS header
```text
!==============================================================================
!       **CMS**=SLCSHR_CONTROL
!
! Name: AIDASLCDB_GENERAL.OPT
!
```
4. Copy your files to MCCDEV
```shell
> sftp mccdev
Connecting to mccdev...
 
sftp> mput STARTAIDASLC.COM
sftp> lcd SLCDB
sftp> mput AIDA_SLCDB.SUBMIT
sftp> mput AIDASLCDB_GENERAL.OPT
```
5. Add to CMS
```shell
MCCDEV> slccms
SlcCMS> set libr CMS_COM_GEN
%CMS-I-LIBIS, library is CMS_:[COM.GEN]
%CMS-S-LIBSET, library set
-CMS-I-SUPERSEDE, library list superseded
SlcCMS> create element STARTAIDASLC.COM,AIDA_SLCDB.SUBMIT
_Remark: Creating Submit files for AIDASLCDB
%CMS-S-CREATED, element CMS_:[COM.GEN]STARTAIDASLC.COM created
%CMS-S-CREATED, element CMS_:[COM.GEN]AIDA_SLCDB.SUBMIT created
SlcCMS> exit
```
@note `STARTAIDASLC.COM` needs to be created only once and is reused for all channel providers.

### Verify Code has correct CARDS and transfer to VMS
1. Make sure all C-files have correct CMS header and ATTRIBUTE for your Channel Provider
```c
  /*    **MEMBER**=SLCLIBS:AIDASLCDBLIB
   *    **ATTRIBUTES**=JNI                 */
```
2. Make sure all include files have correct CMS header
```c
  /*   **CMS**=C_INC   */
```
3. Check out original code from github (first time only - afterwards maintain code in CMS)
```shell
cd dev
git clone  git@github.com:slaclab/aida-pva.git
cd aida-pva/src/cpp/providers/SLCDB
```
4. Copy your code (1 C file, and 1 header file usually) to a directory on MCCDEV
```shell
> sftp mccdev
Connecting to mccdev...
 
sftp> mkdir AIDASLCDB
sftp> cd AIDASLCDB
sftp> mput *.c
sftp> mput *.h
```
5. On VMS, Go into development directory
```shell
MCCDEV> set def [.AIDASLCDB] 
```

@subsection cbtd Compiling, Building, Testing, and Deploying
1. Push headers to CMS
```shell
 MCCDEV> CMP *.h
  Processing file DATA_DISK_SLC:[.AIDASLCDB]AIDASLCDB_SERVER.H;11
 
 SLY, Why are You doing this?
 Data:	AIDASLCDB Deployment
  Processing file DATA_DISK_SLC:[.AIDASLCDB]AIDASLCDB_SERVER.H;11
 %CMS-I-LIBIS, library is CMS_:[C_INC]
 %CMS-S-LIBSET, library set
 -CMS-I-SUPERSEDE, library list superseded
%CMS-E-NOTFOUND, Element AIDASLCDB_SERVER.H not found
 Please enter CMS CREATE ELEMENT remark. [AIDASLCDB Deployment]:
%CMS-S-CREATED, element CMS_:[C_INC]AIDASLCDB_SERVER.H created
 %STRIP_C_COMMENTS-I, producing file REF_C_INC:AIDASLCDB_SERVERNO_COMMENTS_H
 MCCDEV>
```
2. Copy AIDA_PVALIB_XFR_ALPHA.OPT to your directory, rename and add to CMS
```shell
MCCDEV> COPY [-.AIDA-PVA]AIDA_PVALIB_XFR_ALPHA.OPT AIDASLCDB_XFR_ALPHA.OPT
MCCDEV>
MCCDEV> cmp AIDASLCDB_XFR_ALPHA.OPT
Processing file DATA_DISK_SLC:[]AIDASLCDB_XFR_ALPHA.OPT;1

SLY, Why are You doing this?
Data:
%CMS-I-LIBIS, library is CMS_:[SLCSHR_CONTROL]
%CMS-S-LIBSET, library set
-CMS-I-SUPERSEDE, library list superseded
Please enter CMS REPLACE/IF_CHANGED remark.
%CMS-S-GENCREATED, generation 2 of element CMS_:[SLCSHR_CONTROL]AIDASLCDB_XFR_ALPHA.OPT created
MCCDEV> 
```
3. Compile your source code into AIDASLCDB_DEVLIB.OLB
```shell
MCCDEV> LIBRARY /CREATE AIDASLCDB_DEVLIB
MCCDEV> CINC *.C
 Processing file DATA_DISK_SLC:[.AIDASLCDB]AIDASLCDB_SERVERC;13
**MEMBER**=SLCLIBS:AIDASLCDBLIB overridden to AIDASLCDB_DEVLIB
Note:  using NOOPT by default for DECC
    %LIBRAR-S-INSERTED, module SLC_SERVER inserted in DATA_DISK_SLC:[.AIDASLCDB]AIDASLCDB_DEVLIB.OLB;1
```
4. Copy to working directory and make sure your provided `AIDASLCDB_GENERAL.OPT` file contains the correct CMS Card
```text
!==============================================================================
!       **CMS**=SLCSHR_CONTROL
!
! Name: AIDASLCDB_GENERAL.OPT
!
! Rem: Option file to link Channel Provider with AIDA-PVA Modules in AIDA_PVALIB

case_sensitive=YES
SLCLIBS:AIDA_PVALIB.OLB/LIB
SLCLIBS:AIDA_PVAIB.OLB/LIB
SLCLIBS:AIDA_PVALIB.OLB/LIB
case_sensitive=NO

  AIDASHR/shareable
  SYSUTIL/shareable
  UTILSHR/shareable
```
5. Link your Channel Provider
```shell
MCCDEV> BUILDTEST AIDASLCDB /ALL /DEFAULT

Processing AIDASLCDB
 Building Shareable image TST_AIDASLCDB.EXE
%BUILDTEST-I, using AIDASLCDB_DEVLIB/INCL=(all-modules)
MCCDEV> 
```
### AIDASHR Modifications
In order for the Channel Providers to access legacy AIDA functions in AIDASHR the procedure names needed to be added to 
a `SYMBOL_VECTOR` in the AIDASHR_XFR_ALPHA.OPT file. 
This following section was added which contains all the legacy functions called by implemented Channel Providers:
```text

!
! AIDASHR General
case_sensitive=YES
SYMBOL_VECTOR=(-
    DPSLCBPM_ACQINIT=PROCEDURE,-
    DPSLCBPM_ACQTERM=PROCEDURE,-
    DPSLCBPM_BPMACQ=PROCEDURE,-
    DPSLCBPM_GETHSTA=PROCEDURE,-
    DPSLCBPM_GETNAMES=PROCEDURE,-
    DPSLCBPM_GETSTAT=PROCEDURE,-
    DPSLCBPM_GETTMIT=PROCEDURE,-
    DPSLCBPM_GETX=PROCEDURE,-
    DPSLCBPM_GETY=PROCEDURE,-
    DPSLCBPM_GETZ=PROCEDURE,-
    DPSLCBPM_SORTBYZ=PROCEDURE,-
    DPSLCBUFF_ACQ=PROCEDURE,-
    DPSLCBUFF_ACQINIT=PROCEDURE,-
    DPSLCBUFF_ACQTERM=PROCEDURE,-
    DPSLCBUFF_GETGOODMEASES=PROCEDURE,-
    DPSLCBUFF_GETNAMES=PROCEDURE,-
    DPSLCBUFF_GETPULSEIDS=PROCEDURE,-
    DPSLCBUFF_GETSTATS=PROCEDURE,-
    DPSLCBUFF_GETTABLEMROWS=PROCEDURE,-
    DPSLCBUFF_GETTMITS=PROCEDURE,-
    DPSLCBUFF_GETXS=PROCEDURE,-
    DPSLCBUFF_GETYS=PROCEDURE,-
    DPSLCBUFF_SETMODE=PROCEDURE,-
    DPSLCKLYS_ACCESSENABLED=PROCEDURE,-
    DPSLCKLYS_GETSTATUS=PROCEDURE,-
    DPSLCKLYS_SETCONFIG=PROCEDURE,-
    DPSLCKLYS_SETDEACTORREACT=PROCEDURE,-
    DPSLCKLYS_SETTRIMPHASE=PROCEDURE,-
    DPSLCMAGNET_ACCESSENABLED=PROCEDURE,-
    DPSLCMAGNET_GET=PROCEDURE,-
    DPSLCMAGNET_GETCLEANUP=PROCEDURE,-
    DPSLCMAGNET_GETNAMES=PROCEDURE,-
    DPSLCMAGNET_GETSECNVALUES=PROCEDURE,-
    DPSLCMAGNET_RET_BACTVALUES=PROCEDURE,-
    DPSLCMAGNET_RET_MAGLIMITS=PROCEDURE,-
    DPSLCMAGNET_RET_SETNUMPVS=PROCEDURE,-
    DPSLCMAGNET_RET_STATENAMES=PROCEDURE,-
    DPSLCMAGNET_SET=PROCEDURE,-
    DPSLCMAGNET_SETCLEANUP=PROCEDURE,-
    DPSLCMAGNET_SETCONFIG=PROCEDURE,-
    DPSLCMAGNET_SETNAMESVALIDATE=PROCEDURE,-
    DPSLCMOSC_ACCESSENABLED=PROCEDURE,-
    DPSLCMOSC_MEASMASTEROSC=PROCEDURE,-
    DPSLCMOSC_SETMASTEROSC=PROCEDURE,-
    DPSLCUTIL_BGRP_ACCESSENABLED=PROCEDURE,-
    DPSLCUTIL_BGRP_SETVAR=PROCEDURE,-
    DPSLCUTIL_DO_MKB=PROCEDURE,-
    DPSLCUTIL_MKB_ACCESSENABLED=PROCEDURE,-
    DPSLCUTIL_MKB_GETABSFLAG=PROCEDURE,-
    DPSLCUTIL_MKB_GETCLEANUP=PROCEDURE,-
    DPSLCUTIL_MKB_GETNAMES=PROCEDURE,-
    DPSLCUTIL_MKB_GETSECNVALUES=PROCEDURE,-
    DPSLCUTIL_TRIG_ACCESSENABLED=PROCEDURE,-
    DPSLCUTIL_TRIG_GETSTATUS=PROCEDURE,-
    DPSLCUTIL_TRIG_SETDEACTORREACT=PROCEDURE,-
    ERRTRANSLATE=PROCEDURE,-
    JNI_DBACCESSENABLED=PROCEDURE,-
    JNI_DBGETBYTE=PROCEDURE,-
    JNI_DBGETBYTEA=PROCEDURE,-
    JNI_DBGETFLOAT=PROCEDURE,-
    JNI_DBGETFLOATA=PROCEDURE,-
    JNI_DBGETINT=PROCEDURE,-
    JNI_DBGETINTA=PROCEDURE,-
    JNI_DBGETSHORT=PROCEDURE,-
    JNI_DBGETSHORTA=PROCEDURE,-
    JNI_DBGETSTRING=PROCEDURE,-
    JNI_DBSETFLOAT=PROCEDURE-
)
case_sensitive=NO
```
### Test the Channel Provider
You can run your provider by starting the forwarder in another terminal and then starting your Channel Provider.
1. Start the Forwarder
```shell
MCCDEV> java -jar SLCLIBS:EPICS_FORWARDER.JAR
Nov 15, 2021 4:17:05 AM org.epics.forwarder.PVAForwarder main
INFO: EPICS Request Forwarder started: 1530 milliseconds
12:17 >
13:17 > 78 requests/h
```
2. Start your Channel Provider
```shell
MCCDEV> java -jar "-Daida.pva.channels.filename=/SLCTXT/AIDASLCDB_CHANNELS.YAML" "-Djava.library.path=." "-Daida.pva.lib.name=TST_AIDASLCDB" SLCLIBS:AIDA-PVA.JAR
Nov 4, 2021 2:17:04 AM edu.stanford.slac.aida.impl.AidaService <clinit>
INFO: Loading Provider Shared Library: TST_AIDASLCDB

       db         88  88888888ba,           db
      d88b        88  88      `"8b         d88b
     d8'`8b       88  88        `8b       d8'`8b
    d8'  `8b      88  88         88      d8'  `8b                8b,dPPYba,   8b       d8  ,adPPYYba,
   d8YaaaaY8b     88  88         88     d8YaaaaY8b     aaaaaaaa  88P'    "8a  `8b     d8'  ""     `Y8
  d8""""""""8b    88  88         8P    d8""""""""8b    """"""""  88       d8   `8b   d8'   ,adPPPPP88
 d8'        `8b   88  88      .a8P    d8'        `8b             88b,   ,a8"    `8b,d8'    88,    ,88
d8'          `8b  88  88888888Y"'    d8'          `8b            88`YbbdP"'       "8"      `"8bbdP"Y8
                                                                 88
                                                                 88
Nov 4, 2021 2:17:06 AM edu.stanford.slac.aida.lib.ChannelProviderFactory create
INFO: Loading channel configuration from: /SLCTXT/AIDASLCDB_CHANNELS.YAML
Nov 4, 2021 2:17:19 AM edu.stanford.slac.aida.lib.ChannelProvider logHostedChannels
INFO:
AIDA-pva Channel Provider : SLC
Nov 4, 2021 2:17:19 AM edu.stanford.slac.aida.lib.ChannelProvider logHostedChannels
INFO: Channels hosted:
  [LEMG:????:*//EEND, RADS:????:*//MTIM, WIRE:????:*:SK2P, FDBK:????:*//CDEL, SOLN:????:*//BMAX, PTGM:????:*:RSOF, MPSI:????:*:CGID, FDBK:????:*:ULBL, LENS:????:*/
/DVIC, ...]

Aida SLC Database Service Initialised
Nov 4, 2021 2:17:29 AM edu.stanford.slac.aida.lib.AidaProviderRunner run
INFO: Server Ready: 27.064s
```
3. Run some tests.  See [Testing](2_5_4_Testing_Framework.md) for information on how to test.

### Push the Channel Provider to Production
Follow the steps below to push the channel provider to production.
#### Create a TEST sharable
  1. Log into `SLCSHR` account on `MCCDEV`
  2. Create `TEST` sharable
```shell
MCCDEV> BUILDSHR AIDASLCDB SAME
```

@note You have to specify one of `SAME`, `MINOR`, or `MAJOR`.  Since no one links against these shareables, `SAME` should always
be appropriate.

#### Set a logical name so that in your session, executables use the TEST shareable in SLCSHR
  1. Logout of `SLCSHR` account and back in as yourself
  2. Set logical name
```shell
MCCDEV> TESTSHR AIDASLCDB
```
  3. Make sure that the forwarder is running on MCCDEV using WARMSLC then start AIDASLCDB with WARMSLC
  4. Run some client tests (pvcall etc) to verify all is working 

#### Create a TEST shareable in a place where all MCCDEV users will use it by default
  1. Log into `SLCSHR` account on `MCCDEV` 
  2. Create shared `TEST` sharable
```shell
MCCDEV> DEVSHR AIDASLCDB
```  
  3. Leave for a few days to make sure no problems surface

#### Create a production shareable and release it into production
  1. Create production shareable
```shell
MCCDEV> NEWSHR AIDASLCDB
```
  3. All servers and clients will now use this version

## Making Changes to a Channel Provider
### Reserve the code for changes
1. Go into working directory
```shell
MCCDEV> set def [.AIDASLCDB]  
```
2. Go into CMS command line
```shell
MCCDEV> SLCCMS
SlcCMS> 
```
3. If you are updating the C source file reserve it
```shell
SlcCMS> SET LIBR CMS_AIDASLCDB
%CMS-I-LIBIS, library is CMS_:[AIDASLCDB]
%CMS-S-LIBSET, library set
SlcCMS> RESERVE AIDASLCDB_SERVER.C
_Remark: Update SLC DB Channel Configuration C File
%CMS-S-RESERVED, generation 6 of element CMS_:[AIDASLCDB]AIDASLCDB_SERVER.C reserved
SlcCMS> 
```
4. If you are updating the header file reserve it
```shell
SlcCMS> SET LIBR CMS_C_INC
%CMS-I-LIBIS, library is CMS_:[C_INC]
%CMS-S-LIBSET, library set
SlcCMS> RESERVE AIDASLCDB_SERVER.H
_Remark: Update SLC DB Channel Configuration Header File
%CMS-S-RESERVED, generation 3 of element CMS_:[C_INC]AIDASLCDB_SERVER.H reserved
SlcCMS> 
```
5. If you are updating the OPT file reserve it
```shell
SlcCMS> SET LIBR CMS_SLCSHR_CONTROL
%CMS-I-LIBIS, library is CMS_:[SLCSHR_CONTROL]
%CMS-S-LIBSET, library set
SlcCMS> RESERVE AIDASLCDB_GENERAL.OPT
_Remark: Update SLC DB Channel Configuration OPT File
%CMS-S-RESERVED, generation 2 of element CMS_:[SLCSHR_CONTROL]AIDASLCDB_GENERAL.OPT reserved
SlcCMS> exit 
MCCDEV>
```
6. Build and test
```shell
MCCDEV> LIBRARY /CREATE AIDASLCDB_DEVLIB
MCCDEV> CINC *.C
 Processing file DATA_DISK_SLC:[.AIDASLCDB]AIDASLCDB_SERVERC;13
**MEMBER**=SLCLIBS:AIDASLCDBLIB overridden to AIDASLCDB_DEVLIB
Note:  using NOOPT by default for DECC
    %LIBRAR-S-INSERTED, module SLC_SERVER inserted in DATA_DISK_SLC:[.AIDASLCDB]AIDASLCDB_DEVLIB.OLB;1
MCCDEV> BUILDTEST AIDASLCDB /ALL /DEFAULT
Processing AIDASLCDB
 Building Shareable image TST_AIDASLCDB.EXE
%BUILDTEST-I, using AIDASLCDB_DEVLIB/INCL=(all-modules)
MCCDEV> 
```

7. After you've verified the changes, push Header and OPT back to CMS

```shell
MCCDEV> CMP AIDASLCDB_SERVER.H,AIDASLCDB_GENERAL.OPT
 Processing file DATA_DISK_SLC:[.AIDASLCDB]AIDASLCDB_SERVER.H;11

SLY, Why are You doing this?
Data:	Update SLC DB Channel Configuration Header File
%CMS-I-LIBIS, library is CMS_:[C_INC]
%CMS-S-LIBSET, library set
-CMS-I-SUPERSEDE, library list superseded
Please enter CMS REPLACE/IF_CHANGED remark. [Update SLC DB Channel Configuration Header File]:
%CMS-S-GENCREATED, generation 3 of element CMS_:[C_INC]AIDASLCDB_SERVER.H created
%STRIP_C_COMMENTS-I, producing file REF_C_INC:AIDASLCDB_SERVER.NO_COMMENTS_H
 Processing file DATA_DISK_SLC:[.AIDASLCDB]AIDASLCDB_GENERAL.OPT;11
%CMS-I-LIBIS, library is CMS_:[OPT]
%CMS-S-LIBSET, library set
-CMS-I-SUPERSEDE, library list superseded
Please enter CMS REPLACE/IF_CHANGED remark. [Update SLC DB Channel Configuration Header File]: Update SLC DB Channel Configuration Opt File
%CMS-S-GENCREATED, generation 2 of element CMS_:[OPT]AIDASLCDB_GENERAL.OPT created
MCCDEV> 
```
7. Go to @ref cbtd to complete deployment process
