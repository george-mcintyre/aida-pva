#-*-csh-*-
#==============================================================
#
# Name: aidaSetEnv.sh
#
# Abs:  Environment variables needed for Aida on unix.
#
# Rem:  "source" this shell script from a "c" shell (like tcsh)
#       when you need to run Aida servers. If you are developing Aida servers
#       source aidaSetEnvDev.csh instead.
#
#       example:
#         flora03:~> source /afs/slac/package/aida/common/script/aidaSetEnv.sh
# 
# Auth:  06-Jul-2001, Greg White
# Rev:   dd-mmm-1990, Whoever. 
#
#==============================================================
# Mods:
#        15-Aug-2013, Greg White
#          Exlicitly set LD_LIBRARY_PATH entry for Oracle libraries to use
#          the oracle 32bit libs (lib32/) rather than 64 bit libs (lib/). This 
#          fixes UnatisfiedLink errors when JDBC ojdbc5/6.jar attempts to link
#          to oracle/f/11.1.0/sun4x_59/11.1.0/lib/libocijdbc11.so at runtime. That
#          .so is 64 bit, but 32bit is required. NOTE, we can not regularise on 
#          64 bit because it seems the OOC CORBA implementation requires 32 bit
#          java model.
#	 26-Oct-2012 Partha/Greg Implemented Oracle Wallet Support
#        08-Mar-2011, Greg White (greg)
#          Changed setting of AIDA_CONNECTION_STRING from
#              jdbc:oracle:thin:@slac-oracle03.slac.stanford.edu:1521:SLACPROD
#          to
#              jdbc:oracle:thin:@slacprod.slac.stanford.edu:1521:SLACPROD
#          (replacing slac-oracle03 with slacprod) for AIDADEV and AIDAPROD.
#        11-Nov-2010, Greg White (greg)
#          Change -Xmx param in all startup "aida*" alias, to 1G from 256M 
#        10-Sep-2010, Greg White (greg)
#          Set AIDADEV to use IP alias to MCCOTEST, to verify IP Aliasing works for JDBC.
#          This is a precondition of the DB failover architecture.
#        13-Jan-2010, Greg White (greg)
#          Add aliases for testing
#        30-Nov-2007, Greg White (greg)
#          Changed URL of NameServer IOR bootstrap file to one that can be seen by LCLS CA.
#        27-Oct-2007, Bob Hall (rdh)
#          Added AIDA_CLIENT_TIMEOUT for aidatest and aidatestdebug.
#        04-Apr-2007, Bob Hall (rdh)
#          Added aidaprocrealm and aidarunrealm for use in st.DpCaServer to
#          pass a string to the LCLS Aida Channel Access server using a
#          Java system property key named "realm". 
#        18-Jul-2006, Greg White (greg)
#          Added -Xms and -Xmx to aidaproc, and remove comment that aidarun is deprecated.
#          We'll use aidarun on production machines, and aidaproc on development. aidaproc
#          includes spec of a debugger port.
#        14-Jul-2006, Greg White (greg)
#          Add -Xms and -Xmx to allocate more heap to JVM. So far only added to aidarun,
#          (which is not used in general), but this release goes with new st.DaNameServer
#          to use aidarun insead of aidaproc. This is to see if changes fix java.OutOfMemory
#          errors in DaNameServer (see logs of $CD_LOG/proc/*st.DaNameServer* 13 & 14 Jul 2006).
#        31-Jul-2005, Greg White (greg)
#          Removed AIDA_NAMESERVER_IOR_FILE from aidatest and aidatestdebug since that
#          is only used by NameServer's init, so only relevant to aidarun and aidaproc
#        25-Jul-2005, Greg White (greg)
#          Added -Dooc.config= java property to "run" aliases, so servers get assigned port.
#        10-Mar-2005, Greg White (greg)
#          Added R13toR14.csh for development and execution of DpCa.
#        17-Jan-2005, Greg White (greg)
#          Add aidaproc for running servers in attachable processes.
#          Remove EventService specs from server arguments - that's now hard coded into Err.
#          Add -server JVM option, per review recommendation.
#        06-Dec-2004, Greg White (greg)
#          Only add ORACLE_CLASSPATH if it's not already there.
#        29-Nov-2004, Greg White (greg)
#          Moved sourcing setup files (ENVS.csh, oracle_env.csh) etc
#          to here from aidaSetEnvDev.csh, so st. startup files can
#          source this to set up an environment.
#        11-May-2004, Bob Hall (rdh)
#          Removed "#!/bin/tcsh -f" from first line since this is a
#          non-executable script (that is, it should be sourced, not
#          executed). 
#        02-Feb-2004, Ron MacKenzie
#          Change   EVENT_SERVICE_HOST
#        07-Jan-2004, Greg White
#          Set the JAVAVER to 1.4
#        24-Apr-2003, Ron MacKenzie
#          Removed AIDA_CHADS_REFERENCE_FILE because dpChads uses Oracle 
#          to store reference now.
#        ...
#============================================================== 

set mode = DEV
if ( $1 != "" ) then
	set mode = `(echo $1|tr '[[:lower:]]' '[[:upper:]]')`
endif

# For DpCa, we need R3.14, for both development (to find jca.jar), and 
# at runtime (for jca.jar, libjca.so and libca.so)
source $CD_SOFT/ref/package/aida/common/script/R13toR14.csh

# Oracle
# 
# MOD: 15-Aug-2013. Replaced use of CD_SOFTs oracle_env11g.csh 
# with explicit assignments here, since AIDA requires LD_LIBRARY_PATH to
# point to 32 bit libs - see MOD note for more.
#
# source ${CD_SOFT}/dev/script/oracle_env11g.csh
unsetenv ORA_SETUP
setenv ORACLE_HOME /afs/slac/package/oracle/@sys/11.1.0
setenv PATH $ORACLE_HOME/bin:${PATH}
if ( ${?LD_LIBRARY_PATH} ) then
  setenv LD_LIBRARY_PATH $ORACLE_HOME/lib32:${LD_LIBRARY_PATH}
else
  setenv LD_LIBRARY_PATH $ORACLE_HOME/lib32
endif
setenv ORACLE_WALLET_TOP /afs/slac/g/lcls/tools/oracle/wallets
setenv TWO_TASK SLACDEV
setenv ORACLE_CLASSPATH $ORACLE_HOME/jdbc/lib/ojdbc5.jar
caddpath CLASSPATH ${ORACLE_CLASSPATH}
setenv ORACLE_PATH /afs/slac/g/cd/soft/dev/ora

# JAMA is a Matrix package from NIST
#
caddpath CLASSPATH /afs/slac.stanford.edu/g/cd/soft/support/package/jama/Jama-1.0.2.jar

# Set environment dependent on the AIDA_MODE.
#
# AIDA_MODE sets which AIDA network is set up "AIDADEV", "AIDAPROD" or "AIDALCLS".
# That is, whether this setup is for AIDA development, "production" (Desktops, offices,
# Facet, the Test Accelerator etc, or specifically LCLS. This determines which Oracle 
# DB instance is used (TWO_TASK).
#
if ( x$mode == x"DEV" ) then
   setenv AIDA_MODE DEV
   setenv TWO_TASK SLACPROD
   setenv AIDA_DATABASE_USERID AIDADEV
   setenv TNS_ADMIN /afs/slac/g/lcls/tools/oracle/wallets/aidadev
   setenv AIDA_CONNECTION_STRING jdbc:oracle:oci:/@SLACPROD
else if ( x$mode == x"LCLS" ) then
   setenv AIDA_MODE LCLS
   setenv TWO_TASK MCCO
   setenv AIDA_DATABASE_USERID AIDA
   setenv TNS_ADMIN /afs/slac/g/lcls/tools/oracle/wallets/aida
else if ( x$mode == x"LCLSDEV" ) then
   setenv AIDA_MODE LCLS
   setenv TWO_TASK MCCQA
   setenv AIDA_DATABASE_USERID AIDA
   setenv TNS_ADMIN /afs/slac/g/lcls/tools/oracle/wallets/aida
else 
   setenv AIDA_MODE PROD
   setenv TWO_TASK SLACPROD
   setenv AIDA_DATABASE_USERID AIDAPROD
   setenv AIDA_CONNECTION_STRING jdbc:oracle:oci:/@SLACPROD
   setenv TNS_ADMIN /afs/slac/g/lcls/tools/oracle/wallets/aidaprod
endif


# Set default Javaver for AIDA
setenv JAVAVER 1.5

# Set bootclasspath. This is necessary for ORBACUS' Corba specifically, because
# there is a bug in the mechanism Orbacus uses to override the default Corba 
# implementation of Java in Java 1.4 and 1.5! 
#
if ($JAVAVER == 1.3) setenv AIDABCSTRING ' '
if ($JAVAVER == 1.4 || $JAVAVER == 1.5) then 
	setenv AIDABCSTRING '-Xbootclasspath/p:'${AIDABCLASSPATH}
endif

#
# These two env. varbs. are used in construction of java alias' below 
# and ALSO used by C++ programs directly. This is defined here rather than 
# ENVS or pathSetup, because at present, ENVS and pathsetup distinguish 
# only /afs and gateway machines, whereas uses /afs for dev and prod. We 
# should move this to ENVS when ENVS system can set up an environment for
# PRODONDEV as well as DEV.
#
# Most servers (all except NameServer) use AIDA_NAMESERVER_IOR_URL
# AIDA_MODE sets which AIDA network is set up "AIDADEV", "AIDAPROD" or "AIDALCLS".
# That is, whether this setup is for AIDA development, "production" (Desktops, offices,
# Facet, the Test Accelerator etc, or specifically LCLS. This determines which Oracle 
# DB instance is used (TWO_TASK).
#
if ( x$mode == x"DEV" ) then
   setenv AIDA_NAMESERVER_IOR_URL http://mccas0.slac.stanford.edu/aida/NameServerDEV.ior
else if ( x$mode == x"LCLS" ) then
   setenv AIDA_NAMESERVER_IOR_URL http://mccas0.slac.stanford.edu/u1/lcls/package/aida/ior/NameServerLCLS.ior
else 
   setenv AIDA_NAMESERVER_IOR_URL http://mccas0.slac.stanford.edu/aida/NameServerPROD.ior
endif


# The NameServer uses an non-URL specification of the IOR file because it has to write 
# to the filesystem. Also dpTestHist calls the above env variable IOR_FILE "REF_FILE", 
# so additionally define it for dpTestHist. Only set these variables in this script
# if we are setting up the DEV or PROD network, because these this script is ONLY
# used on the CLIENT side for AIDALCLS network, where these env vars are specifically not
# needed.
#
if ( x$mode == x"DEV" || x$mode == x"PROD" ) then
   setenv AIDA_NAMESERVER_IOR_FILE /nfs/slac/g/cd/www/aida/NameServer${AIDA_MODE}.ior 
   setenv AIDA_NAME_SERVER_REF_FILE /nfs/slac/g/cd/www/aida/NameServer${AIDA_MODE}.ior
endif

setenv AIDA_CLIENT_TIMEOUT 30 

##
## Run aliases: Not moved to setAlias.csh yet, in case of weird resource
## limit problem.
##

# To start a server in debugger (don't put in background w/ "&" obviously).
# Syntax example: aidadebug $AIDA_NAME_NAME sys.daNameServer.NameServer
alias aidadebug 'jdb ${AIDABCSTRING} -Dooc.config=${CD_CONFSYS}/\!:1.conf -DAIDA_DATABASE_USERID=AIDA${AIDA_MODE} -DAIDA_NAMESERVER_IOR_URL=${AIDA_NAMESERVER_IOR_URL} -DAIDA_NAMESERVER_IOR_FILE=${AIDA_NAMESERVER_IOR_FILE} edu.stanford.slac.aida.\!:2* '

# To start a server so debugger can attach to it. Usage is
# # aidaproc <socket> <aida-executable>
# The <socket> arg is codifed in environment variables of form AIDA_DA_PORT.
# eg: aidaproc $AIDA_DA_DBGADDR sys.daServer.DaServer
# To debug a server started in this way, start it with aidaproc, 
# and then, in another process ON THE SAME HOST AS THE SERVER do: 
#     jdb -attach <socket> [-sourcepath $AIDADEVROOT] (see aidadebugproc below)
#     .. Then in this process, set breakpoints etc. If an aida client causes 
#        the breakpoint to be reached, this process will bring 
#        up the breakpoint. 
# Syntax example: aidaproc $AIDA_NAME_NAME $AIDA_NAME_DBGADDR sys.daNameServer.NameServer 
alias aidaproc 'java -server ${AIDABCSTRING} -Dooc.config=${CD_CONFSYS}/\!:1.conf -DAIDA_DATABASE_USERID=AIDA${AIDA_MODE} -DAIDA_NAMESERVER_IOR_URL=${AIDA_NAMESERVER_IOR_URL} -DAIDA_NAMESERVER_IOR_FILE=${AIDA_NAMESERVER_IOR_FILE} -Xdebug -Xrunjdwp:transport=dt_socket,address=\!:2,server=y,suspend=n -Xms256m -Xmx1G edu.stanford.slac.aida.\!:3* '

# As aidaproc, but adds -Daida.trace to turn on debug tracing
alias aidatrace 'java -server ${AIDABCSTRING} -Dooc.config=${CD_CONFSYS}/\!:1.conf -DAIDA_DATABASE_USERID=AIDA${AIDA_MODE} -DAIDA_NAMESERVER_IOR_URL=${AIDA_NAMESERVER_IOR_URL} -DAIDA_NAMESERVER_IOR_FILE=${AIDA_NAMESERVER_IOR_FILE} -Daida.trace -Xdebug -Xrunjdwp:transport=dt_socket,address=\!:2,server=y,suspend=n -Xms256m -Xmx1G edu.stanford.slac.aida.\!:3* '

alias aidaprocrealm 'java -server ${AIDABCSTRING} -Dooc.config=${CD_CONFSYS}/\!:1.conf -Drealm=\!:4 -DAIDA_DATABASE_USERID=AIDA${AIDA_MODE} -DAIDA_NAMESERVER_IOR_URL=${AIDA_NAMESERVER_IOR_URL} -DAIDA_NAMESERVER_IOR_FILE=${AIDA_NAMESERVER_IOR_FILE} -Xdebug -Xrunjdwp:transport=dt_socket,address=\!:2,server=y,suspend=n -Xms256m -Xmx1G edu.stanford.slac.aida.\!:3* '

# To start an aida server not with any debugger
# Syntax example: aidarun $AIDA_NAME_NAME sys.daNameServer.NameServer
alias aidarun 'java -server ${AIDABCSTRING} -Dooc.config=${CD_CONFSYS}/\!:1.conf -DAIDA_DATABASE_USERID=${AIDA_DATABASE_USERID} -DAIDA_CONNECTION_STRING=${AIDA_CONNECTION_STRING} -DAIDA_NAMESERVER_IOR_URL=${AIDA_NAMESERVER_IOR_URL} -DAIDA_NAMESERVER_IOR_FILE=${AIDA_NAMESERVER_IOR_FILE} -Xms256m -Xmx1G edu.stanford.slac.aida.\!:2* '

alias aidarunrealm 'java -server ${AIDABCSTRING} -Dooc.config=${CD_CONFSYS}/\!:1.conf -Drealm=\!:3 -DAIDA_DATABASE_USERID=AIDA${AIDA_MODE} -DAIDA_NAMESERVER_IOR_URL=${AIDA_NAMESERVER_IOR_URL} -DAIDA_NAMESERVER_IOR_FILE=${AIDA_NAMESERVER_IOR_FILE} -Xms256m -Xmx1G edu.stanford.slac.aida.\!:2* '

# To run a test suite program
alias aidatest 'java ${AIDABCSTRING} -DAIDA_NAMESERVER_IOR_URL=${AIDA_NAMESERVER_IOR_URL} -DAIDA_CLIENT_TIMEOUT=${AIDA_CLIENT_TIMEOUT} test.java.\!* '

# To debug a test suite program
alias aidatestdebug 'jdb ${AIDABCSTRING} -DAIDA_NAMESERVER_IOR_URL=${AIDA_NAMESERVER_IOR_URL} -DAIDA_CLIENT_TIMEOUT=${AIDA_CLIENT_TIMEOUT} test.java.\!* '

# To debug a server that has been started by aidaproc
alias aidaprocdebug 'jdb -attach \!*'

exit
