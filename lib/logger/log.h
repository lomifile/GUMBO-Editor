#ifndef LOGGER_LIBRARY_H
#define LOGGER_LIBRARY_H

#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <cstring>
#include <ctime>
#include <malloc.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstdarg>
#include <cstdint>

#define LOG_FILE "gumboeditorlog.txt"
#define INIT_MSG "LOG FOR GUMBO-EDITOR CREATED"

namespace Logger
{
	void create_log_file();
	void append_log(char* date, char* input);
	bool check_log();
	char* time_now();
	char* formated_string(char* format, ...);
}

#endif //LOGGER_LIBRARY_H
