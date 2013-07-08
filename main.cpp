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
#include "debug.hpp"

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
		debug_out("INFO: fetching database feeds");
	    int feed_pid = fork();
	    if (feed_pid == 0)
	    {
		    Database d;
		    string feed;
		    while (d.getfeed(feed))
		    {
			    Fetcher g(feed);
			    XParser(g.fetch());
		    }
		    return 0;
	    }
	    if (feed_pid < 0)
	    {
		    perror("Failed to fork fetch database feeds!");
		    return 1;
	    }
	    int status;
	    wait(&status);
            sleep(1);
        }
    }

    WebServer serv(8000);
    int childcount = 0;
    int spawn_children = 1;
    while (1)
    {
if (spawn_children == 0)
debug_out("INFO: child process now waiting for a new connection");
        if (serv.do_accept(spawn_children))
        {
		debug_out("INFO: Parent spawned a new pid");
            int status;
            if (childcount++ >= 5)
            {
		debug_out("INFO: 5 children, not spawning any more.");
                wait(&status);
		debug_out("INFO: child exited, ready to launch another.");
                childcount--;
            }
            continue;
        }
	debug_out("INFO: Child process, just accepted a connection.");
        spawn_children = 0;

        string line, request_string;
	Display display;

        
int lc= 0;
        while (getline(cin, line))
        {
lc++;
            line = line.substr(0, line.find('\r'));
        	if (line.length() == 0) break;
            if (line.compare(0,4,"GET ") == 0)
            {
                request_string = line.substr(4, line.find(" ", 4) - 4);
debug_out("INFO: GET request: " + request_string);
                if (request_string.compare("/") == 0)
                {
debug_out("INFO: Responding with full page");
                    cout << "HTTP/1.1 200 OK\r\n";
                    cout << "Content-type: text/html\r\n\r\n";
                    display.update();
                    display.printpage();
debug_out("INFO: Done with full page response");
                } else if (0 == request_string.compare(0,2,"/?")) // request
                {
debug_out("INFO: CGI request");
                    CGI nam;
                    nam.process_request(request_string.substr(2));
                    if (nam.get_value("content").length())
                    {
debug_out("INFO: item content request");
                        Database g;
                        cout << "HTTP/1.1 200 OK\r\n";
                        cout << "Content-type: text/html\r\n\r\n";
                        cout << g.getcontent(nam.get_value("content"));
                        g.markread(nam.get_value("content"));
debug_out("INFO: done");
                    } else if (nam.get_value("addurl").length())
                    {
debug_out("INFO: url add request");
                        Database g;
                        g.addurl(nam.get_value("addurl"));
                        kill(pid, SIGUSR1); // update feed list
                        cout << "HTTP/1.1 200 OK\r\n";
                        cout << "Content-type: text/html\r\n\r\n";
                        display.update();
                        display.printnavbar();
debug_out("INFO: done");
                    } else if (nam.get_value("refresh").length())
                    {
debug_out("INFO: refresh request");
                        kill(pid, SIGUSR1);
                        cout << "HTTP/1.1 200 OK\r\n";
                        cout << "Content-type: text/plain\r\n\r\n";
                        cout << ".\r\n";
debug_out("INFO: done");
                    } else
                    {
debug_out("INFO: Bad request");
                        cerr << "Unrecognized data in request_string: " << request_string << endl;
                    }
                } else
                {
debug_out("INFO: file request");
                    if (0 == request_string.compare("/tmp.js") || 0 == request_string.compare("/tmp.css"))
                    {
                        request_string = "html" + request_string;
debug_out("INFO: serving " + request_string);
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
debug_out("INFO: done");
                        }
                        else
                        {
                            cout << "HTTP/1.1 404 " << "Not found\r\n";
                            cout << "Content-type: text/plain\r\n\r\n";
                            cout << "404: File not found (" << request_string << ")" << endl;
debug_out("INFO: failed; sent 404");
                        }
                    }
                    else
                    {
debug_out("INFO: Bad request, sending 404");
                        cout << "HTTP/1.1 404 " << "Not found\r\n";
                        cout << "Content-type: text/plain\r\n\r\n";
                        cout << "404: File not found (" << request_string << ")" << endl;
debug_out("INFO: done");
                    }
                }
            }
        }
if (0 == lc)
debug_out("ERROR: no lines returned from getline!");
debug_out("INFO: All done,  shutting down socket.");
        serv.do_shutdown();
return 0;

    }
    return 0;
}
