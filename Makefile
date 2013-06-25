
CFLAGS=-Wall
LDFLAGS=-lcurl

default: all

fetcher.o: fetcher.cpp fetcher.hpp
	g++ ${CFLAGS} -c fetcher.cpp

main.o: main.cpp fetcher.hpp xparser.hpp
	g++ ${CFLAGS} -c main.cpp

pugixml.o: pugixml.cpp pugixml.hpp pugiconfig.hpp
	g++ ${CFLAGS} -c pugixml.cpp

xparser.o: xparser.cpp xparser.hpp pugixml.hpp
	g++ ${CFLAGS} -c xparser.cpp

all: fetcher.o main.o pugixml.o xparser.o
	g++ ${CFLAGS} ${LDFLAGS} fetcher.o main.o pugixml.o xparser.o -o rssgen
	./rssgen > tmp.html

clean:
	rm -f fetcher.o main.o pugixml.o xparser.o rssgen 

.PHONY: default all clean
