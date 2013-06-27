using namespace std;

#include "sock.hpp"
#include <ext/stdio_filebuf.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <fstream>
#include <string.h>
//#include <

WebServer::WebServer(int port)
{
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        throw errno;

    int on = 1;
    setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    memset(&this->serv_addr, 0, sizeof(this->serv_addr));
    this->portno = port;

    this->serv_addr.sin_family = AF_INET;
    this->serv_addr.sin_addr.s_addr = INADDR_ANY;
    this->serv_addr.sin_port = htons(this->portno);
    if (bind(this->sockfd, (struct sockaddr *) &this->serv_addr, sizeof(this->serv_addr)) < 0)
    {
        perror("bind");
    }
    listen(this->sockfd, 5);
}

int WebServer::do_accept(int do_fork)
{
    if (do_fork)
    {
        int pid = fork();
        if (pid < 0)
        {
            perror("fork");
            throw errno;
        }
        if (pid)
            return pid;
    }
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    this->clisockfd = accept(this->sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (this->clisockfd < 0)
    {
        perror("accept");
        throw errno;
    }
    dup2(this->clisockfd, STDOUT_FILENO);
    dup2(this->clisockfd, STDIN_FILENO);
    return 0;
}

void WebServer::do_shutdown()
{
    shutdown(this->clisockfd, SHUT_RDWR);
}

