#!/usr/bin/perl
$|=1;
#########################################################################
# mudcheck.pl				Version 1.0			#
# Created: Jun 28, 1997			Jared Proudfoot			#
# Last Modified: Jun 28, 1997		jproudfo@footprints.net		#
#########################################################################
# README								#
#									#
# This is a pretty simple script that checks to see if a mud is running	#
# properly.  Be sure to change the 'important vars' section.  The best	#
# idea is to run this script from cron every couple of minutes... That	#
# way you can be sure that it stays running.				#
#########################################################################
# CHANGES AND HISTORY							#
#									#
# v 1.0, Jun 28, 1997	Created.  Seems to work fine now.		#
#########################################################################
#########################################################################
# Define the important vars						#
#									#
# Define the host and port number where the mud resides.		#

$server		=	"localhost";
$port		=	"6969";

# $string is the string of characters we will look for upon connecting.	#
# If we connect, but don't see this string, we will assume the mud	#
# isn't responding (locked up?) and we'll restart it.  The string	#
# *must* be on the first line after connect.	

# You may enter this as a regexp if you wish.				#

$replyString		=	"^\n";

# How long do we wait before we connect timeout (in seconds)?		#

$timeOut	=	"30";
$timer		= 	"30";

# What to execute if we need to restart the mud.  Please include the	#
# FULL path.								#

$exec		=	"/home/redpower/Anotherland/dystopia/src/dystopia";
$mud            =       "/home/redpower/Anotherland/dystopia/src/";
# Path where you want the mud logs to be kept.

$logdir		=	"/home/redpower/Anotherland/dystopia/log";

# Path where we should start the mud from.				#
$startPath	=	"/home/redpower/Anotherland/dystopia/area";

# That's it.  You shouldn't need to change anything after this line.	#
#########################################################################

# What do we need to use?
use Socket;
require 5.003;



#########################################################################
# Main									#
#########################################################################

	chdir $mud;
        open (MUDFILE, "mud.txt") or die "Can't open file $!";
        @pid = <MUDFILE>;
        close(MUDFILE);

if (&connect_server == 0) 
{
	# If we couldn't connect, try and restart.			#
	print ("Connection to $server on port $port failed or timed out after $timeOut seconds!\n");
	$time = (scalar localtime);
	print ("Attempting to restart the mud on $time...\n");
	# Restart the mud						#
	&restart_mud;
}
else 
{
	# We connected, but is it working properly?			#
		eval 
		{
                        alarm ($timeOut);
        		local $SIG{ALRM} = sub 
			{         
				print ("Attempting to restart the mud on $time...SIGALRM\n");
        			print("Killing process and restarting mud PID: @pid");
        			kill 9, @pid;
				system ( "kill -9 @pid");
        			&restart_mud;
			};
                        $readline = (&gl);
        		alarm 0;
    		};
    		if ($@) {
    		    die unless $@ eq "alarm\n";   # propagate unexpected errors
    		    # timed out
    		}
    		else {
        		# didn't
    		}

		if ($readline =~ /$replyString/) 
		{
		    # We found what we were looking for, so exit 	#
		    # properly.					#
		    alarm (0);
		    &disconnect_server;
		    exit 1;
		}
		else
		{
			kill 9, @pid;
        		&restart_mud;
		}

	# After all those searches, we didn't find anything.  The mud	#
	# must be locked up.  Lets kill and restart it.			#
	&disconnect_server;
	print ("The connection was sucessful, but it doesn't seem to be responding\n");
	$time = (scalar localtime);
	print ("Killed: @pid > At: $time.\n");
}
 
 



#########################################################################
# Subroutines								#
#########################################################################


sub connect_server 
{
	# Connect to the server						#
	my ($iaddr, $paddr, $proto);
	$iaddr = inet_aton ($server)
		or die ("ERROR: No host: $server!\n");
	$paddr = sockaddr_in ($port, $iaddr);
	$proto = getprotobyname('tcp');
	socket (SOCK, PF_INET, SOCK_STREAM, $proto)
		or die ("ERROR: Socket error $!\n");
	alarm ($timeOut);
	if (connect (SOCK, $paddr)) 
	{;
		alarm (0);
		return 1;
	}
	else 
	{
		return 0;
	}
}

sub disconnect_server {
	# Disconnect from the server					#
	close (SOCK);
        system ("ulimit -c unlimited");
        system ("ulimit -f 25000");
	return;
}

sub sl 
{
	# Send a line							#
  	my ($line)=@_;
  	print SOCK ("$line")
		or die ("ERROR: Error writing to server: $!\n");
	select SOCK;
  	$|=1;
  	select STDOUT;
  	$|=1;
	return;
}
	
sub gl 
{
        # Get a line                                                    #
        my ($buffer, @reply);
        $buffer=(<SOCK>);
#       (@reply) = split (/\s/, $buffer);
#       return (@reply);
        return ($buffer);
}

sub restart_mud 
{
		# Restart the mud					#
		$timet = time();
		chdir $logdir;
		system ("cp dummy $timet.log");
#		system ("ulimit -c unlimited");
#		system ("ulimit -f 25000");

		chdir $startPath;
		system ("$exec $port > $logdir/$timet.log 2>&1 &");
		return;
}
