#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "model.hpp"
#include "message.hpp"
#include "session.hpp"

using namespace std;
using namespace nlohmann;

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

std::string datestr(struct tm *date)
{
    char str[30];
    strftime(str, sizeof(str), "%Y-%m-%d %Hhr", date);
    return std::string(str);
}

void printpt(pathpt_t pt)
{
    printf("%s %f,%f\n", datestr(&pt.date).c_str(), pt.loc.lat, pt.loc.lon);
}

json path2json(path_t* path)
{
    json j = {
        {"id", path->id},
        {"runId", path->runId},
        {"loginID", path->loginID},
        {"startdate", date2json(path->startdate)},
        {"enddate", date2json(path->enddate)},
        {"startloc", loc2json(path->startloc)},
        {"land_collision", path->land_collision},
        {"dataset", path->dataset},
        {"mode", path->mode},
        {"color", path->color},
        {"destloc", loc2json(path->destloc)},
        {"destheading", path->destheading},
        {"altitude", path->altitude},
        {"windlimit", path->windlimit},
        {"sailstarthour", path->sailstarthour},
        {"sailendhour", path->sailendhour},
        {"alpha", path->alpha},
        {"date", json::array()},
        {"loc", json::array()},
    };
    for (int i = 0; i < path->pts.size(); i++)
    {
        j["date"].push_back(date2json(path->pts[i].date));
        j["loc"].push_back(loc2json(path->pts[i].loc));
    }
    return j;
}

path_t json2path(json j)
{
    path_t path = {};
    if (!j["id"].is_null())             path.id = j["id"];
    if (!j["runId"].is_null())          path.runId = j["runId"];
    if (!j["loginID"].is_null())        path.loginID = j["loginID"];
    if (!j["startdate"].is_null())      path.startdate = json2date(j["startdate"]);
    if (!j["enddate"].is_null())        path.enddate = json2date(j["enddate"]);
    if (!j["startloc"].is_null())       path.startloc = json2loc(j["startloc"]);
    if (!j["land_collision"].is_null()) path.land_collision = j["land_collision"];

    if (!j["dataset"].is_null())        path.dataset = j["dataset"];
    if (!j["mode"].is_null())           path.mode = j["mode"];
    if (!j["color"].is_null())          path.color = j["color"];
    if (!j["destloc"].is_null())        path.destloc   = json2loc(j["destloc"]);
    if (!j["destheading"].is_null())    path.destheading = j["destheading"];
    if (!j["altitude"].is_null())       path.altitude = j["altitude"];
    if (!j["windlimit"].is_null())      path.windlimit = j["windlimit"];
    if (!j["sailstarthour"].is_null())  path.sailstarthour = j["sailstarthour"];
    if (!j["sailendhour"].is_null())    path.sailendhour = j["sailendhour"];
    if (!j["alpha"].is_null())          path.alpha = j["alpha"];

    if (!j["date"].is_null() && !j["loc"].is_null())
        for (json::iterator date = j["date"].begin(), loc = j["loc"].begin(); date != j["date"].end(); ++date, ++loc) {
            pathpt_t pt = {.date = json2date(*date), .loc = json2loc(*loc)};
            path.pts.push_back(pt);
        }
    return path;
}

mymsg_t msg;

json server_newpath(path_t* path)
{
    return {{"cmd", "new_path"},
            {"path", path2json(path)}};
}

json server_sendpts(path_t* path, int last_step, int step)
{
    json j = {
        {"cmd", "pts"},
        {"runId", path->runId},
        {"loginID", path->loginID},
        {"id", path->id},
        {"step", last_step},
        {"date", json::array()},
        {"loc", json::array()},
    };
    for (int i = last_step; i <= step; i++) {
        j["date"].push_back(date2json(path->pts[i].date));
        j["loc"].push_back(loc2json(path->pts[i].loc));
    }
    return j;
}

int server_decode(my_pss_t *pss, uint8_t *in, size_t len)
{
    try {
        json j = json::from_cbor(mymsg_t(in, in + len));
    
        cout << "Message: " << j << "\n";

        if (j["cmd"] == "new_path")
        {
            uint32_t id = Session::new_path();
            path_t* path = &Session::paths[id];
            *path = json2path(j);
            path->id = id;

            server_pushall(json::to_cbor(server_newpath(path)));

            int step = 0;
            int last_step = 0;
            path->pts.push_back({path->startdate, path->startloc});
            while(mktime(&path->pts.back().date) < mktime(&path->enddate)) {
                if (sail_step(path) != 0) { printf("Land collision!\n"); path->land_collision = true; path->enddate = path->pts.back().date; }
                else {
                    step++;
                    if (step%(24*5) == 0) {
                        // send incrementally
                        server_pushall(json::to_cbor(server_sendpts(path, last_step, step)));
                        last_step = step;
                    }
                }
            }
            printf("id=%d ", path->id); printpt(path->pts.back());
            if (last_step < step) {
                server_pushall(json::to_cbor(server_sendpts(path, last_step, step))); // send remaining data here
            }
            if (path->land_collision) server_pushall(json::to_cbor({{"cmd", "land_collision"},{"runId", path->runId},{"id", path->id}}));
        }
        else if (j["cmd"] == "restore")
        {
            printf("restore 0x%x\n", pss);

            map<uint32_t, path_t>::iterator it;
            for (it = Session::paths.begin(); it != Session::paths.end(); it++)
            {
                path_t* path = &it->second;
                server_pushto(pss, json::to_cbor(server_newpath(path)));
                server_pushto(pss, json::to_cbor(server_sendpts(path, 0, path->pts.size()-1)));
            }
            server_pushto(pss, json::to_cbor({{"cmd", "ready"}}));
        }
        else if (j["cmd"] == "delete")
        {
            uint32_t id = j["id"];
            uint32_t runId = j["runId"];
            std::string loginID = j["loginID"];
            if (Session::paths[id].runId == runId && Session::paths[id].loginID == loginID)
            {
                Session::paths.erase(id);
                server_pushall(json::to_cbor({{"cmd", "delete"},{"runId", runId},{"id", id}}));
            }
        }

    }
    catch(const std::runtime_error& re)
    {
        // specific handling for runtime_error
        std::cerr << "Runtime error: " << re.what() << std::endl;
    }
    catch(const std::exception& ex)
    {
        // specific handling for all exceptions extending std::exception, except
        // std::runtime_error which is handled explicitly
        std::cerr << "Error occurred: " << ex.what() << std::endl;
    }
    catch(...)
    {
        // catch any other errors (that we have no information about)
        std::cerr << "Unknown failure occurred" << std::endl;
    }

    return 0;
}