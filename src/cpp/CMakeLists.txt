cmake_minimum_required(VERSION 3.17)
project(aida-pva C)

set(CMAKE_C_STANDARD 99)

include_directories(include
        /Library/Java/JavaVirtualMachines/jdk1.8.0.jdk/Contents/Home/include
        /Library/Java/JavaVirtualMachines/jdk1.8.0.jdk/Contents/Home/include/darwin
        /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include
        /Users/george/Projects/com/SLAC/include
        include
        )

add_executable(cpp
        aida-pva/aida_pva_jni_helper.c
        aida-pva/aida_pva_jni_helper.h
        aida-pva/aida_pva_server_helper.c
        aida-pva/aida_pva_server_helper.h
        aida-pva/aida_pva_types_helper.c
        aida-pva/aida_pva_types_helper.h
        aida-pva/NativeChannelProviderJni.c
        aida-pva/slac_aida_NativeChannelProvider.h
        aida-pva/aida_pva_json.c
        include/aida_pva_json.h
        include/aida_pva.h
        include/aida_pva_api.h
        include/aida_pva_convert.h
        include/aida_pva_exceptions.h
        include/aida_pva_memory.h
        include/aida_pva_types.h
        include/aida_pva_uri.h
        providers/Reference/AIDASLCREF_SERVER.c
        providers/Reference/AIDASLCREF_SERVER.h
        providers/SLC/AIDASLCDB_SERVER.c
        providers/SLC/AIDASLCDB_SERVER.h
        providers/SLCBpm/AIDASLCBPM_SERVER.c
        providers/SLCBpm/AIDASLCBPM_SERVER.h
        providers/SLCBuffAcq/AIDASLCBPMBUFF_SERVER.c
        providers/SLCBuffAcq/AIDASLCBPMBUFF_SERVER.h
        providers/SLCKlys/AIDASLCKLYS_SERVER.c
        providers/SLCKlys/AIDASLCKLYS_SERVER.h
        providers/SLCMagnet/AIDASLCMAGNET_SERVER.c
        providers/SLCMagnet/AIDASLCMAGNET_SERVER.h
        providers/SLCModel/AIDASLCMODEL_SERVER.c
        providers/SLCModel/AIDASLCMODEL_SERVER.h
        providers/SLCMosc/AIDASLCMOSC_SERVER.c
        providers/SLCMosc/AIDASLCMOSC_SERVER.h
        providers/SLCUtil/AIDASLCUTIL_SERVER.c
        providers/SLCUtil/AIDASLCUTIL_SERVER.h)
