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
    display.printpage();
    return 0;
}
