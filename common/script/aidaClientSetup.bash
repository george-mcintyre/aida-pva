#-*-sh-*-
#==============================================================
#
# Name: aidaClientSetup.bash
#
# Abs:  Environment variables for running Aida clients on production.
#
# Rem:  "source" this shell script from a bash/bourne shell (like bash)
#       when you need to run Aida clients.
#
#       example:
#         lcls-srv01:~> [ export AIDA_MODE=DEV ]            
#         lcls-srv01:~> source /usr/local/lcls/physics/package/...
#                       aida/common/script/aidaClientSetup.bash
# 
# Auth:  08-Jan-2008, Greg White
# Rev:   dd-mmm-1990, Whoever. 
#
#==============================================================
# Mods:  25-Jan-2010, Greg White
#        Add ./aida.jar at head of classpath, so can test a local one.
#============================================================== 


# AIDA_MODE sets whether this setup is for the AIDA development 
# or production instance. eg, whic AIDA name database is used.
if [ -z $AIDA_MODE ]; then
    export AIDA_MODE=PROD
fi

# Database setup. The Aida schemata AIDADEV and AIDAPROD are in the 
# SLACPROD Oracle instance
export TWO_TASK=SLACPROD

# The required CLASSPATH for Aida clients (and servers in fact)
export CLASSPATH=./aida.jar
export CLASSPATH=${CLASSPATH}:/afs/slac.stanford.edu/g/cd/soft/ref/package/aida/lib/aida.jar
export CLASSPATH=${CLASSPATH}:/afs/slac.stanford.edu/g/cd/soft/ref/package/err/lib/err.jar
export CLASSPATH=${CLASSPATH}:/afs/slac.stanford.edu/g/cd/soft/ref/package/except/lib/except.jar
export CLASSPATH=${CLASSPATH}:/afs/slac/package/iona/orbacus/prod/JOB/lib/OB.jar
export CLASSPATH=${CLASSPATH}:/afs/slac/package/iona/orbacus/prod/JOB/lib/OBEvent.jar
export CLASSPATH=${CLASSPATH}:/afs/slac/package/iona/orbacus/prod/JOB/lib/OBUtil.jar

# For JAVAVER == 1.4 and above, set bootclasspath. 
export AIDABCLASSPATH=/afs/slac/package/iona/orbacus/prod/JOB/lib/OB.jar
export AIDABCSTRING='-Xbootclasspath/p:'${AIDABCLASSPATH}

#
# Most servers (all except NameServer) use AIDA_NAMESERVER_IOR_URL
if [ x$AIDA_MODEL == x"DEV" ]; then
   export AIDA_NAMESERVER_IOR_URL=http://mccas0.slac.stanford.edu/aida/NameServerDEV.ior
elif [ x$AIDA_MODE == x"LCLS" ]; then
   export AIDA_NAMESERVER_IOR_URL=http://mccas0.slac.stanford.edu/u1/lcls/package/aida/ior/NameServerLCLS.ior
else 
   export AIDA_NAMESERVER_IOR_URL=http://mccas0.slac.stanford.edu/aida/NameServerPROD.ior
fi

export AIDA_CLIENT_TIMEOUT=100

