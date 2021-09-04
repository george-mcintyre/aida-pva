cmake_minimum_required(VERSION 3.17)
project(aida-pva C)

set(CMAKE_C_STANDARD 99)

include_directories(include
        /Library/Java/JavaVirtualMachines/jdk1.8.0.jdk/Contents/Home/include
        /Library/Java/JavaVirtualMachines/jdk1.8.0.jdk/Contents/Home/include/darwin)

add_executable(cpp
        impl/Reference/reference_server.c
        include/Reference/reference_server.h
        impl/SLC/slc_server.c
        include/SLC/slc_server.h
        impl/SLCBpm/slcBpm_server.c
        include/SLCBpm/slcBpm_server.h
        impl/SLCBuffAcq/slcBuffAcq_server.c
        include/SLCBuffAcq/slcBuffAcq_server.h
        impl/SLCKlys/slcKlys_server.c
        include/SLCKlys/slcKlys_server.h
        impl/SLCModel/slcModel_server.c
        include/SLCModel/slcModel_server.h
        impl/SLCMosc/slcMosc_server.c
        include/SLCMosc/slcMosc_server.h
        impl/SLCUtil/slcUtil_server.c
        include/SLCUtil/slcUtil_server.h
        include/aida-pva/aida_jni_helper.h
        include/aida-pva/aida_server_helper.h
        include/aida-pva/aida_types.h
        include/aida-pva/slac_aida_NativeChannelProvider.h
        aida-pva/aida_jni_helper.c
        aida-pva/aida_server_helper.c
        aida-pva/aida_types_helper.c
        aida-pva/NativeChannelProviderJni.c)
