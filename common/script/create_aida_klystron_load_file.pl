#!/usr/local/bin/perl5
#
#==============================================================
#
#  Abs:  This script creates a file containing Aida instance/
#        attribute pairs for the Aida klystron server.  This
#        created instance/attribute file is read by the
#        set_aida_slc_klystron.sql PL/SQL script to populate
#        the aida_names and aida_directory Oracle tables for
#        the Aida klystron server.
#
#  Name: create_aida_klystron_load_file.pl
#
#  Rem:  This script reads the files klysnames.dmp and sbstnames.dmp
#        from the /nfs/slac/g/cd/oracle/aida directory and creates 
#        the output file aida_klystron_load.dat in this directory.
#        The input files klysnames.dmp and sbstnames.dmp are created
#        as a result of running the VMS dbdump utility in the VMS command
#        script slccom:slcdb2oracle.submit, which creates these
#        files in the NFS_NDB directory.  This directory is mapped to
#        the UNIX /mccdev/nfs_slcdb directory, which is visible
#        from UNIX machines such as slcs2.  The UNIX script
#        run_pmu_load.sh is run later by the VMS command script
#        slccom:slcdb2oracle.submit during a DB install and the
#        run_pmu_load.sh script invokes the
#        slc_klystron_populate_aida_database.sh script.  This
#        latter script copies the klysnames.dmp and sbstnames.dmp
#        files from the /mccdev/nfs_slcdb directory to the
#        /nfs/slac/g/cd/orcle/aida directory and then invokes
#        this Perl script. 
#
#        The output file aida_klystron_load.dat contains Aida
#        instance/attribute pairs that are read by the
#        set_aida_slc_klystron.sql PL/SQL script to populate
#        the aida_names and aida_directory Oracle tables for
#        the Aida klystron server.  This PL/SQL script is also
#        invoked by the slc_klystron_populate_aida_database.sh
#        script.
#
#  Facility:  SLAC
#
#  Auth: 16-May-2007, Bob Hall (RDH)
#  Rev:  dd-mmm-yyyy, Reviewer's Name    (USERNAME)
#
#--------------------------------------------------------------
#
#  Mod:
#        04-Dec-2007, Bob Hall (RDH)
#           Added processing to create instance/attribute pairs
#           for the KLYS and SBST devices having attribute KPHR.
#        03-Oct-2007, Bob Hall (RDH)
#           Added processing to create instance/attribute pairs
#           for the KLYS and SBST devices having attributes PCON
#           and ACON.
#        23-Aug-2007, Bob Hall (RDH)
#           Added processing to create instance/attribute pairs
#           for the KLYS and SBST devices having attributes PDES
#           and PHAS.  Also removed the creation of the KLYSTRIM/PDES
#           instance/attribute pair.  This was done as part of
#           the work needed to simplify the set operation of the
#           Aida Klystron server for set PDES and trim operations,
#           which requires that KLYS/SBST device secondaries PDES
#           and PHAS be processed by this server rather than the
#           SLC Database server.
#        17-May-2007, Bob Hall (RDH)
#           Fixed to remove trailing whitespace from primary/
#           micro/unit lines.
#
#==============================================================
#
#  Open the klystron names input file klysnames.dmp.
#
$klys_input_file = "/nfs/slac/g/cd/oracle/aida/klysnames.dmp";

open(KLYS_INPUT_FILE, $klys_input_file) ||
    die "Can't open input file $klys_input_file";

#$output_file = "aida_klystron_load.dat";
$output_file = "/nfs/slac/g/cd/oracle/aida/aida_klystron_load.dat";

open(OUTPUT_FILE, ">$output_file") ||
    die "Can't open output file $output_file";

#
# Loop through each line of the klysnames.dmp input file, which is
# formatted by the VMS dbdump utility.  An example of this format is:
#
# KLYS:LI00,1
#    NAME K-INJR 
#
# That is, a primary/micro/unit number followed by lines containing
# the channel names for the unit. 
#

while (<KLYS_INPUT_FILE>)
{
    chomp;

    $line = $_;

#
#   Blank lines in the input file are ignored.
#
    if ($line ne "")
    {
        if ($line =~ m/KLYS:(.*)/)
        {
#
#           The current line contains a klystron micro name and unit number,
#           which is extracted in parameter $1.  First, this parameter is
#           assigned to the cur_micro_unit variable and trailing whitespace
#           is removed.  The micro name and unit number, which are separated
#           by a comma, are split into micro name and unit number strings.
#           Then the Aida instance name (the primary, micro, and unit strings,
#           separated by colons) is formed.  Finally, the Aida instance/attribute
#           name pairs (the attributes are "TACT", "PDES", "KPHR", "PHAS", "PCON",
#           and "ACON") are written to the output file.
#
            $cur_micro_unit = $1;

            $cur_micro_unit =~s/[ \t]+$//;

            #print "cur_micro_unit = $cur_micro_unit\n";

            ($cur_micro, $cur_unit) = split(/,/, $cur_micro_unit);

            $prim_micro_unit = "KLYS:" . $cur_micro . ":" . $cur_unit;

            #print "prim_micro_unit = $prim_micro_unit\n";

            print OUTPUT_FILE ($prim_micro_unit . "\n");
            print OUTPUT_FILE ("TACT\n");

            print OUTPUT_FILE ($prim_micro_unit . "\n");
            print OUTPUT_FILE ("PDES\n");

            print OUTPUT_FILE ($prim_micro_unit . "\n");
            print OUTPUT_FILE ("KPHR\n");

            print OUTPUT_FILE ($prim_micro_unit . "\n");
            print OUTPUT_FILE ("PHAS\n");

            print OUTPUT_FILE ($prim_micro_unit . "\n");
            print OUTPUT_FILE ("PCON\n");

            print OUTPUT_FILE ($prim_micro_unit . "\n");
            print OUTPUT_FILE ("ACON\n");
        } 
    }
}

close(KLYS_INPUT_FILE);

#
#  Open the subbooster names input file sbstnames.dmp.
#
$sbst_input_file = "/nfs/slac/g/cd/oracle/aida/sbstnames.dmp";

open(SBST_INPUT_FILE, $sbst_input_file) ||
    die "Can't open input file $sbst_input_file";

#
# Loop through each line of the sbstnames.dmp input file, which is
# formatted by the VMS dbdump utility.  An example of this format is:
#
# SBST:LI00,1 
#    NAME S-00-1 
#
# That is, a primary/micro/unit number followed by lines containing
# the channel names for the unit. 
#

while (<SBST_INPUT_FILE>)
{
    chomp;

    $line = $_;

#
#   Blank lines in the input file are ignored.
#
    if ($line ne "")
    {
        if ($line =~ m/SBST:(.*)/)
        {
#
#           The current line contains a subbooster micro name and unit number,
#           which is extracted in parameter $1.  First, this parameter is
#           assigned to the cur_micro_unit variable and trailing whitespace
#           is removed.  The micro name and unit number, which are separated
#           by a comma, are split into micro name and unit number strings.
#           Then the Aida instance name (the primary, micro, and unit strings,
#           separated by colons) is formed.  Finally, the Aida instance/attribute
#           name pairs (the attributes are "PDES", "KPHR", "PHAS", "PCON", and
#           "ACON") are written to the output file.
#
            $cur_micro_unit = $1;

            $cur_micro_unit =~s/[ \t]+$//;

            #print "cur_micro_unit = $cur_micro_unit\n";

            ($cur_micro, $cur_unit) = split(/,/, $cur_micro_unit);

            $prim_micro_unit = "SBST:" . $cur_micro . ":" . $cur_unit;

            #print "prim_micro_unit = $prim_micro_unit\n";

            print OUTPUT_FILE ($prim_micro_unit . "\n");
            print OUTPUT_FILE ("PDES\n");

            print OUTPUT_FILE ($prim_micro_unit . "\n");
            print OUTPUT_FILE ("KPHR\n");

            print OUTPUT_FILE ($prim_micro_unit . "\n");
            print OUTPUT_FILE ("PHAS\n");

            print OUTPUT_FILE ($prim_micro_unit . "\n");
            print OUTPUT_FILE ("PCON\n");

            print OUTPUT_FILE ($prim_micro_unit . "\n");
            print OUTPUT_FILE ("ACON\n");
        } 
    }
}

close(SBST_INPUT_FILE);

close(OUTPUT_FILE);
