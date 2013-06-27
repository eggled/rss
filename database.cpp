#include "database.hpp"
#include <gtmxc_types.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <string.h>
#include <signal.h>
#include <linux/limits.h>
#include <fstream>

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
    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL);
    atexit((void (*)())&gtm_exit);
}

void Database::mark_read(string id)
{
	gtm_char_t * arg;
	arg = (gtm_char_t*) malloc((id.length()+1) * sizeof(gtm_char_t));
	strcpy(arg, id.c_str());
	gtm_status_t stat = gtm_ci("markread",arg);
	free(arg);
	if (0 != stat)
	{
		char buf[1024];
		gtm_zstatus(buf, 1023);
		cerr << "Failed to run callin: " << buf << endl;
	}
}

int Database::is_read(string id)
{
	gtm_char_t * arg;
	arg = (gtm_char_t*) malloc((id.length()+1) * sizeof(gtm_char_t));
	strcpy(arg, id.c_str());
	gtm_int_t ret;
	gtm_status_t stat = gtm_ci("isread",&ret, arg);
	free(arg);
	if (0 != stat)
	{
		char buf[1024];
		gtm_zstatus(buf, 1023);
		cerr << "Failed to run callin: " << buf << endl;
	}
	return (int) ret;
}

string Database::getfname(string id)
{
	gtm_char_t *idarg, *filename;
	filename = (gtm_char_t*) malloc((1+PATH_MAX) * sizeof(gtm_char_t));
	idarg = (gtm_char_t*) malloc((1+id.length()) * sizeof(gtm_char_t));
    strcpy(idarg, id.c_str());
    if (0 != gtm_ci("getfname", idarg, filename))
    {
		char buf[1024];
		gtm_zstatus(buf, 1023);
		cerr << "Failed to run callin: " << buf << endl;
    }
    free(idarg);
    string retval = filename;
    free(filename);
    return retval;
}

string Database::newfname(string id)
{
	gtm_char_t *idarg, *filename;
	filename = (gtm_char_t*) malloc((1+PATH_MAX) * sizeof(gtm_char_t));
	idarg = (gtm_char_t*) malloc((1+id.length()) * sizeof(gtm_char_t));
    strcpy(idarg, id.c_str());
    if (0 != gtm_ci("newfname", idarg, filename))
    {
		char buf[1024];
		gtm_zstatus(buf, 1023);
		cerr << "Failed to run callin: " << buf << endl;
    }
    free(idarg);
    string retval = filename;
    free(filename);
    retval = "html/doc/" + retval;
    return retval;
}

string Database::getcontent(string id)
{
    string file = this->getfname(id);
    file = "html/doc/" + file;
    string retval;
    char buffer [4096];
    ifstream fs(file.c_str());
    while (fs.read(buffer, sizeof(buffer)))
        retval.append(buffer, sizeof(buffer));
    retval.append(buffer, fs.gcount());
    return retval;
}
