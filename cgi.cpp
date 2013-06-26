#include "cgi.hpp"
#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;

CGI::CGI()
{
}

string urldecode(string val)
{
    string retval = "";
    while (val.length())
    {
        retval += val.substr(0,val.find('+'));
        if (val.find('+') < val.length())
        {
            retval += " ";
            val = val.substr(val.find('+')+1);
        }
        else
        {
            val = "";
        }
    }
    val = retval;
    retval = "";
    while(val.length())
    {
        retval += val.substr(0,val.find('%'));
        if (val.find('%') < val.length() - 2) // 2 accounts for the hex digits
        {
            val = val.substr(val.find('%')+1);
            char entity[2];
            entity[0] = strtoul(val.substr(0,2).c_str(), NULL, 16);
            entity[1] = '\0';
            retval += entity;
            val = val.substr(2);
        }
        else
        {
            val = "";
        }
    }
    return retval;
}



void CGI::process_request(string request)
{
    while (request.length())
    {
        string key = request.substr(0,request.find('='));
        string value = "";
        if (request.find('=') < request.length())
        {
            request = request.substr(request.find('=')+1);
            value = request.substr(0, request.find('&'));
            if (request.find('&') < request.length())
            {
                request = request.substr(request.find('&')+1);
            } else
            {
                request = "";
            }
        } else
        {
            request = "";
        }
        key = urldecode(key);
        value = urldecode(value);
        cerr << "Found " << key << "=>" << value << endl;
        this->data[key] = value;
    }
}

string CGI::get_value(string valname)
{
    return this->data[valname];
}


