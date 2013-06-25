#include <iostream>
#include "display.hpp"

using namespace std;

Display::Display()
{
    //cout << "<!-- Have some fun... -->" << endl;
    this->document.load("");
    pugi::xml_node head = document.append_child("html").append_child("head");
    head.append_child("meta").append_attribute("charset").set_value("utf-8");

    pugi::xml_node node = head.append_child("link");
    node.append_attribute("rel").set_value("stylesheet");
    node.append_attribute("type").set_value("text/css");
    node.append_attribute("href").set_value("tmp.css");

    node = head.append_child("script");
    node.append_attribute("type").set_value("text/javascript");
    node.append_attribute("src").set_value("tmp.js");

    node = document.child("html").append_child("body");
    node = node.append_child("div");
    node.append_attribute("class").set_value("wrapall");

    this->navbarnode = node.append_child("div");
    this->navbarnode.append_attribute("class").set_value("navbar");

    this->bodynode = node.append_child("div");
    this->bodynode.append_attribute("class").set_value("wrapcontent");

}

void Display::add(XParser feed)
{
    this->members.push_back(feed);
}

void Display::generate()
{
    static int generated = 0;
    if (generated) return;
    generated = 1;
    for (unsigned int i = 0; i < this->members.size(); i++)
    {
        cout << "looking at member " << i << endl;
        vector <pugi::xml_node> nodelist = this->members[i].generate();
        for (unsigned int j = 0; j < nodelist.size(); j++)
        {
            cout << "\tlooking at member " << j << endl;
            this->bodynode.append_copy(nodelist[j]);
            cout << "\tappended " << nodelist[j].name();
        }
    }

}

void Display::printpage()
{
    this->generate();
    this->document.save(cout);
}

