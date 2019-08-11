#!/usr/bin/env perl

use strict;
use warnings;
use Verilog::Netlist;

# prepare netlist
my $nl = new Verilog::Netlist();

$nl->read_file(filename => $ARGV[0]);

# read in any sub modules
$nl->link();
$nl->lint();
$nl->exit_if_error();

for my $mod ( $nl->modules()) {
	for my $sig ($mod->ports_sorted()) {
		print $sig->direction(), " ", $sig->name(), " ", $sig->type(), 
			"\n";
	}
}

