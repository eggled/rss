
CFLAGS=-Wall
LDFLAGS=-lcurl

default: all

fetcher.o: fetcher.cpp fetcher.hpp
	g++ ${CFLAGS} -c fetcher.cpp

main.o: main.cpp fetcher.hpp xparser.hpp display.hpp
	g++ ${CFLAGS} -c main.cpp

pugixml.o: pugixml.cpp pugixml.hpp pugiconfig.hpp
	g++ ${CFLAGS} -c pugixml.cpp

xparser.o: xparser.cpp xparser.hpp pugixml.hpp
	g++ ${CFLAGS} -c xparser.cpp

display.o: display.cpp display.hpp xparser.hpp pugixml.hpp
	g++ ${CFLAGS} -c display.cpp

all: fetcher.o main.o pugixml.o xparser.o display.o 
	g++ ${CFLAGS} ${LDFLAGS} fetcher.o main.o pugixml.o xparser.o display.o -o rssgen
	./rssgen > tmp.html

clean:
	rm -f fetcher.o main.o pugixml.o xparser.o display.o rssgen 

.PHONY: default all clean
