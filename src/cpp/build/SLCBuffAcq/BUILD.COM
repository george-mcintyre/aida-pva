$! Build shareable C library for Java interface
$ OPTS = "/prefix=all /float=ieee /ieee=denorm /define=JIT_OPTION /names=(shortened,as_is) /reentrancy=multithread"
$ INCL = "/include=sys$common:[java$122.include...] /include=slctxt:[C_INC] /include=[--.INCLUDE...] "
$ CC/LIS 'OPTS'  'INCL' [--.IMPL.SLCBUFFACQ]slcBuffAcq_server.c
$ CC/LIS 'OPTS'  'INCL' [--.AIDA-PVA]NativeChannelProviderJni.c
$ CC/LIS 'OPTS'  'INCL' [--.AIDA-PVA]aida_server_helper.c
$ CC/LIS 'OPTS'  'INCL' [--.AIDA-PVA]aida_jni_helper.c
$ CC/LIS 'OPTS'  'INCL' [--.AIDA-PVA]aida_types_helper.c
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
$ LINK -
   /MAP/DEBUG/SHARE=AIDA-PVA.EXE -
   global_symbol_option /OPT, -
   library_option /OPT, -
   SLCOPT:SLC_COMMON.OPT /OPT, -
   SLCOPT:ALPHA_SYSSHR.OPT /OPT, -
   SLCOPT:DBSYMBOL /OPT
$!
$ COPY AIDA-PVA.EXE [---.lib]
$ COPY AIDA-PVA.EXE [---.lib.SLCBUFFACQ]
