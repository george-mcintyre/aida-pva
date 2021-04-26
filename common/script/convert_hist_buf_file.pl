#!/usr/local/bin/perl5
#
# This script reads a history buffer file (dev_device_sequ.dat
# or prod_device_sequ.dat) from the /nfs/slac/g/cd/oracle/aida
# directory.  The history buffer file read depends on the
# specified parameter, dev or prod.  This script creates the
# output file converted_device_sequ.dat.  The output file contains
# Aida instance/attribute pairs which are read by the PL/SQL script
# set_aida_slc_hist.pl to populate the Aida Oracle database.
#
# Auth:  05-Aug-2005, Bob Hall (rdh)
#
#
if ($#ARGV < 0)
{
    die "$0 requires argument (dev or prod)\n";
}

$argument_mode = $ARGV[0];

if ($argument_mode eq "dev")
{
    $input_file = "/nfs/slac/g/cd/oracle/aida/dev_device_sequ.dat"
}
elsif ($argument_mode eq "prod")
{
    $input_file = "/nfs/slac/g/cd/oracle/aida/prod_device_sequ.dat"
}
else
{
    die "invalid argument (must be dev or prod)\n";
}

open(INPUT_FILE, $input_file) ||
    die "Can't open input file $input_file";

$output_file = "/nfs/slac/g/cd/oracle/aida/converted_device_sequ.dat";
open(OUTPUT_FILE, ">" . $output_file) ||
    die "Can't open output file $output_file";

$last_line = "";

#
# Read each line from the input history buffer file.  Each line contains
# 4 or 5 fields, separated by whitespace (e.g., blanks).
#
while(<INPUT_FILE>)
{
    chomp;

    $cur_line = $_;

#
#   Parse the line into array elements.  Each field is separated by
#   whitespace.  After parsing, each field contains no whitespace.
#
    @array = split(/\s+/,$cur_line);

#
#   If the there were only 4 fields, the instance name is the first,
#   second, and third elements separated by colons and the attribute
#   name is the fourth element with the suffix ".HIST".  If there
#   were 5 fields, the instance name is the first, second, third,
#   and fourth elements separated by colons and the attribute name
#   is the fifth element with the suffix ".HIST".
#
    $valid_line = 1;
    if ($array[4] eq "")
    {
        if (($array[0] ne "") &&
            ($array[1] ne "") &&
            ($array[2] ne "") &&
            ($array[3] ne "") )
        {
            $instance = $array[0] . ":" . $array[1] . ":" . $array[2];

            $attribute = $array[3] . ".HIST";
        }
        else
        {
            $valid_line = 0;
        }
    }
    elsif ($array[5] eq "")
    {
        if (($array[0] ne "") &&
            ($array[1] ne "") &&
            ($array[2] ne "") &&
            ($array[3] ne "") &&
            ($array[4] ne "") )
        { 
            $instance = $array[0] . ":" . $array[1] . ":" . $array[2] . ":" . $array[3]; 

            $attribute = $array[4] . ".HIST";
        }
        else
        {
            $valid_line = 0;
        }
    }
    elsif ($array[6] eq "")
    {
        if (($array[0] ne "") &&
            ($array[1] ne "") &&
            ($array[2] ne "") &&
            ($array[3] ne "") &&
            ($array[4] ne "") &&
            ($array[5] ne "") )
        {
#
#           There were 6 fields.  However, the fifth and six fields are
#           really one field, the fifth, with embedded blanks.  Find
#           the index of the fifth field in the substring after the
#           fourth field.  Then extract the substring of the fifth field
#           to the end of the current line (removing trailing whitespace).
#           The attribute is this substring (with embedded blanks) concatenated
#           with the string ".HIST".
#
            $fourth_field_pos = rindex($cur_line, $array[3]);

            $fourth_field_len = length($array[3]);

            $after_fourth_field_pos = $fourth_field_pos + $fourth_field_len;

            $after_fourth_field_substr = substr($cur_line, $after_fourth_field_pos);

            $fifth_field_pos = index($after_fourth_field_substr, $array[4]);

            $fifth_field = substr($after_fourth_field_substr, $fifth_field_pos);

            $fifth_field =~s/[ \t]+$//;

            $instance = $array[0] . ":" . $array[1] . ":" . $array[2] . ":" . $array[3]; 

            $attribute = $fifth_field . ".HIST";
        }
        else
        {
            $valid_line = 0;
        }
    }
    else
    {
        $valid_line = 0;
    }

    if ($valid_line)
    {
        print OUTPUT_FILE ($instance, "\n");
        print OUTPUT_FILE ($attribute, "\n");
    }
    else
    {
        print "invalid line = $cur_line\n";
    }

}

close(INPUT_FILE);
close(OUTPUT_FILE);
