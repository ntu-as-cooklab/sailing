#ifndef MYPROTOCOL_H
#define MYPROTOCOL_H

#include <libwebsockets.h>

int callback_minimal(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

/* one of these is created for each client connecting to us */
struct per_session_data__minimal {
    struct per_session_data__minimal *pss_list;
    struct lws *wsi;
    int last; /* the last message number we sent */
};

#define LWS_PLUGIN_PROTOCOL_MINIMAL \
    { \
        "lws-minimal", \
        callback_minimal, \
        sizeof(struct per_session_data__minimal), \
        128, \
        0, NULL, 0 \
    }

#endif