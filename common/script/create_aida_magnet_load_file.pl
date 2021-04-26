#!/usr/local/bin/perl5
#
#==============================================================
#
#  Abs:  This script creates a file containing Aida instance/
#        attribute pairs for the Aida magnet server.  This
#        created instance/attribute file is read by the
#        set_aida_slc_magnet.sql PL/SQL script to populate
#        the aida_names and aida_directory Oracle tables for
#        the Aida magnet server.
#
#  Name: create_aida_magnet_load_file.pl
#
#  Rem:  This script reads the files dgrpnames.dmp and
#        magnetprims.dmp from the /nfs/slac/g/cd/oracle/aida
#        directory and creates the output file aida_magnet_load.dat
#        in this directory.  The input files are created as a
#        result of running the VMS dbdump utility in the VMS
#        command script slccom:slcdb2oracle.submit, which creates
#        these files in the NFS_NDB.  This directory is mapped to
#        the UNIX /mccdev/nfs_slcdb directory, which is visible
#        from UNIX machines such as slcs2.  The UNIX script
#        run_pmu_load.sh is run later by the VMS command script
#        slccom:slcdb2oracle.submit during a DB install and the
#        run_pmu_load.sh script invokes the
#        slc_magnet_populate_aida_database.sh script.  This latter
#        script copies the dgrpnames.dmp and magnetprims.dmp files
#        from the /mccdev/nfs_slcdb directory to the
#        /nfs/slac/g/cd/orcle/aida and then invokes this Perl
#        script. 
#
#        The output file aida_magnet_load.dat contains Aida
#        instance/attribute pairs that are read by the
#        set_aida_slc_magnet.sql PL/SQL script to populate
#        the aida_names and aida_directory Oracle tables for
#        the Aida magnet server.  This PL/SQL script is also
#        invokded by the slc_magnet_populate_aida_database.sh
#        script.
#
#  Facility:  SLAC
#
#  Auth: 09-Oct-2006, Bob Hall (RDH)
#  Rev:  dd-mmm-yyyy, Reviewer's Name    (USERNAME)
#
#--------------------------------------------------------------
#
#  Mod:
#        03-Oct-2007, Bob Hall (RDH)
#            Added instance/attribute output lines for
#            instance MAGNETSET, attribute BCON.
#        16-Oct-2006, Bob Hall (RDH)
#            Replaced attribute separator from a forward slash
#            to a colon.
#
#==============================================================
#
#  Open the display group input file dgrpnames.dmp.
#
#$display_group_input_file = "dgrpnames.dmp";
$display_group_input_file = "/nfs/slac/g/cd/oracle/aida/dgrpnames.dmp";

open(DISPLAY_GROUP_INPUT_FILE, $display_group_input_file) ||
    die "Can't open input file $display_group_input_file";

#$output_file = "aida_magnet_load.dat";
$output_file = "/nfs/slac/g/cd/oracle/aida/aida_magnet_load.dat";

open(OUTPUT_FILE, ">$output_file") ||
    die "Can't open output file $output_file";

#
# Loop through each line of the display group input file, which is
# formatted by the VMS dbdump utility.  An example of this format is:
#
#DGRP:VX00,1
#    MNEM LINAC
#
# The display group name follows the keyword MNEM.  Display group
# names that start with the characters "SPAR" are spare display
# groups and are discarded in this script.
#
#

@display_group_names = ();
while (<DISPLAY_GROUP_INPUT_FILE>)
{
    chomp;

    $line = $_;

#
#   Blank lines in the input file are ignored.
#
    if ($line ne "")
    {
#
#       If the line contains the MNEM keyword, extract the
#       display name which follows it.  Then remove any whitespace
#       characters, such as blank characters that may follow the
#       display group name.  If the display group is not a spare
#       display group, append its name to the list of display group
#       names.
#
        if ($line =~ m/    MNEM (.*)/)
        {
            $display_group = $1;
            $display_group =~ s/\s//g;

            if ($display_group =~ /^SPAR/)
            {
            }
            else
            {
                push @display_group_names, $display_group;
            }
        } 
    }
}

close(DISPLAY_GROUP_INPUT_FILE);

#
#  Initialize lists used in processing the magnet primaries input
#  file and creating an attribute line, which is the name of a
#  magnet primary, the forward slash ("/") character, and the name
#  of a secondary.  The magnet_units list contains the magnet units
#  of interest in the magnet primaries input file.  The magnet_secondaries
#  list contains the secondary name (either "BDES" or "VDES") for each
#  associated unit of interest.
#
@magnet_units = (1, 2, 3, 14, 22);
@magnet_secondaries = ("BDES", "VDES", "BDES", "VDES", "VDES");

@bdes_primaries = ();
@vdes_primaries = ();

#
#  Open the magnet primaries input file magnetprim.dmp.
#

#$magnet_primaries_input_file = "magnetprim.dmp";
$magnet_primaries_input_file = "/nfs/slac/g/cd/oracle/aida/magnetprims.dmp";

open(MAGNET_PRIMARIES_INPUT_FILE, $magnet_primaries_input_file) ||
    die "Can't open input file $magnet_primaries_input_file";

#
# Loop through each line of the magnet primary input file, which is
# formatted by the VMS dbdump utility.  An example of this format is:
#
#DVMD:VX00,1
#    PRIM QUAD XCOR
#         YCOR SOLN
#         LENS SEXT
#         QTRM BEND
#         SEPT SMPS
#         BTRM
#
#
# As can be seen from the example, each line that contains a primary
# name may contain one or two primary names.
#
#

while (<MAGNET_PRIMARIES_INPUT_FILE>)
{
    chomp;

    $line = $_;

#
#   Blank lines in the input file are ignored.
#
    if ($line ne "")
    {
#
#       If the line contains a unit number, extract the current unit number.
#       Then loop through the list that contains the units of interest
#       to determine whether the current unit number being process is in
#       this list.  If it does, set the prev_magnet_unit flag (so that
#       primary names following this line will be processed) and save the
#       index into the list of units of interest.
#
        if ($line =~ m/DVMD:VX00,(.*)/)
        {
            $cur_unit = $1;
            #print "cur_unit = $cur_unit\n";

            $prev_magnet_unit = 0;

            $found = 0;
            $i = 0;
            while (($i < @magnet_units) && (!$found))
            {
                if ($magnet_units[$i] == $cur_unit)
                {
                    #print "found unit $cur_unit\n";
                    $prev_magnet_unit = 1;
                    $prev_magnet_unit_index = $i;
 
                    $found = 1;
                }
                else
                {
                    $i++;
                }
            }

        }
        else
        {
#
#           The current line does not contain a unit number.  If the last line
#           processed that contained a unit number was in the list of units of
#           interest, the current line contains one or more magnet primary names.
#           In this case, split the line into fields delimited by whitespace
#           and disregard any NULL fields or any field containing the "PRIM"
#           keyword.  Then add the name of each primary name to either the list
#           of BDES primary names or VDES primary names, depending on whether
#           the unit number is associated with the "BDES" or "VDES" secondary.
#
            if ($prev_magnet_unit)
            {
                #print "magnet_unit line = $line\n";

                @fields = split(/\s+/, $line);

                $i = 0;
                while ($i < @fields)
                {
                    if (($fields[$i] ne "") && ($fields[$i] ne "PRIM"))
                    {
                        #print("field = $fields[$i]\n");
                        #print("type = $magnet_secondaries[$prev_magnet_unit_index]\n");

                        if ($magnet_secondaries[$prev_magnet_unit_index] eq "BDES")
                        {
                            push @bdes_primaries, $fields[$i]; 
                        }
                        elsif ($magnet_secondaries[$prev_magnet_unit_index] eq "VDES")
                        {
                            push @vdes_primaries, $fields[$i]; 
                        }
                    }

                    $i++;
                }

            }
        }
    }
}

#
#  For each name in the list of display group names, first write
#  the Aida instance/attribute pairs for each primary name in
#  the BDES secondary list of primary names.  Then write the
#  instance/attribute pairs for each primary name in the VDES
#  secondary list of primary names.  Finally, write a final
#  instance/attribute pair with instance name MAGNETSET and
#  an attribute line containing blank characters (this attribute
#  will be set to NULL in the Aida Oracle aida_names table by
#  the set_aida_slc_magnet.sql PL/SQL script).
#
foreach $dgrp (@display_group_names)
{
    foreach $bdes_prim (@bdes_primaries)
    {
        print OUTPUT_FILE ($dgrp, "\n");

        $attribute = $bdes_prim . ":" . "BDES";
        print OUTPUT_FILE ($attribute, "\n");
    }

    foreach $vdes_prim (@vdes_primaries)
    {
        print OUTPUT_FILE ($dgrp, "\n");

        $attribute = $vdes_prim . ":" . "VDES";
        print OUTPUT_FILE ($attribute, "\n");
    }

}

print OUTPUT_FILE ("MAGNETSET\n");
print OUTPUT_FILE ("BDES\n");

print OUTPUT_FILE ("MAGNETSET\n");
print OUTPUT_FILE ("VDES\n");

print OUTPUT_FILE ("MAGNETSET\n");
print OUTPUT_FILE ("BCON\n");

close(MAGNET_PRIMARIES_INPUT_FILE);
close(OUTPUT_FILE);
