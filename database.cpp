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

gtm_char_t * string_to_gtm_char_t(string input)
{
    gtm_char_t *retval;
	retval = (gtm_char_t*) malloc((input.length()+1) * sizeof(gtm_char_t));
    if (retval == NULL) 
    {
        perror("Failed to malloc");
        exit(1);
    }
    strcpy(retval, input.c_str());
    return retval;
}

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

void Database::markread(string id)
{
	gtm_char_t * arg = string_to_gtm_char_t(id);
	gtm_status_t stat = gtm_ci("markread",arg);
	free(arg);
	if (0 != stat)
	{
		char buf[1024];
		gtm_zstatus(buf, 1023);
		cerr << "Failed to run callin markread: " << buf << endl;
	}
}

int Database::is_read(string id)
{
	gtm_char_t *arg = string_to_gtm_char_t(id);
	gtm_int_t ret;
	gtm_status_t stat = gtm_ci("isread",&ret, arg);
	free(arg);
	if (0 != stat)
	{
		char buf[1024];
		gtm_zstatus(buf, 1023);
		cerr << "Failed to run callin isread: " << buf << endl;
	}
	return (int) ret;
}

string Database::getfname(string id)
{
	gtm_char_t *idarg = string_to_gtm_char_t(id);
	gtm_char_t *filename = (gtm_char_t*) malloc((1+PATH_MAX) * sizeof(gtm_char_t));
    if (0 != gtm_ci("getfname", idarg, filename))
    {
		char buf[1024];
		gtm_zstatus(buf, 1023);
		cerr << "Failed to run callin getfname: " << buf << endl;
    }
    free(idarg);
    string retval = filename;
    free(filename);
    if (0 < retval.length())
	    retval = "html/doc/" + retval;
    return retval;
}

string Database::newfname(string id)
{
	gtm_char_t *idarg, *filename;
	filename = (gtm_char_t*) malloc((1+PATH_MAX) * sizeof(gtm_char_t));
	idarg = string_to_gtm_char_t(id);
    if (0 != gtm_ci("newfname", idarg, filename))
    {
		char buf[1024];
		gtm_zstatus(buf, 1023);
		cerr << "Failed to run callin newfname: " << buf << endl;
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
    string retval;
    char buffer [4096];
    ifstream fs(file.c_str());
    while (fs.read(buffer, sizeof(buffer)))
        retval.append(buffer, sizeof(buffer));
    retval.append(buffer, fs.gcount());
    return retval;
}

void Database::setfields(string guid, string title, string link, unsigned long pubDate, string description, string creator, string publisher, string publink)
{
    gtm_char_t *g_guid = string_to_gtm_char_t(guid);
    gtm_char_t *g_title = string_to_gtm_char_t(title);
    gtm_char_t *g_link = string_to_gtm_char_t(link);
    gtm_ulong_t g_pubDate = pubDate;
    gtm_char_t *g_description = string_to_gtm_char_t(description);
    gtm_char_t *g_creator = string_to_gtm_char_t(creator);
    gtm_char_t *g_publisher = string_to_gtm_char_t(publisher);
    gtm_char_t *g_publink = string_to_gtm_char_t(publink);

    if (0 != gtm_ci("setfields",g_guid,g_title,g_link,g_pubDate,g_description,g_creator,g_publisher,g_publink))
    {
        char buf[1024];
        gtm_zstatus(buf, 1023);
        cerr << "Failed to run callin setfields: " << buf << endl;
    }
    free(g_guid);
    free(g_title);
    free(g_link);
    free(g_description);
    free(g_creator);
    free(g_publisher);
    free(g_publink);
}

