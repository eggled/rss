#ifndef __CGI_HPP_
#define __CGI_HPP_

#include <map>
#include <string>

class CGI
{
    std::map <std::string, std::string> data;
    public:
    CGI();
    void process_request(std::string request);
    std::string get_value(std::string valname);
};

#endif
