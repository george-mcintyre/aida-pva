#!/bin/bash -f
#==============================================================
#
#  Abs: Process for periodically checking the status of Aida
#       data providers and so the status can be indicated on
#       the UNIX Watchdog display.
#
#  Name: aidaDpServerStatus.bash 
#
#  Usage: Invoked by a system "type 1" startup file when the
#         machine it runs on is booted.  This script does not
#         terminate but sleeps at a specified interval (e.g.,
#         5 minutes) between checking the status of each Aida
#         data provider.
#
#         This script writes to UNIX Watchdog PVs to indicate
#         the status of each Aida data provider.
#
#  Facility:  SLAC/LCLS/PEPII/NLCTA
#
#  Auth: 06-Feb-2008, Bob Hall (rdh)
#--------------------------------------------------------------
#
#  Mod:  (latest to oldest)
#        02-Feb-2011, Bob Hall (rdh)
#           Added support for new FACET Channel Archiver server.
#        29-Jul-2008, Bob Hall (rdh)
#           Replaced
#               blank=" "
#               beamratePos=`expr index "$beamrate" "$blank"`
#           with
#               beamratePos=`echo $beamrate | sed -n 's/[ ].*//p' | wc -c`
#           to find the position of the first blank in variable beamrate
#           because the former is Linux-specific and the latter is
#           portable (e.g., will run on lcls-uwd, which is Solaris).
#        29-Jul-2008, Bob Hall (rdh)
#           Modified to perform a caget of a PV indicating the
#           beam rate when the Aida SLC BPM server is unable to
#           retrieve data successfully.  If the beam rate is 0
#           at this time, the Aida SLC BPM server is expected to
#           be unable to retrieve data successfully so a bad
#           status indication is not written.
#        21-Jul-2008, Bob Hall (rdh)
#           Added support for new Aida Model server.  Also
#           changed to explicitly set the environment variable
#           JAVAVER to 1.5, which Aida uses now. 
#        26-Jun-2008, Bob Hall (rdh)
#           Added support for new Aida Rdb server.
#        04-Jun-2008, Bob Hall (rdh)
#           Modified to be able to be able to do caget and
#           caput operations for UNIX Watchdog status PVs
#           using the new UNIX Watchdog system.  First, changed
#           /afs/slac/g/cd/soft/ref/common/sys-admin/watchdog_setup.sh
#           to /afs/slac/g/lcls/tools/uwd/script/watchdog_setup
#           for setting up EPICS envinronment variables.
#           Secondly, changed the names of VMS Aida server
#           status PVs at the request of Ken Brobeck to be
#           prefixed with "AIDA:" rather than "MCC:". 
#
#==============================================================
#

#
# Invoke the script that sets up the EPICS environment variables
# for writing to UNIX Watchdog status PVs.
#
. /afs/slac/g/lcls/tools/uwd/script/watchdog_setup

#
# Setup Aida environment variables for invoking the Aida Java status
# test programs AidaSlcDpServerStatus and AidaUnixDpServerStatus.
# An alternative to setting these environment variables explicitly
# would be to create a Bash version of the C shell nonexecutable
# script aidaSetEnvDev.sh.
#
AIDA_MODE=PROD
export AIDA_MODE

JAVAVER=1.5
export JAVAVER 

AIDA_NAMESERVER_IOR_URL=http://mccas0.slac.stanford.edu/aida/NameServer$AIDA_MODE.ior
export AIDA_NAMESERVER_IOR_URL

AIDA_NAMESERVER_IOR_FILE=/nfs/slac/g/cd/www/aida/NameServer$AIDA_MODE.ior
export AIDA_NAMESERVER_IOR_FILE

AIDA_NAME_SERVER_REF_FILE=/nfs/slac/g/cd/www/aida/NameServer$AIDA_MODE.ior
export AIDA_NAME_SERVER_REF_FILE

AIDA_CLIENT_TIMEOUT=60
export AIDA_CLIENT_TIMEOUT

#
# Add to the PATH environment variable the path for the caRepeater process.
#
export PATH=$PATH:/afs/slac/package/epics/R3.14.6/base/bin/solaris-sparc

#
# Set the CLASSPATH environment variables to the standard setup for the
# invoked Aida Java test status programs.  These use the Aida package
# classes, the err and except classes, and the Orbacus CORBA jar files.
#
CLASSPATH=.:/afs/slac/g/cd/soft/dev/javalib:/afs/slac/g/cd/soft/new/javalib:/afs/slac/g/cd/soft/prod/javalib:/afs/slac/package/epics/R3.14.6/base/javalib:/afs/slac/package/epics/R3.14.6/site/javalib:/afs/slac/package/epics/R3.14.6/extensions/javalib:/afs/slac/package/epics/R3.14.6/extensions/javalib/vdct/VisualDCT.jar:/afs/slac/package/iona/orbacus/prod/JOB/lib/OB.jar:/afs/slac/package/iona/orbacus/prod/JOB/lib/OBEvent.jar:/afs/slac/package/iona/orbacus/prod/JOB/lib/OBUtil.jar:/afs/slac/package/cmlog/new/java/classes:/afs/slac/g/cd/soft/ref/package/aida:/afs/slac/g/cd/soft/ref/package/err:/afs/slac/g/cd/soft/ref/package/except

export CLASSPATH

#
# Loop forever, sleeping at the end of the loop for a specified time interval
# (e.g., 5 minutes).
#
while true
do

#
# Invoke the AidaSlcDpServerStatus Aida Java test status program to determine
# the status of the 8 Aida SLC (VMS) data providers.  The returned status code
# indicates which Aida SLC data providers are functioning correctly.  Each
# bit in the returned 8 bit status code indicates whether an Aida SLC data
# provider is functioning correctly (value "0") or not (value "1").  An Aida
# Java test status program cannot indicate the status of more than 8 processes
# since the status code contains only 8 bits (values 0-255).
#
    java ${AIDABCSTRING} -DAIDA_DATABASE_USERID=AIDA${AIDA_MODE} -DAIDA_NAMESERVER_IOR_URL=${AIDA_NAMESERVER_IOR_URL} -DAIDA_CLIENT_TIMEOUT=${AIDA_CLIENT_TIMEOUT} test.java.AidaSlcDpServerStatus

    exitValue=$?

    echo "AidaSlcDpServerStatus Exit Code: " $exitValue

    remExitValue=$exitValue

#
# The returned status code is decoded by checking for values that are powers of
# 2 (starting with 128) and subtracting those values from the current remainder
# status code whose values are greater than or equal to its current value.
# The value "1" is written to the UNIX watchdog PV for an Aida SLC data provider
# if the data provider is not functioning correctly.  If it is functioning
# correctly, the value "0" is written to it.
# 
    if [ $remExitValue -ge 128 ]
    then
        remExitValue=$(( $remExitValue - 128 ))
        echo "SLC Model server not functioning"
        caput AIDA:AIDA:MODEL:STATUS 1
    else
        caput AIDA:AIDA:MODEL:STATUS 0
    fi

    if [ $remExitValue -ge 64 ]
    then
        remExitValue=$(( $remExitValue - 64 ))
        echo "SLC Klystron server not functioning"
        caput AIDA:AIDA:KLYS:STATUS 1
    else
        caput AIDA:AIDA:KLYS:STATUS 0
    fi

    if [ $remExitValue -ge 32 ]
    then
        remExitValue=$(( $remExitValue - 32 ))
        echo "SLC Master Oscillator server not functioning"
        caput AIDA:AIDA:MOSC:STATUS 1
    else
        caput AIDA:AIDA:MOSC:STATUS 0
    fi

    if [ $remExitValue -ge 16 ]
    then
        remExitValue=$(( $remExitValue - 16 ))
        echo "SLC Utility server not functioning"
        caput AIDA:AIDA:UTIL:STATUS 1
    else
        caput AIDA:AIDA:UTIL:STATUS 0
    fi

    if [ $remExitValue -ge 8 ]
    then
        remExitValue=$(( $remExitValue - 8 ))
        echo "SLC Magnet server not functioning"
        caput AIDA:AIDA:MGNT:STATUS 1
    else
        caput AIDA:AIDA:MGNT:STATUS 0
    fi

    if [ $remExitValue -ge 4 ]
    then
        remExitValue=$(( $remExitValue - 4 ))

#
#       Check the LCLS beam rate.  If it is equal to 0,
#       the SLC Bpm server is not functioning because there
#       is no beam so do not write a bad status indication.
#
        beamrate=`caget EVNT:SYS0:1:LCLSBEAMRATE`

#        echo "beamrate = $beamrate"
#        echo "${beamrate} is ${#beamrate} characters long"

        if [ ${#beamrate} -gt 0 ]
        then 
#
#           The value of the variable beamrate is the string
#           "EVNT:SYS0:1:LCLSBEAMRATE       0 " if the LCLS
#           beam rate is 0.  To extract the beam rate, first
#           find the position of the first blank.  Then extract
#           the substring beginning at that first blank (to the
#           end of the string).  Then loop through each
#           character of that substring, looking for digit
#           characters.  For each digit character found, concatenate
#           it to the end of the beamrateValue string.  This
#           results in the beamrateValue string containing the
#           value of the caget of the LCLS beam rate PV. 
#
            beamratePos=`echo $beamrate | sed -n 's/[ ].*//p' | wc -c`
#            echo "position = $beamratePos"

            beamrateSubstr=${beamrate:$beamratePos-1}
#            echo "${beamrateSubstr} is ${#beamrateSubstr} characters long"

            beamrateValue=""
            for ((i=0;i<${#beamrateSubstr};i+=1)); do
                beamrateChar=${beamrateSubstr:$i:1}

                if [ "$beamrateChar" = "0" ]; then
                    beamrateValue=$beamRateValue$beamrateChar
                elif [ "$beamrateChar" = "1" ]; then
                    beamrateValue=$beamRateValue$beamrateChar
                elif [ "$beamrateChar" = "2" ]; then
                    beamrateValue=$beamRateValue$beamrateChar
                elif [ "$beamrateChar" = "3" ]; then
                    beamrateValue=$beamRateValue$beamrateChar
                elif [ "$beamrateChar" = "4" ]; then
                    beamrateValue=$beamRateValue$beamrateChar
                elif [ "$beamrateChar" = "5" ]; then
                    beamrateValue=$beamRateValue$beamrateChar
                elif [ "$beamrateChar" = "6" ]; then
                    beamrateValue=$beamRateValue$beamrateChar
                elif [ "$beamrateChar" = "7" ]; then
                    beamrateValue=$beamRateValue$beamrateChar
                elif [ "$beamrateChar" = "8" ]; then
                    beamrateValue=$beamRateValue$beamrateChar
                elif [ "$beamrateChar" = "9" ]; then
                    beamrateValue=$beamRateValue$beamrateChar
                fi
            done

#            echo "beamrateValue = $beamrateValue"

            if [ $beamrateValue -gt 0 ]
            then
                echo "SLC Bpm server not functioning"
                caput AIDA:AIDA:BPM:STATUS 1
            else
                echo "SLC Bpm server not functioning because of no beam"
                caput AIDA:AIDA:BPM:STATUS 0
            fi
        else
            echo "SLC Bpm server not functioning"
            caput AIDA:AIDA:BPM:STATUS 1
        fi
    else
        caput AIDA:AIDA:BPM:STATUS 0 
    fi

    if [ $remExitValue -ge 2 ]
    then
        remExitValue=$(( $remExitValue - 2 ))
        echo "SLC History server not functioning"
        caput AIDA:AIDA:HIST:STATUS 1
    else
        caput AIDA:AIDA:HIST:STATUS 0
    fi

    if [ $remExitValue -ge 1 ]
    then
        remExitValue=$(( $remExitValue - 1 ))
        echo "SLC Database server not functioning"
        caput AIDA:AIDA:DB:STATUS 1
    else
        caput AIDA:AIDA:DB:STATUS 0
    fi

#
# Invoke the AidaUnixDpServerStatus Aida Java test status program to determine
# the status of the 5 Aida UNIX data providers.  The returned status code
# indicates which Aida UNIX data providers are functioning correctly.
#
    java ${AIDABCSTRING} -DAIDA_DATABASE_USERID=AIDA${AIDA_MODE} -DAIDA_NAMESERVER_IOR_URL=${AIDA_NAMESERVER_IOR_URL} -DAIDA_CLIENT_TIMEOUT=${AIDA_CLIENT_TIMEOUT} test.java.AidaUnixDpServerStatus

    exitValue=$?

    echo "AidaUnixDpServerStatus Exit Code: " $exitValue

    remExitValue=$exitValue

#
# The returned status code is decoded by checking for values that are powers of
# 2 (starting with 16) and subtracting those values from the current remainder
# status code whose values are greater than or equal to its current value.
# The value "1" is written to the UNIX watchdog PV for an Aida UNIX data provider
# if the data provider is not functioning correctly.  If it is functioning
# correctly, the value "0" is written to it.
#
    if [ $remExitValue -ge 128 ]
    then
        remExitValue=$(( $remExitValue - 128 ))
        echo "FACET Channel Archiver server not functioning"
        caput MCCAS0:FACET:ARCH:STATUS 1
    else
        caput MCCAS0:FACET:ARCH:STATUS 0
    fi

    if [ $remExitValue -ge 64 ]
    then
        remExitValue=$(( $remExitValue - 64 ))
        echo "Aida Model server not functioning"
        caput MCCAS0:AIDA:MODEL:STATUS 1
    else
        caput MCCAS0:AIDA:MODEL:STATUS 0
    fi

    if [ $remExitValue -ge 32 ]
    then
        remExitValue=$(( $remExitValue - 32 ))
        echo "Aida Rdb server not functioning"
        caput MCCAS0:AIDA:RDB:STATUS 1
    else
        caput MCCAS0:AIDA:RDB:STATUS 0
    fi
 
    if [ $remExitValue -ge 16 ]
    then
        remExitValue=$(( $remExitValue - 16 ))
        echo "PEPII Channel Archiver server not functioning"
        caput MCCAS0:PEPII:ARCH:STATUS 1
    else
        caput MCCAS0:PEPII:ARCH:STATUS 0
    fi

    if [ $remExitValue -ge 8 ]
    then
        remExitValue=$(( $remExitValue - 8 ))
        echo "NLCTA Channel Archiver server not functioning"
        caput CS04:NLCTA:ARCH:STATUS 1
    else
        caput CS04:NLCTA:ARCH:STATUS 0
    fi

    if [ $remExitValue -ge 4 ]
    then
        remExitValue=$(( $remExitValue - 4 ))
        echo "LCLS Channel Archiver server not functioning"
        caput MCCAS0:LCLS:ARCH:STATUS 1
    else
        caput MCCAS0:LCLS:ARCH:STATUS 0
    fi

    if [ $remExitValue -ge 2 ]
    then
        remExitValue=$(( $remExitValue - 2 ))
        echo "Non-LCLS Channel Access server not functioning"
        caput MCCAS0:PEPII:ACCESS:STATUS 1
    else
        caput MCCAS0:PEPII:ACCESS:STATUS 0
    fi

    if [ $remExitValue -ge 1 ]
    then
        remExitValue=$(( $remExitValue - 1 ))
        echo "LCLS Channel Access server not functioning"
        caput MCCAS0:LCLS:ACCESS:STATUS 1
    else
        caput MCCAS0:LCLS:ACCESS:STATUS 0
    fi

    sleep 300 
done
