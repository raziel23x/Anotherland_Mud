#!/usr/bin/perl

open (MUDFILE, "mud.txt") or die "Can't open file $!";
$pid = <MUDFILE>;

print("Killing process $pid\n");
kill 9, $pid;
