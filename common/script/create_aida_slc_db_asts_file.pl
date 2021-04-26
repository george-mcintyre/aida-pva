#!/usr/local/bin/perl5
#
#==============================================================
#
#  Abs:  This script creates an Oracle SQL Loader load file 
#        containing Aida SLC Database ASTS primary/micro/channel
#        name and pseudo-secondary instance/attribute pairs for
#        loading into the Aida aida_slc_db_asts table.
#
#  Name: create_aida_slc_db_asts_file.pl 
#
#  Rem:  This script reads the file astsnames.dmp from the
#        /mccdev/nfs_slcdb directory and creates an output file,
#        aida_slc_db_asts.dat, in this same directory.  The
#        input file is the output of the VMS dbdump utility,
#        which is used for listing the primary, micro, and
#        channel name for all ASTS channels.  This script
#        reads this file and transforms it to the output file
#        which contains one line for each ASTS primary/micro/
#        channel name and pseudo-secondary instance/attribute
#        pair.  This output file is suitable for loading into
#        the Aida Oracle aida_slc_db_asts table with the
#        load_aida_slc_db_asts script, which uses the Oracle
#        SQL Loader utility.  This load_aida_slc_db_asts script
#        is run by the run_pmu_load.sh, which also later runs
#        the aidaPmuNames_update script.  This latter script causes
#        the Oracle aida_slc_db_asts table to be read for populating
#        the Aida Oracle aida_names and aida_directory tables for
#        use by the Aida SLC Database data provider.
#        
#
#  Facility:  SLAC
#
#  Auth: 19-Apr-2006, Bob Hall (RDH)
#  Rev:  dd-mmm-yyyy, Reviewer's Name    (USERNAME)
#
#--------------------------------------------------------------
#
#  Mod:
#
#==============================================================
#
@asts_pseudo_secondaries = ("NAME", "LIMS", "LIM1", "LIM2", "SCAL",
                            "CTRL", "STAT", "DATA", "RAW ", "RMS ",
                            "REFS", "TRMS", "DCON", "DAT2", "LMT2",
                            "VSTA", "PTR ", "CHAN", "CTLW", "HDSC");

$input_file = "/mccdev/nfs_slcdb/astsnames.dmp";
open(INPUT_FILE, $input_file) ||
    die "Can't open input file $input_file";

$output_file = "/mccdev/nfs_slcdb/aida_slc_db_asts.dat";
open(OUTPUT_FILE, ">$output_file") ||
    die "Can't open output file $output_file";

#
# Loop through each line of the input file, which is formatted
# by the VMS dbdump utility.  An example of this format is:
#
# ASTS:PC01,1
#    NAME WM1060AX
#         WM1060BX
#         WM1060CX
#         WM1060AY
#
# That is, a primary/micro/unit number followed by lines containing 
# the channel names for the unit.
#
#
$prev_micro = "UNKNOWN";
while (<INPUT_FILE>)
{
    chomp;

    $line = $_;

#
#   Blank lines in the input file are ignored.
#
    if ($line ne "")
    {
        if ($line =~ m/ASTS:(.*),/)
        {
#
#           The current line contains a micro name, which is
#           extracted in parameter $1.  If the micro name is
#           different than the previous micro name that was found
#           and it is not the first one (prevous micro name not
#           equal to the "UNKNOWN" string), loop through all of
#           the channel names that have been stored for this
#           current micro name in the cur_micro_channel_names
#           associative array and output a line for each of the
#           possible ASTS pseudo-secondaries that has the
#           primary/micro/channel name string (separated by colons)
#           and the pseudo-secondary name.  These two parts of
#           each line (primary/micro/channel name string followed by
#           pseudo-secondary name string) are separated by a blank. 
#
            $cur_micro = $1;
            #print "micro = $cur_micro\n";

            if ($cur_micro ne $prev_micro)
            {
                if ($prev_micro ne "UNKNOWN")
                {
                    foreach $name (sort keys(%cur_micro_channel_names))
                    {
                        if (length($name) <= 4)
                        {
                            print "name = $name\n";
                        }

                        for ($j = 0; $j < @asts_pseudo_secondaries; $j++)
                        { 
                            print OUTPUT_FILE ("ASTS:" . $prev_micro . ":" . $name . " " . 
                                        $asts_pseudo_secondaries[$j] . "\n");
                        }

#
#                       Each element of the cur_micro_channel_names
#                       associative array is deleted so that this array
#                       can now be used to store the channel names for
#                       the new micro name that was found. 
#
                        delete($cur_micro_channel_names{$name});
                    }
                }

                $prev_micro = $cur_micro;
            }
        }
        else
        {
#
#           The current line contains a channel name.  Do not
#           process "SPARE" or "USED" channel names so that they
#           will not appear in the output file.
#
            $channel_name = substr($line, 9, 8); 

            if (($channel_name ne "SPARE   ") &&
                ($channel_name ne "USED    ") )
            {
                $cur_micro_channel_names{$channel_name} += 1;
            }
        } 
    }
}

close(INPUT_FILE);

#
# Finally, output lines for each channel name associated with the
# last micro name that was processed.
#
foreach $name (sort keys(%cur_micro_channel_names))
{
    for ($j = 0; $j < @asts_pseudo_secondaries; $j++)
    {
        print OUTPUT_FILE ("ASTS:" . $prev_micro . ":" . $name . " " .
            $asts_pseudo_secondaries[$j] . "\n");
    }
}

close(OUTPUT_FILE);
