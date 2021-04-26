# Write a SQL script where each record executes switch_service.sql, to switch
# the AIDA service of a given instance attribute.
#
# Use this script when devices such as magnets or BPMs switch from SLC to EPICS
# and hence from micro:primary naming primary:micro naming in XAL model
#
# Usage:  awk -f makeSwitcher.awk < corrs.txt > switch_correctors.sql
# where corrs.txt is a file of the form: 
# 
# YOU MAY HAVE TO EDIT THIS DEPENDING ON EXACT FORM OF INPUT 
# XCOR:LI24:202
# XCOR:LI24:302
# ...
#
# BEGIN { FS = "'" } - if quoted strings
/^BPMS|^QUAD|^XCOR|^YCOR/ { # print $2 
             printf("@switch_service %s twiss 202\n",$1);
             printf("@switch_service %s R 202\n",$1);
	    }
            
