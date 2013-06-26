#include "fetcher.hpp"
#include "xparser.hpp"
#include "display.hpp"
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    ifstream urls;
    urls.open("rss_urls.txt");

    string line;
    Display display;
    while (getline(urls, line))
    {
        Fetcher g(line);
        display.add(XParser(g.fetch()));
    }
while (getline(cin, line))
{
	if (line.length() == 0) break;
	if (line.length() == 1 and 13 == line.c_str()[0]) break;
	cerr << "Got line length " << line.length() << (int) line.c_str()[0] << line << endl;
}
cout << "HTTP/1.1 200 OK\r\n";
    cout << "Content-type: text/html\r\n\r\n";
    display.printpage();
    return 0;
}
