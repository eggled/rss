#ifndef __XPARSER_H_
#define __XPARSER_H_

#include <string>
#include <sstream>
#include <vector>
#include "pugixml.hpp"

class subItem
{
    private: 
        std::string title, link, pubDate, guid, description, content;
    public:
        subItem(pugi::xml_node node);
        void dump();
};

class XParser
{
    private:
        std::string title, link, description;
        std::vector <subItem> items;
        void add_item(pugi::xml_node node);

    public:
        XParser(std::string input);

        void dump();
};

#endif
