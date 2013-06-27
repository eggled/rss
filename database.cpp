#include "database.hpp"
#include <gtmxc_types.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <string.h>
#include <signal.h>

#ifndef GTMDIST
DIE
#endif

using namespace std;

Database::Database()
{
    setenv("gtm_dist", GTMDIST, 1);
    setenv("GTMCI", (string(GTMDIST) + "/../db/gtmci.ci").c_str(), 1);
    setenv("gtmroutines", (string(GTMDIST) + "/../db/r").c_str(), 1);
    setenv("gtmgbldir", (string(GTMDIST) + "/../db/cur/g/gtm.gld").c_str(), 1);
    if (0 != gtm_init())
        throw "Failed to init GT.M";
    atexit((void (*)())&gtm_exit);
}

void Database::mark_read(string id)
{
    //gtm_string_t arg;
    gtm_char_t * arg;
    //arg.length = id.length();
    //arg.address = (gtm_char_t*) malloc((arg.length+1) * sizeof(gtm_char_t));
    arg = (gtm_char_t*) malloc((id.length()+1) * sizeof(gtm_char_t));
    strcpy(arg, id.c_str());
sigset_t mask;
sigfillset(&mask);
sigprocmask(SIG_SETMASK, &mask, NULL);
    gtm_status_t stat = gtm_ci("markread",arg);
free(arg);
   if (0 != stat)
{
	char buf[1024];
	gtm_zstatus(buf, 1023);
	cerr << "Failed to run callin: " << buf << endl;
}
}

