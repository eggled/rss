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

    node = head.append_child("link");
    node.append_attribute("rel").set_value("stylesheet");
    node.append_attribute("type").set_value("text/css");
    node.append_attribute("href").set_value("http://code.jquery.com/mobile/1.2.1/jquery.mobile-1.2.1.min.css");

    node = head.append_child("script");
    node.append_attribute("type").set_value("text/javascript");
    node.append_attribute("src").set_value("http://code.jquery.com/jquery-1.8.3.min.js");
    node.text().set(" ");

    node = head.append_child("script");
    node.append_attribute("type").set_value("text/javascript");
    node.append_attribute("src").set_value("http://code.jquery.com/mobile/1.2.1/jquery.mobile-1.2.1.min.js");
    node.text().set(" ");
    
    node = head.append_child("script");
    node.append_attribute("type").set_value("text/javascript");
    node.append_attribute("src").set_value("tmp.js");
    node.text().set(" ");
    
    node = head.append_child("meta");
    node.append_attribute("name").set_value("viewport");
    node.append_attribute("content").set_value("width=device-width, initial-scale=1");

    this->bodynode = document.child("html").append_child("body");
    //node = node.append_child("div");
    //node.append_attribute("class").set_value("wrapall");

    //this->navbarnode = node.append_child("div");
    //this->navbarnode.append_attribute("class").set_value("navbar");
    //this->navbarnode.text().set(" ");
//
    //this->bodynode = node.append_child("div");
    //this->bodynode.append_attribute("class").set_value("wrapcontent");

}

void Display::update()
{
    Database g;
    string s_id = "", s_title, s_link, s_publink, s_publisher, s_creator;
    int spec = 0;
    int count = 0;
    unsigned long i_pubDate;
pugi::xml_node main_item = this->bodynode.append_child("div");
main_item.append_attribute("data-role").set_value("page");
    while (g.getmetadata(s_id, s_title, s_link, s_publink, s_publisher, s_creator, i_pubDate))
    {
	count++;
        ostringstream idval, onclickfunc, creatorstring, outputdata;
        idval << "spec" << ++spec;
        onclickfunc << "showme('" << idval.str() << "')";
        pugi::xml_node item = main_item.append_child("div");
        item.append_attribute("id").set_value("itemwrapper");
        item.append_attribute("data-role").set_value("content");
        pugi::xml_node node = item.append_child("div");
        node.append_attribute("id").set_value("item");
        node.append_attribute("onclick").set_value(onclickfunc.str().c_str());

        //pugi::xml_node artlink = node.append_child("a");
	//artlink.append_attribute("href").set_value(("#" + idval.str()).c_str());
	//artlink.append_child("b").text().set(s_title.c_str());
	node.append_child("b").text().set(s_title.c_str());

        node = this->bodynode.append_child("div");
	node.append_attribute("data-role").set_value("page");
        node.append_attribute("class").set_value("content");
        node.append_attribute("id").set_value(idval.str().c_str());

node = node.append_child("div");
node.append_attribute("data-role").set_value("content");

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
        if (i_pubDate > 0)
	{
		ostringstream dateInfo;
		string suffix;
		int offset = (time(NULL) - i_pubDate);
		if (offset < 0)
			offset = 0;
		offset /= 60;
		suffix = " minutes ago";
		if (offset > 60)
		{
			offset /= 60;
			suffix = " hours ago";
		}
		if (offset > 24)
		{
			offset /= 24;
			suffix = " days ago";
		}
		if (offset > 7)
		{
			offset /= 7;
			suffix = " weeks ago";
		}
		dateInfo << offset << suffix;
		span.append_child("span").text().set(dateInfo.str().c_str());
	}
	span.append_child("br");

node = node.append_child("div");
node.append_attribute("id").set_value(("content_"+idval.str()).c_str());
node.append_attribute("data-guid").set_value(s_id.c_str());
node.text().set(" ");
    }
    if (0 == count)
    {
	pugi::xml_node nothing = this->bodynode.append_child("div");
	nothing.append_attribute("style").set_value("color: #CCCCCC; position: relative; top: 50%; font-size: 18px; text-align: center;");
	nothing.text().set("Nothing to display");
    }

//    pugi::xml_node link = this->navbarnode.append_child("a");
//    link.append_attribute("style").set_value("cursor: pointer; color: blue;");
//    link.append_attribute("onclick").set_value("var tmp = function() { load('/?refresh=now', window.location.refresh); return 0; }; tmp();");
//    link.text().set("refresh");
//    this->navbarnode.append_child("br");
//    string feed_url = "";
//    while (g.getfeed(feed_url))
//    {
//        this->navbarnode.append_child("p").text().set(feed_url.c_str());
//    }
//    pugi::xml_node input = this->navbarnode.append_child("input");
//    input.append_attribute("id").set_value("addurl");
//    input.append_attribute("type").set_value("text");
//    input.text().set(" ");
}

void Display::printnavbar()
{
    this->navbarnode.print(cout, "\t", (pugi::format_default | pugi::format_no_declaration) & (~pugi::format_no_escapes));
}

void Display::printpage()
{
cout << "<!DOCTYPE html>" << endl;
    this->document.save(cout, "\t", (pugi::format_default | pugi::format_no_declaration) & (~pugi::format_no_escapes));
}

