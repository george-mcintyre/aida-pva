# 4 - A DevOps Guide to AIDA-PVA

## Components

AIDA-PVA is made up of the following components:

![AIDA-PVA Files](images/aida-pva-files.png)

### AIDA-PVA SERVICE
* `AIDA-PVA.JAR`
* launches the Channel Providers integrating them into the EPICS PV-Access network
* Built from [AIDA-PVA Github Repo](https://github.com/slaclab/aida-pva) on Linux and copied to SLCLIBS

@see [Building AIDA-PVA Service](3_1_Building_AIDA_PVA_Service.md) for instructions on how to build it.

### EPICS 7 
EPICS 7 that run on VMS (specially ported for AIDA-PVA).  These Jars are not in VMS SLCLIBS.  They exist only in Linux for building AIDA-PVA.JAR
* `EPICS-PVACCESS-bp15.JAR`
* `EPICS-PVDATA-bp15.JAR`
* Built from [EPICS Backport GitHub Repo](https://github.com/slaclab/epics-server-java-backport-1.5) on Linux.

@see [Porting EPICS to Java 1.5](5_0_Porting_EPICS_to_Java_1_5_on_VMS.md) for instructions on how to build it.

### EPICS-7 Forwarder (specially built for AIDA-PVA)
* `EPICS-FORWARDER.JAR`
* Built from [EPICS Backport GitHub Repo](https://github.com/slaclab/epics-server-java-backport-1.5) on Linux and copied to SLCLIBS.

@see [Porting EPICS to Java 1.5](5_0_Porting_EPICS_to_Java_1_5_on_VMS.md) for instructions on how to build it.

### Individual shared libraries for each Channel Provider loaded by the AIDA-PVA SERVICE
* Libraries (CMS Library):
    * `AIDASLCDB.EXE` - (AIDASLCDBLIB)
    * `AIDASLCBPM.EXE` - (AIDASLCBPMLIB)
    * `AIDASLCBPMBUFF.EXE` - (AIDASLCBPMBUFFLIB)
    * `AIDASLCKLYS.EXE` - (AIDASLCKLYSLIB)
    * `AIDASLCMAGNET.EXE` - (AIDASLCMAGNETLIB)
    * `AIDASLCMODEL.EXE` - (AIDASLCMODELLIB)
    * `AIDASLCMOSC.EXE` - (AIDASLCMOSCLIB)
    * `AIDASLCUTIL.EXE` - (AIDASLCUTILLIB)
* Built from CMS but original source code [AIDA-PVA Github repo](https://github.com/slaclab/aida-pva/tree/master/src/cpp/providers)

@see [Building an AIDA-PVA Channel Provider](3_3_Building_AIDA_PVA_Channel_Provider.md) for instructions on how to build one.

### The updated STANDALONELIB with the following modules collectively known as AIDA-PVA Module:
* Modules:
    * `AIDA_PVA_SERVER_HELPER` - **Helper functions for the AIDA-PVA Providers**
    * `AIDA_PVA_JNI_HELPER` - Used by the AIDA-PVA Module to interoperate in a JNI environment
    * `AIDA_PVA_TYPES_HELPER` - Functions that help AIDA-PVA Module marshal and unmarshal JNI types
    * `NATIVECHANNELPROVIDERJNI` - JNI Entry points from AIDA-PVA.JAR
    * `AIDA_PVA_JSON` - Used by AIDA-PVA Module to parse JSON
* Build from CMS STANDALONELIB but original code from [AIDA-PVA Github repo](https://github.com/slaclab/aida-pva/tree/master/src/cpp/aida-pva)

@see [Building AIDA-PVA Service](3_2_Building_AIDA_PVA_into_STANDALONELIB.md) for instructions on how to build it.

### AIDA-PVA Header Files
* Channel Provider header files in C_INC
  * slc_server.h
  * slcBpm_server.h
  * slcBuffAcq_server.h
  * slcKlys_server.h
  * slcMagnet_server.h
  * slcModel_server.h
  * slcMosc_server.h
  * slcUtil_server.h
  * reference_server.h
* AIDA-PVA API header files in C_INC
  * aida_pva.h
  * aida_pva_api.h
  * aida_pva_convert.h 
  * aida_pva_exceptions.h 
  * aida_pva_json.h 
  * aida_pva_memory.h 
  * aida_pva_types.h 
  * aida_pva_uri.h
* AIDA-PVA internal header files in C_INC
  * aida_pva_jni_helper.h
  * aida_pva_server_helper.h
  * aida_pva_types_helper.h
  * slac_aida_NativeChannelProvider.h
* Maintained in CMS in C_INC but original code from [AIDA-PVA github repo](https://github.com/slaclab/aida-pva/tree/master/src/cpp)

## Running AIDA-PVA

![Running AIDA-PVA](images/aida-pva-system-components-wa.png)

1. **Run the Forwarder**
   * It is imperative that the Forwarder is started before any of the provider services. 
   * If the Forwarder dies then all the other services must be shutdown and only restarted after starting the forwarder. This is because of a port contention that exists if the Forwarder finds any other provider service running when it starts up.
   * startup with the following command:
```shell
MCCDEV> java -jar SLCLIBS:EPICS-FORWARDER.JAR
Oct 24, 2021 2:35:33 AM org.epics.forwarder.PVAForwarder main
INFO: EPICS Request Forwarder started: 2344 milliseconds
9:35 > 
```
   * You can also run it using `java -jar /SLCLIBS/EPICS-FORWARDER.JAR`
2. **Run the AIDA-PVA SERVICE for each Channel Provider**
   * e.g. 
```shell
java -jar "-Daida.pva.channels.filename=channels.yaml" "-Djava.library.path=/SLCLIBS" "-Daida.pva.lib.name=AIDASLCDB" SLCLIBS:AIDA-PVA.JAR
Oct 24, 2021 12:58:50 PM edu.stanford.slac.aida.impl.AidaService <clinit>
INFO: Loading Channel Provider Shared Library: AIDASLCDB

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
Oct 24, 2021 12:58:51 PM edu.stanford.slac.aida.lib.ChannelProviderFactory create
INFO: Loading channel configuration from: channels.yaml
Oct 24, 2021 12:59:02 PM edu.stanford.slac.aida.lib.ChannelProvider logHostedChannels
INFO:
AIDA-pva Channel Provider : SLC
Oct 24, 2021 12:59:02 PM edu.stanford.slac.aida.lib.ChannelProvider logHostedChannels
INFO: Channels hosted:
  [LEMG:????:*//EEND, RADS:????:*//MTIM, WIRE:????:*:SK2P, FDBK:????:*//CDEL, SOLN:????:*//BMAX, PTGM:????:*:RSOF, MPSI:????:*:CGID, FDBK:????:*:ULBL,
LENS:????:*//DVIC, ...]
```
3. **AIDA-PVA SERVICE is linked with EPICS BACKPORT jars** prior to installing in SLCLIBS
4. **AIDA-PVA SERVICE loads the Channel Provider**
   * _Selection of Provider Service_
     * Channel Provider shared library location selection:
       1. A property set on the launch commandline with the `-D` option named `java.library.path`
           * fully qualified path name or
           * relative path
           * e.g. `-Djava.library.path=/SLCLIBS`
       2. * By default, the standard library locations and working directory are searched
     * Channel Provider shared library name selection:
       1. An Environment Variable `AIDA_PVA_LIB_NAME`
           * fully qualified path name of library (without the .EXE) or
           * library name (without the .EXE) to search in the working directory and standard library locations
           * e.g. `AIDA_PVA_LIB_NAME :== AIDASLCDB` 
       2. A property set on the launch commandline with the `-D` option named `aida.pva.lib.name`
           * fully qualified path name (without the .EXE) of library or
           * library name (without the .EXE) which will search in the working directory and standard library locations
           * e.g. `-Daida.pva.lib.name=AIDASLCDB`
       3. A file called `AIDA.EXE`
       4. Note that the file extension is assumed to be `.EXE` and is always omitted.
   * _Specifying the Supported Channels_.  [Channels YAML file](2_3_CHANNELS_YML_file) selection: 
     1. An Environment Variable `AIDA_PVA_CHANNELS_FILENAME`
         * fully qualified path name of channels file or
         * channels file name to search for in the working directory
         * e.g. `AIDA_PVA_CHANNELS_FILENAME :== SLCTXT:AIDASLCDB_CHANNELS.YML`
     2. A property set on the launch commandline with the `-D` option named `aida.pva.channels.filename`
         * fully qualified path name of channels file or
         * channels file name to search in the working directory
         * e.g. `-Daida.pva.channels.filename=/SLCTXT/AIDASLCDB_CHANNELS.YML`
         * A file in the working directory called `CHANNELS.YML`
5. The Channel Provider will load Modules in AIDASHR
6. The Channel Provider will also load Modules in STANDALONELIB

## Submitting jobs

## Restarting

## Verification of service status

