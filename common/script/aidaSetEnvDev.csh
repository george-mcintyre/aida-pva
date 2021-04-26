#-*-csh-*-
#==============================================================
#
# Name: aidaSetEnvDev.sh
#
# Abs: Sets Aida development environment on UNIX
#
# Rem:  "source" this shell script from a "c" shell (like tcsh)
#       when you need to develop or change the aida package.
#
# Use:  source aidaSetEnvDev.sh [{DEV|PROD}] [<development-dir>]
#       
#       [{DEV|PROD}] 1st arg is optional (but must be given if the second
#                    is given), and specifies which
#                    of development or production Aida network
#                    environment variables will be setup for subsequent
#                    server or client invocations. Default is DEV.
#       [<development-dir>] 2nd arg is optional (but the 1st
#                    must be given if the 2nd is given). The development dir
#                    is a directory-pathname containing the java root of
#                    Aida package(s) for testing. It will be prepended to
#                    the CLASSPATH (so it probably has an "edu" subdirectory).
#       E.g.: 
#       > source $AIDA/common/script/aidaSetEnvDev.sh DEV ~/dev/aida/package/aida
#
# 
# Facility: SLAC/UTILITY (or something else)
#
# Auth:  30-Aug-2002, Greg White
# Rev:   dd-mmm-1990, Whoever. 
#
#==============================================================
# Mods:
#  06-Dec-2004, Greg White (greg)
#     undo environment var changes if no test directory is given (return to production).
#  29-Nov-2004, Greg White (greg)
#     Moved sourcing setup files (ENVS.csh, oracle_env.csh) etc
#     from here to aidaSetEnv.csh, so st. startup files can
#     source that to set up an environment.
#  18-May-2004, Greg White (greg)
#     Added oracle_env.csh and append ORACLE_CLASSPATH to CLASSPATH, as do
#     Aida st files.
#  11-MAy-2004, Greg White (greg)
#     Set the default value of AIDADEVROOT to the production Aida root.
#  11-May-2004, Bob Hall (rdh)
#     Added check whether an xdm path argument is supplied to
#     this script.
#  29-Apr-2004, Greg White (greg)
#     Added unsetting and resetting the basic environment from ENVS.csh
#  24-Apr-2003, Ron MacKenzie
#     Create AIDA_NAMESERVER_IOR* env. varbs so C++ programs can use them.
#     Then use them in creation of java alias' also.
#  24-Feb-2003, Robert C. Sass (RCS)
#     Add properties to aidadev & aidatest aliases for PROD/DEV mode.
#  21-Jan-2003, Ron MacKenzie (RONM)
#     Add creation of bin directory and symbolic link for epics extensions.
#   1-Nov-2002, Greg White
#     Remove dos line endings.
#        
#============================================================== 
#
# Set or reset project roots, and prepend the CLASSPATH if a test Aida package
# root was given in the 2nd argument. Rememeber, AIDADEVROOT is only defined
# by this script (not ENVS), but must be defined in order to run aidamake, even
# for production builds (so AIDADEVROOT should never be unsetenv by this script).
#
if ( ${2} != "" ) then
   setenv AIDADEVROOT ${2}
   setenv AIDASCRIPT ${AIDADEVROOT}/common/script
   caddprepath CLASSPATH ${AIDADEVROOT}
else
   if ( $?AIDADEVROOT ) then 
	  if ( ${AIDADEVROOT} != ${AIDAROOT} ) then
	     delpath CLASSPATH ${AIDADEVROOT}
      endif
   endif
   setenv AIDASCRIPT ${AIDAROOT}/common/script
   setenv AIDADEVROOT ${AIDAROOT}
endif

source ${AIDASCRIPT}/aidaSetEnv.csh ${1}

if ( ${2} != "" ) then
    caddprepath ORACLE_PATH ${AIDASCRIPT}
    caddprepath ORACLE_PATH ${2}/../../app/pvudb_load
    caddprepath PATH ${AIDASCRIPT}
    caddprepath PATH ${2}/../../common/tool
    caddprepath PATH ${2}/../../app/pvudb_load
endif
rehash

alias aidamake 'cd ${AIDADEVROOT} && gmake -f common/script/MakefileAida.sun4 \!*'

