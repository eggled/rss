#include <iostream>
#include <fstream>
#include "display.hpp"
#include "database.hpp"

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
    node.text().set(" ");

    node = document.child("html").append_child("body");
    node = node.append_child("div");
    node.append_attribute("class").set_value("wrapall");

    this->navbarnode = node.append_child("div");
    this->navbarnode.append_attribute("class").set_value("navbar");
    this->navbarnode.text().set(" ");

    this->bodynode = node.append_child("div");
    this->bodynode.append_attribute("class").set_value("wrapcontent");

}

void Display::update()
{
    Database g;
    string s_id = "", s_title, s_link, s_publink, s_publisher, s_creator;
    int spec = 0;
    while (g.getmetadata(s_id, s_title, s_link, s_publink, s_publisher, s_creator))
    {
        ostringstream idval, onclickfunc, creatorstring, outputdata;
        idval << "spec" << ++spec;
        onclickfunc << "showme('" << idval.str() << "')";
        pugi::xml_node item = this->bodynode.append_child("div");
        item.append_attribute("id").set_value("itemwrapper");
        pugi::xml_node node = item.append_child("div");
        node.append_attribute("id").set_value("item");
        node.append_attribute("onclick").set_value(onclickfunc.str().c_str());

        node.append_child("b").text().set(s_title.c_str());

        node = item.append_child("div");
        node.append_attribute("class").set_value("content");
        node.append_attribute("id").set_value(idval.str().c_str());
        node.append_attribute("data-guid").set_value(s_id.c_str());

        pugi::xml_node link = node.append_child("a");
        link.append_attribute("class").set_value("titleLink");
        link.append_attribute("href").set_value(s_link.c_str());
        link.text().set(s_title.c_str());

        node.append_child("br");

        pugi::xml_node span = node.append_child("span");
        span.append_attribute("class").set_value("authorInfo");
        span.append_child("span").text().set("from ");
        link = span.append_child("a");
        link.append_attribute("href").set_value(s_publink.c_str());
        link.text().set(s_publisher.c_str());
        if (s_creator.length())
        {
            creatorstring << " by " << s_creator;
            span.append_child("span").text().set(creatorstring.str().c_str());
        }
    }
    string feed_url = "";
    while (g.getfeed(feed_url))
    {
        this->navbarnode.append_child("p").text().set(feed_url.c_str());
    }
    pugi::xml_node input = this->navbarnode.append_child("input");
    input.append_attribute("id").set_value("addurl");
    input.append_attribute("type").set_value("text");
    input.text().set(" ");
}

void Display::printnavbar()
{
    this->navbarnode.print(cout, "\t", (pugi::format_default | pugi::format_no_declaration) & (~pugi::format_no_escapes));
}

void Display::printpage()
{
    this->document.save(cout, "\t", (pugi::format_default | pugi::format_no_declaration) & (~pugi::format_no_escapes));
}

