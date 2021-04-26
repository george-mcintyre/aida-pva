#!
#!-*-sh-*-
#==============================================================
#
# Name: aidaSetEnv.sh
#
# Abs:  Environment variables needed for Aida on bash unix.
#
# Rem:  "source" this shell script from a bourne or bash shell 
#       when you need to run an Aida peer (client or server) server. 
#       If you are *developing* Aida servers
#       source aidaSetEnvDev.csh on Solaris instead (no sh version yet).
#
#       example:
#         > source /afs/slac/package/aida/common/script/aidaSetEnv.sh
#
# Auth:  06-Jul-2013, Greg White
# Rev:   dd-mmm-1990, Whoever. 
#
#==============================================================
# Mods:
# 10-Jun-2013, Greg White (greg)
# Increase to 100s (from 30) for case of a lot of archive data.
#============================================================== 

mode=DEV
if [ x$1 != x"" ]; then
	mode=`(echo $1|tr '[[:lower:]]' '[[:upper:]]')`
fi

# Move to aidaSetEnvDev.sh when that exists
# For DpCa, we need R3.14, for both development (to find jca.jar), and 
# at runtime (for jca.jar, libjca.so and libca.so)
#source $CD_SOFT/ref/package/aida/common/script/R13toR14.csh

# Need oracle for development
unset ORA_SETUP
source $CD_SOFT/dev/script/oracle_env11g.sh
export ORACLE_CLASSPATH=$ORACLE_HOME/jdbc/lib/ojdbc6_g.jar
export CLASSPATH=$CLASSPATH:${ORACLE_CLASSPATH}

# JAMA is a Matrix package from NIST
# The required CLASSPATH for Aida clients (and servers in fact)
# export CLASSPATH=./aida.jar
export CLASSPATH=${CLASSPATH}:/afs/slac.stanford.edu/g/cd/soft/ref/package/aida/lib/aida.jar
export CLASSPATH=${CLASSPATH}:/afs/slac.stanford.edu/g/cd/soft/ref/package/err/lib/err.jar
export CLASSPATH=${CLASSPATH}:/afs/slac.stanford.edu/g/cd/soft/ref/package/except/lib/except.jar
export CLASSPATH=${CLASSPATH}:/afs/slac/package/iona/orbacus/prod/JOB/lib/OB.jar
export CLASSPATH=${CLASSPATH}:/afs/slac/package/iona/orbacus/prod/JOB/lib/OBEvent.jar
export CLASSPATH=${CLASSPATH}:/afs/slac/package/iona/orbacus/prod/JOB/lib/OBUtil.jar

# Move to aidaSetEnvFull.sh
# export CLASSPATH=/afs/slac.stanford.edu/g/cd/soft/support/package/jama/Jama-1.0.2.jar

# AIDA_MODE sets which AIDA network is set up "AIDADEV", "AIDAPROD" or "AIDALCLS".
# That is, whether this setup is for AIDA development, "production" (Desktops, offices,
# Facet, the Test Accelerator etc, or specifically LCLS. This determines which Oracle 
# DB instance is used (TWO_TASK).
#
if [ x$mode == x"DEV" ]; then
   export AIDA_MODE=DEV
   export TWO_TASK=SLACPROD
   export TNS_ADMIN=/afs/slac/g/lcls/tools/oracle/wallets/aidadev
   export AIDA_DATABASE_USERID=AIDADEV
   export AIDA_CONNECTION_STRING=jdbc:oracle:thin:@slac-oracle03.slac.stanford.edu:1521:SLACPROD
elif [ x$mode == x"LCLS" ]; then
   export AIDA_MODE=LCLS
   export TWO_TASK=MCCO
   export AIDA_DATABASE_USERID=AIDA
   export TNS_ADMIN=/afs/slac/g/lcls/tools/oracle/wallets/aida
elif [ x$mode == x"LCLSDEV" ]; then
   export AIDA_MODE=LCLS
   export TWO_TASK=MCCQA
   export AIDA_DATABASE_USERID=AIDA
   export TNS_ADMIN=/afs/slac/g/lcls/tools/oracle/wallets/aida
else 
   export AIDA_MODE=PROD
   export TWO_TASK=SLACPROD
   export AIDA_DATABASE_USERID=AIDAPROD
   export TNS_ADMIN=/afs/slac/g/lcls/tools/oracle/wallets/aidaprod
   export AIDA_CONNECTION_STRING=jdbc:oracle:thin:@slac-oracle03.slac.stanford.edu:1521:SLACPROD
fi


# Set default Javaver for AIDA
export JAVAVER=1.5

# Set bootclasspath. This is necessary for ORBACUS' Corba specifically, because
# there is a bug in the mechanism Orbacus uses to override the default Corba 
# implementation of Java in Java 1.4 and 1.5! 
#
if [ $JAVAVER == 1.3 ]; then 
    export AIDABCSTRING=' '
elif [ $JAVAVER == 1.4 -o $JAVAVER == 1.5 ]; then 
    export AIDABCSTRING='-Xbootclasspath/p:'${AIDABCLASSPATH}
fi


# Most servers (all except NameServer) use AIDA_NAMESERVER_IOR_URL
# AIDA_MODE sets which AIDA network is set up "AIDADEV", "AIDAPROD" or "AIDALCLS".
# That is, whether this setup is for AIDA development, "production" (Desktops, offices,
# Facet, the Test Accelerator etc, or specifically LCLS. This determines which Oracle 
# DB instance is used (TWO_TASK).
#
if [ x$mode == x"DEV" ]; then
   export AIDA_NAMESERVER_IOR_URL=http://mccas0.slac.stanford.edu/aida/NameServerDEV.ior
elif [ x$mode == x"LCLS" ]; then
   export AIDA_NAMESERVER_IOR_URL=http://mccas0.slac.stanford.edu/u1/lcls/package/aida/ior/NameServerLCLS.ior
else 
   export AIDA_NAMESERVER_IOR_URL=http://mccas0.slac.stanford.edu/aida/NameServerPROD.ior
fi


# The NameServer uses an non-URL specification of the IOR file because it has to write 
# to the filesystem. Also dpTestHist calls the above env variable IOR_FILE "REF_FILE", 
# so additionally define it for dpTestHist. Only set these variables in this script
# if we are setting up the DEV or PROD network, because these this script is ONLY
# used on the CLIENT side for AIDALCLS network, where these env vars are specifically not
# needed.
#
if [ x$mode == x"DEV" -o x$mode == x"PROD" ]; then
   export AIDA_NAMESERVER_IOR_FILE=/nfs/slac/g/cd/www/aida/NameServer${AIDA_MODE}.ior 
   export AIDA_NAME_SERVER_REF_FILE=/nfs/slac/g/cd/www/aida/NameServer${AIDA_MODE}.ior
fi

# How long the CORBA client waits for server response.
export AIDA_CLIENT_TIMEOUT=100
