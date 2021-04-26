#!/usr/local/bin/perl5
#
#==============================================================
#
#  Abs:  This script creates a file containing Aida instance/
#        attribute pairs for the Aida utility server.  This
#        created instance/attribute file is read by the
#        set_aida_slc_utility.sql PL/SQL script to populate
#        the aida_names and aida_directory for this server.
#
#  Name: create_aida_utility_load_file.pl
#
#  Rem:  This script reads the file trignames.dmp from the 
#        /nfs/slac/g/cd/oracle/aida directory and creates the
#        output file aida_utility_load.dat in this directory.
#        The input file trignames.dmp is created as a result
#        of running the VMS dbdump utility in the VMS command
#        script slccom:slcdb2oracle.submit, which creates this
#        file in the NFS_NDB directory.  This directory is mapped to
#        the UNIX /mccdev/nfs_slcdb directory, which is visible
#        from UNIX machines such as slcs2.  The UNIX script
#        run_pmu_load.sh is run later by the VMS command script
#        slccom:slcdb2oracle.submit during a DB install and the
#        run_pmu_load.sh script invokes the
#        slc_utility_populate_aida_database.sh script.  This
#        latter script copies the trignames.dmp file from the
#        /mccdev/nfs_slcdb directory to the
#        /nfs/slac/g/cd/orcle/aida directory and then invokes
#        this Perl script. 
#
#        The output file aida_utility_load.dat contains Aida
#        instance/attribute pairs that are read by the
#        set_aida_slc_utility.sql PL/SQL script to populate
#        the aida_names and aida_directory Oracle tables for
#        the Aida utility server.  This PL/SQL script is also
#        invoked by the slc_utility_populate_aida_database.sh
#        script.
#
#  Facility:  SLAC
#
#  Auth: 27-Jul-2007, Bob Hall (RDH)
#  Rev:  dd-mmm-yyyy, Reviewer's Name    (USERNAME)
#
#--------------------------------------------------------------
#
#  Mod:
#        10-Aug-2007, Bob Hall (RDH)
#           Added generation of instance/attribute pair
#           BGRP/VAL for new BGRP set variable functionality.
#
#==============================================================
#
#  Open the TRIG device names input file trignames.dmp.
#
$input_file = "/nfs/slac/g/cd/oracle/aida/trignames.dmp";

open(INPUT_FILE, $input_file) ||
    die "Can't open input file $input_file";

#$output_file = "aida_utility_load.dat";
$output_file = "/nfs/slac/g/cd/oracle/aida/aida_utility_load.dat";

open(OUTPUT_FILE, ">$output_file") ||
    die "Can't open output file $output_file";

#
# Loop through each line of the input file, which is formatted
# by the VMS dbdump utility.  An example of this format is:
#
# TRIG:LI00,100
#    NAME DRTC 0 
#
# That is, a primary/micro/unit number followed by lines containing
# the channel names for the unit. 
#

while (<INPUT_FILE>)
{
    chomp;

    $line = $_;

#
#   Blank lines in the input file are ignored.
#
    if ($line ne "")
    {
        if ($line =~ m/TRIG:(.*)/)
        {
#
#           The current line contains a trigger device micro name and unit number,
#           which is extracted in parameter $1.  First, this parameter is
#           assigned to the cur_micro_unit variable and trailing whitespace
#           is removed.  The micro name and unit number, which are separated
#           by a comma, are split into micro name and unit number strings.
#           Then the Aida instance name (the primary, micro, and unit strings,
#           separated by colons) is formed.  Finally, the Aida instance/attribute
#           name pair (the attribute is the string "TACT") is written to the
#           output file. 
#
#
            $cur_micro_unit = $1;

            $cur_micro_unit =~s/[ \t]+$//;

            #print "cur_micro_unit = $cur_micro_unit\n";

            ($cur_micro, $cur_unit) = split(/,/, $cur_micro_unit);

            $prim_micro_unit = "TRIG:" . $cur_micro . ":" . $cur_unit;

            #print "prim_micro_unit = $prim_micro_unit\n";

            print OUTPUT_FILE ($prim_micro_unit . "\n");
            print OUTPUT_FILE ("TACT\n");
        } 
    }
}

close(INPUT_FILE);

#
# Finally, output the instance/attribute pairs MKB/VAL and
# BGRP/VAL. 
#
print OUTPUT_FILE ("MKB\n");
print OUTPUT_FILE ("VAL\n");

print OUTPUT_FILE ("BGRP\n");
print OUTPUT_FILE ("VAL\n");

close(OUTPUT_FILE);
