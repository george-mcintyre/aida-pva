# Deploying AIDA-PVA - A DevOps Guide
## Overview 
AIDA-PVA is made up of the following components:
* AIDA-PVA core 
  * AIDA-PVA.JAR
    * launches the service providers integrating them into the EPICS PV-Access network
* EPICS 7 that run on VMS (specially ported for AIDA-PVA)
  * EPICS-PVACCESS-bp15.JAR 
  * EPICS-PVDATA-bp15.JAR
* EPICS-7 Forwarder (specially built for AIDA-PVA)
  * EPICS-FORWARDER.jar
* Individual shared libraries for each provider service loaded by AIDA-PVA jar
  * SLCAPVA-DB.EXE
  * SLCAPVA-BPM.EXE
  * SLCAPVA-BUFFACQ.EXE
  * SLCAPVA-KLYSTRON.EXE
  * SLCAPVA-MAGNET.EXE
  * SLCAPVA-MODEL.EXE
  * SLCAPVA-MOSC.EXE
  * SLCAPVA-UTIL.EXE
* The updated Aida Shared Library
  * AIDASHR.EXE


# Running
It is imperative that the Forwarder is started before any of the provider services.  If the Forwarder dies then all of the other services must be shutdown and only restarted after starting the forwarder.  This is because of a port contention that exists if the Forwarder finds any other provider service running when it starts up.

# Running a Provider Service
* Selection of Provider Service
  * The AIDA-PVA jar loads the service provider shared library based on the following rules:
    * An Environment Variable `AIDA_PVA_LIB_NAME` 
      * fully qualified path name of library or 
      * library name to search in the working directory and standard library locations  
    * A property set on the launch commandline with the `-D` option named `aida.pva.lib.name`
      * fully qualified path name of library or 
      * library name which will search in the working directory and standard library locations  
    * A file in the working directory called `AIDA.EXE`
  * Note that the file extension is assumed to be `.EXE` if it is omitted

  * The AIDA-PVA jar loads the service provider shared library from based on:
    * A property set on the launch commandline with the `-D` option named `java.library.path`
      * fully qualified path name or 
      * relative path 
    * By default the standard library locations or working directory

* Specifying the channels the provider will support
  * The channels are configured in a yaml file.  The default name for the file is `CHANNELS.YML`
  * The AIDA-PVA jar loads the service provider's channels definition file based on the following rules:
    * An Environment Variable `AIDA_PVA_CHANNELS_FILENAME` 
      * fully qualified path name of channels file or 
      * channels file name to search in the working directory  
    * A property set on the launch commandline with the `-D` option named `aida.pva.channels.filename`
      * fully qualified path name of channels file or 
      * channels file name to search in the working directory  
    * A file in the working directory called `CHANNELS.YML`

## Example

```shell
MCCDEV> lib
Default:= DATA_DISK_SLC:[DEV.AIDA-PVA.LIB]
MCCDEV> java -jar "-Djava.library.path=./" "-Daida.pva.lib.name=SLCAPVA-BPM" AIDA-PVA.JAR
Aida BPM Service Initialised

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

AIDA-pva Channel Provider : SLCBpm
Channels hosted:
  [????????//BPMS, ???????:BPMS, ???????//BPMS, ????????:BPMS]

```


There are no health checks so in order to see if a service is running you'll have to check with the operating system.

# Submitting jobs
# Restarting
# Verification of service status

