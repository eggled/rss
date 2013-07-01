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
    {
	cerr << "Failed to init GT.M" << endl;
	exit(1);
    }
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
		cerr << "Callin for markread failed: " << buf << endl;
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
		cerr << "Callin for isread failed: " << buf << endl;
	}
	return (int) ret;
}

string Database::getfname(string id, int is_desc)
{
	gtm_char_t *idarg = string_to_gtm_char_t(id);
	gtm_char_t *filename = (gtm_char_t*) malloc((1+PATH_MAX) * sizeof(gtm_char_t));
    gtm_int_t desc = is_desc;
    if (0 != gtm_ci("getfname", idarg, filename, desc))
    {
		char buf[1024];
		gtm_zstatus(buf, 1023);
		cerr << "Callin for getfname failed: " << buf << endl;
    }
    free(idarg);
    string retval = filename;
    free(filename);
    if (0 < retval.length())
	    retval = "html/doc/" + retval;
    return retval;
}

string Database::newfname(string id, int is_desc)
{
	gtm_char_t *idarg, *filename;
    gtm_int_t desc = is_desc;
	filename = (gtm_char_t*) malloc((1+PATH_MAX) * sizeof(gtm_char_t));
	idarg = string_to_gtm_char_t(id);
    if (0 != gtm_ci("newfname", idarg, filename, desc))
    {
		char buf[1024];
		gtm_zstatus(buf, 1023);
		cerr << "Callin for newfname failed: " << buf << endl;
    }
    free(idarg);
    string retval = filename;
    free(filename);
    retval = "html/doc/" + retval;
    return retval;
}

void Database::addurl(string which)
{
    gtm_char_t *url = string_to_gtm_char_t(which);
    if (0 != gtm_ci("addurl", url))
    {
	    char buf[1024];
	    gtm_zstatus(buf, 1023);
	    cerr << "Callin for addurl failed: " << buf << endl;
    }
}

string Database::getcontent(string id)
{
    string file = this->getfname(id);
    string retval;
    char buffer [4096];
    ifstream fs(file.c_str());
    while (fs.read(buffer, sizeof(buffer)))
    {
        retval.append(buffer, sizeof(buffer));
    }
    retval.append(buffer, fs.gcount());
    fs.close();
    if (0 == retval.length())
    {
        file = this->getfname(id, 1);
        fs.open(file.c_str(), ifstream::in);
        while (fs.read(buffer, sizeof(buffer)))
        {
            retval.append(buffer, sizeof(buffer));
        }
        retval.append(buffer, fs.gcount());
        fs.close();
    }
    size_t srcpos;
    string publink;
    this->get(id, "publink", publink);
    if (publink.substr(0,1) != string("/"))
    {
        while ((srcpos = retval.find("src=\"//")) != string::npos)
        {
            retval.replace(srcpos,7, string("src=\"http://"));
        }
        while ((srcpos = retval.find("src=\"/")) != string::npos)
        {
            retval.replace(srcpos,6, string("src=\"") + publink + "/");
        }
    }
    if (publink.substr(0,26) != string("http://cdn.arstechnica.net"))
    {
        while ((srcpos = retval.find("http://cdn.arstechnica.net")) != string::npos)
        {
            retval.replace(srcpos, 26, string("http://arstechnica.com"));
        }
    }
    return retval;
}

void Database::get(string id, string which, string &value)
{
	gtm_char_t * cid = string_to_gtm_char_t(id);
	gtm_char_t * cwhich = string_to_gtm_char_t(which);
	gtm_char_t buffer[1025];
	buffer[1024] = '\0';
	if (0 != gtm_ci("get", cid, cwhich, buffer))
	{
		char buf[1024];
		gtm_zstatus(buf, 1023);
		cerr << "Callin for " << which << " failed: " << buf << endl;
	}
	value = buffer;
	free(cid);
	free(cwhich);
}

bool Database::getmetadata(string &id, string &title, string &link, string &publink, string &publisher, string &creator)
{
// 1025 because it's larger than key size (255)
    gtm_char_t cid[1025];
// 4097 because it's larger than the record size.
    gtm_char_t ctitle[4097], clink[4097], cpublink[4097], cpublisher[4097], ccreator[4097];
ctitle[4096] = '\0';
clink[4096] = '\0';
cpublink[4096] = '\0';
cpublisher[4096] = '\0';
ccreator[4096] = '\0';

    strncpy(cid, id.c_str(), 1024);

    if (0 != gtm_ci("getmetadata", cid, ctitle, clink, cpublink, cpublisher,ccreator))
    {
		char buf[1024];
		gtm_zstatus(buf, 1023);
		cerr << "Callin for getmetadata failed: " << buf << endl;
    }

    id = cid;
    title = ctitle;
    link = clink;
    publink = cpublink;
    publisher = cpublisher;
    creator = ccreator;
    if (0 == id.length())
        return 0;
    return 1;
}

void Database::setfields(string guid, string title, string link, unsigned long pubDate, string creator, string publisher, string publink)
{
    gtm_char_t *g_guid = string_to_gtm_char_t(guid);
    gtm_char_t *g_title = string_to_gtm_char_t(title);
    gtm_char_t *g_link = string_to_gtm_char_t(link);
    gtm_ulong_t g_pubDate = pubDate;
    gtm_char_t *g_creator = string_to_gtm_char_t(creator);
    gtm_char_t *g_publisher = string_to_gtm_char_t(publisher);
    gtm_char_t *g_publink = string_to_gtm_char_t(publink);

    if (0 != gtm_ci("setfields",g_guid,g_title,g_link,g_pubDate,g_creator,g_publisher,g_publink))
    {
        char buf[1024];
        gtm_zstatus(buf, 1023);
	cerr << "Callin for setfields failed: " << buf << endl;
    }
    free(g_guid);
    free(g_title);
    free(g_link);
    free(g_creator);
    free(g_publisher);
    free(g_publink);
}

bool Database::getfeed(string &startfrom)
{
    gtm_char_t g_feed[1025];
    strncpy(g_feed, startfrom.c_str(), 1024);
    if (0 != gtm_ci("getfeed",g_feed))
    {
        char buf[1024];
        gtm_zstatus(buf, 1023);
	cerr << "Callin for getfeed failed: " << buf << endl;
    }
    startfrom = g_feed;
    if (0 == startfrom.length())
	return 0;
    return 1;
}
