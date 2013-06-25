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
        this->add_item(item);
        item = item.next_sibling("item");
    }
}

void XParser::add_item(pugi::xml_node node)
{
    this->items.push_back(subItem(node));
}

void XParser::dump()
{
    cout << "<html><head><meta charset=\"utf-8\">" << endl;
    cout << "<style type=\"text/css\">" 
	<< "#itemwrapper { "
	<< "border-bottom: 1px solid #CCCCCC; margin-left: 1in; "
	<< "font-family: arial, sans-serif; font-size: 80%; }"
	<< "#item { height: 27px; overflow: hidden; white-space: nowrap; "
	<< " line-height: 27px;  } " 
	<< "#item a { color: black; font-weight: bold; text-decoration: none } "
	<< "#item span { display: inline; width: 2in; overflow: hidden; } "
	<< "</style>" << endl;
    cout << "<script type=\"text/javascript\">"
	<< "var shown; function showme(which) {"
	<< "if (shown) { shown.style.display = 'none'; } shown = document.getElementById(which); "
	<< "shown.style.display = 'block';  shown.parentNode.scrollIntoView(1); }" 
	<< "function mj() { if (shown) { showme('spec' + String(parseInt(shown.id.toString().substr(4))+1)) } } "
	<< "function mk() { if (shown) { showme('spec' + String(parseInt(shown.id.toString().substr(4))-1)) } } "
	<< " document.onkeypress = function(e) { if (e.charCode == 74 || e.charCode == 106) { mj(); } else if (e.charCode == 75 || e.charCode == 107) { mk(); }  }; "
	<< "</script>";
    cout << "</head><body>" << endl;
    cout << this->title << endl;
    cout << this->link << endl;
    cout << this->description << endl;
    for (int i=0; i < this->items.size(); i++)
    {
        this->items[i].dump();
    }
    cout << "</body>" << endl;
}


subItem::subItem(pugi::xml_node node)
{
    this->title = node.child_value("title");
    this->link = node.child_value("link");
    this->pubDate = node.child_value("pubDate");
    this->guid = node.child_value("guid");
    this->description = node.child_value("description");
    this->content = node.child_value("content:encoded");
}
void subItem::dump()
{
	static int specid = 0;
specid++;
cout << "<div id=\"itemwrapper\">" << endl;
    cout << "<div id=\"item\" onclick=\"showme('spec" << specid << "');\">"
	<< "<a href=\"" << this->link << "\">" << this->title << "</a> - " << this->description << "</div>" << endl;
    cout << "<div id=\"spec" << specid << "\" style=\"display: none\">" << this->content << "</div>" << endl;
cout << "</div>" << endl;
}
