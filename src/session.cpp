#include "session.hpp"
#include <stdio.h>

std::map<uint32_t, path_t> Session::paths;
static uint32_t next_pathid = 0;

int Session::new_path()
{
    printf("New path: id=%d\n", next_pathid);
    Session:paths[next_pathid] = (path_t){.id = next_pathid};
    return next_pathid++;
}