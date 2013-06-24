#ifndef __FETCHER_H_
#define __FETCHER_H_

#include <string>


class Fetcher
{
    std::string url, data;
    public:
        Fetcher(std::string new_url);
        std::string fetch();
        ~Fetcher();
};
#endif
