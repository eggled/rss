
CFLAGS=-Wall -ggdb
LDFLAGS=-lcurl

default: all

fetcher.o: fetcher.cpp fetcher.hpp
	g++ ${CFLAGS} -c fetcher.cpp

main.o: main.cpp fetcher.hpp xparser.hpp display.hpp sock.hpp
	g++ ${CFLAGS} -c main.cpp

pugixml.o: pugixml.cpp pugixml.hpp pugiconfig.hpp
	g++ ${CFLAGS} -c pugixml.cpp

xparser.o: xparser.cpp xparser.hpp pugixml.hpp
	g++ ${CFLAGS} -c xparser.cpp

display.o: display.cpp display.hpp xparser.hpp pugixml.hpp
	g++ ${CFLAGS} -c display.cpp

sock.o: sock.cpp sock.hpp 
	g++ ${CFLAGS} -c sock.cpp

all: fetcher.o main.o pugixml.o xparser.o display.o sock.o
	g++ ${CFLAGS} ${LDFLAGS} fetcher.o main.o pugixml.o sock.o xparser.o display.o -o rssgen

clean:
	rm -f fetcher.o main.o pugixml.o xparser.o display.o sock.o rssgen 

.PHONY: default all clean
