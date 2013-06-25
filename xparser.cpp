#include "xparser.hpp"
#include "pugixml.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <istream>

using namespace std;

XParser::XParser(string input)
{
    pugi::xml_document doc;
    //pugi::xml_parse_result result = doc.load(input.c_str());
    istringstream iss(input);
    pugi::xml_parse_result result = doc.load(iss, pugi::parse_default, pugi::encoding_utf8);
    pugi::xml_node channel = doc.child("rss").child("channel");

    this->title = string(channel.child_value("title"));
    this->link = string(channel.child_value("link"));
    this->description = string(channel.child_value("description"));

    pugi::xml_node item = channel.child("item");
    while (item)
    {
        this->add_item(item, this->title, this->link);
        item = item.next_sibling("item");
    }
}

void XParser::add_item(pugi::xml_node node, string publisher, string publink)
{
    this->items.push_back(subItem(node, publisher, publink));
}

vector <pugi::xml_node> XParser::generate()
{
    vector <pugi::xml_node> retval;
    for (unsigned int i=0; i < this->items.size(); i++)
    {
        retval.push_back(this->items[i].generate());
    }
    return retval;
}


subItem::subItem(pugi::xml_node node, string publisher, string publink)
{
    struct tm tm[1] = {{0}};
    strptime(node.child_value("pubDate"), "%a, %d %b %Y %H:%M:%S %z", tm);

    this->title = node.child_value("title");
    this->link = node.child_value("link");
    this->pubDate = tm->tm_gmtoff;
    this->guid = node.child_value("guid");
    this->description = node.child_value("description");
    this->content = node.child_value("content:encoded");
    this->creator = node.child_value("dc:creator");
    this->publisher = publisher;
    this->publink = publink;
}
pugi::xml_node subItem::generate()
{
    static int spec = 0;
    ostringstream idval, onclickfunc, creatorstring, outputdata;
    idval << "spec" << ++spec;
    onclickfunc << "showme('" << idval << "')";

    // FIXME: This is a mess... I'll have to return a full document, apparently.
    pugi::xml_node retval;
    retval.set_name("div");
    retval.append_attribute("id").set_value("itemwrapper");

    pugi::xml_node node = retval.append_child("div");
    node.append_attribute("id").set_value("item");
    node.append_attribute("onclick").set_value(onclickfunc.str().c_str());

    node.append_child("b").text().set(this->title.c_str());

    node = retval.append_child("div");
    node.append_attribute("class").set_value("content");
    node.append_attribute("id").set_value(idval.str().c_str());
    node.append_attribute("data-guid").set_value(this->guid.c_str());

    pugi::xml_node link = node.append_child("a");
    link.append_attribute("class").set_value("titleLink");
    link.append_attribute("href").set_value(this->link.c_str());
    link.text().set(this->title.c_str());

    node.append_child("br");

    pugi::xml_node span = node.append_child("span");
    span.append_attribute("class").set_value("authorInfo");
    span.append_child("span").text().set("from ");
    link = span.append_child("a");
    link.append_attribute("href").set_value(this->publink.c_str());
    link.text().set(this->publisher.c_str());
    creatorstring << " by " << this->publink;
    span.append_child("span").text().set(creatorstring.str().c_str());

    outputdata << "<!--<<<<<"
        << this->content 
        << ">>>>>-->";
    span.text().set(outputdata.str().c_str());
    return retval;
}
