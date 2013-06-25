

default: all

fetcher.o: fetcher.cpp fetcher.hpp
	g++ -Wall -c fetcher.cpp

main.o: main.cpp fetcher.hpp xparser.hpp
	g++ -Wall -c main.cpp

pugixml.o: pugixml.cpp pugixml.hpp pugiconfig.hpp
	g++ -Wall -c pugixml.cpp

xparser.o: xparser.cpp xparser.hpp pugixml.hpp
	g++ -Wall -c xparser.cpp

all: fetcher.o main.o pugixml.o xparser.o
	g++ fetcher.o main.o pugixml.o xparser.o -o rssgen
	./rssgen > tmp.html

clean:
	rm -f fetcher.o main.o pugixml.o xparser.o rssgen 

.PHONY: default all clean
