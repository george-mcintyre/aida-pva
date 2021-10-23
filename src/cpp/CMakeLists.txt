cmake_minimum_required(VERSION 3.17)
project(aida-pva C)

set(CMAKE_C_STANDARD 99)

include_directories(include
        /Library/Java/JavaVirtualMachines/jdk1.8.0.jdk/Contents/Home/include
        /Library/Java/JavaVirtualMachines/jdk1.8.0.jdk/Contents/Home/include/darwin
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
        providers/Reference/reference_server.c
        providers/Reference/reference_server.h
        providers/SLC/slc_server.c
        providers/SLC/slc_server.h
        providers/SLCBpm/slcBpm_server.c
        providers/SLCBpm/slcBpm_server.h
        providers/SLCBuffAcq/slcBuffAcq_server.c
        providers/SLCBuffAcq/slcBuffAcq_server.h
        providers/SLCKlys/slcKlys_server.c
        providers/SLCKlys/slcKlys_server.h
        providers/SLCMagnet/SLCMagnet_server.c
        providers/SLCMagnet/slcMagnet_server.h
        providers/SLCModel/slcModel_server.c
        providers/SLCModel/slcModel_server.h
        providers/SLCMosc/slcMosc_server.c
        providers/SLCMosc/slcMosc_server.h
        providers/SLCUtil/slcUtil_server.c
        providers/SLCUtil/slcUtil_server.h)
