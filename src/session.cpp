#include "session.hpp"
#include <stdio.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <mongoc/mongoc.h>

std::map<uint32_t, path_t> Session::paths;
static uint32_t next_pathid = 0;

using namespace std;
using namespace nlohmann;

int Session::new_path()
{
    printf("New path: id=%d\n", next_pathid);
    Session:paths[next_pathid] = (path_t){.id = next_pathid};
    return next_pathid++;
}

json path2json(path_t* path);


int Session::writeBson()
{
    json paths;
    map<uint32_t, path_t>::iterator it;
    for (it = Session::paths.begin(); it != Session::paths.end(); it++)
    {
        path_t* path = &it->second;
        paths[path->id] = path2json(path);
    }
    json session = json::object();
    session["paths"] = paths;
    std::vector<std::uint8_t> v_bson = json::to_bson(session);

    FILE *write_ptr;
    write_ptr = fopen("session.bson","wb");
    fwrite(v_bson.data(), 1, v_bson.size(), write_ptr);
    return 0;
}

path_t json2path(json j);

int Session::loadBson()
{
    FILE *fp;
    fp = fopen("session.bson","rb");
    if (!fp) return -1;
    fseek(fp, 0L, SEEK_END);
    size_t sz = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    uint8_t buffer[sz];
    fread(buffer, sizeof(buffer), 1, fp);
    std::vector<std::uint8_t> v_bson(buffer, buffer+sz);
    json session = json::from_bson(v_bson);

    json paths = session["paths"];
    for (json::iterator it = paths.begin(); it != paths.end(); ++it) {
        uint32_t id = (*it)["id"];
        Session::paths[id] = json2path(*it);
        if (id>= next_pathid) next_pathid = id + 1;
    }

    return 0;
};