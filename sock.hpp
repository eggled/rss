#ifndef __SOCKET_HPP_
#define __SOCKET_HPP_

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>

class WebServer
{
    int sockfd, portno, clisockfd;
    struct sockaddr_in serv_addr;

    public:
    WebServer(int port);
    int do_accept(int do_fork);
    void do_shutdown();
};

#endif
