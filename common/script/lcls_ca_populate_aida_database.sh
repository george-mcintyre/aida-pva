#! /bin/tcsh -f
#
# Abs:   This script populates the Aida Oracle database tables for
#        the LCLS Aida Channel Access server.
#
# Name:  lcls_ca_populate_aida_database.sh  
#
# Rem:   This script is run daily by trscrontab on mccas0 under
#        the cddev account.  The script invokes the
#        set_aida_lcls_ca.sql script to populate the dev and
#        prod Aida Oracle database tables for the LCLS Aida
#        Channel Access server from LCLS system entries in
#        the irmisdb Oracle account curr_pvs table.  The
#        IRMIS crawler scripts that are run daily from trscrontab
#        under the cddev account must run before this script is
#        run.
#
#  Auth: 17-Apr-2007, Bob Hall (rdh)
#  Rev:  dd-mmm-yyyy, Reviewer's Name    (USERNAME)
#--------------------------------------------------------------
#
#  Mod:
#        04-Apr-2014, poonam
#           Each schema processing will send email with the logfile contents if there is any ERROR.
#           Each schema will also email the contents of the logfile if there is a Failure.
#        06-June-2013, divekar
#           Added CONTROLS-SOFTWARE-REPORTS@SLAC.STANFORD.EDU entry for getting status emails. 
#        04-Sep-2012, brobeck
#           Changed RDH to controls-system-admins for mail
#        04-Apr-2011, Bob Hall (rdh)
#           Changed to get Oracle password using getPwd.
#        15-Sep-2010, Bob Hall (rdh)
#           Changed Oracle password.
#        23-Mar-2010, Bob Hall (rdh)
#           Changed Oracle password.
#        12-Mar-2010, Bob Hall (rdh)
#           Changed status checking "tail -6" to "tail -7"
#           to find "errcode" since the end of the log files
#           now have a new "Elapsed:" line.
#        08-Mar-2010, Bob Hall (rdh)
#           Added now needed parameters to set_aida_lcls_ca.sql.
#        07-Jan-2010, Greg White (greg)
#           Add aidalcls
#        26-Aug-2009, Bob Hall (rdh)
#           Changed Oracle password.
#        23-Apr-2009, Bob Hall (rdh)
#           Modified to generate set_aida_lcls_ca log files
#           in /nfs/slac/g/cd/log/aidadb.  Also modified to
#           send mail upon the completion of running
#           set_aida_lcls_ca.sql for the aidaprod account.
#        09-Mar-2009, Bob Hall (rdh)
#           Changed Oracle password.
#        25-Aug-2008, Bob Hall (rdh)
#           Changed Oracle password.
#        13-Feb-2008, Bob Hall (rdh)
#           Changed Oracle password.
#        21-Jun-2007, Bob Hall (rdh)
#           Changed Oracle password.
#
#==============================================================
#
#
source /afs/slac/g/cd/soft/dev/script/ENVS.csh
source $CD_SOFT/dev/script/oracle_env.csh
setenv TWO_TASK SLACPROD

caddpath ORACLE_PATH ${CD_SOFT}/tst/ora
caddpath ORACLE_PATH ${CD_SOFT}/dev/ora
setenv ORACLE_HOME /afs/slac/package/oracle/f/11.1.0/sun4x_59/11.1.0
setenv PATH $ORACLE_HOME/bin:${PATH}

#
# Invoke the set_aida_lcls_ca.sql PL/SQL script to populate
# the aida_names and aida_directory Oracle tables in the Aida
# aidadev and aidaprod Oracle accounts from LCLS system entries
# in the irmisdb Oracle account curr_pvs table.
#
set search_string = "errcode: 0"

set logfname = /nfs/slac/g/cd/log/aidadb/set_aida_lcls_ca_aidadev.log
setenv TNS_ADMIN /afs/slac/g/lcls/tools/oracle/wallets/aidadev
sqlplus /@$TWO_TASK @set_aida_lcls_ca.sql EPICS_lcls Y >> ${logfname}
#####sqlplus aidadev/`getPwd aidadev` @set_aida_lcls_ca.sql EPICS_lcls Y >> ${logfname}
tail -7 ${logfname} | grep "$search_string"'$'
if ( $status != 0 ) then
    echo set_aida_lcls_ca.sql failed for aidadev. >>! /tmp/set_aida_lcls_ca_aidadev.log
    echo check ${logfname}
    echo set_aida_lcls_ca.sql failed for aidadev | Mail -s "aidadev set_aida_lcls_ca.sql failed" controls-system-admins@slac.stanford.edu
    echo set_aida_lcls_ca.sql failed for aidadev | Mail -s "aidadev set_aida_lcls_ca.sql failed" controls-software-reports@slac.stanford.edu
    echo set_aida_lcls_ca.sql failed for aidadev | Mail -s "aidadev set_aida_lcls_ca.sql failed" poonam@slac.stanford.edu

    if ( -e /nfs/slac/g/cd/log/oracle/set_aida_lcls_ca_slacprod_aidadev.log ) then
       cat /nfs/slac/g/cd/log/oracle/set_aida_lcls_ca_slacprod_aidadev.log | Mail -s "set_aida_lcls_ca_slacprod_aidadev.log errors and warnings" controls-system-admins@slac.stanford.edu controls-software-reports@slac.stanford.edu poonam@slac.stanford.edu
    endif
    exit 1
endif

if ( -e /nfs/slac/g/cd/log/oracle/set_aida_lcls_ca_slacprod_aidadev.log ) then
  cat  /nfs/slac/g/cd/log/oracle/set_aida_lcls_ca_slacprod_aidadev.log  | grep -i error >& /dev/null
  if  ($? == 0) then
     cat /nfs/slac/g/cd/log/oracle/set_aida_lcls_ca_slacprod_aidadev.log | Mail -s "set_aida_lcls_ca_slacprod_aidadev.log errors and warnings" controls-system-admins@slac.stanford.edu controls-software-reports@slac.stanford.edu poonam@slac.stanford.edu

  endif
else
  echo "file does not exist"
endif

set logfname = /nfs/slac/g/cd/log/aidadb/set_aida_lcls_ca_aidaprod.log

setenv TNS_ADMIN /afs/slac/g/lcls/tools/oracle/wallets/aidaprod
sqlplus /@$TWO_TASK @set_aida_lcls_ca.sql EPICS_lcls Y >> ${logfname}
########sqlplus aidaprod/`getPwd aidaprod` @set_aida_lcls_ca.sql EPICS_lcls Y >> ${logfname}
tail -7 ${logfname} | grep "$search_string"'$'
if ( $status != 0 ) then
    echo set_aida_lcls_ca.sql failed for aidaprod. >>! /tmp/set_aida_lcls_ca_aidaprod.log
    echo check ${logfname}
    echo set_aida_lcls_ca.sql failed for aidaprod | Mail -s "aidaprod set_aida_lcls_ca.sql failed" controls-system-admins@slac.stanford.edu
    echo set_aida_lcls_ca.sql failed for aidaprod | Mail -s "aidaprod set_aida_lcls_ca.sql failed" controls-software-reports@slac.stanford.edu
    echo set_aida_lcls_ca.sql failed for aidaprod | Mail -s "aidaprod set_aida_lcls_ca.sql failed" poonam@slac.stanford.edu
    if ( -e /nfs/slac/g/cd/log/oracle/set_aida_lcls_ca_slacprod_aidaprod.log ) then
       cat /nfs/slac/g/cd/log/oracle/set_aida_lcls_ca_slacprod_aidaprod.log | Mail -s "set_aida_lcls_ca_slacprod_aidaprod.log errors and warnings" controls-system-admins@slac.stanford.edu controls-software-reports@slac.stanford.edu poonam@slac.stanford.edu
    endif
    exit 1
else
    echo set_aida_lcls_ca.sql aidaprod finished | Mail -s "aidaprod set_aida_lcls_ca.sql completed" controls-system-admins@slac.stanford.edu
    echo set_aida_lcls_ca.sql aidaprod finished | Mail -s "aidaprod set_aida_lcls_ca.sql completed" controls-software-reports@slac.stanford.edu 
    echo set_aida_lcls_ca.sql aidaprod finished | Mail -s "aidaprod set_aida_lcls_ca.sql completed" poonam@slac.stanford.edu 
endif

if ( -e /nfs/slac/g/cd/log/oracle/set_aida_lcls_ca_slacprod_aidaprod.log ) then
  cat  /nfs/slac/g/cd/log/oracle/set_aida_lcls_ca_slacprod_aidaprod.log  | grep -i error >& /dev/null
  if  ($? == 0) then

     cat /nfs/slac/g/cd/log/oracle/set_aida_lcls_ca_slacprod_aidaprod.log | Mail -s "set_aida_lcls_ca_slacprod_aidaprod.log errors and warnings" controls-system-admins@slac.stanford.edu controls-software-reports@slac.stanford.edu poonam@slac.stanford.edu

  endif
else
  echo "file does not exist"
endif

setenv TWO_TASK MCCQA
set logfname = /nfs/slac/g/cd/log/aidadb/set_aida_lcls_ca_aidalcls.log
setenv TNS_ADMIN /afs/slac/g/lcls/tools/oracle/wallets/aida
sqlplus /@$TWO_TASK @set_aida_lcls_ca.sql EPICS_lcls Y >> ${logfname}
#####sqlplus aida/`getPwd aida` @set_aida_lcls_ca.sql EPICS_lcls Y >> ${logfname}
tail -7 ${logfname} | grep "$search_string"'$'
if ( $status != 0 ) then
    echo set_aida_lcls_ca.sql failed for aidalcls. >>! /tmp/set_aida_lcls_ca_aidalcls.log
    echo check ${logfname}
    echo set_aida_lcls_ca.sql failed for aidalcls | Mail -s "aidalcls set_aida_lcls_ca.sql failed" controls-system-admins@slac.stanford.edu
    echo set_aida_lcls_ca.sql failed for aidalcls | Mail -s "aidalcls set_aida_lcls_ca.sql failed" controls-software-reports@slac.stanford.edu
    echo set_aida_lcls_ca.sql failed for aidalcls | Mail -s "aidalcls set_aida_lcls_ca.sql failed" poonam@slac.stanford.edu
    if ( -e /nfs/slac/g/cd/log/oracle/set_aida_lcls_ca_mccqa_aida.log ) then
       cat /nfs/slac/g/cd/log/oracle/set_aida_lcls_ca_mccqa_aida.log | Mail -s "set_aida_lcls_ca_mccqa_aida.log errors and warnings" controls-system-admins@slac.stanford.edu controls-software-reports@slac.stanford.edu poonam@slac.stanford.edu
    endif
    exit 1
else
    echo set_aida_lcls_ca.sql aidalcls finished | Mail -s "aidalcls set_aida_lcls_ca.sql completed" controls-system-admins@slac.stanford.edu
    echo set_aida_lcls_ca.sql aidalcls finished | Mail -s "aidalcls set_aida_lcls_ca.sql completed" controls-software-reports@slac.stanford.edu
    echo set_aida_lcls_ca.sql aidalcls finished | Mail -s "aidalcls set_aida_lcls_ca.sql completed" poonam@slac.stanford.edu
endif

if ( -e /nfs/slac/g/cd/log/oracle/set_aida_lcls_ca_mccqa_aida.log ) then
  cat  /nfs/slac/g/cd/log/oracle/set_aida_lcls_ca_mccqa_aida.log  | grep -i error >& /dev/null
  if  ($? == 0) then
     cat /nfs/slac/g/cd/log/oracle/set_aida_lcls_ca_mccqa_aida.log | Mail -s "set_aida_lcls_ca_mccqa_aida.log errors and warnings" controls-system-admins@slac.stanford.edu controls-software-reports@slac.stanford.edu poonam@slac.stanford.edu

  endif
else
  echo "file does not exist"
endif


