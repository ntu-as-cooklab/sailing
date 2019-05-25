#include <cbor.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include "sailing.hpp"
#include "server_msg.h"
#include <nlohmann/json.hpp>
#include <iostream>

using namespace std;
using namespace nlohmann;

int server_decode(uint8_t *in, size_t len)
{
    vector<uint8_t> v_cbor(in, in + len);
    json j = json::from_cbor(v_cbor);

    if (j["command"] == "newPath")
    {
        path_t path;
        latlon_t orig = {j["orig"]["lat"], j["orig"]["lng"]};
        path.pts.push_back((pathpt_t){{.tm_mday = 1,.tm_mon = 0,.tm_year = 79}, orig});
        for (int i = 0; i < 48; i++)
            sail_step(path);
    }

    return 0;
}