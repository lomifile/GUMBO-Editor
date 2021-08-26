#include "log.h"

void Logger::create_log_file()
{
	FILE* fp = fopen(LOG_FILE, "w");
	char t_now[30];
	if (fp == NULL)return;

	time_t t = time(NULL);
	fprintf(fp, "%s --------> %s", INIT_MSG, ctime(&t));
	fclose(fp);
}

void Logger::append_log(char* date, char* input)
{
	strtok(date, "\n");
	FILE* fp = fopen(LOG_FILE, "a");
	if (fp == NULL) return;

	fprintf(fp, "%s --------> %s\n", date, input);
	fclose(fp);
}

bool Logger::check_log()
{
	if (access(LOG_FILE, F_OK) != -1)
		return true;
	else
		return false;
}

char* Logger::formated_string(char* format, ...)
{
	char* string = (char*)malloc(sizeof format);
	va_list args;
	va_start(args, format);
	vsnprintf(string, (uint32_t)strlen(format), format, args);
	va_end(args);
	return string;
}

char* Logger::time_now()
{
	time_t now = time(NULL);
	return ctime(&now);
}