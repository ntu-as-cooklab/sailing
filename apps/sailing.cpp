#include <stdio.h>
#include <signal.h>
#include "ws/server.h"

static int interrupted;
static void sigint_handler(int sig)
{
    interrupted = 1;
}

int main(void)
{
	printf(	"*************** Sailing Simulation ***************\n"
			"Build: %s %s\n", __DATE__, __TIME__);

	lws_set_log_level(LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE, NULL);

	const struct lws_http_mount mount = {
		.mountpoint             = "/",		/* mountpoint URL */
		.origin                 = "./www",  /* serve from dir */
		.def                    = "index.html",	/* default filename */
		.origin_protocol        = LWSMPRO_FILE,	/* files in a dir */
		.mountpoint_len         = 1, /* char count */
	};

	struct lws_context_creation_info info =
    {
        .port = 8000,
        .protocols = protocols,
        .options = LWS_SERVER_OPTION_DISABLE_IPV6,
        .vhost_name = "localhost",
        .mounts = &mount,
        .ws_ping_pong_interval = 10,
    };

    lwsl_user("LWS server starting on port %u\n", info.port);
    struct lws_context *context = lws_create_context(&info);
    if (!context) {
        lwsl_err("lws init failed\n");
        return 1;
    }

    signal(SIGINT, sigint_handler);
    while (lws_service(context, 1000) >= 0 && !interrupted) ;

    lws_context_destroy(context);
	
   	return 0;
}
