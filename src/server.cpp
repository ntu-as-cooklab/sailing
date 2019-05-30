#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libwebsockets.h>
#include "message.hpp"
#include "server.h"
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include "session.hpp"
#include "kml.hpp"
#include "csv.hpp"
#include <iostream>

#define LWS_PLUGIN_STATIC

using namespace std;

/* per session data: one of these is created for each client connecting to us */
typedef struct my_pss_t {
    struct my_pss_t *pss_list;
    struct lws      *wsi;

    queue<shared_ptr<mymsg_t>> msgq; // message queue
} my_pss_t;

/* per vhost data: one of these is created for each vhost our protocol is used with */
typedef struct my_vhd_t {
    struct lws_context          *context;
    struct lws_vhost            *vhost;
    const struct lws_protocols  *protocol;
    my_pss_t                    *pss_list; /* linked-list of live pss */
} my_vhd_t;

typedef struct http_pss_t {
	char path[128];
    std::string content;
    const char* pointer;
} http_pss_t;

static struct lws_context *context;
static int server_callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);
static int callback_dynamic_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);
static volatile bool interrupted = false;

static struct lws_protocols protocols[] = {
    { "http", callback_dynamic_http, sizeof(struct http_pss_t), 0 },
    {
        .name                   = SERVER_PROTOCOL,
        .callback               = server_callback,
        .per_session_data_size  = sizeof(my_pss_t),
        .rx_buffer_size         = 1024,
        .id                     = 0,
        .user                   = NULL,
        .tx_packet_size         = 0,
    },
    {} /* terminator */
};

static const struct lws_http_mount mount_dyn = {
	.mountpoint         = "/gen", /* mountpoint URL */
	.protocol           = "http",
	.origin_protocol    = LWSMPRO_CALLBACK, /* dynamic */
	.mountpoint_len     = 4, /* char count */
};

static const struct lws_http_mount mount = {
    .mount_next             = &mount_dyn,
    .mountpoint             = "/",		/* mountpoint URL */
    .origin                 = "./www",  /* serve from dir */
    .def                    = "index.html",	/* default filename */
    .origin_protocol        = LWSMPRO_FILE,	/* files in a dir */
    .mountpoint_len         = 1, /* char count */
};

static struct lws_context_creation_info info =
{
    .port = 80,
    .protocols = protocols,
    .options = LWS_SERVER_OPTION_DISABLE_IPV6,
    .vhost_name = "localhost",
    .mounts = &mount,
    .ws_ping_pong_interval = 10,
};

int server_init(void)
{
    lws_set_log_level(LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE, NULL);

    lwsl_user("LWS server starting on port %u\n", info.port);
    context = lws_create_context(&info);
    if (!context) {
        lwsl_err("lws init failed\n");
        return -1;
    }

    return 0;
}

void server_run(void)
{
    while (lws_service(context, 1000) >= 0 && !interrupted) ;
    lws_context_destroy(context);
}

void server_stop(void)
{
    interrupted = true;
}

static my_vhd_t *myvhd = NULL;

int server_pushall(mymsg_t msg)
{
    my_vhd_t *vhd = myvhd;
    if (!vhd) return -1;

    // shared_ptr<mymsg_t> msgp = make_shared<mymsg_t>(msg);

    lws_start_foreach_llp(my_pss_t **, ppss, vhd->pss_list) {

        shared_ptr<mymsg_t> msgp = make_shared<mymsg_t>(msg); // FIXME: defeats use of shared pointer

        msgp->insert(msgp->begin(), LWS_PRE, 0x0);
        (*ppss)->msgq.push(msgp);
        lws_callback_on_writable((*ppss)->wsi);
    } lws_end_foreach_llp(ppss, pss_list);

    return 0;
}

int server_pushto(my_pss_t *pss, mymsg_t msg)
{
    shared_ptr<mymsg_t> msgp = make_shared<mymsg_t>(msg); // FIXME: defeats use of shared pointer

    msgp->insert(msgp->begin(), LWS_PRE, 0x0);
    pss->msgq.push(msgp);
    lws_callback_on_writable(pss->wsi);

    return 0;
}

static int server_callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    my_pss_t *pss = (my_pss_t *)user;
    my_vhd_t *vhd = (my_vhd_t *)lws_protocol_vh_priv_get(lws_get_vhost(wsi), lws_get_protocol(wsi));

    switch (reason) {
        case LWS_CALLBACK_PROTOCOL_INIT:
            vhd = (my_vhd_t*) lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi), lws_get_protocol(wsi), sizeof(my_vhd_t));
            vhd->context    = lws_get_context(wsi);
            vhd->protocol   = lws_get_protocol(wsi);
            vhd->vhost      = lws_get_vhost(wsi);
            myvhd = vhd;
            break;

        case LWS_CALLBACK_ESTABLISHED:
            lwsl_user("New connection established\n");
            lws_ll_fwd_insert(pss, pss_list, vhd->pss_list); /* add ourselves to the list of live pss held in the vhd */
            pss->wsi    = wsi;
            pss->msgq   = queue<shared_ptr<mymsg_t>>();
            break;

        case LWS_CALLBACK_CLOSED:
            lws_ll_fwd_remove(my_pss_t, pss_list, pss, vhd->pss_list); /* remove our closing pss from the list of live pss */
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE: {
            if(pss->msgq.empty()) break;
            
            /* notice we allowed for LWS_PRE in the payload already */
            int len = lws_write(wsi, pss->msgq.front()->data() + LWS_PRE, pss->msgq.front()->size() - LWS_PRE, LWS_WRITE_BINARY);
            //lwsl_user("Write %d len %d\n", pss->wsi, len);
            if (len < (int)pss->msgq.front()->size() - LWS_PRE) {
                lwsl_err("ERROR msg len %d writing to ws\n", len);
                return -1;
            }
            pss->msgq.pop();

            if(!pss->msgq.empty()) lws_callback_on_writable(wsi);
        } break;

        case LWS_CALLBACK_RECEIVE: {
            thread t = thread(server_decode, pss, (uint8_t*)in, len);
            t.detach();
            // server_decode((uint8_t*)in, len);
        } break;

        default:
            break;
    }

    return 0;
}

static int callback_dynamic_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
	http_pss_t *pss = (http_pss_t*)user;
	uint8_t buf[LWS_PRE + 2048];
    uint8_t *start = &buf[LWS_PRE];
    uint8_t *end = &buf[sizeof(buf) - LWS_PRE - 1];

	switch (reason)
    {
	    case LWS_CALLBACK_HTTP: {
            /* in contains the url part after our mountpoint, if any */
            lws_snprintf(pss->path, sizeof(pss->path), "%s", (const char *)in);
            std::string path(pss->path);
            std::string ext = path.substr(path.find_last_of(".") + 1);
            uint32_t id = std::atoi(path.substr(1, path.find_last_of(".")).c_str());
            printf("Http request: %s id %d ext %s\n", pss->path, id, ext.c_str());

            /*
            * prepare and write http headers... with regards to content-
            * length, there are three approaches:
            *
            *  - http/1.0 or connection:close: no need, but no pipelining
            *  - http/1.1 or connected:keep-alive
            *     (keep-alive is default for 1.1): content-length required
            *  - http/2: no need, LWS_WRITE_HTTP_FINAL closes the stream
            *
            * giving the api below LWS_ILLEGAL_HTTP_CONTENT_LEN instead of
            * a content length forces the connection response headers to
            * send back "connection: close", disabling keep-alive.
            *
            * If you know the final content-length, it's always OK to give
            * it and keep-alive can work then if otherwise possible.  But
            * often you don't know it and avoiding having to compute it
            * at header-time makes life easier at the server.
            */

            uint8_t *p = start;
            const char* mime = "text/html";
            enum http_status status = HTTP_STATUS_OK;
            if     (ext == "csv")
            {
                mime = "text/csv";
                pss->content = csv_fromPath(&Session::paths[id]).str();
                pss->pointer = pss->content.c_str();
            }
            else if(ext == "kml")
            {
                mime = "application/vnd.google-earth.kml+xml";
                //try {
                    pss->content = kml_fromPath(&Session::paths[id]).str();
                    //std::cout << pss->content;
                    pss->pointer = pss->content.c_str();
                    //printf("kml size %d\n", pss->content.size());
                //} catch(...) return -1;
            }
            else
            {
                status = HTTP_STATUS_NOT_FOUND;
            }

            if (lws_add_http_common_headers(wsi, status, mime, LWS_ILLEGAL_HTTP_CONTENT_LEN /* no content len */, &p, end))
                return 1;

            const char* val = "attachment";
            if (lws_add_http_header_by_token(wsi, WSI_TOKEN_HTTP_CONTENT_DISPOSITION, (const uint8_t*)val, sizeof(val), &p, end))
                return 1;

            if (lws_finalize_write_http_header(wsi, start, &p, end))
                return 1;

            if (status == HTTP_STATUS_OK)
            {
                /* write the body separately */
                lws_callback_on_writable(wsi);
            }
            else if (lws_http_transaction_completed(wsi)) return 1;

            return 0;
        }

	case LWS_CALLBACK_HTTP_WRITEABLE: {
		if (!pss) break;
		/*
		 * We send a large reply in pieces of around 2KB each.
		 *
		 * For http/1, it's possible to send a large buffer at once,
		 * but lws will malloc() up a temp buffer to hold any data
		 * that the kernel didn't accept in one go.  This is expensive
		 * in memory and cpu, so it's better to stage the creation of
		 * the data to be sent each time.
		 *
		 * For http/2, large data frames would block the whole
		 * connection, not just the stream and are not allowed.  Lws
		 * will call back on writable when the stream both has transmit
		 * credit and the round-robin fair access for sibling streams
		 * allows it.
		 *
		 * For http/2, we must send the last part with
		 * LWS_WRITE_HTTP_FINAL to close the stream representing
		 * this transaction.
		 */

        const char* content_end = pss->content.c_str() + pss->content.size();

        uint8_t* p = start;
        size_t len = std::min(end - p, content_end - pss->pointer);
        //int l = lws_snprintf((char*)p, len, pss->pointer);
        memcpy(p, pss->pointer, len);
		p += len;
        pss->pointer += len;

        lws_write_protocol n = LWS_WRITE_HTTP;
		if (pss->pointer >= content_end) n = LWS_WRITE_HTTP_FINAL;

		if (lws_write(wsi, (uint8_t*)start, lws_ptr_diff(p, start), n) != lws_ptr_diff(p, start))
			return 1;

		/*
		 * HTTP/1.0 no keepalive: close network connection
		 * HTTP/1.1 or HTTP1.0 + KA: wait / process next transaction
		 * HTTP/2: stream ended, parent connection remains up
		 */
		if (n == LWS_WRITE_HTTP_FINAL) {
		    if (lws_http_transaction_completed(wsi))
            pss->pointer = NULL;
            pss->content = std::string("");
			return -1;
		} else
			lws_callback_on_writable(wsi);

		return 0;
    }

        default:
            break;
	}

	return lws_callback_http_dummy(wsi, reason, user, in, len);
}
