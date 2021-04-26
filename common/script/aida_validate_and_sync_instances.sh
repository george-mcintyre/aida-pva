#!/bin/tcsh -f

#
# Mods:
#   04-Apr-2011, Bob Hall
#      Changed to use getPwd to get Oracle passwords.
#   15-Sep-2010, Bob Hall
#      Changed Aida Oracle password. 
#   23-Mar-2010, Bob Hall
#      Changed Aida Oracle password. 
#

# source /afs/slac/g/cd/soft/dev/script/ENVS.csh
source /afs/slac/g/cd/soft/dev/script/oracle_env.csh
setenv TWO_TASK MCCQA

#
setenv ORACLE_PATH .:${ORACLE_PATH}
setenv ORACLE_PATH /afs/slac/g/cd/soft/tst/ora:${ORACLE_PATH}
setenv ORACLE_PATH /afs/slac/g/cd/soft/dev/ora:${ORACLE_PATH}
setenv ORACLE_HOME /afs/slac/package/oracle/f/11.1.0/sun4x_59/11.1.0
setenv PATH $ORACLE_HOME/bin:${PATH}

rehash

echo
echo '++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++'
echo "Synchronizing $1 names from MCCQA to MCCO BEGINS"
date
setenv TNS_ADMIN /afs/slac/g/lcls/tools/oracle/wallets/aida
sqlplus /@$TWO_TASK @aida_qatoo_sync.sql "$1"
#####sqlplus aida/`getPwd aida` @aida_qatoo_sync.sql "$1" 
set errcode = $status
if( $errcode == 0 ) then 
    set msgtext = 'successfully'
else
    set msgtext = 'UNSUCCESSFULLY'
endif    

echo
echo "Synchronizing $1 names from MCCQA to MCCO ENDS " $msgtext
date
echo '----------------------------------------------------------------'
