#include <string.h>
#include <stdlib.h>
#include <libwebsockets.h>
#include "ws/myprotocol.h"
#include "server_msg.h"

#define LWS_PLUGIN_STATIC

/* one of these created for each message */
struct msg {
    void *payload; /* is malloc'd */
    size_t len;
};

/* per vhost data: one of these is created for each vhost our protocol is used with */
typedef struct my_vhd_t {
    struct lws_context          *context;
    struct lws_vhost            *vhost;
    const struct lws_protocols  *protocol;
    struct my_pss_t             *pss_list; /* linked-list of live pss */

    struct msg amsg; /* the one pending message... */
    int current; /* the current message number we are caching */
} my_vhd_t;

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

/* destroys the message when everyone has had a copy of it */
static void __minimal_destroy_message(void *_msg)
{
    struct msg *msg = _msg;

    free(msg->payload);
    msg->payload = NULL;
    msg->len = 0;
}

int my_callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    struct my_pss_t *pss = (struct my_pss_t *)user;
    struct my_vhd_t *vhd = (struct my_vhd_t *)lws_protocol_vh_priv_get(lws_get_vhost(wsi), lws_get_protocol(wsi));

    switch (reason) {
        case LWS_CALLBACK_PROTOCOL_INIT:
            //lwsl_user("LWS_CALLBACK_PROTOCOL_INIT\n");
            vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi), lws_get_protocol(wsi), sizeof(my_vhd_t));
            vhd->context    = lws_get_context(wsi);
            vhd->protocol   = lws_get_protocol(wsi);
            vhd->vhost      = lws_get_vhost(wsi);
            break;

        case LWS_CALLBACK_ESTABLISHED:
            /* add ourselves to the list of live pss held in the vhd */
            lws_ll_fwd_insert(pss, pss_list, vhd->pss_list);
            pss->wsi    = wsi;
            pss->last   = vhd->current;
            break;

        case LWS_CALLBACK_CLOSED:
            /* remove our closing pss from the list of live pss */
            lws_ll_fwd_remove(my_pss_t, pss_list, pss, vhd->pss_list);
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE: {
            if (!vhd->amsg.payload)
                    break;
            if (pss->last == vhd->current)
                    break;

            /* notice we allowed for LWS_PRE in the payload already */
            // m = lws_write(wsi, ((unsigned char *)vhd->amsg.payload) + LWS_PRE, vhd->amsg.len, LWS_WRITE_TEXT);
            // if (int m < (int)vhd->amsg.len) {
            //     lwsl_err("ERROR %d writing to ws\n", m);
            //     return -1;
            // }

            pss->last = vhd->current;
        } break;

        case LWS_CALLBACK_RECEIVE: {
            server_decode(in, len);

            if (vhd->amsg.payload)
                __minimal_destroy_message(&vhd->amsg);

            vhd->amsg.len = len;
            /* notice we over-allocate by LWS_PRE */
            vhd->amsg.payload = malloc(LWS_PRE + len);
            if (!vhd->amsg.payload) {
                lwsl_user("OOM: dropping\n");
                break;
            }

            memcpy((char *)vhd->amsg.payload + LWS_PRE, in, len);
            vhd->current++;

            /* let everybody know we want to write something on them as soon as they are ready */
            lws_start_foreach_llp(my_pss_t **, ppss, vhd->pss_list) {
                lws_callback_on_writable((*ppss)->wsi);
            } lws_end_foreach_llp(ppss, pss_list);
        } break;

        default:
            break;
    }

    return 0;
}
