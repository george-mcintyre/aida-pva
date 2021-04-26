#! /bin/tcsh -f
#
# Abs: Sets CVS (code versioning system) options for Aida project
#
# Usage: > aidaSetCVS.sh
#
# Sys: unix
#
# Rem: 
#
# Auth: 27-Sep-2001 Greg White
#
# -----------------------------------------------------------------------------
# Mod:  15-Auf-2002, Greg White (GREG):
#       Removed script path and replaced $AIDA with $CD_SOFT root,
#       for first instance in CD_SOFT.
#       10-Jun-2002, Ron MacKenzie (RONM):
#       Set the location of  the cvs script utilities for those of
#       us that like to use them.   
# =============================================================================
setenv CVSROOT ${CD_SOFT}/cvs
setenv CVSIGNORE "*~ *.class"
setenv CVSEDITOR "emacs -nw"
