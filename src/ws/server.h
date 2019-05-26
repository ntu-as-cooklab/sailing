#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>
#include <libwebsockets.h>
#include <vector>

#ifdef  __cplusplus
extern  "C" {
#endif

extern struct lws_protocols protocols[];

#ifdef  __cplusplus
}
#endif

#endif