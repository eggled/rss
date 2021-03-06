
gtm_dist="/home/eggled/rss/gtm/gtmV60002"
INCLUDE=-I${gtm_dist}
CFLAGS=-Wall -g -rdynamic ${INCLUDE}
LDFLAGS=-lcurl -L${gtm_dist} -lgtmshr -lutil

default: all

fetcher.o: fetcher.cpp fetcher.hpp
	g++ ${CFLAGS} -c fetcher.cpp

main.o: main.cpp fetcher.hpp xparser.hpp display.hpp sock.hpp cgi.hpp debug.hpp
	g++ ${CFLAGS} -c main.cpp

pugixml.o: pugixml.cpp pugixml.hpp pugiconfig.hpp
	g++ ${CFLAGS} -c pugixml.cpp

xparser.o: xparser.cpp xparser.hpp pugixml.hpp
	g++ ${CFLAGS} -c xparser.cpp

display.o: display.cpp display.hpp xparser.hpp pugixml.hpp
	g++ ${CFLAGS} -c display.cpp

database.o: database.cpp database.hpp debug.hpp
	c++ ${CFLAGS} -DGTMDIST="\"${gtm_dist}\"" -c database.cpp

sock.o: sock.cpp sock.hpp 
	g++ ${CFLAGS} -c sock.cpp

cgi.o: cgi.cpp cgi.hpp 
	g++ ${CFLAGS} -c cgi.cpp

debug.o: debug.cpp debug.hpp
	g++ ${CFLAGS} -c debug.cpp

all: fetcher.o main.o pugixml.o xparser.o display.o sock.o cgi.o database.o debug.o
	g++ ${CFLAGS} fetcher.o main.o pugixml.o sock.o xparser.o cgi.o database.o display.o debug.o ${LDFLAGS} -o rssgen

clean:
	rm -f fetcher.o main.o pugixml.o xparser.o display.o sock.o cgi.o database.o rssgen 

.PHONY: default all clean
