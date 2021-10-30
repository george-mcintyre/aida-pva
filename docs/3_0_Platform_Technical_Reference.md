# 3 - Platform Technical Reference

Guide to the implementation of AIDA-PVA in Stanford National Laboratory.

## EPICS Backport 1.5
In order to make AIDA-PVA work on VMS EPICS had to be backported to Java 1.5. This created
the following libraries that were used to build the AIDA-PVA SERVICE Jar and run the EPICS framework on VMS.
- EPICS-PVACCESS-bp15.JAR - Backported version of PVACCESS.JAR.
- EPICS-PVDATA-bp15.JAR - Backported version of PVDATA.JAR.
- EPICS-FORWARDER.JAR - New component of EPICS required for the framework on VMS.

@see [Porting EPICS to Java 1.5 on VMS](5_0_Porting_EPICS_to_Java_1_5_on_VMS.md) for more information

## AIDA-PVA SERVICE
The AIDA-PVA Service is a [shaded jar](https://stackoverflow.com/questions/49810578/what-is-a-shaded-jar-and-what-is-the-difference-similarities-between-uber-jar-a) 
that implements the AIDA-PVA framework.   It is therefore a self-contained executable JAR that when run becomes the Channel Provider Process.   

The AIDA-PVA service uses a plug-and-play architecture for the implementation of the channel provider.  It is configured as follows:
- It checks the value of a property (`java.library.path`) to determine which
path to load the Channel Provider library from.  In our SLAC environment this is always `/SLCLIBS`.
- It checks the value of a property (`aida.pva.lib.name`) or environment variable (`AIDA_PVA_LIB_NAME`) to determine which
Channel Provider library to load.  e.g. `AIDASLCDB` to load `AIDASLCDB.EXE` library.
- It checks the value of a property (`aida.pva.channels.filename`) or environment variable (`AIDA_PVA_CHANNELS_FILENAME`) to determine which
 channel configuration file to load. This can be a full path name.  e.g. `/SLCTXT/AIDASLCDB_CHANNELS.YML`
- It reads configuration from the selected channel configuration file.

This determines which channels the Channel Provider will support, and where it will find implementations for the endpoints required to service the supported channels. 
In this way the same JAR can become any Channel Provider simply by modifying these configuration parameters.

The source code is found in the github repository [slaclab/aida-pva](https://github.com/slaclab/aida-pva)

### Source Code Explanation
- aida-pva
  - edu.stanford.slac
    - aida
      - impl
        - @ref edu.stanford.slac.aida.impl.AidaChannelProvider "AidaChannelProvider" 
          - _Subclass of_ @ref edu.stanford.slac.aida.lib.ChannelProvider "ChannelProvider"
        - @ref edu.stanford.slac.aida.impl.AidaService "AidaService" 
          - => **The main() Entry point for the AIDA-PVA Service**
          - _Start a timer to measure uptime. Can be obtained at any time by calling static function elapsedTime()_ 
          - _Loads the Channel Provider Library_
          - _Creates a new_ @ref edu.stanford.slac.aida.lib.ChannelProvider "ChannelProvider" _Object_
          - _Run it with_ @ref edu.stanford.slac.aida.lib.AidaProviderRunner "AidaProviderRunner.run()"
      - lib
        - model - _classes representing various platform objects.
          - @ref edu.stanford.slac.aida.lib.model.AidaArgument "AidaArgument"
          - @ref edu.stanford.slac.aida.lib.model.AidaArguments "AidaArguments"
          - @ref edu.stanford.slac.aida.lib.model.AidaChannel "AidaChannel"
          - @ref edu.stanford.slac.aida.lib.model.AidaChannelOperationConfig "AidaChannelOperationConfig"
          - @ref edu.stanford.slac.aida.lib.model.AidaConfigGroup "AidaConfigGroup"
          - @ref edu.stanford.slac.aida.lib.model.AidaField "AidaField"
          - @ref edu.stanford.slac.aida.lib.model.AidaProvider "AidaProvider"
          - @ref edu.stanford.slac.aida.lib.model.AidaTable "AidaTable"
          - @ref edu.stanford.slac.aida.lib.model.AidaType "AidaType"
          - @ref edu.stanford.slac.aida.lib.model.DoubleArgument "DoubleArgument"
          - @ref edu.stanford.slac.aida.lib.model.FloatArgument "FloatArgument"
        - util - _various utilities_
          - @ref edu.stanford.slac.aida.lib.util.AidaPva "AidaPva"
            - Prints AIDA-PVA banner on console
          - @ref edu.stanford.slac.aida.lib.util.AidaPVHelper "AidaPVHelper"
            - _asScalar() - Convert given List of scalar objects to PVStructure conforming to PVScalarArray_
            - _asScalarArray() - Convert given scalar object to PVStructure conforming to PVScalar_
            - _asNtTable() - Convert List of Lists to PVStructure conforming to NTTable_
            - _conversion functions from PVField, PVArray, and PVStructure to String, List of Strings, and Json String respectively, extracting out any Floats and Doubles to be sent in ieee format._
          - @ref edu.stanford.slac.aida.lib.util.AidaStringUtils "AidaStringUtils"
            - _boring string manipulation functions_
        - @ref edu.stanford.slac.aida.lib.AidaProviderRunner "AidaProviderRunner"
          - One method run().
            - _Create a new_ @ref edu.stanford.slac.aida.lib.AidaRPCServer "AidaRPCServer"
            - _Create a new_ @ref edu.stanford.slac.aida.lib.AidaRPCService "AidaRPCService" _to service requests coming from the EPICS network_
            - _Register all the configured channels_
            - _Start the_ @ref edu.stanford.slac.aida.lib.AidaRPCServer "AidaRPCServer" _with run() method_.
        - @ref edu.stanford.slac.aida.lib.AidaRPCServer "AidaRPCServer"
          - _Subclass of EPICS RPCServer class which provides run() function to start server_
        - @ref edu.stanford.slac.aida.lib.AidaRPCService "AidaRPCService" 
          - _Subclass of EPICS RPCService class_
          - _implements request() which is called whenever the service is called by EPICS_.
        - @ref edu.stanford.slac.aida.lib.ChannelProvider "ChannelProvider"
          - _Subclass of_ @ref slac.aida.NativeChannelProvider "NativeChannelProvider"
          - _On creation_ 
            - _prints banner using_ @ref edu.stanford.slac.aida.lib.util.AidaPva "AidaPva.banner()"
            - _Create a_ @ref edu.stanford.slac.aida.lib.model.AidaProvider "AidaProvider" _using_ @ref edu.stanford.slac.aida.lib.ChannelProviderFactory "ChannelProviderFactory.create()" _which will load channels from configuration file._
            - _log the hosted channels to the console_
            - _call the aidaServiceInit() in superclass_ @ref slac.aida.NativeChannelProvider "NativeChannelProvider" _which will call the Native Channel Provider via JNI._
          - _Provides requestScalar(), requestScalarArray(), setValue(), setValueWithScalar() and requestTable() methods to handle all requests and pass them on to the correct_ @ref slac.aida.NativeChannelProvider "NativeChannelProvider" _endpoint for forwarding to the Native Channel Provider via JNI._
        - @ref edu.stanford.slac.aida.lib.ChannelProviderFactory "ChannelProviderFactory"
          - _One method called create()_
            - _Determines where to load the channel configuration from_
            - _Loads channel configuration from the channel configuration file_
    - except - _exception classes_
      - @ref edu.stanford.slac.except.AidaInternalException "AidaInternalException"
      - @ref edu.stanford.slac.except.MissingRequiredArgumentException "MissingRequiredArgumentException"
      - @ref edu.stanford.slac.except.ServerInitialisationException "ServerInitialisationException"
      - @ref edu.stanford.slac.except.UnableToGetDataException "UnableToGetDataException"
      - @ref edu.stanford.slac.except.UnableToSetDataException "UnableToSetDataException"
      - @ref edu.stanford.slac.except.UnsupportedChannelException "UnsupportedChannelException"
      - @ref edu.stanford.slac.except.UnsupportedChannelTypeException "UnsupportedChannelTypeException"
- slac.aida
  - @ref slac.aida.NativeChannelProvider "NativeChannelProvider"
    - _The native endpoints that link to the JNI entry-points in the Channel Provider C code, implemented in AIDA-PVA Module in `STANDALONELIB`_.

## AIDA-PVA Module
The AIDA-PVA module is a set of C files compiled into Modules and inserted into `STANDALONELIB`, as well as a set of 
header files that are used to reference the functions declared in the Modules and to define types that they use.  
Module header files are used internally by the AIDA-PVA Module.

- C Files
  - aida_pva_jni_helper.c
  - aida_pva_server_helper.c
  - aida_pva_types_helper.c
  - NativeChannelProviderJni.c
- Module Header Files
  - aida_pva_jni_helper.h
  - aida_pva_server_helper.h
  - aida_pva_types_helper.h
  - slac_aida_NativeChannelProvider.h
- AIDA-PVA Module header files
  - aida_pva.h
  - aida_pva_api.h
  - aida_pva_convert.h
  - aida_pva_exceptions.h
  - aida_pva_json.h
  - aida_pva_memory.h
  - aida_pva_types.h
  - aida_pva_uri.h

The files are all committed to CMS and in STANDALONELIB.  The headers are all committed into C_INC.

For an explanation of the functions provided by AIDA-PVA Module see [AIDA-PVA Module Documentation](2_5_1_AIDA-PVA_Module_Functions.md)

## AIDA-PVA Channel Provider

Native Providers are coded to implement the AIDA-PVA API defined in aida_pva_api.h.

For an explanation of how the Channel Providers are coded see [The Channel Provider Programmers Reference](2_0_Channel_Provider_Programmers_Reference)

## Legacy AIDA Modules

AIDA Modules are legacy modules that were created for AIDA. They are used in AIDA-PVA as the low level modules that
access the SLAC Network devices. They are implemented as libraries in `AIDASHR` and are linked with the AIDA-PVA Channel
Provider in the build process.

General Documentation for these AIDA Modules can be [found here](https://www.slac.stanford.edu/grp/cd/soft/aida/)

_Specific Code for each legacy module_:

* [SLC Control Database Module](http://www-mcc.slac.stanford.edu/ref_0/AIDASHR/DPSLC_JNI_HELPER.C)
* [SLC Bpm Module](http://www-mcc.slac.stanford.edu/ref_0/AIDASHR/DPSLCBPM_JNI_HELPER.C)
* [SLC Buffered Data Module](http://www-mcc.slac.stanford.edu/ref_0/AIDASHR/DPSLCBUFF_JNI_HELPER.C)
* [SLC Klystron Module](http://www-mcc.slac.stanford.edu/ref_0/AIDASHR/DPSLCKLYS_JNI_HELPER.C)
* [SLC Magnet Module](http://www-mcc.slac.stanford.edu/ref_0/AIDASHR/DPSLCMAGNET_JNI_HELPER.C)
* [LCLS Model Module](http://www-mcc.slac.stanford.edu/ref_0/AIDASHR/DPSLCMODEL_JNI_HELPER.C)
* [SLC Master Oscillator](http://www-mcc.slac.stanford.edu/ref_0/AIDASHR/DPSLCMOSC_JNI_HELPER.C)
* [SLC Util](http://www-mcc.slac.stanford.edu/ref_0/AIDASHR/DPSLCUTIL_JNI_HELPER.C)

## Building AIDA-PVA SERVICE
see [Building AIDA-PVA SERVICE](3_1_Building_AIDA_PVA_Service.md)
## Building AIDA-PVA Module
see [Building AIDA-PVA Module](3_2_Building_AIDA_PVA_into_STANDALONELIB.md)
## Building AIDA-PVA a Channel Provider
see [Building AIDA-PVA a Channel Provider](3_3_Building_AIDA_PVA_Channel_Provider.md)
