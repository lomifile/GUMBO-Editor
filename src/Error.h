#ifndef GUMBO_SRC_ERROR_H
#define GUMBO_SRC_ERROR_H

#include <errno.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

namespace Error
{
	void die(const char* s);
}

#endif //GUMBO_SRC_ERROR_H
