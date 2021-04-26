#
#  Abs:  README file Aida DpCa data provider
#
#  Ver:  not applicable
#
#  Auth: 10-Mar-2005, Greg White (greg)
#  Rev:  
#--------------------------------------------------------------
#  Mod:
#==============================================================

PREREQUISTIES
=============

1. Java Channel Access (JCA) v>= 2.1.5 is required.

2. JCA requires >= EPICS R3.14, and specifically in order to link, JCA
   requires that instance of EPICS be built with shareable libs.

3. Development of DpCa requires Aida envirnment variables, which are
   defined by 'source /afs/slac/g/cd/soft/dev/script/ENVS.csh', but
   since that ENVS.csh also defines EPICS3.13 (13, not 14), we have to
   replace R3.13 *PATH* elements with R3.14 elements after sourcing
   ENVS.csh. So, you must run source
   ${AIDADEVROOT}/common/script/R13toR14.csh after ENVS.csh when doing
   development. For consistency, the st.DpCaServer file is also
   written this way, although it could just as well have sourced
   R3.14's ENVS.csh in /afs/slac/package/epics/script/ENVS.csh and be
   done.


CONFIGURATION
=============

DpCaServer must be run from an account (eg cddev) where there
exists a ~/.JCALibrary/JCALibrary properties file, which details
the ca server, repeater and addr_list which JCA uses to access
Channel Access.


BUILDING
========

BUILDING IN TEST/DEVELOPMENT

source /afs/slac/g/cd/soft/dev/script/ENVS.csh
source $CD_SCRIPT/aidaSetEnvDev.csh dev ${HOME}/dev/aida/package/aida
aidamake dpCa DEBUG=-g (or just gmake -f Makefile.sun4)

BUILDING IN PRODUCTION

aidamake dpCa
See http://www.slac.stanford.edu/grp/cd/soft/aida/newLabourCheatsheet.html


DEPLOYMENT
==========

See http://www.slac.stanford.edu/grp/cd/soft/aida/newLabourCheatsheet.html


EXECUTION
=========

IN TEST/DEVELOPMENT

To run in the dev network, be on $AIDA_CA_HOST_DEV
To run in the prod network, be on $AIDA_CA_HOST_PROD (mccas0 atow)
then
source /afs/slac/g/cd/soft/dev/script/ENVS.csh
source $CD_SCRIPT/aidaSetEnvDev.csh dev ${HOME}/dev/aida/package/aida
aidaproc $AIDA_CA_DBGADDR dp.dpCa.DpCaServer

IN PRODUCTION

NOTE: dpCa knows realms DEV and PEPII, not as others do, DEV and PROD.
A dpCa started in the PEPII realm joins the PROD aida network.
aidamanager $AIDA_CA_NAME restart {dev|pepii}


TROUBLESHOOTING
===============

* SYMPTOM: java.lang.UnsatisfiedLinkError: _setenv ..  
SOLUTION: make sure, no version of libjca.so prior to 2.1.5 is in
LD_LIBRARY_PATH, for instance,
afs/slac.stanford.edu/g/spear/epics/extensions/lib/solaris, since that
also contains a libjca.so. For instance, such a "broken"
LD_LIBRARY_PATH would result from having run
$CD_SOFT/dev/script/ENVS.csh or some epicsSetup*.csh script that set
EPICS paths to pre EPICS 3.14 directories, or to include only an
instance of EPICS 3.14 that was built for static libs (.a), not
dynamic libs (.so). Be sure to run R13toR14.csh before running dpCa,
to use R3.14 paths.


* SYMPTOM: caget/probe gets different values than Aida.
  Eg [mccas0]:aida/common/script> caget PR00:ASTS:CH117
PR00:ASTS:CH117                -2.7 
[mccas0]:aida/common/script> aidatest daDpCaTest 1
			Float v = (Float)da.get("PR00:ASTS:CH117//VAL", DaValue.Type.FLOAT);
			System.out.println( "\t=" + v);
       =-0.4826742
EXPL: EPICS_CA_ADDR_LIST, as setup by $CD_SOFT/dev/script/ENVS.csh when run on AFS (development) is valued like this:
[mccas0]:aida/common/script> pe EPICS_CA_ADDR_LIST
134.79.49.200 134.79.49.202 134.79.49.205 134.79.49.207 134.79.48.15 134.79.76.34 134.79.51.61
                                                             mccdev
the others are development IOCs
However, dpCa, which runs on mccas0, uses a Channel Access Address list defined in 
~cddev/.JCALibrary/JCALibrary.properties like this:
[mccas0]:u/cd/cddev> cat .JCALibrary/JCALibrary.properties | grep addr_list
gov.aps.jca.jni.JNIContext.auto_addr_list=false
gov.aps.jca.jni.JNIContext.addr_list=134.79.48.11 134.79.51.48 134.79.51.43 134.79.51.127
                                          mcc

That is, Aida gets SLC control system PVs from MCC, not MCCDEV, as
would a caget/probe run on AFS after just sourcing ENVS.csh
  
       
