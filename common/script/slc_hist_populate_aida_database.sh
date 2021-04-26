#! /bin/tcsh -f
#
# Abs:   This script populates the Aida Oracle database tables from PV
#        information stored in history buffer files.
#
# Name:  slc_hist_populate_aida_database.sh  
#
# Rem:   After a database install, the VMS command script
#        slccom:slcdb2oracle.submit is invoked, which among other
#        things copies the dev$hist:[history]device_sequ.dat and
#        prod$hist:[history]device_sequ.dat files to
#        NFD_NDB:dev_device_sequ.dat and NFD_NDB:prod_device_sequ.dat,
#        respectively.  The VMS NFS_NDB area is mapped to the UNIX
#        /mccdev/nfs_slcdb area, which is visable from such UNIX machines
#        as slcs2.  Later, this VMS command script invokes the UNIX
#        run_pmu_load.sh script on the slcs2 machine using the command
#        server mechanism.  The UNIX run_pmu_load.sh script invokes this
#        script. 
#
#  Facility:  SLAC
#
#  Auth: 05-Aug-2005, Bob Hall (rdh)
#  Rev:  dd-mmm-yyyy, Reviewer's Name    (USERNAME)
#--------------------------------------------------------------
#
#  Mod:
#        04-Sep-2012, brobeck
#          Changed rdh mail to controls-system-admins
#        20-Sep-2011, Bob Hall (rdh)
#          Changed Oracle password. 
#        04-Apr-2011, Bob Hall (rdh)
#          Changed Oracle password. 
#        15-Sep-2010, Bob Hall (rdh)
#          Changed Oracle password. 
#        23-Mar-2010, Bob Hall (rdh)
#          Changed Oracle password. 
#        26-Aug-2009, Bob Hall (rdh)
#          Changed Oracle password. 
#        23-Apr-2009, Bob Hall (rdh)
#           Modified to generate set_aida_slc_hist log
#           files in /nfs/slac/g/cd/log/aidadb.  Also modified to
#           send mail upon the completion of running
#           set_aida_slc_hist.sql for the aidaprod account.
#        09-Mar-2009, Bob Hall (rdh)
#          Changed Oracle password. 
#        25-Aug-2008, Bob Hall (rdh)
#          Changed Oracle password. 
#        13-Feb-2008, Bob Hall (rdh)
#          Changed Oracle password. 
#        21-Jun-2007, Bob Hall (rdh)
#          Changed Oracle password. 
#        16-Apr-2006, Bob Hall (rdh)
#          Added source of ENVS and additions to ORACLE_PATH.
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
# First copy the dev_device_sequ.dat and prod_device_sequ.dat into the
# /nfs/slac/g/cd/oracle/aida directory.
# 
cp /mccdev/nfs_slcdb/dev_device_sequ.dat /nfs/slac/g/cd/oracle/aida
cp /mccdev/nfs_slcdb/prod_device_sequ.dat /nfs/slac/g/cd/oracle/aida

#
# The Aida development Oracle database will be populated first with
# SLC history buffer file PV information.  Run the convert_hist_buf_file.pl
# Perl script to convert the dev_device_sequ.dat history buffer file
# to /nfs/slac/g/cd/oracle/aida/converted_device_sequ.dat, which will
# contain Aida instance/attribute pairs.
#
convert_hist_buf_file.pl dev

#
# Invoke the set_aida_slc_hist.sql PL/SQL script to read the file containing
# instance/attribute pairs generated by the convert_hist_buf_file.pl Perl
# script and populate the appropriate tables in the Aida aidadev
# Oracle account.
#
set search_string = "errcode: 0"

set logfname = /nfs/slac/g/cd/log/aidadb/set_aida_slc_hist_aidadev.log
setenv TNS_ADMIN /afs/slac/g/lcls/tools/oracle/wallets/aidadev
sqlplus /@$TWO_TASK @set_aida_slc_hist.sql >> ${logfname}
####sqlplus aidadev/gver3d_i @set_aida_slc_hist.sql >> ${logfname}
tail -6 ${logfname} | grep "$search_string"'$'
if ( $status != 0 ) then
    echo set_aida_slc_hist.sql failed for aidadev. >>! /tmp/set_aida_slc_hist_aidadev.log
    echo check ${logfname}
    echo set_aida_slc_hist.sql failed for aidadev | Mail -s "aidadev set_aida_slc_hist.sql failed" controls-system-admins@slac.stanford.edu
    exit 1
endif

#
# Populate the Aida production Oracle database with SLC history buffer
# file PV information in the same manner as was done for the Aida
# development Oracle database.
#
convert_hist_buf_file.pl prod

set logfname = /nfs/slac/g/cd/log/aidadb/set_aida_slc_hist_aidaprod.log
setenv TNS_ADMIN /afs/slac/g/lcls/tools/oracle/wallets/aidaprod
sqlplus /@$TWO_TASK @set_aida_slc_hist.sql >> ${logfname}

####sqlplus aidaprod/gver3d_i @set_aida_slc_hist.sql >> ${logfname}
tail -6 ${logfname} | grep "$search_string"'$'
if ( $status != 0 ) then
    echo set_aida_slc_hist.sql failed for aidaprod. >>! /tmp/set_aida_slc_hist_aidaprod.log
    echo check ${logfname}
    echo set_aida_slc_hist.sql failed for aidaprod | Mail -s "aidaprod set_aida_slc_hist.sql failed" controls-system-admins@slac.stanford.edu
    exit 1
else
    echo set_aida_slc_hist.sql aidaprod finished | Mail -s "aidaprod set_aida_slc_hist.sql completed" controls-system-admins@slac.stanford.edu
endif

# # Populate the Aida LCLS MCCO Oracle database with SLC history buffer
# file PV information in the same manner as above 
setenv TWO_TASK MCCQA
set logfname = /nfs/slac/g/cd/log/aidadb/set_aida_slc_hist_aidalcls.log
setenv TNS_ADMIN /afs/slac/g/lcls/tools/oracle/wallets/aida
sqlplus /@$TWO_TASK @set_aida_slc_hist.sql >> ${logfname}
####sqlplus aida/gver3d_i @set_aida_slc_hist.sql >> ${logfname}
tail -6 ${logfname} | grep "$search_string"'$'
if ( $status != 0 ) then
    echo set_aida_slc_hist.sql failed for aidalcls. >>! /tmp/set_aida_slc_hist_aidalcls.log
    echo check ${logfname}
    echo set_aida_slc_hist.sql failed for aidalcls | Mail -s "aidalcls set_aida_slc_hist.sql failed" controls-system-admins@slac.stanford.edu
    exit 1
else
    echo set_aida_slc_hist.sql aidalcls finished | Mail -s "aidalcls set_aida_slc_hist.sql completed" controls-system-admins@slac.stanford.edu
endif
