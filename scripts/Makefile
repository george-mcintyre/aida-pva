aida-pva-reference.so : main.o kbd.o command.o display.o insert.o search.o files.o utils.o
        ln /MAP/DEBUG/SHARE=aida-pva-reference.EXE aida_types_helper.o aida_jni_helper.o aida_server_helper.o NativeChannelProviderJni.o reference_server.o

aida_types_helper.o : lib/aida_types_helper.c include/aida_types.h
        cc -c lib/aida_types_helper.c
aida_jni_helper.o : lib/aida_jni_helper.c include/aida_types.h include/aida_jni_helper.h
        cc -c lib/aida_jni_helper.c
aida_server_helper.o : lib/aida_server_helper.c include/aida_types.h include/aida_server_helper.h
        cc -c lib/aida_server_helper.c
NativeChannelProviderJni.o : lib/NativeChannelProviderJni.c include/aida_types.h include/reference_server.h include/aida_jni_helper.h
        cc -c lib/NativeChannelProviderJni.c
reference_server.o : impl/reference_server.c include/reference_server.h
        cc -c impl/reference_server.c

clean :
        rm aida-pva-reference.EXE aida_types_helper.o aida_jni_helper.o aida_server_helper.o NativeChannelProviderJni.o reference_server.o
