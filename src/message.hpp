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
typedef struct my_pss_t my_pss_t;

int server_decode(my_pss_t *pss, uint8_t *in, size_t len);
int server_pushall(mymsg_t msg);
int server_pushto(my_pss_t *pss, mymsg_t msg);

#ifdef  __cplusplus
}
#endif

#endif