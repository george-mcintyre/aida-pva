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
    - [➥](@ref group123)`aida_pva_jni_helper.h` (_**C header file**_)
    - [➥](@ref group123)`aida_pva_server_helper.h` (_**C header file**_)
    - [➥](@ref group123)`aida_pva_types_helper.h` (_**C header file**_)
    - [➥](@ref group123)`slac_aida_NativeChannelProvider.h` (_**C header file**_)
- **Group 2**: `AIDA-PVA` Module C Source files
    - [➥](@ref group123)`aida_pva_json.c` (_**C source file**_)
    - [➥](@ref group123)`aida_pva_server_helper.c` (_**C source file**_)
    - [➥](@ref group123)`aida_pva_types_helper.c` (_**C source file**_)
    - [➥](@ref group123)`NativeChannelProviderJni.c` (_**C source file**_)
- **Group 3**: `AIDA-PVA` Common Header Files for both `AIDA-PVA` Module and Channel Provider(s)
    - [➥](@ref group123)`aida_pva.h` (_Common **C header file**_)
    - [➥](@ref group123)`aida_pva_api.h` (_Common **C header file**_)
    - [➥](@ref group123)`aida_pva_convert.h` (_Common **C header file**_)
    - [➥](@ref group123)`aida_pva_exceptions.h` (_Common **C header file**_)
    - [➥](@ref group123)`aida_pva_json.h` (_Common **C header file**_)
    - [➥](@ref group123)`aida_pva_memory.h` (_Common **C header file**_)
    - [➥](@ref group123)`aida_pva_types.h` (_Common **C header file**_)
    - [➥](@ref group123)`aida_pva_uri.h` (_Common **C header file**_)
- **Group 4**: `AIDASHR` Transfer Vectors
    - [➥](@ref group4)`AIDASHR_XFR_ALPHA.OPT` (_**Transfer vectors**_)
- **Group 5**: `AIDA-PVA` BPM Channel Provider Files (Group 5)
    - [➥](@ref groupconfig)`AIDASLCBPM_CHANNELS.YML` (_**Channel Config**_)
    - [➥](@ref groupnonconfig)`AIDASLCBPM_GENERAL.OPT` (_**Linker options** file_)
    - [➥](@ref groupnonconfig)`AIDASLCBPM_SERVER.c` (_**C source file**_)
    - [➥](@ref groupnonconfig)`AIDASLCBPM_SERVER.h` (_**C header file**_)
    - [➥](@ref groupnonconfig)`AIDASLCBPM_XFR_ALPHA.OPT` (_**Transfer vectors**_)
- **Group 6**: `AIDA-PVA` BPM Buffered Acquisition Channel Provider Files
    - ➥`AIDASLCBPMBUFF_CHANNELS.YML` (_**Channel Config**_)
    - [➥](@ref groupnonconfig)`AIDASLCBPMBUFF_GENERAL.OPT` (_**Linker options** file_)
    - [➥](@ref groupnonconfig)`AIDASLCBPMBUFF_SERVER.c` (_**C source file**_)
    - [➥](@ref groupnonconfig)`AIDASLCBPMBUFF_SERVER.h` (_**C header file**_)
    - [➥](@ref groupnonconfig)`AIDASLCBPMBUFF_XFR_ALPHA.OPT` (_**Transfer vectors**_)
- **Group 7**: `AIDA-PVA` SLC Database Channel Provider Files
    - [➥](@ref groupconfig)`AIDASLCDB_CHANNELS.YAML` (_**Channel Config**_)
    - [➥](@ref groupconfig)`AIDASLCDB_CHANNELS.YML` (Very large _**Channel Config**_)
    - [➥](@ref groupnonconfig)`AIDASLCDB_GENERAL.OPT` (_**Linker options** file_)
    - [➥](@ref groupnonconfig)`AIDASLCDB_SERVER.c` (_**C source file**_)
    - [➥](@ref groupnonconfig)`AIDASLCDB_SERVER.h` (_**C header file**_)
    - [➥](@ref groupnonconfig)`AIDASLCDB_XFR_ALPHA.OPT` (_**Transfer vectors**_)
- **Group 8**: `AIDA-PVA` Klystron Channel Provider Files
    - [➥](@ref groupconfig)`AIDASLCKLYS_CHANNELS.YML` (_**Channel Config**_)
    - [➥](@ref groupnonconfig)`AIDASLCKLYS_GENERAL.OPT` (_**Linker options** file_)
    - [➥](@ref groupnonconfig)`AIDASLCKLYS_SERVER.c` (_**C source file**_)
    - [➥](@ref groupnonconfig)`AIDASLCKLYS_SERVER.h` (_**C header file**_)
    - [➥](@ref groupnonconfig)`AIDASLCKLYS_XFR_ALPHA.OPT` (_**Transfer vectors**_)
- **Group 9**: `AIDA-PVA` Magnet Channel Provider Files
    - [➥](@ref groupconfig)`AIDASLCMAGNET_CHANNELS.YML` (_**Channel Config**_)
    - [➥](@ref groupnonconfig)`AIDASLCMAGNET_GENERAL.OPT` (_**Linker options** file_)
    - [➥](@ref groupnonconfig)`AIDASLCMAGNET_SERVER.c` (_**C source file**_)
    - [➥](@ref groupnonconfig)`AIDASLCMAGNET_SERVER.h` (_**C header file**_)
    - [➥](@ref groupnonconfig)`AIDASLCMAGNET_XFR_ALPHA.OPT` (_**Transfer vectors**_)
- **Group 10**: `AIDA-PVA` Utilities Channel Provider Files
    - [➥](@ref groupconfig)`AIDASLCUTIL_CHANNELS.YML` (_**Channel Config**_)
    - [➥](@ref groupnonconfig)`AIDASLCUTIL_GENERAL.OPT` (_**Linker options** file_)
    - [➥](@ref groupnonconfig)`AIDASLCUTIL_SERVER.c` (_**C source file**_)
    - [➥](@ref groupnonconfig)`AIDASLCUTIL_SERVER.h` (_**C header file**_)
    - [➥](@ref groupnonconfig)`AIDASLCUTIL_XFR_ALPHA.OPT` (_**Transfer vectors**_)

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
- [⚙-7 Build Private AIDASHR](@ref procedure7)
- For each _channel-provider_ in [`SLCBPM`, `SLCBPMBUFF`, `SLCDB`, `SLCKLYS`, `SLCMAGNET`, `SLCUTIL`]
    - [⚙-8 Build Private Channel Provider Shareable Image using Private AIDASHR](@ref procedure8)
- [⚙-9 Build and Test AIDA Providers Privately](@ref procedure9)
- [⚙-10 Commit AIDASHR changes to CMS and Deploy to PROD](@ref procedure10)
- For each _channel-provider_ in [`SLCBPM`, `SLCBPMBUFF`, `SLCDB`, `SLCKLYS`, `SLCMAGNET`, `SLCUTIL`]
    - [⚙-4 Build & Test Channel Provider Shareable in DEV](@ref procedure4)
    - [⚙-5 Move Channel Provider Shareable to PROD](@ref procedure5)
### Groups 5, 6, 7, 8, 9, & 10
#### Channel Config files {#groupconfig}
- [⚙-6 Deploy Channel Config to PROD](@ref procedure6)
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

@note to stop press `CTRL-Y` not `CTRL-C`. Pressing `CTRL-Y` this will prevent an automatic restart

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

### ⚙-6: Deploy Channel Config to PROD {#procedure6}

Deploy appropriate Channel Config file to PROD

<table class="markdownTable">
<tr class="markdownTableHead"><th class="markdownTableHeadNone">step</th><th class="markdownTableHeadNone">sub-step</th><th class="markdownTableHeadNone">procedure</th></tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➀ Reserve changed files in CMS

</td>
<td class="markdownTableBodyNone">

➊ Enter CMS CLI

</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV> slccms
SlcCMS> set libr CMS_SLCTXT
```

</td>
</tr>
<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">
</td>
<td class="markdownTableBodyNone">

➋ 🔂 Reserve changed file

</td>
<td class="markdownTableBodyNone">

```shell
SlcCMS> reserve AIDASLCBPM_CHANNELS.YML /nooutput
SlcCMS> reserve AIDASLCBPMBUFF_CHANNELS.YML /nooutput
SlcCMS> reserve AIDASLCDB_CHANNELS.YML /nooutput
SlcCMS> reserve AIDASLCDB_CHANNELS.YAML /nooutput
SlcCMS> reserve AIDASLCKLYS_CHANNELS.YML /nooutput
SlcCMS> reserve AIDASLCMAGNET_CHANNELS.YML /nooutput
SlcCMS> reserve AIDASLCUTIL_CHANNELS.YML /nooutput
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">
</td>
<td class="markdownTableBodyNone">

➌ Exit CMS CLI

</td>
<td class="markdownTableBodyNone">

```shell
SlcCMS> exit 
```

</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

➁ Commit changed file:

</td>
<td class="markdownTableBodyNone">
➊ 🔂
</td>
<td class="markdownTableBodyNone">

```shell
MCCDEV>  cmp AIDASLCBPM_CHANNELS.YML  
MCCDEV>  cmp AIDASLCBPMBUFF_CHANNELS.YML  
MCCDEV>  cmp AIDASLCDB_CHANNELS.YML  
MCCDEV>  cmp AIDASLCDB_CHANNELS.YAML  
MCCDEV>  cmp AIDASLCKLYS_CHANNELS.YML  
MCCDEV>  cmp AIDASLCMAGNET_CHANNELS.YML  
MCCDEV>  cmp AIDASLCUTIL_CHANNELS.YML  
```

</td>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

➂ Reload Config on DEV if required

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

➃ Reload Config on PROD if required

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

### ⚙-7: Build Private AIDASHR {#procedure7}

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

### ⚙-8: Build Private Channel Provider Shareable Image using Private AIDASHR {#procedure8}

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

@note to stop press `CTRL-Y` not `CTRL-C`. Pressing `CTRL-Y` this will prevent an automatic restart

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

### ⚙-9: Build and Test AIDA Channel Providers Privately {#procedure9}

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

### ⚙-10: Commit AIDASHR changes to CMS and deploy to PROD {#procedure10}

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

