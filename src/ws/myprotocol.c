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

/* one of these is created for each vhost our protocol is used with */
struct per_vhost_data__minimal {
    struct lws_context *context;
    struct lws_vhost *vhost;
    const struct lws_protocols *protocol;

    struct per_session_data__minimal *pss_list; /* linked-list of live pss*/

    struct msg amsg; /* the one pending message... */
    int current; /* the current message number we are caching */
};

struct lws_protocols protocols[] = {
    { "http", lws_callback_http_dummy, 0, 0 },
    {
        .name                   = "lws-minimal",
        .callback               = callback_minimal,
        .per_session_data_size  = sizeof(struct per_session_data__minimal),
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

int callback_minimal(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    struct per_session_data__minimal *pss = (struct per_session_data__minimal *)user;
    struct per_vhost_data__minimal *vhd = (struct per_vhost_data__minimal *) lws_protocol_vh_priv_get(lws_get_vhost(wsi), lws_get_protocol(wsi));
    int m;

    switch (reason) {
        case LWS_CALLBACK_PROTOCOL_INIT:
            lwsl_user("LWS_CALLBACK_PROTOCOL_INIT\n");
            vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi), lws_get_protocol(wsi), sizeof(struct per_vhost_data__minimal));
            vhd->context = lws_get_context(wsi);
            vhd->protocol = lws_get_protocol(wsi);
            vhd->vhost = lws_get_vhost(wsi);
            break;

        case LWS_CALLBACK_ESTABLISHED:
            lwsl_user("LWS_CALLBACK_ESTABLISHED\n");
            /* add ourselves to the list of live pss held in the vhd */
            lws_ll_fwd_insert(pss, pss_list, vhd->pss_list);
            pss->wsi = wsi;
            pss->last = vhd->current;
            break;

        case LWS_CALLBACK_CLOSED:
            lwsl_user("LWS_CALLBACK_CLOSED\n");
            /* remove our closing pss from the list of live pss */
            lws_ll_fwd_remove(struct per_session_data__minimal, pss_list, pss, vhd->pss_list);
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE:
            lwsl_user("LWS_CALLBACK_SERVER_WRITEABLE\n");

            if (!vhd->amsg.payload)
                    break;
            if (pss->last == vhd->current)
                    break;

            /* notice we allowed for LWS_PRE in the payload already */
            // m = lws_write(wsi, ((unsigned char *)vhd->amsg.payload) + LWS_PRE, vhd->amsg.len, LWS_WRITE_TEXT);
            // if (m < (int)vhd->amsg.len) {
            //     lwsl_err("ERROR %d writing to ws\n", m);
            //     return -1;
            // }

            pss->last = vhd->current;
            break;

        case LWS_CALLBACK_RECEIVE:

            lwsl_user("LWS_CALLBACK_RECEIVE\n");

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
            lws_start_foreach_llp(struct per_session_data__minimal **, ppss, vhd->pss_list) {
                lws_callback_on_writable((*ppss)->wsi);
            } lws_end_foreach_llp(ppss, pss_list);
            break;

        default:
            break;
    }

    return 0;
}
