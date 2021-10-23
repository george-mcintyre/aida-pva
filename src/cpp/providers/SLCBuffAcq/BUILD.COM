$! Build shareable C library for Java interface
$ OPTS = "/prefix=all /float=ieee /ieee=denorm /define=JIT_OPTION /names=(shortened,as_is) /reentrancy=multithread"
$ INCL = "/include=sys$common:[java$122.include...] /include=slctxt:[C_INC] /include=[--.INCLUDE] "
$ CC/LIS 'OPTS'  'INCL' slcBuffAcq_server.c
$!
$! Global option file
$ GLB_OPT:= global_symbol_option
$ AIDA_MODULE_OPT:= [--.aida-pva]global_symbol_option
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
   'AIDA_MODULE_OPT' /OPT, -
   SLCLIBS:AIDASHRLIB/LIB
$!
