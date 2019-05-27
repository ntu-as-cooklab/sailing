#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>
#include <libwebsockets.h>
#include <vector>

#define SERVER_PROTOCOL "lws_minimal"

#ifdef  __cplusplus
extern  "C" {
#endif

int server_init(void);
void server_run(void);
void server_stop(void);

#ifdef  __cplusplus
}
#endif

#endif