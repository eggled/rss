#include <fcntl.h>
#include <cstdio>
#include "debug.hpp"
#include <string>
#include <sys/file.h>
#include <time.h>

using namespace std;

void debug_out(string msg)
{
	struct tm ts;
	time_t t;
	char buf[256];
	time(&t);
	localtime_r(&t, &ts);
	asctime_r(&ts, buf);
	FILE *f = fopen("debug.log","a");
	flock(fileno(f), LOCK_EX);
	fprintf(f, "%09d %s\t%s\n", getpid(), buf,  msg.c_str());
	fclose(f);
}
