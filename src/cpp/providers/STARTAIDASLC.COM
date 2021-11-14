$!       **CMS**=COM_GEN
$!
$!===============================================================================
$!
$!
$! Name:  STARTAIDASLC.COM
$!
$! Abs:   Starts an AIDA-PVA Channel Provider on VMS.
$!
$!
$!===============================================================================
$!
$!
$! Make intermediate logicals to pass system properties
$! and avoid the VMS DCL 255 char limit.
$!
$  IF procname .EQS. "EPICS-FORWARDER" THEN GOTO STARTFORWARDER
$ libpath == "-Djava.library.path=/SLCSHR"
$ conf == "-Daida.pva.channels.filename=/SLCTXT/''config'"
$ lib == "-Daida.pva.lib.name=''procname'"
$!
$!
$ write sys$output "Starting ''procname' Server"
$       java_argstring = "-jar $conf $libpath $lib SLCLIBS:AIDA-PVA.JAR"
$       @slccom:RUNJAVA_BUT_RESTART_ON_ABORT
$       exit
$!
$STARTFORWARDER:
$ write sys$output "Starting FORWARDER Server"
$       java_argstring = "-jar SLCLIBS:EPICS-FORWARDER.JAR"
$       @slccom:RUNJAVA_BUT_RESTART_ON_ABORT
$       exit
