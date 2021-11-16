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
        providers/SLCREF/AIDASLCREF_SERVER.c
        providers/SLCREF/AIDASLCREF_SERVER.h
        providers/SLCDB/AIDASLCDB_SERVER.c
        providers/SLCDB/AIDASLCDB_SERVER.h
        providers/SLCBPM/AIDASLCBPM_SERVER.c
        providers/SLCBPM/AIDASLCBPM_SERVER.h
        providers/SLCBPMBUFF/AIDASLCBPMBUFF_SERVER.c
        providers/SLCBPMBUFF/AIDASLCBPMBUFF_SERVER.h
        providers/SLCKLYS/AIDASLCKLYS_SERVER.c
        providers/SLCKLYS/AIDASLCKLYS_SERVER.h
        providers/SLCMAGNET/AIDASLCMAGNET_SERVER.c
        providers/SLCMAGNET/AIDASLCMAGNET_SERVER.h
        providers/SLCMODEL/AIDASLCMODEL_SERVER.c
        providers/SLCMODEL/AIDASLCMODEL_SERVER.h
        providers/SLCMOSC/AIDASLCMOSC_SERVER.c
        providers/SLCMOSC/AIDASLCMOSC_SERVER.h
        providers/SLCUTIL/AIDASLCUTIL_SERVER.c
        providers/SLCUTIL/AIDASLCUTIL_SERVER.h)
