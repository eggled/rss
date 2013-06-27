#ifndef __DATABASE_HPP_
#define __DATABASE_HPP_

#include <string>
extern "C" {
#include <gtmxc_types.h>
}


class Database
{
    
    public:
        Database();
        void mark_read(std::string id);
        int is_read(std::string id);
};

#endif
