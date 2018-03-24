#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <string.h>
#include <string>

static void skeleton_daemon()
{
	pid_t pid;
	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	if (pid > 0)
		exit(EXIT_SUCCESS);
	if (setsid() < 0)
		exit(EXIT_FAILURE);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	if (pid > 0)
		exit(EXIT_SUCCESS);
	umask(0);
	chdir("/");
	int x;
	for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
	{
		close (x);
	}
}

int check_battery()
{
	char command_output[1000];
	char charging[1000];
	char percentage_string[100];
	FILE *f = popen("upower -i `upower -e | grep BAT` | grep -E 'percentage|state'", "r");
	fgets(charging, sizeof(charging), f);
	sscanf(charging, "%s%s", command_output, charging);
	if(!strcmp(charging, "charging")) {
		return -1;
	}
	fgets(command_output, sizeof(command_output), f);
	pclose(f);
	sscanf(command_output, "%s%s", command_output, percentage_string);
	int i = 0;
	while(percentage_string[i++] != '\0');
	percentage_string[i-2] = '\0';
	return atoi(percentage_string);
}

int main()
{
	skeleton_daemon();
	int notice_level = 0;

	while (1)
	{
		if(check_battery() <=1 && notice_level <= 6 && check_battery() >= 0) {
			system("notify-send \"Bye Bye!\" -u critical");
			notice_level = 6;
		}
		if(check_battery() <=2 && notice_level <= 5) {
			system("notify-send \"I am dying, (at 2%)\" -u critical");
			notice_level = 6;
		}
		if(check_battery() <=3 && notice_level <= 4) {
			system("notify-send \"Cummon you lazy peice of sh*t!! battery 3%\" -u critical");
			notice_level = 5;
		}
		if(check_battery() <=5 && notice_level <= 3) {
			system("notify-send \"Battery is critically low (5%)\" -u critical");
			notice_level = 4;
		}
		if(check_battery() <=10 && notice_level <= 2) {
			system("notify-send \"Battery is critically low (10%)\" -u critical");
			notice_level = 3;
		}
		if(check_battery() <= 20 && notice_level <= 1) {
			system("notify-send \"Battery is less than 20%\" -u normal");
			notice_level = 2;
		}
		if(check_battery() <= 50 && notice_level <= 0) {
			system("notify-send \"Battery is less than 50%\" -u low");
			notice_level = 1;
		}
		sleep (20);
	}

	return EXIT_SUCCESS;
}
