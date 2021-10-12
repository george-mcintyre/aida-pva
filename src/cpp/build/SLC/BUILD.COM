$! Build shareable C library for Java interface
$ OPTS = "/prefix=all /float=ieee /ieee=denorm /define=JIT_OPTION /names=(shortened,as_is) /reentrancy=multithread"
$ INCL = "/include=sys$common:[java$122.include...] /include=slctxt:[C_INC] /include=[--.INCLUDE...] "
$ CC/LIS 'OPTS'  'INCL' [--.IMPL.SLC]slc_server.c
$ CC/LIS 'OPTS'  'INCL' [--.AIDA-PVA]NativeChannelProviderJni.c
$ CC/LIS 'OPTS'  'INCL' [--.AIDA-PVA]aida_server_helper.c
$ CC/LIS 'OPTS'  'INCL' [--.AIDA-PVA]aida_jni_helper.c
$ CC/LIS 'OPTS'  'INCL' [--.AIDA-PVA]aida_types_helper.c
$ CC/LIS 'OPTS'  'INCL' [--.AIDA-PVA]json.c
$!
$!
$! Global option file
$ GLB_OPT:= global_symbol_option
$ LIB_OPT:= library_option
$ AIDAPVA_OPT:= [-]AIDAPVA
$ DELETE GLOBAL_SYMBOL_OPTION.OPT;*
$ @java$jni_example:scan_globals_for_option *.OBJ 'GLB_OPT'.OPT
$
$! link native shareable
$!
$ LINK -
   /MAP/SHARE=AIDA-PVA.EXE -
   'GLB_OPT' /OPT, -
   'LIB_OPT' /OPT, -
   'AIDAPVA_OPT' /OPT, -
   SLCLIBS:AIDASHRLIB/LIB
$!
$ COPY AIDA-PVA.EXE [---.lib.SLC]
$ COPY CHANNELS.YML [---.lib.SLC]
$ COPY CHANNELS.YAML [---.lib.SLC]
$ COPY REBUILD.COM [---.lib.SLC]
