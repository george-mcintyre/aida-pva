#!/usr/local/bin/perl
#
# Abs:   This script reads the channel archiver configuration files
#        for a specified subsystem and creates a file containing
#        Aida instance/attribute pairs.
#
# Name:  convert_archiver_config_files.pl
#
# Rem:   This script reads a LCLS, FACET, NLCTA, PEPII, or 8-Pack channel archiver
#        XML configuration files, which contains the list of PV channel
#        names that are currently being archived for the specified
#        subsystem.  The script creates an output file with a name
#        of the form "converted_arch_pvs_<subsystem>.dat", where <subsystem>
#        is either lcls, facet, nlcta, pepii, or pack.  The output file contains 
#        Aida instance/attribute pairs which are read by the PL/SQL script
#        set_aida_channel_archiver.sql to populate the Aida database.
#
#
# Auth:  17-Apr-2007, Bob Hall (rdh)
# Rev:   dd-mmm-yyyy, Reviewer's Name    (USERNAME)
#--------------------------------------------------------------
#
# Mods:
#        02-Feb-2011, Bob Hall (rdh)
#           Added support for the 1st FACET archive engine.  
#        03-Jun-2010, Bob Hall (rdh)
#           Changed to reflect the addition of the 16th LCLS
#           archive engine.
#        21-May-2010, Bob Hall (rdh)
#           Changed to reflect the addition of the 15th LCLS
#           archive engine.
#        10-May-2010, Bob Hall (rdh)
#           Changed to reflect the addition of the 14th LCLS
#           archive engine.
#        25-Apr-2010, Bob Hall (rdh)
#           Changed to reflect the addition of the 13th LCLS
#           archive engine.
#        21-Apr-2010, Bob Hall (rdh)
#           Changed to reflect the addition of the 12th LCLS
#           archive engine.
#        01-Apr-2010, Bob Hall (rdh)
#           Changed to reflect the addition of the ninth, tenth,
#           and eleventh LCLS archive engines.
#        21-Jul-2009, Bob Hall (rdh)
#           Changed to reflect the addition of the eighth LCLS
#           archiver.
#        13-Jul-2009, Bob Hall (rdh)
#           Changed to reflect the addition of the seventh LCLS
#           archiver.
#        02-Apr-2009, Bob Hall (rdh)
#           Changed to reflect the addition of the sixth LCLS
#           archiver.
#        09-Jan-2009, Bob Hall (rdh)
#           Changed to read PEPII archiver configuration files
#           from the /nfs/slac/g/cd_archiver area to the latest
#           new archiver disk area, /nfs/slac/g/archiver.
#        22-Sep-2008, Bob Hall (rdh)
#           Changed to reflect the addition of the third, fourth,
#           and fifth LCLS archiver.
#        22-Feb-2008, Bob Hall (rdh)
#           Changed to reflect the addition of the second LCLS
#           archiver.
#        20-Feb-2008, Bob Hall (rdh)
#           Changed to read archiver configuration files from
#           the new archiver disk /nfs/slac/g/cd_archiver area
#           rather than the old archiver disk /nfs/slac/g/esd/archiver
#           area.
#        25-Jul-2007, Bob Hall (rdh)
#           Changed to reflect the addition of the pepii_4 and
#           pepii_5 archivers.
#
#
#==============================================================

use XML::Simple;
use Data::Dumper;

if ($#ARGV < 0)
{
    die "$0 requires input subsystem argument (lcls, pepii, nlcta, or pack)\n";
}

$subsystem = $ARGV[0];

$lcls_1_input_file = "/arch/lcls/lcls_1/lcls_1-group.xml";
$lcls_2_input_file = "/arch/lcls/lcls_2/lcls_2-group.xml";
$lcls_3_input_file = "/arch/lcls/lcls_3/lcls_3-group.xml";
$lcls_4_input_file = "/arch/lcls/lcls_4/lcls_4-group.xml";
$lcls_5_input_file = "/arch/lcls/lcls_5/lcls_5-group.xml";
$lcls_6_input_file = "/arch/lcls/lcls_6/lcls_6-group.xml";
$lcls_7_input_file = "/arch/lcls/lcls_7/lcls_7-group.xml";
$lcls_8_input_file = "/arch/lcls/lcls_8/lcls_8-group.xml";
$lcls_9_input_file = "/arch/lcls/lcls_9/lcls_9-group.xml";
$lcls_10_input_file = "/arch/lcls/lcls_10/lcls_10-group.xml";
$lcls_11_input_file = "/arch/lcls/lcls_11/lcls_11-group.xml";
$lcls_12_input_file = "/arch/lcls/lcls_12/lcls_12-group.xml";
$lcls_13_input_file = "/arch/lcls/lcls_13/lcls_13-group.xml";
$lcls_14_input_file = "/arch/lcls/lcls_14/lcls_14-group.xml";
$lcls_15_input_file = "/arch/lcls/lcls_15/lcls_15-group.xml";
$lcls_16_input_file = "/arch/lcls/lcls_16/lcls_16-group.xml";

$facet_1_input_file = "/arch2/facet/facet_1/facet_1-group.xml";

$nlcta_input_file = "/nfs/slac/g/cd_archiver/tarf/data/ArPVList_nlcta";

$pepii_1_input_file = "/nfs/slac/g/archiver/pepii_1/data/ArPVList_pepii_1";
$pepii_2_input_file = "/nfs/slac/g/archiver/pepii_2/data/ArPVList_pepii_2";
$pepii_3_input_file = "/nfs/slac/g/archiver/pepii_3/data/ArPVList_pepii_3";
$pepii_4_input_file = "/nfs/slac/g/archiver/pepii_4/data/ArPVList_pepii_4";
$pepii_5_input_file = "/nfs/slac/g/archiver/pepii_5/data/ArPVList_pepii_5";

$pack_input_file = "/nfs/slac/g/cd_archiver/pack/data/ArPVList_pack";

@input_files = ();

if ($subsystem eq "lcls")
{
    push @input_files, $lcls_1_input_file;
    push @input_files, $lcls_2_input_file;
    push @input_files, $lcls_3_input_file;
    push @input_files, $lcls_4_input_file;
    push @input_files, $lcls_5_input_file;
    push @input_files, $lcls_6_input_file;
    push @input_files, $lcls_7_input_file;
    push @input_files, $lcls_8_input_file;
    push @input_files, $lcls_9_input_file;
    push @input_files, $lcls_10_input_file;
    push @input_files, $lcls_11_input_file;
    push @input_files, $lcls_12_input_file;
    push @input_files, $lcls_13_input_file;
    push @input_files, $lcls_14_input_file;
    push @input_files, $lcls_15_input_file;
    push @input_files, $lcls_16_input_file;
}
elsif ($subsystem eq "facet")
{
    push @input_files, $facet_1_input_file;
}
elsif ($subsystem eq "nlcta")
{
    push @input_files, $nlcta_input_file;
}
elsif ($subsystem eq "pepii")
{
    push @input_files, $pepii_1_input_file;
    push @input_files, $pepii_2_input_file;
    push @input_files, $pepii_3_input_file;
    push @input_files, $pepii_4_input_file;
    push @input_files, $pepii_5_input_file;
}
elsif ($subsystem eq "pack")
{
    push @input_files, $pack_input_file;
}
else
{
    die "invalid input subsystem argument (must be lcls, facet, nlcta, pepii or pack)\n";
}


$output_file = "/nfs/slac/g/cd/oracle/aida/converted_arch_pvs_" . $subsystem . ".dat";

open(OUTPUT_FILE, ">" . $output_file) ||
    die "Can't open output file $output_file";

#
# Use the XML Simple parser Perl module, which was added to the Perl
# version being used.
# 
$parser = XML::Simple->new();

foreach $input_file (@input_files)
{
#
#   Parse the channel archiver XML configuration file into a
#   Perl structure.  For each channel name in each group write
#   two lines into the output file: (1) the channel name (the Aida
#   instance name), and (2) the string "HIST.<subsystem>" (the Aida
#   attribute name).
#
    $pepii_config = $parser->XMLin($input_file, ForceArray=>1);

    if ($@)
    {
        print "errors = $@\n";
    }

#    print Dumper($pepii_config);

    $attribute_name = "HIST." . $subsystem;

    foreach $pepii_group (@{$pepii_config->{group}} )
    {
#    print "found group\n";

        foreach $pepii_channel (@{$pepii_group->{channel}} )
        {
#            print "found channel @{$pepii_channel->{name}}\n";
            ($channel_name) = @{$pepii_channel->{name}};
            print OUTPUT_FILE ($channel_name, "\n");
            print OUTPUT_FILE ($attribute_name, "\n"); 
        }
    }
}

close(OUTPUT_FILE); 
