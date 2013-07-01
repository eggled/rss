#include "fetcher.hpp"
#include "xparser.hpp"
#include "display.hpp"
#include "sock.hpp"
#include "cgi.hpp"
#include "database.hpp"
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <gtmxc_types.h>

using namespace std;

void sigalrm(int p)
{
    return;
}

int main()
{

    int pid;
    if ((pid = fork())  < 0)
    {
        perror("Failed to fork");
        return 1;
    } else if (pid == 0) {
        while (1)
        {
            Database d;
            string feed;
            while (d.getfeed(feed))
            {
                Fetcher g(feed);
                XParser(g.fetch());
            }
            sigset_t sigs;
            sigemptyset(&sigs);
            sigaddset(&sigs, SIGUSR1);
            sigprocmask(SIG_UNBLOCK, &sigs, NULL);
            signal(SIGUSR1, sigalrm);
            sleep(30);
        }
    }

    WebServer serv(8000);
    int childcount = 0;
    int spawn_children = 1;
    while (1)
    {
        if (serv.do_accept(spawn_children))
        {
            int status;
            if (childcount++ >= 5)
            {
                wait(&status);
                childcount--;
            }
            continue;
        }
        spawn_children = 0;

        string line, request_string;
	Display display;

        
        while (getline(cin, line))
        {
            line = line.substr(0, line.find('\r'));
        	if (line.length() == 0) break;
            if (line.compare(0,4,"GET ") == 0)
            {
                request_string = line.substr(4, line.find(" ", 4) - 4);
                if (request_string.compare("/") == 0)
                {
                    cout << "HTTP/1.1 200 OK\r\n";
                    cout << "Content-type: text/html\r\n\r\n";
                    display.update();
                    display.printpage();
                } else if (0 == request_string.compare(0,2,"/?")) // request
                {
                    CGI nam;
                    nam.process_request(request_string.substr(2));
                    if (nam.get_value("content").length())
                    {
                        Database g;
                        cout << "HTTP/1.1 200 OK\r\n";
                        cout << "Content-type: text/html\r\n\r\n";
                        cout << g.getcontent(nam.get_value("content"));
                        g.markread(nam.get_value("content"));
                    } else if (nam.get_value("addurl").length())
                    {
                        Database g;
                        g.addurl(nam.get_value("addurl"));
                        kill(pid, SIGUSR1); // update feed list
                        cout << "HTTP/1.1 200 OK\r\n";
                        cout << "Content-type: text/html\r\n\r\n";
                        display.update();
                        display.printnavbar();
                    } else if (nam.get_value("refresh").length())
                    {
                        kill(pid, SIGUSR1);
                        cout << "HTTP/1.1 200 OK\r\n";
                        cout << "Content-type: text/plain\r\n\r\n";
                        cout << ".\r\n";
                    } else
                    {
                        cerr << "Unrecognized data in request_string: " << request_string << endl;
                    }
                } else
                {
                    if (0 == request_string.compare("/tmp.js") || 0 == request_string.compare("/tmp.css"))
                    {
                        request_string = "html" + request_string;
                        ifstream file_dump(request_string.c_str());
                        if (file_dump.is_open())
                        {
                            cout << "HTTP/1.1 200 OK\r\n";
                            if (request_string.compare("/tmp.css"))
                                cout << "Content-type: text/css\r\n\r\n";
                            else
                                cout << "Content-type: text/javascript\r\n\r\n";
                            while (getline(file_dump, line))
                            {
                                cout << line << endl;
                            }
                            file_dump.close();
                        }
                        else
                        {
                            cout << "HTTP/1.1 404 " << "Not found\r\n";
                            cout << "Content-type: text/plain\r\n\r\n";
                            cout << "404: File not found (" << request_string << ")" << endl;
                        }
                    }
                    else
                    {
                        cout << "HTTP/1.1 404 " << "Not found\r\n";
                        cout << "Content-type: text/plain\r\n\r\n";
                        cout << "404: File not found (" << request_string << ")" << endl;
                    }
                }
            }
        }
        serv.do_shutdown();

    }
    return 0;
}
