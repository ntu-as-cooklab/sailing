#include "session.hpp"
#include <stdio.h>
#include <nlohmann/json.hpp>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

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

int Session::writeBson(path_t* path)
{
    struct stat st = {0};
    if (stat("session", &st) == -1)
        mkdir("session", 0755);

    std::vector<std::uint8_t> v_bson = json::to_bson(path2json(path));
    FILE *write_ptr;
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "session/%04d.bson", path->id);
    write_ptr = fopen(filepath,"wb");
    fwrite(v_bson.data(), 1, v_bson.size(), write_ptr);
    return 0;
}

path_t json2path(json j);

int Session::loadBson()
{
    DIR *dfd;
    if ((dfd = opendir("session")) == NULL)
        return -1;

    struct dirent *dp;
    while ((dp = readdir(dfd)) != NULL)
    {
        struct stat stbuf;
        char filename[100];
        sprintf(filename, "session/%s", dp->d_name);

        char *dot = strrchr(filename, '.');
        if (dot && strcmp(dot, ".bson"))
            continue;
        if (stat(filename, &stbuf) == -1)
            continue;
        if (stbuf.st_mode & S_IFMT == S_IFDIR)
            continue; // Skip directories
        
        printf("File: %s\n", filename);
        FILE *fp;
        fp = fopen(filename,"rb");
        if (!fp) return -1;
        fseek(fp, 0L, SEEK_END);
        size_t sz = ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        uint8_t buffer[sz];
        fread(buffer, sizeof(buffer), 1, fp);
        std::vector<std::uint8_t> v_bson(buffer, buffer+sz);
        json j = json::from_bson(v_bson);

        uint32_t id = j["id"];
        Session::paths[id] = json2path(j);
        if (id>= next_pathid) next_pathid = id + 1;
    }

    return 0;
};