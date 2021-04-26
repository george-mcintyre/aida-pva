#-*-csh-*-
#==============================================================
#
#  Abs:   Converts path dierctories of R3.13 to R3.14 for Aida Dp Ca.
#
#  Usage: source $CD_SOFT/dev/script/R13toR14.csh
#         
#  Rem:   Relies on tools defined in $CD_SOFT/dev/script/ENVS.csh
# 
#  This script replaces EPICS R3.13 library and executables in the
#  environment, set by eg $CD_SOFT/dev/script/ENVS.csh, to R3.14
#  locations.  "Why don't you just use a R3.14 setup?" enquiring minds
#  may ask.  Ans: 1) because so far the default setup most people have
#  in their .login is ENVS.csh, which does an epicsSetupPepii for you,
#  which sets R3.13.6.  2) ENVS.csh is needed for aida development. So,
#  ideally, ENVS.csh would not do an epicsSetupPepii, and I would
#  instead source some r3.14 setup.
#
#  Side:  
#
#  Auth:  10-Mar-2005, greg White (greg) 
#  Rev:   dd-mmm-yyyy, Whoever    (user):
#
#--------------------------------------------------------------
# Mods: (Latest to oldest)
#
#============================================================== 

# LD_LIBRARY_PATH
# extensions
delpath LD_LIBRARY_PATH /afs/slac/package/epics/R3.13.6/extensions/lib/solaris
caddpath LD_LIBRARY_PATH /afs/slac/package/epics/R3.14.6/extensions/lib/solaris-sparc
# site
delpath LD_LIBRARY_PATH /afs/slac/package/epics/R3.13.6/site/lib/solaris
caddpath LD_LIBRARY_PATH /afs/slac/package/epics/R3.14.6/site/lib/solaris-sparc
# base
delpath LD_LIBRARY_PATH /afs/slac/package/epics/R3.13.6/base/lib/solaris
caddpath LD_LIBRARY_PATH /afs/slac/package/epics/R3.14.6/base/lib/solaris-sparc

# PATH 
# extensions
delpath PATH /afs/slac/package/epics/R3.13.6/extensions/bin/solaris
caddpath PATH /afs/slac/package/epics/R3.14.6/extensions/bin/solaris-sparc
# base
delpath PATH /afs/slac/package/epics/R3.13.6/base/bin/solaris
caddpath PATH /afs/slac/package/epics/R3.14.6/base/bin/solaris-sparc
# site
delpath PATH /afs/slac/package/epics/R3.13.6/site/bin/solaris
caddpath PATH /afs/slac/package/epics/R3.14.6/site/bin/solaris-sparc
rehash

# CLASSPATH
delpath CLASSPATH /afs/slac/package/epics/R3.13.6/extensions/javalib
caddpath CLASSPATH /afs/slac/package/epics/R3.14.6/extensions/javalib/jca.jar




