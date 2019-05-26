#ifndef MYPROTOCOL_H
#define MYPROTOCOL_H

#include <libwebsockets.h>

#ifdef  __cplusplus
extern  "C" {
#endif

int my_callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

#ifdef  __cplusplus
}
#endif

/* per session data: one of these is created for each client connecting to us */
typedef struct my_pss_t {
    struct my_pss_t *pss_list;
    struct lws      *wsi;
    int last; /* the last message number we sent */
} my_pss_t;

extern struct lws_protocols protocols[];

#endif