#ifndef __DISPLAY_HPP_
#define __DISPLAY_HPP_

#include <vector>
#include <fstream>
#include "xparser.hpp"
#include "pugixml.hpp"

class Display
{
    std::vector <XParser> members;
    pugi::xml_document document;
    pugi::xml_node bodynode;
    pugi::xml_node navbarnode;
    void generate();

    public:

    void add(XParser feed);
    void printpage();
    Display();
};

#endif