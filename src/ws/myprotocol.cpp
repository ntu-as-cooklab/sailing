#include <string.h>
#include <stdlib.h>
#include <libwebsockets.h>
#include "server_msg.hpp"
#include "server.h"
#include <vector>
#include <queue>

#define LWS_PLUGIN_STATIC

/* per session data: one of these is created for each client connecting to us */
typedef struct my_pss_t {
    struct my_pss_t *pss_list;
    struct lws      *wsi;

    std::queue<mymsg_t*> msgq; // message queue
} my_pss_t;

/* per vhost data: one of these is created for each vhost our protocol is used with */
typedef struct my_vhd_t {
    struct lws_context          *context;
    struct lws_vhost            *vhost;
    const struct lws_protocols  *protocol;
    my_pss_t                    *pss_list; /* linked-list of live pss */
} my_vhd_t;

int my_callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

struct lws_protocols protocols[] = {
    { "http", lws_callback_http_dummy, 0, 0 },
    {
        .name                   = "lws-minimal",
        .callback               = my_callback,
        .per_session_data_size  = sizeof(my_pss_t),
        .rx_buffer_size         = 1024,
        .id                     = 0,
        .user                   = NULL,
        .tx_packet_size         = 0,
    },
    { NULL, NULL, 0, 0 } /* terminator */
};

my_vhd_t *myvhd = NULL;

int server_pushmsg(mymsg_t* msg)
{
    my_vhd_t *vhd = myvhd;
    if (!vhd) return -1;

    lws_start_foreach_llp(my_pss_t **, ppss, vhd->pss_list) {
        msg->insert(msg->begin(), LWS_PRE, 0x0);
        (*ppss)->msgq.push(msg);
        lws_callback_on_writable((*ppss)->wsi);
    } lws_end_foreach_llp(ppss, pss_list);

    return 0;
}

int my_callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
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
            lws_ll_fwd_insert(pss, pss_list, vhd->pss_list); /* add ourselves to the list of live pss held in the vhd */
            pss->wsi    = wsi;
            pss->msgq   = std::queue<mymsg_t*>();
            break;

        case LWS_CALLBACK_CLOSED:
            lws_ll_fwd_remove(my_pss_t, pss_list, pss, vhd->pss_list); /* remove our closing pss from the list of live pss */
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE: {
            if(pss->msgq.empty()) break;
            
            /* notice we allowed for LWS_PRE in the payload already */
            int m = lws_write(wsi, pss->msgq.front()->data() + LWS_PRE, pss->msgq.front()->size(), LWS_WRITE_BINARY);
            if (m < (int)pss->msgq.front()->size()) {
                lwsl_err("ERROR %d writing to ws\n", m);
                return -1;
            }
            pss->msgq.pop();

            if(!pss->msgq.empty()) lws_callback_on_writable(wsi);
        } break;

        case LWS_CALLBACK_RECEIVE:
            server_decode((uint8_t*)in, len);
            break;

        default:
            break;
    }

    return 0;
}
