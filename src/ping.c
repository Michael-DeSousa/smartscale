#include "ping.h"

char* get_wifi_status()
{
	FILE *pipe;
	char command[100];
	static char wifiStatus[30];
	
	//We assume that if we can access Google, the internet is working. Sends 1 packet and times out after 1 second.
	sprintf(command, "ping -q -w1 -c1 www.google.com &>/dev/null && echo Wifi Connected || echo Wifi Disconnected");
	
	//Open pipe and set up a "read output" mode https://www.mkssoftware.com/docs/man3/popen.3.asp
	pipe = popen(command, "r");
	
	//Recieve the echo on wifi status.
	fgets(wifiStatus, 30, pipe);
	
	//Close pipe
	if (pclose(pipe) != 0)
	{
		fprintf(stderr, "Failed to close wifi status pipe \n");
	}
	
	return wifiStatus;
}
