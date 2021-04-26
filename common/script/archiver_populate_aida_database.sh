#! /bin/tcsh -f
#
# Abs:   This script populates the Aida Oracle database tables from
#        PVs contained in the archiver configuration files. 
#
# Name:  archiver_populate_aida_database.sh  
#
# Rem:   This script is run daily by trscrontab on mccas0 under
#        the cddev account.  The script first invokes a Perl
#        script (convert_archiver_config_files.pl) that parses
#        the PV names from the Channel Archiver configuration
#        files for the LCLS, FACET, PEPII, and NLCTA subsystems to
#        create files that contain Aida instance/attribute pair
#        lines.  This script then invokes the set_aida_channel_archiver.sql
#        script to populate the dev and prod Aida Oracle database
#        tables from files that contain the Channel Archiver
#        Aida instance/attribute pair lines. 
#
#  Auth: 17-Apr-2007, Bob Hall (rdh)
#  Rev:  dd-mmm-yyyy, Reviewer's Name    (USERNAME)
#--------------------------------------------------------------
#
#  Mod:
#        04-Sep-2012, brobeck
#           Changed rdh to controls-system-admins (mail)
#        04-Apr-2011, Bob Hall (rdh)
#           Changed to get Oracle password using getPwd.
#        02-Feb-2011, Bob Hall (rdh)
#           Modified to support the new FACET Aida Channel Archiver
#           server. 
#        15-Sep-2010, Bob Hall (rdh)
#           Changed Oracle password.
#        23-Mar-2010, Bob Hall (rdh)
#           Changed Oracle password.
#        03-Mar-2010, Greg White (greg)
#           Add support for AIDALCLS
#        26-Aug-2009, Bob Hall (rdh)
#           Changed Oracle password.
#        23-Apr-2009, Bob Hall (rdh)
#           Modified to generate set_aida_channel_archiver log
#           files in /nfs/slac/g/cd/log/aidadb.  Also modified to
#           send mail upon the completion of running
#           set_aida_channel_archiver.sql for the aidaprod account.
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
# Run the convert_archiver_config_files.pl script for each of the
# Channel Archiver subsystems (LCLS, FACET, PEPII, and NLCTA) to create
# the converted_arch_pvs_lcls.dat, converted_arch_pvs_facet.dat,
# converted_arch_pvs_pepii.dat,
# and converted_arch_pvs_nlcta.dat files in the /nfs/slac/g/cd/oracle/aida
# directory that contain Aida instance/attribute pair lines.  
#
convert_archiver_config_files.pl lcls
convert_archiver_config_files.pl facet 
convert_archiver_config_files.pl pepii 
convert_archiver_config_files.pl nlcta 

#
# Invoke the set_aida_channel_archiver.sql PL/SQL script to read the files
# containing instance/attribute pairs for the Channel Archiver subsystems
# (LCLS, FACET, PEPII, and NLCTA) and populate the aida_names and aida_directory
# Oracle tables in the Aida aidadev and aidaprod Oracle accounts.
#
set search_string = "errcode: 0"

set logfname = /nfs/slac/g/cd/log/aidadb/set_aida_channel_archiver_aidadev.log
setenv TNS_ADMIN /afs/slac/g/lcls/tools/oracle/wallets/aidadev
sqlplus /@$TWO_TASK @set_aida_channel_archiver.sql >> ${logfname}
##########sqlplus aidadev/`getPwd aidadev` @set_aida_channel_archiver.sql >> ${logfname}
tail -6 ${logfname} | grep "$search_string"'$'
if ( $status != 0 ) then
    echo set_aida_channel_archiver.sql failed for aidadev. >>! /tmp/set_aida_channel_archiver_aidadev.log
    echo check ${logfname}
    echo set_aida_channel_archiver.sql failed for aidadev | Mail -s "aidadev set_aida_channel_archiver.sql failed" controls-system-admins@slac.stanford.edu
    exit 1
endif

set logfname = /nfs/slac/g/cd/log/aidadb/set_aida_channel_archiver_aidaprod.log
setenv TNS_ADMIN /afs/slac/g/lcls/tools/oracle/wallets/aidaprod
sqlplus /@$TWO_TASK @set_aida_channel_archiver.sql >> ${logfname}
##########sqlplus aidaprod/`getPwd aidaprod` @set_aida_channel_archiver.sql >> ${logfname}
tail -6 ${logfname} | grep "$search_string"'$'
if ( $status != 0 ) then
    echo set_aida_channel_archiver.sql failed for aidaprod. >>! /tmp/set_aida_channel_archiver_aidaprod.log
    echo check ${logfname}
    echo set_aida_channel_archiver.sql failed for aidaprod | Mail -s "aidaprod set_aida_channel_archiver.sql failed" controls-system-admins@slac.stanford.edu
    exit 1
else
    echo set_aida_channel_archiver.sql aidaprod finished | Mail -s "aidaprod set_aida_channel_archiver.sql completed" controls-system-admins@slac.stanford.edu
endif

setenv TWO_TASK MCCQA
set logfname = /nfs/slac/g/cd/log/aidadb/set_aida_channel_archiver_aidalcls.log
setenv TNS_ADMIN /afs/slac/g/lcls/tools/oracle/wallets/aida
sqlplus /@$TWO_TASK @set_aida_channel_archiver.sql >> ${logfname}
#sqlplus aida/`getPwd aida` @set_aida_channel_archiver.sql >> ${logfname}
tail -6 ${logfname} | grep "$search_string"'$'
if ( $status != 0 ) then
    echo set_aida_channel_archiver.sql failed for aidalcls. >>! /tmp/set_aida_channel_archiver_aidalcls.log
    echo check ${logfname}
    echo set_aida_channel_archiver.sql failed for aidalcls | Mail -s "aidalcls set_aida_channel_archiver.sql failed" controls-system-admins@slac.stanford.edu
    exit 1
else
    aida_validate_and_sync_instances.sh "LCLS Archive Names"  >> ${logfname}
    echo set_aida_channel_archiver.sql aidalcls finished | Mail -s "aidalcls set_aida_channel_archiver.sql completed" controls-system-admins@slac.stanford.edu
endif

