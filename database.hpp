#ifndef __DATABASE_HPP_
#define __DATABASE_HPP_

#include <string>
extern "C" {
#include <gtmxc_types.h>
}


class Database
{
    
    public:
        Database();
        void markread(std::string id);
        int is_read(std::string id);
        std::string  newfname(std::string id);
        std::string getfname(std::string id);
        std::string getcontent(std::string id);
        void setfields(std::string guid, std::string title, std::string link, unsigned long pubDate, std::string description, std::string creator, std::string publisher, std::string publink);
        bool getmetadata(std::string &id, std::string &title, std::string &link, std::string &publink, std::string &publisher);
};

#endif
