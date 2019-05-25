#include <cbor.h>
#include <stdio.h>
#include <string.h>

int server_decode(void *in, size_t len)
{
    struct cbor_load_result result;
    cbor_item_t *msg = cbor_load(in, len, &result);
    if (msg != NULL)
    {
        cbor_describe(msg, stdout);
        for (int i = 0; i < cbor_map_size(msg); i++)
        {
            const char* name = (const char*)cbor_string_handle(cbor_map_handle(msg)[i].key);
            if (strcmp(name, "SDM.TIN_MUX_on") == 0)
            {
                //Global.Debug.TIN_MUX_on = cbor_get_uint8(cbor_map_handle(msg)[i].value);
                //lprintf("Global.Debug.TIN_MUX_on: %u\n", Global.Debug.TIN_MUX_on);
            }
        }
        cbor_decref(&msg);
    }
}