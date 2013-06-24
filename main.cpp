#include "fetcher.hpp"
#include "xparser.hpp"
#include <iostream>

using namespace std;

int main()
{
    Fetcher g("http://feeds.arstechnica.com/arstechnica/everything");

    XParser(g.fetch()).dump();
    return 0;
}
