# 3.3 - Building an AIDA-PVA Channel Provider

## First Time
This step only needs to be done once to do the initial compilation and test and push code to CMS. 
All examples below show AIDASLCDB.

### Deploy your Channel Configuration File
1. Check out original code from github (first time only - afterwards maintain code in CMS)
```shell
cd dev
git clone  git@github.com:slaclab/aida-pva.git
cd aida-pva/src/cpp/providers/slc
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
cd aida-pva/src/cpp/providers/slc
```
4. Copy your code (1 C file, 1 header file, and 1 OPT file usually) to a directory on MCCDEV
```shell
> sftp mccdev
Connecting to mccdev...
 
sftp> mkdir AIDASLCDB
sftp> cd AIDASLCDB
sftp> mput *.c
sftp> mput *.h
sftp> mput *.OPT
```
5. On VMS, Go into development directory
```shell
MCCDEV> set def [.AIDASLCDB] 
```

### Compile the code
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
2. Copy STANDALONELIB_XFR_ALPHA.OPT to your directory
```shell
 MCCDEV> COPY [-.AIDA-PVA]STANDALONELIB_XFR_ALPHA.OPT AIDASLCDB_XFR_ALPHA.OPT
 MCCDEV>
```
3. Compile your source code into AIDASLCDB_DEVLIB.OLB
```shell
MCCDEV> LIBRARY /CREATE AIDASLCDB_DEVLIB
MCCDEV> CINC *.C
 Processing file DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.PROVIDERS.SLCDB]AIDASLCDB_SERVERC;13
**MEMBER**=SLCLIBS:AIDASLCDBLIB overridden to AIDASLCDB_DEVLIB
Note:  using NOOPT by default for DECC
%LIBRAR-S-INSERTED, module SLC_SERVER inserted in DATA_DISK_SLC:[SCRATCH.SLY.DEV.AIDA-PVA.PROVIDERS.SLCDB]AIDASLCDB_DEVLIB.OLB;1
```
4. Make sure your provided `AIDASLCDB_GENERAL.OPT` file contains the correct CMS Card, and references to all the Legacy AIDA Modules you require
```text
!==============================================================================
!       **CMS**=OPT
!
! Name: AIDASLCDB_GENERAL.OPT
!
! Rem: Option file to link Channel Provider with AIDA-PVA Modules in STANDALONELIB

case_sensitive=YES
SLCLIBS:STANDALONELIB.OLB/INCLUDE=( -
 NATIVECHANNELPROVIDERJNI, -
 AIDA_PVA_SERVER_HELPER, -
 AIDA_PVA_JNI_HELPER, -
 AIDA_PVA_TYPES_HELPER, -
 AIDA_PVA_JSON) / LIB
SLCLIBS:AIDASHRLIB.OLB/LIB
case_sensitive=NO

  DBSSHR/shareable
  IOBSIC/shareable
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
### Test the Channel Provider
You can run your provider by starting the forwarder in another terminal and then starting your Channel Provider.
1. Start the Forwarder
```shell
MCCDEV> java -jar SLCLIBS:EPICS-FORWARDER.JAR
Nov 4, 2021 2:22:29 AM org.epics.forwarder.PVAForwarder main
INFO: EPICS Request Forwarder started: 1987 milliseconds
9:22 >
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

### When you are happy push the Channel Provider to Production
1. Log into `SLCSHR` account on `MCCDEV`
2. `BUILDSHR AIDASLCDB` - creates a test sharable
3. Logout of `SLCSHR` account and back in as yourself 
4. `TESTSHRX AIDASLCDB /dev`  - sets a logical name so that local executables use that shareable in `SLCSHR` and not any other. 
5. Test to verify all is working 
6. Log into `SLCSHR` account on `MCCDEV` 
7. `DEVSHR AIDASLCDB` - creates a test shareable in a place where all `MCCDEV` users are using it by default
8. Leave for a few days to make sure no problems surface 
9. `NEWSHR AIDASLCDB` - create production shareable and releases it into production - so all servers and clients are using it

