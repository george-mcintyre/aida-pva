# 3.2 - Building AIDA-PVA into AIDA_PVALIB

Whenever you make changes to any of the following files you need to rebuild `AIDA_PVALIB` and deploy all shareable
images that
are affected by your change. First you need to verify that the code runs **before** committing it to CMS.

## Files in AIDA_PVALIB

If any of these files change follow the procedures below to rebuild and deploy `AIDA_PVALIB` dependent shareables.

- AIDA-PVA Module Header Files (Group 1)
    - `aida_pva_jni_helper.h` (_**C header file** for AIDA-PVA Module implementation_)
    - `aida_pva_server_helper.h` (_**C header file** for AIDA-PVA Module implementation_)
    - `aida_pva_types_helper.h` (_**C header file** for AIDA-PVA Module implementation_)
    - `slac_aida_NativeChannelProvider.h` (_**C header file** for AIDA-PVA Module implementation_)
- AIDA-PVA Module source files (Group 2)
    - `aida_pva_json.c` (_**C source file** for AIDA-PVA Module implementation_)
    - `aida_pva_server_helper.c` (_**C source file** for AIDA-PVA Module implementation_)
    - `aida_pva_types_helper.c` (_**C source file** for AIDA-PVA Module implementation_)
    - `NativeChannelProviderJni.c` (_**C source file** for AIDA-PVA Module implementation_)
- AIDA-PVA Common Header Files (Group 3)
    - `aida_pva.h` (_Common **C header file** for AIDA-PVA Module and provider implementation_)
    - `aida_pva_api.h` (_Common **C header file** for AIDA-PVA Module and provider implementation_)
    - `aida_pva_convert.h` (_Common **C header file** for AIDA-PVA Module and provider implementation_)
    - `aida_pva_exceptions.h` (_Common **C header file** for AIDA-PVA Module and provider implementation_)
    - `aida_pva_json.h` (_Common **C header file** for AIDA-PVA Module and provider implementation_)
    - `aida_pva_memory.h` (_Common **C header file** for AIDA-PVA Module and provider implementation_)
    - `aida_pva_types.h` (_Common **C header file** for AIDA-PVA Module and provider implementation_)
    - `aida_pva_uri.h` (_Common **C header file** for AIDA-PVA Module and provider implementation_)
- AIDA-PVA Provider Common Files (Group 4)
    - `AIDASHR_XFR_ALPHA.OPT` (_**Transfer vectors** for `AIDASHR` library_)
- AIDA-PVA BPM Provider Files (Group 5)
    - `AIDASLCBPM_CHANNELS.YML` (_Yaml **Channel Config** File_)
    - `AIDASLCBPM_GENERAL.OPT` (_**Linker options** file_)
    - `AIDASLCBPM_SERVER.c` (_**C source file** for this provider implementation_)
    - `AIDASLCBPM_SERVER.h` (_**C header file** for this provider implementation_)
    - `AIDASLCBPM_XFR_ALPHA.OPT` (_**Transfer vectors** for use in `AIDA_PVALIB`_)
- AIDA-PVA BPM Buffered Acquisition Provider Files (Group 6)
    - `AIDASLCBPMBUFF_CHANNELS.YML` (_Yaml **Channel Config** File_)
    - `AIDASLCBPMBUFF_GENERAL.OPT` (_**Linker options** file_)
    - `AIDASLCBPMBUFF_SERVER.c` (_**C source file** for this provider implementation_)
    - `AIDASLCBPMBUFF_SERVER.h` (_**C header file** for this provider implementation_)
    - `AIDASLCBPMBUFF_XFR_ALPHA.OPT` (_**Transfer vectors** for use in `AIDA_PVALIB`_)
- AIDA-PVA SLC Database Provider Files (Group 7)
    - `AIDASLCDB_CHANNELS.YAML` (_Yaml **Channel Config** File_)
    - `AIDASLCDB_CHANNELS.YML` (Very large _Yaml **Channel Config** File_)
    - `AIDASLCDB_GENERAL.OPT` (_**Linker options** file_)
    - `AIDASLCDB_SERVER.c` (_**C source file** for this provider implementation_)
    - `AIDASLCDB_SERVER.h` (_**C header file** for this provider implementation_)
    - `AIDASLCDB_XFR_ALPHA.OPT` (_**Transfer vectors** for use in `AIDA_PVALIB`_)
- AIDA-PVA Klystron Provider Files (Group 8)
    - `AIDASLCKLYS_CHANNELS.YML` (_Yaml **Channel Config** File_)
    - `AIDASLCKLYS_GENERAL.OPT` (_**Linker options** file_)
    - `AIDASLCKLYS_SERVER.c` (_**C source file** for this provider implementation_)
    - `AIDASLCKLYS_SERVER.h` (_**C header file** for this provider implementation_)
    - `AIDASLCKLYS_XFR_ALPHA.OPT` (_**Transfer vectors** for use in `AIDA_PVALIB`_)
- AIDA-PVA Magnet Provider Files (Group 9)
    - `AIDASLCMAGNET_CHANNELS.YML` (_Yaml **Channel Config** File_)
    - `AIDASLCMAGNET_GENERAL.OPT` (_**Linker options** file_)
    - `AIDASLCMAGNET_SERVER.c` (_**C source file** for this provider implementation_)
    - `AIDASLCMAGNET_SERVER.h` (_**C header file** for this provider implementation_)
    - `AIDASLCMAGNET_XFR_ALPHA.OPT` (_**Transfer vectors** for use in `AIDA_PVALIB`_)
- AIDA-PVA Utilities Provider Files (Group 10)
    - `AIDASLCUTIL_CHANNELS.YML` (_Yaml **Channel Config** File_)
    - `AIDASLCUTIL_GENERAL.OPT` (_**Linker options** file_)
    - `AIDASLCUTIL_SERVER.c` (_**C source file** for this provider implementation_)
    - `AIDASLCUTIL_SERVER.h` (_**C header file** for this provider implementation_)
    - `AIDASLCUTIL_XFR_ALPHA.OPT` (_**Transfer vectors** for use in `AIDA_PVALIB`_)

@note These files are also stored in the SLAC git repository for use with modern IDEs via git. The master version is in
CMS.

```shell
git clone  git@github.com:slaclab/aida-pva.git
```

### Graphical representation of files in APDA-PVA

![AIDA-PVA files graphical representation](images/aida-pva-files.png)

## Preparation

### Verify Source Code has correct CARDS

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

### Copy changed files to a clean directory in VMS

To build a new version for test and subsequent deployment you need to copy changed
files to a clean directory on MCCDEV. The build tools will combine the changed files with the
remaining files that are in CMS to build the libraries and shared libraries required.

## Procedures for Building, Testing and Deploying

- Groups 1, 2, & 3
    1. For each _channel-provider_ in [`SLCBPM`, `SLCBPMBUFF`, `SLCDB`, `SLCKLYS`, `SLCMAGNET`, `SLCUTIL`]
        1. **BUILD PRIVATE CHANNEL PROVIDER MODULE**
        2. **BUILD PRIVATE CHANNEL PROVIDER SHAREABLE**
    2. For each _channel-provider_ in [`SLCBPM`, `SLCBPMBUFF`, `SLCDB`, `SLCKLYS`, `SLCMAGNET`, `SLCUTIL`]
        1. **COMMIT & BUILD CHANNEL PROVIDER MODULE**
        2. **BUILD & TEST CHANNEL PROVIDER SHAREABLE IN DEV**
        3. **MOVE CHANNEL PROVIDER SHAREABLE TO PROD**
- Group 4
    1. **BUILD PRIVATE AIDASHR**
    2. For each _channel-provider_ in [`SLCBPM`, `SLCBPMBUFF`, `SLCDB`, `SLCKLYS`, `SLCMAGNET`, `SLCUTIL`]
        1. **BUILD PRIVATE CHANNEL PROVIDER SHAREABLE USING PRIVATE AIDASHR**
    3. **BUILD AND TEST AIDA PROVIDERS PRIVATELY**
    4. **COMMIT AIDASHR CHANGES TO CMS**
    5. For each _channel-provider_ in [`SLCBPM`, `SLCBPMBUFF`, `SLCDB`, `SLCKLYS`, `SLCMAGNET`, `SLCUTIL`]
        1. **BUILD & TEST CHANNEL PROVIDER SHAREABLE IN DEV**
        2. **MOVE CHANNEL PROVIDER SHAREABLE TO PROD**
- Groups 5, 6, 7, 8, 9, & 10
    - _Channel Config files_
        1. **DEPLOY CHANNEL CONFIG TO PROD**
    - All files except _Channel Config files_
        1. **BUILD PRIVATE CHANNEL PROVIDER MODULE**
        2. **BUILD PRIVATE CHANNEL PROVIDER SHAREABLE**
        3. **COMMIT & BUILD CHANNEL PROVIDER MODULE**
        4. **BUILD & TEST CHANNEL PROVIDER SHAREABLE IN DEV**
        5. **MOVE CHANNEL PROVIDER SHAREABLE TO PROD**

## Channel Provider Build Procedures

### BUILD PRIVATE CHANNEL PROVIDER MODULE

Build an `AIDA-PVA` channel provider module in a private object library

1. Create DEV library

    ```shell
    MCCDEV> LIBRARY /CREATE DEVLIB
    ```

2. Compile all changed C files including local header files

    ```shell
    MCCDEV> CINC *.C
    ```

### BUILD PRIVATE CHANNEL PROVIDER SHAREABLE

Build and Test an `AIDA-PVA` channel provider shareable image privately

1. Build shareable image
    1. Edit `GENERAL.OPT` file (either checkout or change updated) to comment out line ending
       in `! copy to DEV direct and remove this line for BUILDTEST /DEF`
    2. Build by running one of the following commands:
       ```shell
       MCCDEV> BUILDTEST AIDASLCBPM /ALL /DEFAULT
       MCCDEV> BUILDTEST AIDASLCBPMBUFF /ALL /DEFAULT
       MCCDEV> BUILDTEST AIDASLCDB /ALL /DEFAULT
       MCCDEV> BUILDTEST AIDASLCKLYS /ALL /DEFAULT
       MCCDEV> BUILDTEST AIDSLCMAGNET /ALL /DEFAULT
       MCCDEV> BUILDTEST AIDASLCUTIL /ALL /DEFAULT
       ```

   @note `/DEFAULT` means use the files in the local directory

2. Test shareable image
    1. Log into dev machine using `SLCSHR` account
       ```shell
       MCCDEV> ssh MCCDEV /user=slcshr
       ```
    2. Shutdown running provider on dev, one of:
        ```shell
        MCCDEV::SLCSHR> WARMSLC AIDA_SLCBPM /KILL
        MCCDEV::SLCSHR> WARMSLC AIDA_SLCBPMBUFF /KILL
        MCCDEV::SLCSHR> WARMSLC AIDA_SLCDB /KILL
        MCCDEV::SLCSHR> WARMSLC AIDA_SLCKLYS /KILL
        MCCDEV::SLCSHR> WARMSLC AIDS_LCMAGNET /KILL
        MCCDEV::SLCSHR> WARMSLC AIDA_SLCUTIL /KILL
        ```
    3. Log out of `SLCSHR` account
       ```shell
       MCCDEV> logout
       ```

    4. Run private shareable, one of:
        ```shell
        MCCDEV> @AIDA_ASLCBPM.SUBMIT
        MCCDEV> @AIDA_ASLCBPMBUFF.SUBMIT
        MCCDEV> @AIDA_ASLCDB.SUBMIT
        MCCDEV> @AIDA_ASLCKLYS.SUBMIT
        MCCDEV> @AIDA_SLCMAGNET.SUBMIT
        MCCDEV> @AIDA_ASLCUTIL.SUBMIT
        ```

### COMMIT & BUILD CHANNEL PROVIDER MODULE

Commit and Rebuild an `AIDA-PVA` channel provider module in the `AIDA_PVALIB` object library

1. Build and Test locally first (see above)
2. Reserve changed files in CMS
    1. Enter CMS cli
       ```shell
       MCCDEV> slccms
       ```
    2. Select library based on type of changed file, one of:
       ```shell
       SlcCMS> set libr CMS_AIDA_PVA
       SlcCMS> set libr CMS_C_INC
       SlcCMS> set libr CMS_SLCSHR_CONTROL  
       SlcCMS> set libr CMS_SLCTXT  
       ```
    3. Reserve changed file, one of:
       ```shell
       SlcCMS> reserve AIDAASLCBPM_SERVER.C /nooutput 
       SlcCMS> reserve AIDAASLCBPM_SERVER.h /nooutput 
       SlcCMS> reserve AIDAASLCBPM_GENERAL.OPT /nooutput 
     
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

    4. Exit CMS cli
       ```shell
       SlcCMS> exit 
       ```

3. Commit changed files, one of:

   ```shell
   MCCDEV>  cmp AIDAASLCBPM_SERVER.C  
   MCCDEV>  cmp AIDAASLCBPM_SERVER.h  
   MCCDEV>  cmp AIDAASLCBPM_GENERAL.OPT  
   
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

### BUILD & TEST CHANNEL PROVIDER SHAREABLE IN DEV

Rebuild and Test an `AIDA-PVA` channel provider shareable image in `DEV`

1. Must have committed changes to CMS prior to executing this step
2. Rebuild a new shareable on DEV
    1. Log into dev machine using `SLCSHR` account
       ```shell
       MCCDEV> ssh MCCDEV /user=slcshr
       ```

    2. Build new shareable, one of:
       ```shell
       MCCDEV::SLCSHR> BUILDSHR AIDASLCBPMBUFF
       MCCDEV::SLCSHR> BUILDSHR AIDASLCDB
       MCCDEV::SLCSHR> BUILDSHR AIDASLCKLYS
       MCCDEV::SLCSHR> BUILDSHR AIDSLCMAGNET
       MCCDEV::SLCSHR> BUILDSHR AIDASLCUTIL
       ```

    3. Present share on DEV server, one of:
       ```shell
       MCCDEV::SLCSHR> DEVSHR AIDASLCBPMBUFF
       MCCDEV::SLCSHR> DEVSHR AIDASLCDB
       MCCDEV::SLCSHR> DEVSHR AIDASLCKLYS
       MCCDEV::SLCSHR> DEVSHR AIDSLCMAGNET
       MCCDEV::SLCSHR> DEVSHR AIDASLCUTIL
       ```
    4. Start new provider on dev, one of:
       ```shell
       MCCDEV::SLCSHR> WARMSLC AIDASLCBPM /RESTART
       MCCDEV::SLCSHR> WARMSLC AIDASLCBPMBUFF /RESTART
       MCCDEV::SLCSHR> WARMSLC AIDASLCDB /RESTART
       MCCDEV::SLCSHR> WARMSLC AIDASLCKLYS /RESTART
       MCCDEV::SLCSHR> WARMSLC AIDSLCMAGNET /RESTART
       MCCDEV::SLCSHR> WARMSLC AIDASLCUTIL /RESTART
        ```
    5. Log out of `SLCSHR` account
       ```shell
       MCCDEV::SLCSHR> logout
       ```
    6. Leave and test for a number of days

### MOVE CHANNEL PROVIDER SHAREABLE TO PROD

Move an `AIDA-PVA` channel provider shareable image to `PROD`

1. Log into dev machine using `SLCSHR` account

    ```shell
    MCCDEV> ssh MCCDEV /user=slcshr
    ```

2. Copy DEV share to PROD, one of:

    ```shell
    MCCDEV::SLCSHR> NEWSHR AIDASLCBPMBUFF
    MCCDEV::SLCSHR> NEWSHR AIDASLCDB
    MCCDEV::SLCSHR> NEWSHR AIDASLCKLYS
    MCCDEV::SLCSHR> NEWSHR AIDSLCMAGNET
    MCCDEV::SLCSHR> NEWSHR AIDASLCUTIL
    ```

3. Start new provider on dev, one of:

   ```shell
   MCCDEV::SLCSHR> WARMSLC AIDA_SLCBPM /RESTART
   MCCDEV::SLCSHR> WARMSLC AIDA_SLCBPMBUFF /RESTART
   MCCDEV::SLCSHR> WARMSLC AIDA_SLCDB /RESTART
   MCCDEV::SLCSHR> WARMSLC AIDA_SLCKLYS /RESTART
   MCCDEV::SLCSHR> WARMSLC AIDS_LCMAGNET /RESTART
   MCCDEV::SLCSHR> WARMSLC AIDA_SLCUTIL /RESTART
   ```

4. Log into prod machine using `SLCSHR` account

   ```shell
   MCCDEV::SLCSHR> ssh MCC /user=slcshr
   ```

5. Start new provider on prod, one of:

   ```shell
   MCC::SLCSHR> WARMSLC AIDASLCBPM /RESTART
   MCC::SLCSHR> WARMSLC AIDASLCBPMBUFF /RESTART
   MCC::SLCSHR> WARMSLC AIDASLCDB /RESTART
   MCC::SLCSHR> WARMSLC AIDASLCKLYS /RESTART
   MCC::SLCSHR> WARMSLC AIDSLCMAGNET /RESTART
   MCC::SLCSHR> WARMSLC AIDASLCUTIL /RESTART
   ```

6. Log out of `SLCSHR` account on prod

   ```shell
   MCC::SLCSHR> logout
   ```

7. Log out of `SLCSHR` account on dev

   ```shell
   MCCDEV::SLCSHR> logout
   ```

### DEPLOY CHANNEL CONFIG TO PROD

Deploy appropriate Channel Config file to PROD

1. Reserve changed files in CMS
    1. Enter CMS cli
       ```shell
       MCCDEV> slccms
       SlcCMS> set libr CMS_SLCTXT  
       ```
    2. Reserve changed file, one of:
       ```shell
       SlcCMS> reserve AIDAASLCBPM_CHANNELS.YML /nooutput 
       SlcCMS> reserve AIDASLCBPMBUFF_CHANNELS.YML /nooutput 
       SlcCMS> reserve AIDASLCDB_CHANNELS.YML /nooutput 
       SlcCMS> reserve AIDASLCDB_CHANNELS.YAML /nooutput 
       SlcCMS> reserve AIDASLCKLYS_CHANNELS.YML /nooutput 
       SlcCMS> reserve AIDASLCMAGNET_CHANNELS.YML /nooutput 
       SlcCMS> reserve AIDASLCUTIL_CHANNELS.YML /nooutput 
       ```
    3. Exit CMS cli
       ```shell
       SlcCMS> exit 
       ```
2. Commit changed file, one of:

    ```shell
    MCCDEV>  cmp AIDAASLCBPM_CHANNELS.YML  
    MCCDEV>  cmp AIDASLCBPMBUFF_CHANNELS.YML  
    MCCDEV>  cmp AIDASLCDB_CHANNELS.YML  
    MCCDEV>  cmp AIDASLCDB_CHANNELS.YAML  
    MCCDEV>  cmp AIDASLCKLYS_CHANNELS.YML  
    MCCDEV>  cmp AIDASLCMAGNET_CHANNELS.YML  
    MCCDEV>  cmp AIDASLCUTIL_CHANNELS.YML  
    ```

## AIDASHR Specific Build Procedures

### BUILD PRIVATE AIDASHR

Build `AIDASHR` shared library privately

1. See [legacy AIDA documentation](https://www.slac.stanford.edu/grp/cd/soft/aida/) for instructions

### BUILD PRIVATE CHANNEL PROVIDER SHAREABLE USING PRIVATE AIDASHR

Build and Test an `AIDA-PVA` channel provider shareable image privately using private `AIDASHR` library

1. Build shareable image
    1. Edit `GENERAL.OPT` file (checkout first) to comment out line ending
       in `! copy to DEV direct and remove this line for BUILDTEST /DEF`
    2. Build by running one of the following commands:
        ```shell
        MCCDEV> BUILDTEST AIDASLCBPM /ALL /DEFAULT
        MCCDEV> BUILDTEST AIDASLCBPMBUFF /ALL /DEFAULT
        MCCDEV> BUILDTEST AIDASLCDB /ALL /DEFAULT
        MCCDEV> BUILDTEST AIDASLCKLYS /ALL /DEFAULT
        MCCDEV> BUILDTEST AIDSLCMAGNET /ALL /DEFAULT
        MCCDEV> BUILDTEST AIDASLCUTIL /ALL /DEFAULT
        ```
       @note `/DEFAULT` means use the files in the local directory

2. Test shareable image
    1. Log into dev machine using `SLCSHR` account
       ```shell
       MCCDEV> ssh MCCDEV /user=slcshr
       ```
    2. Shutdown running provider on dev, one of:
       ```shell
       MCCDEV::SLCSHR> WARMSLC AIDASLCBPM /KILL
       MCCDEV::SLCSHR> WARMSLC AIDASLCBPMBUFF /KILL
       MCCDEV::SLCSHR> WARMSLC AIDASLCDB /KILL
       MCCDEV::SLCSHR> WARMSLC AIDASLCKLYS /KILL
       MCCDEV::SLCSHR> WARMSLC AIDSLCMAGNET /KILL
       MCCDEV::SLCSHR> WARMSLC AIDASLCUTIL /KILL
       ```
    3. Log out of `SLCSHR` account
       ```shell
       MCCDEV> logout
       ```
    4. Run private shareable, one of:
       ```shell
       MCCDEV> @AIDA_ASLCBPM.SUBMIT
       MCCDEV> @AIDA_ASLCBPMBUFF.SUBMIT
       MCCDEV> @AIDA_ASLCDB.SUBMIT
       MCCDEV> @AIDA_ASLCKLYS.SUBMIT
       MCCDEV> @AIDA_SLCMAGNET.SUBMIT
       MCCDEV> @AIDA_ASLCUTIL.SUBMIT
       ```

### BUILD AND TEST AIDA PROVIDERS PRIVATELY

Build and Test all `AIDA` providers privately

1. See [legacy AIDA documentation](https://www.slac.stanford.edu/grp/cd/soft/aida/) for instructions

### COMMIT AIDASHR CHANGES TO CMS

Commit changes to `AIDASHR` shared library

1. See [legacy AIDA documentation](https://www.slac.stanford.edu/grp/cd/soft/aida/) for instructions

