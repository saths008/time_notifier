use strict;
use warnings;


print "Are you running this script as sudo? (Y/N)\n";
my $running_as_sudo = <STDIN>;
chomp $running_as_sudo;
$running_as_sudo = lc($running_as_sudo);
if ($running_as_sudo ne "y") {
	print "Exiting...Please run this script as sudo!\n";
	exit(1);
}

# Create systemd file
print "What is the pwd to the time_notifier repo? eg./home/username/dev/time_notifier?\n";
my $pwd = <STDIN>;
chomp $pwd;
my $exe_pwd = $pwd."/prog";

print "How often would you like a notification to appear in minutes eg. 45?\n";
my $notif_dur = <STDIN>;
chomp $notif_dur;

# script requires to be run in root, so we can't just
# capture the output of whoami
print "What is your linux username? (ie. output of whoami on another shell)\n";
my $username = <STDIN>;
chomp $username;

print "What is your user id? (ie. output of `id -u` on another shell)\n";
my $user_id = <STDIN>;
chomp $user_id;

my $display = `echo \$DISPLAY`;
chomp $display;

my $service_path = "/etc/systemd/system/time_notifier.service";
my $service_file_contents = "
[Unit]
Description=Time Notifier
[Service]
ExecStart=$exe_pwd
WorkingDirectory=$pwd
Restart=always
User=$username
Environment=DISPLAY=$display
Environment=TIME_NOTIFY_DURATION=$notif_dur
Environment=XDG_RUNTIME_DIR=/run/user/$user_id

[Install]
WantedBy=multi-user.target
";

print "Is the following the correct .service file? (Y/N)\n$service_file_contents";

my $confirmation = <STDIN>;
chomp $confirmation;
$confirmation = lc($confirmation);

if ($confirmation ne "y") {
	print "File was incorrect, exiting...";
	exit(1);
}

open(my $fh, '>', $service_path) or die "Could not open file '$service_path' $!";
print $fh $service_file_contents;
close $fh;

print "Service file written\n";

system("systemctl unmask time_notifier.service") == 0 or die("set_up: unmasking time_notifier service file failed");
system("systemctl enable time_notifier.service") == 0 or die("set_up: enabling time_notifier service file failed");
system("systemctl daemon-reload") == 0 or die("setup: daemon reloading failed");
system("systemctl restart time_notifier.service") == 0 or die("set_up: restarting time_notifier service failed");

print "Enabled and running the daemon. Displaying logs: \n";

system("sudo journalctl -u time_notifier.service | less") == 0 or die("set_up: displaying journalctl logs failed");

print "Finishing script\n";
