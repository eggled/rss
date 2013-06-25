#include "fetcher.hpp"
#include "xparser.hpp"
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    ifstream urls;
    urls.open("rss_urls.txt");

    string line;
    while (getline(urls, line))
    {
        Fetcher g(line);
        XParser(g.fetch()).dump();
    }
    return 0;
}
