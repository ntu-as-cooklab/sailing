#include "session.hpp"

std::map<uint32_t, path_t> Session::paths;
static uint32_t next_pathid = 0;

int Session::new_path()
{
    Session:paths[next_pathid];
    return next_pathid++;
}