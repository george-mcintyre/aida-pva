cmake_minimum_required(VERSION 3.17)
project(aida-pva C)

set(CMAKE_C_STANDARD 99)

include_directories(include
        /Library/Java/JavaVirtualMachines/jdk1.8.0.jdk/Contents/Home/include
        /Library/Java/JavaVirtualMachines/jdk1.8.0.jdk/Contents/Home/include/darwin)

add_executable(cpp
        impl/reference_server.c
        include/aida_jni_helper.h
        include/aida_server_helper.h
        include/aida_types.h
        include/slc_macr.hc
        include/slc_macros.h
        include/reference_server.h
        include/slac_aida_NativeChannelProvider.h
        lib/aida_jni_helper.c
        lib/aida_server_helper.c
        lib/aida_types_helper.c
        lib/NativeChannelProviderJni.c)
