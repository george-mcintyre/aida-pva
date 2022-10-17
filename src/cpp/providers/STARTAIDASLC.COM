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
$! Set the correct java version 1.5 for running AIDA-PVA
$ @DISK$JAVA:[JAVA$150.COM]JAVA$150_SETUP.COM
$!
$! Make intermediate logicals to pass system properties
$! and avoid the VMS DCL 255 char limit.
$!
$  IF procname .EQS. "EPICS_FORWARDER" THEN GOTO STARTFORWARDER
$ libpath == "-Djava.library.path=/SLCSHR"
$ conf == "-Daida.pva.channels.filename=/SLCTXT/''config'"
$ libname = procname - "_"
$ lib == "-Daida.pva.lib.name=''libname'"
$!
$! There are two types of garbage collector that are implemented for Java 1.5 in the Open VMS Alpha JDK.
$!  1) the compacting garbage collector (default),
$!  2) the copying garbage collector.
$! Switching to the copying garbage collector eliminated an access violation error that was present
$! for long periods in the klystron provider.  It may also positively affect the apparent memory leak that depletes
$! the page file count over the course of operation of the providers.
$ gcflags == "-Xgc:copying"
$!
$ write sys$output "Starting ''procname' Server"
$       java_argstring = "-jar $conf $gcflags $libpath $lib SLCLIBS:AIDA-PVA.JAR"
$       @slccom:RUNJAVA_BUT_RESTART_ON_ABORT
$       exit
$!
$STARTFORWARDER:
$ write sys$output "Starting FORWARDER Server"
$       java_argstring = "-jar SLCLIBS:EPICS_FORWARDER.JAR"
$       @slccom:RUNJAVA_BUT_RESTART_ON_ABORT
$       exit
