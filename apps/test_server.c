#include <libwebsockets.h>
#include <string.h>
#include <signal.h>
#include "ws/myprotocol.h"

static struct lws_protocols protocols[] = {
    { "http", lws_callback_http_dummy, 0, 0 },
    LWS_PLUGIN_PROTOCOL_MINIMAL,
    { NULL, NULL, 0, 0 } /* terminator */
};

static int interrupted;

static const struct lws_http_mount mount = {
    .mount_next             = NULL,		/* linked-list "next" */
    .mountpoint             = "/",		/* mountpoint URL */
    .origin                 = "./www",  /* serve from dir */
    .def                    = "index.html",	/* default filename */
    .protocol               = NULL,
    .cgienv                 = NULL,
    .extra_mimetypes        = NULL,
    .interpret              = NULL,
    .cgi_timeout            = 0,
    .cache_max_age          = 0,
    .auth_mask              = 0,
    .cache_reusable         = 0,
    .cache_revalidate       = 0,
    .cache_intermediaries   = 0,
    .origin_protocol        = LWSMPRO_FILE,	/* files in a dir */
    .mountpoint_len         = 1, /* char count */
    .basic_auth_login_file  = NULL,
};

void sigint_handler(int sig)
{
    interrupted = 1;
}

int main(int argc, const char **argv)
{
    lws_set_log_level(LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE, NULL);
    /* for LLL_ verbosity above NOTICE to be built into lws,
    * lws must have been configured and built with
    * -DCMAKE_BUILD_TYPE=DEBUG instead of =RELEASE */
    /* | LLL_INFO */ /* | LLL_PARSER */ /* | LLL_HEADER */
    /* | LLL_EXT */ /* | LLL_CLIENT */ /* | LLL_LATENCY */
    /* | LLL_DEBUG */

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