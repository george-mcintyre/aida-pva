$! Build shareable C library for AIDA-PVA-MODULE
$ OPTS = "/prefix=all /float=ieee /ieee=denorm /define=JIT_OPTION /names=(shortened,as_is) /reentrancy=multithread"
$ INCL = "/include=sys$common:[java$122.include...] /include=slctxt:[C_INC] /include=[.]  /include=[-.INCLUDE] "
$ CC/LIS 'OPTS'  'INCL' NativeChannelProviderJni.c
$ CC/LIS 'OPTS'  'INCL' aida_pva_server_helper.c
$ CC/LIS 'OPTS'  'INCL' aida_pva_jni_helper.c
$ CC/LIS 'OPTS'  'INCL' aida_pva_types_helper.c
$ CC/LIS 'OPTS'  'INCL' aida_pva_json.c
$!
$!
$! Global option file
$ GLB_OPT:= global_symbol_option
$ LIB_OPT:= library_option
$ DELETE GLOBAL_SYMBOL_OPTION.OPT;*
$ @java$jni_example:scan_globals_for_option *.OBJ 'GLB_OPT'.OPT
$
$! Create AIDA-PVA object library
$ LIBRARY/ALPHA/CREATE AIDA-PVA -
   NativeChannelProviderJni.OBJ, -
   aida_pva_server_helper.OBJ, -
   aida_pva_jni_helper.OBJ, -
   aida_pva_types_helper.OBJ, -
   aida_pva_json.OBJ
$!
$ COPY AIDA-PVA.OLB [-.lib]
