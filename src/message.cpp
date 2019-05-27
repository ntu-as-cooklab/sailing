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

struct tm json2date(json& j)
{
    return (struct tm){.tm_hour=j[3], .tm_mday = j[2],.tm_mon = j[1],.tm_year = j[0]};
}

latlon_t json2loc(json& j)
{
    return (latlon_t){j[0], j[1]};
}

const char* datestr(struct tm *date)
{
    static char str[30];
    strftime(str, sizeof(str), "%Y-%m-%d %Hhr", date);
    return str;
}

void printpt(pathpt_t& pt)
{
    printf("%s %f,%f\n", datestr(&pt.date), pt.loc.lat, pt.loc.lon);
}

json date2json(struct tm& date)
{
    return json({date.tm_year, date.tm_mon, date.tm_mday, date.tm_hour});
}

json loc2json(latlon_t& loc)
{
    return json({loc.lat, loc.lon});
}


json path2json(path_t& path)
{
    json j;
    j["id"]     = path.id;
    j["user"]   = path.user;

    j["date"] = json::array();
    j["loc"]  = json::array();
    for (int i = 0; i < path.pts.size(); i++)
    {
        j["date"].push_back(date2json(path.pts[i].date));
        j["loc"].push_back(loc2json(path.pts[i].loc));
    }
    return j;
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

        json response = json({});
        response["cmd"]  = "new_path";
        response["path"] = path2json(path);
        response_cbor = json::to_cbor(response);
        server_pushmsg(&response_cbor);
    }

    return 0;
}