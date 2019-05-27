#include <stdio.h>
#include <signal.h>
#include "server.h"

static void sigint_handler(int sig)
{
    server_stop();
}

int main(void)
{
	printf(	"*************** Sailing Simulation ***************\n"
			"Build: %s %s\n", __DATE__, __TIME__);

	server_init();
    signal(SIGINT, sigint_handler);
    server_run();
	
   	return 0;
}
