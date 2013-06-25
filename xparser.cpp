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

void XParser::dump()
{
    cout << "<html><head><meta charset=\"utf-8\">" << endl;
    cout << "<style type=\"text/css\">" 
	<< "#itemwrapper { "
	<< "border-bottom: 1px solid #CCCCCC;  "
	<< "font-family: arial, sans-serif; font-size: 80%; }"
	<< "#item { padding: 6px; cursor: pointer; height: 27px; overflow: hidden; white-space: nowrap; "
	<< " line-height: 27px;  color: #999999 } " 
	<< "#item b { color: black; font-weight: bold; } "
    << "a.titleLink { color: #16f; font-size: 18px; font-weight: bold; text-decoration: none;} "
    << ".content { display: none; padding: 10px; height: auto; overflow: auto; } "
    << "#padding { height: 0; } "
    << ".authorInfo { color: #777777; margin-bottom: 10px; } "
    << ".navbar { position: absolute; border-right: solid 1px #CCCCCC; height: 100%; overflow: auto; width: 200px;  float: left; }"
    << ".wrapall { width: 100%; height: 100%} "
    << ".wrapcontent { padding-left: 204px;  position: relative;  width: auto; height: 100%; overflow: auto; }"
    << "body { overflow: hidden } "
	<< "</style>" << endl;
    cout << "<script type=\"text/javascript\">"
	<< "var shown; function showme(which) {"
    << "if (document.getElementById(which)) { "
	<< "if (shown) { shown.style.display = 'none'; if (shown.id == which) {  shown = null; return } }  shown = document.getElementById(which); "
    << "var data = shown.innerHTML.toString(); data =data.replace('<!--<<<<<', ''); data = data.replace('>>>>>-->', '');  shown.innerHTML = data;  "
    // Better to just know the ID of the parent, and use #item as a class, not an id...
	<< "shown.style.display = 'block';   shown.parentNode.scrollIntoView(1); var bar = shown.parentNode.firstChild; "
    << "while (bar && !( bar.id == 'item')) { bar = bar.nextSibling; } if (bar) { //alert('marking ' + shown.getAttribute('data-guid') + ' as read.'); "
    << endl << "bar.style.backgroundColor='#EEEEEE'; } } }" 
	<< "function mj() { if (shown) { showme('spec' + String(parseInt(shown.id.toString().substr(4))+1)) } } "
	<< "function mk() { if (shown) { showme('spec' + String(parseInt(shown.id.toString().substr(4))-1)) } } "
	<< " document.onkeypress = function(e) { if (e.charCode == 74 || e.charCode == 106) { mj(); } else if (e.charCode == 75 || e.charCode == 107) { mk(); }  }; "
	<< "</script>";
    cout << "</head><body>" << endl;
    //cout << this->title << endl;
    //cout << this->link << endl;
    //cout << this->description << endl;
    cout << "<div class=\"wrapall\">" << endl;
    cout << "<div class=\"navbar\">Navbar goes here.</div>" << endl
     << "<div class=\"wrapcontent\">";
    for (unsigned int i=0; i < this->items.size(); i++)
    {
        this->items[i].dump();
    }
cout << "<div id=\"padding\"></div></div>" << endl;
    cout << "</div>" << endl;
    cout << "</body>" << endl;
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
void subItem::dump()
{
	static int specid = 0;
specid++;
cout << "<div id=\"itemwrapper\">" << endl;
    cout << "<div id=\"item\" onclick=\"showme('spec" << specid << "');\">"
	<< "<b>" << this->title << "</b> - " << this->description 
    << "</div>" << endl;
    cout << "<div class=\"content\" id=\"spec" << specid << "\"  data-guid=\"" << this->guid << "\">" 
	<< "<a class=\"titleLink\" href=\"" << this->link << "\">" << this->title << "</a><br />"
    << "<span class=\"authorInfo\">from <a href=\"" << this->publink << "\">" << this->publisher << "</a> by " << this->creator << "</span><br />"
    << "<!--<<<<<"
    << this->content 
    << ">>>>>-->"
    << "</div>" << endl;
cout << "</div>" << endl;
}
