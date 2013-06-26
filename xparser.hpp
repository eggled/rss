#ifndef __XPARSER_H_
#define __XPARSER_H_

#include <string>
#include <sstream>
#include <vector>
#include "pugixml.hpp"

class subItem
{
    private: 
        std::string title, link, guid, description, content, creator, publisher, publink;
        unsigned long pubDate; 
	pugi::xml_document* doc;
    public:
        subItem(pugi::xml_node node, std::string publisher, std::string publink);
        pugi::xml_node generate();
};

class XParser
{
    private:
        std::string title, link, description;
        std::vector <subItem> items;
        void add_item(pugi::xml_node node, std::string publisher, std::string publink);

    public:
        XParser(std::string input);

        std::vector <pugi::xml_node> generate();
};

#endif
