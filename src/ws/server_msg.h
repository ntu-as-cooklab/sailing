#ifndef SERVER_MSG_H
#define SERVER_MSG_H

#include <stdint.h>
#include <stddef.h>

#ifdef  __cplusplus
extern  "C" {
#endif

int server_decode(uint8_t *in, size_t len);

#ifdef  __cplusplus
}
#endif

#endif