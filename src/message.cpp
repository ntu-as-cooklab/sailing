#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "sailing.hpp"
#include "message.hpp"
#include "session.hpp"

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

json date2json(struct tm& date)
{
    return json({date.tm_year, date.tm_mon, date.tm_mday, date.tm_hour});
}

json loc2json(latlon_t& loc)
{
    return json({loc.lat, loc.lon});
}

json path2json(path_t* path, int step)
{
    json j;
    j["id"]     = path->id;
    j["user"]   = path->user;
    j["startdate"] = date2json(path->startdate);
    j["enddate"] = date2json(path->enddate);
    j["startloc"] = loc2json(path->startloc);

    j["step"] = step;
    j["date"] = json::array();
    j["loc"]  = json::array();
    for (int i = step; i < path->pts.size(); i++)
    {
        j["date"].push_back(date2json(path->pts[i].date));
        j["loc"].push_back(loc2json(path->pts[i].loc));
    }
    return j;
}

std::string datestr(struct tm *date)
{
    char str[30];
    strftime(str, sizeof(str), "%Y-%m-%d %Hhr", date);
    return std::string(str);
}

// void update_path(path_t* path, int i0, int i1)
// {
//     mymsg_t msg = json::to_cbor({{"cmd", "new_path"},{"path", path2json(path)}});
//     server_pushmsg(&msg);
// }

void printpt(pathpt_t pt)
{
    printf("%s %f,%f\n", datestr(&pt.date).c_str(), pt.loc.lat, pt.loc.lon);
}

mymsg_t msg;

int server_decode(uint8_t *in, size_t len)
{
    json j;
    try {
        j = json::from_cbor(mymsg_t(in, in + len));
    } catch (...) {}
    cout << "Message: " << j << "\n";

    if (j["cmd"] == "new_path")
    {
        uint32_t id = Session::new_path();
        path_t* path = &Session::paths[id];
        path->id         = id;
        path->user       = j["user"];
        path->startdate  = json2date(j["startdate"]);
        path->enddate    = json2date(j["enddate"]);
        path->startloc   = json2loc(j["startloc"]);

        server_pushmsg(json::to_cbor({{"cmd", "new_path"},
            {"path", {
                {"user", path->user},
                {"id", path->id},
                {"startdate", date2json(path->startdate)},
                {"enddate", date2json(path->enddate)},
                {"startloc", loc2json(path->startloc)}
            }
        }}));

        //int i = path->pts.size();
        // path->pts.push_back((pathpt_t){path->startdate, path->startloc});
        // printpt(path->pts.back());
        int step = 0;
        int last_step = 0;
        path->pts.push_back({path->startdate, path->startloc});
        while(mktime(&path->pts.back().date) < mktime(&path->enddate)) {
            sail_step(path);
            step++;
            if (step%(24*5) == 0) {
                printf("id=%d ", path->id);
                printpt(path->pts.back());

                // send incrementally
                json j1 = {
                    {"cmd", "pts"},
                    {"user", path->user},
                    {"id", path->id},
                    {"step", last_step},
                    {"date", json::array()},
                    {"loc", json::array()},
                };
                for (int i = last_step; i <= step; i++) {
                    j1["date"].push_back(date2json(path->pts[i].date));
                    j1["loc"].push_back(loc2json(path->pts[i].loc));
                }
                server_pushmsg(json::to_cbor(j1));

                last_step = step;
            }
        }
        // printpt(path->pts.back());

        // json response = json({});
        // response["cmd"]  = "new_path";
        // response["path"] = path2json(*path);
        // response_cbor = json::to_cbor(response);
        // server_pushmsg(&response_cbor);
    }

    return 0;
}