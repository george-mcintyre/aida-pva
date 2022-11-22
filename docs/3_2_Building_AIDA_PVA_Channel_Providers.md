# 3.2 - Building AIDA-PVA Channel Providers

Whenever you make changes to any of the following files you need to:

- ✅ Test your changes privately on DEV
- ✅ Commit your changes to CMS
- ✅ Rebuild the `AIDA_PVALIB` object library for the affected Channel Provider Module(s) and/or AIDA-PVA Module
- ✅ Rebuild the affected Channel Provider Sharable Image (EXE) file(s)
- ✅ Deploy on DEV and restart the affected Channel Provider process
- ✅ Deploy on PROD and restart the affected Channel Provider process

@warning You must verify that your code runs as expected **before** committing it to CMS.

## 3.2.1 AIDA-PVA Channel Provider source files

If any of these files change follow the procedures below to rebuild and deploy the updated Channel Providers. 

@note Clicking on the ➥ symbol will link to the correct procedure to follow for the changed file.

- **Group 1**: `AIDA-PVA` Module Header Files
    - [➥ aida_pva_jni_helper.h](@ref group123) (_**C header file**_)
    - [➥ aida_pva_server_helper.h](@ref group123) (_**C header file**_)
    - [➥ aida_pva_types_helper.h](@ref group123) (_**C header file**_)
    - [➥ slac_aida_NativeChannelProvider.h](@ref group123) (_**C header file**_)
- **Group 2**: `AIDA-PVA` Module C Source files
    - [➥ aida_pva_json.c](@ref group123) (_**C source file**_)
    - [➥ aida_pva_server_helper.c](@ref group123) (_**C source file**_)
    - [➥ aida_pva_types_helper.c](@ref group123) (_**C source file**_)
    - [➥ NativeChannelProviderJni.c](@ref group123) (_**C source file**_)
- **Group 3**: `AIDA-PVA` Common Header Files for both `AIDA-PVA` Module and Channel Provider(s)
    - [➥ aida_pva.h](@ref group123) (_Common **C header file**_)
    - [➥ aida_pva_api.h](@ref group123) (_Common **C header file**_)
    - [➥ aida_pva_convert.h](@ref group123) (_Common **C header file**_)
    - [➥ aida_pva_exceptions.h](@ref group123) (_Common **C header file**_)
    - [➥ aida_pva_json.h](@ref group123) (_Common **C header file**_)
    - [➥ aida_pva_memory.h](@ref group123) (_Common **C header file**_)
    - [➥ aida_pva_types.h](@ref group123) (_Common **C header file**_)
    - [➥ aida_pva_uri.h](@ref group123) (_Common **C header file**_)
- **Group 4**: `AIDASHR` Transfer Vectors
    - [➥ AIDASHR_XFR_ALPHA.OPT](@ref group4) (_**Transfer vectors**_)
- **Group 5**: `AIDA-PVA` BPM Channel Provider Files (Group 5)
    - [➥ AIDASLCBPM_CHANNELS.YML](@ref groupconfig) (_**Channel Config**_)
    - [➥ AIDASLCBPM_GENERAL.OPT](@ref groupnonconfig) (_**Linker options** file_)
    - [➥ AIDASLCBPM_SERVER.c](@ref groupnonconfig) (_**C source file**_)
    - [➥ AIDASLCBPM_SERVER.h](@ref groupnonconfig) (_**C header file**_)
    - [➥ AIDASLCBPM_XFR_ALPHA.OPT](@ref groupnonconfig) (_**Transfer vectors**_)
- **Group 6**: `AIDA-PVA` BPM Buffered Acquisition Channel Provider Files
    - [➥ AIDASLCBPMBUFF_CHANNELS.YML](@ref groupconfig) (_**Channel Config**_)
    - [➥ AIDASLCBPMBUFF_GENERAL.OPT](@ref groupnonconfig) (_**Linker options** file_)
    - [➥ AIDASLCBPMBUFF_SERVER.c](@ref groupnonconfig) (_**C source file**_)
    - [➥ AIDASLCBPMBUFF_SERVER.h](@ref groupnonconfig) (_**C header file**_)
    - [➥ AIDASLCBPMBUFF_XFR_ALPHA.OPT](@ref groupnonconfig) (_**Transfer vectors**_)
- **Group 7**: `AIDA-PVA` SLC Database Channel Provider Files
    - [➥ AIDASLCDB_CHANNELS.YAML](@ref groupconfig) (_**Channel Config**_)
    - [➥ AIDASLCDB_CHANNELS.YML](@ref groupconfig) (Very large _**Channel Config**_)
    - [➥ AIDASLCDB_GENERAL.OPT](@ref groupnonconfig) (_**Linker options** file_)
    - [➥ AIDASLCDB_SERVER.c](@ref groupnonconfig) (_**C source file**_)
    - [➥ AIDASLCDB_SERVER.h](@ref groupnonconfig) (_**C header file**_)
    - [➥ AIDASLCDB_XFR_ALPHA.OPT](@ref groupnonconfig) (_**Transfer vectors**_)
- **Group 8**: `AIDA-PVA` Klystron Channel Provider Files
    - [➥ AIDASLCKLYS_CHANNELS.YML](@ref groupconfig) (_**Channel Config**_)
    - [➥ AIDASLCKLYS_GENERAL.OPT](@ref groupnonconfig) (_**Linker options** file_)
    - [➥ AIDASLCKLYS_SERVER.c](@ref groupnonconfig) (_**C source file**_)
    - [➥ AIDASLCKLYS_SERVER.h](@ref groupnonconfig) (_**C header file**_)
    - [➥ AIDASLCKLYS_XFR_ALPHA.OPT](@ref groupnonconfig) (_**Transfer vectors**_)
- **Group 9**: `AIDA-PVA` Magnet Channel Provider Files
    - [➥ AIDASLCMAGNET_CHANNELS.YML](@ref groupconfig) (_**Channel Config**_)
    - [➥ AIDASLCMAGNET_GENERAL.OPT](@ref groupnonconfig) (_**Linker options** file_)
    - [➥ AIDASLCMAGNET_SERVER.c](@ref groupnonconfig) (_**C source file**_)
    - [➥ AIDASLCMAGNET_SERVER.h](@ref groupnonconfig) (_**C header file**_)
    - [➥ AIDASLCMAGNET_XFR_ALPHA.OPT](@ref groupnonconfig) (_**Transfer vectors**_)
- **Group 10**: `AIDA-PVA` Utilities Channel Provider Files
    - [➥ AIDASLCUTIL_CHANNELS.YML](@ref groupconfig) (_**Channel Config**_)
    - [➥ AIDASLCUTIL_GENERAL.OPT](@ref groupnonconfig) (_**Linker options** file_)
    - [➥ AIDASLCUTIL_SERVER.c](@ref groupnonconfig) (_**C source file**_)
    - [➥ AIDASLCUTIL_SERVER.h](@ref groupnonconfig) (_**C header file**_)
    - [➥ AIDASLCUTIL_XFR_ALPHA.OPT](@ref groupnonconfig) (_**Transfer vectors**_)

@note These files are also stored in the SLAC git repository for use with modern IDEs via git. The master version is in
CMS.

```shell
git clone  git@github.com:slaclab/aida-pva.git
```

## 3.2.2 Preparation

### 3.2.2.1 Verify Source Code has correct CARDS

CARDS are comments that are placed at the top of files that are recognised by the SLAC build tools.

- All **C header files** regardless of group:

```c
/*   **CMS**=C_INC   */
```

- All **Transfer vectors** and **Linker options** files regardless of group:

```c
! **CMS**=SLCSHR_CONTROL
```

- All **Channel Config** files regardless of group:

```yaml
#       **CMS**=SLCTXT
#       **MEMBER**=NONE
```

- Group 2

```c
/*   **MEMBER**=SLCLIBS:AIDA_PVALIB
     **ATTRIBUTES**=JNI   */
```

- Any other **C source files** files regardless of group:

```c
/*     **MEMBER**=SLCLIBS:AIDA_PVALIB
       **ATTRIBUTES**=JNI,LIBR_NOGLOBAL */
```

### 3.2.2.2 Copy changed files to a clean directory on DEV

To build a new version for test and subsequent deployment you need to copy changed
files to a clean directory on MCCDEV. The build tools will combine the changed files with the
remaining unchanged files from CMS to build the object library modules and shareable images required.

To get the files you wish to change from CMS do the following:

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">step</th><th class="markdownTableHeadNone">sub-step</th><th class="markdownTableHeadNone">procedure</th></tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

➀ Fetch files you want to change from CMS

</td>
<td class="markdownTableBodyNone">

➊ Enter CMS CLI

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> slccms
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">
</td>
<td class="markdownTableBodyNone">

➋ 🔂 Select library based on type of changed file

</td>
<td class="markdownTableBodyNone">

```shell
SlcCMS> set libr CMS_AIDA_PVA
SlcCMS> set libr CMS_C_INC
SlcCMS> set libr CMS_SLCSHR_CONTROL  
SlcCMS> set libr CMS_SLCTXT  
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➌ 🔂 Fetch changed file(s)

</td>
<td class="markdownTableBodyNone">

```shell
SlcCMS> fetch AIDASLCBPM_SERVER.C 
SlcCMS> fetch AIDASLCBPM_SERVER.h 
SlcCMS> fetch AIDASLCBPM_GENERAL.OPT 
SlcCMS> fetch AIDASLCBPM_CHANNELS.YML

SlcCMS> fetch AIDASLCBPMBUFF_SERVER.C 
SlcCMS> fetch AIDASLCBPMBUFF_SERVER.h 
SlcCMS> fetch AIDASLCBPMBUFF_GENERAL.OPT 
SlcCMS> fetch AIDASLCBPMBUFF_CHANNELS.YML

SlcCMS> fetch AIDASLCDB_SERVER.C 
SlcCMS> fetch AIDASLCDB_SERVER.h 
SlcCMS> fetch AIDASLCDB_GENERAL.OPT 
SlcCMS> fetch AIDASLCDB_CHANNELS.YML
SlcCMS> fetch AIDASLCDB_CHANNELS.YAML

SlcCMS> fetch AIDASLCKLYS_SERVER.C 
SlcCMS> fetch AIDASLCKLYS_SERVER.h 
SlcCMS> fetch AIDASLCKLYS_GENERAL.OPT 
SlcCMS> fetch AIDASLCKLYS_CHANNELS.YML

SlcCMS> fetch AIDASLCMAGNET_SERVER.C 
SlcCMS> fetch AIDASLCMAGNET_SERVER.h 
SlcCMS> fetch AIDASLCMAGNET_GENERAL.OPT 
SlcCMS> fetch AIDASLCMAGNET_CHANNELS.YML

SlcCMS> fetch AIDASLCUTIL_SERVER.C 
SlcCMS> fetch AIDASLCUTIL_SERVER.h 
SlcCMS> fetch AIDASLCUTIL_GENERAL.OPT 
SlcCMS> fetch AIDASLCUTIL_CHANNELS.YML

SlcCMS> fetch DPSLC_JNI_HELPER.C 
SlcCMS> fetch DPSLCBUFF.H 
SlcCMS> fetch DPSLCBUFF_JNI_HELPER.C 
SlcCMS> fetch DPSLCKLYS_JNI_HELPER.c 
SlcCMS> fetch AIDASHR_XFR_ALPHA.OPT 

```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➍ Exit CMS CLI

</td>
<td class="markdownTableBodyNone">

```shell
SlcCMS> exit 
```

</td>
</tr>

</table>


## 3.2.3 Procedures for Building, Testing and Deploying
Select the appropriate ⚙-numbered Build Procedure(s) based on the files you've changed.  

@note Clicking on the ⚙ symbol will link to the procedure steps to follow.

### Groups 1, 2, & 3  {#group123}
- For each _channel-provider_ in [`SLCBPM`, `SLCBPMBUFF`, `SLCDB`, `SLCKLYS`, `SLCMAGNET`, `SLCUTIL`]
    - [⚙-1 Build Private Channel Provider Module](@ref procedure1)
    - [⚙-2 Build Private Channel Provider Shareable Image](@ref procedure2)
- For each _channel-provider_ in [`SLCBPM`, `SLCBPMBUFF`, `SLCDB`, `SLCKLYS`, `SLCMAGNET`, `SLCUTIL`]
    - [⚙-3 Commit & Build Channel Provider Module](@ref procedure3)
    - [⚙-4 Build & Test Channel Provider Shareable in DEV](@ref procedure4)
    - [⚙-5 Move Channel Provider Shareable to PROD](@ref procedure5)
### Group 4 {#group4}
- [⚙-9 Build Private AIDASHR](@ref procedure9)
- For each _channel-provider_ in [`SLCBPM`, `SLCBPMBUFF`, `SLCDB`, `SLCKLYS`, `SLCMAGNET`, `SLCUTIL`]
    - [⚙-10 Build Private Channel Provider Shareable Image using Private AIDASHR](@ref procedure10)
- [⚙-11 Build and Test AIDA Providers Privately](@ref procedure11)
- [⚙-12 Commit AIDASHR changes to CMS and Deploy to PROD](@ref procedure12)
- For each _channel-provider_ in [`SLCBPM`, `SLCBPMBUFF`, `SLCDB`, `SLCKLYS`, `SLCMAGNET`, `SLCUTIL`]
    - [⚙-4 Build & Test Channel Provider Shareable in DEV](@ref procedure4)
    - [⚙-5 Move Channel Provider Shareable to PROD](@ref procedure5)
### Groups 5, 6, 7, 8, 9, & 10
#### Channel Config files {#groupconfig}
- [⚙-6 Test Channel Config locally](@ref procedure6)
- [⚙-7 Deploy and Test Channel Config in DEV](@ref procedure7)
- [⚙-8 Move Channel Config to PROD](@ref procedure8)
#### All files except Channel Config files {#groupnonconfig}
- [⚙-1 Build Private Channel Provider Module](@ref procedure1)
- [⚙-2 Build Private Channel Provider Shareable Image](@ref procedure2)
- [⚙-3 Commit & Build Channel Provider Module](@ref procedure3)
- [⚙-4 Build & Test Channel Provider Shareable in DEV](@ref procedure4)
- [⚙-5 Move Channel Provider Shareable to PROD](@ref procedure5)

## 3.2.4 Channel Provider Build Procedures
@note the 🔂 symbol means, select only one or more of the listed procedures that apply to your changed files 

### ⚙-1: Build Private Channel Provider Module {#procedure1}

Build an `AIDA-PVA` Channel Provider Module in a private object library

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">step</th><th class="markdownTableHeadNone">procedure</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➀ Create DEV library

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> LIBRARY /CREATE DEVLIB
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

➁ Compile all changed C files including local header files
</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> CINC *.C
```

</td>
</tr>
</table>

### ⚙-2: Build Private Channel Provider Shareable Image {#procedure2}

Build and Test an `AIDA-PVA` Channel Provider Shareable Image privately

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">step</th><th class="markdownTableHeadNone">sub-step</th><th class="markdownTableHeadNone">procedure</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➀ Build Shareable Image

</td>
<td class="markdownTableBodyNone">

➊ 🔂 Edit `GENERAL.OPT` file

@note to save and exit press `F10`

</td>
<td class="markdownTableBodyNone">

- Get OPT file
    - checkout of CMS or
    - change if updated
- Comment out line ending in

  `remove this line for BUILDTEST /DEF`

```shell
MCCDEV>  eve AIDASLCBPM_GENERAL.OPT  
MCCDEV>  eve AIDASLCBPMBUFF_GENERAL.OPT  
MCCDEV>  eve AIDASLCDB_GENERAL.OPT  
MCCDEV>  eve AIDASLCKLYS_GENERAL.OPT  
MCCDEV>  eve AIDASLCMAGNET_GENERAL.OPT  
MCCDEV>  eve AIDASLCUTIL_GENERAL.OPT  
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➋ 🔂 Build

@note `/DEFAULT` means use the files in the local directory

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> BUILDTEST AIDASLCBPM /ALL /DEFAULT
MCCDEV> BUILDTEST AIDASLCBPMBUFF /ALL /DEFAULT
MCCDEV> BUILDTEST AIDASLCDB /ALL /DEFAULT
MCCDEV> BUILDTEST AIDASLCKLYS /ALL /DEFAULT
MCCDEV> BUILDTEST AIDASLCMAGNET /ALL /DEFAULT
MCCDEV> BUILDTEST AIDASLCUTIL /ALL /DEFAULT
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➁ Test Shareable Image

</td>
<td class="markdownTableBodyNone">

➊ Log into DEV machine using `SLCSHR` account

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> ssh MCCDEV /user=slcshr
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➋ Shutdown the running Channel Provider on DEV 🔂

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> WARMSLC AIDA_SLCBPM /KILL
MCCDEV::SLCSHR> WARMSLC AIDA_SLCBPMBUFF /KILL
MCCDEV::SLCSHR> WARMSLC AIDA_SLCDB /KILL
MCCDEV::SLCSHR> WARMSLC AIDA_SLCKLYS /KILL
MCCDEV::SLCSHR> WARMSLC AIDA_SLCMAGNET /KILL
MCCDEV::SLCSHR> WARMSLC AIDA_SLCUTIL /KILL
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➌ Log out of `SLCSHR` account

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> logout
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➍ 🔂 Run private Shareable Image

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> @AIDA_SLCBPM.SUBMIT
MCCDEV> @AIDA_SLCBPMBUFF.SUBMIT
MCCDEV> @AIDA_SLCDB.SUBMIT
MCCDEV> @AIDA_SLCKLYS.SUBMIT
MCCDEV> @AIDA_SLCMAGNET.SUBMIT
MCCDEV> @AIDA_SLCUTIL.SUBMIT
```

</td>
</tr>

</table>

### ⚙-3: Commit & Build Channel Provider Module {#procedure3}

Commit and Rebuild an `AIDA-PVA` Channel Provider Module in the `AIDA_PVALIB` object library

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">step</th><th class="markdownTableHeadNone">sub-step</th><th class="markdownTableHeadNone">procedure</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➀ Build and Test locally first

</td>
<td class="markdownTableBodyNone">
</td>
<td class="markdownTableBodyNone">

see above

</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

➁ Reserve changed files in CMS

</td>
<td class="markdownTableBodyNone">

➊ Enter CMS CLI

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> slccms
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">
</td>
<td class="markdownTableBodyNone">

➋ 🔂 Select library based on type of changed file

</td>
<td class="markdownTableBodyNone">

```shell
SlcCMS> set libr CMS_AIDA_PVA
SlcCMS> set libr CMS_C_INC
SlcCMS> set libr CMS_SLCSHR_CONTROL  
SlcCMS> set libr CMS_SLCTXT  
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➌ 🔂 Reserve changed file

</td>
<td class="markdownTableBodyNone">

```shell
SlcCMS> reserve AIDASLCBPM_SERVER.C /nooutput 
SlcCMS> reserve AIDASLCBPM_SERVER.h /nooutput 
SlcCMS> reserve AIDASLCBPM_GENERAL.OPT /nooutput 

SlcCMS> reserve AIDASLCBPMBUFF_SERVER.C /nooutput 
SlcCMS> reserve AIDASLCBPMBUFF_SERVER.h /nooutput 
SlcCMS> reserve AIDASLCBPMBUFF_GENERAL.OPT /nooutput 

SlcCMS> reserve AIDASLCDB_SERVER.C /nooutput 
SlcCMS> reserve AIDASLCDB_SERVER.h /nooutput 
SlcCMS> reserve AIDASLCDB_GENERAL.OPT /nooutput 

SlcCMS> reserve AIDASLCKLYS_SERVER.C /nooutput 
SlcCMS> reserve AIDASLCKLYS_SERVER.h /nooutput 
SlcCMS> reserve AIDASLCKLYS_GENERAL.OPT /nooutput 

SlcCMS> reserve AIDASLCMAGNET_SERVER.C /nooutput 
SlcCMS> reserve AIDASLCMAGNET_SERVER.h /nooutput 
SlcCMS> reserve AIDASLCMAGNET_GENERAL.OPT /nooutput 

SlcCMS> reserve AIDASLCUTIL_SERVER.C /nooutput 
SlcCMS> reserve AIDASLCUTIL_SERVER.h /nooutput 
SlcCMS> reserve AIDASLCUTIL_GENERAL.OPT /nooutput 
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➍ Exit CMS CLI

</td>
<td class="markdownTableBodyNone">

```shell
SlcCMS> exit 
```

</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

➂ Commit changed files:

</td>
<td class="markdownTableBodyNone">
➊ 🔂
</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV>  cmp AIDASLCBPM_SERVER.C  
MCCDEV>  cmp AIDASLCBPM_SERVER.h  
MCCDEV>  cmp AIDASLCBPM_GENERAL.OPT  

MCCDEV>  cmp AIDASLCBPMBUFF_SERVER.C  
MCCDEV>  cmp AIDASLCBPMBUFF_SERVER.h  
MCCDEV>  cmp AIDASLCBPMBUFF_GENERAL.OPT  

MCCDEV>  cmp AIDASLCDB_SERVER.C  
MCCDEV>  cmp AIDASLCDB_SERVER.h  
MCCDEV>  cmp AIDASLCDB_GENERAL.OPT  

MCCDEV>  cmp AIDASLCKLYS_SERVER.C  
MCCDEV>  cmp AIDASLCKLYS_SERVER.h  
MCCDEV>  cmp AIDASLCKLYS_GENERAL.OPT  

MCCDEV>  cmp AIDASLCMAGNET_SERVER.C  
MCCDEV>  cmp AIDASLCMAGNET_SERVER.h  
MCCDEV>  cmp AIDASLCMAGNET_GENERAL.OPT  

MCCDEV>  cmp AIDASLCUTIL_SERVER.C  
MCCDEV>  cmp AIDASLCUTIL_SERVER.h  
MCCDEV>  cmp AIDASLCUTIL_GENERAL.OPT  
```

</td>
</tr>

</table>

### ⚙-4: Build & Test Channel Provider Shareable Image in DEV {#procedure4}

Rebuild and Test an `AIDA-PVA` Channel Provider Shareable Image in `DEV`

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">step</th><th class="markdownTableHeadNone">sub-step</th><th class="markdownTableHeadNone">procedure</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➀ Must have committed changes to CMS prior to executing this step

</td>
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

see above

</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

➁ Rebuild a new Shareable Image on DEV

</td>
<td class="markdownTableBodyNone">

➊ Log into dev machine using `SLCSHR` account

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> ssh MCCDEV /user=slcshr
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➋ 🔂 Build new Shareable Image

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> BUILDSHR AIDASLCBPM SAME
MCCDEV::SLCSHR> BUILDSHR AIDASLCBPMBUFF SAME
MCCDEV::SLCSHR> BUILDSHR AIDASLCDB SAME
MCCDEV::SLCSHR> BUILDSHR AIDASLCKLYS SAME
MCCDEV::SLCSHR> BUILDSHR AIDASLCMAGNET SAME
MCCDEV::SLCSHR> BUILDSHR AIDASLCUTIL SAME
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➌ 🔂 Make Sharable Image available on DEV server

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> DEVSHR AIDASLCBPM
MCCDEV::SLCSHR> DEVSHR AIDASLCBPMBUFF
MCCDEV::SLCSHR> DEVSHR AIDASLCDB
MCCDEV::SLCSHR> DEVSHR AIDASLCKLYS
MCCDEV::SLCSHR> DEVSHR AIDASLCMAGNET
MCCDEV::SLCSHR> DEVSHR AIDASLCUTIL
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➍ 🔂 Start new Channel Provider on DEV

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> WARMSLC AIDA_SLCBPM /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCBPMBUFF /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCDB /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCKLYS /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCMAGNET /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCUTIL /RESTART
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➎ Log out of `SLCSHR` account

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> logout
```

</td>
</tr>

<tr class="markdownTableRowEven">
 <td class="markdownTableBodyNone"></td>
 <td class="markdownTableBodyNone">

➏ Leave and test for a number of days ...

 </td>
 <td class="markdownTableBodyNone">

 </td>
 </tr>

</table>

### ⚙-5: Move Channel Provider Shareable Image to PROD {#procedure5}

Move an `AIDA-PVA` Channel Provider Shareable Image to `PROD`

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">step</th><th class="markdownTableHeadNone">procedure</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➀ Log into DEV using SLCSHR account

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> ssh MCCDEV /user=slcshr
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

➁ 🔂 Copy DEV Shareable Image to PROD

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> NEWSHR AIDASLCBPM
MCCDEV::SLCSHR> NEWSHR AIDASLCBPMBUFF
MCCDEV::SLCSHR> NEWSHR AIDASLCDB
MCCDEV::SLCSHR> NEWSHR AIDASLCKLYS
MCCDEV::SLCSHR> NEWSHR AIDASLCMAGNET
MCCDEV::SLCSHR> NEWSHR AIDASLCUTIL
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➂ 🔂 Start new Channel Provider on DEV

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> WARMSLC AIDA_SLCBPM /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCBPMBUFF /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCDB /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCKLYS /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCMAGNET /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCUTIL /RESTART
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

➃ Log into PROD machine using `SLCSHR` account

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> ssh MCC /user=slcshr
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➄ 🔂 Start new Channel Provider on PROD

</td>
<td class="markdownTableBodyNone">

```shell
MCC::SLCSHR> WARMSLC AIDA_SLCBPM /RESTART
MCC::SLCSHR> WARMSLC AIDA_SLCBPMBUFF /RESTART
MCC::SLCSHR> WARMSLC AIDA_SLCDB /RESTART
MCC::SLCSHR> WARMSLC AIDA_SLCKLYS /RESTART
MCC::SLCSHR> WARMSLC AIDA_SLCMAGNET /RESTART
MCC::SLCSHR> WARMSLC AIDA_SLCUTIL /RESTART
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

➅ Log out of `SLCSHR` account on PROD

</td>
<td class="markdownTableBodyNone">

```shell
MCC::SLCSHR> logout
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➆ Log out of `SLCSHR` account on DEV

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> logout
```

</td>
</tr>

</table>

### ⚙-6: Test Channel Config locally {#procedure6}
Set logical so testing uses local Channel Config file

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">step</th><th class="markdownTableHeadNone">sub-step</th><th class="markdownTableHeadNone">procedure</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➀ Set logical to use local config

</td>
<td class="markdownTableBodyNone">

➊ 🔂 Set logical

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> TESTYML AIDASLCBPM_CHANNELS.YML
MCCDEV> TESTYML AIDASLCBPMBUFF_CHANNELS.YML
MCCDEV> TESTYML AIDASLCDB_CHANNELS.YAML
MCCDEV> TESTYML AIDASLCKLYS_CHANNELS.YML
MCCDEV> TESTYML AIDASLCMAGNET_CHANNELS.YML
MCCDEV> TESTYML AIDASLCUTIL_CHANNELS.YML
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➋ 🔂 Optionally Verify 

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> SHOWTESTYML AIDASLCBPM_CHANNELS.YML
MCCDEV> SHOWTESTYML AIDASLCBPMBUFF_CHANNELS.YML
MCCDEV> SHOWTESTYML AIDASLCDB_CHANNELS.YAML
MCCDEV> SHOWTESTYML AIDASLCKLYS_CHANNELS.YML
MCCDEV> SHOWTESTYML AIDASLCMAGNET_CHANNELS.YML
MCCDEV> SHOWTESTYML AIDASLCUTIL_CHANNELS.YML
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➁ Test Config

</td>
<td class="markdownTableBodyNone">

➊ Log into DEV machine using `SLCSHR` account

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> ssh MCCDEV /user=slcshr
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➋ 🔂 Shutdown running Channel Provider on DEV

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> WARMSLC AIDA_SLCBPM /KILL
MCCDEV::SLCSHR> WARMSLC AIDA_SLCBPMBUFF /KILL
MCCDEV::SLCSHR> WARMSLC AIDA_SLCDB /KILL
MCCDEV::SLCSHR> WARMSLC AIDA_SLCKLYS /KILL
MCCDEV::SLCSHR> WARMSLC AIDA_SLCMAGNET /KILL
MCCDEV::SLCSHR> WARMSLC AIDA_SLCUTIL /KILL
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➌ Log out of `SLCSHR` account

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> logout
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➍ 🔂 Run private Shareable Image

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> @AIDA_SLCBPM.SUBMIT
MCCDEV> @AIDA_SLCBPMBUFF.SUBMIT
MCCDEV> @AIDA_SLCDB.SUBMIT
MCCDEV> @AIDA_SLCKLYS.SUBMIT
MCCDEV> @AIDA_SLCMAGNET.SUBMIT
MCCDEV> @AIDA_SLCUTIL.SUBMIT
```

</td>
</tr>


</table>


### ⚙-7 Deploy and Test Channel Config in DEV {#procedure7}

Deploy appropriate Channel Config file to PROD

@note Because of its large size, `AIDASLCDB_CHANNELS.YML` is treated as a special case and so is not copied to `SLCIMAGE` and `CMS_REPLACE` is used instead of `CMS_REPLACE/IF_CHANGED` when committing it to CMS.

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">step</th><th class="markdownTableHeadNone">sub-step</th><th class="markdownTableHeadNone">procedure</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➀ Set Config for use in DEV and update in `CMS_SLCTXT`

</td>
<td class="markdownTableBodyNone">

➊ 🔂

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> DEVYML AIDASLCBPM_CHANNELS.YML
MCCDEV> DEVYML AIDASLCBPMBUFF_CHANNELS.YML
MCCDEV> DEVYML AIDASLCDB_CHANNELS.YML
MCCDEV> DEVYML AIDASLCDB_CHANNELS.YAML
MCCDEV> DEVYML AIDASLCKLYS_CHANNELS.YML
MCCDEV> DEVYML AIDASLCMAGNET_CHANNELS.YML
MCCDEV> DEVYML AIDASLCUTIL_CHANNELS.YML
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➁ Load Config on DEV

</td>
<td class="markdownTableBodyNone">

➊ Log into DEV machine using `SLCSHR` account

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> ssh MCCDEV /user=slcshr
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➋ Restart running Channel Provider on DEV 🔂

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> WARMSLC AIDA_SLCBPM /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCBPMBUFF /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCDB /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCKLYS /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCMAGNET /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCUTIL /RESTART
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➌ Log out of `SLCSHR` account

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> logout
```

</td>
</tr>

</table>

### ⚙-8 Move Channel Config to PROD {#procedure8}

Deploy Channel Config file to PROD

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">step</th><th class="markdownTableHeadNone">sub-step</th><th class="markdownTableHeadNone">procedure</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➀ Move to Prod

</td>
<td class="markdownTableBodyNone">

➊ 🔂

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> NEWYML AIDASLCBPM_CHANNELS.YML
MCCDEV> NEWYML AIDASLCBPMBUFF_CHANNELS.YML
MCCDEV> NEWYML AIDASLCDB_CHANNELS.YAML
MCCDEV> NEWYML AIDASLCKLYS_CHANNELS.YML
MCCDEV> NEWYML AIDASLCMAGNET_CHANNELS.YML
MCCDEV> NEWYML AIDASLCUTIL_CHANNELS.YML
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➁ Reload Config on DEV if required

</td>
<td class="markdownTableBodyNone">

➊ Log into DEV machine using `SLCSHR` account

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> ssh MCCDEV /user=slcshr
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➋ Restart running Channel Provider on DEV 🔂

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> WARMSLC AIDA_SLCBPM /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCBPMBUFF /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCDB /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCKLYS /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCMAGNET /RESTART
MCCDEV::SLCSHR> WARMSLC AIDA_SLCUTIL /RESTART
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➌ Log out of `SLCSHR` account

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> logout
```

</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

➂ Load Config on PROD

</td>
<td class="markdownTableBodyNone">

➊ Log into PROD machine using `SLCSHR` account

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> ssh MCC /user=slcshr
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➋ Restart running Channel Provider on PROD 🔂

</td>
<td class="markdownTableBodyNone">

```shell
MCC::SLCSHR> WARMSLC AIDA_SLCBPM /RESTART
MCC::SLCSHR> WARMSLC AIDA_SLCBPMBUFF /RESTART
MCC::SLCSHR> WARMSLC AIDA_SLCDB /RESTART
MCC::SLCSHR> WARMSLC AIDA_SLCKLYS /RESTART
MCC::SLCSHR> WARMSLC AIDA_SLCMAGNET /RESTART
MCC::SLCSHR> WARMSLC AIDA_SLCUTIL /RESTART
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➌ Log out of `SLCSHR` account

</td>
<td class="markdownTableBodyNone">

```shell
MCC::SLCSHR> logout
```

</td>
</tr>

</table>

## 3.2.5 AIDASHR Specific Build Procedures

### ⚙-9: Build Private AIDASHR {#procedure9}

Build `AIDASHR` Shared Library privately

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">step</th><th class="markdownTableHeadNone">sub-step</th><th class="markdownTableHeadNone">procedure</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➀ Create DEV library

</td>
<td class="markdownTableBodyNone">

➊ Create DEV library

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> LIBRARY /CREATE AIDASHR_DEVLIB
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➋ 🔂 Build

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> CINC DPSLC_JNI_HELPER.C
MCCDEV> CINC DPSLCBUFF_JNI_HELPER.C
MCCDEV> CINC DPSLCKLYS_JNI_HELPER.c
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➁ Create Shareable Image

</td>
<td class="markdownTableBodyNone">

➊ Create image

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> BUILDTEST AIDASHR /ALL /DEFAULT
```

</td>
</tr>
</table>

### ⚙-10: Build Private Channel Provider Shareable Image using Private AIDASHR {#procedure10}

Build and Test an `AIDA-PVA` Channel Provider Shareable Image privately using private `AIDASHR` library

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">step</th><th class="markdownTableHeadNone">sub-step</th><th class="markdownTableHeadNone">procedure</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➀ Build Shareable Image

</td>
<td class="markdownTableBodyNone">

➊ 🔂 Edit `GENERAL.OPT` file

@note to save and exit press `F10`

</td>
<td class="markdownTableBodyNone">

- Get OPT file
    - checkout of CMS or
    - change if updated
- Comment out line ending in

  `remove this line for BUILDTEST /DEF`

```shell
MCCDEV>  eve AIDASLCBPM_GENERAL.OPT  
MCCDEV>  eve AIDASLCBPMBUFF_GENERAL.OPT  
MCCDEV>  eve AIDASLCDB_GENERAL.OPT  
MCCDEV>  eve AIDASLCKLYS_GENERAL.OPT  
MCCDEV>  eve AIDASLCMAGNET_GENERAL.OPT  
MCCDEV>  eve AIDASLCUTIL_GENERAL.OPT  
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➋ 🔂 Build

@note `/DEFAULT` means use the files in the local directory

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> BUILDTEST AIDASLCBPM /ALL /DEFAULT
MCCDEV> BUILDTEST AIDASLCBPMBUFF /ALL /DEFAULT
MCCDEV> BUILDTEST AIDASLCDB /ALL /DEFAULT
MCCDEV> BUILDTEST AIDASLCKLYS /ALL /DEFAULT
MCCDEV> BUILDTEST AIDASLCMAGNET /ALL /DEFAULT
MCCDEV> BUILDTEST AIDASLCUTIL /ALL /DEFAULT
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➁ Test Shareable Image

</td>
<td class="markdownTableBodyNone">

➊ Log into DEV machine using `SLCSHR` account

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> ssh MCCDEV /user=slcshr
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➋ 🔂 Shutdown running Channel Provider on DEV

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> WARMSLC AIDA_SLCBPM /KILL
MCCDEV::SLCSHR> WARMSLC AIDA_SLCBPMBUFF /KILL
MCCDEV::SLCSHR> WARMSLC AIDA_SLCDB /KILL
MCCDEV::SLCSHR> WARMSLC AIDA_SLCKLYS /KILL
MCCDEV::SLCSHR> WARMSLC AIDA_SLCMAGNET /KILL
MCCDEV::SLCSHR> WARMSLC AIDA_SLCUTIL /KILL
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➌ Log out of `SLCSHR` account

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> logout
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➍ 🔂 Run private Shareable Image

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> @AIDA_SLCBPM.SUBMIT
MCCDEV> @AIDA_SLCBPMBUFF.SUBMIT
MCCDEV> @AIDA_SLCDB.SUBMIT
MCCDEV> @AIDA_SLCKLYS.SUBMIT
MCCDEV> @AIDA_SLCMAGNET.SUBMIT
MCCDEV> @AIDA_SLCUTIL.SUBMIT
```

</td>
</tr>

</table>

### ⚙-11: Build and Test AIDA Channel Providers Privately {#procedure11}

Build and Test all `AIDA` Channel Providers privately

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">step</th><th class="markdownTableHeadNone">procedure</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➀ Build and Test all `AIDA` Channel Providers privately

</td>
<td class="markdownTableBodyNone">

See [legacy AIDA documentation](https://www.slac.stanford.edu/grp/cd/soft/aida/) for instructions

</td>
</tr>


</table>

### ⚙-12: Commit AIDASHR changes to CMS and deploy to PROD {#procedure12}

Commit changes to `AIDASHR` Shared Library

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">step</th><th class="markdownTableHeadNone">sub-step</th><th class="markdownTableHeadNone">procedure</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➀ Build and Test locally first

</td>
<td class="markdownTableBodyNone">
</td>
<td class="markdownTableBodyNone">

see above

</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

➁ Reserve changed files in CMS

</td>
<td class="markdownTableBodyNone">

➊ Enter CMS CLI

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> slccms
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">
</td>
<td class="markdownTableBodyNone">

➋ 🔂 Select library based on type of changed file

</td>
<td class="markdownTableBodyNone">

```shell
SlcCMS> set libr CMS_AIDASHR
SlcCMS> set libr CMS_C_INC
SlcCMS> set libr CMS_SLCSHR_CONTROL  
SlcCMS> set libr CMS_SLCTXT  
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➌ 🔂 Reserve changed file

</td>
<td class="markdownTableBodyNone">

```shell
SlcCMS> reserve DPSLC_JNI_HELPER.C /nooutput 
SlcCMS> reserve DPSLCBUFF.H /nooutput 
SlcCMS> reserve DPSLCBUFF_JNI_HELPER.C /nooutput 
SlcCMS> reserve DPSLCKLYS_JNI_HELPER.c /nooutput 
SlcCMS> reserve AIDASHR_XFR_ALPHA.OPT /nooutput 
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➍ Exit CMS CLI

</td>
<td class="markdownTableBodyNone">

```shell
SlcCMS> exit 
```

</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

➂ Commit changed files:

</td>
<td class="markdownTableBodyNone">
➊ 🔂
</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV>  cmp DPSLC_JNI_HELPER.C  
MCCDEV>  cmp DPSLCBUFF.H  
MCCDEV>  cmp DPSLCBUFF_JNI_HELPER.C  
MCCDEV>  cmp DPSLCKLYS_JNI_HELPER.c  
MCCDEV>  cmp AIDASHR_XFR_ALPHA.OPT  
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➃ Rebuild a new Shareable Image on DEV

</td>
<td class="markdownTableBodyNone">

➊ Log into dev machine using `SLCSHR` account

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> ssh MCCDEV /user=slcshr
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➋ 🔂 Build new Shareable Image

@note use `SAME` if no new entry points are added, `MINOR` if you added some

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> BUILDSHR AIDASHR SAME
MCCDEV::SLCSHR> BUILDSHR AIDASHR MINOR
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➌ Make Sharable Image available on DEV server

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> DEVSHR AIDASHR
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➍ 🔂 Build and deploy Channel providers using `AIDASHR` on DEV

</td>
<td class="markdownTableBodyNone">

[⚙-3 Commit & Build Channel Provider Module](@ref procedure3)

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➎ Log out of `SLCSHR` account

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> logout
```

</td>
</tr>

<tr class="markdownTableRowEven">
 <td class="markdownTableBodyNone"></td>
 <td class="markdownTableBodyNone">

➏ Leave and test for a number of days ...

 </td>
 <td class="markdownTableBodyNone">

 </td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➃ Deploy `AIDASHR` to PROD

</td>
<td class="markdownTableBodyNone">

➊ Log in to `SLCSHR` account

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> ssh MCCDEV /user=slcshr
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➋ Copy DEV Shareable Image to PROD

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> NEWSHR AIDASHR
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone"></td>
<td class="markdownTableBodyNone">

➌ Log out of `SLCSHR` account on DEV

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV::SLCSHR> logout
```

</td>
</tr>

</table>

## 3.2.6 Graphical representation of files in AIDA-PVA

![AIDA-PVA files graphical representation](images/aida-pva-files.png)

