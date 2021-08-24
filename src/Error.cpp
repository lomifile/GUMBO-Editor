#include "Error.h"

void Error::die(const char* s)
{
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
	perror(s);
	Logger::append_log(Logger::time_now(), (char*)s);
	exit(1);
}