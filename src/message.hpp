#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <memory>

#ifdef  __cplusplus
extern  "C" {
#endif

typedef std::vector<uint8_t> mymsg_t;

int server_decode(uint8_t *in, size_t len);
int server_pushmsg(mymsg_t msg);

#ifdef  __cplusplus
}
#endif

#endif