#include "server_msg.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include "sailing.hpp"

using namespace std;
using namespace nlohmann;

int server_decode(uint8_t *in, size_t len)
{
    vector<uint8_t> v_cbor(in, in + len);
    json j = json::from_cbor(v_cbor);
    cout << j << "\n";

    if (j["cmd"] == "newPath")
    {
        path_t path;
        struct tm date = {.tm_hour=j["date"][3], .tm_mday = j["date"][2],.tm_mon = j["date"][1],.tm_year = j["date"][0]};
        latlon_t orig = {j["orig"][0], j["orig"][1]};
        path.pts.push_back((pathpt_t){date, orig});
        for (int i = 0; i < 48; i++)
            sail_step(path);

        json jpath({});
        size_t len = path.pts.size();
        for (int i = 0; i < len; i++)
            jpath[i] = {
                { "date", {path.pts[i].date.tm_year, path.pts[i].date.tm_mon, path.pts[i].date.tm_mday, path.pts[i].date.tm_hour}},
                { "loc", {path.pts[i].loc.lat, path.pts[i].loc.lon}},
            };
        vector<uint8_t> jpath_cbor = json::to_cbor(jpath_cbor);
    }

    return 0;
}