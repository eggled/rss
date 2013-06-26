#include "database.hpp"
#include <gtmxc_types.h>
#include <stdlib.h>
#include <string>

#ifndef GTMDIST
DIE
#endif

using namespace std;

Database::Database()
{
    setenv("gtm_dist", GTMDIST, 1);
    if (0 != gtm_init())
        throw "Failed to init GT.M";
}

void Database::mark_read(string id)
{
    gtm_string_t arg;
    arg.length = id.length();
    arg.address = const_cast<gtm_char_t*> (id.c_str());
    gtm_ci("markread",arg);
}

Database::~Database()
{
    gtm_exit();
}
