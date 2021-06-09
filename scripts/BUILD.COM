$! Build shareable C library for Java interface
$ OPTS = "/prefix=all /float=ieee /ieee=denorm /define=JIT_OPTION /names=(shortened,as_is) /reentrancy=multithread"
$ INCL = "/include=sys$common:[java$122.include...]"
$ CC/LIS 'OPTS'  'INCL'  NativeChannelProviderJni.c
$! CC/DEBUG reference_server.c
$! CC/DEBUG aida_server_helper.c
$! CC/DEBUG aida_jni_helper.c
$! CC/DEBUG aida_types_helper.c
$!
$!
$! Global option file
$ GLB_OPT:= global_symbol_option
$ LIB_OPT:= library_option
$ DELETE GLOBAL_SYMBOL_OPTION.OPT;*
$ @java$jni_example:scan_globals_for_option *.OBJ 'GLB_OPT'.OPT
$
$! link native shareable
$!
$ LINK /MAP/DEBUG/SHARE=aida-pva-reference.EXE global_symbol_option /OPT, library_option /OPT, slcopt:slc_common/opt
$!
