#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include "sailing.hpp"
#include "message.hpp"

using namespace std;
using namespace nlohmann;

// should use shared pointer
mymsg_t response_cbor;

struct tm json2date(json j)
{
    return (struct tm){.tm_hour=j[3], .tm_mday = j[2],.tm_mon = j[1],.tm_year = j[0]};
}

latlon_t json2loc(json j)
{
    return (latlon_t){j[0], j[1]};
}

const char* datestr(struct tm *date)
{
    static char str[30];
    strftime(str, sizeof(str), "%Y-%m-%d %Hhr", date);
    return str;
}

void printpt(pathpt_t pt)
{
    printf("%s %f,%f\n", datestr(&pt.date), pt.loc.lat, pt.loc.lon);
}

int server_decode(uint8_t *in, size_t len)
{
    json j;
    try {
        j = json::from_cbor(mymsg_t(in, in + len));
    } catch (...) {}
    cout << j << "\n";

    if (j["cmd"] == "new_path")
    {
        path_t path = {.user = j["user"]};

        struct tm startdate = json2date(j["startdate"]);
        struct tm enddate   = json2date(j["enddate"]);
        latlon_t startloc   = json2loc(j["startloc"]);
        
        path.pts.push_back((pathpt_t){startdate, startloc});
        printpt(path.pts.back());
        while(mktime(&path.pts.back().date) < mktime(&enddate))
            sail_step(path);
        printpt(path.pts.back());

        json jpath = json::array();
        size_t len = path.pts.size();
        for (int i = 0; i < len; i++)
            jpath.push_back({
                { "date", {path.pts[i].date.tm_year, path.pts[i].date.tm_mon, path.pts[i].date.tm_mday, path.pts[i].date.tm_hour}},
                { "loc", {path.pts[i].loc.lat, path.pts[i].loc.lon}},
            });
        json response = json({});
        response["cmd"]  = "new_path";
        response["path"] = jpath;
        response_cbor = json::to_cbor(response);
        server_pushmsg(&response_cbor);
    }

    return 0;
}