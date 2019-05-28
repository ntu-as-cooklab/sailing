#include <stdio.h>
#include <signal.h>
#include "server.h"
#include "session.hpp"

static void sigint_handler(int sig)
{
    server_stop();
	printf("Saving session\n");
	Session::writeBson();
}

int main(void)
{
	printf(	"*************** Sailing Simulation ***************\n"
			"Build: %s %s\n", __DATE__, __TIME__);

	Session::loadBson();
	server_init();
    signal(SIGINT, sigint_handler);
    server_run();
	
   	return 0;
}
