#ifndef GUMBO_SRC_ERROR_H
#define GUMBO_SRC_ERROR_H

#include <cerrno>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <lib/logger/log.h>

namespace Error
{
	void die(const char* s);
}

#endif //GUMBO_SRC_ERROR_H
